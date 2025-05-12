#include <sys/types.h>
#include <debug.h>
#include <err.h>
#include "timer.h"
#include "boot/dev_interface/timer_interface.h"

#define TIMER_QUANTUM 10  //10ms
static volatile time_t ticks_now = 0;
static platform_timer_callback time_callback;

static enum handler_return timer_irq()
{
   ticks_now += TIMER_QUANTUM;
   return time_callback(NULL, ticks_now);
}

void timer_slice_scheduler(void* cookie)
{
   interface_timer_ack_irq();

   timer_irq();

   //LXO: not use this anymore, irq handler will do it.
   //interface_timer_ack_gic();

   interface_timer_one_shot();
}

status_t launch_periodic_irq_timer(platform_timer_callback callback)
{
   time_callback = callback;
   interface_timer_init(TIMER_QUANTUM, timer_slice_scheduler);

   return NO_ERROR;
}

time_t current_time(void)
{
   return ticks_now;
}

bigtime_t current_time_hires(void) //tick to time
{
   return dev_timer_get_time_us();
}

time_t current_time_stamps(void) //tick to time
{
   return (time_t)dev_timer_get_time_us();
}

