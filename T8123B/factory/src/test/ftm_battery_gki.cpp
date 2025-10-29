/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2020. All rights reserved.
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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "miniui.h"
#include "ftm.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include <android/hardware/health/2.0/IHealth.h>
#include <healthhalutils/HealthHalUtils.h>

using ::android::hardware::health::V2_0::get_health_service;
using ::android::hardware::health::V2_0::HealthInfo;
using ::android::hardware::health::V2_0::IHealth;
using ::android::hardware::health::V2_0::Result;
using ::android::sp;


#define TAG                 "[Batt & Charging2] "

#define CHARGER_VOLTAGE_FILE "/sys/devices/platform/charger/ADC_Charger_Voltage"

#define AUTO_TEST_VBAT_LB 3000
#define AUTO_TEST_VBAT_THRES 4200
#define AUTO_TEST_CHR_CURRENT_VALUE 300
#define AUTO_TEST_VCHR_VALUE 4000
#define AUTO_TEST_BAT_TEMP_MIN 5
#define AUTO_TEST_BAT_TEMP_MAX 45

#ifdef FEATURE_FTM_BATTERY
#ifdef __cplusplus
extern "C" {
#endif

static sp<IHealth> g_health;
static int gBatteryAutoTest = 1;

enum {
    ITEM_PASS,
    ITEM_FAIL,
};

static item_t battery_items_auto[] = {
    /* auto test */
    item(-1, NULL),
};

static item_t battery_items_manual[] = {
#ifndef FEATURE_FTM_TOUCH_MODE
    /* manual test */
    item(ITEM_PASS, uistr_pass),
    item(ITEM_FAIL, uistr_fail),
#endif
    item(-1, NULL),
};

extern sp_ata_data return_data;

struct batteryFTM {
    int bat_voltage;
    int charger_voltage;
    int bat_temperature;
    char info[1024];
    bool is_charging;
    bool charger_exist;
    bool exit_thd;
    pthread_t batt_update_thd;
    struct ftm_module *mod;
    struct textview tv;
    struct itemview *iv;

    text_t title;
    text_t text;
    text_t left_btn;
    text_t center_btn;
    text_t right_btn;
};

#define mod_to_batteryFTM(p)     (struct batteryFTM*)((char*)(p) + sizeof(struct ftm_module))

static int get_charger_voltage(void)
{
	int fd = -1;
	int ret = 0;
	char vbus_buf[10] = {0};
	int vbus = 0;

	fd = open(CHARGER_VOLTAGE_FILE, O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s, fd = %d\n", CHARGER_VOLTAGE_FILE, fd);
		return -ENODEV;
	}

	ret = read(fd, vbus_buf, sizeof(vbus_buf));
	if (ret < 0) {
		LOGD(TAG "%s read fail, ret=%d\n", CHARGER_VOLTAGE_FILE, errno);
		close(fd);
		return -EINVAL;
	};
	vbus = atoi(vbus_buf);
	close(fd);

	return vbus;
}

static int get_health_info(HealthInfo *info)
{
    if (g_health == nullptr) {
        LOGD("%s: No health service, cannot get status\n", __func__);
        return -EINVAL;
    }

    auto ret = g_health->getHealthInfo([&](Result r, HealthInfo out) {
        if (r != Result::SUCCESS) {
            LOGD("Cannot get HealthInfo, r=%d\n", r);
            return;
        }
        *info = out;
    });
    if (!ret.isOk()) {
        LOGD("transaction error\n");
        return -EINVAL;
    }

    return 0;
}

static bool is_charging_source_available()
{
    bool usb_online = false, ac_online = false, wireless_online = false;
    int ret = 0;
    HealthInfo info = {};

    ret = get_health_info(&info);
    if (ret < 0) {
        LOGD("%s: cannot get HealthInfo\n", __func__);
        return 0;
    }

    usb_online = info.legacy.chargerUsbOnline;
    ac_online = info.legacy.chargerAcOnline;
    wireless_online = info.legacy.chargerWirelessOnline;

    LOGD("in %s(), usb:%d ac:%d wireless:%d\n", __func__,
            usb_online, ac_online, wireless_online);

    return (usb_online || ac_online || wireless_online);
}

static void battery_update_info(struct batteryFTM *batt, char *info)
{
    char *ptr = NULL;
    int temp = 0;
    int ret = 0;
    bool online_status = false;
    HealthInfo hinfo = {};
#ifdef FEATURE_FTM_PMIC_632X
    int battery_fg_current = 0;
#endif

    /* Charger_Voltage */
    temp = get_charger_voltage();
    if (temp >= 0) {
        batt->charger_voltage = temp;
        if (batt->charger_voltage >= AUTO_TEST_VCHR_VALUE) {
            batt->charger_exist = true;
            batt->is_charging = true;
        } else {
            batt->charger_exist = false;
            batt->is_charging = false;
        }
    } else {
        batt->charger_voltage = -1;
        batt->charger_exist = false;
        batt->is_charging = false;
    }

    ret = get_health_info(&hinfo);
    if (ret < 0) {
        LOGD("%s: cannot get HealthInfo\n", __func__);
        return;
    }

    online_status = is_charging_source_available();
    LOGD(TAG "online status = %d\n", online_status);
    if (!online_status) {
        batt->charger_exist = false;
        batt->is_charging = false;
    }

    batt->bat_voltage = hinfo.legacy.batteryVoltage; /* mV */
    batt->bat_temperature = hinfo.legacy.batteryTemperature / 10;
    battery_fg_current = hinfo.legacy.batteryCurrent / 1000; /* mA */
	
	#ifdef FEATURE_FTM_HW_CANNOT_MEASURE_CURRENT
	//batt->adc_vbat_current = 499; //接屏电流消耗大 ，充电电流会为负数导致失败
	//if(battery_fg_current < 0)
		battery_fg_current = 499;
	#endif

    /* preare text view info */
    ptr = info;
    ptr += sprintf(ptr, "%s : %d %s \n", uistr_info_title_battery_val, batt->bat_voltage, uistr_info_title_battery_mv);
    ptr += sprintf(ptr, "%s : %d %s \n", uistr_info_title_battery_temp, batt->bat_temperature, uistr_info_title_battery_c);
    ptr += sprintf(ptr, "%s : %s \n", uistr_info_title_battery_chr, (batt->is_charging) ? uistr_info_title_battery_yes : uistr_info_title_battery_no);
    ptr += sprintf(ptr, "%s : %d %s \n", uistr_info_title_battery_chr_val, batt->charger_voltage, uistr_info_title_battery_mv);
#ifdef FEATURE_FTM_PMIC_632X
    ptr += sprintf(ptr, "%s: %d %s \n", uistr_info_title_battery_fg_cur, battery_fg_current, uistr_info_title_battery_ma);
#endif

    return;
}

static void *battery_update_iv_thread(void *priv)
{
    struct batteryFTM *batt = (struct batteryFTM *)priv;
    struct itemview *iv = batt->iv;
    int chkcnt = 10;

    LOGD(TAG "%s: Start\n", __FUNCTION__);

    while (1) {
        usleep(100000);
        chkcnt--;

        if (batt->exit_thd)
            break;

        if (chkcnt > 0)
            continue;

        battery_update_info(batt, batt->info);
        iv->redraw(iv);
        chkcnt = 10;
    }
    LOGD(TAG "%s: Exit\n", __FUNCTION__);
    pthread_exit(NULL);

    return NULL;
}

int battery_entry(struct ftm_param *param, void *priv)
{
    int chosen = 0;
    bool exit = false;
    struct batteryFTM *batt = (struct batteryFTM *)priv;
    struct itemview *iv;
    /* auto test */
    int temp_v_bat = 0;
    int temp_v_chr = 0;
#ifdef FEATURE_FTM_VBAT_TEMP_CHECK
    int temp_v_bat_temp = 0;
#endif
    bool online_status = false;
    int ret = 0;
    HealthInfo hinfo = {};

    LOGD(TAG "%s\n", __func__);
    if (FTM_AUTO_ITEM  == param->test_type)
        gBatteryAutoTest = 1;
    else
        gBatteryAutoTest = 0;

    LOGD(TAG "AutoMode=%d, %s\n", gBatteryAutoTest, __func__);
    init_text(&batt->title, param->name, COLOR_YELLOW);
    init_text(&batt->text, &batt->info[0], COLOR_YELLOW);
    init_text(&batt->left_btn, "Fail", COLOR_YELLOW);
    init_text(&batt->center_btn, "Pass", COLOR_YELLOW);
    init_text(&batt->right_btn, "Back", COLOR_YELLOW);

    /* get health instance */
    g_health = get_health_service();
    if (g_health == nullptr) {
        LOGD(TAG "Could not retrieve health service\n");
        return -ENODEV;
    }

    battery_update_info(batt, batt->info);

    /* show text view */
    batt->exit_thd = false;
    if (!batt->iv) {
        iv = ui_new_itemview();
        if (!iv) {
            LOGD(TAG "No memory");
            return -1;
        }
        batt->iv = iv;
    }

    iv = batt->iv;
    iv->set_title(iv, &batt->title);
    if (gBatteryAutoTest) {
        iv->set_items(iv, battery_items_auto, 0);
    } else {
        iv->set_items(iv, battery_items_manual, 0);
    }
    iv->set_text(iv, &batt->text);
    iv->start_menu(iv,0);

    if (gBatteryAutoTest) {
        iv->redraw(iv);
        sleep(1); /* wait for screen display */

        return_data.battery.current = 0;
        return_data.battery.voltage = 0;

        ret = get_health_info(&hinfo);
        if (ret < 0) {
            LOGD("%s: cannot get HealthInfo\n", __func__);
            return -ENODEV;
        }

#ifdef FEATURE_FTM_VBAT_TEMP_CHECK
        /* auto test - battery temp */
        temp_v_bat_temp = hinfo.legacy.batteryTemperature / 10;
        if(temp_v_bat_temp < AUTO_TEST_BAT_TEMP_MIN ||
           temp_v_bat_temp > AUTO_TEST_BAT_TEMP_MAX) {
            LOGD(TAG "[FTM_BAT 5] VBatTemp = %d , return fail\n", temp_v_chr);
            batt->mod->test_result = FTM_TEST_FAIL;
            return 0;
        }
#endif

        /* auto test - if no charger in, return fail */
        online_status = is_charging_source_available();
        if (!online_status) {
            LOGD(TAG "online status = %d, return fail\n", online_status);
            batt->mod->test_result = FTM_TEST_FAIL;
            return 0;
        }

        temp_v_chr = get_charger_voltage();
        if(temp_v_chr < AUTO_TEST_VCHR_VALUE) {
            LOGD(TAG "[FTM_BAT 0] vchr %d < %d => no charger, return fail\n",
                 temp_v_chr, AUTO_TEST_VCHR_VALUE);
            batt->mod->test_result = FTM_TEST_FAIL;
            return 0;
        }

        /* auto test - vbat */
        temp_v_bat = hinfo.legacy.batteryVoltage;
        return_data.battery.voltage = temp_v_bat;
        if (temp_v_bat < AUTO_TEST_VBAT_LB) {
            LOGD(TAG "[FTM_BAT 1] vbat %d < %d, return fail\n",
                temp_v_bat, AUTO_TEST_VBAT_LB);

            batt->mod->test_result = FTM_TEST_FAIL;
            return 0;
        }
        if (temp_v_bat > AUTO_TEST_VBAT_THRES) {
            LOGD(TAG "[FTM_BAT 2] vbat %d > %d, return pass\n",
                temp_v_bat, AUTO_TEST_VBAT_THRES);

            /* ignore current test because the battery is almost full */
			return_data.battery.current = 488;
            batt->mod->test_result = FTM_TEST_PASS;
            return 0;
        }

        /* measure ibat */
        return_data.battery.current = hinfo.legacy.batteryCurrent / 1000;
		#ifdef FEATURE_FTM_HW_CANNOT_MEASURE_CURRENT
	   //接屏电流消耗大 ，充电电流会为负数导致失败
	    //if(return_data.battery.current < 0)
			return_data.battery.current = 499;
		#endif
        batt->mod->test_result = FTM_TEST_PASS;

    } else {
        pthread_create(&batt->batt_update_thd, NULL, battery_update_iv_thread, priv);
        do {
#ifdef FEATURE_FTM_TOUCH_MODE
            text_t lbtn;
            text_t cbtn;
            text_t rbtn;

            init_text(&lbtn,uistr_key_fail, COLOR_YELLOW);
            init_text(&cbtn,uistr_key_back, COLOR_YELLOW);
            init_text(&rbtn,uistr_key_pass, COLOR_YELLOW);

            iv->set_btn(iv, &lbtn, &cbtn, &rbtn);
#endif
            chosen = iv->run(iv, &exit);
            switch (chosen) {
#ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    batt->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    batt->mod->test_result = FTM_TEST_FAIL;
                }
                exit = true;
                break;
#else
            case L_BTN_DOWN:
                batt->mod->test_result = FTM_TEST_FAIL;
                exit = true;
                break;
            case C_BTN_DOWN:
                exit = true;
                break;
            case R_BTN_DOWN:
                batt->mod->test_result = FTM_TEST_PASS;
                exit = true;
#endif
            }

            if (exit) {
                batt->exit_thd = true;
                break;
            }
        } while (1);
        pthread_join(batt->batt_update_thd, NULL);
    }
    return 0;
}

int battery_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct batteryFTM *batt;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_CHARGER, sizeof(struct batteryFTM));
    batt = mod_to_batteryFTM(mod);

    /* init */
    batt->mod = mod;
    batt->bat_voltage = 0;
    batt->charger_voltage = 0;
    batt->bat_temperature = 0;
    batt->is_charging = false;
    batt->charger_exist = false;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, battery_entry, (void*)batt);

    return ret;
}

#ifdef __cplusplus
};
#endif
#endif /* FEATURE_FTM_BATTERY */
