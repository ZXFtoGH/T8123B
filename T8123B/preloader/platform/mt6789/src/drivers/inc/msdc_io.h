/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2015. All rights reserved.
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

#ifndef _MSDC_IO_H_
#define _MSDC_IO_H_

#include "msdc.h"
#include "mmc_types.h"

#include "platform.h"
#include "pll.h"
#include "upmu_hw.h"

/*******************************************************************************
 *PINMUX and GPIO definition
 ******************************************************************************/

#define MSDC_PIN_PULL_NONE      (0)
#define MSDC_PIN_PULL_DOWN      (1)
#define MSDC_PIN_PULL_UP        (2)
#define MSDC_PIN_KEEP           (3)

/* add pull down/up mode define */
#define MSDC_GPIO_PULL_UP       (0)
#define MSDC_GPIO_PULL_DOWN     (1)

#define MSDC_TDRDSEL_SLEEP      (0)
#define MSDC_TDRDSEL_3V         (1)
#define MSDC_TDRDSEL_1V8        (2)
#define MSDC_TDRDSEL_CUST       (3)

/*--------------------------------------------------------------------------*/
/* MSDC0~3 GPIO and IO Pad Configuration Base                               */
/*--------------------------------------------------------------------------*/
#ifndef GPIO_BASE
#error definition of GPIO_BASE does not exist
#endif
#define MSDC_GPIO_BASE          GPIO_BASE

#define MSDC0_IO_PAD_BASE       (IOCFG_LM_BASE)

/*--------------------------------------------------------------------------*/
/* MSDC GPIO Related Register                                               */
/*--------------------------------------------------------------------------*/
/* MSDC0 */
#define MSDC0_GPIO_MODE6        (MSDC_GPIO_BASE + 0x360)
#define MSDC0_GPIO_MODE7        (MSDC_GPIO_BASE + 0x370)
#define MSDC0_GPIO_MODE8        (MSDC_GPIO_BASE + 0x380)
#define MSDC0_GPIO_DRV0         (MSDC0_IO_PAD_BASE + 0x20)
#define MSDC0_GPIO_DRV1         (MSDC0_IO_PAD_BASE + 0x30)
#define MSDC0_GPIO_IES          (MSDC0_IO_PAD_BASE + 0xD0)
#define MSDC0_GPIO_PUPD0        (MSDC0_IO_PAD_BASE + 0x120)
#define MSDC0_GPIO_R0           (MSDC0_IO_PAD_BASE + 0x160)
#define MSDC0_GPIO_R1           (MSDC0_IO_PAD_BASE + 0x170)
#define MSDC0_GPIO_RDSEL0       (MSDC0_IO_PAD_BASE + 0x180)
#define MSDC0_GPIO_RDSEL1       (MSDC0_IO_PAD_BASE + 0x190)
#define MSDC0_GPIO_RDSEL2       (MSDC0_IO_PAD_BASE + 0x1A0)
#define MSDC0_GPIO_SMT          (MSDC0_IO_PAD_BASE + 0x1E0)
#define MSDC0_GPIO_TDSEL0       (MSDC0_IO_PAD_BASE + 0x200)
#define MSDC0_GPIO_TDSEL1       (MSDC0_IO_PAD_BASE + 0x210)
#define MSDC0_GPIO_TDSEL2       (MSDC0_IO_PAD_BASE + 0x220)

/*
 * MSDC0 GPIO and PAD register and bitfields definition
 */
/* MSDC0_GPIO_MODE6, 001b is msdc mode*/
#define MSDC0_MODE_DSL_MASK     (0x7 << 24)
#define MSDC0_MODE_CLK_MASK     (0x7 << 28)
/* MSDC0_GPIO_MODE7, 001b is msdc mode */
#define MSDC0_MODE_CMD_MASK     (0x7 << 0)
#define MSDC0_MODE_RSTB_MASK    (0x7 << 4)
#define MSDC0_MODE_DAT0_MASK    (0x7 << 8)
#define MSDC0_MODE_DAT1_MASK    (0x7 << 12)
#define MSDC0_MODE_DAT2_MASK    (0x7 << 16)
#define MSDC0_MODE_DAT3_MASK    (0x7 << 20)
#define MSDC0_MODE_DAT4_MASK    (0x7 << 24)
#define MSDC0_MODE_DAT5_MASK    (0x7 << 28)
/* MSDC0_GPIO_MODE8, 001b is msdc mode */
#define MSDC0_MODE_DAT6_MASK    (0x7 << 0)
#define MSDC0_MODE_DAT7_MASK    (0x7 << 4)
/* MSDC0 IES mask */
#define MSDC0_IES_CLK_MASK      (0x1 << 8)
#define MSDC0_IES_CMD_MASK      (0x1 << 9)
#define MSDC0_IES_DAT_MASK      (0xFF << 10)
#define MSDC0_IES_DSL_MASK      (0x1 << 18)
#define MSDC0_IES_RSTB_MASK     (0x1 << 19)
#define MSDC0_IES_ALL_MASK      (0xFFF << 8)
/* MSDC0 SMT mask */
#define MSDC0_SMT_CLK_MASK      (0x1 << 7)
#define MSDC0_SMT_CMD_MASK      (0x1 << 8)
#define MSDC0_SMT_DAT_MASK      (0xFF << 9)
#define MSDC0_SMT_DSL_MASK      (0x1 << 17)
#define MSDC0_SMT_RTSB_MASK     (0x1 << 18)
#define MSDC0_SMT_ALL_MASK      (0xFFF << 7)
/* MSDC0 TDSEL0 mask */
#define MSDC0_TDSEL0_CLK_MASK     (0xF << 28)
#define MSDC0_TDSEL0_ALL_MASK     (0xF << 28)
/* MSDC0 TDSEL1 mask */
#define MSDC0_TDSEL1_CMD_MASK     (0xF << 0)
#define MSDC0_TDSEL1_DAT0_MASK    (0xF << 4)
#define MSDC0_TDSEL1_DAT1_MASK    (0xF << 8)
#define MSDC0_TDSEL1_DAT2_MASK    (0xF << 12)
#define MSDC0_TDSEL1_DAT3_MASK    (0xF << 16)
#define MSDC0_TDSEL1_DAT4_MASK    (0xF << 20)
#define MSDC0_TDSEL1_DAT5_MASK    (0xF << 24)
#define MSDC0_TDSEL1_DAT6_MASK    (0xF << 28)
#define MSDC0_TDSEL1_ALL_MASK     (0xFFFFFFFF << 0)
/* MSDC0 TDSEL2 mask */
#define MSDC0_TDSEL2_DAT7_MASK    (0xF << 0)
#define MSDC0_TDSEL2_DSL_MASK     (0xF << 4)
#define MSDC0_TDSEL2_RSTB_MASK    (0xF << 8)
#define MSDC0_TDSEL2_ALL_MASK     (0xFFF << 0)
/* MSDC0 RDSEL0 mask */
#define MSDC0_RDSEL0_CLK_MASK    (0x3F << 14)
#define MSDC0_RDSEL0_CMD_MASK    (0x3F << 20)
#define MSDC0_RDSEL0_DAT0_MASK   (0x3F << 26)
#define MSDC0_RDSEL0_ALL_MASK    (0x3FFFF << 14)
/* MSDC0 RDSEL1 mask */
#define MSDC0_RDSEL1_DAT1_MASK   (0x3F << 0)
#define MSDC0_RDSEL1_DAT2_MASK   (0x3F << 6)
#define MSDC0_RDSEL1_DAT3_MASK   (0x3F << 12)
#define MSDC0_RDSEL1_DAT4_MASK   (0x3F << 18)
#define MSDC0_RDSEL1_DAT5_MASK   (0x3F << 24)
#define MSDC0_RDSEL1_ALL_MASK    (0x3FFFFFFF << 0)
/* MSDC0 RDSEL2 mask */
#define MSDC0_RDSEL2_DAT6_MASK   (0x3F << 0)
#define MSDC0_RDSEL2_DAT7_MASK   (0x3F << 6)
#define MSDC0_RDSEL2_DSL_MASK    (0x3F << 12)
#define MSDC0_RDSEL2_RSTB_MASK   (0x3F << 18)
#define MSDC0_RDSEL2_ALL_MASK    (0xFFFFFF << 0)
/* MSDC0 DRV0 mask */
#define MSDC0_DRV0_CLK_MASK      (0x7 << 12)
#define MSDC0_DRV0_CMD_MASK      (0x7 << 15)
#define MSDC0_DRV0_DAT0_MASK     (0x7 << 18)
#define MSDC0_DRV0_DAT1_MASK     (0x7 << 21)
#define MSDC0_DRV0_DAT2_MASK     (0x7 << 24)
#define MSDC0_DRV0_DAT3_MASK     (0x7 << 27)
#define MSDC0_DRV0_ALL_MASK      (0x3FFFF << 12)
/* MSDC0 DRV1 mask */
#define MSDC0_DRV1_DAT4_MASK     (0x7 << 0)
#define MSDC0_DRV1_DAT5_MASK     (0x7 << 3)
#define MSDC0_DRV1_DAT6_MASK     (0x7 << 6)
#define MSDC0_DRV1_DAT7_MASK     (0x7 << 9)
#define MSDC0_DRV1_DSL_MASK      (0x7 << 12)
#define MSDC0_DRV1_RTSB_MASK     (0x7 << 15)
#define MSDC0_DRV1_ALL_MASK      (0x3FFFF << 0)
/* MSDC0 PUPD mask*/
#define MSDC0_PUPD_CLK_MASK      (0x1  << 0)
#define MSDC0_PUPD_CMD_MASK      (0x1  << 1)
#define MSDC0_PUPD_DAT0_MASK     (0x1  << 2)
#define MSDC0_PUPD_DAT1_MASK     (0x1  << 3)
#define MSDC0_PUPD_DAT2_MASK     (0x1  << 4)
#define MSDC0_PUPD_DAT3_MASK     (0x1  << 5)
#define MSDC0_PUPD_DAT4_MASK     (0x1  << 6)
#define MSDC0_PUPD_DAT5_MASK     (0x1  << 7)
#define MSDC0_PUPD_DAT6_MASK     (0x1  << 8)
#define MSDC0_PUPD_DAT7_MASK     (0x1  << 9)
#define MSDC0_PUPD_DSL_MASK      (0x1  << 10)
#define MSDC0_PUPD_RSTB_MASK     (0x1  << 11)
#define MSDC0_PUPD_DAT_MASK      (0xFF << 2)
#define MSDC0_PUPD_ALL_MASK      (0x7FF << 0)
/* MSDC0 R0 mask*/
#define MSDC0_R0_RSTB_MASK       (0x1  << 11)
#define MSDC0_R0_DSL_MASK        (0x1  << 10)
#define MSDC0_R0_DAT7_MASK       (0x1  << 9)
#define MSDC0_R0_DAT6_MASK       (0x1  << 8)
#define MSDC0_R0_DAT5_MASK       (0x1  << 7)
#define MSDC0_R0_DAT4_MASK       (0x1  << 6)
#define MSDC0_R0_DAT3_MASK       (0x1  << 5)
#define MSDC0_R0_DAT2_MASK       (0x1  << 4)
#define MSDC0_R0_DAT1_MASK       (0x1  << 3)
#define MSDC0_R0_DAT0_MASK       (0x1  << 2)
#define MSDC0_R0_CMD_MASK        (0x1  << 1)
#define MSDC0_R0_CLK_MASK        (0x1  << 0)
#define MSDC0_R0_DAT_MASK        (0xFF << 2)
#define MSDC0_R0_ALL_MASK        (0x7FF << 0)
/* MSDC0 R1 mask*/
#define MSDC0_R1_RSTB_MASK       (0x1  << 11)
#define MSDC0_R1_DSL_MASK        (0x1  << 10)
#define MSDC0_R1_DAT7_MASK       (0x1  << 9)
#define MSDC0_R1_DAT6_MASK       (0x1  << 8)
#define MSDC0_R1_DAT5_MASK       (0x1  << 7)
#define MSDC0_R1_DAT4_MASK       (0x1  << 6)
#define MSDC0_R1_DAT3_MASK       (0x1  << 5)
#define MSDC0_R1_DAT2_MASK       (0x1  << 4)
#define MSDC0_R1_DAT1_MASK       (0x1  << 3)
#define MSDC0_R1_DAT0_MASK       (0x1  << 2)
#define MSDC0_R1_CMD_MASK        (0x1  << 1)
#define MSDC0_R1_CLK_MASK        (0x1  << 0)
#define MSDC0_R1_DAT_MASK        (0xFF << 2)
#define MSDC0_R1_ALL_MASK        (0x7FF << 0)

/*
   MSDC Driving Strength Definition: specify as gear instead of driving
*/
#define MSDC_DRVN_GEAR0         0x0
#define MSDC_DRVN_GEAR1         0x1
#define MSDC_DRVN_GEAR2         0x2
#define MSDC_DRVN_GEAR3         0x3
#define MSDC_DRVN_GEAR4         0x4
#define MSDC_DRVN_GEAR5         0x5
#define MSDC_DRVN_GEAR6         0x6
#define MSDC_DRVN_GEAR7         0x7
#define MSDC_DRVN_DONT_CARE     0x0

#ifndef FPGA_PLATFORM
void msdc_set_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18);
void msdc_get_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18);
void msdc_set_ies_by_id(u32 id, int set_ies);
void msdc_set_sr_by_id(u32 id, int clk, int cmd, int dat, int rst, int ds);
void msdc_set_smt_by_id(u32 id, int set_smt);
void msdc_set_tdsel_by_id(u32 id, u32 flag, u32 value);
void msdc_set_rdsel_by_id(u32 id, u32 flag, u32 value);
void msdc_get_tdsel_by_id(u32 id, u32 *value);
void msdc_get_rdsel_by_id(u32 id, u32 *value);
void msdc_dump_clock_sts(struct mmc_host *host);
void msdc_dump_ldo_sts(struct mmc_host *host);
void msdc_dump_padctl_by_id(u32 id);
void msdc_pin_config_by_id(u32 id, u32 mode);
void msdc_set_pin_mode(struct mmc_host *host);
void msdc_set_tdsel_wrap(struct mmc_host *host);
void msdc_set_rdsel_wrap(struct mmc_host *host);
void msdc_gpio_and_pad_init(struct mmc_host *host);
void msdc_apply_timing(unsigned int id);
void msdc_lock_aes_key(void);
#endif

#ifdef FPGA_PLATFORM
#define msdc_set_driving_by_id(id, msdc_cap, sd_18)
#define msdc_get_driving_by_id(id, msdc_cap, sd_18)
#define msdc_set_ies_by_id(id, set_ies)
#define msdc_set_sr_by_id(id, clk, cmd, dat, rst, ds)
#define msdc_set_smt_by_id(id, set_smt)
#define msdc_set_tdsel_by_id(id, flag, value)
#define msdc_set_rdsel_by_id(id, flag, value)
#define msdc_get_tdsel_by_id(id, value)
#define msdc_get_rdsel_by_id(id, value)
#define msdc_dump_clock_sts(host)
#define msdc_dump_ldo_sts(host)
#define msdc_dump_padctl_by_id(id)
#define msdc_pin_config_by_id(id, mode)
#define msdc_set_pin_mode(host)
#endif

#define msdc_get_driving(host, msdc_cap, sd_18) \
	msdc_get_driving_by_id(host->id, msdc_cap, sd_18)

#define msdc_set_driving(host, msdc_cap, sd_18) \
	msdc_set_driving_by_id(host->id, msdc_cap, sd_18)

#define msdc_set_ies(host, set_ies) \
	msdc_set_ies_by_id(host->id, set_ies)

#define msdc_set_sr(host, clk, cmd, dat, rst, ds) \
	msdc_set_sr_by_id(host->id, clk, cmd, dat, rst, ds)

#define msdc_set_smt(host, set_smt) \
	msdc_set_smt_by_id(host->id, set_smt)

#define msdc_set_tdsel(host, flag, value) \
	msdc_set_tdsel_by_id(host->id, flag, value)

#define msdc_set_rdsel(host, flag, value) \
	msdc_set_rdsel_by_id(host->id, flag, value)

#define msdc_get_tdsel(host, value) \
	msdc_get_tdsel_by_id(host->id, value)

#define msdc_get_rdsel(host, value) \
	msdc_get_rdsel_by_id(host->id, value)

#define msdc_dump_padctl(host) \
	msdc_dump_padctl_by_id(host->id)

#define msdc_pin_config(host, mode) \
	msdc_pin_config_by_id(host->id, mode)


/*******************************************************************************
 * CLOCK definition
 ******************************************************************************/
#if defined(FPGA_PLATFORM)
#define MSDC_OP_SCLK            (10000000)
#define MSDC_SRC_CLK            (10000000)
#define MSDC_MAX_SCLK           (MSDC_SRC_CLK>>1)
#define MSDC_MIN_SCLK           (100000)
#else
#define MSDC_OP_SCLK            (200000000)
#define MSDC_MAX_SCLK           (200000000)
#define MSDC_MIN_SCLK           (260000)
#endif

/* each PLL have different gears for select
 * software can used mux interface from clock management module to select */
enum {
	MSDC0_CLKSRC_26MHZ  = 0,
	MSDC0_CLKSRC_400MHZ,
	MSDC0_CLKSRC_200MHZ,
	MSDC0_CLKSRC_156MHZ,
	MSDC0_CLKSRC_182MHZ,
	MSDC0_CLKSRC_312MHZ,
	MSDC0_CLKSRC_MAX
};

enum {
	MSDC1_CLKSRC_26MHZ   = 0,
	MSDC1_CLKSRC_208MHZ,
	MSDC1_CLKSRC_182MHZ,
	MSDC1_CLKSRC_156MHZ,
	MSDC1_CLKSRC_200MHZ,
	MSDC1_CLKSRC_MAX
};

#define MSDC0_CLKSRC_DEFAULT    MSDC0_CLKSRC_400MHZ
#define MSDC1_CLKSRC_DEFAULT    MSDC1_CLKSRC_200MHZ
#define MSDC_CLKSRC_DEFAULT(ID) MSDC##ID_CLKSRC_DEFAULT

#define MSDCPLL_FREQ            384000000
//#define MSDCPLL_FREQ            416000000
#define MSDC0_SRC_0             26000000
#define MSDC0_SRC_1             MSDCPLL_FREQ
#define MSDC0_SRC_2             (MSDCPLL_FREQ/2)
#define MSDC0_SRC_3             156000000
#define MSDC0_SRC_4             182000000
#define MSDC0_SRC_5             312000000

#define MSDC1_SRC_0             26000000
#define MSDC1_SRC_1             208000000
#define MSDC1_SRC_2             182000000
#define MSDC1_SRC_3             156000000
#define MSDC1_SRC_4             (MSDCPLL_FREQ/2)

void msdc_clock(struct mmc_host *host, int on);
void msdc_config_clksrc(struct mmc_host *host, int clksrc);

#if defined(FPGA_PLATFORM)
#define msdc_get_hclk(host, src)        MSDC_SRC_CLK
#else
extern u32 hclks_msdc0[];
extern u32 hclks_msdc1[];
extern u32 *hclks_msdc_all[];
#define msdc_get_hclk(id, src)          hclks_msdc_all[id][src]
#endif

/*set cg bit*/
#define MSDC0_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x088)
#define MSDC1_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x088)
/*clr cg bit*/
#define MSDC0_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x08c)
#define MSDC1_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x08c)

#define MSDC0_CLOCK_CG          (1 << 2)
#define MSDC0_CLOCK_SRC_CG      (1 << 6)
#define MSDC1_CLOCK_CG          (1 << 4)
#define MSDC1_CLOCK_SRC_CG      (1 << 16)


/*******************************************************************************
 * Power Definition
 ******************************************************************************/
typedef enum MSDC_POWER_VOL_TAG {
	VOL_DEFAULT,
	VOL_0900 = 900,
	VOL_1000 = 1000,
	VOL_1100 = 1100,
	VOL_1200 = 1200,
	VOL_1300 = 1300,
	VOL_1350 = 1350,
	VOL_1500 = 1500,
	VOL_1800 = 1800,
	VOL_2000 = 2000,
	VOL_2100 = 2100,
	VOL_2500 = 2500,
	VOL_2800 = 2800,
	VOL_2900 = 2900,
	VOL_3000 = 3000,
	VOL_3300 = 3300,
	VOL_3400 = 3400,
	VOL_3500 = 3500,
	VOL_3600 = 3600
} MSDC_POWER_VOLTAGE;

#if defined(MMC_MSDC_DRV_CTP) && !defined(FPGA_PLATFORM)
typedef enum MSDC_POWER_TAG {
	MSDC_VMC,
	MSDC_VMCH,
	MSDC_VEMC,
} MSDC_POWER;
#endif

#define REG_VEMC_VOSEL_CAL      PMIC_RG_VEMC_VOCAL_0_ADDR
#define MASK_VEMC_VOSEL_CAL     PMIC_RG_VEMC_VOCAL_0_MASK
#define SHIFT_VEMC_VOSEL_CAL    PMIC_RG_VEMC_VOCAL_0_SHIFT
#define FIELD_VEMC_VOSEL_CAL    (MASK_VEMC_VOSEL_CAL \
					<< SHIFT_VEMC_VOSEL_CAL)

#define REG_VEMC_VOSEL          PMIC_RG_VEMC_VOSEL_0_ADDR
#define MASK_VEMC_VOSEL         PMIC_RG_VEMC_VOSEL_0_MASK
#define SHIFT_VEMC_VOSEL        PMIC_RG_VEMC_VOSEL_0_SHIFT
#define FIELD_VEMC_VOSEL        (MASK_VEMC_VOSEL << SHIFT_VEMC_VOSEL)

#define REG_VEMC_EN             PMIC_RG_LDO_VEMC_EN_ADDR
#define MASK_VEMC_EN            PMIC_RG_LDO_VEMC_EN_MASK
#define SHIFT_VEMC_EN           PMIC_RG_LDO_VEMC_EN_SHIFT
#define FIELD_VEMC_EN           (MASK_VEMC_EN << SHIFT_VEMC_EN)

#define REG_VMC_VOSEL_CAL       PMIC_RG_VMC_VOCAL_ADDR
#define MASK_VMC_VOSEL_CAL      PMIC_RG_VMC_VOCAL_MASK
#define SHIFT_VMC_VOSEL_CAL     PMIC_RG_VMC_VOCAL_SHIFT
#define FIELD_VMC_VOSEL_CAL     (MASK_VMC_VOSEL_CAL \
					<< SHIFT_VMC_VOSEL_CAL)

#define REG_VMC_VOSEL           PMIC_RG_VMC_VOSEL_ADDR
#define MASK_VMC_VOSEL          PMIC_RG_VMC_VOSEL_MASK
#define SHIFT_VMC_VOSEL         PMIC_RG_VMC_VOSEL_SHIFT
#define FIELD_VMC_VOSEL         (MASK_VMC_VOSEL << SHIFT_VMC_VOSEL)

#define REG_VMC_EN              PMIC_RG_LDO_VMC_EN_ADDR
#define MASK_VMC_EN             PMIC_RG_LDO_VMC_EN_MASK
#define SHIFT_VMC_EN            PMIC_RG_LDO_VMC_EN_SHIFT
#define FIELD_VMC_EN            (MASK_VMC_EN << SHIFT_VMC_EN)

#define REG_VMCH_VOSEL_CAL      PMIC_RG_VMCH_VOCAL_ADDR
#define MASK_VMCH_VOSEL_CAL     PMIC_RG_VMCH_VOCAL_MASK
#define SHIFT_VMCH_VOSEL_CAL    PMIC_RG_VMCH_VOCAL_SHIFT
#define FIELD_VMCH_VOSEL_CAL    (MASK_VMCH_VOSEL_CAL \
					<< SHIFT_VMCH_VOSEL_CAL)

#define REG_VMCH_VOSEL          PMIC_RG_VMCH_VOSEL_ADDR
#define MASK_VMCH_VOSEL         PMIC_RG_VMCH_VOSEL_MASK
#define SHIFT_VMCH_VOSEL        PMIC_RG_VMCH_VOSEL_SHIFT
#define FIELD_VMCH_VOSEL        (MASK_VMCH_VOSEL << SHIFT_VMCH_VOSEL)

#define REG_VMCH_EN             PMIC_RG_LDO_VMCH_EN_ADDR
#define MASK_VMCH_EN            PMIC_RG_LDO_VMCH_EN_MASK
#define SHIFT_VMCH_EN           PMIC_RG_LDO_VMCH_EN_SHIFT
#define FIELD_VMCH_EN           (MASK_VMCH_EN << SHIFT_VMCH_EN)

#define VEMC_VOSEL_CAL_mV(cal)  ((cal >= 0) ? ((cal)/10) : 0)
#define VEMC_VOSEL_2V9          (10)
#define VEMC_VOSEL_3V           (11)
#define VEMC_VOSEL_3V3          (13)
#define VMC_VOSEL_CAL_mV(cal)   ((cal >= 0) ? ((cal)/10) : 0)
#define VMC_VOSEL_1V8           (4) /* Acutally 1V86 */
#define VMC_VOSEL_2V9           (10)
#define VMC_VOSEL_3V            (11)
#define VMC_VOSEL_3V3           (13)
#define VMCH_VOSEL_CAL_mV(cal)  ((cal >= 0) ? ((cal)/10) : 0)
#define VMCH_VOSEL_2V9          (2)
#define VMCH_VOSEL_3V           (3)
#define VMCH_VOSEL_3V3          (5)

#define EMMC_VOL_ACTUAL         VOL_3000
#define SD_VOL_ACTUAL           VOL_3000


/*******************************************************************************
 * Power Definition
 ******************************************************************************/
#ifndef MSDC1_EMMC
#define MSDC_PB0_DEFAULT        0x403C0007
#define MSDC_PB1_DEFAULT        0xFFE64309
#define MSDC_PB2_DEFAULT        0x14881803
#else
#define MSDC_PB0_DEFAULT        0x403C0007
#define MSDC_PB1_DEFAULT        0xFFE64309
#define MSDC_PB2_DEFAULT        0x14881803
#endif

#define CMD_RSP_TA_CNTR_DEFAULT         0
#define WRDAT_CRCS_TA_CNTR_DEFAULT      0
#define BUSY_MA_DEFAULT                 1

#define CRCSTSENSEL_HS400_DEFAULT       3
#define RESPSTENSEL_HS400_DEFAULT       3
#define CRCSTSENSEL_HS_DEFAULT          1
#define RESPSTENSEL_HS_DEFAULT          1
#define CRCSTSENSEL_FPGA_DEFAULT        0

#endif /* end of _MSDC_IO_H_ */

