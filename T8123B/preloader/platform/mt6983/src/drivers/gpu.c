/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly
 * prohibited.
 */
/* MediaTek Inc. (C) 2021. All rights reserved.
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
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY
 * ACKNOWLEDGES THAT IT IS RECEIVER\'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY
 * THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK
 * SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO
 * RECEIVER\'S SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN
 * FORUM. RECEIVER\'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK\'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER
 * WILL BE, AT MEDIATEK\'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE
 * AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY
 * RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation
 * ("MediaTek Software") have been modified by MediaTek Inc. All revisions are
 * subject to any receiver\'s applicable license agreements with MediaTek Inc.
 */

#include "platform.h"
#include "sec_devinfo.h"
#include "gpu.h"

#define GPU_DEBUG_ENABLE          0

#define GPUOP(_freq, _volt) \
	{                       \
		.freq = _freq,      \
		.volt = _volt,      \
	}

#define VOLT_NORMALIZATION(volt) \
	((volt % 625) ? (volt - (volt % 625) + 625) : volt)

#define EFUSE_SEGMENT             30
#define EFUSE_CPU_BRISKET11       115
#define EFUSE_PTPOD21             48
#define EFUSE_PTPOD22             49
#define EFUSE_PTPOD23             50
#define EFUSE_PTPOD24             51
#define EFUSE_PTPOD25             53

#define GPU_SIGNED_OPP_0          0
#define GPU_SIGNED_OPP_1          2
#define GPU_SIGNED_OPP_2          10
#define GPU_SIGNED_OPP_3          18
#define GPU_SIGNED_OPP_4          34
#define GPU_SIGNED_OPP_5          42

#define VCORE_OPP_0_BOUND_IDX     0
#define VCORE_OPP_1_BOUND_IDX     13
#define VCORE_OPP_2_BOUND_IDX     20
#define VCORE_OPP_3_BOUND_IDX     30
#define VCORE_OPP_4_BOUND_IDX     34

#define ENG_SEGMENT               0x0
#define MT6983_SEGMENT            0x1
#define MT6985_SEGMENT            0x2
#define MT6983W_SEGMENT           0x3
#define MT6983T_SEGMENT           0x4

#define GPU_SIGNED_OPP_0_VOLT     75000
#define GPU_SIGNED_OPP_1_VOLT     75000
#define GPU_SIGNED_OPP_2_VOLT     70000
#define GPU_SIGNED_OPP_3_VOLT     65000
#define GPU_SIGNED_OPP_4_VOLT     55000
#define GPU_SIGNED_OPP_5_VOLT     50000

#define GPU_SIGNED_OPP_0_OFFSET   0
#define GPU_SIGNED_OPP_1_OFFSET   2500
#define GPU_SIGNED_OPP_2_OFFSET   3125
#define GPU_SIGNED_OPP_3_OFFSET   10625
#define GPU_SIGNED_OPP_4_OFFSET   3125
#define GPU_SIGNED_OPP_5_OFFSET   0

#define NUM_OPP_IDX               ARRAY_SIZE(gpu_opp_table)
#define NUM_SIGNED_IDX            ARRAY_SIZE(gpu_signed_idx)
#define NUM_CRITICAL_IDX          ARRAY_SIZE(gpu_critical_idx)

static unsigned int g_init_done;
static const int gpu_signed_idx[] = {
	GPU_SIGNED_OPP_0,
	GPU_SIGNED_OPP_1,
	GPU_SIGNED_OPP_2,
	GPU_SIGNED_OPP_3,
	GPU_SIGNED_OPP_4,
	GPU_SIGNED_OPP_5,
};
static const int gpu_signed_volt[] = {
	GPU_SIGNED_OPP_0_VOLT,
	GPU_SIGNED_OPP_1_VOLT,
	GPU_SIGNED_OPP_2_VOLT,
	GPU_SIGNED_OPP_3_VOLT,
	GPU_SIGNED_OPP_4_VOLT,
	GPU_SIGNED_OPP_5_VOLT,
};
static const unsigned int gpu_volt_offset[NUM_SIGNED_IDX] = {
	GPU_SIGNED_OPP_0_OFFSET,
	GPU_SIGNED_OPP_1_OFFSET,
	GPU_SIGNED_OPP_2_OFFSET,
	GPU_SIGNED_OPP_3_OFFSET,
	GPU_SIGNED_OPP_4_OFFSET,
	GPU_SIGNED_OPP_5_OFFSET,
};
static struct gpu_opp_info gpu_opp_table[] = {
	GPUOP(955000, GPU_SIGNED_OPP_0_VOLT), /*  0 sign off */
	GPUOP(880000, 75000), /*  1 */
	GPUOP(848000, GPU_SIGNED_OPP_1_VOLT), /*  2 sign off */
	GPUOP(841000, 74375), /*  3 */
	GPUOP(835000, 73750), /*  4 */
	GPUOP(828000, 73125), /*  5 */
	GPUOP(822000, 72500), /*  6 */
	GPUOP(816000, 71875), /*  7 */
	GPUOP(809000, 71250), /*  8 */
	GPUOP(803000, 70625), /*  9 */
	GPUOP(797000, GPU_SIGNED_OPP_2_VOLT), /* 10 sign off */
	GPUOP(770000, 69375), /* 11 */
	GPUOP(744000, 68750), /* 12 */
	GPUOP(717000, 68125), /* 13 */
	GPUOP(691000, 67500), /* 14 */
	GPUOP(665000, 66875), /* 15 */
	GPUOP(638000, 66250), /* 16 */
	GPUOP(612000, 65625), /* 17 */
	GPUOP(586000, GPU_SIGNED_OPP_3_VOLT), /* 18 sign off */
	GPUOP(569000, 64375), /* 19 */
	GPUOP(552000, 63750), /* 20 */
	GPUOP(535000, 63125), /* 21 */
	GPUOP(518000, 62500), /* 22 */
	GPUOP(501000, 61875), /* 23 */
	GPUOP(484000, 61250), /* 24 */
	GPUOP(467000, 60625), /* 25 */
	GPUOP(451000, 60000), /* 26 */
	GPUOP(434000, 59375), /* 27 */
	GPUOP(417000, 58750), /* 28 */
	GPUOP(400000, 58125), /* 29 */
	GPUOP(383000, 57500), /* 30 */
	GPUOP(366000, 56875), /* 31 */
	GPUOP(349000, 56250), /* 32 */
	GPUOP(332000, 55625), /* 33 */
	GPUOP(316000, GPU_SIGNED_OPP_4_VOLT), /* 34 sign off */
	GPUOP(303000, 54375), /* 35 */
	GPUOP(290000, 53750), /* 36 */
	GPUOP(277000, 53125), /* 37 */
	GPUOP(265000, 52500), /* 38 */
	GPUOP(252000, 51875), /* 39 */
	GPUOP(239000, 51250), /* 40 */
	GPUOP(226000, 50625), /* 41 */
	GPUOP(214000, GPU_SIGNED_OPP_5_VOLT), /* 42 sign off */
};
static const unsigned int gpu_aging_table[NUM_OPP_IDX] = {
	625, 625, 625, 625, 625, 625, 625, 625, 625, 625,  /* OPP 0-9   */
	625, 625, 625, 625, 625, 625, 625, 625, 625, 625,  /* OPP 10-19 */
	625, 625, 625, 625, 625, 625, 625, 625, 625, 625,  /* OPP 20-29 */
	625, 625, 625, 625, 625, 625, 625, 625, 625, 625,  /* OPP 30-39 */
	625, 625, 625,                                     /* OPP 40-42 */
};
static int gpu_critical_idx[] = {
	VCORE_OPP_0_BOUND_IDX,
	VCORE_OPP_1_BOUND_IDX,
	VCORE_OPP_2_BOUND_IDX,
	VCORE_OPP_3_BOUND_IDX,
	VCORE_OPP_4_BOUND_IDX,
};
static unsigned int gpu_critical_volt[NUM_CRITICAL_IDX] = {
	0,
	0,
	0,
	0,
	0,
};
static const unsigned int gpu_avs_efuse_idx[NUM_SIGNED_IDX] = {
	EFUSE_CPU_BRISKET11,
	EFUSE_PTPOD21,
	EFUSE_PTPOD22,
	EFUSE_PTPOD23,
	EFUSE_PTPOD24,
	EFUSE_PTPOD25,
};

static void gpu_avs_adjustment(void)
{
	u32 val = 0;
	unsigned int temp_volt = 0, temp_freq = 0;
	unsigned int sb_version = 0, ptp_version = 0;
	int i = 0, oppidx = 0, oppidx_next = 0;

	/*
	 * Freq (MHz) | Signedoff Volt (V) | Efuse name    | Efuse address
	 * ===============================================================
	 * 955        | 0.75               | CPU_BRISKET11 | 0x11EE_0B10
	 * 848        | 0.75               | PTPOD21       | 0x11EE_05D4
	 * 797        | 0.7                | PTPOD22       | 0x11EE_05D8
	 * 586        | 0.65               | PTPOD23       | 0x11EE_05DC
	 * 316        | 0.55               | PTPOD24       | 0x11EE_05E0
	 * 214        | 0.5                | PTPOD25       | 0x11EE_05E4
	 *
	 * the binning result of lowest voltage will be 0.45V from 214MHz/0.5V.
	 */

	/* read AVS efuse and compute Freq and Volt */
	for (i = 0; i < NUM_SIGNED_IDX; i++) {
		oppidx = gpu_signed_idx[i];
		val = seclib_get_devinfo_with_index(gpu_avs_efuse_idx[i]);

#ifdef MTK_MCL50_FLAVOR_LOAD
		if (i == 0)
			val = 0x0048BCF6;
		else if (i == 1)
			val = 0x00C30C65;
		else if (i == 2)
			val = 0x008E1C64;
		else if (i == 3)
			val = 0x00886825;
		else if (i == 4)
			val = 0x008C84D4;
		else if (i == 5)
			val = 0x0047A047;
#endif /* MTK_MCL50_FLAVOR_LOAD */

		/* if efuse value is not set */
		if (!val)
			continue;

		/* compute Freq from efuse */
		temp_freq = 0;
		temp_freq |= (val & 0x00100000) >> 10; /* Get freq[10]  from efuse[20]    */
		temp_freq |= (val & 0x00000C00) >> 2;  /* Get freq[9:8] from efuse[11:10] */
		temp_freq |= (val & 0x00000003) << 6;  /* Get freq[7:6] from efuse[1:0]   */
		temp_freq |= (val & 0x000000C0) >> 2;  /* Get freq[5:4] from efuse[7:6]   */
		temp_freq |= (val & 0x000C0000) >> 16; /* Get freq[3:2] from efuse[19:18] */
		temp_freq |= (val & 0x00003000) >> 12; /* Get freq[1:0] from efuse[13:12] */
		/* Freq is stored in efuse with MHz unit */
		temp_freq *= 1000;
		/* verify with signoff Freq */
		if (temp_freq != gpu_opp_table[oppidx].freq) {
			print("[E]%s: OPP[%d]: efuse[%d].freq(%d) != signed-off.freq(%d)\n",
				__func__, oppidx, i, temp_freq, gpu_opp_table[oppidx].freq);
			return;
		}

		/* compute Volt from efuse */
		temp_volt = 0;
		temp_volt |= (val & 0x0003C000) >> 14; /* Get volt[3:0] from efuse[17:14] */
		temp_volt |= (val & 0x00000030);       /* Get volt[5:4] from efuse[5:4]   */
		temp_volt |= (val & 0x0000000C) << 4;  /* Get volt[7:6] from efuse[3:2]   */
		/* Volt is stored in efuse with 6.25mV unit */
		temp_volt *= 625;
		gpu_opp_table[oppidx].volt = temp_volt;

		/* CPU_BRISKET11 [31:24] GPU_SB_version */
		if (i == 0)
			sb_version = (val & 0xFF000000) >> 24;
		/* PTPOD21 [31:24] GPU_PTP_version */
		else if (i == 1)
			ptp_version = (val & 0xFF000000) >> 24;
	}

	/* check AVS Volt */
	for (i = NUM_SIGNED_IDX - 1; i >= 0; i--) {
		oppidx = gpu_signed_idx[i];

		/*
		 * AVS Volt reverse check, start from NUM_SIGNED_IDX -2
		 * Volt of sign-off[i] should always be larger than sign-off[i+1]
		 * if not, add Volt offset to sign-off[i]
		 */
		if (i != (NUM_SIGNED_IDX - 1)) {
			oppidx_next = gpu_signed_idx[i+1];

			if (gpu_opp_table[oppidx].volt <= gpu_opp_table[oppidx_next].volt) {
				print("[W]%s: efuse[%d].volt(%d) <= efuse[%d].volt(%d)\n",
					__func__, i, gpu_opp_table[oppidx].volt,
					i+1, gpu_opp_table[oppidx_next].volt);
				gpu_opp_table[oppidx].volt =
					gpu_opp_table[oppidx_next].volt + gpu_volt_offset[i];
			}
		}

		/* clamp to signoff Volt */
		if (gpu_opp_table[oppidx].volt > gpu_signed_volt[i]) {
			print("[W]%s: OPP[%d]: efuse[%d].volt(%d) > signed-off.volt(%d)\n",
				__func__, oppidx, i, gpu_opp_table[oppidx].volt,
				gpu_signed_volt[i]);
			gpu_opp_table[oppidx].volt = gpu_signed_volt[i];
		}
	}

	for (i = 0; i < NUM_SIGNED_IDX; i++) {
		oppidx = gpu_signed_idx[i];
		print("[I]%s: OPP[%d]: efuse[%d] freq(%d), volt(%d)\n",
			__func__, oppidx, i, gpu_opp_table[oppidx].freq,
			gpu_opp_table[oppidx].volt);
	}

	print("[I]%s: sb_version: 0x%x, ptp_version: 0x%x\n", sb_version, ptp_version);
}

static void gpu_update_critical_idx(void)
{
	unsigned int segment_id = 0x0;

	segment_id = seclib_get_devinfo_with_index(EFUSE_SEGMENT) & 0xFF;
	switch (segment_id) {
	case MT6983T_SEGMENT:
		gpu_critical_idx[0] = 0;
		break;
	case MT6983W_SEGMENT:
		gpu_critical_idx[0] = 0;
		break;
	case MT6985_SEGMENT:
		gpu_critical_idx[0] = 1;
		break;
	case MT6983_SEGMENT:
		gpu_critical_idx[0] = 2;
		break;
	default: /* ENG_SEGMENT */
		gpu_critical_idx[0] = 2;
		break;
	}

#if defined(MTK_INFOB_FLAVOR_LOAD)
	gpu_critical_idx[0] = 0;
#endif /* MTK_INFOB_FLAVOR_LOAD */

#if GPU_DEBUG_ENABLE
	print("[I]%s: segment id: %d\n", segment_id);
#endif /* GPU_DEBUG_ENABLE */
}

static void gpu_init_opp(void)
{
	int front_idx = 0, rear_idx = 0, inner_idx = 0;
	unsigned int large_volt = 0, small_volt = 0;
	unsigned int large_freq = 0, small_freq = 0;
	unsigned int inner_volt = 0, inner_freq = 0;
	unsigned int previous_volt = 0;
	int range = 0;
	int slope = 0;
	int i = 0, j = 0;

	gpu_avs_adjustment();

	for (i = 1; i < NUM_SIGNED_IDX; i++) {
		front_idx = gpu_signed_idx[i - 1];
		rear_idx = gpu_signed_idx[i];
		range = rear_idx - front_idx;

		/* increase precision slope */
		large_volt = gpu_opp_table[front_idx].volt * 100;
		large_freq = gpu_opp_table[front_idx].freq / 1000;

		small_volt = gpu_opp_table[rear_idx].volt * 100;
		small_freq = gpu_opp_table[rear_idx].freq / 1000;

		/* slope = volt / freq */
		slope = (int)(large_volt - small_volt) / (int)(large_freq - small_freq);

		if (slope < 0) {
			print("[E]%s: invalid slope: %d\n", __func__, slope);
			continue;
		}

#if GPU_DEBUG_ENABLE
		print("[D]%s: OPP[%d] Freq: %d, Volt: %d, slope: %d\n",
			__func__, rear_idx, small_freq*1000, small_volt/100, slope);
#endif /* GPU_DEBUG_ENABLE */

		/* start from small v and f, and use (+) instead of (-) */
		for (j = 1; j < range; j++) {
			inner_idx = rear_idx - j;
			inner_freq = gpu_opp_table[inner_idx].freq / 1000;
			inner_volt = (small_volt + slope * (inner_freq - small_freq)) / 100;
			inner_volt = VOLT_NORMALIZATION(inner_volt);

			/* compare interpolated volt with volt of previous OPP idx */
			previous_volt = gpu_opp_table[inner_idx + 1].volt;
			if (inner_volt < previous_volt)
				print("[E]%s: invalid interpolated [%d] Volt: %d < [%d] Volt: %d\n",
					__func__, inner_idx, inner_volt, inner_idx + 1, previous_volt);

			gpu_opp_table[inner_idx].volt = inner_volt;

#if GPU_DEBUG_ENABLE
			print("[D]%s: OPP[%d] Freq: %d, Volt: %d\n",
				__func__, inner_idx, inner_freq*1000, inner_volt);
#endif /* GPU_DEBUG_ENABLE */
		}
#if GPU_DEBUG_ENABLE
		print("[D]%s: OPP[%d] Freq: %d, Volt: %d\n",
			__func__, front_idx, large_freq*1000, large_volt/100);
#endif /* GPU_DEBUG_ENABLE */
	}

#if defined(MTK_AGING_FLAVOR_LOAD)
	for (i = 0; i < NUM_OPP_IDX; i++)
		gpu_opp_table[i].volt -= gpu_aging_table[i];
#endif /* MTK_AGING_FLAVOR_LOAD */

	/* adjust first critical index by segment */
	gpu_update_critical_idx();

	for (i = 0; i < NUM_CRITICAL_IDX; i++) {
		gpu_critical_volt[i] = gpu_opp_table[gpu_critical_idx[i]].volt;
		print("[I]%s: OPP[%d] critical Volt: %d\n",
			__func__, gpu_critical_idx[i], gpu_critical_volt[i]);
	}
}

unsigned int gpu_get_critical_volt(int idx)
{
	if (!g_init_done) {
		gpu_init_opp();
		g_init_done = true;
	}

	if (idx < NUM_CRITICAL_IDX)
		return gpu_critical_volt[idx] * 10; /* uV */
	else
		return 0;
}
