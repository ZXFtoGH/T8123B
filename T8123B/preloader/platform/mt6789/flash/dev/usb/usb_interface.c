#include "boot/dev_interface/usb_interface.h"
#include "boot/hal.h"
#include "debug.h"
#include "error_code.h"
#include "include/assert.h"
#include "usb_type.h"

#if INTERFACE_USB_IMPL

#if (DA_MODE_FPGA)
#define INNER_TIMES (100)
#else
#define INNER_TIMES (10000)
#endif

int dev_usb_init(BOOL is_new) {
  if (is_new) {
    uint32_t pid = 0x2001; // should get from custom config.
    uint32_t vid = 0x0e8d;
    uint32_t index_loop;
    uint32_t index;
    gUsbSetupInfo.pid = (uint16_t)pid;
    gUsbSetupInfo.vid = (uint16_t)vid;

    for (index_loop = 0; index_loop < 25; index_loop++) {
      for (index = 0; index < INNER_TIMES; index++) {
        USB_HISR();
      }
    }

    USB2COM_SetupDL(true, true);

    dev_usb_action(USB_CRCODE_QMU_MODE, 0);
  } else {
    // use default
    gUsbSetupInfo.pid = (uint16_t)0;
    gUsbSetupInfo.vid = (uint16_t)0;
    USB2COM_Init();
  }
  return STATUS_OK;
}

BOOL dev_usb_can_higher_speed() {
  return FALSE;
#if 0 // could open later.
  if (USB_IS_High_Speed()) // need fix this if ultra speed supported.
    return FALSE;
  else
    return TRUE;
#endif
}

int dev_usb_switch_higher_speed(const char *speed) {
  if (USB_IS_High_Speed()) // do not need switch again
    return STATUS_OK;
  else
    return dev_usb_init(TRUE);
}

void dev_usb_disconnect(const char *action) {
  LOGI("USB desconnected.");
  USB_Set_DP_Pull_High(0);
}

int dev_usb_action(uint32_t ctrl_code, uint32_t *value) {
  status_t result = STATUS_OK;
  switch (ctrl_code) {
  case USB_CRCODE_GET_SPEED:
    if (USB_IS_High_Speed()) {
      *value = USB_HIGH_SPEED;
    } else {
      *value = USB_FULL_SPEED;
    }
    break;

  case USB_CRCODE_ENABLE_DMA:
    USB_Enable_DMA(value != 0);
    break;

  case USB_CRCODE_DISCONNECT_WITH_PC:
    USB_Set_DP_Pull_High((BOOL)value);
    break;

  case USB_CRCODE_QMU_MODE:
    USB_QMU_Init_and_Start();
    break;

  default:
    result = STATUS_UNSUPPORT_OP;
    LOGE("USB unsupport option");
    ASSERT(0);
    break;
  }
  return result;
}

int dev_usb_write(const uint8_t *buffer, uint32_t length) {
  status_t ret = STATUS_OK;
  arch_clean_cache_range((addr_t)buffer, length);
  USB2COM_PutByte_Buffer((uint32_t *)buffer, length);
  return ret;
}
int dev_usb_read(uint8_t *buffer, uint32_t length) {
  status_t ret = STATUS_OK;
  USB2COM_GetByte_Buffer((uint32_t *)buffer, length);
  arch_invalidate_cache_range((addr_t)buffer, length);
  return ret;
}
BOOL dev_is_usb_download() { return COM_Is_USB_Download_Mode(); }

#else

int dev_usb_init(BOOL is_new) { return STATUS_ERR; }

BOOL dev_usb_can_higher_speed() { return FALSE; }

int dev_usb_switch_higher_speed(const char *speed) { return STATUS_ERR; }

void dev_usb_disconnect(const char *action) {}

int dev_usb_action(uint32_t ctrl_code, uint32_t *value) { return STATUS_ERR; }

int dev_usb_write(const uint8_t *buffer, uint32_t length) { return STATUS_ERR; }
int dev_usb_read(uint8_t *buffer, uint32_t length) { return STATUS_ERR; }
BOOL dev_is_usb_download() { return FALSE; }

#endif