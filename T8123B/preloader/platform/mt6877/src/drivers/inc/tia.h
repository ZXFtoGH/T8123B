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
 * MediaTek Inc. (C) 2019. All rights reserved.
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
#ifndef __TIA_H__
#define __TIA_H__

/******************* SW ENV define *******************/
#define TIA_PRELOADER      1
#define TIA_KERNEL         0

#define TIA_SUPPORT
#define TIA_DEBUG_LOG
#define TIA_GPS_AUXADC_WORKAROUND

/******************* SW ENV header define ************/
#if (TIA_PRELOADER)
#include <typedefs.h>
#include <mt6877.h>
#include <tia_reg.h>
#include <pal_log.h>
#include <upmu_hw.h>
#else
	### Compile error, check SW ENV define
#endif

/******************* External API *******************/
extern signed int tia_init(void);

/******************* Debug Marco ********************/
#define TIATAG		"[TIA] "
#if (TIA_PRELOADER)
	#ifdef TIA_DEBUG_LOG
		#define TIA_LOG(fmt, arg...)   pal_log_info(TIATAG fmt, ##arg)
	#else
		#define TIA_LOG(fmt, arg...)
	#endif
	#define TIA_CRI(fmt, arg...)   pal_log_info(TIATAG fmt, ##arg)
	#define TIA_ERR(fmt, arg...)   pal_log_err(TIATAG fmt, ##arg)
#elif (TIA_KERNEL)
	#ifdef TIA_DEBUG_LOG
		#define TIA_LOG(fmt, arg...)   pr_info(TIATAG fmt, ##arg)
	#else
		#define TIA_LOG(fmt, arg...)
	#endif
	#define TIA_CRI(fmt, arg...)   pr_notice(TIATAG fmt, ##arg)
	#define TIA_ERR(fmt, arg...)   pr_err(TIATAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#else
	### Compile error, check SW ENV define
#endif

#endif /*__TIA_H__*/
