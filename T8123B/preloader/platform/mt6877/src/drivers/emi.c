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

#if !__ETT__
#include <emi_mpu_mt.h>
#include <typedefs.h>
#include <dram_buffer.h>
#define bootarg g_dram_buf->bootarg
#else
#include <barriers.h>
#endif
#include <emi-addr2dram.h>
#include <emi_hw.h>
#include <emi.h>

#ifdef LAST_EMI
static LAST_EMI_INFO_T* last_emi_info_ptr;
#endif

static inline unsigned int mt_emi_sync_read(unsigned int addr)
{
	dsb();
	return *((volatile unsigned int *)addr);
}

#define mt_emi_sync_write(addr, value)				\
	do {							\
		*((volatile unsigned int *)(addr)) = value;	\
		dsb();						\
	} while (0)

#define mt_emi_sync_write_or(addr, or_value)			\
	do {							\
		mt_emi_sync_write(addr,				\
			mt_emi_sync_read(addr) | or_value);	\
	} while (0)

static void emi_cen_config(unsigned cfg_base_addr) {
   unsigned EMI_APB_BASE = cfg_base_addr;

  if (mt_get_dram_type()== TYPE_LPDDR4X) {
   mt_emi_sync_write(EMI_APB_BASE+0x00000000,0x03530154); // 2CH config, row 17bit, per RANK 2GB, total 8GB

   // overhead: 20190821 item1 - synced
   mt_emi_sync_write(EMI_APB_BASE+0x00000004,0x182e2d33); //3733 (1:8) r4  - r1  overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x00000008,0x0f251025); //3733 (1:8) r8  - r5  overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x0000000c,0x122a1027); //3733 (1:8) r12 - r9  overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x00000010,0x1a31162d); //3733 (1:8) r16 - r13 overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x000008b0,0x182e2d33); //3200 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000008b4,0x0f251025); //3200 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000001c,0x122a1027); //3200 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000024,0x1a31162d); //3200 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000034,0x1024202c); //2400 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000006c,0x0b210c21); //2400 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000013c,0x0f250d23); //2400 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000640,0x152b1228); //2400 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000044,0x0c201a28); //1866 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000074,0x0d230a20); //1866 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001e0,0x0e260d24); //1866 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000644,0x132d1229); //1866 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000004c,0x0c201a28); //1600 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000084,0x0d230a20); //1600 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001e4,0x0e260d24); //1600 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000648,0x132d1229); //1600 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000054,0x0c201a28); //1200 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000008c,0x0d230a20); //1200 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001e8,0x0e260d24); //1200 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000064c,0x132d1229); //1200 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000005c,0x0e290e28); //800  (1:4) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000094,0x091e1322); //800  (1:4) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001c8,0x0f29112a); //800  (1:4) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000660,0x0c240a1f); //800  (1:4) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000064,0x0e290e28); //800  (1:4) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000009c,0x091e1322); //800  (1:4) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001f4,0x0f29112a); //800  (1:4) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000664,0x0c240a1f); //800  (1:4) r8  - r5  overhead

   mt_emi_sync_write(EMI_APB_BASE+0x00000030,0x37373a57); //3733 (1:8) r8  - r2  non-align overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x00000014,0x3f3f3c39); //3733 (1:8) r16 - r10 non-align overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x000008b8,0x3836374e); //3200 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000002c,0x41413d3a); //3200 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000c4,0x33313241); //2400 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000668,0x3a3a3835); //2400 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000c8,0x34343542); //1866 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000066c,0x3b3b3835); //1866 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000cc,0x34343542); //1600 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000694,0x3b3b3835); //1600 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000e4,0x34343542); //1200 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000708,0x3b3b3835); //1200 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000f4,0x37333034); //800  (1:4) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000070c,0x39393a39); //800  (1:4) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000012c,0x37333034); //800  (1:4) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000748,0x39393a39); //800  (1:4) r16 - r10 non-align overhead


   mt_emi_sync_write(EMI_APB_BASE+0x00000018,0x3657587a);
   mt_emi_sync_write(EMI_APB_BASE+0x00000020,0x0000C042); //sync to mt6873 #15, async_mode_use_neg_clk[15]=0
   mt_emi_sync_write(EMI_APB_BASE+0x00000028,0x00000000); // disable CEN_EMI scramble

   mt_emi_sync_write(EMI_APB_BASE+0x00000038,0x000000b3); // 2CH config, row 17bit, per RANK 2GB, total 8GB

   mt_emi_sync_write(EMI_APB_BASE+0x0000003c,0x00073200);  //[4]64Byte_Mode_en = 0 for bring-up
   mt_emi_sync_write(EMI_APB_BASE+0x00000040,0x00008802);
   mt_emi_sync_write(EMI_APB_BASE+0x00000048,0x00000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000060,0x007812ff); // reserved buffer to normal read/write :8/7
   mt_emi_sync_write(EMI_APB_BASE+0x00000068,0x02000000);  //FAST_PASS_slv_en[19:12]=0x0 (M0/M1 disable), bit[25] must be 1
   mt_emi_sync_write(EMI_APB_BASE+0x00000078,0x11120c1f);  //22:20=ultra_w=1
   mt_emi_sync_write(EMI_APB_BASE+0x00000710,0x11120c1f);  //22:20=ultra_w=1
   mt_emi_sync_write(EMI_APB_BASE+0x0000007c,0x00001123);
   mt_emi_sync_write(EMI_APB_BASE+0x00000718,0x00001123);
   mt_emi_sync_write(EMI_APB_BASE+0x000000d0,0xa8a8a8a8); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000d4,0x25252525); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000d8,0xa8a8a8a8); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000dc,0x25252525); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000e8,0x00060037); // initial starvation counter div2, [4]=1
   mt_emi_sync_write(EMI_APB_BASE+0x000000f0,0x384a0014);
   mt_emi_sync_write(EMI_APB_BASE+0x000000f8,0xa0000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000100,0x20107244);
   mt_emi_sync_write(EMI_APB_BASE+0x00000108,0x10107044);
   mt_emi_sync_write(EMI_APB_BASE+0x00000110,0x343450df);
   mt_emi_sync_write(EMI_APB_BASE+0x00000118,0x0000f0d0);
   mt_emi_sync_write(EMI_APB_BASE+0x00000120,0x10106048);
   mt_emi_sync_write(EMI_APB_BASE+0x00000128,0x343450df);
   mt_emi_sync_write(EMI_APB_BASE+0x00000130,0x83837044);
   mt_emi_sync_write(EMI_APB_BASE+0x00000138,0x83837044);
   mt_emi_sync_write(EMI_APB_BASE+0x00000140,0x00007108);
   mt_emi_sync_write(EMI_APB_BASE+0x00000144,0x00007108);
   mt_emi_sync_write(EMI_APB_BASE+0x00000150,0x090a4000); //FAST_PASS_os_limit[11:8]=0x0
   mt_emi_sync_write(EMI_APB_BASE+0x00000158,0xff0bff00);
   mt_emi_sync_write(EMI_APB_BASE+0x00000400,0x00ff0001); //[27:20] enable monitor
   mt_emi_sync_write(EMI_APB_BASE+0x0000071c,0x10000008);
   mt_emi_sync_write(EMI_APB_BASE+0x00000800,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000820,0x24240101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000824,0x01012424);
   mt_emi_sync_write(EMI_APB_BASE+0x00000828,0x50500101);
   mt_emi_sync_write(EMI_APB_BASE+0x0000082c,0x01015050);
   mt_emi_sync_write(EMI_APB_BASE+0x00000830,0x0fc39a30); // [6] MD_HRT_URGENT_MASK, if 1 -> mask MD_HRT_URGENT,
   mt_emi_sync_write(EMI_APB_BASE+0x00000834,0x05050003);
   mt_emi_sync_write(EMI_APB_BASE+0x00000838,0x254dffff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000083c,0x465a788c); //update
   mt_emi_sync_write(EMI_APB_BASE+0x00000840,0x000003e8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000844,0x0000036b);
   mt_emi_sync_write(EMI_APB_BASE+0x00000848,0x00000290);
   mt_emi_sync_write(EMI_APB_BASE+0x0000084c,0x00000200);
   mt_emi_sync_write(EMI_APB_BASE+0x00000850,0x00000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000854,0x00000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000858,0x02531cff); //ignore rff threshold
   mt_emi_sync_write(EMI_APB_BASE+0x0000085c,0x00002785); //disable internal MD latency urgent mask
   mt_emi_sync_write(EMI_APB_BASE+0x00000874,0x000001b5);
   mt_emi_sync_write(EMI_APB_BASE+0x00000878,0x003c0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x0000087c,0x0255250d);
   mt_emi_sync_write(EMI_APB_BASE+0x00000890,0xffff3c59);
   mt_emi_sync_write(EMI_APB_BASE+0x00000894,0xffff00ff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008a0,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008a4,0x0000ffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008c0,0x0000014b);
   mt_emi_sync_write(EMI_APB_BASE+0x000008c4,0x002d0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x000008c8,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x000008cc,0x003c0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x000008d0,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x000008d4,0x003c0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x000008e0,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008e4,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008e8,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000920,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000924,0x0000ffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000930,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000934,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000938,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000009f0,0x41547082);
   mt_emi_sync_write(EMI_APB_BASE+0x000009f4,0x38382a38);
   mt_emi_sync_write(EMI_APB_BASE+0x000009f8,0x000001d4);
   mt_emi_sync_write(EMI_APB_BASE+0x000009fc,0x00000190);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b00,0x0000012c);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b04,0x000000ed);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b08,0x000000c8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b0c,0x00000096);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b10,0x000000c8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b14,0x000000c8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b28,0x26304048);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b2c,0x20201820);

   mt_emi_sync_write(EMI_APB_BASE+0x00000b60,0x181e282f);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b64,0x14140f18);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b98,0x7496c8ea);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b9c,0x64644b64);
   mt_emi_sync_write(EMI_APB_BASE+0x00000bd0,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000bd4,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c08,0x7496c8ea);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c0c,0x64644b64);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c40,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c44,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c4c,0x300ff025);       //ignore wff threshold
   mt_emi_sync_write(EMI_APB_BASE+0x00000c80,0x000003e8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c84,0x0000036b);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c88,0x00000290);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c8c,0x00000200);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c90,0x000001b5);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c94,0x0000014b);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c98,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c9c,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cb0,0x52698ca0);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cb4,0x46463546);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cf8,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cfc,0x01010101);

   mt_emi_sync_write(EMI_APB_BASE+0x00000d04,0x00000009); //MDR shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d0c,0x00000000); //MDR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d14,0x00730000); //MDR shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d18,0x00000808); //MDR shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d1c,0x00000028); //MDW shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d24,0x00000000); //MDW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d2c,0x00730000); //MDW shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d30,0x00000808); //MDW shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d34,0x00000080); //APR shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d3c,0x00000000); //APR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d44,0x30201008); //APR shf0/shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d48,0x00000800); //APW shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d50,0x00000000); //APW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d58,0x00008000); //MMR shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d60,0x00020000); //MMR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d64,0x00001000); //MMR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d68,0x00010000); //MMR shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d6c,0x00000800); //MMR shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d70,0x08080000); //MMR shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d74,0x00073030); //MMR shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d78,0x00040000); //MMW shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d80,0x00100000); //MMW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d84,0x00004000); //MMW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d88,0x00080000); //MMW shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d8c,0x00002000); //MMW shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d90,0x08080000); //MMW shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d94,0x00074040); //MMW shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d98,0x00400000); //MDHWR sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000da0,0x00200000); //MDHWR sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000da8,0x10100404); //MDHWWR sh
   mt_emi_sync_write(EMI_APB_BASE+0x00000dac,0x01000000); //MDHWW sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000db4,0x00800000); //MDHWW sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dbc,0x04000000); //GPUR sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dc4,0x02000000); //GPUR sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dcc,0x60602010); //GPUR
   mt_emi_sync_write(EMI_APB_BASE+0x00000dd0,0x10000000); //GPUW sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dd8,0x08000000); //GPUW sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000de0,0x00000009); //ARBR sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000de8,0x04400080); //ARBR sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000df0,0x0f170f11); //ARB
   mt_emi_sync_write(EMI_APB_BASE+0x00000df4,0x0303f7f7); //QOS control
   mt_emi_sync_write(EMI_APB_BASE+0x00000df8,0x00ee03aa); //AFIFO control: CMD_AF_THR = A, GALS_CK_DCM = disable
   mt_emi_sync_write(EMI_APB_BASE+0x00000e04,0x00000166);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e08,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e0c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e14,0x00400166);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e18,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e1c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e24,0x00000266);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e28,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e2c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e34,0x00400266);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e38,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e3c,0xffffffff);

   // Added by Wei-Lun - START


   // prtcl chker - golden setting
   mt_emi_sync_write(EMI_APB_BASE+0x00000304,0xffffffff); // cyc
   mt_emi_sync_write(EMI_APB_BASE+0x0000030c,0x001ffc85); // ctl
   mt_emi_sync_write(EMI_APB_BASE+0x00000314,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000034c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000354,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x0000035c,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x00000394,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000039c,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x000003a4,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x000003d8,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000003dc,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x000003e0,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x000003fc,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000040c,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x00000414,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000044c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000454,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x0000045c,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000049c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000004a4,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x000004ac,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000050c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000514,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x0000051c,0xffffffff); // msk

   
   //weilun for new feature
   mt_emi_sync_write(EMI_APB_BASE+0x00000714,0x0000F000); // chuan - 00 -> fc (set 1 for freq that below 2400); mt6877 LP4 only has 8 levels

   // cen_emi timeout value
   mt_emi_sync_write(EMI_APB_BASE+0x00000628,0x60606060);
   mt_emi_sync_write(EMI_APB_BASE+0x0000062c,0x60606060);

   // fine-grained qos
   mt_emi_sync_write(EMI_APB_BASE+0x00000050,0x00000000);

   //// ostd->bw
   mt_emi_sync_write(EMI_APB_BASE+0x0000061c,0x08ffbbff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000624,0xffff5b3c);
   mt_emi_sync_write(EMI_APB_BASE+0x00000774,0xffff00ff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000077c,0x00ffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000784,0xffff00ff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000078c,0x00ffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000958,0x00000000);
   
   // hash rule
   mt_emi_sync_write(EMI_APB_BASE+0x000007a4,0xc000000b);

  } else if (mt_get_dram_type()== TYPE_LPDDR5) {
   mt_emi_sync_write(EMI_APB_BASE+0x00000000,0xf053f154); // 2CH config, row 16bit, per RANK 2GB, total 8GB

   // overhead: 20190821 item1 - synced
   mt_emi_sync_write(EMI_APB_BASE+0x00000004,0x182e2d33); //3733 (1:8) r4  - r1  overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x00000008,0x0f251025); //3733 (1:8) r8  - r5  overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x0000000c,0x122a1027); //3733 (1:8) r12 - r9  overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x00000010,0x1a31162d); //3733 (1:8) r16 - r13 overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x000008b0,0x182e2d33); //3200 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000008b4,0x0f251025); //3200 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000001c,0x122a1027); //3200 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000024,0x1a31162d); //3200 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000034,0x1024202c); //2400 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000006c,0x0b210c21); //2400 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000013c,0x0f250d23); //2400 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000640,0x152b1228); //2400 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000044,0x0c201a28); //1866 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000074,0x0d230a20); //1866 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001e0,0x0e260d24); //1866 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000644,0x132d1229); //1866 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000004c,0x0c201a28); //1600 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000084,0x0d230a20); //1600 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001e4,0x0e260d24); //1600 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000648,0x132d1229); //1600 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000054,0x0c201a28); //1200 (1:8) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000008c,0x0d230a20); //1200 (1:8) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001e8,0x0e260d24); //1200 (1:8) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000064c,0x132d1229); //1200 (1:8) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000005c,0x0e290e28); //800  (1:4) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000094,0x091e1322); //800  (1:4) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001c8,0x0f29112a); //800  (1:4) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000660,0x0c240a1f); //800  (1:4) r8  - r5  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000064,0x0e290e28); //800  (1:4) r12 - r9  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000009c,0x091e1322); //800  (1:4) r4  - r1  overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000001f4,0x0f29112a); //800  (1:4) r16 - r13 overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000664,0x0c240a1f); //800  (1:4) r8  - r5  overhead

   mt_emi_sync_write(EMI_APB_BASE+0x00000030,0x37373a57); //3733 (1:8) r8  - r2  non-align overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x00000014,0x3f3f3c39); //3733 (1:8) r16 - r10 non-align overhead  // TBD - change to 4266
   mt_emi_sync_write(EMI_APB_BASE+0x000008b8,0x3836374e); //3200 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000002c,0x41413d3a); //3200 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000c4,0x33313241); //2400 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000668,0x3a3a3835); //2400 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000c8,0x34343542); //1866 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000066c,0x3b3b3835); //1866 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000cc,0x34343542); //1600 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000694,0x3b3b3835); //1600 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000e4,0x34343542); //1200 (1:8) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000708,0x3b3b3835); //1200 (1:8) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x000000f4,0x37333034); //800  (1:4) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000070c,0x39393a39); //800  (1:4) r16 - r10 non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x0000012c,0x37333034); //800  (1:4) r8  - r2  non-align overhead
   mt_emi_sync_write(EMI_APB_BASE+0x00000748,0x39393a39); //800  (1:4) r16 - r10 non-align overhead


   mt_emi_sync_write(EMI_APB_BASE+0x00000018,0x3657587a);
   mt_emi_sync_write(EMI_APB_BASE+0x00000020,0x0000C042);; //sync to mt6873 #15, async_mode_use_neg_clk[15]=0
   mt_emi_sync_write(EMI_APB_BASE+0x00000028,0x00000000);  //cen_emi disable scramble
   mt_emi_sync_write(EMI_APB_BASE+0x00000038,0x00000083);  //row15bit/row 16bit
   mt_emi_sync_write(EMI_APB_BASE+0x0000003c,0x00073320);  //[5,8] LP5 16Bankor4BG , [4]64Byte_Mode_en=0 [3]BG_Mode
   mt_emi_sync_write(EMI_APB_BASE+0x00000040,0x00008802);
   mt_emi_sync_write(EMI_APB_BASE+0x00000048,0x00000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000060,0x007812ff); // reserved buffer to normal read/write :8/7
   mt_emi_sync_write(EMI_APB_BASE+0x00000068,0x02000000); //LP5 don't support FAST_PASS_slv_en[19:12]=0x0 (M0/M1 enable), bit[25] must be 1
   mt_emi_sync_write(EMI_APB_BASE+0x00000078,0x11120c1f);  //22:20=ultra_w=1
   mt_emi_sync_write(EMI_APB_BASE+0x00000710,0x11120c1f);  //22:20=ultra_w=1
   mt_emi_sync_write(EMI_APB_BASE+0x0000007c,0x00001123);
   mt_emi_sync_write(EMI_APB_BASE+0x00000718,0x00001123);
   mt_emi_sync_write(EMI_APB_BASE+0x000000d0,0xa8a8a8a8); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000d4,0x25252525); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000d8,0xa8a8a8a8); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000dc,0x25252525); //outstanding
   mt_emi_sync_write(EMI_APB_BASE+0x000000e8,0x00060037); // initial starvation counter div2, [4]=1
   mt_emi_sync_write(EMI_APB_BASE+0x000000f0,0x384a0014);
   mt_emi_sync_write(EMI_APB_BASE+0x000000f8,0xa0000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000100,0x20107244);
   mt_emi_sync_write(EMI_APB_BASE+0x00000108,0x10107044);
   mt_emi_sync_write(EMI_APB_BASE+0x00000110,0x343450df);
   mt_emi_sync_write(EMI_APB_BASE+0x00000118,0x0000f0d0);
   mt_emi_sync_write(EMI_APB_BASE+0x00000120,0x10106048);
   mt_emi_sync_write(EMI_APB_BASE+0x00000128,0x343450df);
   mt_emi_sync_write(EMI_APB_BASE+0x00000130,0x83837044);
   mt_emi_sync_write(EMI_APB_BASE+0x00000138,0x83837044);
   mt_emi_sync_write(EMI_APB_BASE+0x00000140,0x00007108);
   mt_emi_sync_write(EMI_APB_BASE+0x00000144,0x00007108);
   mt_emi_sync_write(EMI_APB_BASE+0x00000150,0x090a4000); //FAST_PASS_os_limit[11:8]=0x0
   mt_emi_sync_write(EMI_APB_BASE+0x00000158,0xff0bff00);
   mt_emi_sync_write(EMI_APB_BASE+0x00000400,0x00ff0001); //[27:20] enable monitor
   mt_emi_sync_write(EMI_APB_BASE+0x0000071c,0x10000008);
   mt_emi_sync_write(EMI_APB_BASE+0x00000800,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000820,0x24240101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000824,0x01012424);
   mt_emi_sync_write(EMI_APB_BASE+0x00000828,0x50500101);
   mt_emi_sync_write(EMI_APB_BASE+0x0000082c,0x01015050);
   mt_emi_sync_write(EMI_APB_BASE+0x00000830,0x0fc39a30); // [6] MD_HRT_URGENT_MASK, if 1 -> mask MD_HRT_URGENT,
   mt_emi_sync_write(EMI_APB_BASE+0x00000834,0x05050003);
   mt_emi_sync_write(EMI_APB_BASE+0x00000838,0x254dffff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000083c,0x465a788c); //update
   mt_emi_sync_write(EMI_APB_BASE+0x00000840,0x000003e8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000844,0x0000036b);
   mt_emi_sync_write(EMI_APB_BASE+0x00000848,0x00000290);
   mt_emi_sync_write(EMI_APB_BASE+0x0000084c,0x00000200);
   mt_emi_sync_write(EMI_APB_BASE+0x00000850,0x00000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000854,0x00000000);
   mt_emi_sync_write(EMI_APB_BASE+0x00000858,0x02531cff); //ignore rff threshold
   mt_emi_sync_write(EMI_APB_BASE+0x0000085c,0x00002785); //disable internal MD latency urgent mask
   mt_emi_sync_write(EMI_APB_BASE+0x00000874,0x000001b5);
   mt_emi_sync_write(EMI_APB_BASE+0x00000878,0x003c0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x0000087c,0x0255250d);
   mt_emi_sync_write(EMI_APB_BASE+0x00000890,0xffff3c59);
   mt_emi_sync_write(EMI_APB_BASE+0x00000894,0xffff00ff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008a0,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008a4,0x0000ffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008c0,0x0000014b);
   mt_emi_sync_write(EMI_APB_BASE+0x000008c4,0x002d0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x000008c8,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x000008cc,0x003c0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x000008d0,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x000008d4,0x003c0000); //update
   mt_emi_sync_write(EMI_APB_BASE+0x000008e0,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008e4,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000008e8,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000920,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000924,0x0000ffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000930,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000934,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000938,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000009f0,0x41547082);
   mt_emi_sync_write(EMI_APB_BASE+0x000009f4,0x38382a38);
   mt_emi_sync_write(EMI_APB_BASE+0x000009f8,0x000001d4);
   mt_emi_sync_write(EMI_APB_BASE+0x000009fc,0x00000190);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b00,0x0000012c);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b04,0x000000ed);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b08,0x000000c8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b0c,0x00000096);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b10,0x000000c8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b14,0x000000c8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b28,0x26304048);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b2c,0x20201820);

   mt_emi_sync_write(EMI_APB_BASE+0x00000b60,0x181e282f);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b64,0x14140f18);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b98,0x7496c8ea);
   mt_emi_sync_write(EMI_APB_BASE+0x00000b9c,0x64644b64);
   mt_emi_sync_write(EMI_APB_BASE+0x00000bd0,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000bd4,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c08,0x7496c8ea);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c0c,0x64644b64);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c40,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c44,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c4c,0x300ff025);       //ignore wff threshold
   mt_emi_sync_write(EMI_APB_BASE+0x00000c80,0x000003e8);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c84,0x0000036b);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c88,0x00000290);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c8c,0x00000200);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c90,0x000001b5);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c94,0x0000014b);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c98,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x00000c9c,0x00000185);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cb0,0x52698ca0);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cb4,0x46463546);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cf8,0x01010101);
   mt_emi_sync_write(EMI_APB_BASE+0x00000cfc,0x01010101);

   mt_emi_sync_write(EMI_APB_BASE+0x00000d04,0x00000009); //MDR shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d0c,0x00000000); //MDR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d14,0x00730000); //MDR shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d18,0x00000808); //MDR shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d1c,0x00000028); //MDW shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d24,0x00000000); //MDW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d2c,0x00730000); //MDW shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d30,0x00000808); //MDW shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d34,0x00000080); //APR shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d3c,0x00000000); //APR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d44,0x30201008); //APR shf0/shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d48,0x00000800); //APW shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d50,0x00000000); //APW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d58,0x00008000); //MMR shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d60,0x00020000); //MMR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d64,0x00001000); //MMR shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d68,0x00010000); //MMR shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d6c,0x00000800); //MMR shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d70,0x08080000); //MMR shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d74,0x00073030); //MMR shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d78,0x00040000); //MMW shf0 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d80,0x00100000); //MMW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d84,0x00004000); //MMW shf1 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d88,0x00080000); //MMW shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d8c,0x00002000); //MMW shf2 event selet
   mt_emi_sync_write(EMI_APB_BASE+0x00000d90,0x08080000); //MMW shf0
   mt_emi_sync_write(EMI_APB_BASE+0x00000d94,0x00074040); //MMW shf1
   mt_emi_sync_write(EMI_APB_BASE+0x00000d98,0x00400000); //MDHWR sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000da0,0x00200000); //MDHWR sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000da8,0x10100404); //MDHWWR sh
   mt_emi_sync_write(EMI_APB_BASE+0x00000dac,0x01000000); //MDHWW sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000db4,0x00800000); //MDHWW sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dbc,0x04000000); //GPUR sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dc4,0x02000000); //GPUR sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dcc,0x60602010); //GPUR
   mt_emi_sync_write(EMI_APB_BASE+0x00000dd0,0x10000000); //GPUW sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000dd8,0x08000000); //GPUW sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000de0,0x00000009); //ARBR sh0 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000de8,0x04400080); //ARBR sh1 event select
   mt_emi_sync_write(EMI_APB_BASE+0x00000df0,0x0f170f11); //ARB
   mt_emi_sync_write(EMI_APB_BASE+0x00000df4,0x0303f7f7); //QOS control
   mt_emi_sync_write(EMI_APB_BASE+0x00000df8,0x00ee03aa); //AFIFO control: CMD_AF_THR = a, GALS_CK_DCM = disable
   mt_emi_sync_write(EMI_APB_BASE+0x00000e04,0x00000166);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e08,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e0c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e14,0x00400166);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e18,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e1c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e24,0x00000266);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e28,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e2c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e34,0x00400266);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e38,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000e3c,0xffffffff);

   // Added by Wei-Lun - START


   // prtcl chker - golden setting
   mt_emi_sync_write(EMI_APB_BASE+0x00000304,0xffffffff); // cyc
   mt_emi_sync_write(EMI_APB_BASE+0x0000030c,0x001ffc85); // ctl
   mt_emi_sync_write(EMI_APB_BASE+0x00000314,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000034c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000354,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x0000035c,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x00000394,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000039c,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x000003a4,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x000003d8,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000003dc,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x000003e0,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x000003fc,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000040c,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x00000414,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000044c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000454,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x0000045c,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000049c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x000004a4,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x000004ac,0xffffffff); // msk

   mt_emi_sync_write(EMI_APB_BASE+0x0000050c,0xffffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000514,0x001ffc85);
   mt_emi_sync_write(EMI_APB_BASE+0x0000051c,0xffffffff); // msk

   //weilun for new feature
   mt_emi_sync_write(EMI_APB_BASE+0x00000714,0x0000F000); // chuan - 00 -> fc (set 1 for freq that below 2400); mt6877 LP5 only has 8 levels

   // cen_emi timeout value
   mt_emi_sync_write(EMI_APB_BASE+0x00000628,0x60606060);
   mt_emi_sync_write(EMI_APB_BASE+0x0000062c,0x60606060);

   // fine-grained qos
   mt_emi_sync_write(EMI_APB_BASE+0x00000050,0x00000000);

   //// ostd->bw
   mt_emi_sync_write(EMI_APB_BASE+0x0000061c,0x08ffbbff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000624,0xffff5b3c);
   mt_emi_sync_write(EMI_APB_BASE+0x00000774,0xffff00ff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000077c,0x00ffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000784,0xffff00ff);
   mt_emi_sync_write(EMI_APB_BASE+0x0000078c,0x00ffffff);
   mt_emi_sync_write(EMI_APB_BASE+0x00000958,0x00000000);

   // hash rule
   mt_emi_sync_write(EMI_APB_BASE+0x000007a4,0xc000000b);

  }
}

static void emi_chn_config(unsigned cfg_base_addr) {
   unsigned EMI_CHANNEL_APB_BASE = cfg_base_addr;

 if (mt_get_dram_type()== TYPE_LPDDR4X) {

  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000000,0x0400005d); // 2CH config, row 17bit, per RANK 2GB, total 8GB

  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000008,0x00ff6048); //age_speed[13:12] sync to mt6873
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000010,0x000421C4); //[6]=1; Disable AFIFO block ==> SEDA5 must-be
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000014,0x00000001); //FAST_PASS_EN[28]=0x1, SEDA5_COL_ALIGN[0]=0x1
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000018,0x99f08c03);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000020,0x00000040); // disablde EBG
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000710,0x9a508c17); // [24:20] = 0x2 : bank throttling (default=0x01f00000)
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000048,0x00038137); // RD_INORDER_THR[20:16]=3 , [11]=64Byte_mode_en=0 ,   WDATA_MASKED_BY_BE[8]=0x1, [13]=short_request_disable=1
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000050,0x38460002); // [1] : MD_RD_AFT_WR_EN
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000058,0x00000100); // disable RDF[0] to avoid rdne bug of CEN_EMI (two rdne events, one belonging to rdf[0] won't be sent to chn_emi)
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000090,0x000002ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000098,0x00003111); //mw2
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000140,0x22607188); //sync to mt6873 #12
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000144,0x22607188); //sync to mt6873 #12
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000148,0x3719595e); // chuan
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000014c,0x2719595e); // chuan
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000150,0x64f3ff79); // chuan
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000154,0x64f3ff79); // chuan // update timeout setting: bit 12~15 (Austin's design change in mt6885; has bug; setting need change in Conti)
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000158,0x011b0868);


  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000015c,0xa74142a2); // Stop urgent read first when write command buffer remain < 7, [31] ultra_read_first, [30:28] wr_rsv_thr_l, [27:    24] wr_rsv_thr_h,
                                                                 // 0x15c[7] = 1, update by Chuan liu to promote page hit priority@201119
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000016c,0x0000f801);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000170,0x40000000);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b0,0x000c804f); // Rank-Aware arbitration , sw_thr[7:4] = 0x4 for LP4
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b4,0xbd3f3f7e); // Rank-Aware arbitration
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b8,0x7e003d7e); // Rank-Aware arbitration
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000300,0x000000CB); // CHN_EMI_GALS_CON , AFIFO_AF_LV[5:0]=0x10, async_mode_use_neg_clk[7] = 0, gals_ck_dcm_en[6] = 0
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000003fc,0x00000000); // Write M17_toggle_mask = 0
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000080,0xaa0148ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000088,0xaa6168ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000404,0xaa516cff); // chuan - [9], [12]
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000408,0xaa0140ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000040c,0x9f658633);

  } else if (mt_get_dram_type()== TYPE_LPDDR5) {

  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000000,0x0400fc51); // 2CH config, row 16bit, per RANK 2GB, total 8GB
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000008,0x00ff6048); //age_speed[13:12] sync to mt6873
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000010,0x004210C4); //chn scramble = 1'b0; adr[12] = adr[12]^ adr[16] ; adr[13] = adr[13]^ adr[17] ; adr[14] = adr[14]^ adr[18] ; [6] AFIFO block disable
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000014,0x00000001); //FAST_PASS_EN[28]=0x0, SEDA5_COL_ALIGN[0]=0x1
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000018,0x99f08c03);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000020,0x00000040); // disable EBG
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000710,0x9a508c17); // [24:20] = 0x2 : bank throttling (default=0x01f00000)
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000048,0x00038137); // RD_INORDER_THR[20:16]=3 , [11]=64Byte_mode_en=0 ,   WDATA_MASKED_BY_BE[8]=0x1, [13]=short_request_disable=1
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000050,0x3F460002); // [1] : MD_RD_AFT_WR_EN, DEFER_WR_WDATA_THR[27:24] = 0xF ( Lp5 push new write command by seq_wdata_cnt < 0xF*4 )
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000058,0x00000100); // disable RDF[0] to avoid rdne bug of CEN_EMI (two rdne events, one belonging to rdf[0] won't be sent to chn_emi)
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000090,0x000002ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000098,0x00001111); // HIT_THR=0x1 , MIS_THR=0x1, MRW_THR=0x1, DQFR_EN=0x1 //LP5 HIT_THR to increase Bankgroup diversity
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000140,0x22607188); //sync to mt6873 #12
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000144,0x22607188); //sync to mt6873 #12
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000148,0x3719595e); // chuan
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000014c,0x2719595e); // chuan
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000150,0x64f3ff79); // chuan
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000154,0x64f3ff79); // chuan // update timeout setting: bit 12~15 (Austin's design change in mt6885; has bug; setting need change in Conti)
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000158,0x011b0868);


  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000015c,0xa74142a2); // Stop urgent read first when write command buffer remain < 7, [31] ultra_read_first, [30:28] wr_rsv_thr_l, [27:    24] wr_rsv_thr_h,
                                                                 // 0x15c[7] = 1, update by Chuan liu to promote page hit priority@201119
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000016c,0x0000f801);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000170,0x40000000);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b0,0x000c804f); // Rank-Aware arbitration  , sw_thr[7:4] = 0x4 for LP5
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b4,0xbd3f3f7e); // Rank-Aware arbitration
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b8,0x7e003d7e); // Rank-Aware arbitration
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000300,0x000000CB); // CHN_EMI_GALS_CON , AFIFO_AF_LV[5:0]=0x10, async_mode_use_neg_clk[7] = 1, gals_ck_dcm_en[6] = 1
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000003fc,0x00000000); // Write M17_toggle_mask = 0
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000080,0xaa0148ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000088,0xaa6168ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000404,0xaa516cff); // chuan - [9], [12]
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000408,0xaa0140ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000040c,0x9f658633);
  }

}

static void emi_sw_setting(void)
{
#ifdef MTK_DOE_CONFIG_ENV_SUPPORT /* from platform/mtxxxx/feature.mak */
	unsigned int emi_mpu_slverr = 0;
	unsigned int emi_dcm = 0;
	unsigned int emi_isu = 0;
	unsigned int domain = 0;
	char *str;
#endif
	/* Enable MPU violation interrupt to MD for D1 and D7 */
	*((volatile unsigned int *)EMI_MPU_CTRL_D(1)) |= 0x10;
	*((volatile unsigned int *)EMI_MPU_CTRL_D(7)) |= 0x10;

	/* mpu irq settings. separate emi mpu and devmpu irq */
	*((volatile unsigned int *) EMI_CONH) = *((volatile unsigned int *) EMI_CONH) | 0xC0;

	/* for DVFS BW monitor */
	*((volatile unsigned int *) EMI_BWCT0) = 0x05000305;
	*((volatile unsigned int *) EMI_BWCT0_6TH) = 0x08FF0705;
	*((volatile unsigned int *) EMI_BWCT0_3RD) = 0x0FFF0B05;
	*((volatile unsigned int *) EMI_BWCT0_4TH) = 0x3FFF1105;

#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	if (last_emi_info_ptr->isu_magic != LAST_EMI_MAGIC_PATTERN) {
		last_emi_info_ptr->isu_magic = LAST_EMI_MAGIC_PATTERN;
		last_emi_info_ptr->isu_version = 0xFFFFFFFF;
		last_emi_info_ptr->isu_dram_type = 0;
		last_emi_info_ptr->isu_diff_us = 0;
		last_emi_info_ptr->os_flag_sspm = 0;
		last_emi_info_ptr->os_flag_ap = 0;
		last_emi_info_ptr->isu_buf_l = 0;
		last_emi_info_ptr->isu_buf_h = 0;
	}
#ifdef MTK_DOE_CONFIG_ENV_SUPPORT /* from platform/mtxxxx/feature.mak */
	str = dconfig_getenv("emi_isu");
	emi_isu = (str) ? atoi(str) : 0;
	emi_log("[EMI DOE] emi_isu %d\n", emi_isu);
	if (emi_isu == 1)
		last_emi_info_ptr->isu_ctrl = 0xDECDDECD;
	else if (emi_isu == 2)
		last_emi_info_ptr->isu_ctrl = 0xDEC0DEC0;
	else {
#if CFG_LAST_EMI_BW_DUMP
		last_emi_info_ptr->isu_ctrl = 0xDECDDECD;
#else
		last_emi_info_ptr->isu_ctrl = 0xDEC0DEC0;
#endif
	}
#endif
#endif
	/* align urgent monitor countrol to bus monitor */
	*((volatile unsigned int *)0x10219858) |= 0x1 << 11;

#ifdef MTK_DOE_CONFIG_ENV_SUPPORT /* from platform/mtxxxx/feature.mak */

	/* EMI doeapp for MPU slaver error */
	str = dconfig_getenv("emi_mpu_slverr");
	emi_mpu_slverr = (str) ? atoi(str) : 0;

	if (emi_mpu_slverr == 1) {
		str = dconfig_getenv("emi_mpu_slverr_domain");
		domain = (str) ? atoi(str) : 0;
		*((volatile unsigned int *)EMI_MPU_CTRL_D(domain)) |= 0x2;
		emi_log("EMI SLVERR ENABLED domain: %d\n", domain);
	} else if (emi_mpu_slverr == 2) {
		for (domain = 0; domain < EMI_MPU_DOMAIN_NUM; domain++) {
			*((volatile unsigned int *)EMI_MPU_CTRL_D(domain)) &= ~0x2;
			emi_log("EMI SLVERR DISABLED\n");
		}
	}

	/* EMI doeapp for DCM */
	str = dconfig_getenv("emi_dcm");
	emi_dcm = (str) ? atoi(str) : 0;

	if (emi_dcm == 1) {
		*((volatile unsigned int *)EMI_CONM) &= ~0xFF000000;//Enable DCM
		*((volatile unsigned int *)EMI_CONN) &= ~0xFF000000;//Enable DCM2
		emi_log("EMI DCM ENABLED\n");
	} else if (emi_dcm == 2) {
		*((volatile unsigned int *)EMI_CONM) |= 0xFF000000;
		*((volatile unsigned int *)EMI_CONN) |= 0xFF000000;
		emi_log("EMI DCM DISABLED\n");
	}

#endif
	dsb();
}

void emi_init(void)
{
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x038, 0x10000000); // switch top mem clock mux source to MEMPLL
   mt_emi_sync_write(EMI_MPU_BASE+0x00000000,0x80000010);   // disable EMI APB protection
   //Broadcast on
   mt_emi_sync_write(INFRA_DRAMC_REG_CONFIG,0x0000007f);

   emi_cen_config(EMI_BASE);
   emi_chn_config(CHN0_EMI_BASE);

   //Broadcast off
   mt_emi_sync_write(INFRA_DRAMC_REG_CONFIG,0x00000000);
}

void emi_init2(void)
{
   unsigned int EMI_APB_BASE = EMI_BASE;
   unsigned int EMI_CHANNEL_APB_BASE = CHN0_EMI_BASE;
   unsigned int emi_temp_data;

   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x038, 0x10000000); // switch top mem clock mux source to MEMPLL
   mt_emi_sync_write(EMI_MPU_BASE+0x00000000,0x80000010);   // disable EMI APB protection
  //Broadcast on
   mt_emi_sync_write(INFRA_DRAMC_REG_CONFIG, 0x0000007f);

   /* Notice : EMI_CONA, EMI_CONF, EMI_CONH, EMI_CONH_2ND,
    *	       CHN0_EMI_CONA and CHN0_EMI_CONC are set by dram.
    *	       Only set these registers with mt_emi_sync_write_or.
    */
   mt_emi_sync_write_or(CHN0_EMI_BASE+0x00000010, 0x00000041);   // [0] EMI enable [6] AFIFO block disable
   mt_emi_sync_write_or(EMI_BASE+0x00000060, 0x00000400);	//[10] EMI enable

   mt_emi_sync_write_or(EMI_MPU_BASE+0x00000000,0x00000010); // [4] Disable emi_mpu_reg interrupt

   // Clear rank_arb_en
   emi_temp_data = mt_emi_sync_read(EMI_CHANNEL_APB_BASE+0x000001b0); // read ch0
   emi_temp_data = emi_temp_data & ~(0x1);
   mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b0, emi_temp_data); // broadcast to all     channel
   // auto-config rank_arb_en according to dual_rank_en setting
   // assume all channel with same configuration
   emi_temp_data = mt_emi_sync_read(EMI_CHANNEL_APB_BASE+0x00000000); // read ch0
   emi_temp_data = emi_temp_data & 0x1;
   mt_emi_sync_write_or(EMI_CHANNEL_APB_BASE+0x000001b0, emi_temp_data); // broadcast to all channel

   // ----- from dcm_setting.c -----
   //mt_emi_sync_write_or(INFRACFG_AO_BASE+0x00000078, 0x08000000); // enable infra_local_cg

   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x028, 0x0000007F); // MEM_CK DCM group related setting; [22:16] group DCM freerun
   //useless mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x02C, 0x00000000); // MEM_CK DCM stall count
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x030, 0x001F0000); // MEM_CK DCM debounce count (m0m1, m2m5)

   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x100, 0xFFFFFFFF);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x104, 0xFFFFFFFF);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x108, 0xFFFFFFFF);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x10C, 0xFFFFFFFF);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x110, 0xF8000003);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x114, 0x0000000F);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x118, 0x0100C002);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x11C, 0x000C0006);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x120, 0x000003FC);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x124, 0x00000000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x128, 0x3000000C);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x12C, 0x01803000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x130, 0x00000C00);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x134, 0x40000000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x138, 0x00000020);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x13C, 0x00000200);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x140, 0x0000F000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x144, 0x20000000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x148, 0x00480011);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x14C, 0x00000100);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x150, 0x07FF0000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x154, 0x80000000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x158, 0x800700C0);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x15C, 0x0E704CF9);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x160, 0x00000000);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x164, 0x08FFFE00);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x168, 0x40003F00);
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x16C, 0x00020000);
   // emi bus parity workaround
   emi_temp_data = mt_emi_sync_read(0x40000000);
   mt_emi_sync_write(0x40000000, emi_temp_data);
   emi_temp_data = mt_emi_sync_read(0x40000100);
   mt_emi_sync_write(0x40000100, emi_temp_data);
   emi_temp_data = mt_emi_sync_read(0x40000200);
   mt_emi_sync_write(0x40000200, emi_temp_data);
   emi_temp_data = mt_emi_sync_read(0x40000300);
   mt_emi_sync_write(0x40000300, emi_temp_data);
 
   // Enable rdata_prty_gen & wdata_prty_chk
   mt_emi_sync_write_or(EMI_APB_BASE+0x00000068,0x00400000);  // enable cen_emi parity (w)
   // Enable chn_emi rdata_prty_gen & wdata_prty_chk
   mt_emi_sync_write_or(EMI_CHANNEL_APB_BASE+0x00000050,0x00000004); // enable chn_emi parity    ity
   /*TINFO="program hash rule"*/
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000050, 0x0000000b); // set disph_chn_en = 0x1
   mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000050, 0x8000000b); // set disph_chg_en = 0x1
  /*TINFO="read central hash setting then write back to EMI internal hash reg"*/
   emi_temp_data = mt_emi_sync_read(INFRACFG_AO_MEM_BASE+0x050);
   emi_temp_data = emi_temp_data & 0xf;
   mt_emi_sync_write_or(EMI_BASE+0x07A4, emi_temp_data);
 
   /*TINFO="Enable EMI wdata bus encode function"*/
   mt_emi_sync_write_or(EMI_APB_BASE+0x00000068,0x00200000); // enable cen_emi wdata bus encode          // *EMI_CONN |= (0x1 << 21);
   mt_emi_sync_write_or(EMI_CHANNEL_APB_BASE+0x00000050,0x00000010); // enable chn_emi wdata bus encode  // *CHN0_EMI_CHN_EMI_DFTC |= (0x1 <<4);
  //Broadcast off
  mt_emi_sync_write(INFRA_DRAMC_REG_CONFIG, 0x00000000);

  emi_sw_setting();
  emi_addr2dram_hash(11);
  emi_addr2dram_disph(0);
}

int get_row_width_by_emi(unsigned int rank)
{
	unsigned int emi_cona;
	unsigned int shift_row, shift_ext;
	int row_width;

	if (rank == 0) {
		shift_row = 12;
		shift_ext = 22;
	} else if (rank == 1) {
		shift_row = 14;
		shift_ext = 23;
	} else
		return -1;

	emi_cona = mt_emi_sync_read(EMI_CONA);
	row_width =
		((emi_cona >> shift_row) & 0x3) |
		((emi_cona >> shift_ext) & 0x4);

	return (row_width + 13);
}

int get_channel_nr_by_emi(void)
{
	int channel_nr;

	channel_nr = 0x1 << ((mt_emi_sync_read(EMI_CONA) >> 8) & 0x3);

	return channel_nr;
}

int get_rank_nr_by_emi(void)
{
	unsigned int cen_emi_cona = mt_emi_sync_read(EMI_CONA);

	if (cen_emi_cona & (0x3 << 16))
		return 2;
	else
		return 1;
}

_Static_assert(DRAMC_MAX_RK > 1, "rank number is violated");
void get_rank_size_by_emi(unsigned long long dram_rank_size[DRAMC_MAX_RK])
{
	int i;
	unsigned int quad_ch_ratio;
	unsigned int dw32_en;
	unsigned int row_ext0, row, chn_bg_16bank_mode, chn_4bank_mode, col;
	unsigned int row2nd_ext0, row2nd, chn_bg_16bank_mode_2nd, chn_4bank_mode_2nd, col2nd;
	unsigned int rank0_row_width, rank0_bank_width, rank0_col_width;
	unsigned int rank1_row_width, rank1_bank_width, rank1_col_width;
	unsigned long long ch0_rank0_size, ch0_rank1_size;
	unsigned long long ch1_rank0_size, ch1_rank1_size;
 	unsigned int cen_emi_conh = mt_emi_sync_read(EMI_CONH);
	unsigned int chn_emi_cona;

	dram_rank_size[0] = 0;
	dram_rank_size[1] = 0;

	ch0_rank0_size = (cen_emi_conh >> 16) & 0xF;
	ch0_rank1_size = (cen_emi_conh >> 20) & 0xF;
	ch1_rank0_size = (cen_emi_conh >> 24) & 0xF;
	ch1_rank1_size = (cen_emi_conh >> 28) & 0xF;

	quad_ch_ratio = (get_rank_nr_by_emi() == 4)? 2 : 1;
	ch0_rank0_size = (ch0_rank0_size * quad_ch_ratio) << 28;
	ch0_rank1_size = (ch0_rank1_size * quad_ch_ratio) << 28;
	ch1_rank0_size = (ch1_rank0_size * quad_ch_ratio) << 28;
	ch1_rank1_size = (ch1_rank1_size * quad_ch_ratio) << 28;

	/* rank_size should be computed by CHN_CONA if CONH[19:16] == 0 */
	if(ch0_rank0_size == 0) {
	    chn_emi_cona = mt_emi_sync_read(CHN_EMI_CONA(CHN0_EMI_BASE));
	    dw32_en            = (chn_emi_cona >> 1)  & 0x1;
	    row_ext0           = (chn_emi_cona >> 2)  & 0x1;
	    col                = (chn_emi_cona >> 4)  & 0x3;
	    chn_bg_16bank_mode = (chn_emi_cona >> 10) & 0x1;
	    row                = (chn_emi_cona >> 12) & 0x3;
	    chn_4bank_mode     = (chn_emi_cona >> 24) & 0x1;

	    rank0_row_width = row_ext0*4 + row + 13;
	    rank0_bank_width = (chn_bg_16bank_mode == 1)? 4 :
		                   (chn_4bank_mode     == 1)? 2 : 3;
	    rank0_col_width  = col + 9;
	    ch0_rank0_size = (1ULL) << (rank0_row_width + rank0_bank_width + rank0_col_width + dw32_en + 1);
 	}
        dram_rank_size[0] += ch0_rank0_size;

	if (get_rank_nr_by_emi() > 1) {
		if(ch0_rank1_size == 0) {
		    chn_emi_cona = mt_emi_sync_read(CHN_EMI_CONA(CHN0_EMI_BASE));
		    dw32_en                = (chn_emi_cona >> 1)  & 0x1;
		    row2nd_ext0            = (chn_emi_cona >> 3)  & 0x1;
		    col2nd                 = (chn_emi_cona >> 6)  & 0x3;
		    chn_bg_16bank_mode_2nd = (chn_emi_cona >> 11) & 0x1;
		    row2nd                 = (chn_emi_cona >> 14) & 0x3;
		    chn_4bank_mode_2nd     = (chn_emi_cona >> 25) & 0x1;

		    rank1_row_width = row2nd_ext0*4 + row2nd + 13;
		    rank1_bank_width = (chn_bg_16bank_mode_2nd == 1)? 4 :
			                  (chn_4bank_mode_2nd == 1)? 2 : 3;
		    rank1_col_width  = col2nd + 9;
		    ch0_rank1_size = (1ULL) << (rank1_row_width + rank1_bank_width + rank1_col_width + dw32_en + 1);
		}
		dram_rank_size[1] += ch0_rank1_size;
	}

	if(get_channel_nr_by_emi() > 1) {
		if(ch1_rank0_size == 0) {
		    ch1_rank0_size = ch0_rank0_size;
		    emi_log("[EMI] ch1_rank0_size==0 set ch1_rank0_size=0x%llx\n",ch1_rank0_size);
		}
		dram_rank_size[0] += ch1_rank0_size;

		if (get_rank_nr_by_emi() > 1) {
		    if(ch1_rank1_size == 0) {
	    		ch1_rank1_size = ch0_rank1_size;
                        emi_log("[EMI] ch1_rank1_size==0 set ch1_rank1_size=0x%llx\n",ch1_rank1_size);
		    }
		    dram_rank_size[1] += ch1_rank1_size;
		}
	}

	emi_log("DRAM rank0 size:0x%llx,\nDRAM rank1 size=0x%llx\n",
		dram_rank_size[0], dram_rank_size[1]);
}

void set_cen_emi_cona(unsigned int cona_val)
{
	mt_emi_sync_write(EMI_CONA, cona_val);
}

void set_cen_emi_conf(unsigned int conf_val)
{
	mt_emi_sync_write(EMI_CONF, conf_val);
}

void set_cen_emi_conh(unsigned int conh_val)
{
	mt_emi_sync_write(EMI_CONH, conh_val);
}

void set_cen_emi_conh_2nd(unsigned int conh_2nd_val)
{
	mt_emi_sync_write(EMI_CONH_2ND, conh_2nd_val);
}

void set_chn_emi_cona(unsigned int cona_val)
{
	mt_emi_sync_write(CHN_EMI_CONA(CHN0_EMI_BASE), cona_val);
	mt_emi_sync_write(CHN_EMI_CONA(CHN1_EMI_BASE), cona_val);
}

void set_chn_emi_conc(unsigned int conc_val)
{
	mt_emi_sync_write(CHN_EMI_CONC(CHN0_EMI_BASE), conc_val);
	mt_emi_sync_write(CHN_EMI_CONC(CHN1_EMI_BASE), conc_val);
}

unsigned int get_cen_emi_cona(void)
{
	return mt_emi_sync_read(EMI_CONA);
}

/* assume all chn emi setting are the same */
unsigned int get_chn_emi_cona(void)
{
	unsigned int ch0_emi_cona;

	ch0_emi_cona = mt_emi_sync_read(CHN0_EMI_BASE);

	return ch0_emi_cona;
}

void phy_addr_to_dram_addr(dram_addr_t *dram_addr, unsigned long long phy_addr)
{
	unsigned int cen_emi_cona, cen_emi_conf;
	unsigned long long rank_size[DRAMC_MAX_RK];
	unsigned int channel_num, rank_num;
	unsigned int bit_scramble, bit_xor, bit_shift, channel_pos, channel_width;
	unsigned int temp;
	unsigned int index;

	cen_emi_cona = mt_emi_sync_read(EMI_CONA);
	cen_emi_conf = mt_emi_sync_read(EMI_CONF) >> 8;
	get_rank_size_by_emi(rank_size);
	rank_num = (unsigned int) get_rank_nr_by_emi();
	channel_num = (unsigned int) get_channel_nr_by_emi();

	phy_addr -= 0x40000000;
	for (index = 0; index < rank_num; index++) {
		if (phy_addr >= rank_size[index])
			phy_addr -= rank_size[index];
		else
			break;
	}

	for (bit_scramble = 11; bit_scramble < 17; bit_scramble++) {
		bit_xor = (cen_emi_conf >> (4 * (bit_scramble - 11))) & 0xf;
		bit_xor &= phy_addr >> 16;
		for (bit_shift = 0; bit_shift < 4; bit_shift++)
			phy_addr ^= ((bit_xor>>bit_shift)&0x1) << bit_scramble;
	}

	if (channel_num > 1) {
		channel_pos = ((cen_emi_cona >> 2) & 0x3) + 7;

		for (channel_width = bit_shift = 0; bit_shift < 4; bit_shift++) {
			if ((unsigned int)(1 << bit_shift) >= channel_num)
				break;
			channel_width++;
		}

		switch (channel_width) {
		case 2:
			dram_addr->addr = ((phy_addr & ~(((0x1 << 2) << channel_pos) - 1)) >> 2);
			break;
		default:
			dram_addr->addr = ((phy_addr & ~(((0x1 << 1) << channel_pos) - 1)) >> 1);
			break;
		}
		dram_addr->addr |= (phy_addr & ((0x1 << channel_pos) - 1));
	}

	temp = dram_addr->addr >> 1;
	switch ((cen_emi_cona >> 4) & 0x3) {
	case 0:
		dram_addr->col = temp & 0x1FF;
		temp = temp >> 9;
		break;
	case 1:
		dram_addr->col = temp & 0x3FF;
		temp = temp >> 10;
		break;
	case 2:
	default:
		dram_addr->col = temp & 0x7FF;
		temp = temp >> 11;
		break;
	}
	dram_addr->bk = temp & 0x7;
	temp = temp >> 3;

	dram_addr->row = temp;

	emi_log("[EMI] ch%d, rk%d, dram addr: %x\n", dram_addr->ch, dram_addr->rk, dram_addr->addr);
	emi_log("[EMI] bk%x, row%x, col%x\n", dram_addr->bk, dram_addr->row, dram_addr->col);
}

static unsigned int cen_emi_conh_backup = 0;
static unsigned int chn_emi_cona_backup = 0;

/* return the start address of rank1 */
unsigned int set_emi_before_rank1_mem_test(void)
{
	cen_emi_conh_backup = mt_emi_sync_read(EMI_CONH);
	chn_emi_cona_backup = get_chn_emi_cona();

	if (get_rank_nr_by_emi() == 2) {
		/* set the rank size to 1GB for 2 channels */
		mt_emi_sync_write(EMI_CONH,
			(cen_emi_conh_backup & 0x0000ffff) | 0x22220000);
		set_chn_emi_cona(
			(chn_emi_cona_backup & 0xff00ffff) | 0x00220000);
	} else {
		/* set the rank size to 1GB for 1 channel */
		mt_emi_sync_write(EMI_CONH,
			(cen_emi_conh_backup & 0x0000ffff) | 0x44440000);
		set_chn_emi_cona(
			(chn_emi_cona_backup & 0xff00ffff) | 0x00440000);
	}

	return 0x40000000;
}

void restore_emi_after_rank1_mem_test(void)
{
	mt_emi_sync_write(EMI_CONH, cen_emi_conh_backup);
	set_chn_emi_cona(chn_emi_cona_backup);
}

void get_emi_isu_info(struct isu_info_t *isu_info_ptr)
{
#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	isu_info_ptr->buf_size = EMI_ISU_BUF_SIZE;
	isu_info_ptr->buf_addr = (unsigned long long)last_emi_info_ptr->isu_buf_h;
	isu_info_ptr->buf_addr <<= 32;
	isu_info_ptr->buf_addr += (unsigned long long)last_emi_info_ptr->isu_buf_l;
	isu_info_ptr->ver_addr = (unsigned long long)(&(last_emi_info_ptr->isu_version));
	isu_info_ptr->con_addr = (unsigned long long)(&(last_emi_info_ptr->isu_ctrl));
#endif
}

int check_emi_mpu_addr(struct region_to_clear *info)
{
	if(!info)
		return 1;

	info->region_sa = (unsigned int)((mt_emi_sync_read(EMI_MPU_SA(info->region_id))) << 16);
	info->region_ea = (unsigned int)((mt_emi_sync_read(EMI_MPU_EA(info->region_id))) << 16) + 0xFFFF;
	info->region_sa += 0x40000000;
	info->region_ea += 0x40000000;

	emi_log("%s : 0x%x 0x%x %d\n", __func__, info->region_sa, info->region_ea, info->region_id);
	return 0;
}

void clr_emi_mpu_prot(void)
{
#if !__ETT__
	int region, dgroup;

	for (region = 0; region < EMI_MPU_REGION_NUM; region++) {
		mt_emi_sync_write(EMI_MPU_SA(region), 0x0);
		mt_emi_sync_write(EMI_MPU_EA(region), 0x0);
		for (dgroup = EMI_MPU_DGROUP_NUM - 1; dgroup >= 0; dgroup--) {
			mt_emi_sync_write(EMI_MPU_APC(region, dgroup), 0x0);
		}
	}
#endif
}

int update_emi_setting(EMI_SETTINGS *default_emi_setting, EMI_INFO_T *emi_info)
{
	unsigned int ddr_type;
	unsigned int cen_cona_val, cen_conh_val, cen_conh_2nd_val, cen_conk_val;
	unsigned int chn_cona_val, chn_conc_val;
	unsigned int col, row, row_ext, rk_size, rk_size_ext, rk_size_chn_ext;
	int i;

	ddr_type = default_emi_setting->type & 0xFF;
	cen_conh_val = 0x00000003;
	cen_conk_val = mt_emi_sync_read(EMI_CONK) & 0x0000FFFF;
	if (u1IsLP4Family(ddr_type)) {
		cen_cona_val = 0x00000104;
		chn_cona_val = 0x04000000;
	} else {
		cen_cona_val = 0x00000104;
		chn_cona_val = 0x04000C00;	
	}

	for (i = 0; i < emi_info->rk_num; i++) {
		row = emi_info->row_width[i] - 13;
		row_ext = row >> 2;
		row &= 0x3;

		col = emi_info->col_width[i] - 9;

		rk_size = emi_info->rank_size[i] >> 28;
		rk_size >>= 1;
		rk_size_ext = rk_size >> 4;
		rk_size_chn_ext = rk_size_ext & 0x1;
		rk_size &= 0xF;

		if (i == 1) {
			col <<= 2;
			row <<= 2;
			row_ext <<= 1;
			rk_size <<= 4;
			rk_size_ext <<= 4;
			rk_size_chn_ext <<= 1;
			cen_cona_val |= 0x00030000;
			chn_cona_val |= 0x00000001;
		}

		cen_cona_val |=
			(col << 4) | (row << 12) | (col << 20) |
			(row_ext << 24) | (row << 28);
		cen_conh_val |=
			(row_ext << 4) | (rk_size << 16) | (rk_size << 24);
		cen_conk_val |=
			(rk_size_ext << 16) | (rk_size_ext << 24);
		chn_cona_val |=
			(row_ext << 2) | (col << 4) | (row << 12) |
			(rk_size << 16) | (rk_size_chn_ext << 8);
	}

	if (u1IsLP4Family(ddr_type)) {
		chn_conc_val = 0x42180;
		cen_conh_2nd_val = 0;
	} else {
		chn_conc_val = 0x421080;
		cen_conh_2nd_val = 0x120;
	}
	emi_log("[EMI] %s(0x%x),%s(0x%x),%s(0x%x),%s(0x%x),%s(0x%x),%s(0x%x)\n",
		"CEN_CONA", cen_cona_val,
		"CEN_CONH", cen_conh_val,
		"CEN_CONH_2ND", cen_conh_2nd_val,
		"CEN_CONK", cen_conk_val,
		"CHN_CONA", chn_cona_val,
		"CHN_CONC", chn_conc_val);

	default_emi_setting->EMI_CONA_VAL = cen_cona_val;
	default_emi_setting->EMI_CONF_VAL = 0x0;
	default_emi_setting->EMI_CONH_VAL = cen_conh_val;
	default_emi_setting->EMI_CONH_2ND_VAL = cen_conh_2nd_val;
	default_emi_setting->EMI_CONK_VAL = cen_conk_val;
	default_emi_setting->CHN0_EMI_CONA_VAL = chn_cona_val;
	default_emi_setting->CHN1_EMI_CONA_VAL = chn_cona_val;
	default_emi_setting->CHN0_EMI_CONC_VAL = chn_conc_val;

	return 0;
}

#if !__ETT__
void reserve_emi_isu_buf(void)
{
	unsigned long long rsv_start;
	int emi_isu;

	emi_isu = atoi(dconfig_getenv("emi_isu"));
	/* to avoid CMA reserve space, need to allocate before 0x80000000 */
	if (emi_isu == 1) {
		rsv_start = mblock_reserve_ext(&bootarg.mblock_info,
			EMI_ISU_BUF_SIZE, EMI_ISU_BUF_SIZE,
			0x80000000, 0, "emi_isu_buf");
	} else if (emi_isu == 2) {
		rsv_start = 0ULL;
	} else {
#if CFG_LAST_EMI_BW_DUMP
		rsv_start = mblock_reserve_ext(&bootarg.mblock_info,
			EMI_ISU_BUF_SIZE, EMI_ISU_BUF_SIZE,
			0x80000000, 0, "emi_isu_buf");
#else
		rsv_start = 0ULL;
#endif
	}

#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	last_emi_info_ptr->isu_buf_l = (unsigned int) (rsv_start & 0xFFFFFFFF);
	last_emi_info_ptr->isu_buf_h = (unsigned int) (rsv_start >> 32);
#endif
}

void reserve_emi_mbist_buf(void)
{
	unsigned long long rsv_start;

	/* to avoid CMA reserve conflict, need to allocate before 0x80000000 */
	rsv_start = mblock_reserve_ext(&bootarg.mblock_info, 0x1000,
					0x1000, 0x80000000, 0, "emi_mbist_buf");

	emi_log("[EMI] mbist rsv_start=0x%llx\n", rsv_start);
	mt_emi_sync_write(EMI_AXI_BIST_ADR0, rsv_start - 0x40000000);
	mt_emi_sync_write(EMI_AXI_BIST_ADR1, rsv_start - 0x40000000 + 0xFFF);

	/* enable BIST on port 0 */
	mt_emi_sync_write(EMI_ARBJ_2ND, 0xC0000100);
}

void record_emi_snst(void)
{
#ifdef LAST_EMI
	if (!last_emi_info_ptr)
		last_emi_info_ptr =
			(LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);

	last_emi_info_ptr->snst_past = last_emi_info_ptr->snst_last;
	last_emi_info_ptr->snst_last = mt_emi_sync_read(EMI_SNST);
	emi_log("[EMI] SNST: 0x%x\n", last_emi_info_ptr->snst_last);

	/* clear EMI_SNST and set target master to M5 */
	mt_emi_sync_write(EMI_SNST, 0x85000000);
#endif
}

unsigned long long platform_memory_size(void)
{
	static unsigned long long mem_size = 0;
	int nr_rank;
	int i;
	unsigned long long rank_size[DRAMC_MAX_RK] = {0};

	if (!mem_size) {
		nr_rank = get_dram_rank_nr();

		get_dram_rank_size(rank_size);

		for (i = 0; i < nr_rank; i++)
			mem_size += rank_size[i];
	}

	return mem_size;
}
#endif


