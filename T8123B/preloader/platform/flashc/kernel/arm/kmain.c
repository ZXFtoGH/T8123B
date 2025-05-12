
#include "boot/hal.h"
#include <arch.h>
#include <boot/arguments.h>
#include <boot/system_objects.h>
#include <debug.h>
#include <kernel/dpc.h>
#include <arch/thread.h>
#include <kernel/timer.h>
#include <lib/heap.h>
#include <platform.h>

status_t monitor_system_status(void *arg);

extern uint32_t BOOT_ARGUMENT_LOCATION;

int bootstrap2(void *arg);

/* called from crt0.S */
void kmain(void) __NO_RETURN __EXTERNALLY_VISIBLE;

void kmain(void) {
  memcpy(&sysob_bootstrap2_arg, (void *)BOOT_ARGUMENT_LOCATION,
         sizeof(struct bootstrap2_arg));
  if (sysob_bootstrap2_arg.magic != BOOT2_ARG_MAGIC) {
    LOGE("kmain argument magic error. halt.\n\n");
    while (1)
      ;
  }

  memcpy(&sysob_ram.dram, &(sysob_bootstrap2_arg.dram_info),
         sizeof(struct ram_item_struct));
  memcpy(&sysob_runtime_params, &(sysob_bootstrap2_arg.runtime_params),
         sizeof(struct runtime_parameters));

  dev_timer_init();
  heap_init();
  init_system_objects();

  // 0.open usb and uart log.
  com_interface.init_uart();
  com_interface.init();
  LOGI("\n***Enter 2nd-DA.***\n");

  platform_early_init();
  LOGI("\n***kmain-platform_early_init pass(irq)***\n");

#if (!DA_ENABLE_MULTI_THREAD)
  LOGI("\n*** No thread bootstrap2 start.***\n");
  bootstrap2((void *)BOOT_ARGUMENT_LOCATION);
  while (1)
    ;
#else
  interface_gic_init();

  thread_init_early();
  arch_early_init();

  thread_init();

  timer_init();
  LOGI("\n***timer_init***\n");

  dpc_init();

  // create a thread to monitor system status
  thread_t *t1 = thread_create("monitor", &monitor_system_status, NULL,
                               DEFAULT_PRIORITY, 8192);
  if (t1 == 0) {
    LOGE("thread_create FAIL.");
    return;
  }
  thread_resume(t1);
  // create a thread to complete system initialization
  thread_t *t2 =
      thread_create("bootstrap2", &bootstrap2, (void *)BOOT_ARGUMENT_LOCATION,
                    DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
  if (t2 == 0) {
    LOGE("thread_create FAIL.");
    return;
  }
  thread_resume(t2);

  exit_critical_section();

  // become the idle thread
  LOGI("\n***Main idle***\n");
  thread_become_idle();
#endif
}
