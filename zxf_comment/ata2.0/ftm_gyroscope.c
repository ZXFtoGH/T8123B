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

#ifdef FEATURE_FTM_GYROSCOPE
#include <linux/sensors_io.h>
#include <linux/hwmsensor.h>

/******************************************************************************
 * MACRO
 *****************************************************************************/
#define TAG "[Gyroscope]"
#define mod_to_gyro_data(p) (struct gyro_data*)((char*)(p) + sizeof(struct ftm_module))
#define FGYROLOGD(fmt, arg ...) LOGD(TAG fmt, ##arg)
#define FGYROLOGE(fmt, arg ...) LOGE("%s [%5d]: " fmt, __func__, __LINE__, ##arg)
#define SENSOR_SAMPLE_PERIOD_NS 200000000
#define TIMEOUT_MS 500

/******************************************************************************
 * global variable
 *****************************************************************************/
bool gyroscope_thread_exit = false;
static int sp_ata_status = 0;
static pthread_mutex_t gyro_mutex = PTHREAD_MUTEX_INITIALIZER;
extern sp_ata_data return_data;

/******************************************************************************
 * Structure
 *****************************************************************************/
enum {
    ITEM_PASS,
    ITEM_FAIL,
};

static item_t gyro_items[] = {
#ifndef FEATURE_FTM_TOUCH_MODE
    item(ITEM_PASS,   uistr_pass),
    item(ITEM_FAIL,   uistr_fail),
#endif
    item(-1, NULL),
};

struct gyro_priv {
    /*specific data field*/
    void *hf_manager;
    void *hf_looper;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    int resultcode;
    int smtflag;
    int smtRes;
};

struct gyro_data {
    struct gyro_priv gyro;

    /*common for each factory mode*/
    char  info[1024];
    bool  avail;
    bool  exit_thd;

    text_t    title;
    text_t    text;
    text_t    left_btn;
    text_t    center_btn;
    text_t    right_btn;

    pthread_t update_thd;
    struct ftm_module *mod;
    struct textview tv;
    struct itemview *iv;
};

/******************************************************************************
 * Functions
 *****************************************************************************/
static int gyro_init_priv(struct gyro_priv *gyro)
{
    memset(gyro, 0x00, sizeof(*gyro));
    return 0;
}

static int gyro_open(struct gyro_priv *gyro)
{
    gyro->hf_manager = HfManagerCreate();
    gyro->hf_looper = HfLooperCreate(HfManagerGetFd(gyro->hf_manager), 64);

    if (HfManagerFindSensor(gyro->hf_manager, SENSOR_TYPE_GYROSCOPE) < 0) {
        LOGD(TAG "find gyro sensor fail\n");
        goto err_out;
    }

    HfManagerEnableSensor(gyro->hf_manager, SENSOR_TYPE_GYROSCOPE, SENSOR_SAMPLE_PERIOD_NS, 0);
    HfManagerEnableRawData(gyro->hf_manager, SENSOR_TYPE_GYROSCOPE);
    return 0;

err_out:
    HfLooperDestroy(gyro->hf_looper);
    HfManagerDestroy(gyro->hf_manager);
    return -1;
}

static int gyro_close(struct gyro_priv *gyro)
{
    HfManagerDisableRawData(gyro->hf_manager, SENSOR_TYPE_GYROSCOPE);
    HfManagerDisableSensor(gyro->hf_manager, SENSOR_TYPE_GYROSCOPE);

    HfLooperDestroy(gyro->hf_looper);
    HfManagerDestroy(gyro->hf_manager);

    memset(gyro, 0x00, sizeof(*gyro));
    return 0;
}

static int gyro_read(struct gyro_priv *gyro)
{
    sensors_event_t data[32] = {0};

    int err = HfLooperEventLooperTimeout(gyro->hf_looper, data, 32, TIMEOUT_MS);

    if (err < 0) {
        LOGE(TAG "Hf looper event looper retrun value:%d\n", err);
        return err;
    }

    for (int i = 0; i < err; ++i) {
        if (data[i].reserved0 == RAW_ACTION) {
            gyro->gyro_x = (float)(data[i].data[0]);
            gyro->gyro_y = (float)(data[i].data[1]);
            gyro->gyro_z = (float)(data[i].data[2]);

            // add sensor data to struct sp_ata data for PC side
            return_data.gyroscope.gyroscope_x = gyro->gyro_x;
            return_data.gyroscope.gyroscope_y = gyro->gyro_y;
            return_data.gyroscope.gyroscope_z = gyro->gyro_z;
            return_data.gyroscope.accuracy= 3;
        }
    }
    return 0;
}

static void *gyro_update_iv_thread(void *priv)
{
    struct gyro_data *dat = (struct gyro_data *)priv;
    struct gyro_priv *gyro = &dat->gyro;
    struct itemview *iv = dat->iv;
    int err = 0;
    int len = 0;
    float total;

    FGYROLOGD(TAG "%s: Start\n", __FUNCTION__);

    if ((err = gyro_open(gyro))) {
        memset(dat->info, 0x00, sizeof(dat->info));
        sprintf(dat->info, "%s\n", uistr_info_sensor_init_fail);
        iv->redraw(iv);
        FGYROLOGE("gyro() err = %d(%s)\n", err, dat->info);
        return NULL;
    }

    while (1) {
        if (dat->exit_thd)
            break;

        if ((err = gyro_read(gyro))) {
            FGYROLOGE("gyro_update_info() = (%s), %d\n", strerror(errno), err);
            break;
        }

        len = snprintf(dat->info, sizeof(dat->info), "X: %+7.4f \nY: %+7.4f \nZ: %+7.4f \n \n",
                gyro->gyro_x, gyro->gyro_y, gyro->gyro_z);
        total = gyro->gyro_x*gyro->gyro_x + gyro->gyro_y*gyro->gyro_y + gyro->gyro_z*gyro->gyro_z;
        if ((total >=0) && (total <=0.75))
            len += snprintf(dat->info+len,sizeof(dat->info)-len, "Test Result: %s\n",uistr_info_sensor_pass );
        else
            len += snprintf(dat->info+len,sizeof(dat->info)-len, "Test Result: %s\n",uistr_info_sensor_fail );

        iv->set_text(iv, &dat->text);
        iv->redraw(iv);
        /**
        pthread_mutex_lock (&gyro_mutex);
        if(sp_ata_status == FTM_AUTO_ITEM)
        {
            gyroscope_thread_exit = true;
            pthread_mutex_unlock (&gyro_mutex);
            break;
        }
        pthread_mutex_unlock (&gyro_mutex);
        **/

    }

    gyro_close(gyro);
    return NULL;
}
/*---------------------------------------------------------------------------*/
int gyro_entry(struct ftm_param *param, void *priv)
{
    int chosen;
    struct gyro_data *dat = (struct gyro_data *)priv;
    struct itemview *iv;
    int len;
    unsigned char usleep_count = 0;

    FGYROLOGD(TAG "%s\n", __FUNCTION__);

    init_text(&dat->title, param->name, COLOR_YELLOW);
    init_text(&dat->text, &dat->info[0], COLOR_YELLOW);
    init_text(&dat->left_btn, uistr_info_sensor_fail, COLOR_YELLOW);
    init_text(&dat->center_btn, uistr_info_sensor_pass, COLOR_YELLOW);
    init_text(&dat->right_btn, uistr_info_sensor_back, COLOR_YELLOW);

    len = snprintf(dat->info, sizeof(dat->info), "%s\n",  uistr_info_sensor_initializing);
    dat->exit_thd = false;
    gyroscope_thread_exit = false;

    if (!dat->iv) {
        iv = ui_new_itemview();
        if (!iv) {
            FGYROLOGD(TAG "No memory");
            return -1;
        }
        dat->iv = iv;
    }
    iv = dat->iv;
    iv->set_title(iv, &dat->title);
    iv->set_items(iv, gyro_items, 0);
    iv->set_text(iv, &dat->text);
    sp_ata_status = param->test_type;
    pthread_create(&dat->update_thd, NULL, gyro_update_iv_thread, priv);
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
            chosen = iv->run(iv, &gyroscope_thread_exit);
            switch (chosen) {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    dat->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    dat->mod->test_result = FTM_TEST_FAIL;
                }
                gyroscope_thread_exit = true;
                break;
#else /* FEATURE_FTM_TOUCH_MODE */
            case L_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_FAIL;
                gyroscope_thread_exit = true;
                break;
            case C_BTN_DOWN:
                gyroscope_thread_exit = true;
                break;
            case R_BTN_DOWN:
                dat->mod->test_result = FTM_TEST_PASS;
                gyroscope_thread_exit = true;
                break;
#endif /* FEATURE_FTM_TOUCH_MODE */
            }
        }
        iv->redraw(iv);
        pthread_mutex_lock (&gyro_mutex);
        if (gyroscope_thread_exit || usleep_count == 100) {
            dat->exit_thd = true;
            pthread_mutex_unlock (&gyro_mutex);
            break;
        } else {
            usleep_count++;
            pthread_mutex_unlock (&gyro_mutex);
            usleep(50000);
        }
    } while (1);

    pthread_join(dat->update_thd, NULL);
    return 0;
}
/*---------------------------------------------------------------------------*/
int gyro_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct gyro_data *dat;

    FGYROLOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_GYROSCOPE, sizeof(struct gyro_data));
    dat  = mod_to_gyro_data(mod);

    memset(dat, 0x00, sizeof(*dat));
    gyro_init_priv(&dat->gyro);

    /*NOTE: the assignment MUST be done, or exception happens when tester press Test Pass/Test Fail*/
    dat->mod = mod;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, gyro_entry, (void*)dat);

    return ret;
}
#endif

