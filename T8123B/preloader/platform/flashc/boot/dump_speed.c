#include "dump_speed.h"
#include "debug.h"
#include "storage_access.h"
#include "protocol.h"
#include "error_code.h"
#include "boot/dev_interface/gpt_timer_interface.h"

struct speed_dump speed_info_record = {0};

void profile_speed_info(void)
{
#if DUMP_SPEED
   uint32_t speed = 0;

   LOGI("*********************************SPEED DUMP*************************************\n");
   speed =  SPEED(speed_info_record.storage_write.data_size, speed_info_record.storage_write.time/1000);
   if(speed)
   {
      LOGI("[Storage Write]storage total size: %d KB | write time: %d ms | write speed: %d KBps\n",
         (uint32_t)(speed_info_record.storage_write.data_size/1024), speed_info_record.storage_write.time/1000, speed);
   }
   speed = SPEED(speed_info_record.storage_read.data_size, speed_info_record.storage_read.time/1000);
   if(speed)
   {
      LOGI("[Storage Read]storage total size: %d KB |read time: %d ms| read speed: %d KBps\n",
         (uint32_t)(speed_info_record.storage_read.data_size/1024), speed_info_record.storage_read.time/1000, speed);
   }
   speed = SPEED(speed_info_record.usb_write.data_size, speed_info_record.usb_write.time/1000);
   if(speed)
   {
      LOGI("[USB Write]usb total size: %d KB | write time: %d ms | write speed: %d KBps\n",
         (uint32_t)(speed_info_record.usb_write.data_size/1024), speed_info_record.usb_write.time/1000, speed);
   }
   speed = SPEED(speed_info_record.usb_read.data_size, speed_info_record.usb_read.time/1000);
   if(speed)
   {
      LOGI("[USB Read]usb total size: %d KB |read time: %d ms | read speed: %d KBps\n",
        (uint32_t)(speed_info_record.usb_read.data_size/1024), speed_info_record.usb_read.time/1000, speed);
   }
   speed = SPEED(speed_info_record.total.data_size, speed_info_record.total.time/1000);
   if(speed)
   {
      LOGI("[Total]total size: %d KB | time: %d ms | speed: %d KBps\n",
       (uint32_t) (speed_info_record.total.data_size/1024), speed_info_record.total.time/1000, speed);
   }

   speed = SPEED(speed_info_record.mcopy.data_size, speed_info_record.mcopy.time/1000);
   if(speed)
   {
      LOGI("[Memcpy]memcpy total size: %d KB | memcpy time: %d ms | memcpy speed: %d KBps\n",
       (uint32_t) (speed_info_record.mcopy.data_size/1024), speed_info_record.mcopy.time/1000, speed);
   }
   LOGI("************************************DUMP END************************************\n");
#endif
}

status_t read_storage_with_profile(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info)
{
   status_t status = STATUS_OK;

#if DUMP_SPEED
   uint32_t time_start = (uint32_t)dev_timer_get_time_us();
   uint32_t time_end =0;
#endif
   status = read_storage(at_address, data, length, part_info);
#if DUMP_SPEED
   time_end = (uint32_t)dev_timer_get_time_us();
   speed_info_record.storage_read.time+=(time_end-time_start);
   speed_info_record.storage_read.data_size+=length;
#endif

   return status;
}

status_t write_storage_with_profile(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info)
{
   status_t status = STATUS_OK;

#if DUMP_SPEED
   uint32_t time_start = (uint32_t)dev_timer_get_time_us();
   uint32_t time_end =0;
#endif
   status = write_storage(at_address, data, length, part_info);
#if DUMP_SPEED
   time_end = (uint32_t)dev_timer_get_time_us();
   speed_info_record.storage_write.time+=(time_end-time_start);
   speed_info_record.storage_write.data_size+=length;
#endif

   return status;
}

status_t write_packet_with_profile(uint8_t* buffer, uint32_t length)
{
   status_t status = STATUS_OK;

#if DUMP_SPEED
   uint32_t time_start = (uint32_t)dev_timer_get_time_us();
   uint32_t time_end =0;
#endif
   status = write_packet(buffer, length);
#if DUMP_SPEED
   time_end =(uint32_t)dev_timer_get_time_us();
   speed_info_record.usb_write.time+=(time_end-time_start);
   speed_info_record.usb_write.data_size+=length;
#endif

   return status;

}

status_t read_packet_with_profile(uint8_t* buffer, uint32_t* length)
{
   status_t status = STATUS_OK;

#if DUMP_SPEED
   uint32_t time_start = (uint32_t)dev_timer_get_time_us();
   uint32_t time_end =0;
#endif
   status = read_packet(buffer, length);
#if DUMP_SPEED
   time_end =(uint32_t)dev_timer_get_time_us();
   speed_info_record.usb_read.time+=(time_end-time_start);
   speed_info_record.usb_read.data_size+=(*length);
#endif

   return status;

}

