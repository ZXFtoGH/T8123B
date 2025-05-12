#ifndef __DUMP_SPEED__
#define __DUMP_SPEED__

#include <type_define.h>
#include "system_objects.h"

#define SPEED(a,b) ((b==0)?0:(((a)*1000)/(b)/(1024)))

struct speed_unit
{
   uint32_t time;
   uint64_t data_size;
};

struct speed_dump
{
    struct speed_unit average;
    struct speed_unit  usb_read;
    struct speed_unit  usb_write;
    struct speed_unit  storage_read;
    struct speed_unit  storage_write;
    struct speed_unit  mcopy;
    struct speed_unit  total;
};

extern struct speed_dump speed_info_record;

void profile_speed_info(void);
status_t read_storage_with_profile(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info);
status_t write_storage_with_profile(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info);
status_t read_packet_with_profile(uint8_t* buffer, uint32_t* length);
status_t write_packet_with_profile(uint8_t* buffer, uint32_t length);

#endif
