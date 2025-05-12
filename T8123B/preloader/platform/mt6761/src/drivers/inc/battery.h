#ifndef _PL_MT_BATTERY_H_
#define _PL_MT_BATTERY_H_

int fg_get_vbat(void);
void pl_battery_init(bool force_init);
int get_bat_sense_volt(int times);

#endif	// _PL_MT_BATTERY_H_
