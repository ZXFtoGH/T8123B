#include "boot/dev_interface/gpt_timer_interface.h"
#include "boot/dev_interface/pcie_interface.h"
#include "boot/hal.h"
#include "debug.h"
#include "error_code.h"
#include "include/assert.h"

#if INTERFACE_PCIE_IMPL

#define PCIE_MAC_REG 0x11280D1C
#define HOST_EVENT_MASK 0xF0000000 //[28:31]
#define HOST_EVENT_SHIFT 28
#define BROM_EVENT_MASK 0x70 //[4:6]
#define BROM_EVENT_SHIFT 4

#define HOST_EV_REBOOT_READY 1
#define HOST_EV_BROM_PCIE_PORT_ACTIVE 2

#define BROM_EV_NORMAL 0
#define BROM_EV_RECV_CMD_JMP_BL 1
#define BROM_EV_TIMEOUT 2
#define BROM_EV_RECV_CMD_JMP_DA 3
#define BROM_EV_CREATE_DL_PORT 4
#define BROM_EV_NORITY_HOST_REBOOT 7

BOOL dev_is_pcie_active() {
  // Host pcie driver set PCIE_MAC_REG after BROM sent jump DA command, and need
  // about 50ms no mdelay will cause detection fail.
  mdelay(100);
  uint32_t *reg = PCIE_MAC_REG;
  uint32_t active = ((*reg) & HOST_EVENT_MASK) >> HOST_EVENT_SHIFT;

  return (active == HOST_EV_BROM_PCIE_PORT_ACTIVE);
}

void dev_pcie_reset() {

  uint32_t *reg = PCIE_MAC_REG;
  uint32_t notify_host = ((*reg) & (~HOST_EVENT_MASK) |
                          (BROM_EV_NORITY_HOST_REBOOT << BROM_EVENT_SHIFT));

  *reg = notify_host;

  uint32_t timeout = 20; // 2000ms
  while (timeout > 0) {
    uint32_t signal = ((*reg) & HOST_EVENT_MASK) >> HOST_EVENT_SHIFT;
    if (signal == HOST_EV_REBOOT_READY)
      break;
    timeout--;
    dev_timer_mdelay(100);
  }

  return;
}

#else

BOOL dev_is_pcie_active() { return FALSE; }
void dev_pcie_reset() { return; }
#endif
