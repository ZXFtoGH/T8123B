
#ifndef __HAL__
#define __HAL__

#include "stdint.h"
#include <malloc.h>
#include <stdbool.h>
#include <boot/dev_interface/hw_interface.h>

typedef int status_t;

struct com_channel_struct {
  status_t (*read)(uint8_t *buffer, uint32_t *length);
  status_t (*write)(uint8_t *buffer, uint32_t length);
  status_t (*log_to_pc)(const uint8_t *buffer, uint32_t length);
  status_t (*log_to_uart)(const uint8_t *buffer, uint32_t length);
};

struct com_interface_struct
{
   status_t (*init_uart)();
   status_t (*init)();
   status_t (*read)(uint8_t* buffer, uint32_t length);
   status_t (*write)(uint8_t* buffer, uint32_t length);
   status_t (*log_to_pc)(uint8_t* buffer, uint32_t length);
   status_t (*log_to_uart)(uint8_t* buffer, uint32_t length);
};


struct storage_interface_struct {
  status_t (*read)(uint64_t at_address, uint8_t *data, uint64_t length,
                   struct partition_info_struct *part_info);
  status_t (*write)(uint64_t at_address, uint8_t *data, uint64_t length,
                    struct partition_info_struct *part_info);
  status_t (*erase)(struct partition_info_struct *part_info,
                    const struct progress_cb *cb);
  status_t (*device_ctrl)(enum storage_type storage, uint32_t ctrl_code, void* in, uint32_t in_len, void* out, uint32_t out_len, uint32_t* ret_len);
  status_t (*sync)(struct partition_info_struct *part_info);
};


struct device_usb_struct
{ 
   status_t (*init)(BOOL is_new);
   status_t (*device_ctrl)(uint32_t ctrl_code, uint32_t* value);
   status_t (*read)(uint8_t* buffer, uint32_t length);
   status_t (*write)(uint8_t* buffer, uint32_t length); 
};

struct device_cldma_struct
{ 
   status_t (*init)();
   status_t (*device_ctrl)(uint32_t ctrl_code, uint32_t* value);
   status_t (*read)(uint8_t* buffer, uint32_t length);
   status_t (*write)(uint8_t* buffer, uint32_t length); 
};

struct device_uart_struct
{ 
   void (*init)(int baudrate);
   status_t (*read)(uint8_t* buffer, uint32_t length);
   status_t (*write)(uint8_t* buffer, uint32_t length);
};


extern struct com_channel_struct com_channel;
extern struct com_interface_struct com_interface;
extern struct storage_interface_struct storage_interface;
extern struct device_usb_struct device_usb0;
extern struct device_usb_struct device_usb_log;
// protocol
extern struct device_uart_struct device_uart0;

extern struct device_cldma_struct device_cldma0;
extern struct device_cldma_struct device_cldma_log;
#endif
