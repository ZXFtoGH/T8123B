#include "boot/dev_interface/irq_interface.h"
#include "error_code.h"
#include "stdint.h"

#if INTERFACE_GIC_IMPL

extern void platform_init_interrupts(void);
extern int32_t irq_handler(void);

int interface_irq_handler() {

 return irq_handler(); 
}

int interface_gic_init() {

  platform_init_interrupts();
  return 0;
}

#else

int interface_gic_init() { return 0; }
int interface_irq_handler() { return 0; }

#endif
