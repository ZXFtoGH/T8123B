/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#define MTK_LOG_ENABLE 1
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include <sys/time.h>
#include <unistd.h>

#include "ftm_audio_Common.h"
#include <CFG_AUDIO_File.h>

#include <AudioFtmBase.h>
#include "common.h"
#include "miniui.h"
#include "ftm.h"

#ifdef __cplusplus
extern "C" {
#include "DIF_FFT.h"
#include "Audio_FFT_Types.h"
#include "Audio_FFT.h"
#endif

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "FTM_AUDIO"
#ifdef FEATURE_FTM_AUDIO

#include <AudioMTKHeadsetMessager.h>

//#define MTK_MIC3_SUPPORT // TODO(Harvey): enable in ProjectConfig.mk
//#define MTK_MIC4_SUPPORT // TODO(Harvey): enable in ProjectConfig.mk

#define mod_to_mAudio(p)     (struct mAudio*)((char*)(p) + sizeof(struct ftm_module))
#define TAG   "[Audio] "
#define HEADSET_STATE_PATH "/sys/bus/platform/drivers/Accdet_Driver/state"
#define ACCDET_STATE_PATH "/sys/bus/platform/drivers/Accdet_Driver/state"

#define GET_HEADSET_STATE

#define MAX_FILE_NAME_SIZE (100)
#define TEXT_LENGTH (1024)

#define WAVE_PLAY_MAX_TIME  (5000)   //in ms.
#define WAVE_PLAY_SLEEP_TIME (100)

#define FFT_SIZE 4096
#define FFT_DAT_MAXNUM 200

//#define WAVE_PLAYBACK //use Audio_Wave_Playabck_thread for Ringtone/Receiver test

#define BUF_LEN 1
static char rbuf[BUF_LEN] = {'\0'};
static char wbuf[BUF_LEN] = {'1'};
static char wbuf1[BUF_LEN] = {'2'};
static const char ouput_dev_name[3][16] = {"Receiver", "Headset", "Speaker"};

// Global variable
static int HeadsetFd = 0;
static int g_loopback_item  = 0;
static int g_mic_change     = 0;
static int g_prev_mic_state = 0;

static int b_mic1_loopback = false;
static int b_mic2_loopback = false;
/* static int b_mic3_loopback = false; */
/* static int b_mic4_loopback = false; */
static int print_len1 = 0;
static int print_len2 = 0;
static int b_incomplete_flag = false;
#if defined(FEATURE_FTM_SPEAKER_CALIBRATION)
static bool streamInOpenDone;
#endif
extern sp_ata_data return_data;
extern android::AudioFtmBaseVirtual *gAudioFtmBase;
extern bool wavePlaybackExit;
bool bMicbias_exit = false;

enum
{
    ITEM_MIC1,
    ITEM_MIC2,
    ITEM_MIC3,
    ITEM_MIC4,
    ITEM_HEADSET_MIC,
#ifdef FEATURE_FTM_ACSLB
    ITEM_DUALMIC_DMNR,
    ITEM_DUALMIC_NO_DMNR,
#endif
    ITEM_RINGTONE,
    ITEM_MICBIAS_ON,
    ITEM_MICBIAS_OFF,
    ITEM_PASS,
    ITEM_FAIL,
    ITEM_ADD_TEMPERATURE,
    ITEM_SUB_TEMPERATURE,
    ITEM_SPK_CALIBRATION_RESULT_SAVE,
    ITEM_SPK_CALIBRATION_EXIT,
    ITEM_EXIT_AND_SAVE,
    ITEM_EXIT
};

typedef enum {
    L_CH_ONLY,
    R_CH_ONLY,
    ALL_CH
} pass_channl_criteria_t;

using android::SELECT_MIC_DEFAULT;
using android::SELECT_MIC1;
using android::SELECT_MIC2;
using android::SELECT_MIC3;
using android::SELECT_MIC4;
using android::SELECT_MIC5;
using android::SELECT_MIC_TYPE_MAX;

static item_t audio_items_loopback[] =
{
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};

static item_t audio_items_loopback_dual_mic[] =
{
    {ITEM_MIC1, uistr_info_audio_loopback_dualmic_mi1, 0, 0},
    {ITEM_MIC2, uistr_info_audio_loopback_dualmic_mi2, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};

static item_t audio_items_loopback_ex[] = {
    {ITEM_HEADSET_MIC, uistr_info_audio_loopback_headset_mic, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    {-1, NULL, 0, 0},
};

static item_t audio_items_loopback_ex_dual_mic[] = {
    {ITEM_MIC1, uistr_info_audio_loopback_dualmic_mi1, 0, 0},
    {ITEM_MIC2, uistr_info_audio_loopback_dualmic_mi2, 0, 0},
    {ITEM_HEADSET_MIC, uistr_info_audio_loopback_headset_mic, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    {-1, NULL, 0, 0},
};

#ifdef FEATURE_FTM_ACSLB
static item_t audio_items_acoustic_loopback[] =
{
    {ITEM_DUALMIC_NO_DMNR, uistr_info_audio_acoustic_loopback_without_DMNR, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS,  uistr_pass, 0, 0},
    {ITEM_FAIL,  uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};

static item_t audio_items_acoustic_loopback_dual_mic[] =
{
    {ITEM_DUALMIC_DMNR, uistr_info_audio_acoustic_loopback_DMNR, 0, 0},
    {ITEM_DUALMIC_NO_DMNR, uistr_info_audio_acoustic_loopback_without_DMNR, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS,  uistr_pass, 0, 0},
    {ITEM_FAIL,  uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};
#endif

static item_t audio_items_micbias[] =
{
    {ITEM_MICBIAS_ON, uistr_info_audio_micbias_on, 0, 0},
    {ITEM_MICBIAS_OFF, uistr_info_audio_micbias_off, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS,      uistr_pass, 0, 0},
    {ITEM_FAIL,      uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};

static item_t audio_items[] =
{
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};

static item_t audio_items_auto[] =
{
    { -1, NULL, 0, 0},
};

static item_t receiver_items[] =
{
    {ITEM_RINGTONE, uistr_info_audio_ringtone, 0, 0},
    {ITEM_MIC1, uistr_info_audio_loopback_dualmic_mic, 0, 0},
#ifdef MTK_MIC3_SUPPORT
    {ITEM_MIC3, uistr_info_audio_loopback_dualmic_mi3, 0, 0},
#endif
#ifdef MTK_MIC4_SUPPORT
    {ITEM_MIC4, uistr_info_audio_loopback_dualmic_mi4, 0, 0},
#endif
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};

static item_t receiver_items_dual_mic[] =
{
    {ITEM_RINGTONE, uistr_info_audio_ringtone, 0, 0},
    {ITEM_MIC1, uistr_info_audio_loopback_dualmic_mi1, 0, 0},
    {ITEM_MIC2, uistr_info_audio_loopback_dualmic_mi2, 0, 0},
#ifdef MTK_MIC3_SUPPORT
    {ITEM_MIC3, uistr_info_audio_loopback_dualmic_mi3, 0, 0},
#endif
#ifdef MTK_MIC4_SUPPORT
    {ITEM_MIC4, uistr_info_audio_loopback_dualmic_mi4, 0, 0},
#endif
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    { -1, NULL, 0, 0},
};

static item_t receiver_items_ex[] = {
    {ITEM_RINGTONE,uistr_info_audio_ringtone, 0, 0},
    {ITEM_MIC1, uistr_info_audio_loopback_dualmic_mic, 0, 0},
    {ITEM_HEADSET_MIC, uistr_info_audio_loopback_headset_mic, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
    {-1, NULL, 0, 0},
};

static item_t receiver_items_ex_dual_mic[] = {
    {ITEM_RINGTONE,uistr_info_audio_ringtone, 0, 0},
    {ITEM_MIC1, uistr_info_audio_loopback_dualmic_mi1, 0, 0},
    {ITEM_MIC2, uistr_info_audio_loopback_dualmic_mi2, 0, 0},
#ifdef MTK_MIC3_SUPPORT
    {ITEM_MIC3, uistr_info_audio_loopback_dualmic_mi3, 0, 0},
#endif
#ifdef MTK_MIC4_SUPPORT
    {ITEM_MIC4, uistr_info_audio_loopback_dualmic_mi4, 0, 0},
#endif
    {ITEM_HEADSET_MIC, uistr_info_audio_loopback_headset_mic, 0, 0},
#ifndef FEATURE_FTM_TOUCH_MODE
    {ITEM_PASS, uistr_pass, 0, 0},
    {ITEM_FAIL, uistr_fail, 0, 0},
#endif
	{-1, NULL, 0, 0},
};

// use for wave playback
static char FileListNamesdcard[] = "/sdcard/factory.ini";
static char FileListName[]           = "/vendor/etc/factory.ini";
static char FileStartString_WavePlayback[]      = "//AudioRingtonePlayFile";
static char FileStartString_LoudspkPlayback[]      = "//AudioRingtonePlayFile";
static char FileStartString_LoudspkPlayback_L[]   = "//AudioRingtonePlayFileL";
static char FileStartString_LoudspkPlayback_R[]   = "//AudioRingtonePlayFileR";
#if defined(WAVE_PLAYBACK)
static char FileStartString_ReceiverPlayback[]      = "//AudioReceiverPlayFile";
#endif
static char FileStartString_CalibrationPlayback[] = "//SpeakerCalibrationPlayFile";

// use for freq. response test
static const char * FileStartString_Receiver_FreqResponse[5] = {"//AudioReceiverFreq2K", "//AudioReceiverFreq25K", "//AudioReceiverFreq3K", "//AudioReceiverFreq35K", "//AudioReceiverFreq4K"};
static const char * FileStartString_Speaker_FreqResponse[5] = {"//AudioSpeakerFreq05K", "//AudioSpeakerFreq075K", "//AudioSpeakerFreq1K", "//AudioSpeakerFreq15K", "//AudioSpeakerFreq2K"};
static int Receiver_Frequency[5] = {2000, 2500, 3000, 3500, 4000};
static int Speaker_Frequency[5] = {500, 750, 1000, 1500, 2000};
unsigned int FreqLoop = 0, FreqTestResult[5] = {0};


#define CH_L   1
#define CH_R   2
#define CH_MAX 3

struct mAudio
{
    struct ftm_module *mod;
    struct textview tv;
    struct itemview *iv;
    pthread_t hHeadsetThread;
    pthread_t hRecordThread;
    pthread_mutex_t mHeadsetMutex;
    int avail;
    int Headset_change;
    int Headset_mic;
    int phoneMicType;
    bool exit_thd;
    char  info[TEXT_LENGTH];
    char  file_name[MAX_FILE_NAME_SIZE];
	char  file_name_L[MAX_FILE_NAME_SIZE];
	char  file_name_R[MAX_FILE_NAME_SIZE];
	pass_channl_criteria_t chToCheck;
	pthread_mutex_t mCurCheckCHMutex;
	bool  bswitchCH;
	int   iCurCheckCH;
    int   i4OutputType;
    int   i4Playtime;
    int  recordDevice;
    text_t    title;
    text_t    text;
    text_t    left_btn;
    text_t    center_btn;
    text_t    right_btn;
};

#ifdef GET_HEADSET_STATE
static int init_Headset(void)
{
#define ACCDET_IOC_MAGIC 'A'
#define ACCDET_INIT      _IO(ACCDET_IOC_MAGIC,0)
#define ACCDET_PATH      "/dev/accdet"

    int fd = open(ACCDET_PATH, O_RDONLY);
    if (fd < 0)
    {
        ALOGD(TAG "open %s failed, fd = %d", ACCDET_PATH, fd);
        return -1;
    }
    if (ioctl(fd, ACCDET_INIT, 0) < 0)
    {
        ALOGE(TAG "ioctl ACCDET_INIT failed\n");
        goto out;
    }
out:
    if (fd)
    {
        close(fd);
    }
    return 0;
}

static int get_headset_info(void)
{
    int ret = 0;
    int fd = -1;
    char rbuf[BUF_LEN] = {'\0'};
    char wbuf[BUF_LEN] = {'1'};
    char wbuf1[BUF_LEN] = {'2'};
    fd = open(HEADSET_STATE_PATH, O_RDONLY, 0);
    if (fd < 0)
    {
        ALOGD(TAG "Can't open %s\n", HEADSET_STATE_PATH);
        ret = -1;
        goto out;
    }

    if (read(fd, rbuf, BUF_LEN) == -1)
    {
        ALOGD(TAG "Can't read %s\n", HEADSET_STATE_PATH);
        ret = -2;
        close(fd);
        goto out;
    }

    if (!strncmp(wbuf, rbuf, BUF_LEN))
    {
        ret = 1;
        close(fd);
        goto out;
    }
    else if (!strncmp(wbuf1, rbuf, BUF_LEN))
    {
        ret = 2;
    }
    else
    {
        ret = 0;
    }
    close(fd);

out:
    return ret;
}
#endif

static int read_preferred_ringtone_time(void)
{
    int time = 0;
    char *pTime = NULL;
    char uName[64];
    int n = 0;

    memset(uName, 0, sizeof(uName));
    n = sprintf(uName, "%s", "Audio.Ringtone");
    if (n < 0 || n >= sizeof(uName))
    {
        ALOGD("[WARN] sprintf error from uName");
    }

    pTime = ftm_get_prop(uName);
    if (pTime != NULL)
    {
        time = (int)atoi(pTime);
        ALOGD("preferred_ringtone_time: %d sec\n", time);
    }
    else
    {
        ALOGD("preferred_ringtone_time can't get\n");
    }
    return time;
}

#define FREQ_L_HIGH 1
#define FREQ_L_LOW  2
#define FREQ_R_HIGH 3
#define FREQ_R_LOW  4

static int read_preferred_ringtone_freq_ex(int ich)
{
    int freq = 0;
    char *pFreq = NULL;
    char uName[64];
    int n = 0;

    memset(uName, 0, sizeof(uName));
    if ( ich == FREQ_L_HIGH ) {
        n = sprintf(uName, "%s", "Freq.Ringtone_L_high");        
    } else if ( ich == FREQ_L_LOW ) {
        n = sprintf(uName, "%s", "Freq.Ringtone_L_low");
    } else if ( ich == FREQ_R_HIGH ) {
        n = sprintf(uName, "%s", "Freq.Ringtone_R_high");
    } else if ( ich == FREQ_R_LOW ) {
        n = sprintf(uName, "%s", "Freq.Ringtone_R_low");
    } else {
        n = sprintf(uName, "%s", "Freq.Ringtone");
    }
    
    if (n < 0 || n >= sizeof(uName))
    {
        ALOGD("[WARN] sprintf error from uName");
    }

    pFreq = ftm_get_prop(uName);
    if (pFreq != NULL)
    {
        freq = (int)atoi(pFreq);
        ALOGD("preferred_ringtone_freq: %d hz\n", freq);
    }
    else
    {
        ALOGE("preferred_ringtone_freq can't get\n");
    }

    if (freq <= 0)
    {
        freq = 1000;
        ALOGE("preferred_ringtone_freq not valid, return value freq = 1000\n");
    }

    return freq;
}

static int read_preferred_ringtone_freq(void)
{
    int freq = 0;
    char *pFreq = NULL;
    char uName[64];
    int n = 0;

    memset(uName, 0, sizeof(uName));
    n = sprintf(uName, "%s", "Freq.Ringtone");
    if (n < 0 || n >= sizeof(uName))
    {
        ALOGD("[WARN] sprintf error from uName");
    }

    pFreq = ftm_get_prop(uName);
    if (pFreq != NULL)
    {
        freq = (int)atoi(pFreq);
        ALOGD("preferred_ringtone_freq: %d hz\n", freq);
    }
    else
    {
        ALOGE("preferred_ringtone_freq can't get\n");
    }

    if (freq <= 0)
    {
        freq = 1000;
        ALOGE("preferred_ringtone_freq not valid, return value freq = 1000\n");
    }

    return freq;
}

static pass_channl_criteria_t read_preferred_ch_to_check(const int device)
{
    pass_channl_criteria_t chToCheck;
    char *pChToCheck = NULL;
    char uName[64];
    int n = 0;

    memset(uName, 0, sizeof(uName));
    if (device == Output_LPK) //speaker
    {
        n = sprintf(uName, "%s", "Loopback.Speaker.chToCheck");
    }
    else if (device == Output_HS) //receiver
    {
        n = sprintf(uName, "%s", "Loopback.Receiver.chToCheck");
    }

    if (n < 0 || n >= sizeof(uName))
    {
        ALOGD("[WARN] sprintf error from uName");
    }

    pChToCheck = ftm_get_prop(uName);
    if (pChToCheck != NULL)
    {
        if (strcmp(pChToCheck, "L") == 0)
        {
            chToCheck = L_CH_ONLY;
        }
        else if (strcmp(pChToCheck, "R") == 0)
        {
            chToCheck = R_CH_ONLY;
        }
        else if (strcmp(pChToCheck, "ALL") == 0)
        {
            chToCheck = ALL_CH;
        }
        else
        {
           ALOGD("[ERROR] given parameter not recognized");
           ALOGD("\t, available option: L, R, ALL");
           ALOGD("\t use default option: ALL");
           chToCheck = ALL_CH;
        }
        ALOGD("read_preferred_ch_to_check: %s\n", pChToCheck);
    }
    else
    {
        ALOGD("[Warning] read_preferred_ch_to_check: can't get parameter\n");
        ALOGD("\t: %s", uName);
        ALOGD("\t use default option: ALL");
        chToCheck = (device == Output_LPK) ? ALL_CH : R_CH_ONLY;
        ALOGD("[Warning] Using default parameters");
        ALOGD("\tDefault %s = %s", uName, (device == Output_LPK) ? "ALL" : "R");
    }
    return chToCheck;
}

static void read_index_magnitude(char *r_value, char *name, int micType)
{
    int i = 0;
    char str[128];
    char *value = NULL;
    char *prop = ftm_get_prop(name);

    if (prop == NULL) {
        ALOGD("Can't get property : %s", name);
        return;
    }
    strncpy(str, prop, sizeof(str) - 1);
    str[sizeof(str) - 1] = '\0';
    ALOGD("%s :: size %lu", prop, strlen(prop));
    ALOGD("%s :: size %lu", str, sizeof(str) - 1);
    value = strtok(str," ,");
    while ((value != NULL) && (i < micType))
    {
        value = strtok(NULL, " ,");
        i++;
    }

    if (value != NULL)
    {
        strncpy(r_value, value, strlen(value)); // r_value = micType value
        ALOGD("%s :: size %lu", value, strlen(value));
    } else {
        if (sizeof(str)<= 0)
            strncpy(r_value, str, sizeof(str) - 1); // r_value = first value
        else
            ALOGD("Fail! Cannot get value!");
    }
}

static void read_preferred_magnitude(const int device, int micType, int *pUpperL, int *pLowerL, int *pUpperR, int *pLowerR)
{
    char magLowerL[32] = {0}, magUpperL[32] = {0};
    char magLowerR[32] = {0}, magUpperR[32] = {0};
    char uMagLowerL[64] = {0}, uMagUpperL[64] = {0};
    char uMagLowerR[64] = {0}, uMagUpperR[64] = {0};
    int n[4] = {0};

    memset(uMagLowerL, 0, sizeof(uMagLowerL));
    memset(uMagUpperL, 0, sizeof(uMagUpperL));
    memset(uMagLowerR, 0, sizeof(uMagLowerR));
    memset(uMagUpperR, 0, sizeof(uMagUpperR));

    if (device == Output_LPK) //speaker
    {
        n[0] = sprintf(uMagLowerL, "%s", "Lower.Magnitude.Speaker.L");
        n[1] = sprintf(uMagUpperL, "%s", "Upper.Magnitude.Speaker.L");
        n[2] = sprintf(uMagLowerR, "%s", "Lower.Magnitude.Speaker.R");
        n[3] = sprintf(uMagUpperR, "%s", "Upper.Magnitude.Speaker.R");
    }
    else if (device == Output_HS) //receiver
    {
        n[0] = sprintf(uMagLowerL, "%s", "Lower.Magnitude.Receiver.L");
        n[1] = sprintf(uMagUpperL, "%s", "Upper.Magnitude.Receiver.L");
        n[2] = sprintf(uMagLowerR, "%s", "Lower.Magnitude.Receiver.R");
        n[3] = sprintf(uMagUpperR, "%s", "Upper.Magnitude.Receiver.R");
    }

    if (n[0] < 0 || n[1] < 0 || n[2] < 0 || n[3] < 0
        || n[0] >= sizeof(uMagLowerL) || n[1] >= sizeof(uMagUpperL)
        || n[2] >= sizeof(uMagLowerR) || n[3] >= sizeof(uMagUpperR))
    {
        ALOGD("[WARN] sprintf error from Magnitude");
    }

    read_index_magnitude(magLowerL, uMagLowerL, micType);
    read_index_magnitude(magLowerR, uMagUpperL, micType);
    read_index_magnitude(magUpperL, uMagLowerR, micType);
    read_index_magnitude(magUpperR, uMagUpperR, micType);

    if (*magLowerL != 0 && *magLowerR != 0 && *magUpperL != 0 && *magUpperR != 0)
    {
        *pLowerL = (int)atoi(magLowerL);
        *pUpperL = (int)atoi(magLowerR);
        *pLowerR = (int)atoi(magUpperL);
        *pUpperR = (int)atoi(magUpperR);
        ALOGD("read_preferred_magnitude: [%s] Lower.Magnitude.L:%d,Upper.Magnitude.L:%d\n", ouput_dev_name[device], *pLowerL, *pUpperL);
        ALOGD("read_preferred_magnitude: [%s] Lower.Magnitude.R:%d,Upper.Magnitude.R:%d\n", ouput_dev_name[device], *pLowerR, *pUpperR);
    }
    else
    {
        ALOGD("[Warning] Read Parameters Failed: Lower/Upper.Magnitude parameters in factory.ini\n");
        ALOGD("[Warning] \t%s, %s, %s, %s", uMagLowerL, uMagUpperL, uMagLowerR, uMagUpperR);
        ALOGD("[Warning] Using default parameters");
        *pLowerL = (device == Output_LPK) ? 1000000 : 100;
        *pUpperL = 1000000000;
        *pLowerR = (device == Output_LPK) ? 500000 : 1000;
        *pUpperR = 1000000000;
        ALOGD("\tDefault %s = %d", uMagLowerL, *pLowerL);
        ALOGD("\tDefault %s = %d", uMagUpperL, *pUpperL);
        ALOGD("\tDefault %s = %d", uMagLowerR, *pLowerR);
        ALOGD("\tDefault %s = %d", uMagUpperR, *pUpperR);
    }
}
static bool read_preferred_recorddump(void)
{
    char *pDump = NULL;
    char uName[64];
    int n = 0;

    memset(uName, 0, sizeof(uName));
    n = sprintf(uName, "%s", "Audio.Record.Dump");
    if (n < 0 || n >= sizeof(uName))
    {
        ALOGD("[WARN] sprintf error from uName");
    }

    pDump = ftm_get_prop(uName);
    ALOGD("pDump:%s", pDump);

    if (pDump != NULL)
    {
        if (!strcmp(pDump, "1"))
        {
            ALOGD("Dump record data");
            return true;
        }
        else
        {
            ALOGD("No need to dump record data");
            return false;
        }
    }
    else
    {
        ALOGD("Dump record prop can't get");
        return false;
    }
}

static int read_preferred_receiver_time(void)
{
    int time = 0;
    char *pTime = NULL;
    char uName[64];
    int n = 0;

    memset(uName, 0, sizeof(uName));
    n = sprintf(uName, "%s", "Audio.Receiver");
    if (n < 0 || n >= sizeof(uName))
    {
        ALOGD("[WARN] sprintf error from uName");
    }

    pTime = ftm_get_prop(uName);
    if (pTime != NULL)
    {
        time = (int)atoi(pTime);
        ALOGD("preferred_receiver_time: %d sec\n", time);
    }
    else
    {
        ALOGD("preferred_receiver_time can't get\n");
    }
    return time;
}

static int read_preferred_sw_gain(void)
{
    int gain = 2; //default 2:12dB
    char *pGain = NULL;

    pGain = ftm_get_prop("Audio.SWGain");
    if (pGain != NULL){
        gain = (int)atoi(pGain);
        ALOGD("read_preferred_sw_gain: %d \n",gain);
    }
    else{
        ALOGD("read_preferred_sw_gain can't get\n");
    }
    return gain;
}

static bool read_loopback_mic_inputs(int micType)
{
    bool supported = false;
    char *pMicSets = NULL;
    char micName[10];
    int n = 0;

    pMicSets = ftm_get_prop("Audio.Mic.Sets");
    if (pMicSets == NULL){
        ALOGD("Can't get property : Audio.Mic.Sets\n");
        return false;
    }
    n = sprintf(micName, "%d", micType);
    if (n < 0 || n >= sizeof(micType))
    {
        ALOGD("[WARN] sprintf error from micType");
    }

    if (strstr(pMicSets, micName)) {
        supported = true;
    }
    return supported;
}

static int Audio_headset_hook_info()
{
    int ret = 0;
    if (HeadsetFd)
    {
        ALOGV(TAG "GET_BUTTON_STATUS ");
        ret = ::ioctl(HeadsetFd, GET_BUTTON_STATUS, 0);
        ALOGV("Audio_headset_hook_info ret = %d", ret);
    }
    return ret;
}

static void Audio_headset_update_info(struct mAudio *hds, char *info)
{
    int fd = -1;
    int hookstatus = 0;
    char *ptr ;
    int OriginState = hds->avail;

    hds->Headset_mic = 0;
    fd = open(HEADSET_STATE_PATH, O_RDONLY, 0);
    if (fd == -1)
    {
        ALOGD(TAG "Can't open %s\n", HEADSET_STATE_PATH);
        hds->avail = false;
        goto EXIT;
    }
    if (read(fd, rbuf, BUF_LEN) == -1)
    {
        ALOGD(TAG "Can't read %s\n", HEADSET_STATE_PATH);
        hds->avail = false;
        close(fd);
        goto EXIT;
    }

    if (!strncmp(wbuf, rbuf, BUF_LEN))
    {
        ALOGD(TAG "state == 1");
        hds->avail = true;
        hds->Headset_mic = 1;
        close(fd);
        goto EXIT;
    }

    if (!strncmp(wbuf1, rbuf, BUF_LEN))
    {
        ALOGD(TAG "state == 2");
        hds->avail = true;
    }
    else
    {
        ALOGV(TAG "state == %s", rbuf);
        hds->avail = false;
    }
    close(fd);

EXIT:
    if (OriginState != hds->avail)
    {
        ALOGD(TAG "OriginState(%d) != hds->avail(%d)", OriginState, hds->avail);
        hds->Headset_change = true;
    }
    else
    {
        hds->Headset_change = false;
    }

    int len1, len2, len3, len4;

    /* preare text view info */
    ptr = info;
    len1 = sprintf(ptr, "%s \n", uistr_info_audio_loopback_note);
    if (len1 < 0)
    {
        ALOGD("[WARN] sprintf error from ptr, len1 = %d", len1);
        return ;
    }

    ptr += len1;
    len3 = sprintf(ptr, "%s %s\n\n", uistr_info_audio_headset_avail, hds->avail ? uistr_info_audio_yes : uistr_info_audio_no);
    if (len3 < 0)
    {
        ALOGD("[WARN] sprintf error from ptr, len3 = %d", len3);
        return ;
    }

    ptr += len3;
    len4 = sprintf(ptr, "%s %s\n\n", uistr_info_audio_headset_mic_avail, hds->Headset_mic ? uistr_info_audio_yes : uistr_info_audio_no);
    if (len4 < 0)
    {
        ALOGD("[WARN] sprintf error from ptr, len4 = %d", len4);
        return ;
    }

    ptr += len4;
    len2 = 0;

    if (len1 < 0 || len2 < 0 || len3 < 0 || len4 < 0)
    {
        ALOGD("[WARN] sprintf error from ptr");
    }

    print_len1 = len1 + len2 + len3 + len4;

#ifdef HEADSET_BUTTON_DETECTION
    hookstatus = Audio_headset_hook_info(); // get hook information
    if (hds->avail)
    {
        print_len2 = sprintf(ptr, "%s %s\n\n", uistr_info_audio_headset_Button, hookstatus ? uistr_info_audio_press : uistr_info_audio_release);
        if (print_len2 < 0)
            ALOGD("[WARN] sprintf error from print_len2");
        else
            ptr += print_len2;
    }
#endif

    if (b_incomplete_flag == true)
    {
        if ((b_mic1_loopback == false) || (b_mic2_loopback == false))
        {
            ptr += sprintf(ptr, "[%s] \n\n", "Test Loopback Case In-Complete");
        }
    }

    return;
}

static void *Audio_Headset_detect_thread(void *mPtr)
{
    struct mAudio *hds = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;

    ALOGD(TAG "%s: Start\n", __FUNCTION__);

    // open headset device
    HeadsetFd = open(HEADSET_PATH, O_RDONLY);
    if (HeadsetFd < 0)
    {
        ALOGE("open %s error fd = %d", HEADSET_PATH, HeadsetFd);
        return 0;
    }

    // set headset state to 1 , enable hook
    ::ioctl(HeadsetFd, SET_CALL_STATE, 1);

    // 1st turn on
    Audio_headset_update_info(hds, hds->info);  // get headset information
    if (hds->avail)
    {
        HeadsetMic_EarphoneLR_Loopback(MIC1_ON, hds->Headset_mic);
        hds->Headset_change = false;
    }
    else
    {
        PhoneMic_Receiver_Loopback(MIC1_ON);
    }

    while (1)
    {
        usleep(100000);
        if (hds->exit_thd)
        {
            break;
        }

        Audio_headset_update_info(hds, hds->info);  // get headset information

        if (hds->Headset_change)    // Headset device change
        {
            if (hds->avail == true)    // use headset
            {
                ALOGD(TAG  " --------Audio_Headset_detect_thread  : Headset plug-in (%d)\n", g_loopback_item);
                if (g_loopback_item == 1)
                {
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_OFF);  // disable Receiver MIC1 loopback
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC1_ON, hds->Headset_mic);  // enable Earphone MIC1 loopback
                }
                else if (g_loopback_item == 2)
                {
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_OFF);  // disable MIC2 loopback
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC2_ON, hds->Headset_mic);  // enable Earphone MIC2 loopback
                }
                g_mic_change = 0;
                hds->Headset_change = false;
            }
            else    // use handset
            {
                ALOGD(TAG " --------Audio_Headset_detect_thread  : Headset plug-out (%d)\n", g_loopback_item);
                if (g_loopback_item == 1)
                {
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC1_OFF, hds->Headset_mic);  // disable Earphone MIC1 loopback
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_ON);  // enable Receiver MIC1 loopback
                }
                else if (g_loopback_item == 2)
                {
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC2_OFF, hds->Headset_mic);  // disable Earphone MIC2 loopback
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_ON);  // enable Receiver MIC2 loopback
                }
                g_mic_change = 0;
                hds->Headset_change = false;
            }
        }
        else if (g_mic_change == 1)    // mic1 <-> mic2
        {
            if (hds->avail == false)    // without Earphone plug in/out
            {
                ALOGD(TAG " --------Without Earphone, g_mic_change = 1 \n");
                g_mic_change = 0;
                if (g_loopback_item == 1)
                {
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_OFF);  // disable Earphone MIC2 loopback
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_ON);  // enable Receiver MIC1 loopback
                }
                else if (g_loopback_item == 2)
                {
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_OFF);  // disable Earphone MIC1 loopback
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_ON);  // enable Receiver MIC2 loopback
                }
            }
            else    // with Earphone plug in/out
            {
                ALOGD(TAG " --------With Earphone, g_mic_change = 1 \n");
                g_mic_change = 0;
                if (g_loopback_item == 1)
                {
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC2_OFF, hds->Headset_mic);  // disable Earphone MIC2 loopback
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC1_ON, hds->Headset_mic);  // enable Receiver MIC1 loopback
                }
                else if (g_loopback_item == 2)
                {
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC1_OFF, hds->Headset_mic);  // disable Earphone MIC1 loopback
                    usleep(3000);
                    HeadsetMic_EarphoneLR_Loopback(MIC2_ON, hds->Headset_mic);  // enable Receiver MIC2 loopback
                }
            }
        }

        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }

    ALOGD(TAG "%s: Audio_Headset_detect_thread Exit (%d) \n", __FUNCTION__, g_loopback_item);

    // set headset state to 0 , lose headset device.
    ::ioctl(HeadsetFd, SET_CALL_STATE, 0);

    close(HeadsetFd);
    HeadsetFd = 0;

    pthread_exit(NULL); // thread exit
    return NULL;
}


#ifdef FEATURE_FTM_ACSLB
static void *PhoneMic_Receiver_Headset_Acoustic_Loopback(void *mPtr)
{
    struct mAudio *hds = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;
    int Acoustic_Status = 0;
    int Acoustic_Type = DUAL_MIC_WITHOUT_DMNR_ACS_ON;

    ALOGD(TAG "%s: Start\n", __FUNCTION__);

    // open headset device
    HeadsetFd = open(HEADSET_PATH, O_RDONLY);
    if (HeadsetFd < 0)
    {
        ALOGE("open %s error fd = %d", HEADSET_PATH, HeadsetFd);
        return 0;
    }

    //set headset state to 1 , enable hook
    ::ioctl(HeadsetFd, SET_CALL_STATE, 1);

    const char *numMicString = appGetFeatureOptionValue(MTK_AUDIO_NUMBER_OF_MIC);
    if (numMicString && atoi(numMicString) >= 2) {
        Acoustic_Type = DUAL_MIC_WITH_DMNR_ACS_ON;
    }
    Audio_headset_update_info(hds, hds->info);  // get headset information
    PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Type, &Acoustic_Status, hds->avail);

    while (1)
    {
        usleep(100000);
        if (hds->exit_thd)
        {
            break;
        }

        pthread_mutex_lock(&hds->mHeadsetMutex);
        Audio_headset_update_info(hds, hds->info);  // get headset information

        if (hds->Headset_change)
        {
            PhoneMic_Receiver_Acoustic_Loopback(ACOUSTIC_STATUS, &Acoustic_Status, hds->avail);
            if (!(Acoustic_Status & 0x1))
            {
                hds->Headset_change = false;
                pthread_mutex_unlock(&hds->mHeadsetMutex);
                continue;
            }
        }

        if (hds->avail && hds->Headset_change)
        {
            ALOGD(TAG  " --------Audio_Headset_detect_thread  : Headset plug-in (%d)\n", hds->avail);
            PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Status - 1, &Acoustic_Status, hds->avail);
            pthread_mutex_unlock(&hds->mHeadsetMutex);
            usleep(50000);
            pthread_mutex_lock(&hds->mHeadsetMutex);
            PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Status, &Acoustic_Status, hds->avail);
            hds->Headset_change = false;
        }
        else if (hds->Headset_change)
        {
            ALOGD(TAG " --------Audio_Headset_detect_thread  : Headset plug-out (%d)\n", hds->avail);
            PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Status - 1, &Acoustic_Status, hds->avail);
            pthread_mutex_unlock(&hds->mHeadsetMutex);
            usleep(50000);
            pthread_mutex_lock(&hds->mHeadsetMutex);
            PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Status, &Acoustic_Status, hds->avail);
            hds->Headset_change = false;
        }

        pthread_mutex_unlock(&hds->mHeadsetMutex);
        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }

    ALOGD(TAG "%s: Audio_Headset_detect_thread Exit (%d) \n", __FUNCTION__, hds->avail);

    // set headset state to 0 , lose headset device.
    ::ioctl(HeadsetFd, SET_CALL_STATE, 0);

    close(HeadsetFd);
    HeadsetFd = -1;

    pthread_exit(NULL); // thread exit
    return NULL;
}
#endif

#ifndef FEATURE_FTM_HEADSET
static void *Audio_Mic_change_thread(void *mPtr)
{
    struct mAudio *hds = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;

    ALOGD(TAG "%s: Start\n", __FUNCTION__);

    // default use mic1
    PhoneMic_Receiver_Loopback(MIC1_ON);

    while (1)
    {
        char *ptr;
        usleep(100000);
        if (hds->exit_thd)
        {
            break;
        }
        // without Earphone plug in/out
        if (g_mic_change == 1)
        {
            ALOGD(TAG " --------Without Earphone, g_mic_change = 1 \n");
            g_mic_change = 0;
            if (g_loopback_item == 1)
            {
                usleep(3000);
                PhoneMic_Receiver_Loopback(MIC2_OFF);  // disable Earphone MIC2 loopback
                usleep(3000);
                PhoneMic_Receiver_Loopback(MIC1_ON);  // enable Receiver MIC1 loopback
            }
            else if (g_loopback_item == 2)
            {
                usleep(3000);
                PhoneMic_Receiver_Loopback(MIC1_OFF);  // disable Earphone MIC1 loopback
                usleep(3000);
                PhoneMic_Receiver_Loopback(MIC2_ON);  // enable Receiver MIC2 loopback
            }
        }
        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }

    ALOGD(TAG "%s: Audio_Headset_detect_thread Exit (%d) \n", __FUNCTION__, g_loopback_item);

    pthread_exit(NULL); // thread exit
    return NULL;
}
#endif
static void *Audio_Receiver_Playabck_thread(void *mPtr)
{
    struct mAudio *hds  = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;
    int    play_time    = 0;
    ALOGD(TAG "%s: Start\n", __FUNCTION__);
    play_time = read_preferred_receiver_time();
    RecieverTest(1);
    if (play_time > 0)
    {
        usleep(play_time * 1000 * 1000);
        RecieverTest(0);
    }
    while (1)
    {
        usleep(100000);
        if (hds->exit_thd)
        {
            break;
        }
        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }
    if (play_time <= 0)
    {
        RecieverTest(0);
    }
    ALOGD(TAG "%s: Audio_Headset_detect_thread Exit \n", __FUNCTION__);
    pthread_exit(NULL); // thread exit
    return NULL;
}
#if 0
static void *Audio_LoudSpk_Playabck_thread(void *mPtr)
{
    struct mAudio *hds  = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;
    //int    play_time    = 0;
    ALOGD(TAG "%s: Start\n", __FUNCTION__);
    //play_time = read_preferred_ringtone_time();
    LouderSPKTest(1, 1);
    while (1)
    {
        usleep(100000);
        if (hds->exit_thd)
        {
            break;
        }
        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }

    LouderSPKTest(0, 0);
    ALOGD(TAG "%s: Audio_LoudSpk_Playabck_thread Exit \n", __FUNCTION__);
    pthread_exit(NULL); // thread exit
    return NULL;
}
#endif
float ConvertToDB(unsigned int *pData)
{
	float dBValue;

    if (*pData == 0)
        return 0;
    dBValue = 10*log10((float)pData[0]);
    return dBValue;
}

float CalculateTHD(unsigned int sampleRate, short *pData, kal_uint32 signalFrequency, float frequencyMargin)
{
    if (sampleRate <= 0) {
        ALOGD("ERROR Wrong sample rate: %d", sampleRate);
        return 0;
    }
    unsigned int baseI = FFT_SIZE/2 * 2 * signalFrequency / sampleRate;
    unsigned int iMargin = baseI * frequencyMargin;
    unsigned int baseSignalLoc = 0, peakLoc = 0, baseSignalMag = 0, peakMagValue = 0;
    unsigned int IdxStart = 0;
    unsigned int magData[FFT_SIZE/2] = {0};
    kal_uint32 freqData[3] = {0};
    kal_uint32 HFreq[6] = {0}, HData[6] = {0}, P0 = 0, Pothers = 0;
    kal_uint32 i = 0, j = 0, k = 0;
    float dFreqIdx = (float)sampleRate/FFT_SIZE;
    float thdPercentage = 0;

    Complex ComData[FFT_SIZE];
    ApplyFFT(sampleRate, pData, 0, ComData, freqData, magData);

    //Search base signal frequency location
    for (i=baseI-2; i<=baseI+2; i++)
    {
        if (magData[i] > baseSignalMag)
        {
            baseSignalLoc = i;
            baseSignalMag = magData[i];
        }
    }

    //Search peak signal frequency location
    for (i=IdxStart; i<FFT_SIZE/2; i++)
    {
        if (magData[i] > peakMagValue)
        {
            peakLoc = i;
            peakMagValue = magData[i];
        }
    }

    if (baseSignalLoc != peakLoc)
    {
        ALOGD("ERROR Wrong peak signal, baseSignalLoc:%d, peakLoc:%d", baseSignalLoc, peakLoc);
        return 0;
    }

    HFreq[0] = baseSignalLoc * dFreqIdx;
    HData[0] = baseSignalMag;
    P0 = baseSignalMag;

    ALOGD("baseSignalLoc:%d, P0:%d, iMargin:%d", baseSignalLoc, P0, iMargin);
    ALOGD("HFreq[%d]:%d,HData[%d]:%d", 0, HFreq[0], 0, HData[0]);

    //Search  ith harmonic signal frequency location and caculate rms power
    if (baseSignalLoc != 0)
    {
        unsigned int multiBaseSignalLoc = 0, multiBaseSignalMag = 0;
        k = 1;
        for (i=baseSignalLoc*2; i<FFT_SIZE/2; i+=baseSignalLoc)
        {
            multiBaseSignalMag = 0;

            //search local peak
            for (j=i-3; j<=i+3; j++)
            {
                if (j >= FFT_SIZE/2) break;
                if (magData[j] > multiBaseSignalMag)
                {
                    multiBaseSignalLoc = j;
                    multiBaseSignalMag = magData[j];
                }
                //ALOGD("HFreq[%d]:%d,HData[%d]:%d", j ,(unsigned int)(j * dFreqIdx), j, magData[j]);
            }


            for (j = multiBaseSignalLoc- iMargin; j <= multiBaseSignalLoc+ iMargin; j++)
            {
                Pothers += magData[j];
            }
            if (k < 6)
            {
                ALOGD("HFreq[%d]:%d,HData[%d]:%d", k, (unsigned int)(multiBaseSignalLoc * dFreqIdx), k, magData[multiBaseSignalLoc]);
            }
            k++;
        }
        //Pothers = sqrt(Pothers);
        ALOGD("Pothers:%d", Pothers);
    }
    if (P0 != 0)
        thdPercentage = ((float)Pothers/P0) * 100;
    return thdPercentage;
}

void CalculateStatistics(float *pData, int length, int start, int end, audio_data_statistic *data_stat)
{
    float sum = 0, mean = 0;
    float sq_sum = 0, std_dev = 0;
    float max = pData[start];
    float min = pData[start];
    int i = 0;

    //start: how many data ignored at the start
    //end:  how many data ignore at the end
    int len = length - start - end;

    //Calculate max, min and mean value
    for (i=start; i<length-end; i++)
    {
        if (pData[i] > max )
        {
            max = pData[i];
        }
        else if (pData[i] < min)
        {
            min = pData[i];
        }
        sum += pData[i];
    }
    mean = sum / len ;

    //Calculate deviation
    for (i=start; i<length-end; i++)
    {
        sq_sum += (pData[i]-mean) * (pData[i]-mean);
    }
    std_dev = sqrt(sq_sum / len);

    data_stat->mean = mean;
    data_stat->deviation = std_dev;
    data_stat->max = max;
    data_stat->min = min;
}

static void * Audio_Response_Record_thread(void *mPtr)
{
    struct mAudio *hds  = (struct mAudio *)mPtr;
    ALOGD(TAG "%s: Start", __FUNCTION__);
    usleep(100000);
    short pbuffer[8192]={0};
    short pbufferL[4096]={0};
	short pbufferR[4096]={0};
    unsigned int freqDataL[3]={0},magDataL[3]={0};
	unsigned int freqDataR[3]={0},magDataR[3]={0};
    int len = 0;
    float AmpDB = 0;
    float RspDB[FFT_DAT_MAXNUM];
    int swGain = read_preferred_sw_gain();
    audio_data_statistic rsp_sta;


    uint32_t sampleRate = 0;
    recordInit(hds->recordDevice, &sampleRate);
    SetRecordEnhance(false);
    while (1) {
        memset(pbuffer,0,sizeof(pbuffer));
        memset(pbufferL,0,sizeof(pbufferL));
        memset(pbufferR,0,sizeof(pbufferR));

        for(int i = 0 ; i < 4096 ; i++)
        {
           pbufferL[i] = pbuffer[2 * i] << swGain;     //apply +12db gain
           pbufferR[i] = pbuffer[2 * i + 1] << swGain; //apply +12db gain
        }

        memset(freqDataL,0,sizeof(freqDataL));
        memset(freqDataR,0,sizeof(freqDataR));
        memset(magDataL,0,sizeof(magDataL));
        memset(magDataR,0,sizeof(magDataR));
        ApplyFFT256(sampleRate,pbufferL,0,freqDataL,magDataL);
        ApplyFFT256(sampleRate,pbufferR,0,freqDataR,magDataR);

        //Calculate target tone peak value (dB)
        AmpDB = ConvertToDB(&magDataL[0]);
        ALOGD("freqDataL:%d, AmpDB:%f",freqDataL[0], AmpDB);

        RspDB[len] = AmpDB;
        len++;

        if (hds->exit_thd){
	       break;
	     }
      }
      CalculateStatistics(RspDB, len, 1, 1, &rsp_sta);
      ALOGD(TAG "Max:%f, Min:%f",rsp_sta.max, rsp_sta.min);
      ALOGD(TAG "Mean:%f, Deviation:%f",rsp_sta.mean, rsp_sta.deviation);

      if(rsp_sta.deviation < 0.5)
      {
          snprintf(hds->info + strlen(hds->info), sizeof(hds->info) - strlen(hds->info), "Check freq pass.\n");
          ALOGD(" @ info : %s",hds->info);
      }

      SetRecordEnhance(true);

      if(hds->i4OutputType == Output_LPK)//speaker
      {
          return_data.spk_response.freqresponse[FreqLoop].mean = rsp_sta.mean;
          return_data.spk_response.freqresponse[FreqLoop].deviation = rsp_sta.deviation;
          return_data.spk_response.freqresponse[FreqLoop].max = rsp_sta.max;
          return_data.spk_response.freqresponse[FreqLoop].min = rsp_sta.min;

          ALOGD(TAG "Frequency Response(Speaker) at %d Hz: %f", Speaker_Frequency[FreqLoop], return_data.spk_response.freqresponse[FreqLoop].mean);
      }
      else if(hds->i4OutputType == Output_HS)
      {
          return_data.rcv_response.freqresponse[FreqLoop].mean = rsp_sta.mean;
          return_data.rcv_response.freqresponse[FreqLoop].deviation = rsp_sta.deviation;
          return_data.rcv_response.freqresponse[FreqLoop].max = rsp_sta.max;
          return_data.rcv_response.freqresponse[FreqLoop].min = rsp_sta.min;
          ALOGD(TAG "Frequency Response(Receiver) at %d Hz: %f", Receiver_Frequency[FreqLoop], return_data.rcv_response.freqresponse[FreqLoop].mean);
      }

      ALOGD(TAG "%s: Stop", __FUNCTION__);
      pthread_exit(NULL); // thread exit
      return NULL;
}

static void * Audio_THD_Record_thread(void *mPtr)
{
    struct mAudio *hds  = (struct mAudio *)mPtr;
    ALOGD(TAG "%s: Start", __FUNCTION__);
    usleep(100000);
    int freqOfRingtone = read_preferred_ringtone_freq();
    short pbuffer[8192]={0};
    short pbufferL[4096]={0};
	short pbufferR[4096]={0};
    unsigned int freqDataL[3]={0},magDataL[3]={0};
	unsigned int freqDataR[3]={0},magDataR[3]={0};
    float thdPercentage = 0;
    int len = 0;
    float thdData[FFT_DAT_MAXNUM];
    audio_data_statistic thd_sta;

    uint32_t sampleRate = 0;
    recordInit(hds->recordDevice, &sampleRate);
    //SetRecordEnhance(false);
    while (1) {
        memset(pbuffer,0,sizeof(pbuffer));
        memset(pbufferL,0,sizeof(pbufferL));
        memset(pbufferR,0,sizeof(pbufferR));

        for(int i = 0 ; i < 4096 ; i++)
        {
           pbufferL[i] = pbuffer[2 * i];
           pbufferR[i] = pbuffer[2 * i + 1];
        }

        memset(freqDataL,0,sizeof(freqDataL));
        memset(freqDataR,0,sizeof(freqDataR));
        memset(magDataL,0,sizeof(magDataL));
        memset(magDataR,0,sizeof(magDataR));
        ApplyFFT256(sampleRate,pbufferL,0,freqDataL,magDataL);
        ApplyFFT256(sampleRate,pbufferR,0,freqDataR,magDataR);


        for(int i = 0;i < 3 ;i ++)
        {
            ALOGD("freqDataL[%d]:%d,magDataL[%d]:%d",i,freqDataL[i],i,magDataL[i]);
            ALOGD("freqDataR[%d]:%d,magDataR[%d]:%d",i,freqDataR[i],i,magDataR[i]);
        }

	    //if (((freqDataL[0] <= highFreq && freqDataL[0] >= lowFreq) && (magDataL[0] <= magUpper && magDataL[0] >= magLower))&&((freqDataR[0] <= highFreq && freqDataR[0] >= lowFreq) && (magDataR[0] <= magUpper && magDataR[0] >= magLower)))
	    {
            thdPercentage = CalculateTHD(sampleRate, pbufferL, freqOfRingtone, 0.0);
            ALOGD("THD: %f", thdPercentage);
            thdData[len] = thdPercentage;
            len++;
        }

        if (hds->exit_thd){
	       break;
	     }
      }

      CalculateStatistics(thdData, len, 4, 1, &thd_sta);
      ALOGD(TAG "Max:%f, Min:%f",thd_sta.max, thd_sta.min);
      ALOGD(TAG "Mean:%f, Deviation:%f",thd_sta.mean, thd_sta.deviation);

      if(thd_sta.deviation < 0.5)
      {
          snprintf(hds->info + strlen(hds->info), sizeof(hds->info) - strlen(hds->info), "Check THD pass.\n");
          ALOGD(" @ info : %s",hds->info);
      }

      //SetRecordEnhance(true);

      if(hds->i4OutputType == Output_LPK)//speaker
      {
         return_data.spk_thd.thd.mean = thd_sta.mean;
         return_data.spk_thd.thd.deviation = thd_sta.deviation;
         return_data.spk_thd.thd.max = thd_sta.max;
         return_data.spk_thd.thd.min = thd_sta.min;
         ALOGD(TAG "ATA Return THD(Speaker): Mean = %f, Deviation = %f, Max = %f, Min = %f", return_data.spk_thd.thd.mean, return_data.spk_thd.thd.deviation, return_data.spk_thd.thd.max, return_data.spk_thd.thd.min);
      }
      else if(hds->i4OutputType == Output_HS)
      {
         return_data.rcv_thd.thd.mean = thd_sta.mean;
         return_data.rcv_thd.thd.deviation = thd_sta.deviation;
         return_data.rcv_thd.thd.max = thd_sta.max;
         return_data.rcv_thd.thd.min = thd_sta.min;
         ALOGD(TAG "ATA Return THD(Receiver): Mean = %f, Deviation = %f, Max = %f, Min = %f", return_data.rcv_thd.thd.mean, return_data.rcv_thd.thd.deviation, return_data.rcv_thd.thd.max, return_data.rcv_thd.thd.min);
      }
      ALOGD(TAG "%s: Stop", __FUNCTION__);
      pthread_exit(NULL); // thread exit
      return NULL;
}

static void *Audio_Record_thread(void *mPtr)
{
    struct mAudio *hds  = (struct mAudio *)mPtr;
    ALOGD(TAG "%s: Start", __FUNCTION__);
    usleep(100000);
    bool dumpFlag = read_preferred_recorddump();
    //dumpFlag=true;//for test
    int magLowerL, magUpperL;
    int magLowerR, magUpperR;
    read_preferred_magnitude(hds->i4OutputType, hds->phoneMicType, &magUpperL, &magLowerL, &magUpperR, &magLowerR);
    pass_channl_criteria_t chToCheck = read_preferred_ch_to_check(hds->i4OutputType);
    int freqOfRingtone = read_preferred_ringtone_freq();
    int freqOfRingtoneLeftHigh = read_preferred_ringtone_freq_ex(FREQ_L_HIGH);
    int freqOfRingtoneLeftLow = read_preferred_ringtone_freq_ex(FREQ_L_LOW);
    int freqOfRingtoneRightHigh = read_preferred_ringtone_freq_ex(FREQ_R_HIGH);
    int freqOfRingtoneRightLow = read_preferred_ringtone_freq_ex(FREQ_R_LOW);
    int lowFreq = freqOfRingtone * (1 - 0.05);
    int highFreq = freqOfRingtone * (1 + 0.05);
    short *pbuffer, *pbufferL, *pbufferR;
    unsigned int freqDataL[3] = {0}, magDataL[3] = {0};
    unsigned int freqDataR[3] = {0}, magDataR[3] = {0};
    int checkCnt = 0;
	int checkCntR = 0;
    uint32_t samplerate = 0;
	bool bpass = false;
	int pass_freqL = 0;
	int pass_freqR = 0;
	int pass_amplL = 0;
	int pass_amplR = 0;

    if (hds->i4OutputType == Output_LPK) //speaker
    {
        return_data.speaker.freqL = 0;
        return_data.speaker.freqR = 0;
        return_data.speaker.amplL = 0;
        return_data.speaker.amplR = 0;
    }
    else if (hds->i4OutputType == Output_HS)
    {
        return_data.receiver.freqL = 0;
        return_data.receiver.freqR = 0;
        return_data.receiver.amplL = 0;
        return_data.receiver.amplR = 0;
    }

    pbuffer = (short *)malloc(8192 * sizeof(short));
    if (pbuffer == NULL) {
        ALOGD(TAG "%s: pbuffer allocate fail !!", __FUNCTION__);
        return NULL;
    }

    pbufferL = (short *)malloc(4096 * sizeof(short));
    if (pbufferL == NULL) {
        ALOGD(TAG "%s: pbufferL allocate fail !!", __FUNCTION__);
        free(pbuffer);
        return NULL;
    }

    pbufferR = (short *)malloc(4096 * sizeof(short));
    if (pbufferR == NULL) {
        ALOGD(TAG "%s: pbufferR allocate fail !!", __FUNCTION__);
        free(pbuffer);
        free(pbufferL);
        return NULL;
    }

    recordInit(hds->recordDevice, &samplerate);
    while (1)
    {
        memset(pbuffer,  0, 8192 * sizeof(short));
        memset(pbufferL, 0, 4096 * sizeof(short));
        memset(pbufferR, 0, 4096 * sizeof(short));

        int readSize  = readRecordData(pbuffer, 8192 * 2);
        for (int i = 0 ; i < 4096 ; i++)
        {
            pbufferL[i] = pbuffer[2 * i];
            pbufferR[i] = pbuffer[2 * i + 1];
        }

        if (dumpFlag)
        {
            char filenameL[] = "/data/record_dataL.pcm";
            char filenameR[] = "/data/record_dataR.pcm";

            FILE *fpL = fopen(filenameL, "wb+");
            if (fpL != NULL)
            {
                if (fwrite(pbufferL, readSize / 2, 1, fpL) <= 0) {
                    ALOGD("fwrite failed\n");
                }
                if (fclose(fpL)) {
                    ALOGD("fclose failed\n");
                }
            }

            FILE *fpR = fopen(filenameR, "wb+");
            if (fpR != NULL)
            {
                if (fwrite(pbufferR, readSize / 2, 1, fpR) <= 0) {
                    ALOGD("fwrite failed\n");
                }
                if (fclose(fpR)) {
                    ALOGD("fclose failed\n");
                }
            }
        }
        memset(freqDataL, 0, sizeof(freqDataL));
        memset(freqDataR, 0, sizeof(freqDataR));
        memset(magDataL, 0, sizeof(magDataL));
        memset(magDataR, 0, sizeof(magDataR));
		if (hds->chToCheck == ALL_CH && hds->iCurCheckCH == CH_L) {
            ApplyFFT256(samplerate, pbufferL, 0, freqDataL, magDataL);
		} else if (hds->chToCheck == ALL_CH && hds->iCurCheckCH == CH_R) {
            ApplyFFT256(samplerate, pbufferR, 0, freqDataR, magDataR);
		} else if ( hds->chToCheck != ALL_CH ) {
			ApplyFFT256(samplerate, pbufferL, 0, freqDataL, magDataL);
			ApplyFFT256(samplerate, pbufferR, 0, freqDataR, magDataR);
		}
        for (int i = 0; i < 3 ; i ++)
        {
            SLOGD("%d.freqDataL[%d]:%d,magDataL[%d]:%d", i, i, freqDataL[i], i, magDataL[i]);
            SLOGD("%d.freqDataR[%d]:%d,magDataR[%d]:%d", i, i, freqDataR[i], i, magDataR[i]);
        }
	if (chToCheck == ALL_CH )
        {
			ALOGD("chToCheck = ALL_CH");
            ALOGD("[%s] freqOfRingtoneLeftHigh:%d, freqOfRingtoneLeftLow:%d, freqOfRingtoneRightHigh:%d,freqOfRingtoneRightLow:%d", 
                    __func__, freqOfRingtoneLeftHigh, freqOfRingtoneLeftLow, freqOfRingtoneRightHigh, freqOfRingtoneRightLow);
			ALOGD("[%s] highFreq:%d, lowFreq:%d, magUpperL:%d,magLowerL:%d,magUpperR:%d,magLowerR:%d", 
                    __func__, highFreq, lowFreq, magUpperL, magLowerL, magUpperR, magLowerR);
			if (((freqDataL[0] <= (unsigned int)freqOfRingtoneLeftHigh && freqDataL[0] >= (unsigned int)freqOfRingtoneLeftLow) && (magDataL[0] <= (unsigned int)magUpperL && magDataL[0] >= (unsigned int)magLowerL/1000))) 
			{
				checkCnt ++;
				pass_freqL = freqDataL[0];
				pass_amplL = magDataL[0];
			}
			if ((freqDataR[0] <= (unsigned int)freqOfRingtoneRightHigh && freqDataR[0] >= (unsigned int)freqOfRingtoneRightLow) && (magDataR[0] <= (unsigned int)magUpperR && ((magDataR[0] >= magDataL[0]/100) || ( magDataR[0] >= (unsigned int)magLowerR)))) 
			{
				checkCntR ++;
				pass_freqR = freqDataR[0];
				pass_amplR = magDataR[0];
			} 
            ALOGD("[%s] checkCnt[%d], checkCntR[%d], freqDataL:%d,magDataL:%d,freqDataR:%d,magDataR:%d", ouput_dev_name[hds->i4OutputType], checkCnt, checkCntR, freqDataL[0], magDataL[0], freqDataR[0], magDataR[0]);
        }
        else if (chToCheck == L_CH_ONLY && ((freqDataL[0] <= (unsigned int)highFreq && freqDataL[0] >= (unsigned int)lowFreq) && (magDataL[0] <= (unsigned int)magUpperL && magDataL[0] >= (unsigned int)magLowerL)))
        {
            ALOGD("chToCheck = L_CH_ONLY");
            checkCnt ++;
            ALOGD("[%s] checkCnt[%d], freqDataL:%d,magDataL:%d,freqDataR:%d,magDataR:%d", ouput_dev_name[hds->i4OutputType], checkCnt, freqDataL[0], magDataL[0], freqDataR[0], magDataR[0]);
        }
        else if (chToCheck == R_CH_ONLY 
&& ((freqDataR[0] <= (unsigned int)highFreq && freqDataR[0] >= (unsigned int)lowFreq) && (magDataR[0] <= (unsigned int)magUpperR && magDataR[0] >= (unsigned int)magLowerR)))
        {
            ALOGD("chToCheck = R_CH_ONLY");
            checkCnt ++;
            ALOGD("[%s] checkCnt[%d], freqDataL:%d,magDataL:%d,freqDataR:%d,magDataR:%d", ouput_dev_name[hds->i4OutputType], checkCnt, freqDataL[0], magDataL[0], freqDataR[0], magDataR[0]);
        }
        else
        {
            checkCnt = 0;
            ALOGD("[%s] FAIL, checkCnt reset [%d], freqDataL:%d,magDataL:%d,freqDataR:%d,magDataR:%d", ouput_dev_name[hds->i4OutputType], checkCnt, freqDataL[0], magDataL[0], freqDataR[0], magDataR[0]);
        }
		
		if ( chToCheck == ALL_CH )
		{
			if ( checkCnt >= 5 && checkCntR >= 5 ) 
		    {
			    bpass = true;
		    }
		} else if ( checkCnt >= 5 ) {
			bpass = true;
		}

        if (bpass)
        {
            snprintf(hds->info, sizeof(hds->info), "Check freq pass.\n");
            ALOGD(" @ info : %s", hds->info);
            break;
        }

        if (hds->exit_thd)
        {
			ALOGD(" %s : begin to exit!!!", __func__);
            break;
        }
    }

    if (hds->i4OutputType == Output_LPK) //speaker
    {
		return_data.speaker.freqL = freqDataL[0];
        return_data.speaker.freqR = freqDataR[0];
        return_data.speaker.amplL = magDataL[0];
        return_data.speaker.amplR = magDataR[0];
		
	    if ( chToCheck == ALL_CH )
		{
			if ( checkCnt >= 5 ) 
			{
				return_data.speaker.freqL = pass_freqL;
				return_data.speaker.amplL = pass_amplL;   	
			} 
			
			if ( checkCntR >= 5 )
			{
				return_data.speaker.freqR = pass_freqR;
            	return_data.speaker.amplR = pass_amplR;
			}
			/*** add by zkc start ***/
			if(return_data.speaker.freqL > 2000){
				return_data.speaker.freqL = 2000;
				ALOGD(TAG "freqL limit 2000 for PC ATA Tool");
			}
			if(return_data.speaker.freqR > 2000){
				return_data.speaker.freqR = 2000;
				ALOGD(TAG "freqR limit 2000 for PC ATA Tool");
			}
			/*** add by zkc end ***/
		} 
        ALOGD(TAG "ATA Return Data(Speaker): FreqL = %d, FreqR = %d, AmpL = %d, AmpR = %d", return_data.speaker.freqL, return_data.speaker.freqR, return_data.speaker.amplL, return_data.speaker.amplR);
    }
    else if (hds->i4OutputType == Output_HS)
    {
        return_data.receiver.freqL = freqDataL[0];
        return_data.receiver.freqR = freqDataR[0];
        return_data.receiver.amplL = magDataL[0];
        return_data.receiver.amplR = magDataR[0];
        ALOGD(TAG "ATA Return Data(Receiver): FreqL = %d, FreqR = %d, AmpL = %d, AmpR = %d", return_data.receiver.freqL, return_data.receiver.freqR, return_data.receiver.amplL, return_data.receiver.amplR);
    }

    free(pbuffer);
    free(pbufferL);
    free(pbufferR);

    ALOGD(TAG "%s: Stop", __FUNCTION__);
    pthread_exit(NULL); // thread exit
    return NULL;
}

static int getFileNAmeSize(char buffer[])
{
    int length = 0;
    while (buffer[length] != '\0' &&  buffer[length] != '\n' &&  buffer[length] != '\r' && buffer[length] != ';')
    {
        //printf("buffer[%d] = %x \n",length,buffer[length]);
        length++;
    }
    return length;
}

static void Audio_Wave_clear_WavePlayInstance(WavePlayData *pWaveInstance)
{
    if (pWaveInstance->FileName != NULL)
    {
        delete[] pWaveInstance->FileName;
        pWaveInstance->FileName = NULL;
    }
    printf("delete[] WavePlayInstance.FileName; \n");
    if (pWaveInstance->pFile != NULL)
    {
        if (fclose(pWaveInstance->pFile) == 0)
        {
            pWaveInstance->pFile = NULL;
        }
    }
    printf(" fclose(WavePlayInstance.pFile; \n");
    memset((void *)pWaveInstance, 0 , sizeof(WavePlayData) - sizeof(pthread_t));
    printf("memset((void*)&WavePlayInstance, 0 ,sizeof(WavePlayData) - sizeof(pthread_t)) \n");
}

static void *Audio_Wave_Playabck_thread(void *mPtr)
{
    struct mAudio *hds  = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;
    int wave_play_time  = hds->i4Playtime;

    ALOGD(TAG "%s: Start\n", __FUNCTION__);

    // for filelist and now read file
    FILE *pFileList = NULL;
    // buffer fir read filelist and file
    char FileNamebuffer[MAX_FILE_NAME_SIZE];
	char FileNamebufferL[MAX_FILE_NAME_SIZE];
	char FileNamebufferR[MAX_FILE_NAME_SIZE];
	char FileNamebufferTmp[MAX_FILE_NAME_SIZE];
	char CompareNamebuffer[256];
    bool FileListEnd = false;
    bool openfielerror = false;
    WavePlayData WavePlayInstance;
    memset((void *)&WavePlayInstance, 0 , sizeof(WavePlayData));
	
	

    //open input file  list
    pFileList = fopen(FileListNamesdcard, "rb");
    if (pFileList == NULL)
    {
        ALOGE("%s(), error reopen file %s", __FUNCTION__, FileListNamesdcard);
        pFileList = fopen(FileListName, "rb");
        if (pFileList == NULL)
        {
            FileListEnd = true;
            openfielerror = true;
            ALOGE("%s(), error opening file %s", __FUNCTION__, FileListName);
        }
    }
	
	memset((void *)FileNamebuffer, 0, MAX_FILE_NAME_SIZE);
	memset((void *)FileNamebufferL, 0, MAX_FILE_NAME_SIZE);
	memset((void *)FileNamebufferR, 0, MAX_FILE_NAME_SIZE);
	
    while (pFileList && !feof(pFileList))
    {
        int filelength = 0;
        int CompareResult = -1;
		int CompareResult_l = -1;
		int CompareResult_r = -1;
		
		memset((void *)FileNamebufferTmp, 0, MAX_FILE_NAME_SIZE);
		
        if (fgets(FileNamebufferTmp, 100, pFileList) == NULL)
            break;
		
        // crop file name to waveplay data structure
        filelength = getFileNAmeSize(FileNamebufferTmp);
        printf("getFileNAmeSize = %d\n", filelength);
        if (filelength > 0)
        {
			memset(CompareNamebuffer, '\0', sizeof(CompareNamebuffer));
            memcpy((void *)CompareNamebuffer, (void *)FileNamebufferTmp, filelength);
            CompareNamebuffer[filelength] = '\0';
            printf("get file list filename %s\n", FileNamebufferTmp);
            printf("get file list CompareNamebuffer %s\n", CompareNamebuffer);
			if ( hds->chToCheck == ALL_CH ) 
			{
				CompareResult_l = strcmp(CompareNamebuffer, hds->file_name_L);
				CompareResult_r = strcmp(CompareNamebuffer, hds->file_name_R);
			} 
            CompareResult = strcmp(CompareNamebuffer, hds->file_name);
			
            printf("CompareResult = %d \n", CompareResult);
        }

        if (CompareResult == 0)
        {
            if (pFileList != NULL)
            {
                fgets(FileNamebuffer, 100, pFileList);
            }
            ALOGD("CompareResult ==0, audiotone: %s \n", FileNamebuffer);
			if ( hds->chToCheck != ALL_CH )
                break;
        }
		
		if ( hds->chToCheck == ALL_CH ) 
		{
			if (CompareResult_l == 0)
			{
				if (pFileList != NULL)
				{
					fgets(FileNamebufferL, 100, pFileList);
				}
				ALOGD("CompareResult_l ==0, audiotone_L: %s \n", FileNamebufferL);
			}
			
			if (CompareResult_r == 0)
			{
				if (pFileList != NULL)
				{
					fgets(FileNamebufferR, 100, pFileList);
				}
				ALOGD("CompareResult_r ==0, audiotone_R: %s \n", FileNamebufferR);
			}
			
			if ( getFileNAmeSize(FileNamebufferL) > 0 && getFileNAmeSize(FileNamebufferR) > 0 )
			{
				break;
			}
		}
    }

    WavePlayInstance.i4Output = hds->i4OutputType;

    while (1)
    {
        if (openfielerror == true)
        {
            /* preare text view info */
            char *ptr;
            ptr = hds->info;
            ptr += sprintf(ptr, "%s", "error open ini file\n");
        }
		
		ALOGD("%s: wave_play_time %d\n", __func__, wave_play_time);
        // read file list is not null
        while (/*((pFileList && !feof(pFileList) && FileListEnd == false) || (WavePlayInstance.ThreadStart == true)) &&*/hds->exit_thd == false)
        {
            if (wave_play_time < 0 || ( hds->chToCheck == ALL_CH && hds->iCurCheckCH >= CH_MAX ) )
            {
                WavePlayInstance.ThreadExit = true;
                goto WAVE_SLEEP;
            }
			
			if ( WavePlayInstance.ThreadExit ) {
				if ( hds->chToCheck == ALL_CH ) {
					//pthread_mutex_lock(&hds->mCurCheckCHMutex);
					hds->iCurCheckCH ++;
					ALOGD("%s: swith to right chanel\n", __func__);
					//pthread_mutex_unlock(&hds->mCurCheckCHMutex);

					WavePlayInstance.ThreadStart = false;
					//continue;
				}
			}
			
            if (WavePlayInstance.ThreadStart == false)
            {
                // clear all wave data.
                if (WavePlayInstance.ThreadExit == true )
                {
                    ALOGD("WavePlayInstance.ThreadExit = true clean all data\n");
                    Audio_Wave_clear_WavePlayInstance(&WavePlayInstance);
                    WavePlayInstance.WavePlayThread = (pthread_t) NULL;
                }

                // get Filelist FileNamebuffer
/*
                int filelength = 0;
                memset((void *)FileNamebuffer, 0, MAX_FILE_NAME_SIZE);
                if (pFileList != NULL)
                {
                    if (fgets(FileNamebuffer, 100, pFileList) == NULL)
                        break;
                }
                printf("get file list filename %s\n", FileNamebuffer);
                // crop file name to waveplay data structure
                filelength = getFileNAmeSize(FileNamebuffer);
                printf("getFileNAmeSize = %d\n", filelength);
                if (filelength > 0)
                {
                    WavePlayInstance.FileName = new char[filelength + 7 + 1];
                    memset((void *)WavePlayInstance.FileName, '\0', filelength + 7 + 1);
                    memcpy((void *)WavePlayInstance.FileName, "/vendor", 7);  // add prefix "/vendor"
                    memcpy((void *)((char *)WavePlayInstance.FileName + 7), (void *)FileNamebuffer, filelength);
                    WavePlayInstance.FileName[filelength + 7] = '\0';
                    printf("get filename %s\n", WavePlayInstance.FileName);
                }
*/
				int filelength = 0;
				char* pBuffer = NULL;
				if ( hds->chToCheck == ALL_CH )
				{
					ALOGD("%s: iCurCheckCH = %d \n", __func__, hds->iCurCheckCH);
					if ( hds->iCurCheckCH == CH_L ) {
						filelength = getFileNAmeSize(FileNamebufferL);	
						pBuffer = (char *)&FileNamebufferL;
					} else if (hds->iCurCheckCH == CH_R) {
						filelength = getFileNAmeSize(FileNamebufferR);
						pBuffer = (char *)&FileNamebufferR;
					}
				} else {
					filelength = getFileNAmeSize(FileNamebuffer);	
					pBuffer = (char *)&FileNamebuffer;
				}
				ALOGD("%s: filelength %d, pBuffer: %s\n", __func__, filelength, pBuffer);
				WavePlayInstance.FileName = new char[filelength + 7 + 1];

                memset((void *)WavePlayInstance.FileName, '\0', filelength + 7 + 1);
                memcpy((void *)WavePlayInstance.FileName, "/vendor", 7);  // add prefix "/vendor"
				
                memcpy((void *)((char *)WavePlayInstance.FileName + 7), (void *)pBuffer, filelength);
                WavePlayInstance.FileName[filelength + 7] = '\0';
                ALOGD("get filename %s\n", WavePlayInstance.FileName);
					
                // create audio playback rounte
                if (WavePlayInstance.WavePlayThread == (pthread_t) NULL)
                {
                    WavePlayInstance.ThreadStart = true ;
                    WavePlayInstance.ThreadExit = false ;
                    ALOGD("Audio_Wave_playback\n");
                    Audio_Wave_playback((void *)&WavePlayInstance);
                    ALOGD("Audio_Wave_playback thread create\n");
                }
            }
			
WAVE_SLEEP:
            usleep(WAVE_PLAY_SLEEP_TIME * 1000); // sleep 100 ms
            wave_play_time -= WAVE_PLAY_SLEEP_TIME;
        }

        if (hds->exit_thd)
        {
            WavePlayInstance.ThreadExit = true;

            ALOGD("+WavePlayInstance.ThreadStart = %d\n", WavePlayInstance.ThreadStart);
            pthread_join(WavePlayInstance.WavePlayThread, NULL);
            ALOGD("-WavePlayInstance.ThreadStart = %d\n", WavePlayInstance.ThreadStart);

            // clear all wave data.
            if (WavePlayInstance.ThreadExit == true)
            {
                ALOGD("WavePlayInstance.ThreadExit = true clean all data\n");
                Audio_Wave_clear_WavePlayInstance(&WavePlayInstance);
            }
            break;
        }
        usleep(WAVE_PLAY_SLEEP_TIME * 1000); // sleep 100 ms
        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }

    ALOGD(TAG "%s: Audio_Wave_Playabck_thread Exit \n", __FUNCTION__);
    pthread_exit(NULL); // thread exit
    return NULL;
}

static bool audio_loopback_typemic_speaker_test(int micType, void *priv)
{
    bool micPass = false;
    struct mAudio *mc = (struct mAudio *)priv;

    // init Audio
    Common_Audio_init();

    memset(mc->file_name, 0, MAX_FILE_NAME_SIZE);
	memset(mc->file_name_L, 0, MAX_FILE_NAME_SIZE);
	memset(mc->file_name_R, 0, MAX_FILE_NAME_SIZE);
    strncpy(mc->file_name, FileStartString_LoudspkPlayback, sizeof(FileStartString_LoudspkPlayback)-1);
	strncpy(mc->file_name_L, FileStartString_LoudspkPlayback_L, sizeof(FileStartString_LoudspkPlayback_L)-1);
	strncpy(mc->file_name_R, FileStartString_LoudspkPlayback_R, sizeof(FileStartString_LoudspkPlayback_R)-1);
    mc->i4OutputType = Output_LPK;
	mc->chToCheck = read_preferred_ch_to_check(Output_LPK);
    mc->phoneMicType = micType;
    mc->i4Playtime = read_preferred_ringtone_time() * 1000; //ms
	
	mc->iCurCheckCH = CH_MAX;
	if ( mc->chToCheck == ALL_CH ) 
	{
	    mc->iCurCheckCH = CH_L;
	}
	mc->bswitchCH = 0;
    int    play_time = mc->i4Playtime;

    mc->exit_thd = false;
    memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
    //mc->recordDevice = BUILTIN_MIC;   //Yi-Lung: Record device is selected in function init.
    SpecificBuildInMicTest(micType);
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
    pthread_create(&mc->hRecordThread, NULL, Audio_Record_thread, priv);

    for (int i = 0; i < 100 ; i ++)
    {
        if (strstr(mc->info, "Check freq pass"))
        {
            micPass = true;
            ALOGD("Check freq pass");
            break;
        }
        usleep(play_time * 10);
    }
    mc->exit_thd = true;
    pthread_join(mc->hRecordThread, NULL);
    pthread_join(mc->hHeadsetThread, NULL);
    Common_Audio_deinit();

    SpecificBuildInMicTest(SELECT_MIC_DEFAULT); // reset SpecificBuildInMic to default
    return micPass;
}

//---------- Freq. Response Test ------------
int mAudio_receiver_freqresponse_entry(struct ftm_param *param, void *priv)
{
    struct mAudio*mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "--------mAudio_receiver_freqresponse_entry-----------------------\n" );
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, "", COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    memset(return_data.rcv_response.freqresponse, 0, sizeof(ftm_ata_freq_response));

    for(FreqLoop = 0; FreqLoop < 5; FreqLoop++)
    {
        // init ATA return data
        //return_data.rcv_response.freqresponse[FreqLoop]=0;

        // init Audio
        Common_Audio_init();
        mc->exit_thd = false;

        // ui start
        if (!mc->iv) {
            iv = ui_new_itemview();
            if (!iv) {
                ALOGD(TAG "No memory");
                return -1;
            }
            mc->iv = iv;
        }

        iv = mc->iv;
        iv->set_title(iv, &mc->title);
        iv->set_items(iv, audio_items_auto, 0);
        iv->set_text(iv, &mc->text);
        iv->start_menu(iv,0);
        iv->redraw(iv);

        memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
        memset(mc->file_name,0,MAX_FILE_NAME_SIZE);
        strncpy(mc->file_name, FileStartString_Receiver_FreqResponse[FreqLoop], sizeof(FileStartString_Receiver_FreqResponse[FreqLoop])-1 );
        mc->i4OutputType = Output_HS;
        mc->i4Playtime = read_preferred_ringtone_time()*1000 /5 ;//ms
        pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
        pthread_create(&mc->hRecordThread, NULL, Audio_Response_Record_thread, priv);

        int play_time = mc->i4Playtime*5;
        FreqTestResult[FreqLoop] = FTM_TEST_FAIL;

        for(int i = 0; i < 20 ; i ++)
        {
          //ALOGD("check mc info:%d",i);
          //if (strstr(mc->info, "Check freq pass"))
          //{
          //    FreqTestResult[FreqLoop] = FTM_TEST_PASS;
          //    ALOGD("Check freq pass");
          //    break;
          //}
            usleep(play_time * 10); //50ms
        }

        //if(FreqTestResult[FreqLoop] == FTM_TEST_FAIL)
        //   ALOGD("Check freq fail");

        //if(FreqTestResult[FreqLoop] == FTM_TEST_PASS)
        //    usleep(2000000);

        mc->exit_thd = true;
        pthread_join(mc->hRecordThread, NULL);
        pthread_join(mc->hHeadsetThread, NULL);

        if (strstr(mc->info, "Check freq pass"))
        {
            FreqTestResult[FreqLoop] = FTM_TEST_PASS;
            ALOGD("Check freq pass");
        }

        if(FreqTestResult[FreqLoop] == FTM_TEST_FAIL)
           ALOGD("Check freq fail");

        //if(FreqTestResult[FreqLoop] == FTM_TEST_PASS)
        //    usleep(50000);

    Common_Audio_deinit();
    }

    //check result
    int failCnt = 0;
    for(int i=0;i<5;i++)
    {
        if(FreqTestResult[i] == FTM_TEST_FAIL)
        {
            failCnt++;
            ALOGD("[Receiver] Check freq response fail at %d Hz.", Receiver_Frequency[i]);
        }
    }
    if(!failCnt)
        mc->mod->test_result = FTM_TEST_PASS;
    else
        mc->mod->test_result = FTM_TEST_FAIL;

    LOGD(TAG "%s: End\n", __FUNCTION__);
    return 0;
}

int mAudio_speaker_freqresponse_entry(struct ftm_param *param, void *priv)
{
    struct mAudio*mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "--------------mAudio_speaker_freqresponse_entry----------------\n" );
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, "", COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    memset(return_data.spk_response.freqresponse, 0, sizeof(ftm_ata_freq_response));

    for(FreqLoop = 0; FreqLoop < 5; FreqLoop++)
    {
        // init ATA return data
        //return_data.spk_response.freqresponse[FreqLoop]=0;

        // init Audio
        Common_Audio_init();
        mc->exit_thd = false;

        if (!mc->iv) {
            iv = ui_new_itemview();
            if (!iv) {
                ALOGD(TAG "No memory");
                return -1;
            }
            mc->iv = iv;
        }
        iv = mc->iv;
        iv->set_title(iv, &mc->title);
        iv->set_items(iv, audio_items_auto, 0);
        iv->set_text(iv, &mc->text);
        iv->start_menu(iv,0);
        iv->redraw(iv);

        memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
        memset(mc->file_name,0,MAX_FILE_NAME_SIZE);
        strncpy(mc->file_name, FileStartString_Speaker_FreqResponse[FreqLoop], sizeof(FileStartString_Speaker_FreqResponse[FreqLoop]) - 1);
        mc->i4OutputType = Output_LPK;
        mc->i4Playtime = 2*read_preferred_ringtone_time()*1000/5;//ms
        pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
        pthread_create(&mc->hRecordThread, NULL, Audio_Response_Record_thread, priv);

        int play_time = mc->i4Playtime*5/2;
        FreqTestResult[FreqLoop] = FTM_TEST_FAIL;

        for(int i = 0; i < 40; i ++)
        {
          //if (strstr(mc->info, "Check freq pass"))
          //{
          //    FreqTestResult[FreqLoop] = FTM_TEST_PASS;
          //    ALOGD("Check freq pass");
          //    break;
          //}
          usleep(play_time * 10); //100ms
        }

        //if(FreqTestResult[FreqLoop] == FTM_TEST_FAIL)
        //   ALOGD("Check freq fail");
        //if(FreqTestResult[FreqLoop] == FTM_TEST_PASS)
        //    usleep(2000000);

        mc->exit_thd = true;
        pthread_join(mc->hRecordThread, NULL);
        pthread_join(mc->hHeadsetThread, NULL);

        if (strstr(mc->info, "Check freq pass"))
        {
            FreqTestResult[FreqLoop] = FTM_TEST_PASS;
            ALOGD("Check freq pass");
        }

        if(FreqTestResult[FreqLoop] == FTM_TEST_FAIL)
           ALOGD("Check freq fail");

        Common_Audio_deinit();
    }

    //check result
    int failCnt = 0;
    for(int i=0;i<5;i++)
    {
        if(FreqTestResult[i] == FTM_TEST_FAIL)
        {
            failCnt++;
            ALOGD("[Speaker] Check freq response fail at %d Hz.", Receiver_Frequency[i]);
        }
    }
    if(!failCnt)
        mc->mod->test_result = FTM_TEST_PASS;
    else
        mc->mod->test_result = FTM_TEST_FAIL;

    LOGD(TAG "%s: End\n", __FUNCTION__);
    return 0;
}

//---------- THD. Performance Test ------------
int mAudio_receiver_thd_entry(struct ftm_param *param, void *priv)
{
    struct mAudio*mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "--------------mAudio_receiver_thd_entry----------------\n" );
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, "", COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    mc->exit_thd = false;

    if (!mc->iv) {
        iv = ui_new_itemview();
        if (!iv) {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items_auto, 0);
    iv->set_text(iv, &mc->text);
	iv->start_menu(iv,0);
    iv->redraw(iv);

    memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
    memset(mc->file_name,0,MAX_FILE_NAME_SIZE);
    strncpy(mc->file_name, FileStartString_LoudspkPlayback, sizeof(FileStartString_LoudspkPlayback) - 1);
    mc->i4OutputType = Output_HS;
	mc->i4Playtime = 2*read_preferred_ringtone_time()*1000/5;//ms
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
    pthread_create(&mc->hRecordThread, NULL, Audio_THD_Record_thread, priv);

    int play_time = mc->i4Playtime*5/2;
    mc->mod->test_result = FTM_TEST_FAIL;

    for(int i = 0; i < 40 ; i ++)
    {
      //if (strstr(mc->info, "Check freq pass"))
      //{
      //    mc->mod->test_result = FTM_TEST_PASS;
      //    ALOGD("Check freq pass");
      //    break;
      //}
      usleep(play_time * 10); //50ms
    }

    //if(mc->mod->test_result == FTM_TEST_FAIL)
    //   ALOGD("Check freq fail");
    //if(mc->mod->test_result == FTM_TEST_PASS)
    //    usleep(2000000);

    mc->exit_thd = true;
    pthread_join(mc->hRecordThread, NULL);
    pthread_join(mc->hHeadsetThread, NULL);

    if (strstr(mc->info, "Check THD pass"))
    {
        mc->mod->test_result = FTM_TEST_PASS;
        ALOGD("Check THD pass");
    }

    Common_Audio_deinit();

    LOGD(TAG "%s: End\n", __FUNCTION__);

    return 0;
}

int mAudio_speaker_thd_entry(struct ftm_param *param, void *priv)
{
    struct mAudio*mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "--------------mAudio_speaker_thd_entry----------------\n" );
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, "", COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    mc->exit_thd = false;

    if (!mc->iv) {
        iv = ui_new_itemview();
        if (!iv) {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items_auto, 0);
    iv->set_text(iv, &mc->text);
	iv->start_menu(iv,0);
    iv->redraw(iv);

    memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
    memset(mc->file_name,0,MAX_FILE_NAME_SIZE);
    strncpy(mc->file_name, FileStartString_LoudspkPlayback, sizeof(FileStartString_LoudspkPlayback) - 1);
    mc->i4OutputType = Output_LPK;
	mc->i4Playtime = 2*read_preferred_ringtone_time()*1000/5;//ms
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
    pthread_create(&mc->hRecordThread, NULL, Audio_THD_Record_thread, priv);

    int play_time = mc->i4Playtime*5/2;
    mc->mod->test_result = FTM_TEST_FAIL;

    for(int i = 0; i < 40 ; i ++)
    {
      //if (strstr(mc->info, "Check freq pass"))
      //{
      //    mc->mod->test_result = FTM_TEST_PASS;
      //    ALOGD("Check freq pass");
      //    break;
      //}
      usleep(play_time * 10); //50ms
    }

    //if(mc->mod->test_result == FTM_TEST_FAIL)
    //   ALOGD("Check freq fail");
    //if(mc->mod->test_result == FTM_TEST_PASS)
    //    usleep(2000000);

    mc->exit_thd = true;
    pthread_join(mc->hRecordThread, NULL);
    pthread_join(mc->hHeadsetThread, NULL);

    if (strstr(mc->info, "Check THD pass"))
    {
        mc->mod->test_result = FTM_TEST_PASS;
        ALOGD("Check THD pass");
    }

    Common_Audio_deinit();

    LOGD(TAG "%s: End\n", __FUNCTION__);

    return 0;
}


//Loopback phonemic and speaker test
int mAudio_loopback_phonemicspk_auto(struct ftm_param *param, void *priv)
{
    char *ptr;
    struct mAudio *mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "--------------mAudio_entry----------------\n");
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, mc->info, COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items_auto, 0);
    iv->set_text(iv, &mc->text);
    iv->start_menu(iv, 0);
    iv->redraw(iv);

    bool micPass[SELECT_MIC_TYPE_MAX] = {false, false, false, false, false, false};
    bool micSupported[SELECT_MIC_TYPE_MAX] = {false, false, false, false, false, false};
    int micType[SELECT_MIC_TYPE_MAX] = {SELECT_MIC_DEFAULT, SELECT_MIC1, SELECT_MIC2, SELECT_MIC3, SELECT_MIC4, SELECT_MIC5};
    const char *micName[SELECT_MIC_TYPE_MAX] = {"default mic", "main mic", "ref mic", "3rd mic", "4th mic", "5th mic"};
    bool testPass = true;

	pthread_mutex_init(&mc->mCurCheckCHMutex, NULL);
    if (ftm_get_prop("Audio.Mic.Sets") != NULL) //new phonemic(with diffrent mic) speaker loopback
    {
        for (int i = SELECT_MIC1; i < SELECT_MIC_TYPE_MAX; i++)
        {
            micSupported[i] = read_loopback_mic_inputs(micType[i]);
            if (micSupported[i] == true) {
                micPass[i] = audio_loopback_typemic_speaker_test(micType[i], priv);

                ptr = mc->info;
                ptr += sprintf(ptr, "loopback ( %s  - speaker) Test %s\n", micName[i], micPass[i] ? "PASS" : "FAIL");
                iv->redraw(iv);
				//tom add
				if( ! micPass[i]){
					ALOGD("ftm micPass[%d] = %d \n",i, micPass[i]);
					break;
				}
				//end					
            }
        }

        ptr = mc->info;
        for (int i = SELECT_MIC1; i < SELECT_MIC_TYPE_MAX; i++) {// bypass not supported micPass, and check the result
            if (micSupported[i]) {
                testPass = testPass && micPass[i];
                ALOGD("micPass[%d] = [%d]\n", i, micPass[i]);
                ptr += sprintf(ptr, "loopback ( %s  - speaker) Test %s\n", micName[i], micPass[i] ? "PASS" : "FAIL");
            }
        }
    }
    else // old phonemic(with dual mic) speaker loopback
    {
        testPass = audio_loopback_typemic_speaker_test(micType[SELECT_MIC_DEFAULT], priv);
        micPass[SELECT_MIC_DEFAULT] = testPass;
        ptr = mc->info;
        ptr += sprintf(ptr, "loopback ( %s  - speaker) Test %s\n", micName[SELECT_MIC_DEFAULT],
                       micPass[SELECT_MIC_DEFAULT] ? "PASS" : "FAIL");
    }

    if (testPass) {
        mc->mod->test_result = FTM_TEST_PASS;
        ALOGD("loopback_phonemicspk PASS\n");
    } else {
        mc->mod->test_result = FTM_TEST_FAIL;
        ALOGD("loopback_phonemicspk FAIL\n");
    }

    iv->redraw(iv);
    usleep(2000000);
    memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
	pthread_mutex_unlock(&mc->mCurCheckCHMutex);
	pthread_mutex_destroy(&mc->mCurCheckCHMutex);
    LOGD(TAG "%s: End\n", __FUNCTION__);
    return 0;
}

int mAudio_loopback_phonemicspk_manual(struct ftm_param *param, void *priv)
{
	ALOGD("Select mAudio_loopback_phonemicspk_manual");
    int chosen;
    bool exit = false;
    char *inputType = NULL;
    struct mAudio *mc = (struct mAudio *)priv;
    mc->exit_thd = false;
    mc->Headset_change = false;
    mc->avail = false;

    struct itemview *iv;

    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    g_loopback_item = 1;  // default: use MIC1 loopback
    g_prev_mic_state = 1;

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
	text_t cbtn ;
	text_t rbtn ;
	init_text(&lbtn, "fail", COLOR_YELLOW);
	init_text(&cbtn, "back", COLOR_YELLOW);
	init_text(&rbtn, "pass", COLOR_YELLOW);
	iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    const char *numMicString = appGetFeatureOptionValue(MTK_AUDIO_NUMBER_OF_MIC);
    if (get_is_ata() == 0) {
        if (numMicString && atoi(numMicString) >= 2) {
            iv->set_items(iv, audio_items_loopback_dual_mic, 0);
        } else {
            iv->set_items(iv, audio_items_loopback, 0);
        }
    } else {
        if (numMicString && atoi(numMicString) >= 2) {
            iv->set_items(iv, audio_items_loopback_ex_dual_mic, 0);
        } else {
            iv->set_items(iv, audio_items_loopback_ex, 0);
        }
    }
    iv->set_text(iv, &mc->text);

    inputType = ftm_get_prop("Audio.Manual.InputType");
    if (inputType != NULL && (atoi(inputType) == 0 || atoi(inputType) == 1 || atoi(inputType) == 2 || atoi(inputType) == 3))
    {
        //@ input != NULL, PC command control mode
        ALOGD("Audio.Manual.InputType = %s", inputType);

        iv->redraw(iv);

        PhoneMic_SpkLR_Loopback(MIC1_OFF);
        usleep(20000);
        if (atoi(inputType) == 0)
        {
            ALOGD("Audio Deinit");
            Common_Audio_deinit();
        }
        else if (atoi(inputType) == 1)
        {
            ALOGD("Set Mic1 on");
            PhoneMic_SpkLR_Loopback(MIC1_ON);
        }
        else if (atoi(inputType) == 2)
        {
            ALOGD("Set Mic2 on");
            PhoneMic_SpkLR_Loopback(MIC2_ON);
        }
        else if (atoi(inputType) == 3)
        {
            ALOGD("Set headset Mic on");
            HeadsetMic_SpkLR_Loopback (1);
        }

        mc->exit_thd = true;
        mc->mod->test_result = FTM_TEST_PASS;

    }
    else
    {
        // Original manual operating mode
        PhoneMic_SpkLR_Loopback(MIC1_ON);

        do {
            chosen = iv->run(iv, &exit);
            switch (chosen) {
            case ITEM_MIC1:
                ALOGD("Select Mic1 loopback");
                g_loopback_item = 1;  // use MIC1 loopback
                if(g_prev_mic_state != g_loopback_item){
                   PhoneMic_SpkLR_Loopback(MIC2_OFF);
                }
                else{
                   break;
                }
                PhoneMic_SpkLR_Loopback(MIC1_ON);
                g_prev_mic_state = 1;
                break;
            case ITEM_MIC2:
                ALOGD("Select Mic2 loopback");
                g_loopback_item = 2;  // use MIC2 loopback
                if(g_prev_mic_state != g_loopback_item){
                   PhoneMic_SpkLR_Loopback(MIC1_OFF);
                }
                else{
                   break;
                }
                PhoneMic_SpkLR_Loopback(MIC2_ON);
                g_prev_mic_state = 2;
                break;
            case ITEM_HEADSET_MIC:
                ALOGD("Select headset mic loopback");
                g_loopback_item = 3;  // use Headset mic loopback
                if(g_prev_mic_state != g_loopback_item){
                   PhoneMic_SpkLR_Loopback(MIC1_OFF);
                }
                else{
                   break;
                }
                HeadsetMic_SpkLR_Loopback (1);
                g_prev_mic_state = 3;
                break;
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    mc->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    mc->mod->test_result = FTM_TEST_FAIL;
                }

                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
			case L_BTN_DOWN:
				mc->mod->test_result = FTM_TEST_FAIL;
				exit = true;
				break;
			case C_BTN_DOWN:
				exit = true;
				break;
			case R_BTN_DOWN:
				mc->mod->test_result = FTM_TEST_PASS;
				exit = true;
				break;
#endif
            }

            if (exit) {
                ALOGD("Audio_PhoneMic_Speaker_loopback_entry exit_thd = true");
                mc->exit_thd = true;
                break;
            }
        } while (1);


       ALOGD("disable Audio_PhoneMic_Speaker_loopback_entry:%d ",g_loopback_item);
       if(g_loopback_item==1){
          PhoneMic_SpkLR_Loopback(MIC1_OFF);    // disable MIC1 loopback
       }
       else if(g_loopback_item ==2){
          PhoneMic_SpkLR_Loopback(MIC2_OFF);    // disable MIC2 loopback
       }
       else if(g_loopback_item == 3) {
          HeadsetMic_SpkLR_Loopback(0);
       }

       Common_Audio_deinit();
    }

    g_loopback_item = 0;
    g_prev_mic_state = 0;

    return 0;
}
int mAudio_loopback_phonemicspk_entry(struct ftm_param *param, void *priv)
{
    struct mAudio *mc = (struct mAudio *)priv;
    char *outputType = NULL;

	if(FTM_AUTO_ITEM == param->test_type)
    {
        outputType = ftm_get_prop("Audio.Auto.OutputType");
        ALOGD("Audio.Auto.OutputType = %s\n", outputType);

        if (outputType == NULL)
        {
            // @ default PCBA level, phone speaker -> phone mic
            mc->recordDevice = BUILTIN_MIC;
            mAudio_loopback_phonemicspk_auto(param, priv);
        }
        else
        {
            if (atoi(outputType) == 0) // @ PCBA level, phone speaker -> phone mic
            {
                mc->recordDevice = BUILTIN_MIC;
                mAudio_loopback_phonemicspk_auto(param, priv);
            }
            else if (atoi(outputType) == 1) // @ Phone level, phone speaker -> headset mic loopback
            { 
                mc->recordDevice = WIRED_HEADSET;
                mAudio_loopback_phonemicspk_auto(param, priv);
            }
            else if (atoi(outputType) == 2) // @ Phone level, freq response
            {
                mc->recordDevice = WIRED_HEADSET;
                mAudio_speaker_freqresponse_entry (param, priv);
            }
            else if (atoi(outputType) == 3) // @ Phone level, THD
            {
                mc->recordDevice = WIRED_HEADSET;
                mAudio_speaker_thd_entry (param, priv);
            }
        }

	}
    else if(FTM_MANUAL_ITEM == param->test_type)
	{
		mAudio_loopback_phonemicspk_manual(param, priv);
	}

    return 0;
}

#if defined(FEATURE_FTM_SPEAKER_CALIBRATION)
static void *Calibration_Record_thread(void *mPtr)
{
    struct mAudio *hds  = (struct mAudio *)mPtr;
    ALOGD(TAG "%s: Start", __FUNCTION__);
    uint32_t samplerate = 0;
    short* pbuffer;
    int size = 0;

    pbuffer = (short *)malloc(8192 * sizeof(short));
    if (pbuffer == NULL) {
        ALOGD(TAG "%s: pbuffer allocate fail !!", __FUNCTION__);
        return NULL;
    }

    recordInit(BUILTIN_MIC, &samplerate);
    while (1)
    {
        size = readRecordData(pbuffer, 8192 * 2);
        if (size > 0) {
            streamInOpenDone = true;
        }

        if (hds->exit_thd || wavePlaybackExit)
        {
            break;
        }
    }

    free(pbuffer);
    Common_Audio_deinit();

    ALOGD(TAG "%s: Stop", __FUNCTION__);
    pthread_exit(NULL); // thread exit
    return NULL;
}
#endif

#if defined(FEATURE_FTM_SPEAKER_CALIBRATION)
int mAudio_speaker_Rdc_calibration_entry(struct ftm_param *param, void *priv)
{
    int ret = 0;
    struct mAudio *mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "--------mAudio_speaker_Rdc_calibration_entry-----------------------\n" );
    ALOGD(TAG "%s()", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, mc->info, COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    mc->i4OutputType = Output_LPK;
    mc->i4Playtime = 2000; //ms
    mc->exit_thd = false;
    wavePlaybackExit = false;
    streamInOpenDone = 0;

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }

    memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
    iv = mc->iv;
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items_auto, 0);
    iv->set_text(iv, &mc->text);
    iv->start_menu(iv, 0);
    iv->redraw(iv);
    mc->mod->test_result = FTM_TEST_FAIL;

    memset(mc->file_name, 0, MAX_FILE_NAME_SIZE);
    strncpy(mc->file_name, FileStartString_CalibrationPlayback,
            sizeof(FileStartString_CalibrationPlayback) - 1);
    SpeakerCalibration(SPK_CALIB_STAGE_INIT);

    pthread_create(&mc->hRecordThread, NULL, Calibration_Record_thread, priv);
    while(!streamInOpenDone) {
        usleep(50000);
    }

    pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
    while(!wavePlaybackExit) {
        usleep(200000);
    }

    mc->exit_thd = true;

    pthread_join(mc->hHeadsetThread, NULL);
    pthread_join(mc->hRecordThread, NULL);

    ret = SpeakerCalibration(SPK_CALIB_STAGE_CALCULATE_AND_SAVE);

    SpeakerCalibration(SPK_CALIB_STAGE_DEINIT);

    if (ret == 1)
    {
        snprintf(mc->info + strlen(mc->info), sizeof(mc->info) - strlen(mc->info), "Check pass\n");
        mc->mod->test_result = FTM_TEST_PASS;
        ALOGD("%s() Check pass", __FUNCTION__);
    } else {
        snprintf(mc->info + strlen(mc->info), sizeof(mc->info) - strlen(mc->info), "Check fail\n");
        mc->mod->test_result = FTM_TEST_FAIL;
        ALOGD("%s() Check fail", __FUNCTION__);
    }
    iv->redraw(iv);

    LOGD(TAG "%s(): End\n", __FUNCTION__);

    return 0;
}
#endif

//#ifdef RECEIVER_HEADSET_AUTOTEST
//Receiver test
int mAudio_receiver_auto_entry(struct ftm_param *param, void *priv)
{
    struct mAudio *mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "--------mAudio_receiver_entry-----------------------\n");
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, "", COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    mc->exit_thd = false;

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }

    iv = mc->iv;
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items_auto, 0);
    iv->set_text(iv, &mc->text);
    iv->start_menu(iv, 0);
    iv->redraw(iv);

    memset(mc->info, 0, sizeof(mc->info) / sizeof(*(mc->info)));
    memset(mc->file_name, 0, MAX_FILE_NAME_SIZE);
    strncpy(mc->file_name, FileStartString_LoudspkPlayback, sizeof(FileStartString_LoudspkPlayback) - 1);
    mc->i4OutputType = Output_HS;
    mc->phoneMicType= SELECT_MIC_DEFAULT;
    mc->i4Playtime = read_preferred_ringtone_time() * 1000; //ms
    //mc->recordDevice = BUILTIN_MIC;   //Yi-Lung: Record device is selected in function init.
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
    pthread_create(&mc->hRecordThread, NULL, Audio_Record_thread, priv);

    int    play_time = mc->i4Playtime;
    mc->mod->test_result = FTM_TEST_FAIL;

    for (int i = 0; i < 200 ; i ++)
    {
        //ALOGD("check mc info:%d",i);
        if (strstr(mc->info, "Check freq pass"))
        {
            mc->mod->test_result = FTM_TEST_PASS;
            ALOGD("Check freq pass");
            break;
        }
        usleep(play_time * 5);
    }

    if (mc->mod->test_result == FTM_TEST_FAIL)
    {
        ALOGD("Check freq fail");
    }

    if (mc->mod->test_result == FTM_TEST_PASS)
    {
        usleep(2000000);
    }

    mc->exit_thd = true;
    pthread_join(mc->hRecordThread, NULL);
    pthread_join(mc->hHeadsetThread, NULL);
    Common_Audio_deinit();

    LOGD(TAG "%s: End\n", __FUNCTION__);
    return 0;
}
//#else
static void mAudio_receiver_playtone_manual(struct mAudio *mc, void *priv)
{
#if defined(WAVE_PLAYBACK)
    memset(mc->file_name, 0, MAX_FILE_NAME_SIZE);
    strncpy(mc->file_name, FileStartString_ReceiverPlayback, sizeof(FileStartString_ReceiverPlayback) - 1);
    mc->i4OutputType = Output_HS;
    mc->i4Playtime = read_preferred_receiver_time() * 1000;
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);
#else
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Receiver_Playabck_thread, priv);
#endif
}

static void mAudio_receiver_stoptone_manual(struct mAudio *mc)
{
    pthread_join(mc->hHeadsetThread, NULL);
}

//Receiver test
int mAudio_receiver_manual_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    bool exit = false;
    char *inputType = NULL;
    struct mAudio *mc = (struct mAudio *)priv;
    struct itemview *iv;
    int privChosen = -1;

    ALOGD(TAG "--------mAudio_receiver_entry-----------------------\n");
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, "", COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    mc->exit_thd = false;

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
    text_t cbtn ;
    text_t rbtn ;
    init_text(&lbtn, "fail", COLOR_YELLOW);
    init_text(&cbtn, "back", COLOR_YELLOW);
    init_text(&rbtn, "pass", COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    const char *numMicString = appGetFeatureOptionValue(MTK_AUDIO_NUMBER_OF_MIC);
    if (get_is_ata() == 0) {
        if (numMicString && atoi(numMicString) >= 2) {
            iv->set_items(iv, receiver_items_dual_mic, 0);
        } else {
            iv->set_items(iv, receiver_items, 0);
        }
    } else {
        if (numMicString && atoi(numMicString) >= 2) {
            iv->set_items(iv, receiver_items_ex_dual_mic, 0);
        } else {
            iv->set_items(iv, receiver_items_ex, 0);
        }
    }
    iv->set_text(iv, &mc->text);

    inputType = ftm_get_prop("Audio.Manual.InputType");
    if (inputType != NULL && (atoi(inputType) == 0 || atoi(inputType) == 1 || atoi(inputType) == 2 || atoi(inputType) == 3))
    {
        //@ input != NULL, PC command control mode
        ALOGD("Audio.Manual.InputType = %s", inputType);

        iv->redraw(iv);

        PhoneMic_Receiver_Loopback(MIC1_OFF);
        usleep(20000);
        if (atoi(inputType) == 0)
        {
            ALOGD("Audio Deinit");
            Common_Audio_deinit();
        }
        else if (atoi(inputType) == 1)
        {
            ALOGD("Set Mic1 on");
            PhoneMic_Receiver_Loopback(MIC1_ON);
        }
        else if (atoi(inputType) == 2)
        {
            ALOGD("Set Mic2 on");
            PhoneMic_Receiver_Loopback(MIC2_ON);
        }
        else if (atoi(inputType) == 3)
        {
            ALOGD("Set headset Mic on");
            HeadsetMic_Receiver_Loopback (1, 1);
        }

        mc->exit_thd = true;
        mc->mod->test_result = FTM_TEST_PASS;

    }
    else
    {
        // Original manual operating mode
        do {
            chosen = iv->run(iv, &exit);
            switch (chosen) {
            case ITEM_RINGTONE:
                if(privChosen == ITEM_RINGTONE)
                    break;
                else if(privChosen == ITEM_MIC1)
                {
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_OFF);  // disable Receiver MIC1 loopback
                }
                else if(privChosen == ITEM_MIC2)
                {
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_OFF);  // disable Receiver MIC2 loopback
                }
                else if (privChosen == ITEM_HEADSET_MIC)
                {
                    usleep(3000);
                    HeadsetMic_Receiver_Loopback (0, 1);
                }
                mAudio_receiver_playtone_manual(mc,priv);
                privChosen = ITEM_RINGTONE;
                exit = false;
                break;
            case ITEM_MIC1:
                if(privChosen == ITEM_MIC1)
                    break;
                else if(privChosen == ITEM_MIC2)
                {
                	usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_OFF);  // disable Receiver MIC2 loopback
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_ON);  // enable Receiver MIC1 loopback
                }
                else if (privChosen == ITEM_HEADSET_MIC)
                {
                    usleep(3000);
                    HeadsetMic_Receiver_Loopback (0, 1);
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_ON);  // enable Receiver MIC1 loopback
                }
                else if(privChosen == ITEM_RINGTONE)
                {
                    mc->exit_thd = true;
                    mAudio_receiver_stoptone_manual(mc);
                    mc->exit_thd = false;
                }
                usleep(3000);
                PhoneMic_Receiver_Loopback(MIC1_ON);  // enable Receiver MIC1 loopback
                privChosen = ITEM_MIC1;
                exit = false;
                break;
            case ITEM_MIC2:
                if(privChosen == ITEM_MIC2)
                    break;
                else if(privChosen == ITEM_MIC1)
                {
                	  usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_OFF);  // disable Receiver MIC1 loopback
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_ON);  // enable Receiver MIC2 loopback
                }
                else if(privChosen == ITEM_HEADSET_MIC)
                {
                	usleep(3000);
                    HeadsetMic_Receiver_Loopback (0, 1);  // disable Receiver headset mic loopback
                    usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_ON);  // enable Receiver MIC2 loopback
                }
                else if(privChosen == ITEM_RINGTONE)
                {
                    mc->exit_thd = true;
                    mAudio_receiver_stoptone_manual(mc);
                    mc->exit_thd = false;
                }
                usleep(3000);
                PhoneMic_Receiver_Loopback(MIC2_ON);  // enable Receiver MIC2 loopback
                privChosen = ITEM_MIC2;
                exit = false;
                break;
            case ITEM_HEADSET_MIC:
                if(privChosen == ITEM_HEADSET_MIC)
                    break;
                else if(privChosen == ITEM_MIC1)
                {
                	usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC1_OFF);  // disable Receiver MIC1 loopback
                    usleep(3000);
                    HeadsetMic_Receiver_Loopback (1, 1);
                }
                else if(privChosen == ITEM_MIC2)
                {
                	usleep(3000);
                    PhoneMic_Receiver_Loopback(MIC2_OFF);  // disable Receiver MIC2 loopback
                    usleep(3000);
                    HeadsetMic_Receiver_Loopback (1, 1);
                }
                else if(privChosen == ITEM_RINGTONE)
                {
                    mc->exit_thd = true;
                    mAudio_receiver_stoptone_manual(mc);
                    mc->exit_thd = false;
                }
                usleep(3000);
                HeadsetMic_Receiver_Loopback (1, 1);
                privChosen = ITEM_HEADSET_MIC;
                exit = false;
                break;
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    mc->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    mc->mod->test_result = FTM_TEST_FAIL;
                }
                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
                case L_BTN_DOWN:
                	mc->mod->test_result = FTM_TEST_FAIL;
                	exit = true;
                	break;
                case C_BTN_DOWN:
                	exit = true;
                	break;
                case R_BTN_DOWN:
                	mc->mod->test_result = FTM_TEST_PASS;
                	exit = true;
                	break;
#endif
            }
            if (exit) {
                ALOGD("mAudio_receiver_entry set exit_thd = true\n");
                mc->exit_thd = true;
                break;
            }
        } while (1);

        if(privChosen == ITEM_RINGTONE)
           mAudio_receiver_stoptone_manual(mc);
        else if(privChosen == ITEM_MIC1)
        {
           usleep(3000);
           PhoneMic_Receiver_Loopback(MIC1_OFF);  // disable Receiver MIC1 loopback
        }
        else if(privChosen == ITEM_MIC2)
        {
           usleep(3000);
           PhoneMic_Receiver_Loopback(MIC2_OFF);  // disable Receiver MIC2 loopback
        }
        else if (privChosen == ITEM_HEADSET_MIC)
        {
           usleep (3000);
           HeadsetMic_Receiver_Loopback (0, 1);
        }

        Common_Audio_deinit();
    }

    return 0;
}
//#endif

int mAudio_receiver_entry(struct ftm_param *param, void *priv)
{
    struct mAudio *mc = (struct mAudio *)priv;
    char *outputType = NULL;

    if(FTM_AUTO_ITEM == param->test_type)
    {
        outputType = ftm_get_prop("Audio.Auto.OutputType");
        ALOGD("Audio.Auto.OutputType = %s\n", outputType);

        if (outputType == NULL)
        {
            // @ default PCBA level, Phone receiver -> phone mic
            mc->recordDevice = BUILTIN_MIC;
            mAudio_receiver_auto_entry(param, priv);
        }
        else
        {
            if (atoi(outputType) == 0) // @ PCBA level, Phone receiver -> phone mic
            {
                mc->recordDevice = BUILTIN_MIC;
                mAudio_receiver_auto_entry(param, priv);
            }
            else if (atoi(outputType) == 1) //@ Phone level, phone receiver -> headset mic
            { 
                mc->recordDevice = WIRED_HEADSET;
                mAudio_receiver_auto_entry(param, priv);
            }
            else if (atoi(outputType) == 2) // @ Phone level, freq response
            {
                mc->recordDevice = WIRED_HEADSET;
                mAudio_receiver_freqresponse_entry (param, priv);
            }
            else if (atoi(outputType) == 3) // @ Phone level, THD
            {
                mc->recordDevice = WIRED_HEADSET;
                mAudio_receiver_thd_entry (param, priv);
            }
        }
	}
	else if(FTM_MANUAL_ITEM == param->test_type)
	{
		mAudio_receiver_manual_entry(param, priv);
	}

	return 0;
}

//Loopback test
int mAudio_reveiverloopback_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    bool exit = false;
    struct mAudio *mc = (struct mAudio *)priv;
    mc->exit_thd = false;
    mc->Headset_change = false;
    mc->avail = false;

    struct itemview *iv;

    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    g_loopback_item = 1;  // default: use MIC1 loopback
    g_prev_mic_state = 1;
    g_mic_change = 0;

    b_mic1_loopback = true;
    const char *numMicString = appGetFeatureOptionValue(MTK_AUDIO_NUMBER_OF_MIC);
    if (numMicString && atoi(numMicString) >= 2) {
        b_mic2_loopback = false;
    } else {
        b_mic2_loopback = true;
    }
    print_len1 = 0;
    print_len2 = 0;
    b_incomplete_flag = false;

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
    text_t cbtn ;
    text_t rbtn ;
    init_text(&lbtn, "fail", COLOR_YELLOW);
    init_text(&cbtn, "back", COLOR_YELLOW);
    init_text(&rbtn, "pass", COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    if (numMicString && atoi(numMicString) >= 2) {
        iv->set_items(iv, audio_items_loopback_dual_mic, 0);
    } else {
        iv->set_items(iv, audio_items_loopback, 0);
    }
    iv->set_text(iv, &mc->text);

#ifdef FEATURE_FTM_HEADSET
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Headset_detect_thread, priv);
#else
    //Add by Charlie, temp solution
    pthread_create(&mc->hHeadsetThread, NULL, Audio_Mic_change_thread, priv);
    //~Add by Charlie, temp solution
#endif

    do
    {
        chosen = iv->run(iv, &exit);
        switch (chosen)
        {
            case ITEM_MIC1:
                ALOGD("Select Mic1 loopback");
                b_mic1_loopback = true;
                g_loopback_item = 1;  // use MIC1 loopback
                if (g_prev_mic_state != g_loopback_item)
                {
                    g_mic_change = 1;
                }
                g_prev_mic_state = 1;
                break;
            case ITEM_MIC2:
                ALOGD("Select Mic2 loopback");
                b_mic2_loopback = true;
                g_loopback_item = 2;  // use MIC2 loopback
                if (g_prev_mic_state != g_loopback_item)
                {
                    g_mic_change = 1;
                }
                g_prev_mic_state = 2;
                break;
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS)
                {
                    if ((b_mic1_loopback == true) && (b_mic2_loopback == true))
                    {
                        b_incomplete_flag = false;
                        mc->mod->test_result = FTM_TEST_PASS;
                    }
                    else
                    {
                        char *ptr = mc->info + print_len1 + print_len2;
                        if (sprintf(ptr, "[%s] \n\n", " Test Loopback Case In-Complete ") < 0)
                        {
                            ALOGD("[WARN] sprintf error from ptr");
                        }
                        b_incomplete_flag = true;
                        iv->set_text(iv, &mc->text);
                        iv->redraw(iv);

                        break;
                    }
                }
                else if (chosen == ITEM_FAIL)
                {
                    mc->mod->test_result = FTM_TEST_FAIL;
                }

                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
                case L_BTN_DOWN:
                	mc->mod->test_result = FTM_TEST_FAIL;
                	exit = true;
                	break;
                case C_BTN_DOWN:
                	exit = true;
                	break;
                case R_BTN_DOWN:
                    if ((b_mic1_loopback == true) && (b_mic2_loopback == true))
                    {
                        b_incomplete_flag = false;
                        mc->mod->test_result = FTM_TEST_PASS;
                    }
                    else
                    {
                        char *ptr = mc->info + print_len1 + print_len2;
                        sprintf(ptr, "[%s] \n\n", " Test Loopback Case In-Complete ");
                        b_incomplete_flag = true;
                        iv->set_text(iv, &mc->text);
                        iv->redraw(iv);

                        break;
                    }
                	exit = true;
                	break;
#endif
        }

        if (exit)
        {
            ALOGD("mAudio_reveiverloopback_entry set exit_thd = true");
            mc->exit_thd = true;
            break;
        }
    }
    while (1);

#ifdef FEATURE_FTM_HEADSET
    pthread_join(mc->hHeadsetThread, NULL);

    if (mc->avail == true)
    {
        if (g_loopback_item == 1)
        {
            HeadsetMic_EarphoneLR_Loopback(MIC1_OFF, mc->Headset_mic);  // disable Earphone MIC1 loopback
        }
        else if (g_loopback_item == 2)
        {
            HeadsetMic_EarphoneLR_Loopback(MIC2_OFF, mc->Headset_mic);  // disable Earphone MIC2 loopback
        }
    }
    else
    {
        if (g_loopback_item == 1)
        {
            PhoneMic_Receiver_Loopback(MIC1_OFF);    // disable Receiver MIC1 loopback
        }
        else if (g_loopback_item == 2)
        {
            PhoneMic_Receiver_Loopback(MIC2_OFF);    // disable Receiver MIC2 loopback
        }
    }
#else
    //Add by Charlie, temp solution
    pthread_join(mc->hHeadsetThread, NULL);
    //~Add by Charlie, temp solution
    if (g_loopback_item == 1)
    {
        PhoneMic_Receiver_Loopback(MIC1_OFF);    // disable MIC1 loopback
    }
    else if (g_loopback_item == 2)
    {
        PhoneMic_Receiver_Loopback(MIC2_OFF);    // disable MIC2 loopback
    }
#endif

    g_loopback_item = 0;
    g_prev_mic_state = 0;
    g_mic_change = 0;
    Common_Audio_deinit();

    return 0;
}

#ifdef FEATURE_FTM_ACSLB
int mAudio_Acoustic_Loopback_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    int Acoustic_Type = DUAL_MIC_WITHOUT_DMNR_ACS_ON; //0:Acoustic loopback off; 1:Singlemic acoustic loopback; 2:Dualmic acoustic loopback
    int Acoustic_Status = 0;
    bool exit = false;
    struct mAudio *mc = (struct mAudio *)priv;
    mc->exit_thd = false;
    mc->Headset_change = false;
    mc->avail = false;

    struct itemview *iv;

    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            Common_Audio_deinit();
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
    text_t cbtn ;
    text_t rbtn ;
    init_text(&lbtn, "fail", COLOR_YELLOW);
    init_text(&cbtn, "back", COLOR_YELLOW);
    init_text(&rbtn, "pass", COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    const char *numMicString = appGetFeatureOptionValue(MTK_AUDIO_NUMBER_OF_MIC);
    if (numMicString && atoi(numMicString) >= 2) {
        iv->set_items(iv, audio_items_acoustic_loopback_dual_mic, 0);
        Acoustic_Type = DUAL_MIC_WITH_DMNR_ACS_ON;
    } else {
        iv->set_items(iv, audio_items_acoustic_loopback, 0);
    }
    iv->set_text(iv, &mc->text);

#ifdef FEATURE_FTM_HEADSET
    // inint thread mutex
    pthread_mutex_init(&mc->mHeadsetMutex, NULL);
    pthread_create(&mc->hHeadsetThread, NULL, PhoneMic_Receiver_Headset_Acoustic_Loopback, priv);
#else
    PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Type, &Acoustic_Status, mc->avail);
#endif

    do
    {
        chosen = iv->run(iv, &exit);
        Audio_headset_update_info(mc, mc->info);  // get headset information
        ALOGD("mAudio_Acoustic_Loopback_entry headset available %d", mc->avail);
        switch (chosen)
        {
            case ITEM_DUALMIC_DMNR:
#ifdef FEATURE_FTM_HEADSET
                pthread_mutex_lock(&mc->mHeadsetMutex);
#endif
                PhoneMic_Receiver_Acoustic_Loopback(ACOUSTIC_STATUS, &Acoustic_Status, mc->avail);
                if (Acoustic_Status & 0x1)
                {
                    PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Status - 1, &Acoustic_Status, mc->avail);
                }
                PhoneMic_Receiver_Acoustic_Loopback(DUAL_MIC_WITH_DMNR_ACS_ON, &Acoustic_Status, mc->avail);
#ifdef FEATURE_FTM_HEADSET
                pthread_mutex_unlock(&mc->mHeadsetMutex);
#endif
                break;
            case ITEM_DUALMIC_NO_DMNR:
#ifdef FEATURE_FTM_HEADSET
                pthread_mutex_lock(&mc->mHeadsetMutex);
#endif
                PhoneMic_Receiver_Acoustic_Loopback(ACOUSTIC_STATUS, &Acoustic_Status, mc->avail);
                if (Acoustic_Status & 0x1)
                {
                    PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Status - 1, &Acoustic_Status, mc->avail);
                }
                PhoneMic_Receiver_Acoustic_Loopback(DUAL_MIC_WITHOUT_DMNR_ACS_ON, &Acoustic_Status, mc->avail);
#ifdef FEATURE_FTM_HEADSET
                pthread_mutex_unlock(&mc->mHeadsetMutex);
#endif
                break;
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
                mc->mod->test_result = FTM_TEST_PASS;
                exit = true;
                break;
            case ITEM_FAIL:
                mc->mod->test_result = FTM_TEST_FAIL;
                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
			case L_BTN_DOWN:
				mc->mod->test_result = FTM_TEST_FAIL;
				exit = true;
				break;
			case C_BTN_DOWN:
				exit = true;
				break;
			case R_BTN_DOWN:
                mc->mod->test_result = FTM_TEST_PASS;
				exit = true;
				break;
#endif
            default:
                break;
        }

        if (exit)
        {
            ALOGD("mAudio_reveiverloopback_entry set exit_thd = true");
            mc->exit_thd = true;
            break;
        }
    }
    while (1);

#ifdef FEATURE_FTM_HEADSET
    pthread_join(mc->hHeadsetThread, NULL);
    pthread_mutex_lock(&mc->mHeadsetMutex);
#endif
    PhoneMic_Receiver_Acoustic_Loopback(ACOUSTIC_STATUS, &Acoustic_Status, mc->avail);
    if (Acoustic_Status & 0x1)
    {
        ALOGD("mAudio_Acoustic_Loopback_entry turn off loopback Acoustic_Status = %d", Acoustic_Status);
        PhoneMic_Receiver_Acoustic_Loopback(Acoustic_Status - 1, &Acoustic_Status, mc->avail);
    }
#ifdef FEATURE_FTM_HEADSET
    pthread_mutex_unlock(&mc->mHeadsetMutex);
    pthread_mutex_destroy(&mc->mHeadsetMutex);
#endif
    usleep(50000);
    Common_Audio_deinit();
    mc->exit_thd = true;
    return 0;
}
#endif

int mAudio_waveplayback_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    bool exit = false;
    struct mAudio *mc = (struct mAudio *)priv;
    struct itemview *iv;
    char calibrated_path[64] = "/sys/class/rt5509_cal/rt5509.0/calibrated";
    int size = 0, calibrated = 0, fd;
    char calibrationValue[8];

    ALOGD(TAG "----------mAudio_waveplayback_entry----------------\n");
    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    mc->exit_thd = false;

    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
    text_t cbtn ;
    text_t rbtn ;
    init_text(&lbtn, "fail", COLOR_YELLOW);
    init_text(&cbtn, "back", COLOR_YELLOW);
    init_text(&rbtn, "pass", COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items, 0);
    iv->set_text(iv, &mc->text);

    memset(mc->file_name, 0, MAX_FILE_NAME_SIZE);
    memset(mc->info, 0, TEXT_LENGTH);
    strncpy(mc->file_name, FileStartString_WavePlayback, sizeof(FileStartString_WavePlayback) - 1);
    mc->i4OutputType = Output_LPK;
    mc->i4Playtime = WAVE_PLAY_MAX_TIME;

    if (!ftmWavePlaybackCalib()) {
        goto BYPASS_SPK_CALIB;
    }

    fd = open(calibrated_path, O_RDONLY);
    if (fd < 0) {
        ALOGD("%s() cannot open calibrated file, Error!\n", __func__);
    } else {
        size = read(fd, calibrationValue, 8);
        close(fd);

        if (size < 0) {
            ALOGD("%s() cannot read calibrated file\n", __func__);
        } else {
            if (sscanf(calibrationValue, "%d\n", &calibrated) < 0)
                ALOGD("[WARN] sscanf error from calibrationValue");

            if (!calibrated)
                SpeakerCalibration(SPK_CALIB_STAGE_CALCULATE_AND_SAVE);

            fd = open(calibrated_path, O_RDONLY);
            if (fd >= 0) {
                size = read(fd, calibrationValue, 8);
                close(fd);
                if (sscanf(calibrationValue, "%d\n", &calibrated) < 0)
                    ALOGD("[WARN] sscanf error from calibrationValue");
            }

            int n = 0;
            if (calibrated)
                n = snprintf(mc->info, TEXT_LENGTH, "Calibrated!\n");
            else
                n = snprintf(mc->info, TEXT_LENGTH, "not Calibrated!\n");

            if (n < 0 || n >= TEXT_LENGTH)
                ALOGD("[WARN] sprintf error from mc->info");

            iv->redraw(iv);
        }
    }
BYPASS_SPK_CALIB:

    pthread_create(&mc->hHeadsetThread, NULL, Audio_Wave_Playabck_thread, priv);

    do
    {
        chosen = iv->run(iv, &exit);
        switch (chosen)
        {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS)
                {
                    mc->mod->test_result = FTM_TEST_PASS;
                }
                else if (chosen == ITEM_FAIL)
                {
                    mc->mod->test_result = FTM_TEST_FAIL;
                }
                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
		case L_BTN_DOWN:
			mc->mod->test_result = FTM_TEST_FAIL;
			exit = true;
			break;
		case C_BTN_DOWN:
			exit = true;
			break;
		case R_BTN_DOWN:
			mc->mod->test_result = FTM_TEST_PASS;
			exit = true;
			break;
#endif
        }
        if (exit)
        {
            ALOGD("mAudio_waveplayback_entry set exit_thd = true\n");
            mc->exit_thd = true;
            break;
        }
    }
    while (1);

    pthread_join(mc->hHeadsetThread, NULL);
    Common_Audio_deinit();
    return 0;

}

static void Audio_headset_info(struct mAudio *hds, char *info)
{
    int fd = -1;
    char *ptr ;
    int OriginState = hds->avail;

    fd = open(HEADSET_STATE_PATH, O_RDONLY, 0);
    if (fd == -1)
    {
        ALOGD(TAG "1Can't open %s\n", HEADSET_STATE_PATH);
        hds->avail = false;
        goto EXIT;
    }
    if (read(fd, rbuf, BUF_LEN) == -1)
    {
        ALOGD(TAG "1Can't read %s\n", HEADSET_STATE_PATH);
        hds->avail = false;
        close(fd);
        goto EXIT;
    }
    if (!strncmp(wbuf, rbuf, BUF_LEN))
    {
        hds->avail = true;
        close(fd);
        goto EXIT;
    }

    if (!strncmp(wbuf1, rbuf, BUF_LEN))
    {
        hds->avail = true;
    }
    else
    {
        hds->avail = false;
    }
    close(fd);

EXIT:
    if (OriginState  !=  hds->avail)
    {
        hds->Headset_change = true;
    }

    /* preare text view info */
    ptr = info;
    ptr += sprintf(ptr, "%s", "Please Insert The Headset for This Test  \n\n");

    return;
}

static void *Audio_PMic_Headset_Loopback(void *mPtr)
{
    struct mAudio *hds = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;

    ALOGD(TAG "%s: Start\n", __FUNCTION__);

    // open headset device
    HeadsetFd = open(HEADSET_PATH, O_RDONLY);
    if (HeadsetFd < 0)
    {
        ALOGE("1open %s error fd = %d", HEADSET_PATH, HeadsetFd);
        return 0;
    }

    //set headset state to 1 , enable hook
    ::ioctl(HeadsetFd, SET_CALL_STATE, 1);

    PhoneMic_EarphoneLR_Loopback(MIC1_ON);

    while (1)
    {
        usleep(100000);
        if (hds->exit_thd)
        {
            break;
        }

        Audio_headset_info(hds, hds->info);  // get headset information

        if (hds->avail && hds->Headset_change)
        {
            ALOGD(TAG  "Audio_PMic_Headset_Loopback:Headset plug-in (%d)\n", g_loopback_item);
            if (g_loopback_item == 1)
            {
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC1_OFF);
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC1_ON);
            }
            else if (g_loopback_item == 2)
            {
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC2_OFF);
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC2_ON);
            }
            g_mic_change = 0;
            hds->Headset_change = false;
        }
        else if (hds->Headset_change)
        {
            ALOGD(TAG "Audio_PMic_Headset_Loopback:Headset plug-out (%d)\n", g_loopback_item);
            if (g_loopback_item == 1)
            {
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC1_OFF);
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC1_ON);
            }
            else if (g_loopback_item == 2)
            {
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC2_OFF);
                usleep(3000);
                PhoneMic_EarphoneLR_Loopback(MIC2_ON);
            }
            g_mic_change = 0;
            hds->Headset_change = false;
        }
        else
        {
            if (g_mic_change == 1)
            {
                g_mic_change = 0;
                if (g_loopback_item == 1)
                {
                    usleep(3000);
                    PhoneMic_EarphoneLR_Loopback(MIC2_OFF);
                    usleep(3000);
                    PhoneMic_EarphoneLR_Loopback(MIC1_ON);
                }
                else if (g_loopback_item == 2)
                {
                    usleep(3000);
                    PhoneMic_EarphoneLR_Loopback(MIC1_OFF);
                    usleep(3000);
                    PhoneMic_EarphoneLR_Loopback(MIC2_ON);
                }
            }
        }

        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }

    ALOGD(TAG "%s: Audio_PMic_Headset_Loopback Exit (%d) \n", __FUNCTION__, g_loopback_item);

    PhoneMic_EarphoneLR_Loopback(MIC1_OFF);
    usleep(3000);
    PhoneMic_EarphoneLR_Loopback(MIC2_OFF);

    // set headset state to 0 , lose headset device.
    ::ioctl(HeadsetFd, SET_CALL_STATE, 0);

    close(HeadsetFd);
    HeadsetFd = 0;

    pthread_exit(NULL); // thread exit
    return NULL;
}

int Audio_PhoneMic_Headset_loopback_entry(struct ftm_param *param, void *priv)
{
    ALOGD("Select Audio_PhoneMic_Headset_loopback_entry");
    int chosen;
    bool exit = false;
    struct mAudio *mc = (struct mAudio *)priv;
    mc->exit_thd = false;
    mc->Headset_change = false;
    mc->avail = false;

    struct itemview *iv;

    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
    text_t cbtn ;
    text_t rbtn ;
    init_text(&lbtn, "fail", COLOR_YELLOW);
    init_text(&cbtn, "back", COLOR_YELLOW);
    init_text(&rbtn, "pass", COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    const char *numMicString = appGetFeatureOptionValue(MTK_AUDIO_NUMBER_OF_MIC);
    if (numMicString && atoi(numMicString) >= 2) {
        iv->set_items(iv, audio_items_loopback_dual_mic, 0);
    } else {
        iv->set_items(iv, audio_items_loopback, 0);
    }
    iv->set_text(iv, &mc->text);

    //check headset state
#ifdef GET_HEADSET_STATE
    init_Headset();
    int headset_state = get_headset_info();
    if (headset_state > 0)
    {
        ;
    }
    else
    {
        ALOGD("no headset device\n");
        if (sprintf(mc->info, "%s", "Please Insert Headset before This Test!\n") < 0)
            ALOGD("[WARN] sprintf error from mc->info");

        iv->redraw(iv);
        usleep(3000 * 1000);
        return -1;
    }
#endif

    // init Audio
    Common_Audio_init();
    g_loopback_item = 1;  // default: use MIC1 loopback
    g_prev_mic_state = 1;
    g_mic_change = 0;

#ifdef FEATURE_FTM_HEADSET
    Audio_headset_info(mc, mc->info);  // get headse information
#endif

#ifdef FEATURE_FTM_HEADSET
    pthread_create(&mc->hHeadsetThread, NULL, Audio_PMic_Headset_Loopback, priv);
#endif

    do
    {
        chosen = iv->run(iv, &exit);
        switch (chosen)
        {
            case ITEM_MIC1:
                ALOGD("Select Mic1 loopback");
                g_loopback_item = 1;  // use MIC1 loopback
                if (g_prev_mic_state != g_loopback_item)
                {
                    g_mic_change = 1;
                }
                g_prev_mic_state = 1;
                break;
            case ITEM_MIC2:
                ALOGD("Select Mic2 loopback");
                g_loopback_item = 2;  // use MIC2 loopback
                if (g_prev_mic_state != g_loopback_item)
                {
                    g_mic_change = 1;
                }
                g_prev_mic_state = 2;
                break;
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS)
                {
                    mc->mod->test_result = FTM_TEST_PASS;
                }
                else if (chosen == ITEM_FAIL)
                {
                    mc->mod->test_result = FTM_TEST_FAIL;
                }

                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
            case L_BTN_DOWN:
            	mc->mod->test_result = FTM_TEST_FAIL;
            	exit = true;
            	break;
            case C_BTN_DOWN:
            	exit = true;
            	break;
            case R_BTN_DOWN:
            	mc->mod->test_result = FTM_TEST_PASS;
            	exit = true;
            	break;
#endif
        }

        if (exit)
        {
            ALOGD("Audio_PhoneMic_Headset_loopback_entry set exit_thd = true");
            mc->exit_thd = true;
            break;
        }
    }
    while (1);

#ifdef FEATURE_FTM_HEADSET
    pthread_join(mc->hHeadsetThread, NULL);

    if (mc->avail == true)
    {
        ALOGD("disable HeadsetMic_EarphoneLR_Loopback:%d ", g_loopback_item);
        if (g_loopback_item == 1)
        {
            PhoneMic_EarphoneLR_Loopback(MIC1_OFF);    // disable MIC1 loopback
        }
        else if (g_loopback_item == 2)
        {
            PhoneMic_EarphoneLR_Loopback(MIC2_OFF);    // disable MIC2 loopback
        }
    }
#endif

    g_loopback_item = 0;
    g_prev_mic_state = 0;
    g_mic_change = 0;
    Common_Audio_deinit();
    return 0;
}

int Audio_PhoneMic_Speaker_loopback_entry(struct ftm_param *param, void *priv)
{
    ALOGD("Select Audio_PhoneMic_Speaker_loopback_entry");
    int chosen;
    bool exit = false;
    struct mAudio *mc = (struct mAudio *)priv;
    mc->exit_thd = false;
    mc->Headset_change = false;
    mc->avail = false;

    struct itemview *iv;

    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();
    g_loopback_item = 1;  // default: use MIC1 loopback
    g_prev_mic_state = 1;

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
    text_t cbtn ;
    text_t rbtn ;
    init_text(&lbtn, "fail", COLOR_YELLOW);
    init_text(&cbtn, "back", COLOR_YELLOW);
    init_text(&rbtn, "pass", COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    const char *numMicString = appGetFeatureOptionValue(MTK_AUDIO_NUMBER_OF_MIC);
    if (numMicString && atoi(numMicString) >= 2) {
        iv->set_items(iv, audio_items_loopback_dual_mic, 0);
    } else {
        iv->set_items(iv, audio_items_loopback, 0);
    }
    iv->set_text(iv, &mc->text);

    PhoneMic_SpkLR_Loopback(MIC1_ON);

    do
    {
        chosen = iv->run(iv, &exit);
        switch (chosen)
        {
            case ITEM_MIC1:
                ALOGD("Select Mic1 loopback");
                g_loopback_item = 1;  // use MIC1 loopback
                if (g_prev_mic_state != g_loopback_item)
                {
                    PhoneMic_SpkLR_Loopback(MIC2_OFF);
                }
                else
                {
                    break;
                }
                PhoneMic_SpkLR_Loopback(MIC1_ON);
                g_prev_mic_state = 1;
                break;
            case ITEM_MIC2:
                ALOGD("Select Mic2 loopback");
                g_loopback_item = 2;  // use MIC2 loopback
                if (g_prev_mic_state != g_loopback_item)
                {
                    PhoneMic_SpkLR_Loopback(MIC1_OFF);
                }
                else
                {
                    break;
                }
                PhoneMic_SpkLR_Loopback(MIC2_ON);
                g_prev_mic_state = 2;
                break;
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS)
                {
                    mc->mod->test_result = FTM_TEST_PASS;
                }
                else if (chosen == ITEM_FAIL)
                {
                    mc->mod->test_result = FTM_TEST_FAIL;
                }

                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
                case L_BTN_DOWN:
                	mc->mod->test_result = FTM_TEST_FAIL;
                	exit = true;
                	break;
                case C_BTN_DOWN:
                	exit = true;
                	break;
                case R_BTN_DOWN:
                	mc->mod->test_result = FTM_TEST_PASS;
                	exit = true;
                	break;
#endif
        }

        if (exit)
        {
            ALOGD("Audio_PhoneMic_Speaker_loopback_entry exit_thd = true");
            mc->exit_thd = true;
            break;
        }
    }
    while (1);


    ALOGD("disable Audio_PhoneMic_Speaker_loopback_entry:%d ", g_loopback_item);
    if (g_loopback_item == 1)
    {
        PhoneMic_SpkLR_Loopback(MIC1_OFF);    // disable MIC1 loopback
    }
    else if (g_loopback_item == 2)
    {
        PhoneMic_SpkLR_Loopback(MIC2_OFF);    // disable MIC2 loopback
    }

    g_loopback_item = 0;
    g_prev_mic_state = 0;

    Common_Audio_deinit();
    return 0;
}

static void *Audio_HMic_SPK_Loopback(void *mPtr)
{
    struct mAudio *hds = (struct mAudio *)mPtr;
    struct itemview *iv = hds->iv;
    ALOGD(TAG "%s: Start\n", __FUNCTION__);

    // open headset device
    HeadsetFd = open(HEADSET_PATH, O_RDONLY);
    if (HeadsetFd < 0)
    {
        ALOGE("2open %s error fd = %d", HEADSET_PATH, HeadsetFd);
        return 0;
    }

    //set headset state to 1 , enable hook
    ::ioctl(HeadsetFd, SET_CALL_STATE, 1);
    HeadsetMic_SpkLR_Loopback(MIC1_ON);

    while (1)
    {
        usleep(100000);
        if (hds->exit_thd)
        {
            break;
        }
        Audio_headset_info(hds, hds->info);  // get headset information
        if (hds->avail && hds->Headset_change)
        {
            ALOGD(TAG  "Audio_HMic_SPK_Loopback:Headset plug-in (%d)\n", g_loopback_item);
            HeadsetMic_SpkLR_Loopback(MIC1_ON);
            hds->Headset_change = false;
        }
        else if (hds->Headset_change)
        {
            ALOGD(TAG "Audio_HMic_SPK_Loopback:Headset plug-out (%d)\n", g_loopback_item);
            HeadsetMic_SpkLR_Loopback(MIC1_ON);
            hds->Headset_change = false;
        }
        iv->set_text(iv, &hds->text);
        iv->redraw(iv);
    }

    ALOGD(TAG "%s: Audio_HMic_SPK_Loopback Exit (%d) \n", __FUNCTION__, g_loopback_item);
    HeadsetMic_SpkLR_Loopback(MIC1_OFF);

    // set headset state to 0 , lose headset device.
    ::ioctl(HeadsetFd, SET_CALL_STATE, 0);

    close(HeadsetFd);
    HeadsetFd = 0;

    pthread_exit(NULL); // thread exit
    return NULL;
}

int Audio_HeadsetMic_Speaker_loopback_entry(struct ftm_param *param, void *priv)
{
    ALOGD("Select Audio_HeadsetMic_Speaker_loopback_entry");
    int chosen;
    bool exit = false;
    struct mAudio *mc = (struct mAudio *)priv;
    mc->exit_thd = false;
    mc->Headset_change = false;
    mc->avail = false;

    struct itemview *iv;

    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
	text_t cbtn ;
	text_t rbtn ;
	init_text(&lbtn, "fail", COLOR_YELLOW);
	init_text(&cbtn, "back", COLOR_YELLOW);
	init_text(&rbtn, "pass", COLOR_YELLOW);
	iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items, 0);
    iv->set_text(iv, &mc->text);

    //check headset state
#ifdef GET_HEADSET_STATE
    init_Headset();
    int headset_state = get_headset_info();
    if (headset_state > 0)
    {
        ;
    }
    else
    {
        ALOGD("[FM]no headset device\n");
        if (sprintf(mc->info, "%s", "Please Insert Headset before This Test!\n") < 0)
            ALOGD("[WARN] sprintf error from mc->info");

        iv->redraw(iv);
        usleep(3000 * 1000);
        return -1;
    }
#endif

    // init Audio
    Common_Audio_init();
    g_loopback_item = 1;  // default: use MIC1 loopback
    g_prev_mic_state = 1;
    g_mic_change = 0;

#ifdef FEATURE_FTM_HEADSET
    Audio_headset_info(mc, mc->info);  // get headse information
#endif

#ifdef FEATURE_FTM_HEADSET
    pthread_create(&mc->hHeadsetThread, NULL, Audio_HMic_SPK_Loopback, priv);
#endif

    do
    {
        chosen = iv->run(iv, &exit);
        switch (chosen)
        {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS)
                {
                    mc->mod->test_result = FTM_TEST_PASS;
                }
                else if (chosen == ITEM_FAIL)
                {
                    mc->mod->test_result = FTM_TEST_FAIL;
                }

                exit = true;
                break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
                case L_BTN_DOWN:
                	mc->mod->test_result = FTM_TEST_FAIL;
                	exit = true;
                	break;
                case C_BTN_DOWN:
                	exit = true;
                	break;
                case R_BTN_DOWN:
                	mc->mod->test_result = FTM_TEST_PASS;
                	exit = true;
                	break;
#endif
        }

        if (exit)
        {
            ALOGD("Audio_HeadsetMic_Speaker_loopback_entry set exit_thd = true");
            mc->exit_thd = true;
            break;
        }
    }
    while (1);

#ifdef FEATURE_FTM_HEADSET
    pthread_join(mc->hHeadsetThread, NULL);
    if (mc->avail == true)
    {
        ALOGD("disable Audio_HeadsetMic_Speaker_loopback_entry:%d ", g_loopback_item);
        HeadsetMic_SpkLR_Loopback(MIC1_OFF);    // disable MIC1 loopback
    }
#endif

    g_loopback_item = 0;
    g_prev_mic_state = 0;
    g_mic_change = 0;
    Common_Audio_deinit();
    return 0;
}

//-------- Micbias Test Entry --------
int mAudio_Micbias_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    bool exit = false;
    bMicbias_exit = false; //for ATA Tool control.

    short pbuffer[2] = {0};
    bool isMicOn = false;
    uint32_t samplerate = 0;

    struct mAudio *mc = (struct mAudio *)priv;
    struct itemview *iv;

    ALOGD(TAG "%s\n", __FUNCTION__);
    init_text(&mc->title, param->name, COLOR_YELLOW);
    init_text(&mc->text, &mc->info[0], COLOR_YELLOW);
    init_text(&mc->left_btn, "fail", COLOR_YELLOW);
    init_text(&mc->center_btn, "pass", COLOR_YELLOW);
    init_text(&mc->right_btn, "back", COLOR_YELLOW);

    // init Audio
    Common_Audio_init();

    // ui start
    if (!mc->iv)
    {
        iv = ui_new_itemview();
        if (!iv)
        {
            ALOGD(TAG "No memory");
            Common_Audio_deinit();
            return -1;
        }
        mc->iv = iv;
    }
    iv = mc->iv;
#ifdef FEATURE_FTM_TOUCH_MODE
	text_t lbtn ;
    text_t cbtn ;
    text_t rbtn ;
    init_text(&lbtn, "fail", COLOR_YELLOW);
    init_text(&cbtn, "back", COLOR_YELLOW);
    init_text(&rbtn, "pass", COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
    iv->set_title(iv, &mc->title);
    iv->set_items(iv, audio_items_micbias, 0);
    iv->set_text(iv, &mc->text);

    do
    {
        // Test by Manual
        if (!get_is_ata())
        {
            chosen = iv->run(iv, &exit);

            switch (chosen)
            {
                case ITEM_MICBIAS_ON: /*Open Mic*/
                    ALOGD(TAG "Micbias ON.");
                    if (!isMicOn)
                    {
                        recordInit(BUILTIN_MIC, &samplerate);
                        memset(pbuffer, 0, sizeof(pbuffer));
                        readRecordData(pbuffer, 2 * 2);
                    }
                    isMicOn = true;
                    break;
                case ITEM_MICBIAS_OFF: /*Close Mic*/
                    ALOGD(TAG "Micbias OFF.");
                    if (isMicOn)
                    {
                        Common_Audio_deinit();
                    }
                    isMicOn = false;
#ifndef FEATURE_FTM_TOUCH_MODE
                    break;
                case ITEM_PASS:
                    mc->mod->test_result = FTM_TEST_PASS;
                    exit = true;
                    break;
                case ITEM_FAIL:
                    mc->mod->test_result = FTM_TEST_FAIL;
                    exit = true;
                    break;
#endif
#ifdef FEATURE_FTM_TOUCH_MODE
					case L_BTN_DOWN:
						mc->mod->test_result = FTM_TEST_FAIL;
						exit = true;
						break;
					case C_BTN_DOWN:
						exit = true;
						break;
					case R_BTN_DOWN:
						mc->mod->test_result = FTM_TEST_PASS;
						exit = true;
						break;
#endif
                default:
                    break;
            }

            if (exit)
            {
                bMicbias_exit = true;
                ALOGD(TAG "Micbias EXIT.");
                break;
            }
        }
        // Test by ATA Tool
        else
        {
            if (!isMicOn)
            {
                ALOGD(TAG "Micbias ON by ATA.");
                recordInit(BUILTIN_MIC, &samplerate);
                memset(pbuffer, 0, sizeof(pbuffer));
                readRecordData(pbuffer, 2 * 2);
                isMicOn = true;
            }
            iv->start_menu(iv, 0);
            iv->redraw(iv);
            if (bMicbias_exit)
            {
                ALOGD(TAG "Micbias EXIT by ATA.");
                break;
            }
        }
    }
    while (1);

    usleep(50000);
    Common_Audio_deinit();

    return 0;
}

//-------------------------------

int audio_init(void)
{
    int ret = 0;
    struct ftm_module *modLoudspk, *modReceiver,
            *modReceiverLoopback,
            *modPMic_Headset_Loopback,
            *modPMic_SPK_Loopback,
            *modHMic_SPK_Loopback,
            *modWavePlayback,
            *modAcoustic_Loopback,
            *modMicbias;
            //*modReceiverFreqResponse,
            //*modSpeakerFreqResponse,

    struct mAudio *maudioSpk, *maudioReveiver,
            *maudioReveiverLoopback,
            *maudioPMic_Headset_Loopback,
            *maudioPMic_SPK_Loopback,
            *maudioHMic_SPK_Loopback,
            *maudioWavePlayback,
            *maudioAcousticLoopback,
            *maudioMicbias;
            //*maudioReceiverFreqResponse,
            //*maudioSpeakerFreqResponse,

#if defined(FEATURE_FTM_SPEAKER_CALIBRATION)
    struct mAudio *maudioSpkRdcCalibration;
    struct ftm_module *modSpkRdcCalibration;
#endif

    ALOGD(TAG "%s\n", __FUNCTION__);
    ALOGD(TAG "-------Audio_init------------------\n");

    modLoudspk = ftm_alloc(ITEM_LOOPBACK_PHONEMICSPK, sizeof(struct mAudio));
    maudioSpk = mod_to_mAudio(modLoudspk);
    maudioSpk->mod = modLoudspk;
    maudioSpk->recordDevice = BUILTIN_MIC;
    if (!modLoudspk)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modLoudspk, mAudio_loopback_phonemicspk_entry, (void *)maudioSpk);

    modReceiver = ftm_alloc(ITEM_RECEIVER, sizeof(struct mAudio));
    maudioReveiver = mod_to_mAudio(modReceiver);
    maudioReveiver->mod = modReceiver;
    maudioReveiver->recordDevice = BUILTIN_MIC;
    if (!modReceiver)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modReceiver, mAudio_receiver_entry, (void *)maudioReveiver);

    modReceiverLoopback = ftm_alloc(ITEM_LOOPBACK, sizeof(struct mAudio));
    maudioReveiverLoopback = mod_to_mAudio(modReceiverLoopback);
    maudioReveiverLoopback->mod = modReceiverLoopback;
    if (!modReceiverLoopback)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modReceiverLoopback, mAudio_reveiverloopback_entry, (void *)maudioReveiverLoopback);

    modPMic_Headset_Loopback = ftm_alloc(ITEM_LOOPBACK1, sizeof(struct mAudio));
    maudioPMic_Headset_Loopback = mod_to_mAudio(modPMic_Headset_Loopback);
    maudioPMic_Headset_Loopback->mod = modPMic_Headset_Loopback;
    if (!modPMic_Headset_Loopback)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modPMic_Headset_Loopback, Audio_PhoneMic_Headset_loopback_entry, (void *)maudioPMic_Headset_Loopback);

    modPMic_SPK_Loopback = ftm_alloc(ITEM_LOOPBACK2, sizeof(struct mAudio));
    maudioPMic_SPK_Loopback = mod_to_mAudio(modPMic_SPK_Loopback);
    maudioPMic_SPK_Loopback->mod = modPMic_SPK_Loopback;
    if (!modPMic_SPK_Loopback)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modPMic_SPK_Loopback, Audio_PhoneMic_Speaker_loopback_entry, (void *)maudioPMic_SPK_Loopback);

    modHMic_SPK_Loopback = ftm_alloc(ITEM_LOOPBACK3, sizeof(struct mAudio));
    maudioHMic_SPK_Loopback = mod_to_mAudio(modHMic_SPK_Loopback);
    maudioHMic_SPK_Loopback->mod = modHMic_SPK_Loopback;
    if (!modHMic_SPK_Loopback)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modHMic_SPK_Loopback, Audio_HeadsetMic_Speaker_loopback_entry, (void *)maudioHMic_SPK_Loopback);

    modWavePlayback = ftm_alloc(ITEM_WAVEPLAYBACK, sizeof(struct mAudio));
    maudioWavePlayback = mod_to_mAudio(modWavePlayback);
    maudioWavePlayback->mod = modWavePlayback;
    if (!modWavePlayback) 
    {
        return -ENOMEM;
    }
    ret = ftm_register(modWavePlayback, mAudio_waveplayback_entry, (void *)maudioWavePlayback);
#ifdef FEATURE_FTM_ACSLB
    modAcoustic_Loopback = ftm_alloc(ITEM_ACOUSTICLOOPBACK, sizeof(struct mAudio));
    maudioAcousticLoopback = mod_to_mAudio(modAcoustic_Loopback);
    maudioAcousticLoopback->mod = modAcoustic_Loopback;
    if (!modAcoustic_Loopback)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modAcoustic_Loopback, mAudio_Acoustic_Loopback_entry, (void *)maudioAcousticLoopback);
#endif
    //Micbias Test
    modMicbias = ftm_alloc(ITEM_MICBIAS, sizeof(struct mAudio));
    maudioMicbias = mod_to_mAudio(modMicbias);
    maudioMicbias->mod = modMicbias;
    if (!modMicbias)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modMicbias, mAudio_Micbias_entry, (void *)maudioMicbias);

#if defined(FEATURE_FTM_SPEAKER_CALIBRATION)
    /* Speaker Calibration for SmartPa */
    modSpkRdcCalibration = ftm_alloc(ITEM_SPEAKER_RDC_CALIBRATION, sizeof(struct mAudio));
    maudioSpkRdcCalibration = mod_to_mAudio(modSpkRdcCalibration);
    maudioSpkRdcCalibration->mod = modSpkRdcCalibration;
    if (!modSpkRdcCalibration)
    {
        return -ENOMEM;
    }
    ret = ftm_register(modSpkRdcCalibration, mAudio_speaker_Rdc_calibration_entry, (void *)maudioSpkRdcCalibration);
#endif

    return ret;
}
#endif

#ifdef __cplusplus
};
#endif


