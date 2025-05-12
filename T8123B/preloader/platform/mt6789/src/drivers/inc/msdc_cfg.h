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

#ifndef _MSDC_CFG_H_
#define _MSDC_CFG_H_

#define MMC_MSDC_DRV_PRELOADER

/*--------------------------------------------------------------------------*/
/* FPGA Definition                                                          */
/*--------------------------------------------------------------------------*/
#if CFG_FPGA_PLATFORM
#define FPGA_PLATFORM
#else
#undef FPGA_PLATFORM
#endif

/*--------------------------------------------------------------------------*/
/* MSDC Host Number                                                         */
/*--------------------------------------------------------------------------*/

#ifndef MSDC1_EMMC
#define MSDC_MAX_NUM            (1)
#else
#define MSDC_MAX_NUM            (2)
#define MMC_HOST_ID             (1)
#endif


/*--------------------------------------------------------------------------*/
/* Transfer Mode Definition                                                 */
/*--------------------------------------------------------------------------*/
/*
  MSDC_MODE_DEFAULT_PIO
  MSDC_MODE_DEFAULT_DMA_BASIC
  MSDC_MODE_DEFAULT_DMA_DESC
  MSDC_MODE_DEFAULT_DMA_ENHANCED
*/

#ifndef _FLASH_MODE_DA_
#define MSDC_MODE_DEFAULT_PIO
#else
#define MSDC_ENABLE_DMA_MODE
#define MSDC_MODE_DEFAULT_DMA_DESC
#endif

/*--------------------------------------------------------------------------*/
/* Common Definition                                                        */
/*--------------------------------------------------------------------------*/
#ifndef _FLASH_MODE_DA_
#define FEATURE_MMC_SLIM
#else
#define FEATURE_ERASE
#endif

#if defined(FPGA_PLATFORM)
#define MSDC_USE_EMMC45_POWER     (1)
#endif

#define FEATURE_EMCP

/*--------------------------------------------------------------------------*/
/* Tuning Definition                                                        */
/*--------------------------------------------------------------------------*/
#define FEATURE_MMC_WR_TUNING
#define FEATURE_MMC_RD_TUNING
#define FEATURE_MMC_CM_TUNING

/*--------------------------------------------------------------------------*/
/* Test Definition                                                         */
/*--------------------------------------------------------------------------*/
//#define MMC_TEST

/*--------------------------------------------------------------------------*/
/* Debug Definition                                                         */
/*--------------------------------------------------------------------------*/
#define MMC_DEBUG               (0)
#define MSDC_DEBUG              (0)

#if MSDC_DEBUG
#define MSG_DEBUG
#endif

//#define MTK_MSDC_BRINGUP_DEBUG

#define MSDC_TUNE_LOG           (1)

#define MSDC_SLT                (0)
#define MSDC_SLT_MASK_LOG       (1)
#define MSDC_SLT_ANALYSIS       (0)

#if MSDC_SLT_ANALYSIS
#undef FEATURE_MMC_WR_TUNING
#undef FEATURE_MMC_RD_TUNING
#undef FEATURE_MMC_CM_TUNING
#endif

//#define USE_SDIO_1V8

#endif /* end of _MSDC_CFG_H_ */
