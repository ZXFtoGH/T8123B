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

#ifdef FEATURE_FTM_BAROMETER
#include <linux/sensors_io.h>

static pthread_mutex_t baro_sensor_mutex = PTHREAD_MUTEX_INITIALIZER;
static int sp_ata_status = 0;

/******************************************************************************
 * MACRO
 *****************************************************************************/
#define TAG "[BARO] "
#define mod_to_baro_data(p) (struct baro_data*)((char*)(p) + sizeof(struct ftm_module))
#define FBLOGD(fmt, arg ...) LOGD(TAG fmt, ##arg)
#define FBLOGE(fmt, arg ...) LOGE("%s [%5d]: " fmt, __func__, __LINE__, ##arg)

#define INVALID_T_VAL -273.150 //0K = -273.15
#define INVALID_P_VAL -1.0*100
#define MAX_P_VAL 1013.25*1.2
#define MIN_P_VAL 1013.25*0.8
#define SAMPLE_RATE_NS 200000000
#define TIMEOUT_MS 500

/******************************************************************************
 * Structure
 *****************************************************************************/
enum {
    ITEM_PASS,
    ITEM_FAIL,
};

static item_t barometer_items[] = {
#ifndef FEATURE_FTM_TOUCH_MODE
    item(ITEM_PASS,   "Test Pass"),
    item(ITEM_FAIL,   "Test Fail"),
#endif
    item(-1, NULL),
};

struct baro_priv {
    /*specific data field*/
    void *hf_manager;
    void *hf_looper;
    float temp_raw;
    float press_raw;
    unsigned int sample_count;
    bool press_constant;
};

struct baro_data {
    struct baro_priv baro;

    /*common for each factory mode*/
    char info[1024];
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

/******************************************************************************
 * Functions
 *****************************************************************************/
static int barometer_init_priv(struct baro_priv *baro)
{
    memset(baro, 0x00, sizeof(*baro));
    baro->temp_raw = INVALID_T_VAL;
    baro->press_raw = INVALID_P_VAL;
    baro->sample_count = 0;
    baro->press_constant = true;
    return 0;
}

static int barometer_open(struct baro_priv *baro)
{
    baro->hf_manager = HfManagerCreate();
    baro->hf_looper = HfLooperCreate(HfManagerGetFd(baro->hf_manager), 64);

    if (HfManagerFindSensor(baro->hf_manager, SENSOR_TYPE_PRESSURE) < 0) {
        LOGD(TAG "find press sensor fail\n");
        goto err_out;
    }

    HfManagerEnableSensor(baro->hf_manager, SENSOR_TYPE_PRESSURE, SAMPLE_RATE_NS, 0);
    HfManagerEnableRawData(baro->hf_manager, SENSOR_TYPE_PRESSURE);
    return 0;

err_out:
    HfLooperDestroy(baro->hf_looper);
    HfManagerDestroy(baro->hf_manager);
    return -1;
}

static int barometer_close(struct baro_priv *baro)
{
    HfManagerDisableRawData(baro->hf_manager, SENSOR_TYPE_PRESSURE);
    HfManagerDisableSensor(baro->hf_manager, SENSOR_TYPE_PRESSURE);

    HfLooperDestroy(baro->hf_looper);
    HfManagerDestroy(baro->hf_manager);

    memset(baro, 0x00, sizeof(*baro));
    return 0;
}

static int barometer_read(struct baro_data *dat)
{
    sensors_event_t data[32] = {0};
    struct baro_priv *baro = &dat->baro;
    float press_raw_data_temp = 0;

    int err = HfLooperEventLooperTimeout(baro->hf_looper, data, 32, TIMEOUT_MS);

    if (err < 0) {
        LOGE(TAG "Hf looper event looper retrun value:%d\n", err);
        return err;
    }

    for (int i = 0; i < err; ++i) {
        if (data[i].reserved0 == RAW_ACTION) {
            baro->press_raw = (float)(data[i].data[0]);

            if (sp_ata_status == FTM_AUTO_ITEM) {
                baro->sample_count++;
                if (baro->sample_count >= (1E9 / SAMPLE_RATE_NS)) {
                    if (baro->sample_count % ((unsigned int)(1E9 / SAMPLE_RATE_NS)) == 0)
                        press_raw_data_temp = baro->press_raw;

                    dat->mod->test_result = FTM_TEST_PASS;

                    if (dat->baro.press_raw > MAX_P_VAL || dat->baro.press_raw < MIN_P_VAL) {
                        LOGE(TAG "Error: Pressure = %.4f out of range", dat->baro.press_raw);
                        dat->mod->test_result = FTM_TEST_FAIL;
                        dat->exit_thd = true;
                        break;
                    }

                    if (dat->baro.press_raw != press_raw_data_temp)
                        baro->press_constant = false;
                }
            }
        }
    }
    return 0;

}

static void *barometer_update_iv_thread(void *priv)
{
    struct baro_data *dat = (struct baro_data *)priv;
    struct baro_priv *baro = &dat->baro;
    struct itemview *iv = dat->iv;
    int err = 0, len = 0;

    LOGD(TAG "%s: Start\n", __FUNCTION__);

    if ((err = barometer_open(baro))) {
        memset(dat->info, 0x00, sizeof(dat->info));
        sprintf(dat->info, "INIT FAILED\n");
        iv->redraw(iv);
        FBLOGE("barometer() err = %d(%s)\n", err, dat->info);
        return NULL;
    }

    while (1) {
        if (dat->exit_thd) {
            if (baro->press_constant == true &&
                sp_ata_status == FTM_AUTO_ITEM)
                dat->mod->test_result = FTM_TEST_FAIL;
            break;
        }

        if ((err = barometer_read(dat)))
            continue;

        len = 0;
        len += snprintf(dat->info+len, sizeof(dat->info)-len, "TEMP: %f \n", baro->temp_raw);
        len += snprintf(dat->info+len, sizeof(dat->info)-len, "PRESS : %f\n", baro->press_raw);
        iv->set_text(iv, &dat->text);
        iv->redraw(iv);
    }
    barometer_close(baro);
    return NULL;
}

int barometer_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    bool baro_thread_exit = false;
    struct baro_data *dat = (struct baro_data *)priv;
    struct itemview *iv;
    unsigned char usleep_count = 0;

    LOGD(TAG "%s\n", __FUNCTION__);

    init_text(&dat->title, param->name, COLOR_YELLOW);
    init_text(&dat->text, &dat->info[0], COLOR_YELLOW);
    init_text(&dat->left_btn, "Fail", COLOR_YELLOW);
    init_text(&dat->center_btn, "Pass", COLOR_YELLOW);
    init_text(&dat->right_btn, "Back", COLOR_YELLOW);
    dat->exit_thd = false;
    dat->mod->test_result = FTM_TEST_FAIL;
    snprintf(dat->info, sizeof(dat->info), "Initializing...\n");

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
    iv->set_items(iv, barometer_items, 0);
    iv->set_text(iv, &dat->text);
#ifdef FEATURE_FTM_TOUCH_MODE
    text_t lbtn;
    text_t cbtn;
    text_t rbtn;
    init_text(&lbtn, uistr_key_fail, COLOR_YELLOW);
    init_text(&cbtn, uistr_key_back, COLOR_YELLOW);
    init_text(&rbtn, uistr_key_pass, COLOR_YELLOW);
    iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif

    sp_ata_status = param->test_type;
    pthread_create(&dat->update_thd, NULL, barometer_update_iv_thread, priv);
    do {
        if (sp_ata_status == FTM_MANUAL_ITEM) {
            chosen = iv->run(iv, &baro_thread_exit);
            switch (chosen) {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    dat->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    dat->mod->test_result = FTM_TEST_FAIL;
                }
                baro_thread_exit = true;
                break;
#else
            case L_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_FAIL;
                baro_thread_exit = true;
                break;
            case C_BTN_DOWN:
                baro_thread_exit = true;
                break;
            case R_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_PASS;
                baro_thread_exit = true;
                break;
#endif
            }

        }

        pthread_mutex_lock(&baro_sensor_mutex);
        if (baro_thread_exit || usleep_count == 100
            || dat->baro.sample_count >= (3E9 / SAMPLE_RATE_NS)) {
            dat->exit_thd = true;
            pthread_mutex_unlock(&baro_sensor_mutex);
            break;
        } else {
            usleep_count++;
            pthread_mutex_unlock(&baro_sensor_mutex);
            usleep(50000);
        }
    } while (1);

    pthread_join(dat->update_thd, NULL);
    return 0;
}

int barometer_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct baro_data *dat;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_BAROMETER, sizeof(struct baro_data));
    dat  = mod_to_baro_data(mod);

    memset(dat, 0x00, sizeof(*dat));
    barometer_init_priv(&dat->baro);

    /*NOTE: the assignment MUST be done,
    or exception happens when tester press Test Pass/Test Fail*/
    dat->mod = mod;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, barometer_entry, (void*)dat);

    return ret;
}
#endif

