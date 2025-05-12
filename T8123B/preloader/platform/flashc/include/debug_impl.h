#ifndef __DEBUG_IMPL_H
#define __DEBUG_IMPL_H
#include <sys/types.h>

int _dputs(const uint8_t *str);


void set_log_level(uint32_t level);
uint32_t get_log_level();
int send_da_alive();

#endif