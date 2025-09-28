/*
 * Copyright (c) 2019 MediaTek Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * 这段代码是 MediaTek (MTK) 平台上 LK (Little Kernel) 阶段的 充电管理模块 的一部分，主要负责在设备启动早期
 * （开机自检阶段）进行电池状态检测、充电控制、低电量处理以及开机充电动画显示等功能。
📌 概述
该模块的核心功能是：
    初始化充电器驱动和自定义参数
    读取设备树 (DTB/FDT) 中的充电配置
    检测电池电压、温度、充电器类型
    在低电量或无电池时进行安全处理（关机或充电）
    支持开机充电动画（Logo 动画）
    支持 KPOC (Key Pressed Power On Charging) 功能

    LK 启动
   ↓
charger_init() → 初始化驱动 + 读取设备树参数
   ↓
mtk_charger_start()
   ├─ 启用 power_measure? → 进入电流测量循环
   └─ disable_charger? → 跳过
      ↓
   check_low_battery()
      ├─ 温度异常? → 显示警告或关机
      ├─ vsys 可供电? → 跳过 vbat 检查
      ├─ vbat < 3450mV?
      │   ├─ 有充电器 → check_charging_status() → 补电至安全电压
      │   └─ 无充电器 → 显示低电警告 → 关机
      └─ 正常 → 继续启动
 */
#include <atm.h>
#include <debug.h>
#include <kernel/thread.h>
#include <lib/pl_boottags.h>
#include <libfdt.h>
#include <mt_logo.h>
#include <mtk_battery.h>
#include <mtk_charger.h>
#include <platform.h>
#include <platform/boot_mode.h>
#include <platform/leds.h>
#include <platform/wdt.h>
#include <platform_mtk.h>
#include <pmic_auxadc.h>
#include <pmic_keys.h>
#include <set_fdt.h>
#include <sysenv.h>
#include <trace.h>

#include "mtk_charger_intf.h"

#if (CHARGER_ETA6947_SUPPORT)
#include "eta6947_charger.h"
#endif

#if (CHARGER_UPM6915_SUPPORT)
#include "upm6915_charger.h"
#endif

#define EM_MAX_TEMP_THRESHOLD	58
#define EM_MIN_TEMP_THRESHOLD	-18

#define LOCAL_TRACE 0

#define BOOT_TAG_CHR_INFO         0x88610018
#define BATTERY_ON_THRESHOLD      2500
#define CHARGER_ON_THRESHOLD      2500
#define MAX_SLEEP_LOOP            20
#define VBUS_CHECK_COUNT          3
#define LOOP_MAX_COUNT            86400

#define OFF_MODE_CHARGE "off-mode-charge"
#if (MULTI_EXT_MAIN_CHARGER)
static int ext_first_mainchg_init_state = 0;
#endif

static struct charger_custom_data chr_cust_data;
static int charger_type;

static uint32_t pl_boottags_get_charger_type(void)
{
    return charger_type;
}

static void pl_boottags_charger_type_hook(struct boot_tag *tag)
{
    memcpy(&charger_type, &tag->data, sizeof(charger_type));
}
PL_BOOTTAGS_INIT_HOOK(charger_type, BOOT_TAG_CHR_INFO, pl_boottags_charger_type_hook);  //在启动时从 BOOT_TAG_CHR_INFO 标签中获取 charger_type，实现阶段间通信。

#ifdef WITH_PLATFORM_MEDIATEK_COMMON_ATM
static void set_charger_atm(void *fdt)
{
    int offset = 0;
    bool atm_is_enabled = false;

    if (fdt != NULL) {
        offset = fdt_node_offset_by_compatible(fdt, -1, "mediatek,charger");

        if (offset >= 0) {
            atm_is_enabled = (get_atm_enable_status() != 0);
            if (atm_is_enabled)
                fdt_setprop(fdt, offset, "atm_is_enabled", &atm_is_enabled,
                    sizeof(atm_is_enabled));

            dprintf(CRITICAL, "%s: atm_is_enabled:%d to dtsi!\n",
                __func__, atm_is_enabled);
        } else {
            dprintf(CRITICAL, "%s: charger: no device tree!\n", __func__);
        }
    } else {
        dprintf(CRITICAL, "%s: fdt is null!\n", __func__);
    }
}

SET_FDT_INIT_HOOK(set_charger_atm, set_charger_atm);
#endif

void set_off_mode_charge_status(int enable_charge)
{
    static char *env_buf;

    env_buf = (char *)malloc(sizeof(int));
    if (!env_buf) {
        dprintf(CRITICAL, "off-mode-charge malloc failed\n");
        return;
    }
    memset(env_buf, 0x00, sizeof(int));

    sprintf(env_buf, "%d", enable_charge);
    if (set_env(OFF_MODE_CHARGE, env_buf) != 0)
        dprintf(CRITICAL, "set off-mode-charge failed\n");

    free(env_buf);
    return;
}

/*
获取系统是否允许“关机充电”（Off-mode Charging）功能的启用状态。
*/
int get_off_mode_charge_status(void)
{
     char *charge_buf = NULL;
     int enable_charge = 1; // 默认值为1（启用关机充电）

    /*
    如果 get_env(OFF_MODE_CHARGE) 返回 NULL，说明：
    没有设置这个环境变量；
    或者变量不存在（可能是首次开机、恢复出厂设置等）
    */
     charge_buf = get_env(OFF_MODE_CHARGE);

     if (charge_buf == NULL) {
          set_off_mode_charge_status(1);
          return 1;
     }
    /*
    如果 charge_buf 不是 NULL，说明存在该环境变量。
    使用 atoi()（ASCII to Integer）将字符串转换为整数。
    例如：
    "1" → 1（启用）
    "0" → 0（禁用）
    "abc" 或空字符串 → 0（atoi 对非法字符串返回 0）
     */
     enable_charge = atoi(charge_buf);
     return enable_charge;
}

static bool chr_fdt_getprop_bool(const void *fdt, int nodeoffset,
                        const char *name)
{
    const void *data = NULL;
    int len = 0;

    data = fdt_getprop(fdt, nodeoffset, name, &len);
    if (data)
        return true;
    else
        return false;
}

static unsigned int chr_fdt_getprop_u32(const void *fdt, int nodeoffset,
                                 const char *name)
{
    const void *data = NULL;
    int len = 0;

    data = fdt_getprop(fdt, nodeoffset, name, &len);
    if (len > 0 && data)
        return fdt32_to_cpu(*(unsigned int *)data);
    else
        return 0;
}

static void init_charger_custom_data(void)
{
    chr_cust_data.disable_charger = false;
    chr_cust_data.power_path_support = true;
    chr_cust_data.enable_pe_plus = true;
    chr_cust_data.boot_battery_voltage = 3450;
    chr_cust_data.max_charger_voltage = 6500;
    chr_cust_data.min_charger_voltage = 4000;
    chr_cust_data.fast_charge_voltage = 3000;

    /* charging current */
    chr_cust_data.usb_charger_current = 500;
    chr_cust_data.ac_charger_current = 2050;
    chr_cust_data.ac_charger_input_current = 2000;
    chr_cust_data.non_std_ac_charger_current = 500;
    chr_cust_data.charging_host_charger_current = 500;
    chr_cust_data.ta_ac_charger_current = 3000;
    chr_cust_data.pd_charger_current = 500;

    /* temperature protection ref sw jeita */
    chr_cust_data.temp_t4_threshold = 50;
    chr_cust_data.temp_t3_threshold = 45;
    chr_cust_data.temp_t1_threshold = 0;

    /* charging anime */
    chr_cust_data.enable_anime = true;
    chr_cust_data.led_brightness = LED_DIM;
    chr_cust_data.blinking_times = 6;
    chr_cust_data.blinking_period = 1500;

    /* vbus resistance */
    chr_cust_data.r_charger_1 = 330;
    chr_cust_data.r_charger_2 = 39;

    /*vsys threshold*/
    chr_cust_data.min_vsys_voltage = 4000;
    chr_cust_data.enable_check_vsys = false;

    chr_cust_data.enable_power_measure = false;

    chr_cust_data.battery_volt_is_low = false;
}

static int init_cust_data_from_dt(void *fdt)
{
    int offset, val;

    offset = fdt_node_offset_by_compatible(fdt, -1, "mediatek,lk_charger");

    if (offset >= 0) {
        val = chr_fdt_getprop_bool(fdt, offset, "disable_charger");
        if (val)
            chr_cust_data.disable_charger = true;
        else
            chr_cust_data.disable_charger = false;

        val = chr_fdt_getprop_bool(fdt, offset, "enable_pe_plus");
        if (val)
            chr_cust_data.enable_pe_plus = true;
        else
            chr_cust_data.enable_pe_plus = false;

        val = chr_fdt_getprop_bool(fdt, offset, "power_path_support");
        if (val)
            chr_cust_data.power_path_support = true;
        else
            chr_cust_data.power_path_support = false;

        val = chr_fdt_getprop_u32(fdt, offset, "boot_battery_voltage");
        if (val)
            chr_cust_data.boot_battery_voltage = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "max_charger_voltage");
        if (val)
            chr_cust_data.max_charger_voltage = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "min_charger_voltage");
        if (val)
            chr_cust_data.min_charger_voltage = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "fast_charge_voltage");
        if (val)
            chr_cust_data.fast_charge_voltage = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "usb_charger_current");
        if (val)
            chr_cust_data.usb_charger_current = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "ac_charger_current");
        if (val)
            chr_cust_data.ac_charger_current = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "ac_charger_input_current");
        if (val)
            chr_cust_data.ac_charger_input_current = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "non_std_ac_charger_current");
        if (val)
            chr_cust_data.non_std_ac_charger_current = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "charging_host_charger_current");
        if (val)
            chr_cust_data.charging_host_charger_current = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "ta_ac_charger_current");
        if (val)
            chr_cust_data.ta_ac_charger_current = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "pd_charger_current");
        if (val)
            chr_cust_data.pd_charger_current = val / 1000;

        val = chr_fdt_getprop_u32(fdt, offset, "temp_t4_threshold");
        if (val)
            chr_cust_data.temp_t4_threshold = val;

        val = chr_fdt_getprop_u32(fdt, offset, "temp_t3_threshold");
        if (val)
            chr_cust_data.temp_t3_threshold = val;

        val = chr_fdt_getprop_u32(fdt, offset, "temp_t1_threshold");
        if (val)
            chr_cust_data.temp_t1_threshold = val;

        val = chr_fdt_getprop_bool(fdt, offset, "enable_anime");
        if (val)
            chr_cust_data.enable_anime = true;
        else
            chr_cust_data.enable_anime = false;

        val = chr_fdt_getprop_u32(fdt, offset, "led_brightness");
        if (val)
            chr_cust_data.led_brightness = val;

        val = chr_fdt_getprop_u32(fdt, offset, "blinking_times");
        if (val)
            chr_cust_data.blinking_times = val;

        val = chr_fdt_getprop_u32(fdt, offset, "blinking_period");
        if (val)
            chr_cust_data.blinking_period = val;

        val = chr_fdt_getprop_u32(fdt, offset, "r_charger_1");
        if (val)
            chr_cust_data.r_charger_1 = val;

        val = chr_fdt_getprop_u32(fdt, offset, "r_charger_2");
        if (val)
            chr_cust_data.r_charger_2 = val;

        val = chr_fdt_getprop_bool(fdt, offset, "enable_check_vsys");
        if (val)
            chr_cust_data.enable_check_vsys = true;
        else
            chr_cust_data.enable_check_vsys = false;

        val = chr_fdt_getprop_bool(fdt, offset, "enable_power_measure");
        if (val)
            chr_cust_data.enable_power_measure = true;
        else
            chr_cust_data.enable_power_measure = false;

        LTRACEF("%s: chroff:%d,pe:%d,powpath:%d,vchrmax:%d,vchrmin:%d,vfast:%d\n", __func__,
            chr_cust_data.disable_charger, chr_cust_data.enable_pe_plus,
            chr_cust_data.power_path_support, chr_cust_data.max_charger_voltage,
            chr_cust_data.min_charger_voltage, chr_cust_data.fast_charge_voltage);
        LTRACEF("%s: usb:%d,ac:%d %d,nac:%d,cdp:%d,ta:%d,pd:%d,t:%d %d %d\n", __func__,
            chr_cust_data.usb_charger_current, chr_cust_data.ac_charger_current,
            chr_cust_data.ac_charger_input_current, chr_cust_data.non_std_ac_charger_current,
            chr_cust_data.charging_host_charger_current, chr_cust_data.ta_ac_charger_current,
            chr_cust_data.pd_charger_current, chr_cust_data.temp_t4_threshold,
            chr_cust_data.temp_t3_threshold, chr_cust_data.temp_t1_threshold);

        return 0;
    } else {
        dprintf(INFO, "%s: lk_charger is not found in dts!\n", __func__);
        return 1;
    }
}

void charger_init(void *fdt)
{
    int ret = 0;

    ret = mtk_charger_init();
#if (MULTI_EXT_MAIN_CHARGER)
    ext_first_mainchg_init_state = ret; // 记录主充电器初始化状态
    if (ret < 0) {
        LTRACEF("%s: mtk_charger_init failed, ret = %d;init sec\n", __func__, ret);
        ret = mtk_charger_sec_init();   // 主充电器失败，初始化备用充电器
        if (ret < 0)
            LTRACEF("%s: mtk_charger_init/sec all failed, ret = %d\n", __func__, ret);
    }
else
    if (ret < 0)
        LTRACEF("%s: mtk_charger_init failed, ret = %d\n", __func__, ret);
#endif

#if (CHARGER_ETA6947_SUPPORT)
    eta6947_probe();
#endif

#if (CHARGER_UPM6915_SUPPORT)
    upm6915_probe();
#endif


    /* set default values */
    init_charger_custom_data(); // 设置默认配置值

    if (fdt) {
        if (fdt_check_header(fdt) != 0)
            panic("Bad DTB header.\n");

        /* override default values by dts */
        init_cust_data_from_dt(fdt);    // 从设备树覆盖默认配置
    } else
        dprintf(CRITICAL, "[chg] fdt null, fall back to default\n");

    LTRACEF("%s: done\n", __func__);
}

void charger_enable_charging(bool enable)
{
    int ret = 0;

#if (MULTI_EXT_MAIN_CHARGER)
    if (ext_first_mainchg_init_state < 0)
    {
        ret = mtk_charger_sec_enable_charging(enable);
    }
    else
#endif
    ret = mtk_charger_enable_charging(enable);
    if (ret < 0)
        LTRACEF("%s: enable/disable charging failed, ret = %d\n", __func__, ret);
    else {
        if (enable)
            LTRACEF("%s: enable charging\n", __func__);
        else
            LTRACEF("%s: disable charging\n", __func__);
    }
}

void charger_enable_power_path(bool enable)
{
    int ret = 0;

    ret = mtk_charger_enable_power_path(enable);
    if (ret < 0)
        LTRACEF("%s: enable/disable power path failed, ret = %d\n", __func__, ret);
}

int charger_check_battery_plugout_once(bool *plugout_once)
{
    return mtk_charger_check_battery_plugout_once(plugout_once);
}

static void chr_power_off(void)
{
    platform_halt(HALT_ACTION_SHUTDOWN, HALT_REASON_LOWVOLTAGE);
}

static int pmic_get_auxadc_value(int channel, enum auxadc_val_type type)
{
    int ret, val = 0;

    ret = pmic_auxadc_read_value_ext(channel, &val, type);
    if (ret < 0)
        return ret;
    return val;
}

static int get_bat_volt(void)
{
    return pmic_get_auxadc_value(AUXADC_CHAN_BATADC, AUXADC_VAL_PROCESSED);
}

static int get_vsys_volt(void)
{
    struct auxadc *main_pmic;
    int val = 0;

    main_pmic = pmic_auxadc_get_by_name("main_pmic");
    pmic_auxadc_read_value(main_pmic, AUXADC_CHAN_VSYSSNS, &val, AUXADC_VAL_PROCESSED);

    return val;
}

static unsigned int get_chr_volt(void)
{
    unsigned int val = 0;

#if (MULTI_EXT_MAIN_CHARGER)
    if (ext_first_mainchg_init_state < 0)
    {
        mtk_charger_sec_get_vbus(&val);
    }
    else
#endif
    mtk_charger_get_vbus(&val);
    val /= 1000;

    return val;
}

static void select_charging_current_limit(int chr_type)
{
    int input_current_limit;
    int charging_current_limit;

    if (chr_type == STANDARD_HOST) {
        input_current_limit = chr_cust_data.usb_charger_current;
        charging_current_limit = chr_cust_data.usb_charger_current;
    } else if (chr_type == NONSTANDARD_CHARGER) {
        input_current_limit = chr_cust_data.non_std_ac_charger_current;
        charging_current_limit = chr_cust_data.non_std_ac_charger_current;
    } else if (chr_type == STANDARD_CHARGER) {
        input_current_limit = chr_cust_data.ac_charger_input_current;
        charging_current_limit = chr_cust_data.ac_charger_current;
    } else if (chr_type == CHARGING_HOST) {
        input_current_limit = chr_cust_data.charging_host_charger_current;
        charging_current_limit = chr_cust_data.charging_host_charger_current;
    } else {
        input_current_limit = chr_cust_data.usb_charger_current;
        charging_current_limit = chr_cust_data.usb_charger_current;
    }

#if (MULTI_EXT_MAIN_CHARGER)
    if (ext_first_mainchg_init_state < 0)
    {
        mtk_charger_sec_set_aicr(input_current_limit);
        mtk_charger_sec_set_ichg(charging_current_limit);
    }
    else {
        mtk_charger_set_aicr(input_current_limit);
        mtk_charger_set_ichg(charging_current_limit);
    }
#else
    mtk_charger_set_aicr(input_current_limit);
    mtk_charger_set_ichg(charging_current_limit);
#endif
}

static void reset_default_charging_current_limit(void)
{
    int input_current_limit;
    int charging_current_limit;

    input_current_limit = chr_cust_data.usb_charger_current;
    charging_current_limit = chr_cust_data.usb_charger_current;

#if (MULTI_EXT_MAIN_CHARGER)
    if (ext_first_mainchg_init_state < 0)   // 主充电器初始化失败
    {
        // 使用备用充电器设置电流
        mtk_charger_sec_set_aicr(input_current_limit);
        mtk_charger_sec_set_ichg(charging_current_limit);
    }
    else {
        // 使用主充电器设置电流
        mtk_charger_set_aicr(input_current_limit);
        mtk_charger_set_ichg(charging_current_limit);
    }
#else
    mtk_charger_set_aicr(input_current_limit);
    mtk_charger_set_ichg(charging_current_limit);
#endif
}

static void show_plug_out_notify(void)
{
    leds_on();
    mt_disp_show_charger_ov_logo();
    spin(4000000);
    leds_off();
    mt_disp_clear_screen(0);
}

static void show_low_battery_notify(void)
{
    leds_set(chr_cust_data.led_brightness);
    mt_disp_show_plug_charger();
    spin(4000000);
    leds_off();
    mt_disp_clear_screen(0);
}

static void check_charger_volt(void)
{
    int i;
    int chr_volt;

    for (i = 0; i < VBUS_CHECK_COUNT; i++) {
        chr_volt = get_chr_volt();
        if (chr_volt < chr_cust_data.min_charger_voltage) {
            LTRACEF("%s: vbus %d is less than %dmv, power off\n", __func__,
                    chr_volt, chr_cust_data.min_charger_voltage);
            chr_power_off();
        }
    }

    if (chr_cust_data.power_path_support == true) {
        chr_volt = get_chr_volt();
        if (chr_volt > chr_cust_data.max_charger_voltage) {
            LTRACEF("%s: vbus %d over max voltage %d, power off\n",  __func__,
                    chr_volt, chr_cust_data.max_charger_voltage);
            show_plug_out_notify();
            chr_power_off();
        }
    }
}

static void show_charging_anime(void)
{
    int i, j;
    bool stop = false;
    /* set LCD brightness */
    leds_set(chr_cust_data.led_brightness); // 设置LED亮度

    /* set blinking times */
    for (i = 0; i < chr_cust_data.blinking_times && !stop; i++) {
        mtk_wdt_restart_timer();
        mt_disp_show_charging(i % 2);   // 显示充电画面（交替显示）

        for (j = 0; j < 2 && !stop; j++) {
            /* set blinking period */
            spin(chr_cust_data.blinking_period * 1000);
            check_charger_volt();
            if (get_pmic_key_event_status(POWER_KEY)) {
                clr_pmic_key_event_status(POWER_KEY);
                stop = true;    // 电源键按下，停止动画
            }
        }
    }
    leds_off(); // 关闭LED
    mt_disp_clear_screen(0);    // 清屏
#if (MULTI_EXT_MAIN_CHARGER)
    if (ext_first_mainchg_init_state < 0)
    {
        mtk_charger_sec_reset_wdt();
    }
    else
#endif
    mtk_charger_reset_wdt();
}

static bool is_power_path_supported(void)
{
    return chr_cust_data.power_path_support;
}

static bool is_disable_charger(void)
{
    return chr_cust_data.disable_charger;
}

static bool is_battery_on(void)
{
    if (get_bat_volt() < BATTERY_ON_THRESHOLD)
        return false;
    else
        return true;
}

static bool is_low_battery(int val)
{
    bool bat_low = false;

    if (val < chr_cust_data.boot_battery_voltage)
        bat_low = true;
    else
        bat_low = false;

    /* The battery voltage low than boot voltage plus 200mV */
    if (val < (chr_cust_data.boot_battery_voltage + 200))
        chr_cust_data.battery_volt_is_low = true;

    dprintf(INFO, "%s: %d vbat:%d th:%d\n", __func__,
            bat_low, val, chr_cust_data.boot_battery_voltage);

    return bat_low;
}

/*
 * When is_battery_voltage_low() returns true, it means the battery
 * voltage is too low to enable dvfs at high frequency safely.
 */
bool is_battery_voltage_low(void)
{
    return chr_cust_data.battery_volt_is_low;
}

/*
 MTK（联发科）平台在设备关机充电（KPOC, Kernel Power Off Charge）或低电量启动时，用于管理充电流程的核心函数。
 它运行在 bootloader 阶段（如 LK），负责：
    检测充电状态、控制充电使能、显示充电动画、防止过热，并逐步提升电量直到满足开机条件。

开始充电循环
    ↓
检查看门狗和充电模式
    ↓
安全检查：电压、温度
    ↓
根据条件设置充电电流
    ↓
启用充电
    ↓
显示充电动画（条件满足时）
    ↓
监控充电电流和电池状态
    ↓
标准充电器特殊处理
    ↓
更新电池电压
    ↓
电压达到目标？ → 是 → 结束充电
    ↓
否 → 继续循环
*/
static void check_charging_status(void)
{
    int i, ret = 0;
    int bat_val = 0;
    int temperature = 0;
    int bat_current = 0;
    bool curr_sign = false; // 电流方向（充电/放电）
    static bool is_first = true;    // 标记是否首次显示动画
    int chr_type = pl_boottags_get_charger_type();  // 获取充电器类型

    dprintf(INFO, "%s: charger_type: %d\n", __func__, chr_type);
    if(chr_type == 0) { //如果 chr_type == 0（未知或未识别），先关闭充电。即使不是 chr_type == 0，后续也会显式开启充电，这里只是初始化安全状态。
#if (MULTI_EXT_MAIN_CHARGER)
        if (ext_first_mainchg_init_state < 0)
        {
            ret = mtk_charger_sec_enable_charging(false);
        }
        else
#endif
    ret = mtk_charger_enable_charging(false);
    }
    if (ret < 0)
        LTRACEF("%s: disable charging failed, ret = %d\n", __func__, ret);

    bat_val = get_bat_volt();
    LTRACEF("%s: check VBAT=%d mV with %d mV\n", __func__, bat_val, chr_cust_data.boot_battery_voltage);

    clr_pmic_key_event_status(POWER_KEY);   // 清除电源键状态,防止旧的按键事件影响后续判断（比如用户之前按过电源键）。

    while (bat_val < chr_cust_data.boot_battery_voltage) {  //循环条件：电池电压低于启动电压阈值（如3450mV）
        mtk_wdt_restart_timer();        // 喂狗防止看门狗复位,必须频繁喂狗，因为这个循环可能运行几十秒甚至几分钟
#if (MULTI_EXT_MAIN_CHARGER)
        if (ext_first_mainchg_init_state < 0)
        {
            mtk_charger_sec_reset_wdt();    
        }
        else
#endif
        mtk_charger_reset_wdt();    // 充电器看门狗
        mtk_charger_check_charging_mode();  // 检查充电模式

#if (MULTI_EXT_MAIN_CHARGER)
        if (LOCAL_TRACE){
            if (ext_first_mainchg_init_state < 0)
            {
                mtk_charger_sec_dump_register();
            }
            else
                mtk_charger_dump_register();
        }
#else
        if (LOCAL_TRACE)
            mtk_charger_dump_register();
#endif

        check_charger_volt();   // 检查VBUS电压是否在安全范围,防止虚接或劣质充电器导致问题

        temperature = force_get_tbat(true);
        LTRACEF("%s: T=%d\n", __func__, temperature);
        if (temperature > chr_cust_data.temp_t4_threshold) {
            LTRACEF("%s: Battery over Temperature or NTC fail %d %d!!\n\r",
                    __func__, temperature, chr_cust_data.temp_t4_threshold);
            break;  //触发后直接 break 跳出循环 → 停止充电，防止起火爆炸
        }

        if (bat_val < chr_cust_data.fast_charge_voltage ||
                temperature > chr_cust_data.temp_t3_threshold ||
                temperature < chr_cust_data.temp_t1_threshold)
            reset_default_charging_current_limit();
        else
            select_charging_current_limit(chr_type);    // 根据充电器类型设置合适电流

#if (MULTI_EXT_MAIN_CHARGER)
        if (ext_first_mainchg_init_state < 0)
        {
            ret = mtk_charger_sec_enable_charging(true);
        }
        else
#endif
        ret = mtk_charger_enable_charging(true);
        if (ret < 0)
            LTRACEF("%s: enable charging failed, ret = %d\n", __func__, ret);

        // 充电动画显示
        if (chr_cust_data.enable_anime && bat_val > chr_cust_data.fast_charge_voltage) {
            if (is_first) {
                show_charging_anime();  // 第一次显示充电动画
                is_first = 0;
            }
            // 动画模式下的轮询（支持按键中断）
            for (i = 0; i < MAX_SLEEP_LOOP; i++) {
                mtk_wdt_restart_timer();
                check_charger_volt();
                /* set polling period */
                spin(1000000);
                if (get_pmic_key_event_status(POWER_KEY)) {
                    clr_pmic_key_event_status(POWER_KEY);
                    show_charging_anime();  // 按键重新显示动画
                }
            }
        } else {
            for (i = 0; i < MAX_SLEEP_LOOP; i++) {
                mtk_wdt_restart_timer();
                check_charger_volt();
                /* set polling period */
                spin(1000000);
            }
        }

        if (is_battery_on()) {
            gauge_get_current(&curr_sign, &bat_current);
            bat_current = bat_current / 10;
            dprintf(INFO, "%s: IBAT=%d\n", __func__, curr_sign ? bat_current : -1 * bat_current);
        } else {
            dprintf(INFO, "%s: no battery, exit charging\n", __func__);
            break;
        }

        //特殊处理：标准充电器达到快充电压后暂停充电
        if (chr_type == STANDARD_CHARGER && bat_val > chr_cust_data.fast_charge_voltage) {
#if (MULTI_EXT_MAIN_CHARGER)
            if (ext_first_mainchg_init_state < 0)
            {
            ret = mtk_charger_sec_enable_charging(false);
            }
            else
#endif
            ret = mtk_charger_enable_charging(false);
            if (ret < 0)
                LTRACEF("%s: disable charging failed, ret = %d\n", __func__, ret);
        }

        bat_val = get_bat_volt();   // 重新读取电池电压，用于循环条件判断,只要电池电压还没达到可以开机的水平，就一直循环充电
        dprintf(INFO, "%s: check VBAT=%d mV with %d mV, start charging...\n", __func__,
                bat_val, chr_cust_data.boot_battery_voltage);
    }
    mtk_wdt_restart_timer();
    reset_default_charging_current_limit(); // 重置为默认充电电流
}

/*
判断设备是否因电量过低而需要关机或进入充电模式，并处理极端温度下的安全保护逻辑

开始 check_low_battery()
   │
   ├─→ 读取电压（VBAT, VBUS, VSYS）和温度
   │
   ├─→ 若启用 VSYS 检查且 VSYS 足够高 → 跳过低电检查（return）
   │
   └─→ 检查电池温度是否异常？
        │
        ├─→ 是 → 进入温度保护循环：
        │      ├─→ 无充电器？ → 关机
        │      ├─→ 有充电器 + 按键？ → 显示警告
        │      └─→ 等待 1s，重读温度 → 直到温度正常
        │
        └─→ 否 → 检查是否低电量？
               ├─→ 是 + 有充电器？ → 进入充电流程
               └─→ 是 + 无充电器？ → 显示低电提示并关机
*/
static void check_low_battery(void)
{
    int bat_vol;
    int chr_vol;
    int vsys_vol;
    int bat_temper_value;

    bat_vol = get_bat_volt();   // 电池电压
    chr_vol = get_chr_volt();   // 充电器电压（VBUS）
    vsys_vol = get_vsys_volt(); // 系统电压
    bat_temper_value = force_get_tbat(true);    // 强制获取电池温度（通过 NTC 热敏电阻）

    LTRACEF("%s: check vsys=%d mV\n", __func__, vsys_vol);

    /*
    特殊条件：是否绕过电池电压检查？
    条件：
        电池电压 < 2500mV（BATTERY_ON_THRESHOLD）
        VSYS电压 > 最小系统电压（通常4000mV）
    作用：当电池电压很低但系统电压正常时，允许绕过电池检查继续启动（可能使用外部电源直接供电）。无需立即关机
    */
    if (chr_cust_data.enable_check_vsys == true) {
        if (bat_vol < BATTERY_ON_THRESHOLD &&
            vsys_vol > chr_cust_data.min_vsys_voltage) {
            dprintf(INFO, "%s: bypass vbat check\n", __func__);
            return;
        }
    }

    LTRACEF("%s: vbat:%d vbus:%d\n", __func__, bat_vol, chr_vol);

    /*
    温度安全监控循环
    如果电池太热或太冷，就进入一个无限循环，直到温度恢复正常，期间禁止充电或开机。
    */
    while ((bat_temper_value > EM_MAX_TEMP_THRESHOLD) || (bat_temper_value < EM_MIN_TEMP_THRESHOLD)) {
        mtk_wdt_restart_timer();    //因为此循环可能长时间运行，必须定期喂狗，否则看门狗会触发重启
        /*
        如果没有充电器（chr_vol <= CHARGER_ON_THRESHOLD）：
        打印严重警告日志；
        显示“请拔掉充电器”提示图（show_plug_out_notify()）；
        直接调用 chr_power_off() 关机。
        📌 原因：即使没充电，电池也超温 → 可能是环境问题或电池故障 → 安全起见强制关机。

        如果有充电器插入（chr_vol > CHARGER_ON_THRESHOLD）：
        进一步判断是否按了电源键：  
        只有用户按下电源键时才响应；
        显示警告图标（可能是火焰或雪花标志）；
        提示用户：“电池温度异常，请拔出充电器”。
        ⚠️ 此时不关机，而是让用户看到警告，等待温度恢复。
        */
        if (chr_vol <= CHARGER_ON_THRESHOLD) {
            dprintf(CRITICAL, "[NTC]bat_temper_value:%d above %d or below %d, system will power off. \n", bat_temper_value,EM_MAX_TEMP_THRESHOLD,EM_MIN_TEMP_THRESHOLD);
            show_plug_out_notify(); // 显示提示
            chr_power_off();    // 安全关机
        } else {
				if(is_pmic_key_pressed(POWER_KEY)){ // 检查电源键是否按下
					if (bat_temper_value > EM_MAX_TEMP_THRESHOLD) {
						dprintf(CRITICAL, "[NTC]bat_temper_value:%d above %d, show warning logo wait... \n", bat_temper_value,EM_MAX_TEMP_THRESHOLD);
						show_plug_out_notify();
					} else if (bat_temper_value < EM_MIN_TEMP_THRESHOLD) {
						dprintf(CRITICAL, "[NTC]bat_temper_value:%d below %d, show warning logo wait... \n", bat_temper_value,EM_MIN_TEMP_THRESHOLD);
						show_plug_out_notify();
					}
				}
        }
        thread_sleep(1000); // 休眠1秒
        bat_temper_value = force_get_tbat(true);    // 重新读取温度
        chr_vol = get_chr_volt();                   // 重新读取充电器电压
        //继续循环检测，直到温度回到正常范围。
        dprintf(CRITICAL, "[NTC]vbus = %d,t = %d\n",chr_vol,bat_temper_value);
    }
    mtk_wdt_restart_timer();
    //温度正常后：检查是否低电量
    if (is_low_battery(bat_vol)) {      // 检查是否低电量
        if (chr_vol > CHARGER_ON_THRESHOLD)
            check_charging_status();    // 有充电器：开始充电
        else {
            LTRACEF("%s: no charger, power off\n", __func__);
            show_low_battery_notify();  // 显示低电量提示
            chr_power_off();    // 安全关机
        }
    }
}

/*
power_measure_loop() 是一个在 bootloader 阶段短暂运行的电流采样循环，用于：

每隔约 1 秒读取一次电池电流；
区分充电/放电方向；
将电流值转换为更易读的单位（如 mA）；
输出调试信息；
在长时间操作中防止看门狗超时复位。
*/
static void power_measure_loop(void)
{
    int bat_current = 0, cnt = 0;
    bool curr_sign = false;

    while (cnt < LOOP_MAX_COUNT) {
        mtk_wdt_restart_timer();
#if (MULTI_EXT_MAIN_CHARGER)
        if (ext_first_mainchg_init_state < 0)
        {
            mtk_charger_sec_reset_wdt();
        }
        else
#endif
        mtk_charger_reset_wdt();
        spin(1000000);
        dprintf(INFO, "%s: loop in lk2 charger start.\n", __func__);
        gauge_get_current(&curr_sign, &bat_current);
        bat_current = bat_current / 10;
        dprintf(INFO, "%s: IBAT=%d\n", __func__, curr_sign ? bat_current : -1 * bat_current);
        cnt++;
    }
}

void mtk_charger_start(void)
{
    if (chr_cust_data.enable_power_measure)
        power_measure_loop();

    if (chr_cust_data.disable_charger == false)
        check_low_battery();
    else
        dprintf(INFO, "%s: skip mtk_charger_start\n", __func__);

    LTRACEF("%s: end\n", __func__);
}

/*
这个函数 mtk_charger_kpoc_check() 用于检查是否需要进行KPOC（Kernel Power On Charging）充电。
该函数用于判断设备在关机时插上充电器，是否应启动并显示充电界面（KPOC）。只有在支持关机充电、正常启动模式、且因插入USB而启动时，才会返回 true。
*/
bool mtk_charger_kpoc_check(void)
{
    uint32_t boot_mode = platform_get_boot_mode();  // 获取启动模式,比如正常启动、 recovery 模式、fastboot 模式
    uint32_t boot_reason = platform_get_boot_reason();  // 获取启动原因,例如是因为按了电源键、插入了充电器、电池上电
    int off_mode_status = 1;    //用于存储“关机充电模式是否启用”的状态，默认设为 1（表示启用）

    LTRACEF("platform_get_boot_mode = %d\n", boot_mode);
    LTRACEF("platform_get_boot_reason = %d\n", boot_reason);

    off_mode_status = get_off_mode_charge_status();
    if (off_mode_status == 0)
        return false;   // 如果禁用关机充电，直接返回false

    if (boot_mode == NORMAL_BOOT && boot_reason == BR_USB)
        return true;

    return false;
}
