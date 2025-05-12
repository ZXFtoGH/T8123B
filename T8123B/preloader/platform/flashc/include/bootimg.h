#ifndef __MT6577_BOOTIMAGE_H__
#define __MT6577_BOOTIMAGE_H__

typedef struct boot_img_hdr boot_img_hdr;

#define BOOT_MAGIC "ANDROID"
#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_ARGS_SIZE 512
#define BOOT_EXTRA_ARGS_SIZE 1024

struct boot_img_hdr
{
   unsigned char magic[BOOT_MAGIC_SIZE];

   unsigned int kernel_size;  /* size in bytes */
   unsigned int kernel_addr;  /* physical load addr */

   unsigned int ramdisk_size; /* size in bytes */
   unsigned int ramdisk_addr; /* physical load addr */

   unsigned int second_size;  /* size in bytes */
   unsigned int second_addr;  /* physical load addr */

   unsigned int tags_addr;    /* physical addr for kernel tags */
   unsigned int page_size;    /* flash page size we assume */
   unsigned int unused[2];    /* future expansion: should be 0 */

   unsigned char name[BOOT_NAME_SIZE]; /* asciiz product name */

   unsigned char cmdline[BOOT_ARGS_SIZE];

   unsigned int id[8]; /* timestamp / checksum / sha1 / etc */

   unsigned char extra_cmdline[BOOT_EXTRA_ARGS_SIZE];
   unsigned int recovery_dtbo_size; /* size of recovery dtbo image */
   unsigned long long recovery_dtbo_offset; /* physical load addr */
   unsigned int header_size; /* size of boot image header in bytes */
   unsigned int dtb_size; /* size in bytes for DTB image */
   unsigned long long dtb_addr; /* physical load address for DTB image */
} __attribute__((packed));

/* When the boot image header has a version of 2, the structure of the boot
 * image is as follows:
 *
 * +---------------------+
 * | boot header         | 1 page
 * +---------------------+
 * | kernel              | n pages
 * +---------------------+
 * | ramdisk             | m pages
 * +---------------------+
 * | second stage        | o pages
 * +---------------------+
 * | recovery dtbo/acpio | p pages
 * +---------------------+
 * | dtb                 | q pages
 * +---------------------+
 * n = (kernel_size + page_size - 1) / page_size
 * m = (ramdisk_size + page_size - 1) / page_size
 * o = (second_size + page_size - 1) / page_size
 * p = (recovery_dtbo_size + page_size - 1) / page_size
 * q = (dtb_size + page_size - 1) / page_size
 *
 * 0. all entities are page_size aligned in flash
 * 1. kernel, ramdisk and DTB are required (size != 0)
 * 2. recovery_dtbo/recovery_acpio is required for recovery.img in non-A/B
 *    devices(recovery_dtbo_size != 0)
 * 3. second is optional (second_size == 0 -> no second)
 * 4. load each element (kernel, ramdisk, second, dtb) at
 *    the specified physical address (kernel_addr, etc)
 * 5. If booting to recovery mode in a non-A/B device, extract recovery
 *    dtbo/acpio and apply the correct set of overlays on the base device tree
 *    depending on the hardware/product revision.
 * 6. prepare tags at tag_addr.  kernel_args[] is
 *    appended to the kernel commandline in the tags.
 * 7. r0 = 0, r1 = MACHINE_TYPE, r2 = tags_addr
 * 8. if second_size != 0: jump to second_addr
 *    else: jump to kernel_addr
 */


#if 0
typedef struct ptentry ptentry;

struct ptentry {
   char name[16];      /* asciiz partition name    */
   unsigned start;     /* starting block number    */
   unsigned length;    /* length in blocks         */
   unsigned flags;     /* set to zero              */
};

/* MSM Partition Table ATAG
**
** length: 2 + 7 * n
** atag:   0x4d534d70
**         <ptentry> x n
*/
#endif

#endif  /* !__MT6577_BOOGIMG_H__ */
