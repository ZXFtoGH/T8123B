#include <boot/hal.h>
#include <boot/dump_speed.h>
#include <boot/dev_interface/cldma_interface.h>
#include <boot/dev_interface/pcie_interface.h>
#include <boot/dev_interface/uart_interface.h>
#include <boot/dev_interface/usb_interface.h>
#include <boot/protocol.h>
#include <boot/storage_access.h>

struct com_channel_struct com_channel = {
    read_packet_with_profile,
    write_packet_with_profile,
    log_packet_to_pc,
    log_to_uart,

};

status_t com_interface_init_uart();
status_t com_interface_init();

struct com_interface_struct com_interface = {
    com_interface_init_uart, com_interface_init, 0, 0, 0, 0};

struct device_usb_struct device_usb0 = {dev_usb_init, dev_usb_action,
                                        dev_usb_read, dev_usb_write};
// for log
struct device_usb_struct device_usb_log = {dev_usb_init, dev_usb_action,
                                           dev_usb_read, dev_usb_write};

struct device_cldma_struct device_cldma0 = {
    dev_cldma0_init, dev_cldma0_device_ctrl, dev_cldma0_read, dev_cldma0_write};
struct device_cldma_struct device_cldma_log = {
    dev_cldma0_init, dev_cldma0_device_ctrl, dev_cldma0_read, dev_cldma0_write};

// protocol
struct device_uart_struct device_uart0 = {dev_uart0_init, dev_uart0_read,
                                          dev_uart0_write};

struct device_uart_struct device_uart_log = {dev_uart1_init, dev_uart1_read,
                                             dev_uart1_write};

struct storage_interface_struct storage_interface = {
    read_storage_with_profile, write_storage_with_profile,
    erase_storage, storage_ctrl, storage_flush};

status_t com_interface_init_uart() {
  status_t ret = 0;
  if (dev_is_pcie_active()) {
    com_interface.read = device_cldma0.read;
    com_interface.write = device_cldma0.write;
    com_interface.log_to_pc = device_cldma0.write;
    com_interface.log_to_uart = device_uart0.write;

    device_uart0.init(921600);
  } else if (dev_is_usb_download()) {
    com_interface.read = device_usb0.read;
    com_interface.write = device_usb0.write;
    com_interface.log_to_pc = device_usb0.write; // or device_usb_log.write
    com_interface.log_to_uart = device_uart0.write;

    device_uart0.init(921600);
  } else {
    com_interface.read = device_uart0.read;
    com_interface.write = device_uart0.write;
    device_uart0.init(115200);

    com_interface.log_to_pc = 0;
    com_interface.log_to_uart = 0; // device_uart_log.write;
  }

  return ret;
}

status_t com_interface_init() {
  status_t ret = 0;
  if (dev_is_pcie_active()) {
    device_cldma0.init();
  } else if (dev_is_usb_download()) {
    device_usb0.init(false); // inherit brom usb.
  }
  return ret;
}

bool is_da_uart_download_mode() {
  return (!(dev_is_pcie_active() || dev_is_usb_download()));
}
