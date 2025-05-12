/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be "strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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

#include <typedefs.h>
#include "emi.h"
#include "dram_buffer.h"

#define bootarg g_dram_buf->bootarg

/*
 * Prevent HN-D response coherence error when set BROADCASTOUTTER=1.
 * It returns error by default.
 */
void booker_disable_nde(void)
{
    asm volatile (
        "ldr	x0, =0x0a550a00\n"
        "mov	x1, #0\n"
        "str	x1, [x0]\n"
        "dsb	ish\n"
        "isb\n"
        ::: "x0", "x1", "memory"
    );

    return;
}

/*
 * Configure Booker except MTU and disable coherence request to HN-D.
 * Notice: do not modify assembly code, just copy what DE provides.
 */
void booker_init(void)
{
    asm volatile (
        // System address map
	// RNF-0
	"ldr	x0, =0x0a4a0e00\n"		// sys_cache_grp_region0
	"ldr	x1, =0x0a00000000000005\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a4a0f00\n"		// sys_cache_grp_hn_nodeid_reg0
	"ldr	x1, =0x000000D00C009008\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a4a0ea0\n"		// sys_cache_grp_hn_count
	"ldr	x1, =0x4\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a4a0c00\n"		// non_hash_mem_region_reg0
	"ldr	x1, =0x0400000000000005\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a4a0d80\n"		// non_hash_tgt_nodeid_reg0
	"ldr	x1, =0x14\n"			// HN-D
	"str	x1, [x0]\n"

	// RNF-4
	"ldr	x0, =0x0a5a0e00\n"		// sys_cache_grp_region0
	"ldr	x1, =0x0a00000000000005\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a5a0f00\n"		// sys_cache_grp_hn_nodeid_reg0
	"ldr	x1, =0x000000D00C009008\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a5a0ea0\n"		// sys_cache_grp_hn_count
	"ldr	x1, =0x4\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a5a0c00\n"		// non_hash_mem_region_reg0
	"ldr	x1, =0x0400000000000005\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a5a0d80\n"		// non_hash_tgt_nodeid_reg0
	"ldr	x1, =0x14\n"			// HN-D
	"str	x1, [x0]\n"

	// Hash rule
	// RNF-0
	"ldr	x0, =0x0a4a1600\n"		// rnsam_generic_regs0, align your EMI hash setting
#if !CFG_FPGA_PLATFORM
#if (CFG_HASH_RULE_MP)
	"ldr	x1, =0xa00000eb\n"
#else
	"ldr	x1, =0xc0000021\n"
#endif
#else
	"ldr	x1, =0x0\n"			// disable hash on single-channel bitfile
#endif
	"str	x1, [x0]\n"

	// RN-4
	"ldr	x0, =0x0a5a1600\n"		// rnsam_generic_regs0, align your EMI hash setting
#if !CFG_FPGA_PLATFORM
#if (CFG_HASH_RULE_MP)
	"ldr	x1, =0xa00000eb\n"
#else
	"ldr	x1, =0xc0000021\n"
#endif
#else
	"ldr	x1, =0x0\n"			// disable hash on single-channel bitfile
#endif

	"str	x1, [x0]\n"

	// Enable CMO propagation
	"ldr	x0, =0x0a120a00\n"
	"ldr	x1, [x0]\n"
	"ldr	x2, =0x8\n"
	"bic	x1, x1, x2\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a160a00\n"
	"ldr	x1, [x0]\n"
	"ldr	x2, =0x8\n"
	"bic	x1, x1, x2\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a130a00\n"
	"ldr	x1, [x0]\n"
	"ldr	x2, =0x8\n"
	"bic	x1, x1, x2\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a170a00\n"
	"ldr	x1, [x0]\n"
	"ldr	x2, =0x8\n"
	"bic	x1, x1, x2\n"
	"str	x1, [x0]\n"

	// Disable early write ack and in address region 0.
	// Disable sending early read receipts from Hn-I in address region 0.
	"ldr 	x0, =0x0a550c00\n"
	"ldr	x1, =0x000000000000003f\n"
	"str	x1, [x0]\n"

	"dsb	ish\n"

	// Enable setting
	// RN-0
	"ldr	x0, =0x0a4a1100\n"		// rnsam_status
	"ldr	x1, =0x14000000000002\n"
	"str	x1, [x0]\n"

	// RN-4
	"ldr	x0, =0x0a5a1100\n"		// rnsam_status
	"ldr	x1, =0x14000000000002\n"
	"str	x1, [x0]\n"

	"dsb	ish\n"
	"isb\n"
        ::: "x0", "x1", "x2", "memory"
    );
}

void booker_post_init(unsigned long long rsv_start)
{
    asm volatile (
	"mov	x2, x0\n"			//backup x1 first

	// MTU programming
	// MTSX8
	"ldr	x0, =0x0a220a40\n"		//por_mtu_tag_addr_ctl, Map type 0 mode
	"ldr	x1, =0x4\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a220a48\n"		//por_mtu_tag_addr_base
	"mov	x1, x2\n"			//8GB-31/32GB + 1GB Peri
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a220a50\n"		//por_mtu_tag_addr_shutter0
	"ldr	x1, =0x2222222222222200\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a220a58\n"		//por_mtu_tag_addr_shutter1
	"ldr	x1, =0x2222222222222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a220a60\n"		//por_mtu_tag_addr_shutter2
	"ldr	x1, =0x0000000002222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a220a08\n"		//por_mtu_aux_ctl
	"ldr	x1, [x0]\n"
	"orr	x1, x1, 0x1\n"			//no_tc_mode
	"str	x1, [x0]\n"

	// MTSX9
	"ldr	x0, =0x0a260a40\n"		//por_mtu_tag_addr_ctl, Map type 0 mode
	"ldr	x1, =0x4\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a260a48\n"		//por_mtu_tag_addr_base
	"mov	x1, x2\n"			//8GB-31/32GB + 1GB Peri
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a260a50\n"		//por_mtu_tag_addr_shutter0
	"ldr	x1, =0x2222222222222200\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a260a58\n"		//por_mtu_tag_addr_shutter1
	"ldr	x1, =0x2222222222222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a260a60\n"		//por_mtu_tag_addr_shutter2
	"ldr	x1, =0x0000000002222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a260a08\n"		//por_mtu_aux_ctl
	"ldr	x1, [x0]\n"
	"orr	x1, x1, 0x1\n"			//no_tc_mode
	"str	x1, [x0]\n"

	// MTSX12
	"ldr	x0, =0x0a230a40\n"		//por_mtu_tag_addr_ctl, Msp type 0 mode
	"ldr	x1, =0x4\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a230a48\n"		//por_mtu_tag_addr_base
	"mov	x1, x2\n"			//8GB-31/32GB + 1GB Peri
	"str	x1, [x0]\n"
	"ldr	x0, =0x0a230a50\n"		//por_mtu_tag_addr_shutter0
	"ldr	x1, =0x2222222222222200\n"
	"str	x1, [x0]\n"
	"ldr	x0, =0x0a230a58\n"		//por_mtu_tag_addr_shutter1
	"ldr	x1, =0x2222222222222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a230a60\n"		//por_mtu_tag_addr_shutter2
	"ldr	x1, =0x0000000002222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a230a08\n"		//por_mtu_aux_ctl
	"ldr	x1, [x0]\n"
	"orr	x1, x1, 0x1\n"			//no_tc_mode
	"str	x1, [x0]\n"

	// MTSX13
	"ldr	x0, =0x0a270a40\n"		//por_mtu_tag_addr_ctl, Msp type 0 mode
	"ldr	x1, =0x4\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a270a48\n"		//por_mtu_tag_addr_base
	"mov	x1, x2\n"			//8GB-31/32GB + 1GB Peri
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a270a50\n"		//por_mtu_tag_addr_shutter0
	"ldr	x1, =0x2222222222222200\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a270a58\n"		//por_mtu_tag_addr_shutter1
	"ldr	x1, =0x2222222222222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a270a60\n"		//por_mtu_tag_addr_shutter2
	"ldr	x1, =0x0000000002222222\n"
	"str	x1, [x0]\n"

	"ldr	x0, =0x0a270a08\n"		//por_mtu_aux_ctl
	"ldr	x1, [x0]\n"
	"orr	x1, x1, 0x1\n"			//no_tc_mode
	"str	x1, [x0]\n"

	"dsb	ish\n"
	"isb\n"
	::: "x0", "x1", "x2", "memory"
    );
}

/*
 * Since we can get the DRAM size, we finally start setting the MTU.
 * (Enable MTE when ARM_MTE_SUPPORT configured in user load, and always ON in non-user load.)
 * Notice: do not modify assembly code, just copy what DE provides.
 */
void booker_config_mte(void)
{
#if ((defined(ARM_MTE_SUPPORT) && defined(TARGET_BUILD_VARIANT_USER)) || defined(TARGET_BUILD_VARIANT_ENG) || defined(TARGET_BUILD_VARIANT_USERDEBUG))
    unsigned long long dram_size, config_dram_size, rsv_start;

    dram_size = platform_memory_size();
    config_dram_size = get_config_max_dram_size();

    if (config_dram_size < dram_size)
	    dram_size = config_dram_size;

    /* we need 4 tag bits for every 16 bytes data */
    rsv_start = mblock_alloc(dram_size >> 5,
		    	     0x1000, MBLOCK_NO_LIMIT, 0x0, 0x0, "security_booker_tag");
    if (!rsv_start) {
	    pal_log_err("%s memory reserve fail, DRAM size: 0x%llx.\n", __func__, dram_size);
	    return;
    }

    /* we need to pass the address by x0, be careful of tail-call optimization */
    booker_post_init(rsv_start);
    pal_log_err("%s DRAM size: 0x%llx, rsv addr: 0x%llx.\n", __func__, dram_size, rsv_start);
#endif
}
