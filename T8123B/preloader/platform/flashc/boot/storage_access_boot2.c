#include "debug.h"
#include "boot/hal.h"
#include "common_struct.h"
#include "boot/storage_access.h"
#include "error_code.h"
#include "boot/dev_interface/storage_ctrl_interface.h"
#include "boot/dev_interface/partial_protect.h"
#include "boot/system_objects.h"

status_t erase_labor(struct partition_info_struct* part_info, const struct progress_cb* cb);
status_t write_labor(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info);

status_t write_storage(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info)
{
   status_t status = STATUS_OK;
   if(enable_partial_protect() ==1)
   {
   unsigned int index = 0;
   struct protect_unit *sec_info = get_protect_info(); 
   unsigned int num = get_partial_number();
   uint8_t* p_cur = data; 
   struct protect_unit remain ;	
   struct protect_unit current ;
   memset(&remain, 0, sizeof(struct protect_unit));  
   memset(&current, 0, sizeof(struct protect_unit));  

   remain.addr = at_address;
   remain.length = length;

      //assert(num != (*(sec_info)/sizeof(struct protect_unit)));
		
      LOGI("%s: partial_protect is enabled, start...\n", __func__);
      for(; index < num; index ++)
      {
         LOGD("%s: ****storage[%d]vs[%d], region[%d]vs[%d]\n", 
            __func__, sec_info[index].storage, part_info->storage,
            sec_info[index].region, part_info->section);
         LOGD("remain: addr[%llx]vs[%llx], len[%llx]vs[%llx]\n p_cur[%p]vs[%p]\n****\n", 
            sec_info[index].addr, remain.addr,
            sec_info[index].length, remain.length, data, p_cur);
         if((sec_info[index].storage == part_info->storage)
            &&(sec_info[index].region == part_info->section))
         {
            //not included in the front
            if((sec_info[index].addr + sec_info[index].length) <= remain.addr)
               continue;

            //protect area >= operation area
            if((sec_info[index].addr<= remain.addr) &&
               ((sec_info[index].addr + sec_info[index].length) >= (remain.addr + remain.length)))
            {
               remain.length = 0;	
               LOGD("whole region is in protection.\n");
               break;
            }
		  
            //not included in the tail
            if((sec_info[index].addr > (remain.addr+ remain.length)))
            {
               LOGD("not included in the tail:  addr: 0x%llx, len: 0x%llx\n", remain.addr, remain.length);
               status = write_labor(remain.addr, p_cur, remain.length, part_info);
               break;
            }
            //parital included and locate in the tail of the area
            if(((sec_info[index].addr + sec_info[index].length) >= (remain.addr + remain.length))
               &&(sec_info[index].addr > remain.addr))
            {
               LOGD("partial included and in the tail\n");
               current.length= sec_info[index].addr - remain.addr;
               status = write_labor(remain.addr, data, current.length, part_info);
               break;
               //need break
            }
            //parital included and locate in the front of the area
            if(((sec_info[index].addr + sec_info[index].length) < (remain.addr + remain.length))
               &&(sec_info[index].addr <= remain.addr))
            {
               LOGD("partial included and in the front\n");
               current.addr = remain.addr;
               remain.addr = sec_info[index].addr + sec_info[index].length;
               p_cur = p_cur + (remain.addr - current.addr);
               remain.length = remain.length - (remain.addr-current.addr);
               remain.region = part_info->section;
               continue;                           
            }
            //whole include in operation area
            if(((sec_info[index].addr + sec_info[index].length) < (remain.addr + remain.length))
               &&(sec_info[index].addr > remain.addr))
            {
               LOGD("whole included in operation area\n");       
               current.length = sec_info[index].addr - remain.addr;
               status = write_labor(remain.addr, p_cur, current.length , part_info);
               if(FAIL(status))
                  return status;
               current.addr = remain.addr;
               remain.addr = sec_info[index].addr + sec_info[index].length;
               p_cur = p_cur + (remain.addr - current.addr);
               remain.length = remain.length -(remain.addr- current.addr); 
            }
         }
         /*else
         {
         //if security region is not in the same region, write directly
            status = write_labor(remain.addr, p_cur, remain.length, part_info);
            if(FAIL(status))
               return status;
         }*/

      }

      if(remain.length != 0)
      {
         LOGD("still have data to handling, addr[0x%llx], len[0x%llx]\n", remain.addr, remain.length);
         status = write_labor(remain.addr, p_cur, remain.length, part_info);
         if(FAIL(status))
            return status;
      }
   }
   else
   {
      //disable partial protect
      status = write_labor(at_address, data, length, part_info);
   }	

   return status;
}

//samsung
status_t erase_storage(struct partition_info_struct* part_info, const struct progress_cb* cb)
{
   status_t status = STATUS_OK;
   
   if(enable_partial_protect() ==1)
   {
	
	unsigned int index = 0;
		struct partition_info_struct t_info ;
		struct protect_unit *sec_info = get_protect_info(); 
		unsigned int num = get_partial_number();
		struct protect_unit remain ;
	
		memset(&remain, 0, sizeof(struct protect_unit));
		memcpy((uint8_t*)&t_info, (uint8_t*)part_info, sizeof(struct partition_info_struct));
		remain.addr = t_info.base_addr;
		remain.length = t_info.max_size;
		remain.region = t_info.section;
	
     //assert(num != (*(sec_info)/sizeof(struct protect_unit)));
	  
      LOGI("%s: partial_protect is enabled, start...\n", __func__);
      for(; index < num; index ++)
      {
         LOGD("%s: storage[%d]vs[%d], region[%d]vs[%d]\n", __func__, sec_info[index].storage, t_info.storage,
            sec_info[index].region, t_info.section);
         LOGD("remain: addr[%llx]vs[%llx]\nlen[%llx]vs[%llx]\n****\n", 
            sec_info[index].addr, remain.addr,
            sec_info[index].length, remain.length);

         if((sec_info[index].storage == part_info->storage)
            &&(sec_info[index].region == part_info->section))
         {
            //not included in the front
            if((sec_info[index].addr + sec_info[index].length) <= remain.addr)
               continue;

            //protect area >= operation area
            if((sec_info[index].addr<= remain.addr) &&
               ((sec_info[index].addr + sec_info[index].length) >= (remain.addr + remain.length)))
            {
               uint32_t stop_flag = 0;
               remain.length = 0;	
               LOGD("whole region is in protection.\n");
               cb->cb(cb->user_arg, 100, &stop_flag);
               break;
            }

	     //protect area >= operation area
	     if((sec_info[index].addr<= remain.addr) &&
		  	((sec_info[index].addr + sec_info[index].length) >= (remain.addr + remain.length)))
	     {
	     		uint32_t stop_flag = 0;
			remain.length = 0;	
			LOGD("whole region is in protection.\n");
			cb->cb(cb->user_arg, 100, &stop_flag);
	     		break;
	     }
            //not included in the tail
            if((sec_info[index].addr > (remain.addr+ remain.length)))
            {
               LOGD("not included in the tail\n");
               status = erase_labor(&t_info,  cb);
               break;
            }
            //parital included and locate in the tail of the area
            if(((sec_info[index].addr + sec_info[index].length) >= (remain.addr + remain.length))
               &&(sec_info[index].addr > remain.addr))
            {
               LOGD("partial included and in the tail\n");
               t_info.base_addr = remain.addr;
               t_info.max_size = sec_info[index].addr - remain.addr;
               t_info.section = remain.region;								
               status = erase_labor(&t_info, cb);
               break;
               //need break
            }
            //parital included and locate in the front of the area
            if(((sec_info[index].addr + sec_info[index].length) < (remain.addr + remain.length))
               &&(sec_info[index].addr <= remain.addr))
            {
               LOGD("partial included and in the front\n");
               t_info.base_addr = remain.addr;
               remain.addr = sec_info[index].addr + sec_info[index].length;
               remain.length = remain.length - (remain.addr-t_info.base_addr);
               remain.region = part_info->section;
               continue;                           
            }
            //whole include in operation area
            if(((sec_info[index].addr + sec_info[index].length) < (remain.addr + remain.length))
               &&(sec_info[index].addr > remain.addr))
            {
               LOGD("whole included in operation area\n");  
               t_info.base_addr = remain.addr;
               t_info.max_size = sec_info[index].addr - remain.addr;
               t_info.section = remain.region;								
               status = erase_labor(&t_info, cb);		
               if(FAIL(status))
                  return status;

               remain.addr = sec_info[index].addr + sec_info[index].length;
               remain.length = remain.length -(remain.addr-t_info.base_addr);
            }
         }
         /*  else
         {
         t_info.base_addr = remain.addr;
         t_info.max_size = remain.length;
         t_info.section = remain.region;	
         status = erase_labor(&t_info, cb);
         if(FAIL(status))
         return status;
         }*/

      }

      if(remain.length != 0)
      {
         LOGD("handling remain:\n");
         t_info.base_addr = remain.addr;
         t_info.max_size = remain.length;
         t_info.section = remain.region;								
         status = erase_labor(&t_info, cb);		
         if(FAIL(status))
            return status;
      }
   }
   else
   {
      //disable partial protect
      status = erase_labor(part_info,  cb);
   }	

   return status;
}


//interface write_storage
status_t write_labor(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info)
{
   status_t status = 0;

   switch(part_info->storage)
   {
   case STORAGE_EMMC:
   case STORAGE_SDMMC:
      status = interface_switch_mmc_section(part_info->section); //this should move externally to reduce count of call.
      if(SUCCESSED(status))
      {
         status = interface_mmc_write(at_address,data, length);
      }
      if(part_info->section == EMMC_BOOT1)
      {
         status = storage_ctrl(STORAGE_EMMC,(uint32_t)STORAGE_CRCODE_DOWNLOAD_BL_PROCESS,(void*)0,(uint32_t)0,(void*)0,(uint32_t)0,(void*)0);
      }
      break;
   case STORAGE_UFS:
      status = interface_switch_ufs_section(part_info->section); //this should move externally to reduce count of call.
      if(SUCCESSED(status))
      {
         status = interface_ufs_write(at_address,data, length);
      }
      break;
   case STORAGE_NAND:
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_MLC:
   case STORAGE_NAND_TLC:
   case STORAGE_NAND_AMLC:
   case STORAGE_NAND_3DMLC:
   case STORAGE_NAND_SPI:
      status = interface_nand_write(at_address, data, length, part_info);
      break;
   case STORAGE_NOR_SERIAL:
   default:
      LOGE("[%s]unknow or unsupport storage[%d]!\n", __func__, part_info->storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }

   return status;
}

//interface erase_storage
status_t erase_labor(struct partition_info_struct* part_info, const struct progress_cb* cb)
{
   status_t status = 0;
   uint32_t stop_flag = 0;

   switch(part_info->storage)
   {
   case STORAGE_EMMC:
   case STORAGE_SDMMC:

      if(part_info->section != EMMC_RPMB)
      {
         status = interface_switch_mmc_section(part_info->section); //this should move externally.
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
   case STORAGE_NAND:
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_MLC:
   case STORAGE_NAND_TLC:
   case STORAGE_NAND_AMLC:
   case STORAGE_NAND_3DMLC:
   case STORAGE_NAND_SPI:
      status = interface_nand_erase(part_info->base_addr, part_info->max_size, part_info, cb);
      break;
   case STORAGE_NOR_SERIAL:
   default:
      LOGE("[%s]unknow or unsupport storage[%d]!\n", __func__, part_info->storage);
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
   case STORAGE_NAND:
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_MLC:
   case STORAGE_NAND_TLC:
   case STORAGE_NAND_AMLC:
   case STORAGE_NAND_3DMLC:
   case STORAGE_NAND_SPI:
      status = interface_nand_read(at_address, data, length, part_info);
      break;
   case STORAGE_NOR_SERIAL:
      break;
   default:
      LOGE("[%s]unknow or unsupport storage[%d]!\n", __func__, part_info->storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }
   return status;
}

//interface erase_storage
status_t diagnose_storage(struct partition_info_struct* part_info)
{
   status_t status = 0;

   switch(part_info->storage)
   {
   case STORAGE_EMMC:
   case STORAGE_SDMMC:
   case STORAGE_UFS:
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_MLC:
   case STORAGE_NAND_SPI:
   case STORAGE_NOR_SERIAL:
   default:
      LOGE("[%s]unknow or unsupport storage[%d]!\n", __func__, part_info->storage);
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
   case STORAGE_NAND:
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_TLC:
   case STORAGE_NAND_AMLC:
   case STORAGE_NAND_3DMLC:
   case STORAGE_NAND_MLC:
   case STORAGE_NAND_SPI:
      status = interface_nand_device_ctrl(ctrl_code, in, in_len, out, out_len, ret_len);
      break;
   case STORAGE_NOR_SERIAL:
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
   case STORAGE_NAND:
   case STORAGE_NAND_SLC:
   case STORAGE_NAND_MLC:
   case STORAGE_NAND_TLC:
   case STORAGE_NAND_AMLC:
   case STORAGE_NAND_3DMLC:
   case STORAGE_NAND_SPI:
      status = interface_nand_flush();
      break;
   case STORAGE_NOR_SERIAL:
   default:
      LOGE("[%s]unknow or unsupport storage[%d]!\n", __func__, part_info->storage);
      status = STATUS_UNKNOWN_STORAGE_TYPE;
      break;
   }
   return status;
}




