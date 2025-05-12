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

#include <emi_hw.h>
#include <emi.h>
#include <pll.h>
#include <emi-addr2dram.h>

#ifdef LAST_EMI
static LAST_EMI_INFO_T* last_emi_info_ptr;
#endif

#ifdef AID_TABLE_ENABLE
#include "emi_aid_table.h"
#endif

void mt_emi_sync_write(unsigned int addr, unsigned int value)
{
                *((volatile unsigned int *)(addr)) = value;
                dsb();
}

unsigned int mt_emi_sync_read(unsigned int addr)
{
                dsb();
                return *((volatile unsigned int *)(addr));
}

void mt_emi_sync_write_or(unsigned int addr, unsigned int or_value)
{
                unsigned int value = mt_emi_sync_read(addr);
                mt_emi_sync_write(addr, value | or_value);
}

void emi_cen_config(unsigned cfg_base_addr)  {
//C:-------------------BEGIN))= EMI Setting--------------------
    unsigned EMI_APB_BASE = cfg_base_addr;
    unsigned int  rd_temp;

    //mt_emi_sync_write(EMI_APB_BASE+0x00000000,0x53535154); // 4CH config, row 18bit, per RANK 3GB, total 24GB
    mt_emi_sync_write(EMI_APB_BASE+0x00000000,0xa053a154); // 4CH config, row 15bit, per RANK 1GB, total 8GB



    // overhead:
    mt_emi_sync_write(EMI_APB_BASE+0x00000004,0x3a544a55); //7500 (1:16) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000008,0x25432f49); //7500 (1:16) r8  - r5  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000000c,0x1f492045); //7500 (1:16) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000010,0x23532255); //7500 (1:16) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000008b0,0x2038303e); //6400 (1:8) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000008b4,0x172e1930); //6400 (1:8) r8  - r5  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000001c,0x182d162d); //6400 (1:8) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000024,0x1d311b31); //6400 (1:8) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000034,0x1d352d3c); //5500 (1:8) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000006c,0x142a172d); //5500 (1:8) r8  - r5  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000013c,0x172a142a); //5500 (1:8) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000640,0x192b162b); //5500 (1:8) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000044,0x19322938); //4266 (1:8) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000074,0x12271329); //4266 (1:8) r8  - r5  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000001e0,0x14261228); //4266 (1:8) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000644,0x172a1428); //4266 (1:8) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000004c,0x172d2a3a); //3200 (1:8) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000084,0x10251227); //3200 (1:8) r8  - r5  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000001e4,0x12250e25); //3200 (1:8) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000648,0x35291127); //3200 (1:8) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000054,0x162b2939); //2667 (1:8) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000008c,0x0f231025); //2667 (1:8) r8  - r5  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000001e8,0x11240f21); //2667 (1:8) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000064c,0x15291328); //2667 (1:8) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000005c,0x14282838); //1866 (1:4) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000094,0x0f240f22); //1866 (1:4) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000001c8,0x1a2c112a); //1866 (1:4) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000660,0x263c2339); //1866 (1:4) r8  - r5  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000064,0x06190718); //800  (1:4) r12 - r9  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000009c,0x11280a20); //800  (1:4) r4  - r1  overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000001f4,0x203b1d31); //800  (1:4) r16 - r13 overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000664,0x263e2139); //800  (1:4) r8  - r5  overhead

    mt_emi_sync_write(EMI_APB_BASE+0x00000030,0x53515862); //7500 (1:16) r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000014,0x63635c56); //7500 (1:16) r16 - r10 non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000008b8,0x3b3c4046); //6400 (1:8)  r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000002c,0x4242403d); //6400 (1:8)  r16 - r10 non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000000c4,0x383a3d44); //5500 (1:8)  r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000668,0x3d3d3c39); //5500 (1:8)  r16 - r10 non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000000c8,0x35363a40); //4266 (1:8)  r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000066c,0x39393837); //4266 (1:8)  r16 - r10 non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000000cc,0x3336383d); //3200 (1:8)  r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000694,0x38383734); //3200 (1:8)  r16 - r10 non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000000e4,0x3133353b); //2667 (1:8)  r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000708,0x38383534); //2667 (1:8)  r16 - r10 non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x000000f4,0x62585051); //1866 (1:4)  r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000070c,0x8b8b7f71); //1866 (1:4)  r16 - r10 non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x0000012c,0x46362f2b); //800  (1:4)  r8  - r2  non-align overhead
    mt_emi_sync_write(EMI_APB_BASE+0x00000748,0x55554e52); //800  (1:4)  r16 - r10 non-align overhead


    mt_emi_sync_write(EMI_APB_BASE+0x00000018,0x3657587a);
    mt_emi_sync_write(EMI_APB_BASE+0x00000020,0x0000c042);  //sync to #15, async_mode_use_neg_clk[15]=1
    mt_emi_sync_write(EMI_APB_BASE+0x00000028,0x00800000);  //scramble, adr[14]=adr[19]^adr[14]
    mt_emi_sync_write(EMI_APB_BASE+0x000007b4,0x24000000);  //scramble, adr[15]=adr[20]^adr[15], adr[16]=adr[21]^[adr[16]
    mt_emi_sync_write(EMI_APB_BASE+0x00000038,0xcccc0083);  //row15bit/row 16bit
    mt_emi_sync_write(EMI_APB_BASE+0x0000003c,0x10073330);  //[5,8] LP5 16Bankor4BG , [4]64Byte_Mode_en [3]BG_Mode
    mt_emi_sync_write(EMI_APB_BASE+0x00000040,0x00008802);
    //Ori
    mt_emi_sync_write(EMI_APB_BASE+0x00000048,0x02000000); // SRAM_RDATA_REQ_EXPIRE_FIRST[25]=0x1 avoiding rdata stuck at SRAM
    //disable bypass for SEMU
    //mt_emi_sync_write(EMI_APB_BASE+0x00000048,0x02FF0000); // SRAM_RDATA_REQ_EXPIRE_FIRST[25]=0x1 avoiding rdata stuck at SRAM , dis_bypass[23:16]=0xff
    mt_emi_sync_write(EMI_APB_BASE+0x00000060,0x407812ff); // reserved buffer to normal read/write :8/7
    mt_emi_sync_write(EMI_APB_BASE+0x00000068,0x02000000); //FAST_PASS_slv_en[19:12]=0x0 (M0/M1 enable), dcm[25]=0x1 (must set1), [11] fp_pipe_en=0
    mt_emi_sync_write(EMI_APB_BASE+0x00000078,0x11120c1f);  //22:20=ultra_w=1
    mt_emi_sync_write(EMI_APB_BASE+0x00000710,0x11120c1f);  //22:20=ultra_w=1
    mt_emi_sync_write(EMI_APB_BASE+0x0000007c,0x00001123);
    mt_emi_sync_write(EMI_APB_BASE+0x00000718,0x00001123);
    mt_emi_sync_write(EMI_APB_BASE+0x000000d0,0xa8a8a8a8); //outstanding
    mt_emi_sync_write(EMI_APB_BASE+0x000000d4,0x25252525); //outstanding
    mt_emi_sync_write(EMI_APB_BASE+0x000000d8,0xa8a8a8a8); //outstanding
    mt_emi_sync_write(EMI_APB_BASE+0x000000dc,0x25252525); //outstanding
    mt_emi_sync_write(EMI_APB_BASE+0x000000e8,0x00062037); // initial starvation counter div2, [4]=1, short_req_dis[13]=0x1 ( disable)
    mt_emi_sync_write(EMI_APB_BASE+0x000000f0,0x384e0014); //[18]: HP_MOD
    mt_emi_sync_write(EMI_APB_BASE+0x000000f8,0xa0000004);
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
    mt_emi_sync_write(EMI_APB_BASE+0x00000150,0x090a4400); //FAST_PASS_os_limit[11:8]=0x4
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
    mt_emi_sync_write(EMI_APB_BASE+0x00000838,0x254dffff); //SRT Throttling Period Setting 1
    mt_emi_sync_write(EMI_APB_BASE+0x0000083c,0x9faec1c1); //MD Latency Monitor 0
    mt_emi_sync_write(EMI_APB_BASE+0x00000840,0x000003e8); //MD Latency Monitor 1
    mt_emi_sync_write(EMI_APB_BASE+0x00000844,0x000003e8); //MD Latency Monitor 2
    mt_emi_sync_write(EMI_APB_BASE+0x00000848,0x000003e8); //MD Latency Monitor 3
    mt_emi_sync_write(EMI_APB_BASE+0x0000084c,0x000003e8); //MD Latency Monitor 4
    mt_emi_sync_write(EMI_APB_BASE+0x00000850,0x00000000); //MD Latency Monitor 5
    mt_emi_sync_write(EMI_APB_BASE+0x00000854,0x00000000); //MD Latency Monitor 6
    mt_emi_sync_write(EMI_APB_BASE+0x00000858,0x02f22cff); //SRT Throttling Control 1, ignore rff threshold
    mt_emi_sync_write(EMI_APB_BASE+0x0000085c,0x00002785); //disable internal MD latency urgent mask
    mt_emi_sync_write(EMI_APB_BASE+0x00000874,0x00000320); //MD Latency Monitor 7
    mt_emi_sync_write(EMI_APB_BASE+0x00000878,0x00840000); //MD Latency Monitor 8
    mt_emi_sync_write(EMI_APB_BASE+0x0000087c,0x022f250a); //SRT Throttling Period Setting 3
    mt_emi_sync_write(EMI_APB_BASE+0x00000890,0xffff3c59);
    mt_emi_sync_write(EMI_APB_BASE+0x00000894,0xffff00ff);
    mt_emi_sync_write(EMI_APB_BASE+0x000008a0,0xffffffff);
    mt_emi_sync_write(EMI_APB_BASE+0x000008a4,0x0000ffff);
    mt_emi_sync_write(EMI_APB_BASE+0x000008c0,0x00000320); //MD Latency Monitor 9
    mt_emi_sync_write(EMI_APB_BASE+0x000008c4,0x00560000); //MD Latency Monitor 10
    mt_emi_sync_write(EMI_APB_BASE+0x000008c8,0x00000190); //MD Latency Monitor 11
    mt_emi_sync_write(EMI_APB_BASE+0x000008cc,0x00360000); //MD Latency Monitor 12
    mt_emi_sync_write(EMI_APB_BASE+0x000008d0,0x000000c8); //MD Latency Monitor 13
    mt_emi_sync_write(EMI_APB_BASE+0x000008d4,0x00360000); //MD Latency Monitor 14
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
    mt_emi_sync_write(EMI_APB_BASE+0x000009f8,0x000003e8); //MD Latency Monitor 29
    mt_emi_sync_write(EMI_APB_BASE+0x000009fc,0x000003e8); //MD Latency Monitor 30
    mt_emi_sync_write(EMI_APB_BASE+0x00000b00,0x000003e8); //MD Latency Monitor 31
    mt_emi_sync_write(EMI_APB_BASE+0x00000b04,0x000003e8); //MD Latency Monitor 32
    mt_emi_sync_write(EMI_APB_BASE+0x00000b08,0x00000320); //MD Latency Monitor 33
    mt_emi_sync_write(EMI_APB_BASE+0x00000b0c,0x00000320); //MD Latency Monitor 34
    mt_emi_sync_write(EMI_APB_BASE+0x00000b10,0x00000190); //MD Latency Monitor 35
    mt_emi_sync_write(EMI_APB_BASE+0x00000b14,0x000000c8); //MD Latency Monitor 36
    mt_emi_sync_write(EMI_APB_BASE+0x00000b18,0x00000000); //MD Latency Monitor 37
    mt_emi_sync_write(EMI_APB_BASE+0x00000b1c,0x00000000); //MD Latency Monitor 38
    mt_emi_sync_write(EMI_APB_BASE+0x00000b20,0x00000000); //MD Latency Monitor 39
    mt_emi_sync_write(EMI_APB_BASE+0x00000b24,0x00000000); //MD Latency Monitor 40
    mt_emi_sync_write(EMI_APB_BASE+0x00000b28,0x9faec1c1); //MD Latency Monitor 41
    mt_emi_sync_write(EMI_APB_BASE+0x00000b2c,0x36365984); //MD Latency Monitor 42

    mt_emi_sync_write(EMI_APB_BASE+0x00000b60,0x181e282f);
    mt_emi_sync_write(EMI_APB_BASE+0x00000b64,0x14140f18);
    mt_emi_sync_write(EMI_APB_BASE+0x00000b68,0x000003e8); //MD Latency Monitor 57
    mt_emi_sync_write(EMI_APB_BASE+0x00000b6c,0x000003e8); //MD Latency Monitor 58
    mt_emi_sync_write(EMI_APB_BASE+0x00000b70,0x000003e8); //MD Latency Monitor 59
    mt_emi_sync_write(EMI_APB_BASE+0x00000b74,0x000003e8); //MD Latency Monitor 60
    mt_emi_sync_write(EMI_APB_BASE+0x00000b78,0x00000320); //MD Latency Monitor 61
    mt_emi_sync_write(EMI_APB_BASE+0x00000b7c,0x00000320); //MD Latency Monitor 62
    mt_emi_sync_write(EMI_APB_BASE+0x00000b80,0x00000190); //MD Latency Monitor 63
    mt_emi_sync_write(EMI_APB_BASE+0x00000b84,0x000000c8); //MD Latency Monitor 64
    mt_emi_sync_write(EMI_APB_BASE+0x00000b88,0x00000000); //MD Latency Monitor 65
    mt_emi_sync_write(EMI_APB_BASE+0x00000b8c,0x00000000); //MD Latency Monitor 66
    mt_emi_sync_write(EMI_APB_BASE+0x00000b90,0x00000000); //MD Latency Monitor 67
    mt_emi_sync_write(EMI_APB_BASE+0x00000b94,0x00000000); //MD Latency Monitor 68
    mt_emi_sync_write(EMI_APB_BASE+0x00000b98,0xd4e8ffff); //MD Latency Monitor 69
    mt_emi_sync_write(EMI_APB_BASE+0x00000b9c,0x505084b0); //MD Latency Monitor 70
    mt_emi_sync_write(EMI_APB_BASE+0x00000bd0,0x01010101);
    mt_emi_sync_write(EMI_APB_BASE+0x00000bd4,0x01010101);
    mt_emi_sync_write(EMI_APB_BASE+0x00000bd8,0x000003e8); //MD Latency Monitor 85
    mt_emi_sync_write(EMI_APB_BASE+0x00000bdc,0x000003e8); //MD Latency Monitor 86
    mt_emi_sync_write(EMI_APB_BASE+0x00000be0,0x000003e8); //MD Latency Monitor 87
    mt_emi_sync_write(EMI_APB_BASE+0x00000be4,0x000003e8); //MD Latency Monitor 88
    mt_emi_sync_write(EMI_APB_BASE+0x00000be8,0x00000320); //MD Latency Monitor 89
    mt_emi_sync_write(EMI_APB_BASE+0x00000bec,0x00000320); //MD Latency Monitor 90
    mt_emi_sync_write(EMI_APB_BASE+0x00000bf0,0x00000190); //MD Latency Monitor 91
    mt_emi_sync_write(EMI_APB_BASE+0x00000bf4,0x00000000); //MD Latency Monitor 92
    mt_emi_sync_write(EMI_APB_BASE+0x00000bf8,0x00000000); //MD Latency Monitor 93
    mt_emi_sync_write(EMI_APB_BASE+0x00000bfc,0x00000000); //MD Latency Monitor 94
    mt_emi_sync_write(EMI_APB_BASE+0x00000c00,0x00000000); //MD Latency Monitor 95
    mt_emi_sync_write(EMI_APB_BASE+0x00000c04,0x00000000); //MD Latency Monitor 96
    mt_emi_sync_write(EMI_APB_BASE+0x00000c08,0xd4e8ffff); //MD Latency Monitor 97
    mt_emi_sync_write(EMI_APB_BASE+0x00000c0c,0x505084b0); //MD Latency Monitor 98
    mt_emi_sync_write(EMI_APB_BASE+0x00000c40,0x01010101); //SRT Throttling Period Setting 4
    mt_emi_sync_write(EMI_APB_BASE+0x00000c44,0x01010101);
    mt_emi_sync_write(EMI_APB_BASE+0x00000c48,0x00110000);
    mt_emi_sync_write(EMI_APB_BASE+0x00000c4c,0x300ff025);       //ignore wff threshold
    mt_emi_sync_write(EMI_APB_BASE+0x00000c80,0x000003e8); //MD Latency Monitor 113
    mt_emi_sync_write(EMI_APB_BASE+0x00000c84,0x000003e8); //MD Latency Monitor 114
    mt_emi_sync_write(EMI_APB_BASE+0x00000c88,0x000003e8); //MD Latency Monitor 115
    mt_emi_sync_write(EMI_APB_BASE+0x00000c8c,0x000003e8); //MD Latency Monitor 116
    mt_emi_sync_write(EMI_APB_BASE+0x00000c90,0x00000320); //MD Latency Monitor 117
    mt_emi_sync_write(EMI_APB_BASE+0x00000c94,0x00000320); //MD Latency Monitor 118
    mt_emi_sync_write(EMI_APB_BASE+0x00000c98,0x00000190); //MD Latency Monitor 119
    mt_emi_sync_write(EMI_APB_BASE+0x00000c9c,0x000000c8); //MD Latency Monitor 120
    mt_emi_sync_write(EMI_APB_BASE+0x00000cb0,0x9faec1c1); //MD Latency Monitor 125
    mt_emi_sync_write(EMI_APB_BASE+0x00000cb4,0x36365684); //MD Latency Monitor 126
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
    mt_emi_sync_write(EMI_APB_BASE+0x00000df8,0x00dd03dd); //EMI gal setting : DCM_EN[9,8]=0x1, RCMD_AFIFO_AF_LVL:CHB[7:4]=CHA[3:0]=0xd ,WDAT_AFIFO_AF_LVL:CHB[23:20]=CHA[19:16] = 0xd          (AUSTIN said follow coda constraint 2021/01/21)
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
    mt_emi_sync_write(EMI_APB_BASE+0x0000039c,0x001ffd85); // [8] near_full enable
    mt_emi_sync_write(EMI_APB_BASE+0x000003a4,0xffffffff); // msk

    mt_emi_sync_write(EMI_APB_BASE+0x000003d8,0xffffffff);
    mt_emi_sync_write(EMI_APB_BASE+0x000003dc,0x001ffc85);
    mt_emi_sync_write(EMI_APB_BASE+0x000003e0,0xffffffff); // msk

    mt_emi_sync_write(EMI_APB_BASE+0x000003fc,0xffffffff);
    mt_emi_sync_write(EMI_APB_BASE+0x0000040c,0x001ffc85);
    mt_emi_sync_write(EMI_APB_BASE+0x00000414,0xffffffff); // msk

    mt_emi_sync_write(EMI_APB_BASE+0x0000044c,0xffffffff);
    mt_emi_sync_write(EMI_APB_BASE+0x00000454,0x001ffd85); // [8] near_full enable
    mt_emi_sync_write(EMI_APB_BASE+0x0000045c,0xffffffff); // msk

    mt_emi_sync_write(EMI_APB_BASE+0x0000049c,0xffffffff);
    mt_emi_sync_write(EMI_APB_BASE+0x000004a4,0x001ffc85);
    mt_emi_sync_write(EMI_APB_BASE+0x000004ac,0xffffffff); // msk

    mt_emi_sync_write(EMI_APB_BASE+0x0000050c,0xffffffff);
    mt_emi_sync_write(EMI_APB_BASE+0x00000514,0x001ffc85);
    mt_emi_sync_write(EMI_APB_BASE+0x0000051c,0xffffffff); // msk


    //weilun for new feature
    mt_emi_sync_write(EMI_APB_BASE+0x00000714,0x00000000); // chuan - 00 -> fc (set 1 for freq that below 2400)

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

    // disable EMI DCM
    //mt_emi_sync_write(EMI_APB_BASE+0x00000060,0xff7812ff); // set EMI_CONM[31:24] = 0xff, to disable emi dcm@iput, by alex 2020.10.04

    // hash rule
    mt_emi_sync_write_or(EMI_APB_BASE+0x000007a4,0xc0000000);

    //Disable read interleave for m0/m1
    mt_emi_sync_write(EMI_APB_BASE+0x0000074c,0x3);
    //mt_emi_sync_write(SUB_EMI_APB_BASE+0x0000074c,0x3);

    //EMI SELF TEST
    mt_emi_sync_write(EMI_APB_BASE+0x0000014c,0xc0000100);
    mt_emi_sync_write(EMI_APB_BASE+0x00000990,0x0000ffff);
    mt_emi_sync_write(EMI_APB_BASE+0x00000994,0x00000000);

    //[5:0] command stall treshold; [7] async half_en
    //mt_emi_sync_write_or(CHN0_EMI_APB_BASE+0x00000300,((0x1<<7)|(0xC<<0)));

    //near_full M2/M5
    mt_emi_sync_write(EMI_APB_BASE+0x00000524,0x00002424);
    mt_emi_sync_write(EMI_APB_BASE+0x00000364,0x00002000);
    mt_emi_sync_write(EMI_APB_BASE+0x00000374,0x20002000);
    mt_emi_sync_write(EMI_APB_BASE+0x0000041c,0x00002000);
    mt_emi_sync_write(EMI_APB_BASE+0x0000042c,0x20002000);

    //SLC_GLOBAL_EN
    #if 1//def SLC_SLB_ENABLE_AND_DEFAULT_SLB
    // SLB vi EMI
    mt_emi_sync_write(EMI_APB_BASE+0x000003B0,0x68000);   // SLB start address : 0x6_8000_0000 which is 26GB
    mt_emi_sync_write(EMI_APB_BASE+0x000003B8,0x6805F);   // SLB end address   : 0x6_805F_FFFF
    // SLB dedicated bus
    mt_emi_sync_write(EMI_MPU_APB_BASE+0x000,0x0);
    mt_emi_sync_write(EMI_MPU_APB_BASE+0x6a0,0xff);
    mt_emi_sync_write(EMI_MPU_APB_BASE+0x680,0x8);
    mt_emi_sync_write(EMI_MPU_APB_BASE+0x000,0x1);    //apb protection
   //SLB MPU enable
    mt_emi_sync_write_or(EMI_MPU_APB_BASE+0x680,0xf0000); //vio_rd, vio_wr, vio_way_en

    //disable rdf
    mt_emi_sync_write(EMI_APB_BASE+0x000003A8,0xFF);  // SLC RDF
    mt_emi_sync_write(EMI_APB_BASE+0x00000300,0x30000);

    // North/South EMI SLC no support broadcast!!!!!
    mt_emi_sync_write(SLC_APB_BASE+0x004,0xFF0443);  //SLC_EN[0] =1 , rcmd_abort_en[1]=0x1 , rfifo_remain_stop_thr[7:4]=0x4 , wfifo_eremain_stop_thr[11:8] = 0x4         , dcm[23:16]=0x0
    mt_emi_sync_write(SUB_SLC_APB_BASE+0x004,0xFF0443);

    mt_emi_sync_write(SLC_APB_BASE+0x010,0x0000); //WAY15~WAY0 disable
    mt_emi_sync_write(SUB_SLC_APB_BASE+0x010,0x0000); //WAY15~WAY0 disable

    mt_emi_sync_write(SLC_APB_BASE+0x00C,0x1);    //trigger way disable trigger
    mt_emi_sync_write(SUB_SLC_APB_BASE+0x00C,0x1);    //trigger way disable trigger

    mt_emi_sync_write(SLC_APB_BASE+0x004,0x443);  //Enable DCM
    mt_emi_sync_write(SUB_SLC_APB_BASE+0x004,0x443);  //Enable DCM

    mt_emi_sync_write(SLC_APB_BASE+0x008,0x87007071); //sw_chg_credit[31]=0x1, slb_prio[26:24]=0x7, r_prio[14:12]=0x7, r_credit[6:4]=0x7, w_credit[2:0]=0x1
    mt_emi_sync_write(SUB_SLC_APB_BASE+0x008,0x87007071); //sw_chg_credit[31]=0x1, slb_prio[26:24]=0x7, r_prio[14:12]=0x7, r_credit[6:4]=0x7, w_credit[2:0]=0x1

    //slc parity check enable
    mt_emi_sync_write_or(SLC_APB_BASE+0x000,0x1000);  //Enable SRAM parity [12]
    mt_emi_sync_write_or(SUB_SLC_APB_BASE+0x000,0x1000);  //Enable SRAM parity [12]
    mt_emi_sync_write_or(SLC_APB_BASE+0x014,0x2);  //Enable SRAM parity check [1]
    mt_emi_sync_write_or(SUB_SLC_APB_BASE+0x014,0x2);  //Enable SRAM parity check [1]

    //slc write command afifo workaround
    //mt_emi_sync_write_or(EMI_APB_BASE+0xDF8,((0x3<<0)|(0x3<<4)|(0xe<<16)|(0xe<<20)));
    #endif
 //C:-------------------END))= EMI Setting--------------------;

}//void emi_cen_config()


void emi_chn_config(unsigned cfg_base_addr)  {
      unsigned EMI_CHANNEL_APB_BASE = cfg_base_addr;

      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000000,0x04cc5c5d); // 4CH config, row 18bit, per RANK 3GB, total 24GB



      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000008,0x50ff604A); //age_speed[13:12] sync to , dramc_queue[7:0]=0xA, chn_afifo_cg_dis= 0x1, afifo/arb dcm disable
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000010,0x008420C4); //chn scramble = 1'b1; adr[12] = adr[12]^ adr[17] ; adr[13] = adr[13]^ adr[18] ; adr[14] = adr[14]^ adr[19] ;
      #if 1//ndef EMI_BRING_UP
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000014,0x00010001); //FAST_PASS_EN[28]=0x0 , chn scramble ;  adr[8] = adr[8] ^ adr[16];
      #else
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000014,0x00010001); //FAST_PASS_EN[28]=0x0 , chn scramble ;  adr[8] = adr[8] ^ adr[16];
      #endif
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000018,0x99f08c03);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000020,0x00000184); //MAX_STARV_CNT[5:0]=EBG_STOP_THRE[13:8]=0x4 , EBG_DROP_RULE[7]=0x1 (drop oldest)
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000710,0x9a508c17); // [24:20] = 0x2 : bank throttling (default=0x01f00000)
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000048,0x0003A837); //RD_INORDER_THR2[27,31:28]=0x0 , RD_INORDER_THR[20:16]=0x3  [11]64Byte_Mode_en=0x1  short_req_dis[13]=0x1
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000050,0x3F460002); // [1] : MD_RD_AFT_WR_EN, DEFER_WR_WDATA_THR[27:24] = 0xF ( Lp5 push new write command by seq_wdata_cnt < 0xF*4 )
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000058,0x00000000); //original full rdf use
      //mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000058,0x00000800); //chn disable 8 level rdf for slc
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000090,0x000002ff);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000098,0x00003111); // HIT_THR=0x3 (DOE1) , MIS_THR=0x1, MRW_THR=0x1, DQFR_EN=0x1 //LP5 HIT_THR to increase Bankgroup diversity
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000140,0x22637188); // [19:16]=0x3, diffbk_closes_priority (DOE6)
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000144,0x22607188);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000148,0x37ff5f5e); // chuan (DOE4)
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000014c,0x2719595e); // chuan
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000150,0x64f37f79); // chuan , reg_serv_cnt_clr_by_cto[15]=0x1
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000154,0x64f3ff79); // chuan // update timeout settin    g: bit 12~15 (Austin's design change in ; has bug; setting need change in C        onti)
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000158,0x011b0868);


      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000015c,0xa74142a2); // Stop urgent read firs    t when write command buffer remain < 7, [31] ultra_read_first, [30:28] wr_rsv_thr_l, [27:            24] wr_rsv_thr_h,
                                                                     // 0x15c[7] = 1, update by Chuan liu to promote page hit priority@201119
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000016c,0x0000f801);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000170,0x40000000);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b0,0x000c804f); // Rank-Aware arbitration  , sw_thr[7:4] = 0x4 for LP5
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b4,0xbd3f3f7e); // Rank-Aware arbitration
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b8,0x7e003d7e); // Rank-Aware arbitration
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000300,0x0000DADA); // CHN_EMI_GALS_CON , AFIFO_AF_LV[5:0],[13:8]=0x1A, async_mode_use_neg_clk[15,7] = 1 , DCM_EN[14,6] = 0x1
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000003fc,0x00000000); // Write M17_toggle_mask = 0
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000080,0xaa0148ff);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000088,0xaa6168ff);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000404,0xaa516cff); // chuan - [9], [12]
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000408,0xaa0140ff);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000040c,0x9f658633);
      mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000600,0x000000ff); //SLC rdf disable

      //mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000500,0x00000020); //design default value

}//void emi_chn_config()

unsigned int gen_rank0_size (unsigned int reg_chn_emi_cona)
{
    // chn_emi_cona
    unsigned int dw32_en                = (reg_chn_emi_cona >>  1) & ((0x1 << 1) - 1);
    unsigned int row_ext0               = (reg_chn_emi_cona >>  2) & ((0x1 << 1) - 1);
    unsigned int col                    = (reg_chn_emi_cona >>  4) & ((0x1 << 2) - 1);
    unsigned int rank0_size_ext         = (reg_chn_emi_cona >>  8) & ((0x1 << 1) - 1);
    unsigned int chn_bg_16bank_mode     = (reg_chn_emi_cona >> 10) & ((0x1 << 1) - 1);
    unsigned int row                    = (reg_chn_emi_cona >> 12) & ((0x1 << 2) - 1);
    unsigned int rank0_size             = (reg_chn_emi_cona >> 16) & ((0x1 << 4) - 1);
    unsigned int chn_4bank_mode         = (reg_chn_emi_cona >> 24) & ((0x1 << 1) - 1);
    // rank0 size
    unsigned int rank0_row_width  = row_ext0*4 + row + 13;
    unsigned int rank0_bank_width = (chn_bg_16bank_mode == 1)? 4 :
                           (chn_4bank_mode     == 1)? 2 : 3;
    unsigned int rank0_col_width  = col + 9;
    unsigned int rank0_size_from_cona_MB = 1 << (rank0_row_width + rank0_bank_width + rank0_col_width + dw32_en + 1 - 20);
    unsigned int rank0_size_from_reg_MB  = (rank0_size_ext*16 + rank0_size) * 256;
    unsigned int rank0_size_MB           = (rank0_size_from_reg_MB == 0)? rank0_size_from_cona_MB : rank0_size_from_reg_MB;

    return rank0_size_MB;
}

void ebg_addr_decode(unsigned int* reg_ebg_addr_decode_0, unsigned int* reg_ebg_addr_decode_1,
                      unsigned int* reg_ebg_addr_decode_2, unsigned int* reg_ebg_addr_decode_3)
{
    // init read register
    unsigned int reg_cen_emi_scrm_ext  = mt_emi_sync_read(EMI_BASE + 0x7b4);
    unsigned int reg_cen_emi_cona      = mt_emi_sync_read(EMI_BASE);
    unsigned int reg_cen_emi_conf      = mt_emi_sync_read(EMI_BASE + 0x028);
    unsigned int reg_cen_emi_conh_2nd  = mt_emi_sync_read(EMI_BASE + 0x03c);
    unsigned int reg_cen_emi_bank_msk0 = mt_emi_sync_read(EMI_BASE + 0x7c0);
    unsigned int reg_cen_emi_bank_msk1 = mt_emi_sync_read(EMI_BASE + 0x7c4);
    unsigned int reg_chn_emi_cona      = mt_emi_sync_read(CHN0_EMI_BASE);
    unsigned int reg_chn_emi_conc      = mt_emi_sync_read(CHN0_EMI_BASE + 0x10);
    unsigned int reg_chn_emi_conc_2nd  = mt_emi_sync_read(CHN0_EMI_BASE + 0x14);

    emi_log("[EMI] reg_cen_emi_scrm_ext: 0x%x\n", reg_cen_emi_scrm_ext);
    emi_log("[EMI] reg_cen_emi_cona: 0x%x\n", reg_cen_emi_cona);
    emi_log("[EMI] reg_cen_emi_conf: 0x%x\n", reg_cen_emi_conf);
    emi_log("[EMI] reg_cen_emi_conh_2nd: 0x%x\n", reg_cen_emi_conh_2nd);
    emi_log("[EMI] reg_cen_emi_bank_msk0: 0x%x\n", reg_cen_emi_bank_msk0);
    emi_log("[EMI] reg_cen_emi_bank_msk1: 0x%x\n", reg_cen_emi_bank_msk1);
    emi_log("[EMI] reg_chn_emi_cona: 0x%x\n", reg_chn_emi_cona);
    emi_log("[EMI] reg_chn_emi_conc: 0x%x\n", reg_chn_emi_conc);
    emi_log("[EMI] reg_chn_emi_conc_2nd: 0x%x\n", reg_chn_emi_conc_2nd);

    unsigned int cen_bit6_scramble     = (reg_cen_emi_scrm_ext >>  0) & ((0x1 << 6) - 1);
    unsigned int cen_bit7_scramble     = (reg_cen_emi_scrm_ext >>  6) & ((0x1 << 6) - 1);
    unsigned int cen_bit8_scramble     = (reg_cen_emi_conh_2nd >> 24) & ((0x1 << 4) - 1);
    unsigned int cen_bit9_scramble     = (reg_cen_emi_conf     >>  0) & ((0x1 << 4) - 1);
    unsigned int cen_bit10_scramble    = (reg_cen_emi_conf     >>  4) & ((0x1 << 4) - 1);
    unsigned int cen_bit11_scramble    = (reg_cen_emi_conf     >>  8) & ((0x1 << 4) - 1);
    unsigned int cen_bit12_scramble    = (reg_cen_emi_conf     >> 12) & ((0x1 << 4) - 1);
    unsigned int cen_bit13_scramble    = (reg_cen_emi_conf     >> 16) & ((0x1 << 4) - 1);
    unsigned int cen_bit14_scramble    = (reg_cen_emi_conf     >> 20) & ((0x1 << 4) - 1);
    unsigned int cen_bit15_scramble    = (reg_cen_emi_conf     >> 24) & ((0x1 << 4) - 1);
    unsigned int cen_bit16_scramble    = (reg_cen_emi_conf     >> 28) & ((0x1 << 4) - 1);

    unsigned int cen_bit8_scramble_ext  = (reg_cen_emi_scrm_ext >> 12) & ((0x1 << 2) - 1);
    unsigned int cen_bit9_scramble_ext  = (reg_cen_emi_scrm_ext >> 14) & ((0x1 << 2) - 1);
    unsigned int cen_bit10_scramble_ext = (reg_cen_emi_scrm_ext >> 16) & ((0x1 << 2) - 1);
    unsigned int cen_bit11_scramble_ext = (reg_cen_emi_scrm_ext >> 18) & ((0x1 << 2) - 1);
    unsigned int cen_bit12_scramble_ext = (reg_cen_emi_scrm_ext >> 20) & ((0x1 << 2) - 1);
    unsigned int cen_bit13_scramble_ext = (reg_cen_emi_scrm_ext >> 22) & ((0x1 << 2) - 1);
    unsigned int cen_bit14_scramble_ext = (reg_cen_emi_scrm_ext >> 24) & ((0x1 << 2) - 1);
    unsigned int cen_bit15_scramble_ext = (reg_cen_emi_scrm_ext >> 26) & ((0x1 << 2) - 1);
    unsigned int cen_bit16_scramble_ext = (reg_cen_emi_scrm_ext >> 28) & ((0x1 << 2) - 1);

    // cen_scramble
    unsigned int cen_bit_6_scramble_rule  = cen_bit6_scramble;
    unsigned int cen_bit_7_scramble_rule  = cen_bit7_scramble;
    unsigned int cen_bit_8_scramble_rule  = cen_bit8_scramble_ext *16 + cen_bit8_scramble;
    unsigned int cen_bit_9_scramble_rule  = cen_bit9_scramble_ext *16 + cen_bit9_scramble;
    unsigned int cen_bit_10_scramble_rule = cen_bit10_scramble_ext*16 + cen_bit10_scramble;
    unsigned int cen_bit_11_scramble_rule = cen_bit11_scramble_ext*16 + cen_bit11_scramble;
    unsigned int cen_bit_12_scramble_rule = cen_bit12_scramble_ext*16 + cen_bit12_scramble;
    unsigned int cen_bit_13_scramble_rule = cen_bit13_scramble_ext*16 + cen_bit13_scramble;
    unsigned int cen_bit_14_scramble_rule = cen_bit14_scramble_ext*16 + cen_bit14_scramble;
    unsigned int cen_bit_15_scramble_rule = cen_bit15_scramble_ext*16 + cen_bit15_scramble;
    unsigned int cen_bit_16_scramble_rule = cen_bit16_scramble_ext*16 + cen_bit16_scramble;

    // chn_emi
    unsigned int chn_scrm_mode              = (reg_chn_emi_conc >>  7) & ((0x1 << 1) - 1);
    unsigned int chn_bit11_scramble         = (reg_chn_emi_conc >>  8) & ((0x1 << 4) - 1);
    unsigned int chn_bit12_scramble         = (reg_chn_emi_conc >> 12) & ((0x1 << 4) - 1);
    unsigned int chn_bit13_scramble         = (reg_chn_emi_conc >> 16) & ((0x1 << 4) - 1);
    unsigned int chn_bit14_scramble         = (reg_chn_emi_conc >> 20) & ((0x1 << 4) - 1);
    unsigned int chn_bit15_scramble         = (reg_chn_emi_conc >> 24) & ((0x1 << 4) - 1);
    unsigned int chn_bit16_scramble         = (reg_chn_emi_conc >> 28) & ((0x1 << 4) - 1);

    // chn_emi_conc_2nd
    unsigned int chn_bit11_scramble_ext     = (reg_chn_emi_conc_2nd >>  4) & ((0x1 << 2) - 1);
    unsigned int chn_bit12_scramble_ext     = (reg_chn_emi_conc_2nd >>  6) & ((0x1 << 2) - 1);
    unsigned int chn_bit13_scramble_ext     = (reg_chn_emi_conc_2nd >>  8) & ((0x1 << 2) - 1);
    unsigned int chn_bit14_scramble_ext     = (reg_chn_emi_conc_2nd >> 10) & ((0x1 << 2) - 1);
    unsigned int chn_bit15_scramble_ext     = (reg_chn_emi_conc_2nd >> 12) & ((0x1 << 2) - 1);
    unsigned int chn_bit16_scramble_ext     = (reg_chn_emi_conc_2nd >> 14) & ((0x1 << 2) - 1);
    unsigned int chn_bit8_scramble          = (reg_chn_emi_conc_2nd >> 16) & ((0x1 << 6) - 1);

    // chn_scramble
    unsigned int chn_bit_8_scramble_rule    = chn_bit8_scramble;
    unsigned int chn_bit_11_scramble_rule   = chn_bit11_scramble_ext*16 + chn_bit11_scramble;
    unsigned int chn_bit_12_scramble_rule   = chn_bit12_scramble_ext*16 + chn_bit12_scramble;
    unsigned int chn_bit_13_scramble_rule   = chn_bit13_scramble_ext*16 + chn_bit13_scramble;
    unsigned int chn_bit_14_scramble_rule   = chn_bit14_scramble_ext*16 + chn_bit14_scramble;
    unsigned int chn_bit_15_scramble_rule   = chn_bit15_scramble_ext*16 + chn_bit15_scramble;
    unsigned int chn_bit_16_scramble_rule   = chn_bit16_scramble_ext*16 + chn_bit16_scramble;

    // output
    unsigned int out_chn_scrm_mode     = chn_scrm_mode;
    unsigned int out_bit6_scramble     = 0; //(chn_scrm_mode == 1)? 0                        : cen_bit_6_scramble_rule;
    unsigned int out_bit7_scramble     = 0; //(chn_scrm_mode == 1)? 0                        : cen_bit_7_scramble_rule;
    unsigned int out_bit8_scramble     = 0; //(chn_scrm_mode == 1)? chn_bit_8_scramble_rule  : cen_bit_8_scramble_rule;
    unsigned int out_bit9_scramble     = 0; //(chn_scrm_mode == 1)? 0                        : cen_bit_9_scramble_rule;
    unsigned int out_bit10_scramble    = 0; //(chn_scrm_mode == 1)? 0                        : cen_bit_10_scramble_rule;
    unsigned int out_bit11_scramble    = (chn_scrm_mode == 1)? chn_bit_11_scramble_rule : cen_bit_11_scramble_rule;
    unsigned int out_bit12_scramble    = (chn_scrm_mode == 1)? chn_bit_12_scramble_rule : cen_bit_12_scramble_rule;
    unsigned int out_bit13_scramble    = (chn_scrm_mode == 1)? chn_bit_13_scramble_rule : cen_bit_13_scramble_rule;
    unsigned int out_bit14_scramble    = (chn_scrm_mode == 1)? chn_bit_14_scramble_rule : cen_bit_14_scramble_rule;
    unsigned int out_bit15_scramble    = (chn_scrm_mode == 1)? chn_bit_15_scramble_rule : cen_bit_15_scramble_rule;
    unsigned int out_bit16_scramble    = (chn_scrm_mode == 1)? chn_bit_16_scramble_rule : cen_bit_16_scramble_rule;

    //APMCU_ADDR_DECODE_0
    unsigned int rank0_size = gen_rank0_size(reg_chn_emi_cona);
    unsigned int rank0_size_ebg = (rank0_size >> 7); // 20 - 27
    *reg_ebg_addr_decode_0  =  reg_cen_emi_bank_msk0;
    *reg_ebg_addr_decode_0 |= (rank0_size_ebg << 18); // reg_rank_decode

    //APMCU_ADDR_DECODE_1
    *reg_ebg_addr_decode_1  = reg_cen_emi_bank_msk1;
    *reg_ebg_addr_decode_1 &= ~(0x1 << 0);                                // reg_chn_en[0]
    *reg_ebg_addr_decode_1 |= (reg_cen_emi_cona >> 8) & ((0x1 << 1) - 1); // reg_chn_en[0]

    *reg_ebg_addr_decode_1 &= ~(0x7 << 5);                                // reg_remap_shift
    *reg_ebg_addr_decode_1 |=  (0x3 << 5);                                // reg_remap_shift

    *reg_ebg_addr_decode_1 &= ~(0x3f << 24);                              // reg_bit6_scrm_in
    *reg_ebg_addr_decode_1 |=  (out_bit6_scramble << 24);                 // reg_bit6_scrm_in

    //APMCU_ADDR_DECODE_2
    *reg_ebg_addr_decode_2 |= (out_bit8_scramble  <<  0); // reg_bit8_scrm
    *reg_ebg_addr_decode_2 |= (out_bit9_scramble  <<  6); // reg_bit9_scrm
    *reg_ebg_addr_decode_2 |= (out_bit10_scramble << 12); // reg_bit10_scrm
    *reg_ebg_addr_decode_2 |= (out_bit11_scramble << 18); // reg_bit11_scrm
    *reg_ebg_addr_decode_2 |= (out_bit12_scramble << 24); // reg_bit12_scrm

    //APMCU_ADDR_DECODE_3
    *reg_ebg_addr_decode_3 |= (out_bit13_scramble  <<  0); // reg_bit13_scrm
    *reg_ebg_addr_decode_3 |= (out_bit14_scramble  <<  6); // reg_bit14_scrm
    *reg_ebg_addr_decode_3 |= (out_bit15_scramble  << 12); // reg_bit15_scrm
    *reg_ebg_addr_decode_3 |= (out_bit16_scramble  << 18); // reg_bit16_scrm
    *reg_ebg_addr_decode_3 |= (out_bit7_scramble   << 24); // reg_bit7_scrm
    *reg_ebg_addr_decode_3 |= (out_chn_scrm_mode   << 30); // reg_emi_scrmblr, 0: cen_emi, 1: chn_emi
    *reg_ebg_addr_decode_3 |= (0x1U                << 31); // reg_scrm_mode, scramble enable
}

void update_earlycke_setting(void)
{
    unsigned int reg_chn_emi_cona      = mt_emi_sync_read(CHN0_EMI_BASE);
    // init write register
    unsigned int reg_ebg_addr_decode_0 = 0x0;
    unsigned int reg_ebg_addr_decode_1 = 0x0;
    unsigned int reg_ebg_addr_decode_2 = 0x0;
    unsigned int reg_ebg_addr_decode_3 = 0x0;
    unsigned int reg_early_cke_addr_decode_0 = 0x0;

    ebg_addr_decode(&reg_ebg_addr_decode_0,&reg_ebg_addr_decode_1,&reg_ebg_addr_decode_2,&reg_ebg_addr_decode_3);

    //APMCU_ADDR_DECODE_0
    unsigned int rank0_size = gen_rank0_size(reg_chn_emi_cona);
    unsigned int rank0_size_ebg = (rank0_size >> 7); // 20 - 27

    //APMCU_EMI_EARLY_CKE_CTL0
    unsigned int dual_rank_en = (reg_ebg_addr_decode_0 >> 16) & ((0x1 << 1) - 1);
    unsigned int chn_loc      = (reg_ebg_addr_decode_1 >>  1) & ((0x1 << 1) - 1);
    unsigned int chn_pos      = (reg_ebg_addr_decode_1 >>  3) & ((0x1 << 2) - 1);
    unsigned int remap_shift  = (reg_ebg_addr_decode_1 >>  5) & ((0x1 << 3) - 1);
    unsigned int chn_hash_en  = (reg_ebg_addr_decode_1 >>  8) & ((0x1 << 4) - 1);
    unsigned int emi_disp_en  = (reg_ebg_addr_decode_1 >> 20) & ((0x1 << 4) - 1);

    unsigned int two_chn_en  = (chn_hash_en == 0)? 0 : 1;
    unsigned int four_chn_en = (emi_disp_en == 0)? 0 : 1;

    reg_early_cke_addr_decode_0 |= (0               <<  0); // reg_enable
    reg_early_cke_addr_decode_0 |= (four_chn_en     <<  1); // reg_4chn_en
    reg_early_cke_addr_decode_0 |= (two_chn_en      <<  2); // reg_chn_en
    reg_early_cke_addr_decode_0 |= (chn_pos         <<  4); // reg_chn_pos
    reg_early_cke_addr_decode_0 |= (chn_loc         <<  6); // reg_chn_loc
    reg_early_cke_addr_decode_0 |= (dual_rank_en    <<  7); // reg_dual_rank_en
    reg_early_cke_addr_decode_0 |= (0               <<  8); // reg_cas_size
    reg_early_cke_addr_decode_0 |= (remap_shift     << 13); // reg_remap_shift
    reg_early_cke_addr_decode_0 |= (rank0_size_ebg  << 16); // reg_rank_dec0
    reg_early_cke_addr_decode_0 |= (0xf             << 24); // reg_rank_cke_ext0
    reg_early_cke_addr_decode_0 |= (0xfU            << 28); // reg_rank_cke_ext1

    emi_log("[EMI] reg_early_cke_addr_decode_0: 0x%x\n", reg_early_cke_addr_decode_0);

    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00000150, reg_early_cke_addr_decode_0);    // EARLY_CKE_EMI0_CONFIG
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00000154, reg_early_cke_addr_decode_0);    // EARLY_CKE_EMI1_CONFIG
}

void update_ebg_setting(void)
{
    // init write register
    unsigned int reg_ebg_addr_decode_0 = 0x0;
    unsigned int reg_ebg_addr_decode_1 = 0x0;
    unsigned int reg_ebg_addr_decode_2 = 0x0;
    unsigned int reg_ebg_addr_decode_3 = 0x0;

    ebg_addr_decode(&reg_ebg_addr_decode_0,&reg_ebg_addr_decode_1,&reg_ebg_addr_decode_2,&reg_ebg_addr_decode_3);

    emi_log("[EMI] reg_ebg_addr_decode_0: 0x%x\n", reg_ebg_addr_decode_0);
    emi_log("[EMI] reg_ebg_addr_decode_1: 0x%x\n", reg_ebg_addr_decode_1);
    emi_log("[EMI] reg_ebg_addr_decode_2: 0x%x\n", reg_ebg_addr_decode_2);
    emi_log("[EMI] reg_ebg_addr_decode_3: 0x%x\n", reg_ebg_addr_decode_3);

    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x160, reg_ebg_addr_decode_0);
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x164, reg_ebg_addr_decode_1);
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x168, reg_ebg_addr_decode_2);
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x16c, reg_ebg_addr_decode_3);

    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x178, reg_ebg_addr_decode_0);
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x17c, reg_ebg_addr_decode_1);
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x180, reg_ebg_addr_decode_2);
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE + 0x184, reg_ebg_addr_decode_3);
}

void md_bank_mask_workaround(unsigned int reg_ebg_addr_decode_2, unsigned int reg_ebg_addr_decode_3, unsigned int *md_reg_ebg_addr_decode_2, unsigned int *md_reg_ebg_addr_decode_3)
{
    unsigned int bit8_scramble  = (reg_ebg_addr_decode_2 >>  0) & ((0x1 << 6) - 1);
    unsigned int bit9_scramble  = (reg_ebg_addr_decode_2 >>  6) & ((0x1 << 6) - 1);
    unsigned int bit10_scramble = (reg_ebg_addr_decode_2 >> 12) & ((0x1 << 6) - 1);
    unsigned int bit11_scramble = (reg_ebg_addr_decode_2 >> 18) & ((0x1 << 6) - 1);
    unsigned int bit12_scramble = (reg_ebg_addr_decode_2 >> 24) & ((0x1 << 6) - 1);
    unsigned int bit13_scramble = (reg_ebg_addr_decode_3 >>  0) & ((0x1 << 6) - 1);
    unsigned int bit14_scramble = (reg_ebg_addr_decode_3 >>  6) & ((0x1 << 6) - 1);
    unsigned int bit15_scramble = (reg_ebg_addr_decode_3 >> 12) & ((0x1 << 6) - 1);
    unsigned int bit16_scramble = (reg_ebg_addr_decode_3 >> 18) & ((0x1 << 6) - 1);
    unsigned int bit7_scramble  = (reg_ebg_addr_decode_3 >> 24) & ((0x1 << 6) - 1);
    unsigned int chn_scrm_mode  = (reg_ebg_addr_decode_3 >> 30) & ((0x1 << 1) - 1);

    unsigned int md_bit8_scramble  = (chn_scrm_mode == 1)? bit8_scramble  : (bit10_scramble >> 2);
    unsigned int md_bit9_scramble  = (chn_scrm_mode == 1)? bit9_scramble  : (bit11_scramble >> 2);
    unsigned int md_bit10_scramble = (chn_scrm_mode == 1)? bit10_scramble : (bit12_scramble >> 2);
    unsigned int md_bit11_scramble = (chn_scrm_mode == 1)? bit11_scramble : (bit13_scramble >> 2);
    unsigned int md_bit12_scramble = (chn_scrm_mode == 1)? bit12_scramble : (bit14_scramble >> 2);
    unsigned int md_bit13_scramble = (chn_scrm_mode == 1)? bit13_scramble : (bit15_scramble >> 2);
    unsigned int md_bit14_scramble = (chn_scrm_mode == 1)? bit14_scramble : (bit16_scramble >> 2);
    unsigned int md_bit15_scramble = (chn_scrm_mode == 1)? bit15_scramble : 0;
    unsigned int md_bit16_scramble = (chn_scrm_mode == 1)? bit16_scramble : 0;

    //MD_ADDR_DECODE_2
    *md_reg_ebg_addr_decode_2 |= (md_bit8_scramble  <<  0); // reg_bit8_scrm
    *md_reg_ebg_addr_decode_2 |= (md_bit9_scramble  <<  6); // reg_bit9_scrm
    *md_reg_ebg_addr_decode_2 |= (md_bit10_scramble << 12); // reg_bit10_scrm
    *md_reg_ebg_addr_decode_2 |= (md_bit11_scramble << 18); // reg_bit11_scrm
    *md_reg_ebg_addr_decode_2 |= (md_bit12_scramble << 24); // reg_bit12_scrm

    //MD_ADDR_DECODE_3
    *md_reg_ebg_addr_decode_3 |= (md_bit13_scramble  <<  0); // reg_bit13_scrm
    *md_reg_ebg_addr_decode_3 |= (md_bit14_scramble  <<  6); // reg_bit14_scrm
    *md_reg_ebg_addr_decode_3 |= (md_bit15_scramble  << 12); // reg_bit15_scrm
    *md_reg_ebg_addr_decode_3 |= (md_bit16_scramble  << 18); // reg_bit16_scrm
    *md_reg_ebg_addr_decode_3 |= (bit7_scramble      << 24); // reg_bit7_scrm
    *md_reg_ebg_addr_decode_3 |= (chn_scrm_mode      << 30); // reg_emi_scrmblr, 0: cen_emi, 1: chn_emi
    *md_reg_ebg_addr_decode_3 |= (0x1U               << 31); // reg_scrm_mode, scramble enable

    return;
}

void update_md_bank_mask(void)
{
    unsigned int md_reg_ebg_addr_decode_0 = 0x0;
    unsigned int md_reg_ebg_addr_decode_1 = 0x0;
    unsigned int md_reg_ebg_addr_decode_2 = 0x0;
    unsigned int md_reg_ebg_addr_decode_3 = 0x0;
    unsigned int reg_ebg_addr_decode_2 = 0x0;
    unsigned int reg_ebg_addr_decode_3 = 0x0;

    ebg_addr_decode(&md_reg_ebg_addr_decode_0,&md_reg_ebg_addr_decode_1,&reg_ebg_addr_decode_2,&reg_ebg_addr_decode_3);

    md_bank_mask_workaround(reg_ebg_addr_decode_2, reg_ebg_addr_decode_3, &md_reg_ebg_addr_decode_2, &md_reg_ebg_addr_decode_3);

    mt_emi_sync_write(EMI_BASE+0x00000108, md_reg_ebg_addr_decode_0);
    mt_emi_sync_write(EMI_BASE+0x00000134, md_reg_ebg_addr_decode_1);
    mt_emi_sync_write(EMI_BASE+0x00000f48, md_reg_ebg_addr_decode_2);
    mt_emi_sync_write(EMI_BASE+0x000007f4, md_reg_ebg_addr_decode_3);
}

#ifdef AID_TABLE_ENABLE

int set_aid_perm(unsigned int aid, unsigned int region, unsigned char perm)
{
	unsigned int reg_num, reg_bit;

	reg_num = region/MPU_REGION_BIT;
	reg_bit = region%MPU_REGION_BIT;

	//set north EMI
	*((volatile unsigned int *)NEMI_AID_PERM_TABEL_IDX) = aid & 0xff;

	if (!region){
		*((volatile unsigned int *)NEMI_AID_PERM_TABEL_NON_SEC) |= perm;
	} else {
		if (reg_bit) {
			*((volatile unsigned int *)NEMI_AID_PERM_TABEL_SEC(reg_num)) |= (perm << ((reg_bit-1)*2));
		} else {
			*((volatile unsigned int *)NEMI_AID_PERM_TABEL_SEC((reg_num-1))) |= (perm << ((MPU_REGION_BIT-1)*2));
		}
	}

	*((volatile unsigned int *)NEMI_AID_PERM_TABEL_TRIG_WRITE) = 1;

	//set south EMI
	*((volatile unsigned int *)SEMI_AID_PERM_TABEL_IDX) = aid & 0xff;

	if (!region){
		*((volatile unsigned int *)SEMI_AID_PERM_TABEL_NON_SEC) |= perm;
	} else {
		if (reg_bit) {
			*((volatile unsigned int *)SEMI_AID_PERM_TABEL_SEC(reg_num)) |= (perm << ((reg_bit-1)*2));
		} else {
			*((volatile unsigned int *)SEMI_AID_PERM_TABEL_SEC((reg_num-1))) |= (perm << ((MPU_REGION_BIT-1)*2));
		}
	}

	*((volatile unsigned int *)SEMI_AID_PERM_TABEL_TRIG_WRITE) = 1;

	return 0;
}

void clear_aid_table(void)
{
	unsigned char i;

	*((volatile unsigned int *)NEMI_AID_PERM_TABEL_NON_SEC) = 0;
	*((volatile unsigned int *)SEMI_AID_PERM_TABEL_NON_SEC) = 0;

	for (i = 0; i < REGION_MAX/MPU_REGION_BIT; i++) {
		*((volatile unsigned int *)NEMI_AID_PERM_TABEL_SEC(i)) = 0;
		*((volatile unsigned int *)SEMI_AID_PERM_TABEL_SEC(i)) = 0;
	}
}

void set_aid_table(void)
{
	int total = ARRAY_SIZE(AID_TABLE);
	int i, j;

	for (i = 0; i < total; i++) {
		//clear content per aid
		clear_aid_table();
		for (j = 0; j < REGION_MAX; j++) {
			if (j != 0 && AID_TABLE[i]->aid_col[j].region == 0) break;
			set_aid_perm(AID_TABLE[i]->aid, AID_TABLE[i]->aid_col[j].region, AID_TABLE[i]->aid_col[j].perm);
		}
	}

        return;
}

void set_aid_table_for_drm(void)
{
	int total = ARRAY_SIZE(AID_TABLE_DRM);
	int i, j;

	for (i = 0; i < total; i++) {
		//clear content per aid
		clear_aid_table();
		for (j = 0; j < REGION_MAX; j++) {
			if (j != 0 && AID_TABLE_DRM[i]->aid_col[j].region == 0) break;
			set_aid_perm(AID_TABLE_DRM[i]->aid, AID_TABLE_DRM[i]->aid_col[j].region, AID_TABLE_DRM[i]->aid_col[j].perm);
		}
	}

        return;
}
#endif

static void slb_table_setting(void)
{
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
    	mt_emi_sync_write(EMI_MPU_APB_BASE+0x000,0x0);    //apb protection

	//set aid 8 to 4 table for slb
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x070, (0x1 << 0)); //aid208 3:0 d1
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x070, (0x1 << 4)); //aid209 7:4 d1

	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x078, (0x2 << 0)); //aid224 3:0 d2
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x078, (0x2 << 4)); //aid225 7:4 d2
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x078, (0x2 << 8)); //aid226 11:8 d2
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x078, (0x2 << 12)); //aid227 15:12 d2

	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x058, (0x3 << 16)); //aid164 19:16 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x058, (0x3 << 20)); //aid165 23:20 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x058, (0x3 << 24)); //aid166 27:24 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x058, (0x3 << 28)); //aid167 31:28 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 0)); //aid128 3:0 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 4)); //aid129 7:4 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 8)); //aid130 11:8 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 12)); //aid131 15:12 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 16)); //aid132 19:16 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 20)); //aid133 23:20 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 24)); //aid134 27:24 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x048, (0x3 << 28)); //aid135 31:28 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x04c, (0x3 << 0)); //aid136 3:0 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x04c, (0x3 << 4)); //aid137 7:4 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 0)); //aid144 3:0 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 4)); //aid145 7:4 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 8)); //aid146 11:8 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 12)); //aid147 15:12 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 16)); //aid148 19:16 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 20)); //aid149 23:20 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 24)); //aid150 27:24 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x050, (0x3 << 28)); //aid151 31:28 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x054, (0x3 << 0)); //aid152 3:0 d3
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x054, (0x3 << 4)); //aid153 7:4 d3

	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x024, (0x4 << 12)); //aid59 15:12 d4
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x024, (0x4 << 20)); //aid61 23:20 d4

	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x00c, (0x5 << 8)); //aid10 11:8 d5
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x00c, (0x5 << 12)); //aid11 15:12 d5
	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x00c, (0x5 << 28)); //aid15 31:28 d5

	mt_emi_sync_write_or(EMI_MPU_APB_BASE + 0x080, (0x6 << 4)); //aid241 7:4 d6

    	mt_emi_sync_write(EMI_MPU_APB_BASE+0x000,0x1);    //apb protection
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
}

static char *emi_dcm_result_str;

static void emi_sw_setting(void)
{
    unsigned int emi_temp_data;
    unsigned int emi_dcm = 0;
    unsigned int emi_sys_dcm = 0;
    char *str;

	/* Enable MPU violation interrupt to MD for aid 34 & 46 */
	*((volatile unsigned int *) MD_IRQ_MONITOR) = (46 << 8) + 34;
	*((volatile unsigned int *) SUB_MD_IRQ_MONITOR) = (46 << 8) + 34;
	*((volatile unsigned int *) MD_IRQ_ENABLE) = 0x1;
	*((volatile unsigned int *) SUB_MD_IRQ_ENABLE) = 0x1;

	/* for DVFS BW monitor */
	*((volatile unsigned int *) EMI_BWCT0) = 0x07000305;
	*((volatile unsigned int *) EMI_BWCT0_6TH) = 0x0BFF0805;
	*((volatile unsigned int *) EMI_BWCT0_3RD) = 0x11FF0C05;
	*((volatile unsigned int *) EMI_BWCT0_4TH) = 0x19FF1605;

#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	last_emi_info_ptr->isu_magic = LAST_EMI_MAGIC_PATTERN;
	last_emi_info_ptr->isu_version = 0xFFFFFFFF;
#if CFG_LAST_EMI_BW_DUMP
	last_emi_info_ptr->isu_ctrl = 0xDECDDECD;
#else
	last_emi_info_ptr->isu_ctrl = 0xDEC0DEC0;
#endif
	last_emi_info_ptr->isu_dram_type = 0;
	last_emi_info_ptr->isu_diff_us = 0;
#endif

#ifdef AID_TABLE_ENABLE
	set_aid_table();
#endif
#if (CFG_HASH_RULE_MP)
	emi_addr2dram_disph(0xe);
	emi_addr2dram_hash(0xb);
#else
	emi_addr2dram_disph(2);
	emi_addr2dram_hash(1);
	//enable cpu slverr d6
	// *((volatile unsigned int *)EMI_MPU_CTRL_D(6)) |= 0x2;
	// *((volatile unsigned int *)SUB_EMI_MPU_CTRL_D(6)) |= 0x2;
#endif
	slb_table_setting();

    //set cpu slc sol A victim mode
    //Broadcastcachemaint = 0, 0x0c000234[4]=1b0
    emi_log("[EMI] read slc\n");
    emi_temp_data = mt_emi_sync_read(MCUSYS_PAR_WRAP_BASE+0x234) & ~(0x1 << 4);
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x234, emi_temp_data);

    //BCLUSTERECTLR_ELx [42]DSFP=1b0, [43]EFC=1b0, [45:44]DCC(2bit) = 2b0
    //[45:42] = 4b0
    emi_log("[EMI] read slcUBUS\n");
    emi_temp_data = mt_emi_sync_read(UBUS_BASE+0x0064) & ~(0xf << 10);
    mt_emi_sync_write(UBUS_BASE+0x0064, emi_temp_data);

    //enable cmo_bridge_en = 1b0
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0090, 0x0); //cmo_bridge_en

    //AxLUT
    emi_log("[EMI] set slc caching policy\n");
    emi_log("[EMI] Read RG_UPDATE %x\n", mt_emi_sync_read(MCUSYS_PAR_WRAP_BASE+0x00CC));
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00A0, 0x000000E4); //PIDGRP-LUT
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00B0, 0x000000E4); //Secure PIDGRP-LUT
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00B4, 0xBBBB3333); //AwSLC_LUT
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00B8, 0xDDDDDDDD); //ArSLC_LUT
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00BC, 0x0003437F); //Tagdata(MTE) AxSLC
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00C0, 0x0030000F); //PREALLOC_CONFIG
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00C4, 0xC0000000); //CNTR_CMO_CONFIG
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00C8, 0x0000007F); //RG_CFG0
    mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x00CC, 0x00000003); //RG_UPDATE

    emi_log("[EMI] Check Enable-Success Read RG_UPDATE %x\n", mt_emi_sync_read(MCUSYS_PAR_WRAP_BASE+0x00CC));

    /* EMI doeapp for DCM */
    str = dconfig_getenv("emi_dcm");
    emi_dcm = (str) ? atoi(str) : 0;

    if (emi_dcm == 1) {
		  *((volatile unsigned int *)EMI_CONM) &= ~0xFF000000;//Enable DCM
		  *((volatile unsigned int *)EMI_CONN) &= ~0xFF000000;//Enable DCM2
		  *((volatile unsigned int *)EMI_CONN) |= 0x02000000;//Enable DCM2
		  emi_dcm_result_str = "EMI DCM ENABLED\n";
    } else if (emi_dcm == 2) {
		  *((volatile unsigned int *)EMI_CONM) |= 0xFF000000;
		  *((volatile unsigned int *)EMI_CONN) |= 0xFF000000;
		  emi_dcm_result_str = "EMI DCM DISABLED\n";
    }

    /* EMISYS doeapp for DCM */
    str = dconfig_getenv("emi_sys_dcm");
    emi_sys_dcm = (str) ? atoi(str) : 0;

    if (emi_sys_dcm == 1) {
		  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000304, 0);
		  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000304, 0);
		  emi_log("EMISYS DCM ENABLED\n");
    } else if (emi_sys_dcm == 2) {
		  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000304, 0x4);
		  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000304, 0x4);
		  emi_log("EMISYS DCM DISABLED\n");
    }

    mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
    mt_emi_sync_write(EMI_BASE+0x000008cc,0x00320000);
    mt_emi_sync_write(EMI_BASE+0x00000b2c,0x36326688);
    mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast for 2+2CH
    mt_emi_sync_write(EMI_BASE+0x000008cc,0x00310000);
    mt_emi_sync_write(EMI_BASE+0x00000b2c,0x36316688);
}

void setdconfigresultString(char* dconfig){
	 char *str;
	 if (strcmp(dconfig, "emi_dcm") == 0){
	    str = emi_dcm_result_str;
	 }else{
	    str = NULL;
	 }
	 if(str != NULL)
	    emi_log("%s\n",str);
}

static void smpu_setting(void)
{
	mt_emi_sync_write(NEMI_SMPU_BASE+0x1c0, 0x1);
	mt_emi_sync_write(SEMI_SMPU_BASE+0x1c0, 0x1);
	mt_emi_sync_write(NEMI_SMPU_BASE+0xe00, 0x3c00);
	mt_emi_sync_write(SEMI_SMPU_BASE+0xe00, 0x3c00);
	mt_emi_sync_write(NEMI_IOMMU_BASE+0x824, 0x1);
	mt_emi_sync_write(SEMI_IOMMU_BASE+0x824, 0x1);
}

static void slc_sram_setting(void)
{
	/* emi_log("[EMI] 0x1C001000 0x%x\n", mt_emi_sync_read(0x1C001000)); */
	/* mt_emi_sync_write(0x1C001000, 0x0b160001); */
	/* emi_log("[EMI] 0x1C001000 0x%x\n", mt_emi_sync_read(0x1C001000)); */

	emi_log("[EMI] 0x1C001f0c 0x%x\n", mt_emi_sync_read(0x1C001f0c));
	mt_emi_sync_write(0x1C001f0c, mt_emi_sync_read(0x1C001f0c) & ~0xfff);
	do {
		emi_log("[EMI] 0x1C001f0c 0x%x\n", mt_emi_sync_read(0x1C001f0c));
	} while(mt_emi_sync_read(0x1C001f0c) & (0xfff << 16));
	emi_log("[EMI] 0x1C001f0c 0x%x\n", mt_emi_sync_read(0x1C001f0c));
}

void emi_init()
{
  unsigned int emi_temp_data;
  //Broadcast on
  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
  /*TINFO="program hash rule"*/
#if (CFG_HASH_RULE_MP)
  mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x000000EB); // [7:4] reg_disph_emi_en, [3:0]: reg_disph_chn_en, constrains: 1
  mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x800000EB); // [31]: reg_disph_lock, constrains: 1
#else
  mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x00000021); // [7:4] reg_disph_emi_en, [3:0]: reg_disph_chn_en, constrains: 1
  mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x80000021); // [31]: reg_disph_lock, constrains: 1
#endif
  /*TINFO="read central hash setting then write back to EMI internal hash reg"*/
  emi_temp_data = mt_emi_sync_read(INFRACFG_AO_BASE+0xee0);
  emi_temp_data = emi_temp_data & 0xf;
  mt_emi_sync_write_or(EMI_BASE+0x07A4, emi_temp_data);

  emi_cen_config(EMI_BASE);
  emi_chn_config(CHN_EMI_BASE);

  //Broadcast off
  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4,0x00000000);
}

void emi_init2()
{

  unsigned int emi_temp_data;

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH

  // CHN_EMI enable after DRAMC setting done
  mt_emi_sync_write(CHN0_EMI_BASE+0x00000010, 0x00842045); //[6]:afifo_blk_dis; adr[12]=adr[12]^adr[17], adr[13]=adr[13]^adr[18], adr[14]=adr[14]^adr[19]
  mt_emi_sync_write(CHN0_EMI_BASE+0x00000014, 0x00000001); //FAST_PASS_EN[28]=0x0;
  mt_emi_sync_write_or(CHN0_EMI_BASE+0x00000048, 0x00000800); //[11] 64B WRAP

  // CEN_EMI enable after CHN_EMI setting done
  mt_emi_sync_write_or(EMI_BASE+0x00000060,0x00000400); // [26] Disable read data DCM, [10] EMI enable
  mt_emi_sync_write_or(EMI_BASE+0x0000003c,0x10073330);  //[28] afifo_blk_dis, [4] 64B wrap, [3] BGM

  emi_temp_data = mt_emi_sync_read(EMI_BASE+0x00000060);
  emi_temp_data = emi_temp_data & 0xBFFFFFFF;
  mt_emi_sync_write(EMI_BASE+0x00000060,emi_temp_data); //dcm on
    
  // EMI_MPU_REG interrupt workaround for
  mt_emi_sync_write_or(EMI_MPU_APB_BASE+0x00000000,0x00000010); // [4] Disable emi_mpu_reg interrupt

  /*TINFO="read central hash setting then write back to EMI internal hash reg"*/
  emi_temp_data = mt_emi_sync_read(INFRACFG_AO_BASE+0xee0);
  emi_log("[EMI] read hash rule\n");
  emi_temp_data = emi_temp_data & 0xf;
  mt_emi_sync_write_or(EMI_BASE+0x07A4, emi_temp_data);

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast

#if 0//def EMI_BRING_UP
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000100,0x4ff00000); // [16]: emi_clk_cg_enable, bring up: 0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000100,0x4ff00000); // [16]: emi_clk_cg_enable, bring up: 0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000104,0x00030000); // W1C, [17]: emi_clk_src_sel; [16]: emi_clk_cg_enable
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000104,0x00030000); // W1C, [17]: emi_clk_src_sel; [16]: emi_clk_cg_enable
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000300,0x000000ff); // [7:0]: dcm_fr, bring up: ff (free run)
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000300,0x000000ff); // [7:0]: dcm_fr, bring up: ff (free run)
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000304,0x000000ff); // [7:0]: stall_en, bring up: ff (free run)
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000304,0x000000ff); // [7:0]: stall_en, bring up: ff (free run)

#else
  //disable dcm of hclk_ck
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000300,0x00000001); // [7:0]: dcm_fr, bring up: 1 (free run)
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000300,0x00000001); // [7:0]: dcm_fr, bring up: 1 (free run)

  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000500, 0xffffffff); // NTH_EMICFG_AO_MEM_REG_SWCG_EN
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000500, 0xffffffff); // STH_EMICFG_AO_MEM_REG_SWCG_EN
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000308, 0xffffffff); // NTH_EMICFG_AO_MEM_REG_DCM_MODE
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000308, 0xffffffff); // STH_EMICFG_AO_MEM_REG_DCM_MODE
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000100, (((0x1<<4)|(0x4<<12)|(0xf<<8)|(0x1<<6)|(0x1<<7))<<16)); // NTH_EMICFG_AO_MEM_REG_DCM_MISC_CTRL_W1S
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000100, (((0x1<<4)|(0x4<<12)|(0xf<<8)|(0x1<<6)|(0x1<<7))<<16)); // STH_EMICFG_AO_MEM_REG_DCM_MISC_CTRL_W1S


  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000204, 0xff9e00eb); // STH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000200, 0x1c0067ff); // STH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x0000020C, 0x20000003); // STH_EMICFG_AO_MEM_REG_M0M1_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000208, 0x10000000); // STH_EMICFG_AO_MEM_REG_M0M1_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000214, 0x101e0000); // STH_EMICFG_AO_MEM_REG_M2M5_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000210, 0x6000);     // STH_EMICFG_AO_MEM_REG_M2M5_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x0000021C, 0x800000);   // STH_EMICFG_AO_MEM_REG_M3_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000218, 0x1);        // STH_EMICFG_AO_MEM_REG_M3_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000224, 0x8000000);  // STH_EMICFG_AO_MEM_REG_M4_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000220, ((0x22)|(0x1<<28))); // STH_EMICFG_AO_MEM_REG_M4_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x0000022C, 0xc40000e8); // STH_EMICFG_AO_MEM_REG_M6M7_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000228, (0x618|(0x1<<28))); // STH_EMICFG_AO_MEM_REG_M6M7_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000234, 0x1000000); // STH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000230, (0x184|(0x1<<28))); // STH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000238, 0xff9e006b); // STH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_0
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x0000023c, 0x140067ff); // STH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_1
  mt_emi_sync_write(STH_EMICFG_AO_MEM_BASE+0x00000304, 0x0);        // STH_EMICFG_AO_MEM_REG_DCM_STALL_EN


  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000204, 0xff9e00eb); // NTH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000200, 0x1c006fff); // NTH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x0000020C, 0x20000003); // NTH_EMICFG_AO_MEM_REG_M0M1_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000208, 0x10000800); // NTH_EMICFG_AO_MEM_REG_M0M1_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000214, 0x101e0000); // NTH_EMICFG_AO_MEM_REG_M2M5_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000210, 0x6000);     // NTH_EMICFG_AO_MEM_REG_M2M5_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x0000021C, 0x800000);   // NTH_EMICFG_AO_MEM_REG_M3_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000218, 0x1);        // NTH_EMICFG_AO_MEM_REG_M3_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000224, 0x8000000);  // NTH_EMICFG_AO_MEM_REG_M4_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000220, ((0x22)|(0x1<<28))); // NTH_EMICFG_AO_MEM_REG_M4_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x0000022C, 0xc40000e8); // NTH_EMICFG_AO_MEM_REG_M6M7_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000228, (0x618|(0x1<<28))); // NTH_EMICFG_AO_MEM_REG_M6M7_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000234, 0x1000000); // NTH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000230, (0x184|(0x1<<28))); // NTH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000238, 0xff9e006b); // NTH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_0
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x0000023c, 0x1c006fff); // NTH_EMICFG_AO_MEM_REG_SRAM_IDLE_BIT_EN_1
  mt_emi_sync_write(NTH_EMICFG_AO_MEM_BASE+0x00000304, 0x0);        // NTH_EMICFG_AO_MEM_REG_DCM_STALL_EN

  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000600, (0x1<<4));     // NTH_EMICFG_REG_APU_ACP_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000604, (0x1<<4));     // NTH_EMICFG_REG_APU_SLB1_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000608, (0x1<<4));     // NTH_EMICFG_REG_APU_SLB0_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x0000060c, (0x1<<4));     // SLB
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000610, (0x1<<4));     // M7
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000614, (0x1<<4));     // M6
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000618, (0x1<<4));     // M4
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000634, (0x1<<4));     // 
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000638, (0x1<<4));     // 
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000624, (0x1<<4));     // NTH_EMICFG_REG_APU_EMI1_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000628, (0x1<<4));     // NTH_EMICFG_REG_APU_EMI0_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x0000062C, (0x1<<4));     // NTH_EMICFG_REG_MFG_EMI1_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000630, (0x1<<4));     // NTH_EMICFG_REG_MFG_EMI0_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x0000061C, (0x1<<4)|0x1); // NTH_EMICFG_REG_MDMCU_EMI_GALS_SLV_CTRL
  mt_emi_sync_write(NTH_EMICFG_REG_BASE+0x00000620, (0x1<<4)|0x1); // NTH_EMICFG_REG_MDHW_EMI_GALS_SLV_CTRL

  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000600, (0x1<<4));     // STH_EMICFG_REG_APU_ACP_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000604, (0x1<<4));     // STH_EMICFG_REG_APU_SLB1_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000608, (0x1<<4));     // STH_EMICFG_REG_APU_SLB0_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x0000060c, (0x1<<4));     // SLB
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000610, (0x1<<4));     // M7
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000614, (0x1<<4));     // M6
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000618, (0x1<<4));     // M4
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000634, (0x1<<4));     // 
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000638, (0x1<<4));     // 
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000624, (0x1<<4));     // STH_EMICFG_REG_APU_EMI1_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000628, (0x1<<4));     // STH_EMICFG_REG_APU_EMI0_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x0000062C, (0x1<<4));     // STH_EMICFG_REG_MFG_EMI1_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000630, (0x1<<4));     // STH_EMICFG_REG_MFG_EMI0_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x0000061C, (0x1<<4)|0x1); // STH_EMICFG_REG_MDMCU_EMI_GALS_SLV_CTRL
  mt_emi_sync_write(STH_EMICFG_REG_BASE+0x00000620, (0x1<<4)|0x1); // STH_EMICFG_REG_MDHW_EMI_GALS_SLV_CTRL
#endif

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH

  //enable EMI wdata bus encode function and set DCM DIS2= 1
  mt_emi_sync_write_or(EMI_BASE+0x00000068,(0x1<<21)|(0x1<<25));
  mt_emi_sync_write_or(CHN0_EMI_APB_BASE+0x00000050,(0x1<<4));

#if !defined(SLT)
  // Config APMCU Early CKE
  update_earlycke_setting();
#endif

#if !defined(MTK_ENABLE_EMI_EBG) || defined(SLT)
  mt_emi_sync_write(CHN0_EMI_APB_BASE+0x00000020, 0x40); // disable ebg
#else
  // Enable EBG
  mt_emi_sync_write(EMI_BASE        +0x00000028,0x00800000);  //scramble, adr[14]=adr[19]^adr[14]
  mt_emi_sync_write(EMI_BASE        +0x000007b4,0x24000000);  //scramble, adr[15]=adr[20]^adr[15], adr[16]=adr[21]^[adr[16]
  mt_emi_sync_write(SUB_EMI_APB_BASE+0x00000028,0x00800000);  //scramble, adr[14]=adr[19]^adr[14]
  mt_emi_sync_write(SUB_EMI_APB_BASE+0x000007b4,0x24000000);  //scramble, adr[15]=adr[20]^adr[15], adr[16]=adr[21]^[adr[16]

  update_ebg_setting();

  mt_emi_sync_write(CHN0_EMI_APB_BASE+0x00000020, 0x3fff0184); //ebg short enable, stop_thre=1
#endif

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast

  emi_sw_setting();
  smpu_setting();
  slc_sram_setting();
  update_md_bank_mask();
}

void emi_parity_init(void)
{
  unsigned int emi_temp_data;

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
  // Enable rdata_prty_gen & wdata_prty_chk
  mt_emi_sync_write_or(EMI_BASE+0x00000068,0x00400000); // enable cen_emi parity

  // emi bus parity workaround
  emi_temp_data = mt_emi_sync_read(0x40000000);
  mt_emi_sync_write(0x40000000, emi_temp_data);
  emi_temp_data = mt_emi_sync_read(0x40000100);
  mt_emi_sync_write(0x40000100, emi_temp_data);
  emi_temp_data = mt_emi_sync_read(0x40000200);
  mt_emi_sync_write(0x40000200, emi_temp_data);
  emi_temp_data = mt_emi_sync_read(0x40000300);
  mt_emi_sync_write(0x40000300, emi_temp_data);
  //Enabale chn_emi rdata_prty_gen & wdata_prty_chk
  mt_emi_sync_write_or(CHN_EMI_BASE+0x00000050,0x00000004); // enable chn_emi parity

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
}

void enable_mcusys_earlycke(void)
{
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x00000150, 0x1);    // EARLY_CKE_EMI0_CONFIG
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x00000154, 0x1);    // EARLY_CKE_EMI1_CONFIG
}

void enable_mcusys_ebg(void)
{
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x00000158, 0x1);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000015c, 0x1);
}

void enable_gmc(void)
{
	mt_emi_sync_write(EMI_BASE+0x00000df8,0x00dd00dd); //EMI gal setting : DCM_EN[9,8]=0x1, RCMD_AFIFO_AF_LVL:CHB[7:4]=CHA[3:0]=0xd ,WDAT_AFIFO_AF_LVL:CHB[23:20]=CHA[19:16] = 0xd          (AUSTIN said follow coda constraint 2021/01/21)
	mt_emi_sync_write(SUB_EMI_APB_BASE+0x00000df8,0x00dd00dd); //EMI gal setting : DCM_EN[9,8]=0x1, RCMD_AFIFO_AF_LVL:CHB[7:4]=CHA[3:0]=0xd ,WDAT_AFIFO_AF_LVL:CHB[23:20]=CHA[19:16] = 0xd          (AUSTIN said follow coda constraint 2021/01/21)
	mt_emi_sync_write_or(EMI_BASE+0x000000f0, 0x1 << 31);
	mt_emi_sync_write_or(SUB_EMI_APB_BASE+0x000000f0, 0x1 << 31);
}

void disable_gmc(void)
{
	unsigned int temp;

	temp = mt_emi_sync_read(EMI_BASE+0x000000f0);
	mt_emi_sync_write(EMI_BASE+0x000000f0, temp & ~(0x1 << 31));
	temp = mt_emi_sync_read(SUB_EMI_APB_BASE+0x000000f0);
	mt_emi_sync_write(SUB_EMI_APB_BASE+0x000000f0, temp & ~(0x1 << 31));
}

void emi_init3(void)
{
#if !defined(SLT)
  enable_mcusys_earlycke();

#if defined(MTK_ENABLE_EMI_EBG)
  // Enable EBG after DRAM calibration is complete
  enable_mcusys_ebg();
#endif

#endif
}

void ddr_reserve_emi_rff(void)
{
	unsigned int emi_temp_data;

	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH

	mt_emi_sync_write_or(EMI_BASE+0x00000060,0x40000000); // [30] = 1 DCM off
	emi_temp_data = mt_emi_sync_read(EMI_BASE+0x00000060);
	emi_temp_data = emi_temp_data & 0xFFFFFBFF; // [10] = 0 EMI disable
	mt_emi_sync_write(EMI_BASE+0x00000060,emi_temp_data);
	emi_log("DCM/EMI off %x: %x\n", EMI_BASE+0x00000060,emi_temp_data);

	mt_emi_sync_write_or(EMI_BASE+0x00000060,0x00000400); // [10] = 0 EMI enable

	emi_temp_data = mt_emi_sync_read(EMI_BASE+0x00000060);
	emi_temp_data = emi_temp_data & 0xBFFFFFFF; // [30] = 0 DCM on
	mt_emi_sync_write(EMI_BASE+0x00000060,emi_temp_data);
	emi_log("DCM/EMI on %x: %x\n", EMI_BASE+0x00000060,emi_temp_data);

	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
}

void ddr_reserve_emi_init(void)
{
	unsigned int emi_temp_data;
	unsigned int emi_mpu_drm = 0;
	char *str;

	//hash rule
#if (CFG_HASH_RULE_MP)
	mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x000000EB); // [7:4] reg_disph_emi_en, [3:0]: reg_disph_chn_en, constrains: 1
	mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x800000EB); // [31]: reg_disph_lock, constrains: 1
#else
	mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x00000021); // [7:4] reg_disph_emi_en, [3:0]: reg_disph_chn_en, constrains: 1
	mt_emi_sync_write(INFRACFG_AO_BASE+0x00000ee0,0x80000021); // [31]: reg_disph_lock, constrains: 1
#endif
#if (CFG_HASH_RULE_MP)
	emi_addr2dram_disph(0xe);
	emi_addr2dram_hash(0xb);
#else
	emi_addr2dram_disph(2);
	emi_addr2dram_hash(1);
#endif

	/*TINFO="read central hash setting then write back to EMI internal hash reg"*/
	emi_temp_data = mt_emi_sync_read(INFRACFG_AO_BASE+0xee0);
	emi_temp_data = emi_temp_data & 0xf;
	mt_emi_sync_write_or(EMI_BASE+0x07A4, emi_temp_data);

	//SLC
	mt_emi_sync_write(SLC_APB_BASE+0x004,0);
	mt_emi_sync_write(SUB_SLC_APB_BASE+0x004,0);

#ifdef AID_TABLE_ENABLE
	//set aid permission for abnormal reboot
	set_aid_table_for_drm();
	//set for cpu aid=241
	device_APC_dom_setup();
#endif
	//disable cpu slverr d6
	*((volatile unsigned int *)EMI_MPU_CTRL_D(6)) &= ~0x2;
	*((volatile unsigned int *)SUB_EMI_MPU_CTRL_D(6)) &= ~0x2;

	mt_emi_sync_write(EMI_BASE+0x00000300,0);
	mt_emi_sync_write(SUB_EMI_APB_BASE+0x00000300,0);

	/* EMI doeapp for MPU on DRM */
	str = dconfig_getenv("emi_mpu_drm");
	emi_mpu_drm = (str) ? atoi(str) : 0;

	if (emi_mpu_drm == 1) {
		mt_emi_sync_write(EMI_BASE+0x000000f0,0x384E0014);
		mt_emi_sync_write(SUB_EMI_APB_BASE+0x000000f0,0x384E0014);
		emi_log("EMI MPU ENABLED on drm flow\n");
	} else if (emi_mpu_drm == 2) {
		mt_emi_sync_write(EMI_BASE+0x000000f0,0x384A0014);
		mt_emi_sync_write(SUB_EMI_APB_BASE+0x000000f0,0x384A0014);
		emi_log("EMI MPU DISABLED on drm flow\n");
	}
}

void mt_emipll_pre_init()
{
	int temp;

	mt_emi_sync_write(EMIPLL_CON1, 0x830F6276);//200M
	temp = mt_emi_sync_read(EMIPLL_CON0);
	mt_emi_sync_write(EMIPLL_CON0, temp | 0x00000011);
}

void mt_emipll_post_init()
{
	int temp;
#if defined(SLT)
	return;
#endif

	// switch mux to fix clk
	mt_emi_sync_write(CLK_CFG_18_CLR, 0x03000000);
	mt_emi_sync_write(CLK_CFG_18_SET, 0x01000000); // Nth mainpll_d6_d2

	mt_emi_sync_write(CLK_CFG_19_CLR, 0x00000003);
	mt_emi_sync_write(CLK_CFG_19_SET, 0x00000001); // Sth mainpll_d6_d2

	mt_emi_sync_write(CLK_CFG_UPDATE2, 0x00006000);

	// set clk
	mt_emi_sync_write(EMIPLL_CON0, 0x0); //disable emi pll
	//mt_emi_sync_write(EMIPLL_CON1, 0x8218C4EC);//644M
	//mt_emi_sync_write(EMIPLL_CON1, 0x82102762); //420M, align with DRAM final_shu=3200
	mt_emi_sync_write(EMIPLL_CON1, 0x820E9D89); //380M, align with DRAM final_shu=3200

	//enable emi pll
	temp = mt_emi_sync_read(EMIPLL_CON0);
	mt_emi_sync_write(EMIPLL_CON0, temp | 0x00000011);

	//delay 20 us
	gpt_busy_wait_us(20);

	//switch mux to emipll
	mt_emi_sync_write(CLK_CFG_18_CLR, 0x03000000);
	mt_emi_sync_write(CLK_CFG_18_SET, 0x03000000); // Nth emipll_ck

	mt_emi_sync_write(CLK_CFG_19_CLR, 0x00000003);
	mt_emi_sync_write(CLK_CFG_19_SET, 0x00000003); // Sth emipll_ck

	mt_emi_sync_write(CLK_CFG_UPDATE2, 0x00006000);

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

	return channel_nr*CEN_EMI_NUM;
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
	unsigned int col_bit, row_bit;
	unsigned long long ch0_rank0_size, ch0_rank1_size;
	unsigned long long ch1_rank0_size, ch1_rank1_size;
	unsigned int cen_emi_conh = mt_emi_sync_read(EMI_CONH);

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

	if(ch0_rank0_size == 0) {
		emi_log("[EMI] undefined CONH for CH0 RANK0\n");
		ASSERT(0);
	}
	dram_rank_size[0] += ch0_rank0_size * ((CHANNEL_NUM == 4)? 2: 1);

	if (get_rank_nr_by_emi() > 1) {
		if(ch0_rank1_size == 0) {
			emi_log("[EMI] undefined CONH for CH0 RANK1\n");
			ASSERT(0);
		}
		dram_rank_size[1] += ch0_rank1_size * ((CHANNEL_NUM == 4)? 2: 1);
    }

	if(get_channel_nr_by_emi() > 1) {
		if(ch1_rank0_size == 0) {
			emi_log("[EMI] undefined CONH for CH1 RANK0\n");
			ASSERT(0);
		}
		dram_rank_size[0] += ch1_rank0_size * ((CHANNEL_NUM == 4)? 2: 1);

		if (get_rank_nr_by_emi() > 1) {
			if(ch1_rank1_size == 0) {
				emi_log("[EMI] undefined CONH for CH1 RANK1\n");
				ASSERT(0);
			}
			dram_rank_size[1] += ch1_rank1_size * ((CHANNEL_NUM == 4)? 2: 1);
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

	//emi_log("[EMI] ch%d, rk%d, dram addr: %x\n", dram_addr->ch, dram_addr->rk, dram_addr->addr);
	//emi_log("[EMI] bk%x, row%x, col%x\n", dram_addr->bk, dram_addr->row, dram_addr->col);
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

#if 0
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

void dis_emi_apb_prot(void)
{
	mt_emi_sync_write(EMI_MPU_CTRL, 0xFFFFFFFE);

	emi_log("[EMI] EMI_MPU_CTRL 0x%x\n", mt_emi_sync_read(EMI_MPU_CTRL));
}
#endif

int update_emi_setting(EMI_SETTINGS *default_emi_setting, EMI_INFO_T *emi_info)
{
	unsigned int ddr_type;
	unsigned int cen_cona_val, cen_conh_val, cen_conh_2nd_val, cen_conk_val;
	unsigned int chn_cona_val, chn_conc_val;
	unsigned int col, row, row_ext, rk_size, rk_size_ext, rk_size_chn_ext;
	DRAMC_CTX_T *p = psCurrDramCtx;
	int i;

	ddr_type = default_emi_setting->type & 0xFF;
	p->dram_type = ddr_type;
	cen_conh_val = 0x00000003;
	cen_conk_val = mt_emi_sync_read(EMI_CONK) & 0x0000FFFF;
	if (is_lp4_family(p)) {
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
		if (CHANNEL_NUM > 2)
			rk_size >>= 2;  //For 4 channel
		else
			rk_size >>= 1;  //For 4 channel
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

	if (is_lp4_family(p)) {
		chn_conc_val = 0x42180;
		cen_conh_2nd_val = 0;
	} else {
		chn_conc_val = 0x42100;
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
	dram_addr_t dram_addr;

	dram_addr.ch = 0;
	dram_addr.rk = 0;
	get_dramc_addr(&dram_addr, 0x0);


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
	unsigned long long rsv_start = 0;
	dram_addr_t dram_addr;

	dram_addr.ch = 0;
	dram_addr.rk = 0;
	get_dramc_addr(&dram_addr, 0x0);

	/* to avoid CMA reserve conflict, need to allocate before 0x80000000 */
#ifdef DUMMY_MBLOCK
    // dummy mblock
#else
	rsv_start = mblock_reserve_ext(&bootarg.mblock_info, 0x1000,
					0x1000, 0x80000000, 0, "emi_mbist_buf");
#endif
	emi_log("[EMI] mbist rsv_start=0x%llx\n", rsv_start);
	mt_emi_sync_write(EMI_AXI_BIST_ADR0, rsv_start - 0x40000000);
	mt_emi_sync_write(EMI_AXI_BIST_ADR1, rsv_start - 0x40000000 + 0xFFF);
}

unsigned long long platform_memory_size(void)
{
	static unsigned long long mem_size = 0;
	int nr_rank;
	int i;
	unsigned long long rank_size[DRAMC_MAX_RK];

	if (!mem_size) {
		nr_rank = get_dram_rank_nr();

		get_dram_rank_size(rank_size);

		for (i = 0; i < nr_rank; i++)
			mem_size += rank_size[i];
	}

	return mem_size;
}
#endif

