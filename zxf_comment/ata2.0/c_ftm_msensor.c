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
/*
🔍 一、整体功能概述
这是一个 工厂测试（Factory Test Mode, FTM） 中的磁力计测试模块，用于：

打开并读取磁力计传感器数据。
在屏幕上实时显示 X/Y/Z 三轴磁场强度。
提供“通过（Pass）”、“失败（Fail）”、“返回（Back）”按钮供测试员操作。
支持自动与手动测试模式。
将测试结果上报给上位机（如 PC 端产线系统）。
✅ 简单说：这是手机生产线上用来检测“指南针”功能是否正常的调试程序。
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

//存储从传感器读取的原始数据
struct mag_evt {
    int x;  // 三轴磁场值（单位：微特斯拉 μT）
    int y;
    int z;
    int yaw;    // 姿态角（偏航、俯仰、翻滚）——当前未使用
    int pitch;
    int roll;
    unsigned int status;    // 传感器状态（0~3，表示精度）
    unsigned int div;   // 数据更新周期
    //int64_t time;
};

//封装了与 MTK 传感器 HAL 层交互所需的资源
struct mag_priv {
    /*specific data field*/
    void *hf_manager;   // HfManager 句柄（MTK 传感器框架）
    void *hf_looper;    // 事件循环处理器
    struct mag_evt evt; // 当前传感器事件数据
};

//整个模块的核心数据结构，包含 UI、线程、传感器状态等。
struct mag_data {
    struct mag_priv mag;
    bool exit_thd;  // 控制线程退出标志
    /*common for each factory mode*/
    char  info[1024];   // 显示文本缓冲区

    text_t    title;    // UI 文本元素
    text_t    text;
    text_t    left_btn;
    text_t    center_btn;
    text_t    right_btn;

    pthread_t update_thd;   // 数据更新线程
    struct ftm_module *mod; // FTM 模块指针
    //struct textview tv;
    struct itemview *iv;    // UI 视图控件
};

/******************************************************************************
 * common interface
 *****************************************************************************/
/*
作用：初始化并启用磁力计传感器。
流程：
    创建 HfManager 和 Looper（MTK 专用传感器管理器）。
    查找是否存在 SENSOR_TYPE_MAGNETIC_FIELD 类型的传感器。
    启用该传感器，设置采样周期为 200ms（SENSOR_SAMPLE_PERIOD_NS = 200000000 ns）。
    启用原始数据模式（HfManagerEnableRawData）。
    ✅ 成功返回 0；失败则清理资源并返回 -1。
*/
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

/*
作用：关闭传感器并释放资源。
流程：
    禁用传感器和原始数据。
    销毁 Looper 和 Manager。
    清空结构体。
*/
static int msensor_close(struct mag_priv *mag)
{
    HfManagerDisableRawData(mag->hf_manager, SENSOR_TYPE_MAGNETIC_FIELD);
    HfManagerDisableSensor(mag->hf_manager, SENSOR_TYPE_MAGNETIC_FIELD);

    HfLooperDestroy(mag->hf_looper);
    HfManagerDestroy(mag->hf_manager);

    memset(mag, 0x00, sizeof(*mag));
    return 0;
}

/*
作用：从传感器读取一批数据。
流程：
    调用 HfLooperEventLooperTimeout(..., TIMEOUT_MS=500) 等待最多 500ms 获取事件。
    遍历返回的事件数组，查找带有 RAW_ACTION 标志的数据。
    提取 X/Y/Z 轴原始值，并更新到 mag->evt 中。
    同时将数据写入全局变量 return_data.msensor，用于上传给 PC 上位机。
    ⚠️ 注意：这里假设数据是整数型（float 被强转为 int），但实际可能是 float，需确认 HAL 实现
*/
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
            
            // add sensor data to struct sp_ata data for PC side  将传感器数据传递给PC端
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

/*
作用：独立线程，持续读取传感器数据并刷新 UI。
流程：
    调用 msensor_open() 初始化传感器。
    进入循环：
        调用 msensor_read() 读取最新数据。
        使用 snprintf 构造显示字符串，包括：
            M-Sensor: [status]
            Status: 3
            Data:
            X: 45
            Y: -23
            Z: 102
        调用 iv->set_text() 和 iv->redraw() 更新屏幕。
    若收到退出信号（dat->exit_thd），则跳出循环并调用 msensor_close()。
    📌 这是一个典型的“生产者-消费者”模型中的生产者线程。
*/
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

/*
作用：FTM 模块的主入口函数，负责创建 UI 并处理用户交互。
流程：
    初始化 UI 文本（标题、说明、按钮文字）。
    创建 itemview（一种菜单/界面控件）。
    设置标题、按钮、文本内容。
    记录测试类型（自动/手动）到 sp_ata_status。
    创建线程 msensor_update_iv_thread 来更新数据。
    进入主循环等待用户点击按钮：
        Pass → 设置测试结果为 FTM_TEST_PASS
        Fail → 设置测试结果为 FTM_TEST_FAIL
        Back → 退出测试
    收到退出信号后，pthread_join 等待线程结束。
📌 这是“消费者”和“控制逻辑”的结合。
*/
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

/*
作用：模块初始化函数，在系统启动时被调用。注册一个名为“磁力计测试”的功能模块到 FTM 框架中，使得用户可以在工程菜单里看到并运行这个测试项。
流程：
    分配一个 ftm_module 结构（FTM 框架的模块描述符）。
    分配 mag_data 结构并初始化。
    注册该模块到 FTM 系统，入口函数为 msensor_entry。
✅ 返回 0 表示注册成功。
*/
int msensor_init(void)
{
    int ret = 0;
    struct ftm_module *mod; //指向 ftm_module 结构体的指针，它是 FTM 框架用来管理每个测试项的标准描述符。相当于“这个测试模块的身份信息”。
    struct mag_data *dat;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_MSENSOR, sizeof(struct mag_data)); //调用 ftm_alloc() 函数从 FTM 框架中申请一块内存空间，用于注册一个新的测试项。
    dat = mod_to_mag_data(mod);

    /*
    将整个 struct mag_data 结构体清零，防止使用未初始化的内存导致崩溃或异常行为。
    包括字符串缓冲区、UI 文本、线程 ID 等全部设为初始状态。
    */
    memset(dat, 0x00, sizeof(*dat));
    /*
    调用本地函数 msensor_init_priv()，进一步初始化 mag_priv 子结构。
    查看源码可知，该函数也做了 memset(..., 0)，确保 hf_manager, hf_looper, evt 等字段安全可用。
    📝 虽然前面已经 memset(dat)，但这里再次初始化子结构是为了代码清晰和可维护性。
    */
    msensor_init_priv(&dat->mag);

    /*NOTE: the assignment MUST be done, or exception happens when tester press Test Pass/Test Fail
    因为在后续的 msensor_entry() 和按钮处理逻辑中，程序需要知道：
    “我现在操作的是哪一个 ftm_module？我要把测试结果写到哪里？”
    所以：
        dat->mod = mod; 建立了 从私有数据回溯到主模块结构的链接。
        当用户点击“Pass”或“Fail”时，代码会通过 dat->mod->test_result = FTM_TEST_PASS; 来设置结果。
        如果没有这句赋值，dat->mod 就是野指针或 NULL，会导致 空指针解引用、崩溃（Segmentation Fault）。
    📌 这是一个典型的 双向链接设计：

    mod → dat：通过偏移获取私有数据（正向）
    dat → mod：通过指针保存主结构（反向）
    */
    dat->mod = mod;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, msensor_entry, (void*)dat); //将我们准备好的测试模块正式注册进 FTM 主框架，使其出现在测试菜单中。

    return ret;
}
#endif

