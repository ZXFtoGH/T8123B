#include "stdint.h"
#include "gpio.h"
#include "debug.h"
#include <driver/register_act_util.h>
#include <boot/dev_interface/gpio_interface.h>


enum BOOT_TRAP_E gpio_get_boot_trap() {
  int val = 0;
#if (!DA_MODE_FPGA)
  val = DRV_Reg32(TRAP_TEST_RD) & BOOT_TRAP_MASK;
#else
  val = BOOT_FROM_UFS;
#endif
  LOGI("!boot trap2: 0x%x=0x%x\n", TRAP_TEST_RD, val);

  return (val == BOOT_FROM_EMMC)
             ? BOOT_TRAP_EMMC
             : (val == BOOT_FROM_UFS)
                   ? BOOT_TRAP_UFS
                   : (val == BOOT_FROM_EMMC_UFS) ? BOOT_TRAP_EMMC_UFS
                                                 : BOOT_TRAP_NOT_SET;
}




