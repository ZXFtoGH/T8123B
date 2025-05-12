#pragma once

#include "stdint.h"
#include "boot/hal.h"
/*synchonized*/ status_t read_packet(uint8_t* buf, uint32_t* length);

/*synchonized*/ status_t write_packet(uint8_t* buf, uint32_t length);

/*synchonized*/ status_t log_packet_to_pc(const uint8_t* buf, uint32_t length);

status_t log_to_uart(const uint8_t* buf, uint32_t length);



