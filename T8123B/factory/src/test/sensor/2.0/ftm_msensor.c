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

#ifdef FEATURE_FTM_MAGNETOMETER
#include <linux/sensors_io.h>

/******************************************************************************
 * global variable
 *****************************************************************************/
bool msensor_thread_exit = false;
static pthread_mutex_t msensor_mutex = PTHREAD_MUTEX_INITIALIZER;
static int sp_ata_status = 0;
extern sp_ata_data return_data;

/******************************************************************************
 * MACRO
 *****************************************************************************/
#define TAG "[MAG] "
#define mod_to_mag_data(p) (struct mag_data*)((char*)(p) + sizeof(struct ftm_module))
#define FTMLOGD(fmt, arg ...) LOGD(TAG fmt, ##arg)
#define FTMLOGE(fmt, arg ...) LOGE(TAG fmt, ##arg)
#define SENSOR_SAMPLE_PERIOD_NS 200000000
#define TIMEOUT_MS 500
/******************************************************************************
 * Structure
 *****************************************************************************/
enum {
    ITEM_PASS,
    ITEM_FAIL,
};

static item_t msensor_items[] = {
#ifndef FEATURE_FTM_TOUCH_MODE
    item(ITEM_PASS,   uistr_pass),
    item(ITEM_FAIL,   uistr_fail),
#endif
    item(-1, NULL),
};

struct mag_evt {
    int x;
    int y;
    int z;
    int yaw;
    int pitch;
    int roll;
    unsigned int status;
    unsigned int div;
    //int64_t time;
};

struct mag_priv {
    /*specific data field*/
    void *hf_manager;
    void *hf_looper;
    struct mag_evt evt;
};

struct mag_data {
    struct mag_priv mag;
    bool exit_thd;
    /*common for each factory mode*/
    char  info[1024];

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
 * common interface
 *****************************************************************************/
static int msensor_open(struct mag_priv *mag)
{
    mag->hf_manager = HfManagerCreate();
    mag->hf_looper = HfLooperCreate(HfManagerGetFd(mag->hf_manager), 64);

    if (HfManagerFindSensor(mag->hf_manager, SENSOR_TYPE_MAGNETIC_FIELD) < 0) {
        LOGD(TAG "find msensor fail\n");
        goto err_out;
    }

    HfManagerEnableSensor(mag->hf_manager, SENSOR_TYPE_MAGNETIC_FIELD, SENSOR_SAMPLE_PERIOD_NS, 0);
    HfManagerEnableRawData(mag->hf_manager, SENSOR_TYPE_MAGNETIC_FIELD);
    return 0;

err_out:
    HfLooperDestroy(mag->hf_looper);
    HfManagerDestroy(mag->hf_manager);
    return -1;
}

static int msensor_close(struct mag_priv *mag)
{
    HfManagerDisableRawData(mag->hf_manager, SENSOR_TYPE_MAGNETIC_FIELD);
    HfManagerDisableSensor(mag->hf_manager, SENSOR_TYPE_MAGNETIC_FIELD);

    HfLooperDestroy(mag->hf_looper);
    HfManagerDestroy(mag->hf_manager);

    memset(mag, 0x00, sizeof(*mag));
    return 0;
}

static int msensor_read(struct mag_priv *mag)
{
    sensors_event_t data[32] = {0};

    int err = HfLooperEventLooperTimeout(mag->hf_looper, data, 32, TIMEOUT_MS);

    if (err < 0) {
        LOGE(TAG "Hf looper event looper retrun value:%d\n", err);
        return err;
    }

    for (int i = 0; i < err; ++i) {
        if (data[i].reserved0 == RAW_ACTION) {
            mag->evt.x = (float)(data[i].data[0]);
            mag->evt.y = (float)(data[i].data[1]);
            mag->evt.z = (float)(data[i].data[2]);

            // add sensor data to struct sp_ata data for PC side
            return_data.msensor.m_sensor_x = mag->evt.x;
            return_data.msensor.m_sensor_y = mag->evt.y;
            return_data.msensor.m_sensor_z = mag->evt.z;
            return_data.msensor.accuracy = 3;
        }
    }

    return 0;
}

static void msensor_init_priv(struct mag_priv *mag)
{
    memset(mag, 0x00, sizeof(*mag));
}

static void *msensor_update_iv_thread(void *priv)
{
    struct mag_data *dat = (struct mag_data *)priv;
    struct mag_priv *mag = &dat->mag;
    struct itemview *iv = dat->iv;
    int err = 0, len;
    char* status = uistr_info_m_sensor_notsupport;

    LOGD(TAG "%s: Start\n", __FUNCTION__);

    if ((err = msensor_open(mag))) {
        memset(dat->info, 0x00, sizeof(dat->info));
        sprintf(dat->info, "INIT FAILED\n");
        iv->redraw(iv); /*force to print the log*/
        FTMLOGD("msensor_open() err = %d (%s)\n", err, dat->info);
        return NULL;
    }

    while (1) {
        if (dat->exit_thd)
            break;

        if ((err = msensor_read(mag))) {
            FTMLOGE("msensor_update_info() = (%s), %d\n", strerror(errno), err);
            break;
        }

        len = 0;
        len += snprintf(dat->info+len, sizeof(dat->info)+len,  "%s: %s \n",uistr_info_m_sensor_self, status);
        len += snprintf(dat->info+len, sizeof(dat->info)+len, "%s: %d\n",uistr_info_m_sensor_status, mag->evt.status);
        //len += snprintf(dat->info+len, sizeof(dat->info)+len, "Yaw: %d\nPietch: %d\nRoll: %d\n",
        //  mag->evt.yaw,mag->evt.pitch,mag->evt.roll);
        len += snprintf(dat->info+len, sizeof(dat->info)+len, "%s: \n", uistr_info_m_sensor_data);
        len += snprintf(dat->info+len, sizeof(dat->info)+len, "X: %d\nY: %d\nZ: %d\n",
                mag->evt.x,mag->evt.y,mag->evt.z);

        iv->set_text(iv, &dat->text);
        iv->redraw(iv);

        /**
        pthread_mutex_lock (&msensor_mutex);
        if(sp_ata_status == FTM_AUTO_ITEM)
        {
            msensor_thread_exit = true;
            pthread_mutex_unlock (&msensor_mutex);
            break;
        }
        pthread_mutex_unlock (&msensor_mutex);
        **/
    }
    msensor_close(mag);
    return NULL;
}

int msensor_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    struct mag_data *dat = (struct mag_data *)priv;
    struct itemview *iv;
    int err;
    unsigned char usleep_count = 0;

    LOGD(TAG "%s\n", __FUNCTION__);

    init_text(&dat->title, param->name, COLOR_YELLOW);
    init_text(&dat->text, &dat->info[0], COLOR_YELLOW);
    init_text(&dat->left_btn, "Fail", COLOR_YELLOW);
    init_text(&dat->center_btn, "Pass", COLOR_YELLOW);
    init_text(&dat->right_btn, "Back", COLOR_YELLOW);

    snprintf(dat->info, sizeof(dat->info), "Initializing...\n");
    dat->exit_thd = false;
    msensor_thread_exit = false;

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
    iv->set_items(iv, msensor_items, 0);
    iv->set_text(iv, &dat->text);
    sp_ata_status = param->test_type;
    pthread_create(&dat->update_thd, NULL, msensor_update_iv_thread, priv);

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
            chosen = iv->run(iv, &msensor_thread_exit);
            LOGD(TAG "chosen value:%d\n", chosen);
            switch (chosen) {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    dat->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    dat->mod->test_result = FTM_TEST_FAIL;
                }
                msensor_thread_exit = true;
                break;
#else // FEATURE_FTM_TOUCH_MODE
            case L_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_FAIL;
                msensor_thread_exit = true;
                break;
            case C_BTN_DOWN:
                msensor_thread_exit = true;
                break;
            case R_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_PASS;
                msensor_thread_exit = true;
                break;
#endif // FEATURE_FTM_TOUCH_MODE
            }
        }
        iv->redraw(iv);
        pthread_mutex_lock(&msensor_mutex);

        if (msensor_thread_exit || usleep_count == 100) {
            dat->exit_thd = true;
            pthread_mutex_unlock(&msensor_mutex);
            break;
        } else {
            usleep_count++;
            pthread_mutex_unlock(&msensor_mutex);
            usleep(50000);
        }
    } while (1);

    pthread_join(dat->update_thd, NULL);
    LOGD(TAG "msensor mainThread exit\n");
    return 0;
}

int msensor_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct mag_data *dat;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_MSENSOR, sizeof(struct mag_data));
    dat = mod_to_mag_data(mod);

    memset(dat, 0x00, sizeof(*dat));
    msensor_init_priv(&dat->mag);

    /*NOTE: the assignment MUST be done, or exception happens when tester press Test Pass/Test Fail*/
    dat->mod = mod;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, msensor_entry, (void*)dat);

    return ret;
}
#endif

