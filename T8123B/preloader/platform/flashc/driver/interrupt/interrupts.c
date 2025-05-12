#include "boot/dev_interface/irq_interface.h"
#include "interrupts.h"
#include <debug.h>

enum handler_return platform_irq(struct arm_iframe *frame)
{
    if (interface_irq_handler() == 0)
        return INT_RESCHEDULE;
    else
        return INT_NO_RESCHEDULE;
}

void platform_fiq(struct arm_iframe *frame)
{
    //do nothing.
    return;
}

