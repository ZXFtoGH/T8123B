#include "debug.h"
#include "boot/hal.h"
#include "common_struct.h"
#include "boot/storage_access.h"
#include "boot/protocol_callbacks.h"
#include "boot/dev_interface/mmc_interface.h"
#include "boot/dev_interface/ufs_interface.h"

#include "error_code.h"


//interface write_storage
status_t write_storage(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info)
{
   status_t status = 0;
   
   switch(part_info->storage)
   {
   case STORAGE_EMMC:		
   case STORAGE_SDMMC:  
      status = interface_switch_mmc_section(part_info->section); //this should move externally to reduce count of call.
      if(SUCCESSED(status))
      {
         status = interface_mmc_write(at_address, data, length);
      }
      break;
   case STORAGE_UFS:
      status = interface_switch_ufs_section(part_info->section); //this should move externally.
      if(SUCCESSED(status))
      {
         status = interface_ufs_write(at_address, data, length);
      }
      break;
   default:
      LOGE("unknow or unsupport storage[%d]!\n", part_info->storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }

   return status;
}

//interface erase_storage
status_t erase_storage(struct partition_info_struct* part_info, const struct progress_cb* cb)
{
   status_t status = 0;
   uint32_t stop_flag = 0;

   switch(part_info->storage)
   {
   case STORAGE_EMMC:		
   case STORAGE_SDMMC: 
      if(part_info->section != EMMC_RPMB)
      {
         status=interface_switch_mmc_section(part_info->section);
         if(SUCCESSED(status))
         {
            status = interface_mmc_erase(part_info->base_addr, part_info->max_size);
         }
         if(SUCCESSED(status) && cb)
         {
            cb->cb(cb->user_arg, 100, &stop_flag);
         }
      }
      break;
   case STORAGE_UFS:
      status = interface_switch_ufs_section(part_info->section); //this should move externally.
      if(SUCCESSED(status))
      {
         status = interface_ufs_erase(part_info->base_addr, part_info->max_size, cb);
      }

      break;
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_MLC:
   case STORAGE_NAND_SPI:
      LOGE("not support NAND.");
      break;
   case STORAGE_NOR_SERIAL:
      LOGE("not support NOR.");
      break;
   default:
      LOGE("unknow or unsupport storage[%d]!\n", part_info->storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }

   return status;
}


//interface read_storage
status_t read_storage(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info)
{
   status_t status = 0;
   switch(part_info->storage)
   {
   case STORAGE_EMMC:		
   case STORAGE_SDMMC:
      status = interface_switch_mmc_section(part_info->section); //this should move externally.
      if(SUCCESSED(status))
      {
         status = interface_mmc_read(at_address, data, length);
      }
      break;
   case STORAGE_UFS:
      status = interface_switch_ufs_section(part_info->section); //this should move externally.
      if(SUCCESSED(status))
      {
         status = interface_ufs_read(at_address, data, length);
      }
      break;

   default:
      LOGE("unknow or unsupport storage[%d]!\n", part_info->storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }
   return status;
}

status_t storage_ctrl(enum storage_type storage, uint32_t ctrl_code, void* in, uint32_t in_len, void* out, uint32_t out_len, uint32_t* ret_len)
{
   status_t status = 0;
   switch(storage)
   {
   case STORAGE_EMMC:
   case STORAGE_SDMMC:
      status = interface_mmc_device_ctrl(ctrl_code, in, in_len, out, out_len, ret_len);
      break;
   case STORAGE_UFS:
      status = interface_ufs_device_ctrl(ctrl_code, in, in_len, out, out_len, ret_len);
      break;
   default:
      LOGE("[%s]unknow or unsupport storage[%d]!\n", __func__, storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }
   return status;
}

status_t storage_flush(struct partition_info_struct* part_info)
{
   status_t status = 0;
   switch(part_info->storage)
   {
   case STORAGE_EMMC:
   case STORAGE_SDMMC:
   case STORAGE_UFS:
      break;
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_MLC:
      status = interface_nand_flush();
      break;
   case STORAGE_NAND_SPI:
   case STORAGE_NOR_SERIAL:
   default:
      LOGE("[%s]unknow or unsupport storage[%d]!\n", __func__, part_info->storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }
   return status;
}


