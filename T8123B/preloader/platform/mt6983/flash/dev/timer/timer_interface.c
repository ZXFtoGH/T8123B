#include "boot/dev_interface/timer_interface.h"
#include "stdint.h"
#include <sys/types.h>

#if INTERFACE_TIMER_IMPL

extern void mtk_timer_init(void);
extern u32  gpt4_get_current_tick(void);
extern U32 gpt4_tick2time_us(uint32_t tick);
extern void gpt_busy_wait_ms(uint32_t timeout_ms);

int interface_timer_init(int timer_quantum, void (*timer_cb)(void*)) {
  mtk_oneshot_timer_init(timer_quantum, timer_cb);
  mtk_oneshot_timer_enable();
  return 0;
}

void interface_timer_ack_irq() { mtk_oneshot_timer_ack_irq(); }

void interface_timer_ack_gic() {}

void interface_timer_one_shot() { mtk_oneshot_timer_enable(); }

uint32_t interface_timer_get_defined_irq_id() {
  return mtk_oneshot_timer_get_irq_id();
}

void dev_timer_init() { mtk_timer_init(); }
uint64_t dev_timer_get_time_us() {
  uint32_t tick, time;

  tick = gpt4_get_current_tick();

  time = gpt4_tick2time_us(tick);

  return time;
}

#else

int interface_timer_init(int timer_quantum, void (*timer_cb)(void*)) { return 0; }
void interface_timer_ack_irq() {}
void interface_timer_ack_gic() {}
void interface_timer_one_shot() {}
uint32_t interface_timer_get_defined_irq_id() { return 0; }

void dev_timer_init() {}
uint64_t dev_timer_get_time_us() { return 0; }
void dev_timer_mdelay(unsigned int msec) {}

#endif
