#include "meta_boot_mode.h"
#include "error_code.h"
#include "boot/system_objects.h"
#include "string.h"
#include "boot/partition/partition_info_repertory.h"
#include "debug.h"
#include "boot/hal.h"


status_t set_meta_boot_mode(int connect_type, BOOL mobile_log, BOOL adb)
{
   struct mtk_boot_mode_flag flag = {0};
   flag.com_type = connect_type;
   flag.boot_mode = 1;
   flag.com_id = adb ? 1 : 0;
   flag.com_id |= (mobile_log ? 2 : 0);
   status_t status = STATUS_OK;
   struct partition_info_struct part_info;
   uint32_t length = sizeof(struct product_info);
   memset(&part_info, 0, sizeof(struct partition_info_struct));

   status = pir_get_partition_info((uint8_t*)"proinfo", &part_info);
   if(status != STATUS_OK) {
      LOGE("find partition info from partition table failed. status: 0x%x\n", status);
      return status;
   }

   struct product_info* proinfo_data = (struct product_info*)malloc(length);
   if (proinfo_data == 0) return STATUS_MEM_ALLOC_FAIL;

   status = storage_interface.read(part_info.base_addr, (uint8_t*)proinfo_data, length, &part_info);
   if(status != STATUS_OK) {
      LOGE("read proinfo data failed. status: 0x%x\n", status);
      free(proinfo_data);
      return status;
   }

   memcpy(&proinfo_data->mtk_boot_mode_flag, &flag, sizeof(struct mtk_boot_mode_flag));
   LOGD("mtk_boot_mode_flag.boot_mode=%d\n", proinfo_data->mtk_boot_mode_flag.boot_mode);
   LOGD("mtk_boot_mode_flag.com_type=%d\n", proinfo_data->mtk_boot_mode_flag.com_type);
   LOGD("mtk_boot_mode_flag.com_id=%d\n", proinfo_data->mtk_boot_mode_flag.com_id);

   status = storage_interface.write(part_info.base_addr, (uint8_t*)proinfo_data, length, &part_info);
   if(status != STATUS_OK) {
     LOGE("write proinfo data failed. status: 0x%x\n", status);
   }

   free(proinfo_data);
   return status;
}


status_t set_atm_mode(void)
{
#define OFFSET_ATM (64+4*10+170+64+4+8)
#define KEY1 12345678
#define KEY2 23131123
#define ENCRYPT_LENTH 8
#define ATM_FLAG 1
#define encrypt(t, k) ((t)^(k))

   status_t status = STATUS_OK;
   struct partition_info_struct part_info;
   uint8_t ciper_text[ENCRYPT_LENTH*2] = {0};

   memset(&part_info, 0, sizeof(struct partition_info_struct));
   status = pir_get_partition_info((uint8_t*)"proinfo", &part_info);
   if(status != STATUS_OK) {
      LOGE("find partition info from partition table failed. status: 0x%x\n", status);
      return status;
   }

   snprintf(ciper_text, ENCRYPT_LENTH + 1, "%d", encrypt ((KEY1 + ATM_FLAG), KEY2));
   LOGD("%s: ciper_text:%s\n", __func__, ciper_text);
   status = storage_interface.write((part_info.base_addr + OFFSET_ATM),
                        ciper_text, ENCRYPT_LENTH, &part_info);
   return status;
}


