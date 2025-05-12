#include <boot/security_export/anti_rollback.h>
#include "boot/hal.h"
#include "boot/handler.h"
#include "boot/system_objects.h"
#include "debug.h"
#include "error_message.h"
#include "lib/heap.h"
#include <stdint.h>

uintptr_t __attribute__((section(".section_end"))) all_section_end =
    0xAAAA;
char __attribute__((section(".da_2nd_hash_section")))
g_da_loop_hash[48] = {0xBE, 0xBF};

const char *slogon = "\n\n"
                     " ______ __     __                                  \n"
                     "|      |  |--.|__|.--------.---.-.-----.----.---.-.\n"
                     "|   ---|     ||  ||        |  _  |  -__|   _|  _  |\n"
                     "|______|__|__||__||__|__|__|___._|_____|__| |___._|\n";

int main(int arg_addr, char **arg_size) {
  // force to 4 bytes align which is required by BROM
  // these 2 lines must exist, or gcc will remove unused sections.
  // cause error.
  all_section_end = 0x4567;
  all_section_end = (uintptr_t)g_da_loop_hash;

  // 0. if parameter has magic number, it means download mode is prelaoder mode,
  // it will skip pll and dram init
  if (((arg_addr & 0x03) == 0) && (*(int *)arg_addr == DA_ARG_MAGIC)) {
    memcpy((void *)&sysob_bootstrap1_arg, (void *)arg_addr,
           sizeof(struct bootstrap1_arg));
  } else {
    // brom mode must clear as 0,bcoz brom will leave this zone trash data, for
    // pll and dram init;
    memset((void *)&sysob_bootstrap1_arg, 0, sizeof(struct bootstrap1_arg));
  }

#if (DA_MODE_FPGA)
  // 1.FPGA no need init PLL and DRAM
  sysob_bootstrap1_arg.flags |= DA_FLAG_PLL_ALREADY_INIT;
  sysob_bootstrap1_arg.flags |= DA_FLAG_EMI_ALREADY_INIT;
#endif
  // 3.
  heap_init();
  init_system_objects();

  dev_timer_init();
  // 4.init com, usb	 ATTENTION: NO LOG BEFORE THIS CODE.
  com_interface.init_uart();

  LOGI(slogon);
  LOGI("\nBUILD TIME [%s %s]\n", __DATE__, __TIME__);

  if (sysob_bootstrap1_arg.flags == 0)
    LOGI("\n***BROM Bootup.***\n");
  else
    LOGI("\n***PRELOADER Bootup.***\n");

#if (!DA_MODE_FPGA)
  if (NEED_PLL_SETTING) {
    dev_pll_init();
    LOGI("\n***PLL Init done.***\n");
  }
#endif

  com_interface.init();
  LOGI("\n***USB/PCIE Init done.***\n");

#if (!DA_MODE_FPGA)
  if (sysob_bootstrap1_arg.flags == 0) {
    dev_i2c_init();
    dev_pmic_init();
    dev_power_set_reset_key();
    LOGI("\n***1.PMIC Init & Set Reset Key done.***\n");
  } else
    LOGI("\n***1.Skip PMIC Init & Set Reset Key.***\n");
#endif

  dev_wdt_init();
  LOGI("\n***WDT Init done.***\n");
#if (DA_ENABLE_SECURITY) && (DA_VER_CHECK_EN)
  if (da_version_check()) {
    LOGE("***Invalid DA version.***\n");
    sysob_runtime_params.hw_init_status = STATUS_ERR;
    set_error_msg("Invalid DA Version");
  } else {
    LOGE("DA version check pass.\n");
  }
#else
  LOGE("[SEC] Warrning! Bypass DA version check.\n");
#endif
  if (sysob_runtime_params.hw_init_status == STATUS_OK) {
    sysob_runtime_params.hw_init_status = platform_init_da();
  }

  LOGI("\n***platform_init_da done.***\n");
  // register function commands.
  dagent_register_commands();

  LOGI("\n***dagent_command_loop:***\n");
  // enter main command loop. never returned.
  dagent_handler();
  return 0;
  // the end.
}
