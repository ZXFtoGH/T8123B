#ifndef __MMC_DRV_INTERFACE_
#define __MMC_DRV_INTERFACE_

#define MMC_BLOCK_BITS_SHFT         (9)
#define MMC_BLOCK_SIZE              (1 << MMC_BLOCK_BITS_SHFT)
#define EMMC_PART_BOOT1             (1)
/*********************************Interface***********************************/
u32 mmc_init_device();
int mmc_switch_part(u32 part_id);
int storage_wrap_read(uint32_t phys_part, uint64_t offset, uint8_t *buf,
     uint64_t size);
long storage_wrap_write(uint32_t phys_part, uint64_t offset, uint8_t *buf,
     uint64_t size);
int mmc_da_info_get(struct mmc_info_struct *info);

void mmc_use_polling_mode();


#define STORAGE_PHYS_PART_UNKNOWN  (0)
#define STORAGE_PHYS_PART_BOOT1    (1)
#define STORAGE_PHYS_PART_BOOT2    (2)
#define STORAGE_PHYS_PART_USER     (3)
#define STORAGE_PHYS_PART_RPMB     (4)


#endif