#include "checksum.h"
#include "boot/error_message.h"


uint32_t checksum_plain_impl(uint8_t* data, uint32_t length);
uint32_t checksum_plain(uint8_t* data, uint32_t length)
{ 
    return checksum_plain_impl(data, length);
}
