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
#include <typedefs.h>
#include <timer.h>

/*
 * This driver is compatible with MTK systimer IP, but
 * in order to ease the porting effort, some function
 * name containing "gptx" means apxgpt is kept
 */
#define SYSTIMER_CNTCV_L            ((P_U32)(SYSTIMER_BASE+0x0008))
#define SYSTIMER_CNTWACR            ((P_U32)(SYSTIMER_BASE+0x0010))
#define SYSTIMER_CNTRACR            ((P_U32)(SYSTIMER_BASE+0x0014))

/*
 * 13MHz setting
 */
#define SYSTIMER_MAX_US_TIMEOUT ((U32)330382100)    // 0xFFFFFFFF /13d
#define SYSTIMER_MAX_MS_TIMEOUT ((U32)330382)       // 0xFFFFFFFF /13000d
#define SYSTIMER_1US_TICK       ((U32)13)           // 1000 / 76.92ns = 13.000
#define SYSTIMER_1MS_TICK       ((U32)13000)        // 1000000 / 76.92ns = 13000.520
// 13MHz: 1us = 13.000 ticks
#define TIME_TO_TICK_US(us)     ((us)*SYSTIMER_1US_TICK + ((us)*0 + (1000-1))/1000)
// 13MHz: 1ms = 13000.520 ticks
#define TIME_TO_TICK_MS(ms)     ((ms)*SYSTIMER_1MS_TICK + ((ms)*520 + (1000-1))/1000)

#define SYSTIMER_MAX_TICK_CNT   ((U32)0xFFFFFFFF)
#define MAX_TIMESTAMP_MS        ((U32)0xFFFFFFFF)

static volatile U32 timestamp;
static volatile U32 lastinc;

/*
 * SYSTIMER fixed 13MHz counter
 */
static U32 systimer_tick2time_ms(U32 tick)
{
    return ((tick + (SYSTIMER_1MS_TICK - 1)) / SYSTIMER_1MS_TICK);
}

void mtk_timer_init(void)
{
	lastinc = systimer_tick2time_ms(*SYSTIMER_CNTCV_L);
	timestamp = 0;

	*SYSTIMER_CNTWACR = 0x3FFFFF0;
	*SYSTIMER_CNTRACR = 0x3FFFFFF;
}

void mtk_timer_deinit (void)
{
	return;
}

U32 gpt4_get_current_tick(void)
{
	U32 cnt1, cnt2, cnt3, value1;

	cnt1 = (*SYSTIMER_CNTCV_L);
	cnt2 = (*SYSTIMER_CNTCV_L);
	cnt3 = (*SYSTIMER_CNTCV_L);
	if (cnt2 < cnt1) {
		if (cnt1 < cnt3)
			value1 = cnt1;
		else {
			value1 = ((cnt2 > cnt3) ? cnt2 :cnt3);
		}
	} else {
		if (cnt2 < cnt3)
			value1 = cnt2;
		else {
			value1= ((cnt1 > cnt3) ? cnt1 :cnt3);
		}
	}
	return value1;
}

//===========================================================================
// us interface
//===========================================================================
U32 gpt4_tick2time_us(U32 tick)
{
	return ((tick + (SYSTIMER_1US_TICK - 1)) / SYSTIMER_1US_TICK);
}

U32 gpt4_time2tick_us(U32 time_us)
{
	if (SYSTIMER_MAX_US_TIMEOUT <= time_us) {
		return SYSTIMER_MAX_TICK_CNT;
	} else {
		return TIME_TO_TICK_US(time_us);
	}
}

//===========================================================================
// ms interface
//===========================================================================
U32 gpt4_tick2time_ms(U32 tick)
{
	return ((tick + (SYSTIMER_1MS_TICK - 1)) / SYSTIMER_1MS_TICK);
}

ulong get_timer_masked (void)
{
	volatile U32 now;

	now = systimer_tick2time_ms(*SYSTIMER_CNTCV_L);

	if (now >= lastinc) {
		timestamp = timestamp + now - lastinc;        /* normal */
	} else {
		timestamp = timestamp + SYSTIMER_MAX_MS_TIMEOUT - lastinc + now;   /* overflow */
	}
	lastinc = now;

	return timestamp;
}

ulong get_timer(ulong base)
{
	ulong current_timestamp = 0;
	ulong temp = 0;

	current_timestamp = get_timer_masked();

	if (current_timestamp >= base) {
		/* timestamp normal */
		return (current_timestamp - base);
	}
	/* timestamp overflow */
	//dbg_print("return = 0x%x\n",MAX_TIMESTAMP_MS - ( base - current_timestamp ));
	temp = base - current_timestamp;

	return (MAX_TIMESTAMP_MS - temp);
}

//===========================================================================
// bust wait
//===========================================================================
bool gpt4_timeout_tick(U32 start_tick, U32 timeout_tick)
{
	register U32 cur_tick;
	register U32 elapse_tick;

	// get current tick
	cur_tick = gpt4_get_current_tick();

	// check elapse time
	if (start_tick <= cur_tick) {
		elapse_tick = cur_tick - start_tick;
	} else {
		elapse_tick = (SYSTIMER_MAX_TICK_CNT - start_tick) + cur_tick;
	}

	// check if timeout
	if (timeout_tick <= elapse_tick) {
		// timeout
		return TRUE;
	}

	return FALSE;
}

void gpt_busy_wait_us(U32 timeout_us)
{
	U32 start_tick, timeout_tick;

	// get timeout tick
	timeout_tick = gpt4_time2tick_us(timeout_us);
	start_tick = gpt4_get_current_tick();

	// wait for timeout
	while (!gpt4_timeout_tick(start_tick, timeout_tick));
}

U32 gpt4_time2tick_ms(U32 time_ms)
{
	if (SYSTIMER_MAX_MS_TIMEOUT <= time_ms) {
		return SYSTIMER_MAX_TICK_CNT;
	}
	return TIME_TO_TICK_MS(time_ms);
}

void gpt_busy_wait_ms(U32 timeout_ms)
{
	U32 start_tick, timeout_tick;

	// get timeout tick
	timeout_tick = gpt4_time2tick_ms(timeout_ms);
	start_tick = gpt4_get_current_tick();

	// wait for timeout
	while (!gpt4_timeout_tick(start_tick, timeout_tick));
}

void mdelay(unsigned long msec)
{
	gpt_busy_wait_ms(msec);
}

void udelay(unsigned long usec)
{
	gpt_busy_wait_us(usec);
}
