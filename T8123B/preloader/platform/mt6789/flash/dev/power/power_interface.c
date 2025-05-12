#include <stdbool.h>
#include <debug.h>
#include <sys/types.h>

#if INTERFACE_POWER_IMPL

extern void PMIC_enable_long_press_reboot(void);
extern void rtc_bbpu_power_on(void);
extern void rtc_bbpu_power_down(void);
extern int hw_check_battery(void);
extern int get_i_sense_volt(int times);
extern int mtk_da_chg_set_aicr(unsigned int mA);
extern int mtk_da_chg_enable_charging(bool en);
extern int mtk_da_subpmic_init(void);

BOOL dev_power_battery_exists() {
#if (!DA_MODE_FPGA)
    return hw_check_battery();
#else
  LOGI("FPGA not support hw_check_battery.");
  return false;
#endif
}

int dev_power_get_battery_voltage() {
#if (!DA_MODE_FPGA)
    uint32_t Vbat_value= 0;
    Vbat_value = get_i_sense_volt(1);
    return Vbat_value;
#else
  return 0;
#endif
}

void dev_power_set_reset_key() {
#if (!DA_MODE_FPGA)
    PMIC_enable_long_press_reboot();
#endif
}

void dev_power_disable_charging_battery() {
#if (!DA_MODE_FPGA)
    mtk_da_chg_enable_charging(false);
#endif
}

void dev_power_set_charging_limit(int current_mA) {
#if (!DA_MODE_FPGA)
    mtk_da_chg_set_aicr(current_mA);
#endif
}

int dev_power_charger_init() {
#if (!DA_MODE_FPGA)
    return mtk_subpmic_init();
#endif
}

int dev_is_power_charger_on() {
#if (!DA_MODE_FPGA)
  return upmu_is_chr_det();
#else
  return 1;
#endif
}

void dev_power_shutdown() {
#if (!DA_MODE_FPGA)
  pl_power_off();
#endif
}

void dev_power_bbpu_clear_key() {
#if (!DA_MODE_FPGA)
#if 1
  rtc_bbpu_power_clear();
#endif
#endif
}

#else

BOOL dev_power_battery_exists() { return 0; }
int dev_power_get_battery_voltage() { return 0; }
void dev_power_set_reset_key() {}
void dev_power_disable_charging_battery() {}
void dev_power_set_charging_limit(int current_mA) {}
int dev_power_charger_init() { return 0; }
int dev_is_power_charger_on() { return 0; }
void dev_power_shutdown() {}
void dev_power_bbpu_clear_key() {}

#endif
