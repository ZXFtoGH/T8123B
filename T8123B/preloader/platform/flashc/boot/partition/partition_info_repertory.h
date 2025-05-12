
#pragma once

#include "boot/system_objects.h"
#include "common_struct.h"
#include "partition_struct.h"
#include "boot/partition/hw_depends.h"

struct partition
{
   uint8_t name[PARTITION_NAME_LEN];
};

typedef struct sys_partition_tbl
{
   BOOL valid;
   int count;
   struct partition_info_struct* info;
}sys_partition_tbl_t;

typedef enum partition_op_type {
  OP_BOOTLOADER = 0,//obselete
  OP_INVISIBLE,
  OP_UPDATE,
  OP_PROTECTED,
  OP_RESERVED,
  OP_BINREGION, //obselete
  OP_LOGIC,
  OP_NEEDRESIZE,
} partition_op_type_e;

struct partition_aux_info_struct {
  uint8_t file_name[MAX_FILE_NAME_LEN];
  BOOL is_download;
  BOOL in_gpt;
  enum nand_cell_usage ncu;
  enum partition_op_type op_type;  
};

struct sys_aux_partition_tbl {
  int count;
  struct partition_aux_info_struct *info;
};

typedef struct sys_scatter_struct {
  struct sys_partition_tbl tbl;
  struct sys_aux_partition_tbl aux_tbl;
} sys_scatter_struct_t;


extern struct sys_partition_tbl sysob_partition_tbl;
extern struct sys_partition_tbl sysob_sys_partition_tbl;
extern struct sys_scatter_struct sysob_scatter_file;

enum partition_changed_status_e {PCS_UNCHANGED = 0, PCS_CHANGED_ADDR = 1, PCS_CHANGED_SIZE = 2 };

struct protected_section_t {
  char partition_name[64];
  enum partition_changed_status_e changed;
  uint32_t checksum;
};
enum flash_update_changed_e {FUC_UNCHANGED = 0, FUC_CHANGED = 1, FUC_EMPTY_DEV = 2 };

enum flash_update_stage_e { FU_S0 = 0, FU_S1, FU_S2, FU_S3 };
struct protected_record_t {
  int cnt;
  struct protected_section_t list[16];
  enum flash_update_stage_e stage;
  enum flash_update_changed_e changed_status;
  enum flash_update_changed_e gpt_changed_status;
};


//interface
void invalidate_sys_partition_tbl();

status_t reset_sys_partition_tbl();

status_t pir_get_partition_info(const char* partition, struct partition_info_struct* info);
    /*
* function:tell user which partitions are in the operation range.
* parameters:
*    para:input, provide operation information such as range, section and storage etc.
*    partitions:output, partition list in the given range
* return:
*    STATUS_OK if success.
*    STATUS_READ_PT_FAIL shows that read partition table failed, could not finish the convert.
*    Other status need to refer to error_code.h
*/
status_t range_to_partition(const struct partition_info_struct *para, struct partition **partitions, uint32_t *count);
void free_partition_buffer(struct partition * partitions);
status_t get_extention_info_from_addr(struct partition_info_struct *para,
                                      uint64_t addr);


