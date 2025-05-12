/*
 * Copyright (c) 2008 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "debug_impl.h"

#include "boot/hal.h"
#include "boot/system_objects.h"

#include <arch/thread.h>
#include <kernel/synchronic_api.h>

static unsigned long log_lock;

void set_log_level(uint32_t level) {
  sysob_runtime_params.da_log_level = level;
}

inline uint32_t get_log_level() { return sysob_runtime_params.da_log_level; }

int _dputs(const uint8_t *str) {
  unsigned int state = 0;
  enter_disable_interrupt(&log_lock, &state);

  if (sysob_runtime_params.da_log_path_flag & DA_LOG_PATH_USB) {
    com_channel.log_to_pc(str, strlen((char const *)str));
  } else if (sysob_runtime_params.da_log_path_flag & DA_LOG_PATH_UART) {
    com_channel.log_to_uart(str, strlen((char const *)str));
  }

  exit_enable_interrupt(&log_lock, state);
  return 0;
}

int send_da_alive() {
  unsigned int state = 0;
  enter_disable_interrupt(&log_lock, &state);

  com_channel.log_to_pc("HB", 3);

  exit_enable_interrupt(&log_lock, state);
  return 0;
}
