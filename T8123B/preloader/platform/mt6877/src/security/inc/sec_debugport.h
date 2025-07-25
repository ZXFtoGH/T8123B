/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2019. All rights reserved.
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
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
* applicable license agreements with MediaTek Inc.
*/

#ifndef SEC_DEBUGPORT_H
#define SEC_DEBUGPORT_H

#include <pal_typedefs.h>
#include <typedefs.h>

/******************************************************************************
 * FPGA PORTING
 ******************************************************************************/
//#define SEC_FPGA_EARLY_PORTING
/******************************************************************************
 * DBGPORT configuration
 ******************************************************************************/
#define DBGPORT_HW_JTAG_DIS     0x0001
#define DBGPORT_SW_JTAG_CON     0x0002
#define DBGPORT_BROM_LOCK_DIS   0x0004

#define C_MAX_DBGPORT_STATUS_NUM 20
typedef struct {
    uint32_t sta[C_MAX_DBGPORT_STATUS_NUM];
} DBGPORT_STATUS;

#define DCU_EN_0    0x0
#define DCU_EN_1    0x1
#define DCU_EN_2    0x2

#define MCU_DFD     0x08000000 //DCU_EN_0
#define GPU_DFD     0x00100000 //DCU_EN_1
#define SOC_DFD     0x00400000 //DCU_EN_1

/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/

/* debug port control */
uint32_t seclib_dbgport_read_hw_config(void);
bool seclib_jtag_brom_lock_disabled(void);
bool seclib_sw_jtag_enabled(void);
bool seclib_hw_jtag_disabled(void);
void jtag_lock(uint32_t field, uint32_t lock_bit);
uint32_t seclib_dbgport_config(uint32_t field, uint32_t bit_config, uint32_t affect_bit);
void seclib_dbgport_set(void);
void seclib_dbgport_lock(void);
void seclib_dbgport_enable(bool bSecDebugEnable, bool bLock);
void seclib_dbgport_disable(bool bLock);
uint32_t seclib_dbgport_status(DBGPORT_STATUS *status);
uint32_t seclib_dbgport_dump(void);

#endif /* SEC_DEBUGPORT_H */

