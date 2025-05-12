#include "boot/system_objects.h"
#include "error_code.h"
#include "lib/string.h"
#include "debug.h"
#include "arch/thread.h"
#include <boot/dev_interface/power_interface.h>

status_t monitor_system_status(void *arg) {
  while (1) {
    LOGI("I'm alive.\n");
    // This project and sold merchandise(M.2) do not connect pin detecting usb
    // power. because of lack of pins. causing dev_is_power_charger_on always
    // return 0. so close it. Internal EVB has this pin, removing this function
    // is acceptable on internal EVB.

    if(!dev_is_power_charger_on())
    {
       LOGI("charger out, power off\n");
       dev_power_shutdown();
    }

    thread_sleep(1000);
  }
  return STATUS_OK;
}

/*
void set_reboot_mode(int mode)
{
   if(mode == BM_FASTBOOT)//FASTBOOT MODE
   {
       DRV_SetReg32(MTK_WDT_NONRST_REG2, DRV_Reg32(MTK_WDT_NONRST_REG2) | (1<<2));
   }
   return;
}*/