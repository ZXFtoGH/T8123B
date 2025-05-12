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

#include "typedefs.h"
#include "platform.h"
#include "download.h"
#include "meta.h"
#include "sec.h"
#include "partition.h"
#include "dram_buffer.h"
#include "wdt.h"
#include "timer.h"
//#include "preloaderATE.h"
#include "mt_uart.h"
#include <regulator/mtk_regulator.h>

#define		thermal_dbg_print(fmt, args...)		//xxxtemp, print0m(fmt, ##args)   //xxx, dbg_print(fmt, ##args)


#define MTK_PMIC_MT6359

//=============================================================
// $$$SLT \common\inc\MT6885\reg_base.H
//=============================================================
// APB Module therm_ctrl
#define THERM_CTRL_BASE (0x1100B000)

//=============================================================
// $$$SLT thermal_settings.h
//=============================================================
#define THERMAL_HW_PROTECT (0)
#define THERMAL_HARDWARE_RESET_POINT (117000)
#define LVTS_DEVICE_AUTO_RCK (0)
#define DUMP_LVTS_REGISTER (0)

#define LVTS_COEFF_A_X_1000			(-252500) // -252.5
#define LVTS_COEFF_B_X_1000			 (252500) //  252.5

//=============================================================
// $$$SLT thermal_internal.h
//=============================================================
/*=============================================================
 * Structure Definition
 *=============================================================
 */
struct register_data {
	const unsigned int addr;
	const unsigned int msb;
	const unsigned int lsb;
};

/* private thermal sensor enum */
enum lvts_sensor_enum {
	L_TS_LVTS1_0 = 0,	/* LVTS1-0 1*/
	L_TS_LVTS1_1,		/* LVTS1-1 2*/
	L_TS_LVTS2_0,		/* LVTS2-0 3*/
	L_TS_LVTS2_1,		/* LVTS2-1 4*/
	L_TS_LVTS3_0,		/* LVTS3-0 5*/
	L_TS_LVTS3_1,		/* LVTS3-1 6*/
	L_TS_LVTS3_2,		/* LVTS3-2 7*/
	L_TS_LVTS3_3,		/* LVTS3-3 8*/
	L_TS_LVTS4_0,		/* LVTS4-0 9*/
	L_TS_LVTS4_1,		/* LVTS4-1 10*/
	L_TS_LVTS5_0,		/* LVTS5-0 11*/
	L_TS_LVTS5_1,		/* LVTS5-1 12*/
	L_TS_LVTS6_0,		/* LVTS6-0 13*/
	L_TS_LVTS6_1,		/* LVTS6-1 14*/
	L_TS_LVTS7_0,		/* LVTS7-0 15*/
	L_TS_LVTS7_1,		/* LVTS7-1 16*/
	L_TS_LVTS7_2,		/* LVTS7-2 17*/
	L_TS_LVTS_NUM
};

enum lvts_tc_enum {
	LVTS_MCU_CONTROLLER0= 0,/* LVTSMONCTL0 */
	LVTS_MCU_CONTROLLER1,	/* LVTSMONCTL0_1 */
	LVTS_MCU_CONTROLLER2,	/* LVTSMONCTL0_2 */
	LVTS_AP_CONTROLLER0,	/* LVTSMONCTL0 */
	LVTS_AP_CONTROLLER1,	/* LVTSMONCTL0_1 */
	LVTS_AP_CONTROLLER2,	/* LVTSMONCTL0_2 */
	LVTS_AP_CONTROLLER3,	/* LVTSMONCTL0_3 */
	LVTS_CONTROLLER_NUM
};

struct lvts_thermal_controller_speed {
	unsigned int group_interval_delay;
	unsigned int period_unit;
	unsigned int filter_interval_delay;
	unsigned int sensor_interval_delay;
};

struct lvts_thermal_controller {
	enum lvts_sensor_enum ts[4]; /* sensor point 0 ~ 3 */
	int ts_number;
	int dominator_ts_idx; /* hw protection ref TS (index of the ts array) */
	int tc_offset;
	struct lvts_thermal_controller_speed tc_speed;
};

/*=============================================================
 * Macro
 *=============================================================
 */
#define PRINT_LVTS_DEVICE_WRITE_LOG (0)
#define PRINT_LVTS_CONTROLLER_WRITE_LOG (0)

#define DRV_WriteReg16(addr,data)   ((*(volatile kal_uint16 *)(addr)) = (kal_uint16)data)
#define DRV_Reg16(addr)             (*(volatile kal_uint16 *)(addr))

#define DRV_SetReg16(addr, data)    ((*(volatile kal_uint16 *)(addr)) |= (kal_uint16)data)
#define DRV_ClrReg16(addr, data)    ((*(volatile kal_uint16 *)(addr)) &= ~((kal_uint16)data))

#define _BIT_(_bit_)		(unsigned)(1 << (_bit_))
#define _BITMASK_(_bits_)	(((unsigned) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1))
#define ARRAY_SIZE(x)   sizeof(x)/sizeof(x[0])

#define MIN(_a_, _b_) ((_a_) > (_b_) ? (_b_) : (_a_))
#define MAX(_a_, _b_) ((_a_) > (_b_) ? (_a_) : (_b_))

#define THERMAL_WRAP_WR32(val,addr)	mt_reg_sync_writel_print((val), ((void *)addr))

#define thermal_readl(addr)         DRV_Reg32(addr)
#define thermal_writel(addr, val)   mt65xx_reg_sync_writel((val), ((void *)addr))
#define thermal_setl(addr, val)     mt65xx_reg_sync_writel(thermal_readl(addr) | (val), ((void *)addr))
#define thermal_clrl(addr, val)     mt65xx_reg_sync_writel(thermal_readl(addr) & ~(val), ((void *)addr))
/*=============================================================
 * extern value
 *=============================================================
 */
extern struct lvts_thermal_controller lvts_tscpu_g_tc[LVTS_CONTROLLER_NUM];
/*=============================================================
 * extern function
 *=============================================================
 */
extern int lvts_write_device(unsigned int config, unsigned int dev_reg_idx, unsigned int data, int tc_num);
extern unsigned int lvts_read_device(unsigned int dev_reg_idx, int tc_num);
extern void lvts_tscpu_reset_thermal(void);
extern void mt_reg_sync_writel_print(unsigned int val, void* addr);
extern unsigned int lvts_temp_to_raw(int temp, enum lvts_sensor_enum ts_name);
extern int lvts_raw_to_temp(unsigned int msr_raw, enum lvts_sensor_enum ts_name);
/*=============================================================
 * Peripheral Configuration Register Definition
 *=============================================================
 */
#define PERI_GLOBALCON_RST0 (PERICFG_BASE + 0x000)
// APB Module infracfg_ao
//#define INFRACFG_AO_BASE (0xF0001000)
#define INFRA_GLOBALCON_RST_0_SET (INFRACFG_AO_BASE + 0x120) //0x10001000
#define INFRA_GLOBALCON_RST_0_CLR (INFRACFG_AO_BASE + 0x124) //0x10001000
#define INFRA_GLOBALCON_RST_0_STA (INFRACFG_AO_BASE + 0x128) //0x10001000


#define INFRA_GLOBALCON_RST_4_SET (INFRACFG_AO_BASE + 0x730)
#define INFRA_GLOBALCON_RST_4_CLR (INFRACFG_AO_BASE + 0x734)
/*=============================================================
 * Clock Gate Definition
 *=============================================================
 */
/*
 * CLK_INFRA_THERM_BCLK at bit 6
 * STA => Check the clock status. 0: clock on, 1: clock off
 * SET => Write 1 to disable the clock (enable clock gating)
 * CLR => Write 1 to enable the clock
 */
#define THERM_MODULE_SW_CG_0_STA (INFRACFG_AO_BASE + 0x0080)
#define THERM_MODULE_SW_CG_0_SET (INFRACFG_AO_BASE + 0x0084)
#define THERM_MODULE_SW_CG_0_CLR (INFRACFG_AO_BASE + 0x0090)


//APMIXED_BASE related with row data convert document( CY Chien)
#define TS_CON0             (APMIXED_BASE + 0x600)
#define TS_CON1             (APMIXED_BASE + 0x604)

/*******************************************************************************
 * Thermal Controller Register Mask Definition
 *****************************************************************************
 */
#define THERMAL_ENABLE_SEN0     0x1
#define THERMAL_ENABLE_SEN1     0x2
#define THERMAL_ENABLE_SEN2     0x4
#define THERMAL_MONCTL0_MASK    0x00000007

#define THERMAL_PUNT_MASK       0x00000FFF
#define THERMAL_FSINTVL_MASK    0x03FF0000
#define THERMAL_SPINTVL_MASK    0x000003FF
#define THERMAL_MON_INT_MASK    0x0007FFFF

#define THERMAL_MON_CINTSTS0    0x00000001
#define THERMAL_MON_HINTSTS0    0x00000002
#define THERMAL_MON_LOINTSTS0   0x00000004
#define THERMAL_MON_HOINTSTS0   0x00000008
#define THERMAL_MON_NHINTSTS0   0x00000010
#define THERMAL_MON_CINTSTS1    0x00000020
#define THERMAL_MON_HINTSTS1    0x00000040
#define THERMAL_MON_LOINTSTS1   0x00000080
#define THERMAL_MON_HOINTSTS1   0x00000100
#define THERMAL_MON_NHINTSTS1   0x00000200
#define THERMAL_MON_CINTSTS2    0x00000400
#define THERMAL_MON_HINTSTS2    0x00000800
#define THERMAL_MON_LOINTSTS2   0x00001000
#define THERMAL_MON_HOINTSTS2   0x00002000
#define THERMAL_MON_NHINTSTS2   0x00004000
#define THERMAL_MON_TOINTSTS    0x00008000
#define THERMAL_MON_IMMDINTSTS0 0x00010000
#define THERMAL_MON_IMMDINTSTS1 0x00020000
#define THERMAL_MON_IMMDINTSTS2 0x00040000
#define THERMAL_MON_FILTINTSTS0 0x00080000
#define THERMAL_MON_FILTINTSTS1 0x00100000
#define THERMAL_MON_FILTINTSTS2 0x00200000

#define THERMAL_MON_CINTSTS3    0x00400000
#define THERMAL_MON_HINTSTS3    0x00800000
#define THERMAL_MON_LOINTSTS3   0x01000000
#define THERMAL_MON_HOINTSTS3   0x02000000
#define THERMAL_MON_NHINTSTS3   0x04000000

#define THERMAL_MON_IMMDINTSTS3 0x08000000
#define THERMAL_MON_FILTINTSTS3 0x10000000

#define THERMAL_tri_SPM_State0  0x20000000
#define THERMAL_tri_SPM_State1  0x40000000
#define THERMAL_tri_SPM_State2  0x80000000

#define THERMAL_MSRCTL0_MASK    0x00000007
#define THERMAL_MSRCTL1_MASK    0x00000038
#define THERMAL_MSRCTL2_MASK    0x000001C0

/*=============================================================
 * LVTS Thermal Controller Register Definition
 *=============================================================
 */
#define LVTSMONCTL0_0 (THERM_CTRL_BASE + 0x000)
#define LVTSMONCTL1_0 (THERM_CTRL_BASE + 0x004)
#define LVTSMONCTL2_0 (THERM_CTRL_BASE + 0x008)
#define LVTSMONINT_0 (THERM_CTRL_BASE + 0x00C)
#define LVTSMONINTSTS_0 (THERM_CTRL_BASE + 0x010)
#define LVTSMONIDET0_0 (THERM_CTRL_BASE + 0x014)
#define LVTSMONIDET1_0 (THERM_CTRL_BASE + 0x018)
#define LVTSMONIDET2_0 (THERM_CTRL_BASE + 0x01C)
#define LVTSMONIDET3_0 (THERM_CTRL_BASE + 0x020)
#define LVTSH2NTHRE_0 (THERM_CTRL_BASE + 0x024)
#define LVTSHTHRE_0 (THERM_CTRL_BASE + 0x028)
#define LVTSCTHRE_0 (THERM_CTRL_BASE + 0x02C)
#define LVTSOFFSETH_0 (THERM_CTRL_BASE + 0x030)
#define LVTSOFFSETL_0 (THERM_CTRL_BASE + 0x034)
#define LVTSMSRCTL0_0 (THERM_CTRL_BASE + 0x038)
#define LVTSMSRCTL1_0 (THERM_CTRL_BASE + 0x03C)
#define LVTSTSSEL_0 (THERM_CTRL_BASE + 0x040)
#define LVTSDEVICETO_0 (THERM_CTRL_BASE + 0x044)
#define LVTSCALSCALE_0 (THERM_CTRL_BASE + 0x048)
#define LVTS_ID_0 (THERM_CTRL_BASE + 0x04C)
#define LVTS_CONFIG_0 (THERM_CTRL_BASE + 0x050)
#define LVTSEDATA00_0 (THERM_CTRL_BASE + 0x054)
#define LVTSEDATA01_0 (THERM_CTRL_BASE + 0x058)
#define LVTSEDATA02_0 (THERM_CTRL_BASE + 0x05C)
#define LVTSEDATA03_0 (THERM_CTRL_BASE + 0x060)
#define LVTSMSR0_0 (THERM_CTRL_BASE + 0x090)
#define LVTSMSR1_0 (THERM_CTRL_BASE + 0x094)
#define LVTSMSR2_0 (THERM_CTRL_BASE + 0x098)
#define LVTSMSR3_0 (THERM_CTRL_BASE + 0x09C)
#define LVTSIMMD0_0 (THERM_CTRL_BASE + 0x0A0)
#define LVTSIMMD1_0 (THERM_CTRL_BASE + 0x0A4)
#define LVTSIMMD2_0 (THERM_CTRL_BASE + 0x0A8)
#define LVTSIMMD3_0 (THERM_CTRL_BASE + 0x0AC)
#define LVTSRDATA0_0 (THERM_CTRL_BASE + 0x0B0)
#define LVTSRDATA1_0 (THERM_CTRL_BASE + 0x0B4)
#define LVTSRDATA2_0 (THERM_CTRL_BASE + 0x0B8)
#define LVTSRDATA3_0 (THERM_CTRL_BASE + 0x0BC)
#define LVTSPROTCTL_0 (THERM_CTRL_BASE + 0x0C0)
#define LVTSPROTTA_0 (THERM_CTRL_BASE + 0x0C4)
#define LVTSPROTTB_0 (THERM_CTRL_BASE + 0x0C8)
#define LVTSPROTTC_0 (THERM_CTRL_BASE + 0x0CC)
#define LVTSCLKEN_0 (THERM_CTRL_BASE + 0x0E4)
#define LVTSDBGSEL_0 (THERM_CTRL_BASE + 0x0E8)
#define LVTSDBGSIG_0 (THERM_CTRL_BASE + 0x0EC)
#define LVTSSPARE0_0 (THERM_CTRL_BASE + 0x0F0)
#define LVTSSPARE1_0 (THERM_CTRL_BASE + 0x0F4)
#define LVTSSPARE2_0 (THERM_CTRL_BASE + 0x0F8)
#define LVTSSPARE3_0 (THERM_CTRL_BASE + 0x0FC)
#define THERMINTST (THERM_CTRL_BASE + 0xF04)


/*============================================================================*/
/* CONSTAND DEFINITIONS                                                       */
/*============================================================================*/

/*=============================================================
 * AUXADC Register Definition
 *=============================================================
 */

/*=============================================================
 * Global variable definition
 *=============================================================
 */
int tscpu_ts_lvts_temp[L_TS_LVTS_NUM];
int tscpu_ts_lvts_temp_r[L_TS_LVTS_NUM];
int tscpu_ts_lvts_temp_0p7[L_TS_LVTS_NUM];
int tscpu_ts_lvts_temp_r_0p7[L_TS_LVTS_NUM];
int tscpu_ts_lvts_temp_0p65[L_TS_LVTS_NUM];
int tscpu_ts_lvts_temp_r_0p65[L_TS_LVTS_NUM];
int tscpu_ts_lvts_temp_0p575[L_TS_LVTS_NUM];
int tscpu_ts_lvts_temp_r_0p575[L_TS_LVTS_NUM];
/* chip dependent */
#if DUMP_LVTS_REGISTER
#define NUM_LVTS_DEVICE_REG (5)
static const unsigned int g_lvts_device_addrs[NUM_LVTS_DEVICE_REG] = {
        0x00,
        0x04,
        0x08,
        0x0C,
        0xF0};
#endif
/*
 * module			LVTS Plan
 *=====================================================
 * MCU_BIG(T1,T2) 		LVTS1-0, LVTS1-1
 * MCU_BIG(T3,T4)		LVTS2-0, LVTS2-1
 * MCU_LITTLE(T5,T6,T7,T8)	LVTS3-0, LVTS3-1, LVTS3-2, LVTS3-3
 * VPU_MLDA(T9,T10)		LVTS4-0, LVTS4-1
 * GPU(T11,T12)			LVTS5-0, LVTS5-1
 * INFA(T13)			LVTS6-0
 * CAMSYS(T18) 			LVTS6-1
 * MDSYS(T14,T15,T20)		LVTS7-0, LVTS7-1, LVTS7-2
 */
/*ptp_therm_ctrl_AP  Base address: (+0x1100_B000), 0x1100B100, 0x1100B200, 0x1100B300*/
/*ptp_therm_ctrl_MCU Base address: (+0x1127_8000), 0x11278100, 0x11278200 */
struct lvts_thermal_controller lvts_tscpu_g_tc[LVTS_CONTROLLER_NUM] = {
	[0] = {/*(MCU)*/
		.ts = {L_TS_LVTS1_0, L_TS_LVTS1_1},
		.ts_number = 2,
		.dominator_ts_idx = 1, //hw protection ref TS (idx of .ts arr)
		.tc_offset = 0x26D000,
		.tc_speed = {
			.group_interval_delay = 0x001,
			.period_unit = 0x00C,
			.filter_interval_delay = 0x001,
			.sensor_interval_delay = 0x001
		}
	},
	[1] = {/*(MCU)*/
		.ts = {L_TS_LVTS2_0, L_TS_LVTS2_1},
		.ts_number = 2,
		.dominator_ts_idx = 1, //hw protection ref TS (idx of .ts arr)
		.tc_offset = 0x26D100,
		.tc_speed = {
			.group_interval_delay = 0x001,
			.period_unit = 0x00C,
			.filter_interval_delay = 0x001,
			.sensor_interval_delay = 0x001
		}
	},
	[2] = {/*(MCU)*/
		.ts = {L_TS_LVTS3_0, L_TS_LVTS3_1, L_TS_LVTS3_2, L_TS_LVTS3_3},
		.ts_number = 4,
		.dominator_ts_idx = 1, //hw protection ref TS (idx of .ts arr)
		.tc_offset = 0x26D200,
		.tc_speed = {
			.group_interval_delay = 0x001,
			.period_unit = 0x00C,
			.filter_interval_delay = 0x001,
			.sensor_interval_delay = 0x001
		}
	},
	[3] = {/*(AP)*/
		.ts = {L_TS_LVTS4_0, L_TS_LVTS4_1},
		.ts_number = 2,
		.dominator_ts_idx = 1, //hw protection ref TS (idx of .ts arr)
		.tc_offset = 0x0,
		.tc_speed = {
			.group_interval_delay = 0x001,
			.period_unit = 0x00C,
			.filter_interval_delay = 0x001,
			.sensor_interval_delay = 0x001
		}
	},
	[4] = {/*(AP)*/
		.ts = {L_TS_LVTS5_0, L_TS_LVTS5_1},
		.ts_number = 2,
		.dominator_ts_idx = 0, //hw protection ref TS (idx of .ts arr)
		.tc_offset = 0x100,
		.tc_speed = {
			.group_interval_delay = 0x001,
			.period_unit = 0x00C,
			.filter_interval_delay = 0x001,
			.sensor_interval_delay = 0x001
		}
	},
	[5] = {/*(AP)*/
		.ts = {L_TS_LVTS6_0, L_TS_LVTS6_1},
		.ts_number = 2,
		.dominator_ts_idx = 1, //hw protection ref TS (idx of .ts arr)
		.tc_offset = 0x200,
		.tc_speed = {
			.group_interval_delay = 0x001,
			.period_unit = 0x00C,
			.filter_interval_delay = 0x001,
			.sensor_interval_delay = 0x001
		}
	},
	[6] = {/*(AP)*/
		.ts = {L_TS_LVTS7_0, L_TS_LVTS7_1, L_TS_LVTS7_2},
		.ts_number = 3,
		.dominator_ts_idx = 1, //hw protection ref TS (idx of .ts arr)
		.tc_offset = 0x300,
		.tc_speed = {
			.group_interval_delay = 0x001,
			.period_unit = 0x00C,
			.filter_interval_delay = 0x001,
			.sensor_interval_delay = 0x001
		}
	}
};

/*=============================================================
 * Local variable definition
 *=============================================================
 */
static int lvts_debug_log;
static unsigned int g_golden_temp;
static unsigned int g_count_r[L_TS_LVTS_NUM];
static unsigned int g_count_rc[L_TS_LVTS_NUM];
static unsigned int g_count_rc_now[L_TS_LVTS_NUM];
static int g_use_fake_efuse;

/*=============================================================
 * Local function declartation
 *=============================================================
 */
static void lvts_efuse_setting(void)
{
	unsigned int offset, i;
	int j, s_index;
	int efuse_data;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		for (j = 0; j < lvts_tscpu_g_tc[i].ts_number; j++) {

			offset = lvts_tscpu_g_tc[i].tc_offset;
			s_index = lvts_tscpu_g_tc[i].ts[j];

#if LVTS_DEVICE_AUTO_RCK == 0
			efuse_data = (((unsigned long long int)g_count_rc_now[s_index]) *
				g_count_r[s_index]) >> 14;
#else
			efuse_data = g_count_r[s_index];
#endif

			switch (j) {
			case 0:
				mt_reg_sync_writel_print(efuse_data,
					(void *)(LVTSEDATA00_0 + offset));
				thermal_dbg_print("efuse LVTSEDATA00_%d 0x%x\n",
					i, DRV_Reg32(LVTSEDATA00_0 + offset));
				break;

			case 1:
				mt_reg_sync_writel_print(efuse_data,
					(void *)(LVTSEDATA01_0 + offset));
				thermal_dbg_print("efuse LVTSEDATA01_%d 0x%x\n",
					i, DRV_Reg32(LVTSEDATA01_0 + offset));
				break;
			case 2:
				mt_reg_sync_writel_print(efuse_data,
					(void *)(LVTSEDATA02_0 + offset));
				thermal_dbg_print("efuse LVTSEDATA02_%d 0x%x\n",
					i, DRV_Reg32(LVTSEDATA02_0 + offset));
				break;
			case 3:
				mt_reg_sync_writel_print(efuse_data,
					(void *)(LVTSEDATA03_0 + offset));
				thermal_dbg_print("efuse LVTSEDATA03_%d 0x%x\n",
					i, DRV_Reg32(LVTSEDATA03_0 + offset));
				break;
			default:
				thermal_dbg_print("%s, illegal ts order : %d!!\n",
								__func__, j);
			}
		}
	}

}

static void lvts_device_identification(void)
{
	unsigned int tc_num, offset, data;

	thermal_dbg_print("===== %s begin ======\n", __func__);
	for (tc_num = 0; tc_num < ARRAY_SIZE(lvts_tscpu_g_tc); tc_num++) {

		offset = lvts_tscpu_g_tc[tc_num].tc_offset; //tc offset

		// Enable LVTS_CTRL Clock
		mt_reg_sync_writel_print(0x00000001,(void *)(LVTSCLKEN_0 + offset));

		// Reset All Devices
		lvts_write_device(0x81030000, 0xFF, 0xFF, tc_num);

		//  Read back Dev_ID with Update
		lvts_write_device(0x85020000, 0xFC, 0x55, tc_num);

		// Check LVTS device ID
		data = (DRV_Reg32(LVTS_ID_0 + offset) & _BITMASK_(7:0));
		if (data != (0x81 + tc_num))
			thermal_dbg_print("LVTS_TC_%d, Device ID should be 0x%x, but 0x%x\n",
				tc_num, (0x81 + tc_num), data);
	}

}

static void lvts_Device_Enable_Init_all_Devices(void)
{
	unsigned int i;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		/* Release Counting StateMachine */
		lvts_write_device(0x81030000, 0x03, 0x00, i);
		/* Set LVTS device counting window 20us */
		lvts_write_device(0x81030000, 0x04, 0x20, i);
		lvts_write_device(0x81030000, 0x05, 0x00, i);
		/* TSV2F_CHOP_CKSEL & TSV2F_EN */
		lvts_write_device(0x81030000, 0x0A, 0xC4, i);
		/* TSBG_DEM_CKSEL * TSBG_CHOP_EN */
		lvts_write_device(0x81030000, 0x0C, 0x7C, i);
		/* Set TS_RSV */
		lvts_write_device(0x81030000, 0x09, 0x8D, i);

#if LVTS_DEVICE_AUTO_RCK == 0
		/* Device low power mode can ignore these settings and
		 * Device auto RCK mode will force device in low power
		 * mode
		 */
#if 1
		/* Enable TS_EN */
		lvts_write_device(0x81030000, 0x08, 0xF5, i);
		/* Toggle TSDIV_EN & TSVCO_TG */
		lvts_write_device(0x81030000, 0x08, 0xFC, i);
		/* Enable TS_EN */
		lvts_write_device(0x81030000, 0x08, 0xF5, i);

		/*  Lantency */
		lvts_write_device(0x81030000, 0x07, 0xA6, i);
#else
		/* Enable TS_EN */
		lvts_write_device(0x81030000, 0x08, 0xF7, i);
		/* Toggle TSDIV_EN & TSVCO_TG */
		lvts_write_device(0x81030000, 0x08, 0xFE, i);
		/* Toggle TSDIV_EN & TSVCO_TG */
		lvts_write_device(0x81030000, 0x08, 0xF7, i);
#endif
#endif
	}
}

static void Enable_LVTS_CTRL_for_thermal_Data_Fetch(void)
{
	unsigned int tc_num, offset;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	for (tc_num = 0; tc_num < ARRAY_SIZE(lvts_tscpu_g_tc); tc_num++) {

		offset = lvts_tscpu_g_tc[tc_num].tc_offset; //tc offset

		// set sensor index of LVTS
		mt_reg_sync_writel_print(0x13121110, (void *)(LVTSTSSEL_0 + offset));
		// set sensor average scheme
		mt_reg_sync_writel_print(0x00000053, (void *)(LVTSMSRCTL0_0 + offset));
		// set calculation scale rules
		mt_reg_sync_writel_print(0x00000300, (void *)(LVTSCALSCALE_0 + offset));
		// set polling unit
		mt_reg_sync_writel_print(0x000003FF, (void *)(LVTSMONCTL1_0 + offset));
		// set polling interval
		mt_reg_sync_writel_print(0x000003FF, (void *)(LVTSMONCTL2_0 + offset));
	}

}

static void lvts_thermal_cal_prepare(void)
{
	unsigned int temp[22];
	char addr[22][10] = {
		"0x01C0",
		"0x01C4",
		"0x01C8",
		"0x01CC",
		"0x01D0",
		"0x01D4",
		"0x01D8",
		"0x01DC",
		"0x01E0",
		"0x01E4",
		"0x01E8",
		"0x01EC",
		"0x01F0",
		"0x01F4",
		"0x01F8",
		"0x01FC",
		"0x0200",
		"0x0204",
		"0x0208",
		"0x020C",
		"0x0210",
		"0x0214"
		};
	int i;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	temp[0] =  DRV_Reg32(0x11C101C0);
	temp[1] =  DRV_Reg32(0x11C101C4);
	temp[2] =  DRV_Reg32(0x11C101C8);
	temp[3] =  DRV_Reg32(0x11C101CC);
	temp[4] =  DRV_Reg32(0x11C101D0);
	temp[5] =  DRV_Reg32(0x11C101D4);
	temp[6] =  DRV_Reg32(0x11C101D8);
	temp[7] =  DRV_Reg32(0x11C101DC);
	temp[8] =  DRV_Reg32(0x11C101E0);
	temp[9] =  DRV_Reg32(0x11C101E4);
	temp[10] = DRV_Reg32(0x11C101E8);
	temp[11] = DRV_Reg32(0x11C101EC);
	temp[12] = DRV_Reg32(0x11C101F0);
	temp[13] = DRV_Reg32(0x11C101F4);
	temp[14] = DRV_Reg32(0x11C101F8);
	temp[15] = DRV_Reg32(0x11C101FC);
	temp[16] = DRV_Reg32(0x11C10200);
	temp[17] = DRV_Reg32(0x11C10204);
	temp[18] = DRV_Reg32(0x11C10208);
	temp[19] = DRV_Reg32(0x11C1020C);
	temp[20] = DRV_Reg32(0x11C10210);
	temp[21] = DRV_Reg32(0x11C10214);

	for (i = 0; i < (sizeof(temp)/sizeof(unsigned int )); i ++)
		thermal_dbg_print("[lvts_cal]%d: %s, 0x%x\n", i, addr[i], temp[i]);


	g_golden_temp = ((temp[0] & _BITMASK_(31:24)) >> 24);//0x11C1_01C0
	g_count_r[0] = (temp[1] & _BITMASK_(23:0)); //0x11C1_01C4,LVTS1_0
	g_count_r[1] = (temp[2] & _BITMASK_(23:0)); //0x11C1_01C8,LVTS1_1
	g_count_r[2] = (temp[3] & _BITMASK_(23:0)); //0x11C1_01CC,LVTS2_0
	g_count_r[3] = (temp[4] & _BITMASK_(23:0)); //0x11C1_01D0,LVTS2_1
	g_count_r[4] = (temp[5] & _BITMASK_(23:0)); //0x11C1_01D4,LVTS3_0
	g_count_r[5] = (temp[6] & _BITMASK_(23:0)); //0x11C1_01D8,LVTS3_1
	g_count_r[6] = (temp[7] & _BITMASK_(23:0)); //0x11C1_01DC,LVTS3_2
	g_count_r[7] = (temp[8] & _BITMASK_(23:0)); //0x11C1_01E0,LVTS3_3
	g_count_r[8] = (temp[9] & _BITMASK_(23:0)); //0x11C1_01E4,LVTS4_0
	g_count_r[9] = (temp[10] & _BITMASK_(23:0)); //0x11C1_01E8,LVTS4_1
	g_count_r[10] = (temp[11] & _BITMASK_(23:0));//0x11C1_01EC,LVTS5_0
	g_count_r[11] = (temp[12] & _BITMASK_(23:0));//0x11C1_01F0,LVTS5_1
	g_count_r[12] = (temp[13] & _BITMASK_(23:0));//0x11C1_01F4,LVTS6_0
	g_count_r[13] = (temp[14] & _BITMASK_(23:0));//0x11C1_01F8,LVTS6_1
	g_count_r[14] = (temp[15] & _BITMASK_(23:0));//0x11C1_01FC,LVTS7_0
	g_count_r[15] = (temp[16] & _BITMASK_(23:0));//0x11C1_0200,LVTS7_1
	g_count_r[16] = (temp[17] & _BITMASK_(23:0));//0x11C1_0204,LVTS7_2

	/*0214, LVTS1_0_COUNT_RC*/
	g_count_rc[0] = (temp[21] & _BITMASK_(23:0));

	/*01C4, 01C8, 01CC, LVTS2_0_COUNT_RC*/
	g_count_rc[1] = ((temp[1] & _BITMASK_(31:24)) >> 8) +
		((temp[2] & _BITMASK_(31:24)) >> 16)+
		((temp[3] & _BITMASK_(31:24)) >> 24);

	/*01D0, 01D4, 01D8, LVTS3_0_COUNT_RC*/
	g_count_rc[2] = ((temp[4] & _BITMASK_(31:24)) >> 8) +
		((temp[5] & _BITMASK_(31:24)) >> 16) +
		((temp[6] & _BITMASK_(31:24)) >> 24);

	/*01DC, 01E0, 01E4, LVTS4_0_COUNT_RC*/
	g_count_rc[3] = ((temp[7] & _BITMASK_(31:24)) >> 8) +
		((temp[8] & _BITMASK_(31:24)) >> 16) +
		((temp[9] & _BITMASK_(31:24)) >> 24);

	/*01E8, 01EC, 01F0, LVTS5_0_COUNT_RC*/
	g_count_rc[4] = ((temp[10] & _BITMASK_(31:24)) >> 8) +
		((temp[11] & _BITMASK_(31:24)) >> 16) +
		((temp[12] & _BITMASK_(31:24)) >> 24);

	/*01F4, 01F8, 01FC, LVTS6_0_COUNT_RC*/
	g_count_rc[5] = ((temp[13] & _BITMASK_(31:24)) >> 8) +
		((temp[14] & _BITMASK_(31:24)) >> 16) +
		((temp[15] & _BITMASK_(31:24)) >> 24);

	/*0200, 0204, 0208, LVTS7_0_COUNT_RC*/
	g_count_rc[6] = ((temp[16] & _BITMASK_(31:24)) >> 8) +
		((temp[17] & _BITMASK_(31:24)) >> 16) +
		((temp[18] & _BITMASK_(31:24)) >> 24);

	for (i = 0; i < L_TS_LVTS_NUM; i++){
		if (i == 0){
			if ((temp[0] & _BITMASK_(7:0)) != 0)
				break;
		} else {
			if (temp[i] != 0)
				break;
		}
	}

	if (i == L_TS_LVTS_NUM) {
		/* It means all efuse data are equal to 0 */
		thermal_dbg_print(
			"[lvts_cal] This sample is not Thermal calibrated, fake !!\n");
		g_golden_temp = 50;
		for (i = 0; i < L_TS_LVTS_NUM; i++) {
			g_count_r[i] = 35000;
			g_count_rc[i] = 2750;
		}
		g_use_fake_efuse = 1;
	}

	thermal_dbg_print("[lvts_cal] g_golden_temp = %d\n", g_golden_temp);
	for (i = 0; i < L_TS_LVTS_NUM; i++)
		thermal_dbg_print("[lvts_cal] g_count_r%d = %u, g_count_rc%d = %u\n",
			i, g_count_r[i], i, g_count_rc[i]);

}

/*
 * temperature2 to set the middle threshold for interrupting CPU.
 * -275000 to disable it.
 */
static void lvts_set_tc_trigger_hw_protect(
int temperature, int temperature2, int tc_num)
{
	int temp = 0, raw_high, config, d_index;
	enum lvts_sensor_enum ts_name;
	unsigned int offset;

	offset = lvts_tscpu_g_tc[tc_num].tc_offset;

	/* temperature2=80000;  test only */
	thermal_dbg_print("set_tc_trigger_hw_protect t1=%d t2=%d\n",
						temperature, temperature2);

	if (lvts_tscpu_g_tc[tc_num].dominator_ts_idx <
		lvts_tscpu_g_tc[tc_num].ts_number){
		d_index = lvts_tscpu_g_tc[tc_num].dominator_ts_idx;
	} else {
		thermal_dbg_print("Error: LVTS TC%d, dominator_ts_idx = %d should smaller than ts_number = %d\n",
			tc_num,
			lvts_tscpu_g_tc[tc_num].dominator_ts_idx,
			lvts_tscpu_g_tc[tc_num].ts_number);

		thermal_dbg_print("Use the sensor point 0 as the dominated sensor\n");
		d_index = 0;
	}

	ts_name = lvts_tscpu_g_tc[tc_num].ts[d_index];

	thermal_dbg_print("%s # in tc%d , the dominator ts_name is %d\n",
						__func__, tc_num, ts_name);

	/* temperature to trigger SPM state2 */
	raw_high = lvts_temp_to_raw(temperature, ts_name);

	temp = DRV_Reg32(offset + LVTSMONINT_0);
	/* disable trigger SPM interrupt */
	mt_reg_sync_writel_print(temp & 0x00000000, (void *)(offset + LVTSMONINT_0));

	/* Use the maximum of 4 sensing point */
	config = (1 << 16);
	mt_reg_sync_writel_print(config, (void *)(offset + LVTSPROTCTL_0));

	/* set hot to HOT wakeup event */
	mt_reg_sync_writel_print(raw_high, (void *)(offset + LVTSPROTTC_0));

	/* enable trigger Hot SPM interrupt */
	mt_reg_sync_writel_print(temp | 0x80000000, (void *)(offset + LVTSMONINT_0));
}

static int lvts_read_tc_raw_and_temp(
		unsigned int *tempmsr_name, enum lvts_sensor_enum ts_name)
{
	int temp = 0, raw = 0, raw1 = 0, raw2 = 0, rdata;

	if (tempmsr_name == 0)
		return 0;

	raw = DRV_Reg32((tempmsr_name));
	raw1 = (raw & 0x10000) >> 16; //bit 16 : valid bit
	raw2 = raw & 0x7fff;
	temp = lvts_raw_to_temp(raw2, ts_name);
	//if(raw2 == 0)
	//	print("%s, ts%d msr_all=%x, valid=%d, msr_temp=%d, temp=%d\n", __func__,
	//	ts_name, raw, raw1, raw2, temp);

	//if((ts_name == 0) || (ts_name == 16)) {print1("---------------------------\n");}
	tscpu_ts_lvts_temp_r[ts_name] = raw2;

	return temp;
}

static void lvts_tscpu_thermal_read_tc_temp(
		int tc_num, enum lvts_sensor_enum type, int order)
{
	unsigned int offset;

	offset = lvts_tscpu_g_tc[tc_num].tc_offset;

	switch (order) {
	case 0:
		tscpu_ts_lvts_temp[type] =
			lvts_read_tc_raw_and_temp((unsigned int *)(offset + LVTSMSR0_0), type);
		break;
	case 1:
		tscpu_ts_lvts_temp[type] =
			lvts_read_tc_raw_and_temp((unsigned int *)(offset + LVTSMSR1_0), type);
		break;
	case 2:
		tscpu_ts_lvts_temp[type] =
			lvts_read_tc_raw_and_temp((unsigned int *)(offset + LVTSMSR2_0), type);
		break;
	case 3:
		tscpu_ts_lvts_temp[type] =
			lvts_read_tc_raw_and_temp((unsigned int *)(offset + LVTSMSR3_0), type);
		break;
	default:
		tscpu_ts_lvts_temp[type] =
			lvts_read_tc_raw_and_temp((unsigned int *)(offset + LVTSMSR0_0), type);
		thermal_dbg_print("Error senor point out of range: %s order %d tc_num %d type %d temp %d\n",
			__func__, order, tc_num, type,
			tscpu_ts_lvts_temp[type]);
		break;
	}

	/*
	   thermal_dbg_print("%s order %d tc_num %d type %d temp %d\n",
		__func__, order, tc_num, type,
		tscpu_ts_lvts_temp[type]);
	 */
}

static int check_all_temp_valid(void)
{
	int i, j, raw;


	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		for (j = 0; j < lvts_tscpu_g_tc[i].ts_number; j++) {
			raw = tscpu_ts_lvts_temp_r[lvts_tscpu_g_tc[i].ts[j]];

			if (raw == 0) {
                                print("driver not ready %d %d %d\n", i, j, raw);
				return 0;	/* The temperature is not valid. */
                        }
		}
	}
        return 1;/* The temperature is valid. */
}

int tscpu_is_temp_valid(void)
{
	int is_valid = 0;
	unsigned long flags;

	is_valid = check_all_temp_valid();

	return is_valid;
}


//thermal_set_vcore_voltage(725000);
#ifdef MTK_PMIC_MT6359
static struct mtk_regulator reg_vcore;
#endif
unsigned int thermal_init_vcore_voltage(void)
{
#ifdef MTK_PMIC_MT6359
	int ret;
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		printf("mtk_regulator_get vcore fail\n");

#endif
}

unsigned int thermal_set_vcore_voltage(unsigned int vcore)
{
#ifdef MTK_PMIC_MT6359
	int ret;
	printf("thermal_set_vcore_voltage vcore:%d\n", vcore);

	return mtk_regulator_set_voltage(&reg_vcore, vcore, vcore);
#endif
	return 0;
}


unsigned int thermal_get_vcore_voltage(void)
{
#ifdef MTK_PMIC_MT6359
	return mtk_regulator_get_voltage(&reg_vcore);
#else
	return 0;
#endif
}

static int system_Vcore;
void lvts_restore_Vcore(void)
{

	print("lvts_restore_Vcore Vcore:%d\n",system_Vcore);
	thermal_set_vcore_voltage(system_Vcore);
	mdelay(2);
	print("lvts_restore_Vcore get Vcore:%d\n",thermal_get_vcore_voltage());

}

void lvts_print_mcu_temp_1_io(unsigned int vcore)
{
	int i=0;
        int count=0, cunt2=0;
	int j=0, raw=0, ret=0;


	print("lvts_print_mcu_temp_1_io set Vcore:%d\n",vcore);
	thermal_set_vcore_voltage(vcore);
        mdelay(2);
        print("lvts_print_mcu_temp_1_io get Vcore:%d\n",thermal_get_vcore_voltage());

	while (ret == 0 && cunt2 < 100) {
		lvts_read_all_tc_temperature();

		ret = tscpu_is_temp_valid();
		if (ret)
			break;
		cunt2++;
		mdelay(2);
		//print("%d,%d\n",cunt2, ret);
	}

	if(vcore == 700000) {
                for(i = 0; i < L_TS_LVTS_NUM; i++)
                        tscpu_ts_lvts_temp_0p7[i] = tscpu_ts_lvts_temp[i];

		//tscpu_ts_lvts_temp_0p7[5] = 50000;
		print("[Thermal]<<< Vcore=0.7V >>>\n");
		print("[Thermal]TS0=%d,TS1=%d,TS2=%d,TS3=%d,TS4=%d,TS5=%d,TS6=%d,TS7=%d,TS8=%d,TS9=%d,TS10=%d,TS11=%d,TS12=%d,TS13=%d,TS14=%d,TS15=%d,TS16=%d\n",
	                tscpu_ts_lvts_temp_0p7[0],
	                tscpu_ts_lvts_temp_0p7[1],
	                tscpu_ts_lvts_temp_0p7[2],
	                tscpu_ts_lvts_temp_0p7[3],
	                tscpu_ts_lvts_temp_0p7[4],
	                tscpu_ts_lvts_temp_0p7[5],
	                tscpu_ts_lvts_temp_0p7[6],
	                tscpu_ts_lvts_temp_0p7[7],
	                tscpu_ts_lvts_temp_0p7[8],
	                tscpu_ts_lvts_temp_0p7[9],
	                tscpu_ts_lvts_temp_0p7[10],
	                tscpu_ts_lvts_temp_0p7[11],
	                tscpu_ts_lvts_temp_0p7[12],
	                tscpu_ts_lvts_temp_0p7[13],
	                tscpu_ts_lvts_temp_0p7[14],
	                tscpu_ts_lvts_temp_0p7[15],
	                tscpu_ts_lvts_temp_0p7[16]
	                );
	}


	if(vcore == 650000) {

                for(i = 0; i < L_TS_LVTS_NUM; i++)
                        tscpu_ts_lvts_temp_0p65[i] = tscpu_ts_lvts_temp[i];


		//tscpu_ts_lvts_temp_0p65[16] = 10000;

		print("[Thermal]<<< Vcore=0.65V >>>\n");
        	print("[Thermal]TS0=%d,TS1=%d,TS2=%d,TS3=%d,TS4=%d,TS5=%d,TS6=%d,TS7=%d,TS8=%d,TS9=%d,TS10=%d,TS11=%d,TS12=%d,TS13=%d,TS14=%d,TS15=%d,TS16=%d\n",
	                tscpu_ts_lvts_temp_0p65[0],
	                tscpu_ts_lvts_temp_0p65[1],
	                tscpu_ts_lvts_temp_0p65[2],
	                tscpu_ts_lvts_temp_0p65[3],
	                tscpu_ts_lvts_temp_0p65[4],
	                tscpu_ts_lvts_temp_0p65[5],
	                tscpu_ts_lvts_temp_0p65[6],
	                tscpu_ts_lvts_temp_0p65[7],
	                tscpu_ts_lvts_temp_0p65[8],
	                tscpu_ts_lvts_temp_0p65[9],
	                tscpu_ts_lvts_temp_0p65[10],
	                tscpu_ts_lvts_temp_0p65[11],
	                tscpu_ts_lvts_temp_0p65[12],
	                tscpu_ts_lvts_temp_0p65[13],
	                tscpu_ts_lvts_temp_0p65[14],
	                tscpu_ts_lvts_temp_0p65[15],
	                tscpu_ts_lvts_temp_0p65[16]
	                );
	}


	if(vcore == 575000) {

                for(i = 0; i < L_TS_LVTS_NUM; i++)
                        tscpu_ts_lvts_temp_0p575[i] = tscpu_ts_lvts_temp[i];


		//tscpu_ts_lvts_temp_0p575[10] = -10000;

		print("[Thermal]<<< Vcore=0.575V >>>\n");
        	print("[Thermal]TS0=%d,TS1=%d,TS2=%d,TS3=%d,TS4=%d,TS5=%d,TS6=%d,TS7=%d,TS8=%d,TS9=%d,TS10=%d,TS11=%d,TS12=%d,TS13=%d,TS14=%d,TS15=%d,TS16=%d\n",
	                tscpu_ts_lvts_temp_0p575[0],
	                tscpu_ts_lvts_temp_0p575[1],
	                tscpu_ts_lvts_temp_0p575[2],
	                tscpu_ts_lvts_temp_0p575[3],
	                tscpu_ts_lvts_temp_0p575[4],
	                tscpu_ts_lvts_temp_0p575[5],
	                tscpu_ts_lvts_temp_0p575[6],
	                tscpu_ts_lvts_temp_0p575[7],
	                tscpu_ts_lvts_temp_0p575[8],
	                tscpu_ts_lvts_temp_0p575[9],
	                tscpu_ts_lvts_temp_0p575[10],
	                tscpu_ts_lvts_temp_0p575[11],
	                tscpu_ts_lvts_temp_0p575[12],
	                tscpu_ts_lvts_temp_0p575[13],
	                tscpu_ts_lvts_temp_0p575[14],
	                tscpu_ts_lvts_temp_0p575[15],
	                tscpu_ts_lvts_temp_0p575[16]
	                );
	}

/*
	print("[Thermal]TS0=%d,TS1=%d,TS2=%d,TS3=%d,TS4=%d,TS5=%d,TS6=%d,TS7=%d,TS8=%d,TS9=%d,TS10=%d,TS11=%d,TS12=%d,TS13=%d,TS14=%d,TS15=%d,TS16=%d\n",
                tscpu_ts_lvts_temp[0],
                tscpu_ts_lvts_temp[1],
                tscpu_ts_lvts_temp[2],
                tscpu_ts_lvts_temp[3],
                tscpu_ts_lvts_temp[4],
                tscpu_ts_lvts_temp[5],
                tscpu_ts_lvts_temp[6],
                tscpu_ts_lvts_temp[7],
                tscpu_ts_lvts_temp[8],
                tscpu_ts_lvts_temp[9],
                tscpu_ts_lvts_temp[10],
                tscpu_ts_lvts_temp[11],
                tscpu_ts_lvts_temp[12],
                tscpu_ts_lvts_temp[13],
                tscpu_ts_lvts_temp[14],
                tscpu_ts_lvts_temp[15],
                tscpu_ts_lvts_temp[16]
                );
*/
}

void lvts_compare_all_temp(void)
{
	int i=0;
       	int Max_TVDD_temp, Min_TVDD_temp;

	for(i = 0;i < L_TS_LVTS_NUM; i++){

		Max_TVDD_temp = MAX(tscpu_ts_lvts_temp_0p7[i],tscpu_ts_lvts_temp_0p65[i]);
		Max_TVDD_temp = MAX(Max_TVDD_temp,tscpu_ts_lvts_temp_0p575[i]);

		Min_TVDD_temp = MIN(tscpu_ts_lvts_temp_0p7[i],tscpu_ts_lvts_temp_0p65[i]);
		Min_TVDD_temp = MIN(Min_TVDD_temp,tscpu_ts_lvts_temp_0p575[i]);

		print("Max_TVDD_temp[%d] =%d, Min_TVDD_temp[%d]=%d\n", i, Max_TVDD_temp, i, Min_TVDD_temp);

                if ((Max_TVDD_temp - Min_TVDD_temp) > 10000) {

			print("[Thermal_VDD_Sweep ERROR]TS:%d diff=%d, bin this IC to fail IC\n",
				i, Max_TVDD_temp - Min_TVDD_temp);

			while(1) {
                               print("SLT_BIN2\n");
                        }
		}
	}

	print("Thermal VDD Sweep test finish!!!\n");
}

void lvts_read_all_tc_temperature(void)
{
        unsigned int i = 0, j = 0;

        for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++)
                for (j = 0; j < lvts_tscpu_g_tc[i].ts_number; j++)
                        lvts_tscpu_thermal_read_tc_temp(i,
                                        lvts_tscpu_g_tc[i].ts[j], j);
}

/* pause ALL periodoc temperature sensing point */
static void lvts_thermal_pause_all_periodoc_temp_sensing(void)
{
	unsigned int i, temp, offset;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		offset = lvts_tscpu_g_tc[i].tc_offset;
		temp = DRV_Reg32(offset + LVTSMSRCTL1_0);
		/* set bit8=bit1=bit2=bit3=1 to pause sensing point 0,1,2,3 */
		mt_reg_sync_writel_print((temp | 0x10E),
			(void *)(offset + LVTSMSRCTL1_0));
	}

}

/* release ALL periodoc temperature sensing point */
static void lvts_thermal_release_all_periodoc_temp_sensing(void)
{
	unsigned int i = 0, temp, offset;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		offset = lvts_tscpu_g_tc[i].tc_offset;

		temp = DRV_Reg32(offset + LVTSMSRCTL1_0);
		/* set bit1=bit2=bit3=bit8=0 to release sensing point 0,1,2,3*/
		mt_reg_sync_writel_print(((temp & (~0x10E))),
			(void *)(offset + LVTSMSRCTL1_0));
	}

}

static void lvts_tscpu_thermal_enable_all_periodoc_sensing_point(int tc_num)
{
	U32 offset;

	offset = lvts_tscpu_g_tc[tc_num].tc_offset;

	thermal_dbg_print("===== %s begin ======\n", __func__);
	thermal_dbg_print("%s %d\n", __func__, lvts_tscpu_g_tc[tc_num].ts_number);

	switch (lvts_tscpu_g_tc[tc_num].ts_number) {
	case 1:
		/* enable periodoc temperature sensing point 0 */
		mt_reg_sync_writel_print(0x00000201,
			(void *)(offset + LVTSMONCTL0_0));
		break;
	case 2:
		/* enable periodoc temperature sensing point 0,1 */
		mt_reg_sync_writel_print(0x00000203,
			(void *)(offset + LVTSMONCTL0_0));
		break;
	case 3:
		/* enable periodoc temperature sensing point 0,1,2 */
		mt_reg_sync_writel_print(0x00000207,
			(void *)(offset + LVTSMONCTL0_0));
		break;
	case 4:
		/* enable periodoc temperature sensing point 0,1,2,3*/
		mt_reg_sync_writel_print(0x0000020F,
			(void *)(offset + LVTSMONCTL0_0));
		break;
	default:
		thermal_dbg_print("Error at %s\n", __func__);
		break;
	}

	thermal_dbg_print(
		"lvts_tscpu_thermal_enable_all_periodoc_sensing_point=%d,%d\n",
		tc_num, lvts_tscpu_g_tc[tc_num].ts_number);
}

/*
 * disable ALL periodoc temperature sensing point
 */
void lvts_disable_all_sensing_points(void)
{
	int i = 0, offset;

	thermal_dbg_print("%s\n", __func__);

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		offset = lvts_tscpu_g_tc[i].tc_offset;
		mt_reg_sync_writel_print(0x00000000, offset + LVTSMONCTL0_0);
	}
}

void lvts_enable_all_sensing_points(void)
{
	int i, offset;

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {

		offset = lvts_tscpu_g_tc[i].tc_offset;

		thermal_dbg_print("%s %d:%d\n", __func__, i,
			lvts_tscpu_g_tc[i].ts_number);

		switch (lvts_tscpu_g_tc[i].ts_number) {
		case 1:
			/* enable sensing point 0 */
			mt_reg_sync_writel_print(0x00000201,
					offset + LVTSMONCTL0_0);
			break;
		case 2:
			/* enable sensing point 0,1 */
			mt_reg_sync_writel_print(0x00000203,
					offset + LVTSMONCTL0_0);
			break;
		case 3:
			/* enable sensing point 0,1,2 */
			mt_reg_sync_writel_print(0x00000207,
					offset + LVTSMONCTL0_0);
			break;
		case 4:
			/* enable sensing point 0,1,2,3*/
			mt_reg_sync_writel_print(0x0000020F,
					offset + LVTSMONCTL0_0);
			break;
		default:
			thermal_dbg_print("Error at %s\n", __func__);
			break;
		}
	}
}

static void lvts_configure_polling_speed_and_filter(int tc_num)
{
	unsigned int offset, lvtsMonCtl1, lvtsMonCtl2;

	thermal_dbg_print("%s\n", __func__);

	offset = lvts_tscpu_g_tc[tc_num].tc_offset;

	lvtsMonCtl1 = (((lvts_tscpu_g_tc[tc_num].tc_speed.group_interval_delay
			<< 20) & _BITMASK_(29:20)) |
			(lvts_tscpu_g_tc[tc_num].tc_speed.period_unit &
			_BITMASK_(9:0)));
	lvtsMonCtl2 = (((lvts_tscpu_g_tc[tc_num].tc_speed.filter_interval_delay
			<< 16) & _BITMASK_(25:16)) |
			(lvts_tscpu_g_tc[tc_num].tc_speed.sensor_interval_delay
			& _BITMASK_(9:0)));
	/*
	 * Calculating period unit in Module clock x 256, and the Module clock
	 * will be changed to 26M when Infrasys enters Sleep mode.
	 */

	/*
	 * bus clock 66M counting unit is
	 *			12 * 1/66M * 256 = 12 * 3.879us = 46.545 us
	 */
	mt_reg_sync_writel_print(lvtsMonCtl1, offset + LVTSMONCTL1_0);
	/*
	 *filt interval is 1 * 46.545us = 46.545us,
	 *sen interval is 429 * 46.545us = 19.968ms
	 */
	mt_reg_sync_writel_print(lvtsMonCtl2, offset + LVTSMONCTL2_0);

	/* temperature sampling control, 2 out of 4 samples */
	mt_reg_sync_writel_print(0x00000492, offset + LVTSMSRCTL0_0);

	udelay(1);
	thermal_dbg_print(
		"%s %d, LVTSMONCTL1_0= 0x%x,LVTSMONCTL2_0= 0x%x,LVTSMSRCTL0_0= 0x%x\n",
		__func__, tc_num,
		DRV_Reg32(LVTSMONCTL1_0 + offset),
		DRV_Reg32(LVTSMONCTL2_0 + offset),
		DRV_Reg32(LVTSMSRCTL0_0 + offset));
}

void lvts_tscpu_thermal_initial_all_tc(void)
{
	int i = 0, offset;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		offset = lvts_tscpu_g_tc[i].tc_offset;

		/*  set sensor index of LVTS */
		mt_reg_sync_writel_print(0x13121110, LVTSTSSEL_0 + offset);
		/*  set calculation scale rules */
		mt_reg_sync_writel_print(0x00000300, LVTSCALSCALE_0 + offset);
		/* Set Device Single mode */
		lvts_write_device(0x81030000, 0x06, 0xB8, i);

		lvts_configure_polling_speed_and_filter(i);
	}
}

#if THERMAL_HW_PROTECT
static void lvts_config_all_tc_hw_protect(int temperature, int temperature2)
{
        unsigned int i = 0;
        int wd_api_ret;
        struct wd_api *wd_api;
        thermal_dbg_print("===== %s begin ======\n", __func__);
        thermal_dbg_print("lvts_config_all_tc_hw_protect,temperature=%d,temperature2=%d,\n",
                        temperature, temperature2);

        /*spend 860~1463 us */
        /*Thermal need to config to direct reset mode
         *this API provide by Weiqi Fu(RGU SW owner).
         */

        /* The way of setting hardware protection is different than Android* load. */
        mtk_wdt_request_en_set(MTK_WDT_REQ_MODE_THERMAL, WD_REQ_EN);

	/* Move thermal HW protection ahead... */
        for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++)
                lvts_set_tc_trigger_hw_protect(temperature, temperature2, i);

        mtk_wdt_request_mode_set(MTK_WDT_REQ_MODE_THERMAL, WD_REQ_RST_MODE);
}
#endif

static void lvts_device_check_counting_status(int tc_num)
{
	/* Check this when LVTS device is counting for
	 * a temperature or a RC now
	 */

	int offset, cnt;

	offset = lvts_tscpu_g_tc[tc_num].tc_offset; //tc offset

	cnt = 0;
	while ((DRV_Reg32(LVTS_CONFIG_0 + offset) & _BIT_(25))) {
		cnt++;

		if (cnt == 100) {
			thermal_dbg_print("Error: DEVICE_SENSING_STATUS didn't ready\n");
			break;
		}
		udelay(2);
	}
}

static void lvts_device_check_read_write_status(int tc_num)
{
	/* Check this when LVTS device is doing a register
	 * read or write operation
	 */

	int offset, cnt;

	offset = lvts_tscpu_g_tc[tc_num].tc_offset;

	cnt = 0;
	while ((DRV_Reg32(LVTS_CONFIG_0 + offset) & _BIT_(24))) {
		cnt++;

		if (cnt == 100) {
			thermal_dbg_print("Error: DEVICE_ACCESS_START didn't ready\n");
			break;
		}
		udelay(2);
	}
}

#if DUMP_LVTS_REGISTER
void read_device_reg_before_active(void)
{
        int i, j;
        unsigned int addr, data;

	thermal_dbg_print("===== %s begin ======\n", __func__);

        for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
                for (j = 0; j < NUM_LVTS_DEVICE_REG; j++) {
                        addr = g_lvts_device_addrs[j];
                        data =  lvts_read_device(addr, i);
			thermal_dbg_print("Addr 0x%x = 0x%x\n", addr, data);
                }
        }
}
#endif

void lvts_device_read_count_RC_N(void)
{
	/* Resistor-Capacitor Calibration */
	/* count_RC_N: count RC now */
	int i, j, offset, num_ts, s_index;
	unsigned int data;

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {

		offset = lvts_tscpu_g_tc[i].tc_offset;
		num_ts = lvts_tscpu_g_tc[i].ts_number;

		/* Set LVTS Manual-RCK operation */
		lvts_write_device(0x81030000, 0x0E, 0x00, i);

		for (j = 0; j < num_ts; j++) {
			s_index = lvts_tscpu_g_tc[i].ts[j];

			/* Select sensor-N with RCK */
			lvts_write_device(0x81030000, 0x0D, j, i);
			/* Set Device Single mode */
			lvts_write_device(0x81030000, 0x06, 0x78, i);
			/* Enable TS_EN */
//			lvts_write_device(0x81030000, 0x08, 0xF7, i);
			lvts_write_device(0x81030000, 0x08, 0xF5, i);
			/* Toggle TSDIV_EN & TSVCO_TG */
//			lvts_write_device(0x81030000, 0x08, 0xFE, i);
			lvts_write_device(0x81030000, 0x08, 0xFC, i);
			/* Toggle TSDIV_EN & TSVCO_TG */
//			lvts_write_device(0x81030000, 0x08, 0xF7, i);
			lvts_write_device(0x81030000, 0x08, 0xF5, i);
			lvts_write_device(0x81030000, 0x07, 0xA6, i);
			/* Wait 8us for device settle + 2us buffer*/
			udelay(10);
			/* Kick-off RCK counting */
			lvts_write_device(0x81030000, 0x03, 0x02, i);
			/* wait 30us */
			udelay(30);

			lvts_device_check_counting_status(i);

			/* Get RCK count data (sensor-N) */
			data = lvts_read_device(0x00, i);
			/* wait 2us + 3us buffer*/
			udelay(5);

			/* Disable TS_EN */
//			lvts_write_device(0x81030000, 0x08, 0xF3, i);
			lvts_write_device(0x81030000, 0x08, 0xF1, i);

			lvts_device_check_read_write_status(i);

			/* Get RCK value from LSB[23:0] */
			g_count_rc_now[s_index] = (data & _BITMASK_(23:0));
		}

		/* Recover Setting for Normal Access on
		 * temperature fetch
		 */
		/* Select Sensor-N without RCK */
		lvts_write_device(0x81030000, 0x0D, 0x10, i);
	}

	thermal_dbg_print("[COUNT_RC_NOW]\n");
	for (i = 0; i < L_TS_LVTS_NUM; i++)
		thermal_dbg_print("%d:%d\n", i, g_count_rc_now[i]);

#if DUMP_LVTS_REGISTER
	read_device_reg_before_active();
#endif
}

void lvts_device_enable_auto_rck(void)
{
	int i;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		/* Set LVTS AUTO-RCK operation */
		lvts_write_device(0x81030000, 0x0E, 0x01, i);
	}
}

static int lvts_tscpu_max_temperature(void)
{
	unsigned int i, j;
	int max = 0;

	thermal_dbg_print("===== %s begin ======\n", __func__);
	for (i = 0; i < ARRAY_SIZE(lvts_tscpu_g_tc); i++) {
		for (j = 0; j < lvts_tscpu_g_tc[i].ts_number; j++) {
			if (i == 0 && j == 0) {
				max = tscpu_ts_lvts_temp[
					lvts_tscpu_g_tc[i].ts[j]];
			} else {
				if (max < tscpu_ts_lvts_temp[
						lvts_tscpu_g_tc[i].ts[j]])
					max = tscpu_ts_lvts_temp[
						lvts_tscpu_g_tc[i].ts[j]];
			}
		}
	}
	return max;
}

static void lvts_print_mcu_temp(void)
{
	int i;

	thermal_dbg_print("===== %s begin ======\n", __func__);
	for (i = 0; i < L_TS_LVTS_NUM; i++)
		thermal_dbg_print("TS_LVTS%d = %d\n", (i + 1), tscpu_ts_lvts_temp[i]);
}

/*=============================================================
 * Extern functions
 *=============================================================
 */
void mt_reg_sync_writel_print(unsigned int val, void* addr)
{
#if PRINT_LVTS_CONTROLLER_WRITE_LOG
	thermal_dbg_print("##### LVTS_REG_WR: addr 0x%x, val 0x%x\n", addr, val);
#endif
		DRV_WriteReg32(addr, val);	//xxx, mt65xx_reg_sync_writel(val, addr);
}

int lvts_write_device(unsigned int config, unsigned int dev_reg_idx,
unsigned int data, int tc_num)
{
	unsigned int offset;

	dev_reg_idx &= 0xFF;
	data &= 0xFF;

	config = config | (dev_reg_idx << 8) | data;

	offset = lvts_tscpu_g_tc[tc_num].tc_offset;

	mt_reg_sync_writel_print(config, (void *)(LVTS_CONFIG_0 + offset));
	udelay(2);
#if PRINT_LVTS_DEVICE_WRITE_LOG
	thermal_dbg_print("lvts_write_device LVTS_CONFIG_%d (0x%x) = 0x%x\n",
			tc_num, LVTS_CONFIG_0 + offset,
			DRV_Reg32(LVTS_CONFIG_0 + offset));
#endif
	return 1;
}

unsigned int lvts_read_device(unsigned int dev_reg_idx, int tc_num)
{
        int offset, cnt;
	unsigned int data;

	offset = lvts_tscpu_g_tc[tc_num].tc_offset;

	/* DEVICE_REG_DATA doesn't care, because it's read operation */
	lvts_write_device(0x81020000, dev_reg_idx, 0x00, tc_num);
	/* wait 2us + 3us buffer*/
	udelay(5);
	/* Check ASIF bus status for transaction finished
	 * Wait until DEVICE_ACCESS_START = 0
	 */
	cnt = 0;
	while ((DRV_Reg32(LVTS_CONFIG_0 + offset) & _BIT_(24))) {
		cnt++;

		if (cnt == 100) {
			thermal_dbg_print("Error: DEVICE_ACCESS_START didn't ready\n");
			break;
		}
		udelay(2);
	}

	data = (DRV_Reg32(LVTSRDATA0_0 + offset));

	return data;
}

int lvts_raw_to_temp(unsigned int msr_raw, enum lvts_sensor_enum ts_name)
{
	/* This function returns degree mC
	 * temp[i] = a * MSR_RAW/16384 + GOLDEN_TEMP/2 + b
	 * a = -204.65
	 * b =  204.65
	 */
	int temp_mC = 0;
	int temp1 = 0;

	temp1 = (LVTS_COEFF_A_X_1000 * ((unsigned long long int) msr_raw)) >> 14;

	temp_mC = temp1 + g_golden_temp * 500 + LVTS_COEFF_B_X_1000;

	return temp_mC;
}

unsigned int lvts_temp_to_raw(int temp, enum lvts_sensor_enum ts_name)
{
        /* MSR_RAW = ((temp[i] - GOLDEN_TEMP/2 - b) * 16384) / a
         * a = -204.65
         * b =  204.65
         */
        unsigned int msr_raw = 0;

        msr_raw = ((long long int)((g_golden_temp * 500 + LVTS_COEFF_B_X_1000
                        - temp)) << 14)/(-1 * LVTS_COEFF_A_X_1000);

	msr_raw = msr_raw & 0xFFFF;

        thermal_dbg_print("lvts_temp_to_raw msr_raw = %u,temp=%d\n",
                                                        msr_raw, temp);
        return msr_raw;
}

void lvts_tscpu_reset_thermal(void)
{
	/* chip dependent, Have to confirm with DE */

	int temp = 0;

	thermal_dbg_print("===== %s begin ======\n", __func__);

	/* reset thremal ctrl */
	/* TODO: Is it necessary to read INFRA_GLOBALCON_RST_0_SET? */
	temp = DRV_Reg32(INFRA_GLOBALCON_RST_0_SET);

	/* 1: Enables thermal control software reset */
	temp |= 0x00000001;
	mt_reg_sync_writel_print(temp, (void *)(INFRA_GLOBALCON_RST_0_SET));

	/* TODO: How long to set the reset bit? */

	/* un reset */
	/* TODO: Is it necessary to read INFRA_GLOBALCON_RST_0_CLR? */
	temp = DRV_Reg32(INFRA_GLOBALCON_RST_0_CLR);

	/* 1: Enable reset Disables thermal control software reset */
	temp |= 0x00000001;

	mt_reg_sync_writel_print(temp, (void *)(INFRA_GLOBALCON_RST_0_CLR));
}


void lvts_tscpu_reset_thermal_MCU(void)
{
	/* chip dependent, Have to confirm with DE */

	int temp = 0;


	/* reset thremal ctrl */
	/* TODO: Is it necessary to read INFRA_GLOBALCON_RST_0_SET? */
	temp = DRV_Reg32(INFRA_GLOBALCON_RST_4_SET);

	/* 1: Enables thermal control software reset */
	temp |= 0x00000400;
	mt_reg_sync_writel_print(temp, (void *)INFRA_GLOBALCON_RST_4_SET);

	/* TODO: How long to set the reset bit? */

	/* un reset */
	/* TODO: Is it necessary to read INFRA_GLOBALCON_RST_0_CLR? */
	temp = DRV_Reg32(INFRA_GLOBALCON_RST_4_CLR);

	/* 1: Enable reset Disables thermal control software reset */
	temp |= 0x00000400;

	mt_reg_sync_writel_print(temp, (void *)INFRA_GLOBALCON_RST_4_CLR);
}


int lvts_thermal_init(void)
{
	int ret,temp;
	int cnt=0;
	print("===== %s begin ======\n", __func__);

	/*get system current Vcore for restore after test finished*/
        thermal_init_vcore_voltage();
        system_Vcore = thermal_get_vcore_voltage();
        print("get system_Vcore before thermal test:%d\n",system_Vcore);


	lvts_tscpu_reset_thermal();
        lvts_tscpu_reset_thermal_MCU();

	lvts_thermal_cal_prepare();
	lvts_device_identification();
	lvts_Device_Enable_Init_all_Devices();
#if LVTS_DEVICE_AUTO_RCK == 0
	lvts_device_read_count_RC_N();
#else
	lvts_device_enable_auto_rck();
#endif
	lvts_efuse_setting();

	lvts_disable_all_sensing_points();
	lvts_tscpu_thermal_initial_all_tc();
	lvts_enable_all_sensing_points();

#if THERMAL_HW_PROTECT
	lvts_config_all_tc_hw_protect(THERMAL_HARDWARE_RESET_POINT, 60000);
#endif
	print("thermal_init: thermal initialized\n");

	return 0;
}

#if 0
int lvts_mtktscpu_get_hw_temp(void)
{
	int curr_temp, i, count = 1;

	thermal_dbg_print("===== %s begin ======\n", __func__);

//xxx, #ifdef __MTK_SLT_

//xxx, #else
	lvts_thermal_init();
	mdelay(100);
//xxx, #endif
	thermal_dbg_print("mtktscpu_get_hw_temp\n");

	for (i = 0; i < count; i++) {
		lvts_read_all_tc_temperature();
		lvts_print_mcu_temp();
		curr_temp = lvts_tscpu_max_temperature();
		thermal_dbg_print("Max temp= %d\n", curr_temp);
		mdelay(1000);
	}

	return curr_temp;
}
#endif

/*********************************************************************************/
/*********************************************************************************/
/* MT6785 */
/*
 * PTP#	module		TSMCU Plan
 *  0	MCU_LITTLE	TSMCU-5,6,7
 *  1	MCU_BIG		TSMCU-8,9
 *  2	MCU_CCI		TSMCU-5,6,7
 *  3	MFG(GPU)	TSMCU-1,2
 *  4	VPU			TSMCU-4
 *  No PTP bank 5
 *  6	TOP			TSMCU-1,2,4
 *  7	MD			TSMCU-0
 */

//=================================================
#define TMP_MAX2(ta,tb)			((ta>=tb)?(ta):(tb))
#define TMP_MAX3(ta,tb,tc)		((ta>=tb)?(TMP_MAX2(ta,tc)):(TMP_MAX2(tb,tc)))
s16 preloader_thermal_sync(u8 ts_channel)
{
	//xxx, return 0;//xxx,

	lvts_read_all_tc_temperature();
	//lvts_print_mcu_temp();
	return (lvts_tscpu_max_temperature() / 1000);

#if 0
	read_all_tc_temperature(); //ARRAY_SIZE(tscpu_g_tc) = 2, tscpu_g_tc[i].ts_number = 3 + 4 = 7
	//tscpu_thermal_read_tc_temp(1, tscpu_g_tc[1].ts[0], 0); //L_TS_MCU5
	//tscpu_thermal_read_tc_temp(1, tscpu_g_tc[1].ts[1], 1); //L_TS_MCU6
	//tscpu_thermal_read_tc_temp(1, tscpu_g_tc[1].ts[2], 2); //L_TS_MCU7
	//tscpu_thermal_read_tc_temp(2, tscpu_g_tc[2].ts[0], 0); //L_TS_MCU8
	//tscpu_thermal_read_tc_temp(2, tscpu_g_tc[2].ts[1], 1); //L_TS_MCU9

	switch (ts_channel) {
		case 0: //CPU-Little
			return TMP_MAX3(tscpu_ts_temp[TS_MCU5], tscpu_ts_temp[TS_MCU6], tscpu_ts_temp[TS_MCU7]) / 1000; //CA7L		TSMCU4+TSMCU5+TSMCU6
			break;
		case 1: //CPU-B
			return TMP_MAX2(tscpu_ts_temp[TS_MCU8], tscpu_ts_temp[TS_MCU9]) / 1000; //CA7B		TSMCU7+TSMUC8
			break;
		default: //get max
			return TMP_MAX2((TMP_MAX3(tscpu_ts_temp[TS_MCU5], tscpu_ts_temp[TS_MCU6], tscpu_ts_temp[TS_MCU7])), (TMP_MAX2(tscpu_ts_temp[TS_MCU8], tscpu_ts_temp[TS_MCU9]))) / 1000;
			break;
	}
#endif
}

//fix wait CPU-L
void preloader_thermal_wait(s16 wait_temp, u8 wait_temp_accuracy)
{
	//s16 mp0_temp, mp1_temp, max_temp;
	s16 max_temp;

	//mp0_temp = preloader_thermal_sync(0); //CPU-LL(MP0)
	//mp1_temp = preloader_thermal_sync(1); //CPU-B(MP1)
	//max_temp = TMP_MAX2(mp0_temp, mp1_temp);
	max_temp = preloader_thermal_sync(0);

	//print current temp
	//print_log("[TemperatureMonster]:CurMax = %d(C), MP0/1 = %d/%d(C), Max = %d(C).\n", max_temp, mp0_temp, mp1_temp, max_temp);
	print_log("[TemperatureMonster]:CurMax = %d(C), MP0/1 = %d/%d(C), Max = %d(C).\n", max_temp, max_temp, max_temp, max_temp);

	//over 120C meanig bypass
	if (wait_temp > 120)
		return;

	//preloader_thermal_sync(1);//sync first
	do {
		//for clear WDT tick
		platform_wdt_kick();

		//mp0_temp = preloader_thermal_sync(0); //CPU-LL(MP0)
		//mp1_temp = preloader_thermal_sync(1); //CPU-B(MP1)
		//max_temp = TMP_MAX2(mp0_temp, mp1_temp);
		max_temp = preloader_thermal_sync(0);

		//print_log("[TemperatureMonster]:CurMax = %d(C), Wait Temp/Range = %d/%d(C), MP0/1 = %d/%d(C), Max = %d(C).\n", max_temp, wait_temp, wait_temp_accuracy, mp0_temp, mp1_temp, max_temp);
		print_log("[TemperatureMonster]:CurMax = %d(C), Wait Temp/Range = %d/%d(C), MP0/1 = %d/%d(C), Max = %d(C).\n", max_temp, wait_temp, wait_temp_accuracy, max_temp, max_temp, max_temp);
		if ((max_temp > (wait_temp + wait_temp_accuracy)) || (max_temp < (wait_temp - wait_temp_accuracy))) {
			mdelay(333);
		} else {
			break;
		}
	} while (1);
}
