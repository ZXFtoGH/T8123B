#include "boot/system_objects.h"
#include "error_code.h"
#include "lib/string.h"
#include "debug.h"
#include "arch/thread.h"
#include <boot/dev_interface/power_interface.h>

status_t monitor_system_status(void *arg) {
  while (1) {
    LOGI("I'm alive.\n");
    if(!dev_is_power_charger_on())
    {
       LOGI("charger out, power off\n");
       dev_power_shutdown();
    }

    thread_sleep(1000);
  }
  return STATUS_OK;
}