#ifndef _BATTERY_H_
#define _BATTERY_H_

extern int hw_check_battery(void);
extern void pl_battery_init(bool force_init);
extern void pl_check_bat_protect_status(void);
extern void fg_set_2sec_reboot(void);
extern int fg_get_vbat(void);
extern int fg_get_shutdowntime(void);

#endif // #ifndef _BATTERY_H_
