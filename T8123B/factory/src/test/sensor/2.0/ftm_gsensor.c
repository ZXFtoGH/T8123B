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
#include <dirent.h>
#include <linux/input.h>
#include <math.h>
#include <unistd.h>
#include "common.h"
#include "miniui.h"
#include "ftm.h"
#include "cust.h"

#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <HfManagerWrapper.h>
#include <hardware/sensors.h>
#include "HfSensorType.h"

#ifdef FEATURE_FTM_ACCELEROMETER
#include <linux/sensors_io.h>

/******************************************************************************
 * MACRO
 *****************************************************************************/
#define TAG "[ACC] "
#define mod_to_acc_data(p) (struct acc_data*)((char*)(p) + sizeof(struct ftm_module))
#define FTGLOGD(fmt, arg ...) LOGD(TAG fmt, ##arg)
#define FTGLOGE(fmt, arg ...) LOGE("%s [%5d]: " fmt, __func__, __LINE__, ##arg)
#define GSENSOR_ATTR_SELFTEST "/sys/bus/platform/drivers/gsensor/selftest"
#define C_MAX_HWMSEN_EVENT_NUM 4
#define GRAVITY_EARTH           (9.80665f)
#define TOLERANCE_0G            (1.00)
#define TOLERANCE_1G            (9.5)
#define C_MAX_MEASURE_NUM (20)
#define SENSOR_SAMPLE_PRRIOD_NS 200000000
#define TIMEOUT_MS 500
/******************************************************************************
 * global variable
 *****************************************************************************/
bool gsensor_thread_exit = false;
static int sp_ata_status = 0;
static pthread_mutex_t gsensor_mutex = PTHREAD_MUTEX_INITIALIZER;
extern sp_ata_data return_data;

/******************************************************************************
 * Structure
 *****************************************************************************/
enum {
    ITEM_PASS,
    ITEM_FAIL,
};

static item_t gsensor_items[] = {
#ifndef FEATURE_FTM_TOUCH_MODE
    item(ITEM_PASS,   uistr_pass),
    item(ITEM_FAIL,   uistr_fail),
#endif
    item(-1, NULL),
};

typedef enum {
    TILT_UNKNOWN = 0,
    TILT_X_POS  = 1,
    TILT_X_NEG  = 2,
    TILT_Y_POS  = 3,
    TILT_Y_NEG  = 4,
    TILT_Z_POS  = 5,
    TILT_Z_NEG  = 6,

    TILT_MAX,
} TILT_POS;

char *gsensor_pos[] = {
    uistr_info_g_sensor_unknow,
    "X+",
    "X-",
    "Y+",
    "Y-",
    "Z+",
    "Z-",
};

struct acc_evt {
    float x;
    float y;
    float z;
    //int64_t time;
};

struct acc_priv {
    /*specific data field*/
    void *hf_manager;
    void *hf_looper;
    struct acc_evt evt;
    int  tilt;
    unsigned int pos_chk;

    /*calculate statical information*/
    int statistics;  /*0: calculating; 1: done*/
    int measure_idx;
    float raw[C_MAX_MEASURE_NUM][C_MAX_HWMSEN_EVENT_NUM];
    float std[C_MAX_HWMSEN_EVENT_NUM];
    float avg[C_MAX_HWMSEN_EVENT_NUM];
    float max[C_MAX_HWMSEN_EVENT_NUM];
    float min[C_MAX_HWMSEN_EVENT_NUM];
};

struct acc_data {
    struct acc_priv acc;

    /*common for each factory mode*/
    char info[1024];
    //bool  avail;
    bool exit_thd;

    text_t title;
    text_t text;
    text_t left_btn;
    text_t center_btn;
    text_t right_btn;

    pthread_t update_thd;
    struct ftm_module *mod;
    //struct textview tv;
    struct itemview *iv;
};

static int gsensor_statistic(struct acc_priv* acc)
{
    int idx;
    float diff;

    if (acc->statistics)
        return 0;

    /*record data*/
    if (acc->measure_idx < C_MAX_MEASURE_NUM) {
        //FTGLOGD("init  acc->measure_idx =%d \n",acc->measure_idx );
        acc->raw[acc->measure_idx][0] = acc->evt.x;
        acc->raw[acc->measure_idx][1] = acc->evt.y;
        acc->raw[acc->measure_idx][2] = acc->evt.z;

        acc->avg[0] += acc->evt.x;
        acc->avg[1] += acc->evt.y;
        acc->avg[2] += acc->evt.z;
    }

    if (acc->measure_idx < C_MAX_MEASURE_NUM) {
        acc->measure_idx++;
        return 0;
    }

    for (idx = 0; idx < C_MAX_HWMSEN_EVENT_NUM; idx++) {
        acc->min[idx] = +100 * GRAVITY_EARTH;
        acc->max[idx] = -100 * GRAVITY_EARTH;
    }
    acc->avg[0] /= C_MAX_MEASURE_NUM;
    acc->avg[1] /= C_MAX_MEASURE_NUM;
    acc->avg[2] /= C_MAX_MEASURE_NUM;

    for (idx = 0; idx < C_MAX_MEASURE_NUM; idx++) {
        diff = acc->raw[idx][0] - acc->avg[0];
        acc->std[0] += diff * diff;
        diff = acc->raw[idx][1] - acc->avg[1];
        acc->std[1] += diff * diff;
        diff = acc->raw[idx][2] - acc->avg[2];
        acc->std[2] += diff * diff;
        if (acc->max[0] < acc->raw[idx][0])
            acc->max[0] = acc->raw[idx][0];
        if (acc->max[1] < acc->raw[idx][1])
            acc->max[1] = acc->raw[idx][1];
        if (acc->max[2] < acc->raw[idx][2])
            acc->max[2] = acc->raw[idx][2];

        if (acc->min[0] > acc->raw[idx][0])
            acc->min[0] = acc->raw[idx][0];
        if (acc->min[1] > acc->raw[idx][1])
            acc->min[1] = acc->raw[idx][1];
        if (acc->min[2] > acc->raw[idx][2])
            acc->min[2] = acc->raw[idx][2];

        FTGLOGD("[%2d][%+6.3f %+6.3f %+6.3f]\n", idx, acc->raw[idx][0], acc->raw[idx][1], acc->raw[idx][2]);
    }
    acc->std[0] = sqrt(acc->std[0] / C_MAX_MEASURE_NUM);
    acc->std[1] = sqrt(acc->std[1] / C_MAX_MEASURE_NUM);
    acc->std[2] = sqrt(acc->std[2] / C_MAX_MEASURE_NUM);
    acc->statistics = 1;
    return 0;
}

/**
 * common interface
 * Return 0 if gsensor_open sucess, otherwise fail.
 */
static int gsensor_open(struct acc_priv *acc)
{
    acc->hf_manager = HfManagerCreate();
    acc->hf_looper = HfLooperCreate(HfManagerGetFd(acc->hf_manager), 64);

    if (HfManagerFindSensor(acc->hf_manager, SENSOR_TYPE_ACCELEROMETER) < 0) {
        LOGD(TAG "find gsensor fail, sensor type is\n");
        goto err_out;
    }

    HfManagerEnableSensor(acc->hf_manager, SENSOR_TYPE_ACCELEROMETER, SENSOR_SAMPLE_PRRIOD_NS, 0);
    HfManagerEnableRawData(acc->hf_manager, SENSOR_TYPE_ACCELEROMETER);
    return 0;

err_out:
    HfLooperDestroy(acc->hf_looper);
    HfManagerDestroy(acc->hf_manager);
    return -1;
}

static int gsensor_close(struct acc_priv *acc)
{
    HfManagerDisableRawData(acc->hf_manager, SENSOR_TYPE_ACCELEROMETER);
    HfManagerDisableSensor(acc->hf_manager, SENSOR_TYPE_ACCELEROMETER);

    HfLooperDestroy(acc->hf_looper);
    HfManagerDestroy(acc->hf_manager);

    memset(acc, 0x00, sizeof(*acc));
    return 0;
}

static int gsensor_check_tilt(struct acc_priv *acc)
{
    if ((acc->evt.x  >  TOLERANCE_1G) &&
        (acc->evt.y  > -TOLERANCE_0G  &&  acc->evt.y  <  TOLERANCE_0G) &&
        (acc->evt.z  > -TOLERANCE_0G  &&  acc->evt.z  <  TOLERANCE_0G)) {
        acc->tilt = TILT_X_POS;
        return 0;
    }
    if ((acc->evt.x  < -TOLERANCE_1G) &&
        (acc->evt.y  > -TOLERANCE_0G  &&  acc->evt.y  <  TOLERANCE_0G) &&
        (acc->evt.z  > -TOLERANCE_0G  &&  acc->evt.z  <  TOLERANCE_0G)) {
        acc->tilt = TILT_X_NEG;
        return 0;
    }
    if ((acc->evt.y  >  TOLERANCE_1G) &&
        (acc->evt.x  > -TOLERANCE_0G  &&  acc->evt.x  <  TOLERANCE_0G) &&
        (acc->evt.z  > -TOLERANCE_0G  &&  acc->evt.z  <  TOLERANCE_0G)) {
        acc->tilt = TILT_Y_POS;
        return 0;
    }
    if ((acc->evt.y  < -TOLERANCE_1G) &&
        (acc->evt.x  > -TOLERANCE_0G  &&  acc->evt.x  <  TOLERANCE_0G) &&
        (acc->evt.z  > -TOLERANCE_0G  &&  acc->evt.z  <  TOLERANCE_0G)) {
        acc->tilt = TILT_Y_NEG;
        return 0;
    }
    if ((acc->evt.z  >  TOLERANCE_1G) &&
        (acc->evt.x  > -TOLERANCE_0G  &&  acc->evt.x  <  TOLERANCE_0G) &&
        (acc->evt.y  > -TOLERANCE_0G  &&  acc->evt.y  <  TOLERANCE_0G)) {
        acc->tilt = TILT_Z_POS;
        return 0;
    }
    if ((acc->evt.z  < -TOLERANCE_1G) &&
        (acc->evt.x  > -TOLERANCE_0G  &&  acc->evt.x  <  TOLERANCE_0G) &&
        (acc->evt.y  > -TOLERANCE_0G  &&  acc->evt.y  <  TOLERANCE_0G)) {
        acc->tilt = TILT_Z_NEG;
        return 0;
    }
    //snprintf(acc->tiltbuf, sizeof(acc->tiltbuf), "Tilt: unknown\n");
    acc->tilt = TILT_UNKNOWN;
    return 0;
}

static int gsensor_init_priv(struct acc_priv *acc)
{
    memset(acc, 0x00, sizeof(*acc));
    acc->pos_chk = 0x00;
    return 0;
}

static int gsensor_read(struct acc_priv *acc)
{
    sensors_event_t data[32] = {0};

    int err = HfLooperEventLooperTimeout(acc->hf_looper, data, 32, TIMEOUT_MS);

    if (err < 0) {
        LOGE(TAG "Hf looper event looper retrun value:%d\n", err);
        return err;
    }

    for (int i = 0; i < err; ++i) {
        if (data[i].reserved0 == RAW_ACTION) {
            acc->evt.x = (float)(data[i].data[0]);
            acc->evt.y = (float)(data[i].data[1]);
            acc->evt.z = (float)(data[i].data[2]);

            gsensor_statistic(acc);

            // add sensor data to struct sp_ata data for PC side
            return_data.gsensor.g_sensor_x = acc->evt.x;
            return_data.gsensor.g_sensor_y = acc->evt.y;
            return_data.gsensor.g_sensor_z = acc->evt.z;
            return_data.gsensor.accuracy = 3;
        }
    }

    return 0;
}

static void *gsensor_update_iv_thread(void *priv)
{
    struct acc_data *dat = (struct acc_data *)priv;
    struct acc_priv *acc = &dat->acc;
    struct itemview *iv = dat->iv;
    int len = 0;
    char *status = uistr_info_g_sensor_notsupport;
    int err = 0;

    if ((err = gsensor_open(acc))) {
        memset(dat->info, 0x00, sizeof(dat->info));
        sprintf(dat->info, "INIT FAILED\n");
        iv->redraw(iv);
        FTGLOGE("gsensor_open() err = %d(%s)\n", err, dat->info);
        return NULL;
    }

    while (1) {
        if (dat->exit_thd)
            break;

        if ((err = gsensor_read(acc))) {
            FTGLOGE("gsensor_update_info() = (%s), %d\n", strerror(errno), err);
            break;
        } else if ((err = gsensor_check_tilt(acc))) {
            FTGLOGE("gsensor_check_tilt() = (%s), %d\n", strerror(errno), err);
            break;
        } else if (acc->tilt >= TILT_MAX) {
            FTGLOGE("invalid tilt = %d\n", acc->tilt);
            break;
        } else if (TILT_UNKNOWN != acc->tilt) {
            acc->pos_chk |= (1 << acc->tilt);
        }

        len = 0;
#ifndef FEATURE_FTM_TWO_KEY
        len = snprintf(dat->info + len, sizeof(dat->info) - len, "%+6.3f %+6.3f %+6.3f\n%s (%s)\n%s: %s %s: %s\n%s: %s %s: %s\n%s: %s %s: %s\n",
                acc->evt.x, acc->evt.y, acc->evt.z,
                (acc->tilt != TILT_UNKNOWN) ? (uistr_info_sensor_pass) : ("NG"), gsensor_pos[acc->tilt],
                gsensor_pos[TILT_X_POS], (acc->pos_chk & (1 << TILT_X_POS)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_X_NEG], (acc->pos_chk & (1 << TILT_X_NEG)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Y_POS], (acc->pos_chk & (1 << TILT_Y_POS)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Y_NEG], (acc->pos_chk & (1 << TILT_Y_NEG)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Z_POS], (acc->pos_chk & (1 << TILT_Z_POS)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Z_NEG], (acc->pos_chk & (1 << TILT_Z_NEG)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing));

        if (len < 0) {
            LOGE(TAG "%s: snprintf error \n", __FUNCTION__);
            len = 0;
        }

        len += snprintf(dat->info + len, sizeof(dat->info) - len, "%s %s\n%s: %+6.3f %+6.3f %+6.3f\n%s: %+6.3f %+6.3f %+6.3f\n%s-%s:%+6.3f %+6.3f %+6.3f\n%s: %+6.3f %+6.3f %+6.3f\n%s: %+6.3f %+6.3f %+6.3f\n", uistr_info_g_sensor_statistic,
                (!acc->statistics) ? (uistr_info_g_sensor_doing) : (uistr_info_g_sensor_done),
                uistr_info_g_sensor_max,
                acc->max[0], acc->max[1], acc->max[2],
                uistr_info_g_sensor_min,
                acc->min[0], acc->min[1], acc->min[2],
                uistr_info_g_sensor_max, uistr_info_g_sensor_min,
                acc->max[0] - acc->min[0], acc->max[1] - acc->min[1], acc->max[2] - acc->min[2],
                uistr_info_g_sensor_avg,
                acc->avg[0], acc->avg[1], acc->avg[2],
                uistr_info_g_sensor_std,
                acc->std[0], acc->std[1], acc->std[2]);
        if (len < 0) {
            LOGE(TAG "%s: snprintf error \n", __FUNCTION__);
            len = 0;
        }
#else //shorten the string
        len = snprintf(dat->info + len, sizeof(dat->info) - len, "%+6.3f %+6.3f %+6.3f\n%s (%s)\n%s: %s %s: %s\n%s: %s %s: %s\n%s: %s %s: %s\n",
                acc->evt.x, acc->evt.y, acc->evt.z,
                (acc->tilt != TILT_UNKNOWN) ? (uistr_info_sensor_pass) : ("NG"), gsensor_pos[acc->tilt],
                gsensor_pos[TILT_X_POS], (acc->pos_chk & (1 << TILT_X_POS)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_X_NEG], (acc->pos_chk & (1 << TILT_X_NEG)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Y_POS], (acc->pos_chk & (1 << TILT_Y_POS)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Y_NEG], (acc->pos_chk & (1 << TILT_Y_NEG)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Z_POS], (acc->pos_chk & (1 << TILT_Z_POS)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing),
                gsensor_pos[TILT_Z_NEG], (acc->pos_chk & (1 << TILT_Z_NEG)) ? (uistr_info_sensor_pass) : (uistr_info_g_sensor_testing));

        len += snprintf(dat->info + len, sizeof(dat->info) - len, "%s%+6.3f %+6.3f %+6.3f\n%s:%+6.3f %+6.3f %+6.3f\n%s:%+6.3f %+6.3f %+6.3f\n%s:%+6.3f %+6.3f %+6.3f\n",
                uistr_info_g_sensor_max, acc->max[0], acc->max[1], acc->max[2],
                uistr_info_g_sensor_min, acc->min[0], acc->min[1], acc->min[2],
                uistr_info_g_sensor_avg, acc->avg[0], acc->avg[1], acc->avg[2],
                uistr_info_g_sensor_std, acc->std[0], acc->std[1], acc->std[2]);

        if (len < 0) {
            LOGE(TAG "%s: snprintf error \n", __FUNCTION__);
            len = 0;
        }

#endif
        //len += snprintf(dat->info+len, sizeof(dat->info)-len, uistr_info_g_sensor_range);

        iv->set_text(iv, &dat->text);
        iv->redraw(iv);

        /**
        pthread_mutex_lock (&gsensor_mutex);
        if(sp_ata_status == FTM_AUTO_ITEM)
        {
            gsensor_thread_exit = true;
            pthread_mutex_unlock (&gsensor_mutex);
            break;
        }
        pthread_mutex_unlock (&gsensor_mutex);
        **/
    }
    gsensor_close(acc);
    return NULL;
}

int gsensor_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    struct acc_data *dat = (struct acc_data *)priv;
    struct itemview *iv;
    int len;
    unsigned char usleep_count = 0;

    LOGD(TAG "%s\n", __FUNCTION__);

    init_text(&dat->title, param->name, COLOR_YELLOW);
    init_text(&dat->text, &dat->info[0], COLOR_YELLOW);
    init_text(&dat->left_btn, uistr_info_sensor_fail, COLOR_YELLOW);
    init_text(&dat->center_btn, uistr_info_sensor_pass, COLOR_YELLOW);
    init_text(&dat->right_btn, uistr_info_sensor_back, COLOR_YELLOW);
    dat->exit_thd = false;

    len = snprintf(dat->info, sizeof(dat->info), "%s\n", uistr_info_sensor_initializing);
    dat->exit_thd = false;
    gsensor_thread_exit = false;

    if (!dat->iv) {
        iv = ui_new_itemview();
        if (!iv) {
            LOGD(TAG "No memory");
            return -1;
        }
        dat->iv = iv;
    }
    iv = dat->iv;
    iv->set_title(iv, &dat->title);
    iv->set_items(iv, gsensor_items, 0);
    iv->set_text(iv, &dat->text);
    sp_ata_status = param->test_type;
    pthread_create(&dat->update_thd, NULL, gsensor_update_iv_thread, priv);
    do {
        if (sp_ata_status == FTM_MANUAL_ITEM) {
#ifdef FEATURE_FTM_TOUCH_MODE
            text_t lbtn;
            text_t cbtn;
            text_t rbtn;

            init_text(&lbtn, uistr_key_fail, COLOR_YELLOW);
            init_text(&cbtn, uistr_key_back, COLOR_YELLOW);
            init_text(&rbtn, uistr_key_pass, COLOR_YELLOW);

            iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
            chosen = iv->run(iv, &gsensor_thread_exit);
            switch (chosen) {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    dat->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    dat->mod->test_result = FTM_TEST_FAIL;
                }
                gsensor_thread_exit = true;
                break;
#else /* FEATURE_FTM_TOUCH_MODE */
            case L_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_FAIL;
                gsensor_thread_exit = true;
                break;
            case C_BTN_DOWN:
                gsensor_thread_exit = true;
                break;
            case R_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_PASS;
                gsensor_thread_exit = true;
                break;
#endif /* FEATURE_FTM_TOUCH_MODE */
            }
        }
        iv->redraw(iv);
        pthread_mutex_lock (&gsensor_mutex);
        if (gsensor_thread_exit || usleep_count == 100) {
            dat->exit_thd = true;
            pthread_mutex_unlock (&gsensor_mutex);
            break;
        } else {
            usleep_count++;
            pthread_mutex_unlock(&gsensor_mutex);
            usleep(50000);
        }
    } while (1);

    pthread_join(dat->update_thd, NULL);
    LOGD(TAG "gsensor entry exit\n");
    return 0;
}

int gsensor_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct acc_data *dat;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_GSENSOR, sizeof(struct acc_data));
    dat  = mod_to_acc_data(mod);

    memset(dat, 0x00, sizeof(*dat));
    gsensor_init_priv(&dat->acc);

    /*NOTE: the assignment MUST be done, or exception happens when tester press Test Pass/Test Fail*/
    dat->mod = mod;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, gsensor_entry, (void*)dat);

    return ret;
}
#endif

