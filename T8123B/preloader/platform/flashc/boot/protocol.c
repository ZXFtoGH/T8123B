
#include "boot/hal.h"
#include "boot/protocol.h"
#include "boot/protocol_format.h"
#include "error_code.h"
#include <arch/thread.h>
#include <boot/system_objects.h>
#include <kernel/synchronic_api.h>
//#include "debug.h"

#define ENTER_CRITICAL(lp, sp)                                                 \
  {                                                                            \
    if (sysob_runtime_params.da_log_path_flag & DA_LOG_PATH_USB)               \
      enter_disable_interrupt(lp, sp);                                         \
  }
#define EXIT_CRITICAL(lp, s)                                                   \
  {                                                                            \
    if (sysob_runtime_params.da_log_path_flag & DA_LOG_PATH_USB)               \
      exit_enable_interrupt(lp, s);                                            \
  }
/*PROTOCOL*/
#define ALIGN_SZ (64)

static unsigned long protocol_lock;

/*synchonized*/ status_t read_packet(uint8_t *buf, uint32_t *length) {
  uint8_t __attribute__((aligned(ALIGN_SZ))) hdr_buf[ALIGN_SZ];
  protocol_header_wrapper_t *p_hdr = (protocol_header_wrapper_t *)hdr_buf;

  if (ALIGN_SZ < sizeof(protocol_header_wrapper_t))
    return STATUS_PROTOCOL_BUFFER_OVERFLOW;

  unsigned int state = 0;
  ENTER_CRITICAL(&protocol_lock, &state);

  com_interface.read((uint8_t *)hdr_buf, sizeof(protocol_header_wrapper_t));
  if (p_hdr->phdr.guard_magic != MAGIC_PROTOCOL) {
    EXIT_CRITICAL(&protocol_lock, state);
    return STATUS_PROTOCOL_ERR;
  }

  if (*length < p_hdr->length) {
    // Overflow.
    EXIT_CRITICAL(&protocol_lock, state);
    return STATUS_PROTOCOL_BUFFER_OVERFLOW;
  }
  *length = p_hdr->length;

  com_interface.read(buf, p_hdr->length);

  EXIT_CRITICAL(&protocol_lock, state);

  return STATUS_OK;
}

/*synchonized*/ status_t write_packet(uint8_t *buf, uint32_t length) {
  protocol_header_wrapper_t __attribute__((aligned(ALIGN_SZ))) hdr;
  hdr.phdr.guard_magic = MAGIC_PROTOCOL;
  hdr.phdr.type = DT_PROTOCOL_FLOW;
  hdr.length = length;

  unsigned int state = 0;
  ENTER_CRITICAL(&protocol_lock, &state);

  com_interface.write((uint8_t *)&hdr, sizeof(protocol_header_wrapper_t));
  com_interface.write(buf, length);

  EXIT_CRITICAL(&protocol_lock, state);

  return STATUS_OK;
}

/*synchonized*/ status_t log_packet_to_pc(const uint8_t *buf, uint32_t length) {
  message_header_wrapper_t hdr;
  hdr.phdr.guard_magic = MAGIC_PROTOCOL;
  hdr.phdr.type = DT_MESSAGE;
  hdr.length = length + sizeof(hdr.priority);
  hdr.priority = 1;
  if (com_interface.log_to_pc != NULL) {
    unsigned int state = 0;
    ENTER_CRITICAL(&protocol_lock, &state);

    com_interface.log_to_pc((uint8_t *)&hdr, sizeof(message_header_wrapper_t));
    com_interface.log_to_pc((uint8_t *)buf, length);
    EXIT_CRITICAL(&protocol_lock, state);
  }

  return STATUS_OK;
}

status_t log_to_uart(const uint8_t *buf, uint32_t length) {
  if (com_interface.log_to_uart != NULL) {
    com_interface.log_to_uart((uint8_t *)buf, length);
  }
  return STATUS_OK;
}
