#pragma once
#include <stdint.h>

void dev_timer_init();
uint64_t dev_timer_get_time_us();
void dev_timer_mdelay(unsigned int msec);
void dev_timer_udelay(unsigned int usec);

