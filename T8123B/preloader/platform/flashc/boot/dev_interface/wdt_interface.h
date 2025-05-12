#pragma once

void dev_wdt_init();
void dev_wdt_kick();
void dev_wdt_reset(const char* mode);
void mtk_mark_da_stage(void);
void dev_wdt_enable();
void dev_wdt_disable();
void dev_wdt_enter_fastboot();
