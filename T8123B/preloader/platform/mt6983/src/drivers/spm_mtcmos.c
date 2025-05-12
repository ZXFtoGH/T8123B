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
 
#include <platform.h>
#include <spm_mtcmos.h>
#include <spm_mtcmos_internal.h>
#include <timer.h>  //udelay
#include <pll.h>

#if CFG_FPGA_PLATFORM
#define IGNORE_MTCMOS_CHECK	1 //for FPGA
#endif

#define POWERON_CONFIG_EN		(SPM_BASE + 0x000)
#define DIS0_PWR_CON				(SPM_BASE + 0xE64)
#define DIS1_PWR_CON				(SPM_BASE + 0xE68)
#define MM_INFRA_PWR_CON		(SPM_BASE + 0xE6C)
#define MFG0_PWR_CON				(SPM_BASE + 0xEB8)
#define MFG1_PWR_CON				(SPM_BASE + 0xEBC)
#define SOC_BUCK_ISO_CON		(SPM_BASE + 0xF30)
#define SPM2GPUPM_CON				(SPM_BASE + 0x410)
#define ADSP_AO_PWR_CON			(SPM_BASE + 0xE20)
#define ADSP_INFRA_PWR_CON	(SPM_BASE + 0xE1C)
#define AUDIO_PWR_CON				(SPM_BASE + 0xE14)

#define ISP_VCORE_PWR_CON				(SPM_BASE + 0xE30)
#define ISP_MAIN_PWR_CON				(SPM_BASE + 0xE24)
#define ISP_DIP1_PWR_CON				(SPM_BASE + 0xE28)
#define ISP_IPE_PWR_CON					(SPM_BASE + 0xE2C)
#define CAM_VCORE_PWR_CON				(SPM_BASE + 0xE58)
#define CAM_MAIN_PWR_CON				(SPM_BASE + 0xE44)
#define CAM_MRAW_PWR_CON				(SPM_BASE + 0xE48)
#define CAM_SUBA_PWR_CON				(SPM_BASE + 0xE4C)
#define CAM_SUBB_PWR_CON				(SPM_BASE + 0xE50)
#define CAM_SUBC_PWR_CON				(SPM_BASE + 0xE54)


/* Define MTCMOS power control */
#define PWR_RST_B                        (0x1 << 0)
#define PWR_ISO                          (0x1 << 1)
#define PWR_ON                           (0x1 << 2)
#define PWR_ON_2ND                       (0x1 << 3)
#define PWR_CLK_DIS                      (0x1 << 4)
#define SRAM_CKISO                       (0x1 << 5)
#define SRAM_ISOINT_B                    (0x1 << 6)
#define SRAM_PDN                         (0x1 << 8)
#define SRAM_SLEEP_B                     (0x1 << 9)
#define SRAM_PDN_S                       (0x1 << 10)
#define SRAM_PDN_ACK                     (0x1 << 12)
#define SRAM_SLEEP_B_ACK                 (0x1 << 13)
#define SRAM_PDN_ACK_S                   (0x1 << 14)
#define PWR_ACK                          (0x1 << 30)
#define PWR_ACK_2ND                      (0x1U << 31)

/* Define MTCMOS Bus Protect Mask */
#define MFG0_PROT_STEP1_0_MASK           ((0x1 << 4))
#define MFG0_PROT_STEP1_0_ACK_MASK       ((0x1 << 4))
#define MFG0_PROT_STEP2_0_MASK           ((0x1 << 9))
#define MFG0_PROT_STEP2_0_ACK_MASK       ((0x1 << 9))
#define MFG1_PROT_STEP1_0_MASK           ((0x1 << 0))
#define MFG1_PROT_STEP1_0_ACK_MASK       ((0x1 << 0))
#define MFG1_PROT_STEP2_0_MASK           ((0x1 << 1))
#define MFG1_PROT_STEP2_0_ACK_MASK       ((0x1 << 1))
#define MFG1_PROT_STEP3_0_MASK           ((0x1 << 2))
#define MFG1_PROT_STEP3_0_ACK_MASK       ((0x1 << 2))
#define MFG1_PROT_STEP4_0_MASK           ((0x1 << 3))
#define MFG1_PROT_STEP4_0_ACK_MASK       ((0x1 << 3))
#define MM_INFRA_PROT_STEP1_0_MASK       ((0x1 << 11))
#define MM_INFRA_PROT_STEP1_0_ACK_MASK   ((0x1 << 11))
#define MM_INFRA_PROT_STEP2_0_MASK       ((0x1 << 14) | (0x1 << 15))
#define MM_INFRA_PROT_STEP2_0_ACK_MASK   ((0x1 << 14) | (0x1 << 15))
#define MM_INFRA_PROT_STEP4_0_MASK       ((0x1 << 15) | (0x1 << 17) | (0x1 << 25))
#define MM_INFRA_PROT_STEP4_0_ACK_MASK   ((0x1 << 15) | (0x1 << 17) | (0x1 << 25))
#define MM_INFRA_PROT_STEP4_1_MASK       ((0x1 << 19) | (0x1 << 29))
#define MM_INFRA_PROT_STEP4_1_ACK_MASK   ((0x1 << 19) | (0x1 << 29))
#define ADSP_INFRA_PROT_STEP1_0_MASK     ((0x1 << 8))
#define ADSP_INFRA_PROT_STEP1_0_ACK_MASK   ((0x1 << 8))
#define ADSP_INFRA_PROT_STEP2_0_MASK     ((0x1 << 4) | (0x1 << 5))
#define ADSP_INFRA_PROT_STEP2_0_ACK_MASK   ((0x1 << 4) | (0x1 << 5))
#define DIS0_PROT_STEP1_0_MASK           ((0x1 << 8) | (0x1 << 30))
#define DIS0_PROT_STEP1_0_ACK_MASK       ((0x1 << 8) | (0x1 << 30))
#define DIS0_PROT_STEP1_1_MASK           ((0x1 << 10))
#define DIS0_PROT_STEP1_1_ACK_MASK       ((0x1 << 10))
#define DIS0_PROT_STEP2_0_MASK           ((0x1 << 9) | (0x1U << 31))
#define DIS0_PROT_STEP2_0_ACK_MASK       ((0x1 << 9) | (0x1U << 31))
#define DIS0_PROT_STEP2_1_MASK           ((0x1 << 11))
#define DIS0_PROT_STEP2_1_ACK_MASK       ((0x1 << 11))
#define DIS1_PROT_STEP1_0_MASK           ((0x1 << 0) | (0x1 << 2) | (0x1 << 12))
#define DIS1_PROT_STEP1_0_ACK_MASK       ((0x1 << 0) | (0x1 << 2) | (0x1 << 12))
#define DIS1_PROT_STEP2_0_MASK           ((0x1 << 1) | (0x1 << 3) | (0x1 << 13))
#define DIS1_PROT_STEP2_0_ACK_MASK       ((0x1 << 1) | (0x1 << 3) | (0x1 << 13))

/* Define MTCMOS Power Status Mask */


/* Define Non-CPU SRAM Mask */
#define DIS0_SRAM_PDN                    (0x1 << 8)
#define DIS0_SRAM_PDN_ACK                (0x1 << 12)
#define DIS0_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define DIS1_SRAM_PDN                    (0x1 << 8)
#define DIS1_SRAM_PDN_ACK                (0x1 << 12)
#define DIS1_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define MM_INFRA_SRAM_PDN                (0x1 << 8)
#define MM_INFRA_SRAM_PDN_ACK            (0x1 << 12)
#define MM_INFRA_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define MFG0_SRAM_PDN                    (0x1 << 8)
#define MFG0_SRAM_PDN_ACK                (0x1 << 12)
#define MFG0_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define MFG0_SRAM_SLEEP_B                (0x1 << 9)
#define MFG0_SRAM_SLEEP_B_ACK            (0x1 << 13)
#define MFG0_SRAM_SLEEP_B_ACK_BIT0       (0x1 << 13)
#define MFG1_SRAM_PDN                    (0x1 << 8)
#define MFG1_SRAM_PDN_ACK                (0x1 << 12)
#define MFG1_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define AUDIO_SRAM_PDN                   (0x1 << 8)
#define AUDIO_SRAM_PDN_ACK               (0x1 << 12)
#define AUDIO_SRAM_PDN_ACK_BIT0          (0x1 << 12)
#define ADSP_TOP_SRAM_PDN                (0x1 << 8)
#define ADSP_TOP_SRAM_PDN_ACK            (0x1 << 12)
#define ADSP_TOP_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define ADSP_TOP_SRAM_SLEEP_B            (0x1 << 9)
#define ADSP_TOP_SRAM_SLEEP_B_ACK        (0x1 << 13)
#define ADSP_TOP_SRAM_SLEEP_B_ACK_BIT0   (0x1 << 13)
#define ADSP_INFRA_SRAM_PDN              (0x1 << 8)
#define ADSP_INFRA_SRAM_PDN_ACK          (0x1 << 12)
#define ADSP_INFRA_SRAM_PDN_ACK_BIT0     (0x1 << 12)
#define ADSP_INFRA_SRAM_SLEEP_B          (0x1 << 9)
#define ADSP_INFRA_SRAM_SLEEP_B_ACK      (0x1 << 13)
#define ADSP_INFRA_SRAM_SLEEP_B_ACK_BIT0   (0x1 << 13)
#define ADSP_AO_SRAM_PDN                 (0x1 << 8)
#define ADSP_AO_SRAM_PDN_ACK             (0x1 << 12)
#define ADSP_AO_SRAM_PDN_ACK_BIT0        (0x1 << 12)
#define ADSP_AO_SRAM_SLEEP_B             (0x1 << 9)
#define ADSP_AO_SRAM_SLEEP_B_ACK         (0x1 << 13)
#define ADSP_AO_SRAM_SLEEP_B_ACK_BIT0    (0x1 << 13)
#define ISP_MAIN_SRAM_PDN                (0x1 << 8)
#define ISP_MAIN_SRAM_PDN_ACK            (0x1 << 12)
#define ISP_MAIN_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define ISP_DIP1_SRAM_PDN                (0x1 << 8)
#define ISP_DIP1_SRAM_PDN_ACK            (0x1 << 12)
#define ISP_DIP1_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define ISP_IPE_SRAM_PDN                 (0x1 << 8)
#define ISP_IPE_SRAM_PDN_ACK             (0x1 << 12)
#define ISP_IPE_SRAM_PDN_ACK_BIT0        (0x1 << 12)
#define VDE0_SRAM_PDN                    (0x1 << 8)
#define VDE0_SRAM_PDN_ACK                (0x1 << 12)
#define VDE0_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define VDE1_SRAM_PDN                    (0x1 << 8)
#define VDE1_SRAM_PDN_ACK                (0x1 << 12)
#define VDE1_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define VEN0_SRAM_PDN                    (0x1 << 8)
#define VEN0_SRAM_PDN_ACK                (0x1 << 12)
#define VEN0_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define VEN1_SRAM_PDN                    (0x1 << 8)
#define VEN1_SRAM_PDN_ACK                (0x1 << 12)
#define VEN1_SRAM_PDN_ACK_BIT0           (0x1 << 12)
#define CAM_MAIN_SRAM_PDN                (0x1 << 8)
#define CAM_MAIN_SRAM_PDN_ACK            (0x1 << 12)
#define CAM_MAIN_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define CAM_MRAW_SRAM_PDN                (0x1 << 8)
#define CAM_MRAW_SRAM_PDN_ACK            (0x1 << 12)
#define CAM_MRAW_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define CAM_SUBA_SRAM_PDN                (0x1 << 8)
#define CAM_SUBA_SRAM_PDN_ACK            (0x1 << 12)
#define CAM_SUBA_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define CAM_SUBB_SRAM_PDN                (0x1 << 8)
#define CAM_SUBB_SRAM_PDN_ACK            (0x1 << 12)
#define CAM_SUBB_SRAM_PDN_ACK_BIT0       (0x1 << 12)
#define CAM_SUBC_SRAM_PDN                (0x1 << 8)
#define CAM_SUBC_SRAM_PDN_ACK            (0x1 << 12)
#define CAM_SUBC_SRAM_PDN_ACK_BIT0       (0x1 << 12)

/**************************************
 * for non-CPU MTCMOS BUS PROTECT
 **************************************/
 int spm_mtcmos_ctrl_dis0_bus_prot(int state) {
  int err = 0;
  
  if(state == SET_BUS_PROTECT) {
  } else {
    /* TINFO="Release bus protect" */
    spm_write(MMSYS_PROTECT_EN_CLR_0, DIS0_PROT_STEP2_0_MASK);
    spm_write(MMSYS_PROTECT_EN_CLR_1, DIS0_PROT_STEP2_1_MASK);
    spm_write(MMSYS_PROTECT_EN_CLR_0, DIS0_PROT_STEP1_0_MASK);
    spm_write(MMSYS_PROTECT_EN_CLR_1, DIS0_PROT_STEP1_1_MASK);
  }
  return err;
}

 int spm_mtcmos_ctrl_dis1_bus_prot(int state) {
  int err = 0;
  
  if(state == SET_BUS_PROTECT) {
  } else {
    /* TINFO="Release bus protect" */
    spm_write(MMSYS_PROTECT_EN_CLR_1, DIS1_PROT_STEP2_0_MASK);
    spm_write(MMSYS_PROTECT_EN_CLR_1, DIS1_PROT_STEP1_0_MASK);
  }
  return err;
}

int spm_mtcmos_ctrl_adsp_infra_bus_prot(int state) {
  int err = 0;
  
  if(state == SET_BUS_PROTECT) {
    /* TINFO="Set bus protect - step1 : 0" */
    spm_write(INFRASYS_PROTECT_EN_SET_1, ADSP_INFRA_PROT_STEP1_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(INFRASYS_PROTECT_RDY_STA_1) & ADSP_INFRA_PROT_STEP1_0_ACK_MASK) != ADSP_INFRA_PROT_STEP1_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step2 : 0" */
    spm_write(INFRASYS_PROTECT_EN_SET_0, ADSP_INFRA_PROT_STEP2_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(INFRASYS_PROTECT_RDY_STA_0) & ADSP_INFRA_PROT_STEP2_0_ACK_MASK) != ADSP_INFRA_PROT_STEP2_0_ACK_MASK) {
    }
  #endif
  } else {
    /* TINFO="Release bus protect" */
    spm_write(INFRASYS_PROTECT_EN_CLR_0, ADSP_INFRA_PROT_STEP2_0_MASK);
    spm_write(INFRASYS_PROTECT_EN_CLR_1, ADSP_INFRA_PROT_STEP1_0_MASK);
  }
  return err;
} 

int spm_mtcmos_ctrl_mfg0_bus_prot(int state) {
  int err = 0;
  
  if(state == SET_BUS_PROTECT) {
    /* TINFO="Set bus protect - step1 : 0" */
    spm_write(MD_MFGSYS_PROTECT_EN_SET_0, MFG0_PROT_STEP1_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(MD_MFGSYS_PROTECT_RDY_STA_0) & MFG0_PROT_STEP1_0_ACK_MASK) != MFG0_PROT_STEP1_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step2 : 0" */
    spm_write(INFRASYS_PROTECT_EN_SET_0, MFG0_PROT_STEP2_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(INFRASYS_PROTECT_RDY_STA_0) & MFG0_PROT_STEP2_0_ACK_MASK) != MFG0_PROT_STEP2_0_ACK_MASK) {
    }
  #endif
  } else {
    /* TINFO="Release bus protect" */
    spm_write(INFRASYS_PROTECT_EN_CLR_0, MFG0_PROT_STEP2_0_MASK);
    spm_write(MD_MFGSYS_PROTECT_EN_CLR_0, MFG0_PROT_STEP1_0_MASK);
  }
  return err;
}

int spm_mtcmos_ctrl_mfg1_bus_prot(int state) {
  int err = 0;
  
  if(state == SET_BUS_PROTECT) {
    /* TINFO="Set bus protect - step1 : 0" */
    spm_write(MD_MFGSYS_PROTECT_EN_SET_0, MFG1_PROT_STEP1_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(MD_MFGSYS_PROTECT_RDY_STA_0) & MFG1_PROT_STEP1_0_ACK_MASK) != MFG1_PROT_STEP1_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step2 : 0" */
    spm_write(MD_MFGSYS_PROTECT_EN_SET_0, MFG1_PROT_STEP2_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(MD_MFGSYS_PROTECT_RDY_STA_0) & MFG1_PROT_STEP2_0_ACK_MASK) != MFG1_PROT_STEP2_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step3 : 0" */
    spm_write(MD_MFGSYS_PROTECT_EN_SET_0, MFG1_PROT_STEP3_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(MD_MFGSYS_PROTECT_RDY_STA_0) & MFG1_PROT_STEP3_0_ACK_MASK) != MFG1_PROT_STEP3_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step4 : 0" */
    spm_write(MD_MFGSYS_PROTECT_EN_SET_0, MFG1_PROT_STEP4_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(MD_MFGSYS_PROTECT_RDY_STA_0) & MFG1_PROT_STEP4_0_ACK_MASK) != MFG1_PROT_STEP4_0_ACK_MASK) {
    }
  #endif
  } else {
    /* TINFO="Release bus protect" */
    spm_write(MD_MFGSYS_PROTECT_EN_CLR_0, MFG1_PROT_STEP4_0_MASK);
    spm_write(MD_MFGSYS_PROTECT_EN_CLR_0, MFG1_PROT_STEP3_0_MASK);
    spm_write(MD_MFGSYS_PROTECT_EN_CLR_0, MFG1_PROT_STEP2_0_MASK);
    spm_write(MD_MFGSYS_PROTECT_EN_CLR_0, MFG1_PROT_STEP1_0_MASK);
  }
  return err;
}

int spm_mtcmos_ctrl_mm_infra_bus_prot(int state) {
  int err = 0;
  
  if(state == SET_BUS_PROTECT) {
    /* TINFO="Set bus protect - step1 : 0" */
    spm_write(INFRASYS_PROTECT_EN_SET_1, MM_INFRA_PROT_STEP1_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(INFRASYS_PROTECT_RDY_STA_1) & MM_INFRA_PROT_STEP1_0_ACK_MASK) != MM_INFRA_PROT_STEP1_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step2 : 0" */
    spm_write(INFRASYS_PROTECT_EN_SET_0, MM_INFRA_PROT_STEP2_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(INFRASYS_PROTECT_RDY_STA_0) & MM_INFRA_PROT_STEP2_0_ACK_MASK) != MM_INFRA_PROT_STEP2_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step4 : 0" */
    spm_write(MMSYS_PROTECT_EN_SET_0, MM_INFRA_PROT_STEP4_0_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(MMSYS_PROTECT_RDY_STA_0) & MM_INFRA_PROT_STEP4_0_ACK_MASK) != MM_INFRA_PROT_STEP4_0_ACK_MASK) {
    }
  #endif
    /* TINFO="Set bus protect - step4 : 1" */
    spm_write(MMSYS_PROTECT_EN_SET_1, MM_INFRA_PROT_STEP4_1_MASK);
  #ifndef IGNORE_MTCMOS_CHECK
    while ((spm_read(MMSYS_PROTECT_RDY_STA_1) & MM_INFRA_PROT_STEP4_1_ACK_MASK) != MM_INFRA_PROT_STEP4_1_ACK_MASK) {
    }
  #endif
  } else {
    /* TINFO="Release bus protect" */
    spm_write(MMSYS_PROTECT_EN_CLR_0, MM_INFRA_PROT_STEP4_0_MASK);
    spm_write(MMSYS_PROTECT_EN_CLR_1, MM_INFRA_PROT_STEP4_1_MASK);
    spm_write(INFRASYS_PROTECT_EN_CLR_0, MM_INFRA_PROT_STEP2_0_MASK);
    spm_write(INFRASYS_PROTECT_EN_CLR_1, MM_INFRA_PROT_STEP1_0_MASK);
  }
  return err;
}
/**************************************
 * for non-CPU MTCMOS
 **************************************/
int spm_mtcmos_ctrl_mfg0_shut_down(int state) {
  int err = 0;

  spm_write(0x1C001000, 0x0B160001);
  if(state == STA_POWER_DOWN) {
    /* TINFO="Start to turn off MFG0" */
    spm_mtcmos_ctrl_mfg0_bus_prot(SET_BUS_PROTECT);
    /* TINFO="Finish to set MFG0 bus protect" */
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG0_SRAM_PDN_ACK = 1" */
    while ((spm_read(MFG0_PWR_CON) & MFG0_SRAM_PDN_ACK) != MFG0_SRAM_PDN_ACK) {
      /* Need f_fmfg_core_ck for SRAM PDN delay IP. */
    }
  #endif
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_ISO);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_CLK_DIS);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_RST_B);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_ON);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG0_PWR_CON = 0 and MFG0_PWR_CON = 0" */
    while((spm_read(MFG0_PWR_CON) & PWR_ACK) ||
          (spm_read(MFG0_PWR_CON) & PWR_ACK_2ND)) {
      /* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
    }
  #endif
    /* TINFO="Finish to turn off MFG0" */
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on MFG0" */
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_ON);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG0_PWR_CON = 1 and MFG0_PWR_CON = 1" */
    while(((spm_read(MFG0_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(MFG0_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
      /* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
    }
  #endif
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_ISO);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_RST_B);
    spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG0_SRAM_PDN_ACK = 0" */
    while(spm_read(MFG0_PWR_CON) & MFG0_SRAM_PDN_ACK) {
      /* Need f_fmfg_core_ck for SRAM PDN delay IP. */
    }
  #endif
    spm_mtcmos_ctrl_mfg0_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release MFG0 bus protect" */
    /* TINFO="Finish to turn on MFG0" */
  }
  return err;
}

int spm_mtcmos_ctrl_mfg1(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
    /* TINFO="Start to turn off MFG1" */
    spm_mtcmos_ctrl_mfg1_bus_prot(SET_BUS_PROTECT);
    /* TINFO="Finish to set MFG1 bus protect" */
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG1_SRAM_PDN_ACK = 1" */
    while ((spm_read(MFG1_PWR_CON) & MFG1_SRAM_PDN_ACK) != MFG1_SRAM_PDN_ACK) {
    }
  #endif
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_ISO);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_CLK_DIS);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_RST_B);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_ON);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG1_PWR_CON = 0 and MFG1_PWR_CON = 0" */
    while((spm_read(MFG1_PWR_CON) & PWR_ACK) ||
          (spm_read(MFG1_PWR_CON) & PWR_ACK_2ND)) {
      /* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
    }
  #endif
    /* TINFO="Finish to turn off MFG1" */
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on MFG1" */
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_ON);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG1_PWR_CON = 1 and MFG1_PWR_CON = 1" */
    while(((spm_read(MFG1_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(MFG1_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
      /* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
    }
  #endif
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_ISO);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_RST_B);
    spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MFG1_SRAM_PDN_ACK = 0" */
    while(spm_read(MFG1_PWR_CON) & MFG1_SRAM_PDN_ACK) {
    }
  #endif
    spm_mtcmos_ctrl_mfg1_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release MFG1 bus protect" */
    /* TINFO="Finish to turn on MFG1" */
  }
  return err;
}

int spm_mtcmos_ctrl_mm_infra(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
    /* TINFO="Start to turn off MM_INFRA" */
    spm_mtcmos_ctrl_mm_infra_bus_prot(SET_BUS_PROTECT);
    /* TINFO="Finish to set MM_INFRA bus protect" */
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) | SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MM_INFRA_SRAM_PDN_ACK = 1" */
    while ((spm_read(MM_INFRA_PWR_CON) & MM_INFRA_SRAM_PDN_ACK) != MM_INFRA_SRAM_PDN_ACK) {
    }
  #endif
    /* TINFO="SOC_BUCK_ISO_CON[18]=1"*/
    spm_write(SOC_BUCK_ISO_CON, spm_read(SOC_BUCK_ISO_CON) | (0x1 << 18));//LATCHEN
    /* TINFO="SOC_BUCK_ISO_CON[17]=1"*/
    spm_write(SOC_BUCK_ISO_CON, spm_read(SOC_BUCK_ISO_CON) | (0x1 << 17));


    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) | PWR_ISO);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) | PWR_CLK_DIS);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) & ~PWR_RST_B);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) & ~PWR_ON);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) & ~PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MM_INFRA_PWR_CON = 0 and MM_INFRA_PWR_CON = 0" */
    while((spm_read(MM_INFRA_PWR_CON) & PWR_ACK) ||
          (spm_read(MM_INFRA_PWR_CON) & PWR_ACK_2ND)) {
    }
  #endif
    /* TINFO="SOC_BUCK_ISO_CON[16]=1"*/
    spm_write(SOC_BUCK_ISO_CON, spm_read(SOC_BUCK_ISO_CON) | (0x1 << 16));
    /* TINFO="Finish to turn off MM_INFRA" */
  } else {    
    /* STA_POWER_ON */
    /* TINFO="SOC_BUCK_ISO_CON[16]=0"*/
    spm_write(SOC_BUCK_ISO_CON, spm_read(SOC_BUCK_ISO_CON) & ~(0x1 << 16));

    /* TINFO="Start to turn on MM_INFRA" */
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) | PWR_ON);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MM_INFRA_PWR_CON = 1 and MM_INFRA_PWR_CON = 1" */
    while(((spm_read(MM_INFRA_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(MM_INFRA_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) & ~PWR_ISO);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) | PWR_RST_B);
    spm_write(MM_INFRA_PWR_CON, spm_read(MM_INFRA_PWR_CON) & ~SRAM_PDN);
    /* TINFO="SOC_BUCK_ISO_CON[17]=0"*/
    spm_write(SOC_BUCK_ISO_CON, spm_read(SOC_BUCK_ISO_CON) & ~(0x1 << 17));
    /* TINFO="SOC_BUCK_ISO_CON[18]=0"*/
    spm_write(SOC_BUCK_ISO_CON, spm_read(SOC_BUCK_ISO_CON) & ~(0x1 << 18));

  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until MM_INFRA_SRAM_PDN_ACK = 0" */
    while(spm_read(MM_INFRA_PWR_CON) & MM_INFRA_SRAM_PDN_ACK) {
    }
  #endif
    spm_mtcmos_ctrl_mm_infra_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release MM_INFRA bus protect" */
    /* TINFO="Finish to turn on MM_INFRA" */
  }
  return err;
}

int spm_mtcmos_ctrl_dis0(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on DIS0" */
    spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_ON);
    spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until DIS0_PWR_CON = 1 and DIS0_PWR_CON = 1" */
    while(((spm_read(DIS0_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(DIS0_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~PWR_ISO);
    spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_RST_B);
    spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until DIS0_SRAM_PDN_ACK = 0" */
    while(spm_read(DIS0_PWR_CON) & DIS0_SRAM_PDN_ACK) {
    }
  #endif
    spm_mtcmos_ctrl_dis0_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release DIS0 bus protect" */
    /* TINFO="Finish to turn on DIS0" */
  }
  return err;
}

int spm_mtcmos_ctrl_adsp_ao(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
    /* TINFO="Start to turn off ADSP_AO" */
    //spm_mtcmos_ctrl_adsp_ao_bus_prot(SET_BUS_PROTECT);
    /* TINFO="Finish to set ADSP_AO bus protect" */
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) | SRAM_PDN);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) | PWR_ISO);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) | PWR_CLK_DIS);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) & ~PWR_RST_B);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) & ~PWR_ON);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) & ~PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ADSP_AO_PWR_CON = 0 and ADSP_AO_PWR_CON = 0" */
    while((spm_read(ADSP_AO_PWR_CON) & PWR_ACK) ||
          (spm_read(ADSP_AO_PWR_CON) & PWR_ACK_2ND)) {
    }
  #endif
    /* TINFO="Finish to turn off ADSP_AO" */
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on ADSP_AO" */
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) | PWR_ON);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ADSP_AO_PWR_CON = 1 and ADSP_AO_PWR_CON = 1" */
    while(((spm_read(ADSP_AO_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(ADSP_AO_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) & ~PWR_ISO);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) | PWR_RST_B);
    spm_write(ADSP_AO_PWR_CON, spm_read(ADSP_AO_PWR_CON) & ~SRAM_PDN);
    //spm_mtcmos_ctrl_adsp_ao_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release ADSP_AO bus protect" */
    /* TINFO="Finish to turn on ADSP_AO" */
  }
  return err;
}

int spm_mtcmos_ctrl_adsp_infra(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
    /* TINFO="Start to turn off ADSP_INFRA" */
    spm_mtcmos_ctrl_adsp_infra_bus_prot(SET_BUS_PROTECT);
    /* TINFO="Finish to set ADSP_INFRA bus protect" */
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) | SRAM_PDN);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) | PWR_ISO);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) | PWR_CLK_DIS);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) & ~PWR_RST_B);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) & ~PWR_ON);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) & ~PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ADSP_INFRA_PWR_CON = 0 and ADSP_INFRA_PWR_CON = 0" */
    while((spm_read(ADSP_INFRA_PWR_CON) & PWR_ACK) ||
          (spm_read(ADSP_INFRA_PWR_CON) & PWR_ACK_2ND)) {
    }
  #endif
    /* TINFO="Finish to turn off ADSP_INFRA" */
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on ADSP_INFRA" */
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) | PWR_ON);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ADSP_INFRA_PWR_CON = 1 and ADSP_INFRA_PWR_CON = 1" */
    while(((spm_read(ADSP_INFRA_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(ADSP_INFRA_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) & ~PWR_ISO);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) | PWR_RST_B);
    spm_write(ADSP_INFRA_PWR_CON, spm_read(ADSP_INFRA_PWR_CON) & ~SRAM_PDN);
    spm_mtcmos_ctrl_adsp_infra_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release ADSP_INFRA bus protect" */
    /* TINFO="Finish to turn on ADSP_INFRA" */
  }
  return err;
}

int spm_mtcmos_ctrl_audio(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
    /* TINFO="Start to turn off AUDIO" */
    //spm_mtcmos_ctrl_audio_bus_prot(SET_BUS_PROTECT);
    /* TINFO="Finish to set AUDIO bus protect" */
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until AUDIO_SRAM_PDN_ACK = 1" */
    while ((spm_read(AUDIO_PWR_CON) & AUDIO_SRAM_PDN_ACK) != AUDIO_SRAM_PDN_ACK) {
    }
  #endif
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_ISO);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_CLK_DIS);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_RST_B);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_ON);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until AUDIO_PWR_CON = 0 and AUDIO_PWR_CON = 0" */
    while((spm_read(AUDIO_PWR_CON) & PWR_ACK) ||
          (spm_read(AUDIO_PWR_CON) & PWR_ACK_2ND)) {
      /* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
    }
  #endif
    /* TINFO="Finish to turn off AUDIO" */
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on AUDIO" */
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_ON);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until AUDIO_PWR_CON = 1 and AUDIO_PWR_CON = 1" */
    while(((spm_read(AUDIO_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(AUDIO_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
      /* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
    }
  #endif
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~PWR_ISO);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) | PWR_RST_B);
    spm_write(AUDIO_PWR_CON, spm_read(AUDIO_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until AUDIO_SRAM_PDN_ACK = 0" */
    while(spm_read(AUDIO_PWR_CON) & AUDIO_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_audio_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release AUDIO bus protect" */
    /* TINFO="Finish to turn on AUDIO" */
  }
  return err;
}

int spm_mtcmos_ctrl_isp_vcore(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on ISP_VCORE" */
    spm_write(ISP_VCORE_PWR_CON, spm_read(ISP_VCORE_PWR_CON) | PWR_ON);
    spm_write(ISP_VCORE_PWR_CON, spm_read(ISP_VCORE_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ISP_VCORE_PWR_CON = 1 and ISP_VCORE_PWR_CON = 1" */
    while(((spm_read(ISP_VCORE_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(ISP_VCORE_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(ISP_VCORE_PWR_CON, spm_read(ISP_VCORE_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(ISP_VCORE_PWR_CON, spm_read(ISP_VCORE_PWR_CON) & ~PWR_ISO);
    spm_write(ISP_VCORE_PWR_CON, spm_read(ISP_VCORE_PWR_CON) | PWR_RST_B);
    spm_write(ISP_VCORE_PWR_CON, spm_read(ISP_VCORE_PWR_CON) & ~SRAM_PDN);
    //spm_mtcmos_ctrl_isp_vcore_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release ISP_VCORE bus protect" */
    /* TINFO="Finish to turn on ISP_VCORE" */
  }
  return err;
}

int spm_mtcmos_ctrl_isp_main(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on ISP_MAIN" */
    spm_write(ISP_MAIN_PWR_CON, spm_read(ISP_MAIN_PWR_CON) | PWR_ON);
    spm_write(ISP_MAIN_PWR_CON, spm_read(ISP_MAIN_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ISP_MAIN_PWR_CON = 1 and ISP_MAIN_PWR_CON = 1" */
    while(((spm_read(ISP_MAIN_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(ISP_MAIN_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(ISP_MAIN_PWR_CON, spm_read(ISP_MAIN_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(ISP_MAIN_PWR_CON, spm_read(ISP_MAIN_PWR_CON) & ~PWR_ISO);
    spm_write(ISP_MAIN_PWR_CON, spm_read(ISP_MAIN_PWR_CON) | PWR_RST_B);
    spm_write(ISP_MAIN_PWR_CON, spm_read(ISP_MAIN_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ISP_MAIN_SRAM_PDN_ACK = 0" */
    while(spm_read(ISP_MAIN_PWR_CON) & ISP_MAIN_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_isp_main_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release ISP_MAIN bus protect" */
    /* TINFO="Finish to turn on ISP_MAIN" */
  }
  return err;
}


int spm_mtcmos_ctrl_isp_dip1(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on ISP_DIP1" */
    spm_write(ISP_DIP1_PWR_CON, spm_read(ISP_DIP1_PWR_CON) | PWR_ON);
    spm_write(ISP_DIP1_PWR_CON, spm_read(ISP_DIP1_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ISP_DIP1_PWR_CON = 1 and ISP_DIP1_PWR_CON = 1" */
    while(((spm_read(ISP_DIP1_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(ISP_DIP1_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(ISP_DIP1_PWR_CON, spm_read(ISP_DIP1_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(ISP_DIP1_PWR_CON, spm_read(ISP_DIP1_PWR_CON) & ~PWR_ISO);
    spm_write(ISP_DIP1_PWR_CON, spm_read(ISP_DIP1_PWR_CON) | PWR_RST_B);
    spm_write(ISP_DIP1_PWR_CON, spm_read(ISP_DIP1_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ISP_DIP1_SRAM_PDN_ACK = 0" */
    while(spm_read(ISP_DIP1_PWR_CON) & ISP_DIP1_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_isp_dip1_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release ISP_DIP1 bus protect" */
    /* TINFO="Finish to turn on ISP_DIP1" */
  }
  return err;
}

int spm_mtcmos_ctrl_isp_ipe(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on ISP_IPE" */
    spm_write(ISP_IPE_PWR_CON, spm_read(ISP_IPE_PWR_CON) | PWR_ON);
    spm_write(ISP_IPE_PWR_CON, spm_read(ISP_IPE_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ISP_IPE_PWR_CON = 1 and ISP_IPE_PWR_CON = 1" */
    while(((spm_read(ISP_IPE_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(ISP_IPE_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(ISP_IPE_PWR_CON, spm_read(ISP_IPE_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(ISP_IPE_PWR_CON, spm_read(ISP_IPE_PWR_CON) & ~PWR_ISO);
    spm_write(ISP_IPE_PWR_CON, spm_read(ISP_IPE_PWR_CON) | PWR_RST_B);
    spm_write(ISP_IPE_PWR_CON, spm_read(ISP_IPE_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until ISP_IPE_SRAM_PDN_ACK = 0" */
    while(spm_read(ISP_IPE_PWR_CON) & ISP_IPE_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_isp_ipe_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release ISP_IPE bus protect" */
    /* TINFO="Finish to turn on ISP_IPE" */
  }
  return err;
}

int spm_mtcmos_ctrl_cam_vcore(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on CAM_VCORE" */
    spm_write(CAM_VCORE_PWR_CON, spm_read(CAM_VCORE_PWR_CON) | PWR_ON);
    spm_write(CAM_VCORE_PWR_CON, spm_read(CAM_VCORE_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_VCORE_PWR_CON = 1 and CAM_VCORE_PWR_CON = 1" */
    while(((spm_read(CAM_VCORE_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(CAM_VCORE_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(CAM_VCORE_PWR_CON, spm_read(CAM_VCORE_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(CAM_VCORE_PWR_CON, spm_read(CAM_VCORE_PWR_CON) & ~PWR_ISO);
    spm_write(CAM_VCORE_PWR_CON, spm_read(CAM_VCORE_PWR_CON) | PWR_RST_B);
    spm_write(CAM_VCORE_PWR_CON, spm_read(CAM_VCORE_PWR_CON) & ~SRAM_PDN);
    //spm_mtcmos_ctrl_cam_vcore_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release CAM_VCORE bus protect" */
    /* TINFO="Finish to turn on CAM_VCORE" */
  }
  return err;
}

int spm_mtcmos_ctrl_cam_main(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on CAM_MAIN" */
    spm_write(CAM_MAIN_PWR_CON, spm_read(CAM_MAIN_PWR_CON) | PWR_ON);
    spm_write(CAM_MAIN_PWR_CON, spm_read(CAM_MAIN_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_MAIN_PWR_CON = 1 and CAM_MAIN_PWR_CON = 1" */
    while(((spm_read(CAM_MAIN_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(CAM_MAIN_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(CAM_MAIN_PWR_CON, spm_read(CAM_MAIN_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(CAM_MAIN_PWR_CON, spm_read(CAM_MAIN_PWR_CON) & ~PWR_ISO);
    spm_write(CAM_MAIN_PWR_CON, spm_read(CAM_MAIN_PWR_CON) | PWR_RST_B);
    spm_write(CAM_MAIN_PWR_CON, spm_read(CAM_MAIN_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_MAIN_SRAM_PDN_ACK = 0" */
    while(spm_read(CAM_MAIN_PWR_CON) & CAM_MAIN_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_cam_main_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release CAM_MAIN bus protect" */
    /* TINFO="Finish to turn on CAM_MAIN" */
  }
  return err;
}

int spm_mtcmos_ctrl_cam_mraw(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on CAM_MRAW" */
    spm_write(CAM_MRAW_PWR_CON, spm_read(CAM_MRAW_PWR_CON) | PWR_ON);
    spm_write(CAM_MRAW_PWR_CON, spm_read(CAM_MRAW_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_MRAW_PWR_CON = 1 and CAM_MRAW_PWR_CON = 1" */
    while(((spm_read(CAM_MRAW_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(CAM_MRAW_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(CAM_MRAW_PWR_CON, spm_read(CAM_MRAW_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(CAM_MRAW_PWR_CON, spm_read(CAM_MRAW_PWR_CON) & ~PWR_ISO);
    spm_write(CAM_MRAW_PWR_CON, spm_read(CAM_MRAW_PWR_CON) | PWR_RST_B);
    spm_write(CAM_MRAW_PWR_CON, spm_read(CAM_MRAW_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_MRAW_SRAM_PDN_ACK = 0" */
    while(spm_read(CAM_MRAW_PWR_CON) & CAM_MRAW_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_cam_mraw_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release CAM_MRAW bus protect" */
    /* TINFO="Finish to turn on CAM_MRAW" */
  }
  return err;
}

int spm_mtcmos_ctrl_cam_suba(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on CAM_SUBA" */
    spm_write(CAM_SUBA_PWR_CON, spm_read(CAM_SUBA_PWR_CON) | PWR_ON);
    spm_write(CAM_SUBA_PWR_CON, spm_read(CAM_SUBA_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_SUBA_PWR_CON = 1 and CAM_SUBA_PWR_CON = 1" */
    while(((spm_read(CAM_SUBA_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(CAM_SUBA_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(CAM_SUBA_PWR_CON, spm_read(CAM_SUBA_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(CAM_SUBA_PWR_CON, spm_read(CAM_SUBA_PWR_CON) & ~PWR_ISO);
    spm_write(CAM_SUBA_PWR_CON, spm_read(CAM_SUBA_PWR_CON) | PWR_RST_B);
    spm_write(CAM_SUBA_PWR_CON, spm_read(CAM_SUBA_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_SUBA_SRAM_PDN_ACK = 0" */
    while(spm_read(CAM_SUBA_PWR_CON) & CAM_SUBA_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_cam_suba_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release CAM_SUBA bus protect" */
    /* TINFO="Finish to turn on CAM_SUBA" */
  }
  return err;
}

int spm_mtcmos_ctrl_cam_subb(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on CAM_SUBB" */
    spm_write(CAM_SUBB_PWR_CON, spm_read(CAM_SUBB_PWR_CON) | PWR_ON);
    spm_write(CAM_SUBB_PWR_CON, spm_read(CAM_SUBB_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_SUBB_PWR_CON = 1 and CAM_SUBB_PWR_CON = 1" */
    while(((spm_read(CAM_SUBB_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(CAM_SUBB_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(CAM_SUBB_PWR_CON, spm_read(CAM_SUBB_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(CAM_SUBB_PWR_CON, spm_read(CAM_SUBB_PWR_CON) & ~PWR_ISO);
    spm_write(CAM_SUBB_PWR_CON, spm_read(CAM_SUBB_PWR_CON) | PWR_RST_B);
    spm_write(CAM_SUBB_PWR_CON, spm_read(CAM_SUBB_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_SUBB_SRAM_PDN_ACK = 0" */
    while(spm_read(CAM_SUBB_PWR_CON) & CAM_SUBB_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_cam_subb_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release CAM_SUBB bus protect" */
    /* TINFO="Finish to turn on CAM_SUBB" */
  }
  return err;
}

int spm_mtcmos_ctrl_cam_subc(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on CAM_SUBC" */
    spm_write(CAM_SUBC_PWR_CON, spm_read(CAM_SUBC_PWR_CON) | PWR_ON);
    spm_write(CAM_SUBC_PWR_CON, spm_read(CAM_SUBC_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_SUBC_PWR_CON = 1 and CAM_SUBC_PWR_CON = 1" */
    while(((spm_read(CAM_SUBC_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(CAM_SUBC_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(CAM_SUBC_PWR_CON, spm_read(CAM_SUBC_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(CAM_SUBC_PWR_CON, spm_read(CAM_SUBC_PWR_CON) & ~PWR_ISO);
    spm_write(CAM_SUBC_PWR_CON, spm_read(CAM_SUBC_PWR_CON) | PWR_RST_B);
    spm_write(CAM_SUBC_PWR_CON, spm_read(CAM_SUBC_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until CAM_SUBC_SRAM_PDN_ACK = 0" */
    while(spm_read(CAM_SUBC_PWR_CON) & CAM_SUBC_SRAM_PDN_ACK) {
    }
  #endif
    //spm_mtcmos_ctrl_cam_subc_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release CAM_SUBC bus protect" */
    /* TINFO="Finish to turn on CAM_SUBC" */
  }
  return err;
}

int spm_mtcmos_ctrl_dis1(int state) {
  int err = 0;

  if(state == STA_POWER_DOWN) {
  } else {    
    /* STA_POWER_ON */
    /* TINFO="Start to turn on DIS1" */
    spm_write(DIS1_PWR_CON, spm_read(DIS1_PWR_CON) | PWR_ON);
    spm_write(DIS1_PWR_CON, spm_read(DIS1_PWR_CON) | PWR_ON_2ND);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until DIS1_PWR_CON = 1 and DIS1_PWR_CON = 1" */
    while(((spm_read(DIS1_PWR_CON) & PWR_ACK) != PWR_ACK) ||
          ((spm_read(DIS1_PWR_CON) & PWR_ACK_2ND) != PWR_ACK_2ND)) {
    }
  #endif
    spm_write(DIS1_PWR_CON, spm_read(DIS1_PWR_CON) & ~PWR_CLK_DIS);
    spm_write(DIS1_PWR_CON, spm_read(DIS1_PWR_CON) & ~PWR_ISO);
    spm_write(DIS1_PWR_CON, spm_read(DIS1_PWR_CON) | PWR_RST_B);
    spm_write(DIS1_PWR_CON, spm_read(DIS1_PWR_CON) & ~SRAM_PDN);
  #ifndef IGNORE_MTCMOS_CHECK
    /* TINFO="Wait until DIS1_SRAM_PDN_ACK = 0" */
    while(spm_read(DIS1_PWR_CON) & DIS1_SRAM_PDN_ACK) {
    }
  #endif
    spm_mtcmos_ctrl_dis1_bus_prot(RELEASE_BUS_PROTECT);
    /* TINFO="Finish to release DIS1 bus protect" */
    /* TINFO="Finish to turn on DIS1" */
  }
  return err;
}
