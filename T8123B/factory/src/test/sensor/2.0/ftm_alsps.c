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
#include <unistd.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include <dirent.h>
#include <linux/input.h>
#include <math.h>

#include "common.h"
#include "miniui.h"
#include "ftm.h"
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <HfManagerWrapper.h>
#include <hardware/sensors.h>
#include "HfSensorType.h"

#ifdef FEATURE_FTM_ALSPS

#include <linux/sensors_io.h>

bool alsps_thread_exit = false;
static pthread_mutex_t alsps_mutex = PTHREAD_MUTEX_INITIALIZER;
static int sp_ata_status = 0;

extern sp_ata_data return_data;

/******************************************************************************
 * MACRO
 *****************************************************************************/
#define TAG "[LPS] "
#define mod_to_lps_data(p) (struct lps_data*)((char*)(p) + sizeof(struct ftm_module))
#define FLPLOGD(fmt, arg ...) LOGD(TAG fmt, ##arg)
#define FLPLOGE(fmt, arg ...) LOGE("%s [%5d]: " fmt, __func__, __LINE__, ##arg)
#define SENSOR_SAMPLE_PERIOD_NS 200000000
#define TIMEOUT_MS 500

/******************************************************************************
 * Structure
 *****************************************************************************/
enum {
    ITEM_PASS,
    ITEM_FAIL,
};

static item_t alsps_items[] = {
#ifndef FEATURE_FTM_TOUCH_MODE
    item(ITEM_PASS,   uistr_pass),
    item(ITEM_FAIL,   uistr_fail),
#endif
    item(-1, NULL),
};

struct lps_priv {
    /*specific data field*/
    void *hf_manager;
    void *hf_looper;
    unsigned int als_raw;
    unsigned int ps_raw;
    unsigned int ps_threshold_value;
    unsigned int ps_threshold_high;
    unsigned int ps_threshold_low;
    unsigned int als_threshold_value;
    unsigned int als_threshold_high;
    unsigned int als_threshold_low;
};

struct lps_data {
    struct lps_priv lps;

    /*common for each factory mode*/
    char  info[1024];
    bool exit_thd;
    text_t    title;
    text_t    text;
    text_t    left_btn;
    text_t    center_btn;
    text_t    right_btn;

    pthread_t update_thd;
    struct ftm_module *mod;
    //struct textview tv;
    struct itemview *iv;
};

/******************************************************************************
 * Functions
 *****************************************************************************/
static int alsps_init_priv(struct lps_priv *lps)
{
    memset(lps, 0x00, sizeof(*lps));
    return 0;
}

static int alsps_open(struct lps_priv *lps)
{
    lps->hf_manager = HfManagerCreate();
    lps->hf_looper = HfLooperCreate(HfManagerGetFd(lps->hf_manager), 64);

    if (HfManagerFindSensor(lps->hf_manager, SENSOR_TYPE_LIGHT) < 0) {
        LOGD(TAG "find lsensor fail\n");
        goto err_out;
    } else {
        HfManagerEnableSensor(lps->hf_manager, SENSOR_TYPE_LIGHT, SENSOR_SAMPLE_PERIOD_NS, 0);
        HfManagerEnableRawData(lps->hf_manager, SENSOR_TYPE_LIGHT);
    }

    if (HfManagerFindSensor(lps->hf_manager, SENSOR_TYPE_PROXIMITY) < 0) {
        LOGD(TAG "find psensor fail\n");
        goto err_out;
    } else {
        HfManagerEnableSensor(lps->hf_manager, SENSOR_TYPE_PROXIMITY, SENSOR_SAMPLE_PERIOD_NS, 0);
        HfManagerEnableRawData(lps->hf_manager, SENSOR_TYPE_PROXIMITY);
    }

    return 0;

err_out:
        HfLooperDestroy(lps->hf_looper);
        HfManagerDestroy(lps->hf_manager);
        return -1;
}

static int alsps_close(struct lps_priv *lps)
{
    HfManagerDisableRawData(lps->hf_manager, SENSOR_TYPE_LIGHT);
    HfManagerDisableSensor(lps->hf_manager, SENSOR_TYPE_LIGHT);
    HfManagerDisableRawData(lps->hf_manager, SENSOR_TYPE_PROXIMITY);
    HfManagerDisableSensor(lps->hf_manager, SENSOR_TYPE_PROXIMITY);

    HfLooperDestroy(lps->hf_looper);
    HfManagerDestroy(lps->hf_manager);

    memset(lps, 0x00, sizeof(*lps));
    return 0;
}

static int alsps_read(struct lps_priv *lps)
{
    sensors_event_t data[32] = {0};

    int err = HfLooperEventLooperTimeout(lps->hf_looper, data, 32, TIMEOUT_MS);

    if (err < 0) {
        LOGE(TAG "Hf looper event looper retrun value:%d\n", err);
        return err;
    }

    for (int i = 0; i < err; ++i) {
        if (data[i].reserved0 == RAW_ACTION) {
            if (data[i].sensor == SENSOR_TYPE_LIGHT) {
                lps->als_raw = (float)(data[i].data[0]);

                // add sensor data to struct sp_ata data for PC side
                return_data.alsps.als = lps->als_raw;
            } else if (data[i].sensor == SENSOR_TYPE_PROXIMITY) {
                lps->ps_raw = (float)(data[i].data[0]);

                // add sensor data to struct sp_ata data for PC side
                return_data.alsps.ps = lps->ps_raw;
            }
        } else if (data[i].reserved0 == DATA_ACTION && data[i].sensor == SENSOR_TYPE_PROXIMITY)
            lps->ps_threshold_value = (float)data[i].data[0];
    }

    return 0;
}

static void *alsps_update_iv_thread(void *priv)
{
    struct lps_data *dat = (struct lps_data *)priv;
    struct lps_priv *lps = &dat->lps;
    struct itemview *iv = dat->iv;
    int err = 0, len = 0;

    LOGD(TAG "%s: Start\n", __FUNCTION__);
    if ((err = alsps_open(lps))) {
        memset(dat->info, 0x00, sizeof(dat->info));
        sprintf(dat->info, "%s", uistr_info_sensor_init_fail);
        iv->redraw(iv);
        FLPLOGE("alsps() err = %d(%s)\n", err, dat->info);
        return NULL;
    }

    while (1) {
        if (dat->exit_thd)
            break;

        if ((err = alsps_read(lps)))
            continue;

        len = 0;
        len += snprintf(dat->info+len, sizeof(dat->info)-len, "ALS: %4Xh (0:dark; +:bright) \n", lps->als_raw);
        len += snprintf(dat->info+len, sizeof(dat->info)-len, "PS : %4Xh (0:far ; +:close) \n", lps->ps_raw);
        if (lps->ps_threshold_high == 0) {
            len += snprintf(dat->info+len, sizeof(dat->info)-len, "%s: %s\n", uistr_info_sensor_alsps_thres_high,
                    uistr_info_sensor_alsps_check_command);
        } else {
            len += snprintf(dat->info+len, sizeof(dat->info)-len, "%s: %4Xh\n", uistr_info_sensor_alsps_thres_high,
                    lps->ps_threshold_high);
        }

        if (lps->ps_threshold_low == 0) {
            len += snprintf(dat->info+len, sizeof(dat->info)-len, "%s: %s\n", uistr_info_sensor_alsps_thres_low,
                    uistr_info_sensor_alsps_check_command);
        } else {
            len += snprintf(dat->info+len, sizeof(dat->info)-len, "%s: %4Xh\n", uistr_info_sensor_alsps_thres_low,
                    lps->ps_threshold_low);
        }

        if (lps->ps_threshold_value == 2) {
            len += snprintf(dat->info+len, sizeof(dat->info)-len, "%s: %s\n", uistr_info_sensor_alsps_result,
                    uistr_info_sensor_alsps_check_command);
        } else {
            len += snprintf(dat->info+len, sizeof(dat->info)-len, "%s: %s\n", uistr_info_sensor_alsps_result,
                    (lps->ps_threshold_value == 0)?uistr_info_sensor_fail:uistr_info_sensor_pass);
        }

        iv->set_text(iv, &dat->text);
        iv->redraw(iv);

        /**
        pthread_mutex_lock (&alsps_mutex);
        if (sp_ata_status == FTM_AUTO_ITEM)
        {
            alsps_thread_exit = true;
            pthread_mutex_unlock (&alsps_mutex);
            break;
        }
        pthread_mutex_unlock (&alsps_mutex);
        **/
    }

    alsps_close(lps);
    return NULL;
}
/*---------------------------------------------------------------------------*/
int alsps_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    struct lps_data *dat = (struct lps_data *)priv;
    struct itemview *iv;
    unsigned char usleep_count = 0;

    LOGD(TAG "%s\n", __FUNCTION__);

    init_text(&dat->title, param->name, COLOR_YELLOW);
    init_text(&dat->text, &dat->info[0], COLOR_YELLOW);
    init_text(&dat->left_btn, uistr_info_sensor_fail, COLOR_YELLOW);
    init_text(&dat->center_btn, uistr_info_sensor_pass, COLOR_YELLOW);
    init_text(&dat->right_btn, uistr_info_sensor_back, COLOR_YELLOW);
    dat->exit_thd = false;

    snprintf(dat->info, sizeof(dat->info), "%s", uistr_info_sensor_initializing);
    alsps_thread_exit = false;

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
    iv->set_items(iv, alsps_items, 0);
    iv->set_text(iv, &dat->text);
    sp_ata_status = param->test_type;
    pthread_create(&dat->update_thd, NULL, alsps_update_iv_thread, priv);
    do {
        if (sp_ata_status == FTM_MANUAL_ITEM) {
#ifdef FEATURE_FTM_TOUCH_MODE
            text_t lbtn;
            text_t cbtn;
            text_t rbtn;

            init_text(&lbtn,uistr_key_fail, COLOR_YELLOW);
            init_text(&cbtn,uistr_key_back, COLOR_YELLOW);
            init_text(&rbtn,uistr_key_pass, COLOR_YELLOW);

            iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
            chosen = iv->run(iv, &alsps_thread_exit);
            switch (chosen) {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    dat->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    dat->mod->test_result = FTM_TEST_FAIL;
                }
                alsps_thread_exit = true;
                break;
#else /* FEATURE_FTM_TOUCH_MODE */
            case L_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_FAIL;
                alsps_thread_exit = true;
                break;
            case C_BTN_DOWN:
                alsps_thread_exit = true;
                break;
            case R_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_PASS;
                alsps_thread_exit = true;
                break;
#endif /* FEATURE_FTM_TOUCH_MODE */
            }
        }
        iv->redraw(iv);
        pthread_mutex_lock(&alsps_mutex);

        if (alsps_thread_exit || usleep_count == 100) {
            dat->exit_thd = true;
            pthread_mutex_unlock (&alsps_mutex);
            break;
        } else {
            usleep_count++;
            pthread_mutex_unlock(&alsps_mutex);
            usleep(50000);
        }
    } while (1);

    pthread_join(dat->update_thd, NULL);

    return 0;
}
/*---------------------------------------------------------------------------*/
int alsps_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct lps_data *dat;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_ALSPS, sizeof(struct lps_data));
    dat  = mod_to_lps_data(mod);

    memset(dat, 0x00, sizeof(*dat));
    alsps_init_priv(&dat->lps);

    /*NOTE: the assignment MUST be done, or exception happens when tester press Test Pass/Test Fail*/
    dat->mod = mod;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, alsps_entry, (void*)dat);

    return ret;
}
#endif

