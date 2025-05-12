#ifndef _UFS_PL_API_
#define _UFS_PL_API_


/*********************************Interface***********************************/
u32 ufs_init_device();
int storage_wrap_read(uint32_t phys_part, uint64_t offset, uint8_t *buf,
     uint64_t size);
long storage_wrap_write(uint32_t phys_part, uint64_t offset, uint8_t *buf,
     uint64_t size);
int ufs_info_get(struct ufs_info_struct *info);
int ufs_switch_part(u32 part_id);
void ufs_use_polling_mode();


#define STORAGE_PHYS_PART_UNKNOWN  (0)
#define STORAGE_PHYS_PART_BOOT1    (1)
#define STORAGE_PHYS_PART_BOOT2    (2)
#define STORAGE_PHYS_PART_USER     (3)
#define STORAGE_PHYS_PART_RPMB     (4)


#endif