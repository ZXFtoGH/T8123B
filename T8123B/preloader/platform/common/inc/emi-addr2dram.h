/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */

/* MediaTek Inc. (C) 2017. All rights reserved.
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

#if !defined(_EMI_ADDR2DRAM_H)
#define _EMI_ADDR2DRAM_H

#include <pal_typedefs.h>

struct emi_addr_map {
	int emi;
	int channel;
	int rank;
	int bank;
	int row;
	int column;
};

/*
 * emi_addr2dram_hash: set the by-platform hash rule for addr2dram
 * @_hash: the hash rule of EMI
 */
extern void emi_addr2dram_hash(int _hash);

/*
 * emi_addr2dram_disph: set the by-platform dispatch rule for addr2dram
 * @_disph: the dispatch rule of EMI
 */
extern void emi_addr2dram_disph(int _disph);

/*
 * emi_addr2dram_con_base: set the base address of EMI_CONx registers
 *                         for addr2dram
 * @base: base address to set
 */
extern void emi_addr2dram_con_base(unsigned int base);

/*
 * emi_addr2dram_con_offset: set the offset of an EMI_CONx register
 *                           for addr2dram
 * @index: 0 - cona, 1 - conf, 2 - conh, 3 - conh_2nd, 4 - conk
 * @offset: offset of the register
 *
 * Return 0 on success, return -1 on failures.
 */
extern int emi_addr2dram_con_offset(int index, int offset);

/*
 * mtk_addr2dram - Translate a physical address to a DRAM-point-of-view map
 * @addr - input physical address
 * @map - output map stored in struct emi_addr_map
 *
 * Return 0 on success, -1 on failures.
 *
 * There is no code comment for the translation. This is intended since
 * the fomular of translation is derived from the implementation of EMI.
 */
extern int emi_addr2dram(uint64_t addr, struct emi_addr_map *map);

/*
 * emi_addr2dram_chn_con_base: set the base address of EMI_CHN_CONx registers
 *                             for addr2dram
 * @base: base address to set
 */
extern void emi_addr2dram_chn_con_base(unsigned int base);

/*
 * emi_addr2dram_chn_con_offset: set the offset of an EMI_CHN_CONx register
 *                               for addr2dram
 * @index: 0 - cona, 1 - conc, 2 - conc_2nd
 * @offset: offset of the register
 *
 * Return 0 on success, return -1 on failures.
 */
extern int emi_addr2dram_chn_con_offset(int index, int offset);

#endif
