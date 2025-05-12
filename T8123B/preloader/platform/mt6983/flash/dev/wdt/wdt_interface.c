#include "boot/dev_interface/wdt_interface.h"
#include <debug.h>
#include <sys/types.h>

#if INTERFACE_WDT_IMPL

extern void mtk_wdt_disable(void);
extern void mtk_wdt_enable(void);
extern void mtk_wdt_init(void);
extern void mtk_wdt_restart(void);
extern void mtk_wdt_reset(char mode);
extern void mtk_wdt_sw_reset(void);
extern void mtk_mark_da_stage(void);
extern void mtk_enter_fastboot(void);

void dev_wdt_init() { mtk_wdt_init(); }

void dev_wdt_kick() { mtk_wdt_restart(); }

void dev_wdt_reset(const char *mode) {
  mtk_mark_da_stage();
  // do not enter charging mode, reboot directly.
  mtk_wdt_enable();
  mtk_wdt_restart();
  mtk_wdt_sw_reset();
}

void dev_wdt_enable() { mtk_wdt_enable(); }
void dev_wdt_disable() { mtk_wdt_disable(); }
void dev_wdt_enter_fastboot() { mtk_enter_fastboot(); }

#else

void dev_wdt_init() {}
void dev_wdt_kick() {}
void dev_wdt_reset(const char *mode) {}
void dev_wdt_enable() {}
void dev_wdt_disable() {}
void dev_wdt_enter_fastboot() {}

#endif
