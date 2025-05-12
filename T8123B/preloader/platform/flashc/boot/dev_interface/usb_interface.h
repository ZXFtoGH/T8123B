#pragma once

#include "stdint.h"

typedef enum {
   USB_CRCODE_GET_SPEED = 1,
   USB_CRCODE_ENABLE_DMA = 2,
   USB_CRCODE_DISCONNECT_WITH_PC =3,
   USB_CRCODE_QMU_MODE = 4
}USB_CRTL_CODE_E;

typedef enum {
   USB_FULL_SPEED = 1,
   USB_HIGH_SPEED = 2,
   USB_ULTRA_HIGH_SPEED = 3,
   USB_STATUS_UNKNOWN = 0xFFFF
}USB_SPEED_STATUS_E;


int dev_usb_init(BOOL is_new);
int dev_usb_action(uint32_t ctrl_code, uint32_t *value);
int dev_usb_write(const uint8_t *buffer, uint32_t length);
int dev_usb_read(uint8_t *buffer, uint32_t length);
BOOL dev_is_usb_download();
int dev_usb_switch_higher_speed(const char *speed);
BOOL dev_usb_can_higher_speed();
void dev_usb_disconnect(const char* action);




