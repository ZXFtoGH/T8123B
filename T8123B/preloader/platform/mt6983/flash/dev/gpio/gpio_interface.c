#include "stdint.h"
#include "gpio.h"
#include "debug.h"
#include <driver/register_act_util.h>
#include <boot/dev_interface/gpio_interface.h>


enum BOOT_TRAP_E gpio_get_boot_trap()
{
  int val = BOOT_FROM_UFS;

  LOGI("!boot trap2: 0x%x\n", val);

  return (val == BOOT_FROM_EMMC) ? BOOT_TRAP_EMMC :
         (val == BOOT_FROM_UFS) ? BOOT_TRAP_UFS : BOOT_TRAP_NOT_SET;
}




