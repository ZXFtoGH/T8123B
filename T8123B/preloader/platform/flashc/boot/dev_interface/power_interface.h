#pragma once
#include <stdint.h>

BOOL dev_power_battery_exists();
int dev_power_get_battery_voltage();
void dev_power_bbpu_clear_key();
void dev_power_set_reset_key();
void dev_power_disable_charging_battery();
void dev_power_set_charging_limit(int current_mA);
int dev_is_power_charger_on();
void dev_power_shutdown();


