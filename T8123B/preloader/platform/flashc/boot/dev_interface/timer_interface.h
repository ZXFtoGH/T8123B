#include "stdint.h"

int interface_timer_init(int timer_quantum, void (*timer_cb)(void*));
void interface_timer_ack_irq();
void interface_timer_ack_gic();
void interface_timer_one_shot();
uint32_t interface_timer_get_defined_irq_id();




