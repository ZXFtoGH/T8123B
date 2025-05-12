#pragma once

#include <boot/dev_interface/hw_interface.h>
/******
!!!!!DO NOT CHANGE THIS STRUCTURE, IT'S PROTOCOL WITH LK!!!!!!!!!
******/
#define OP_NAME_LEN 30
#define FULL_PRJ_NAME_LEN 60
#define RSC_FIELD_SIZE 256
typedef union ex_rsc_field {
  struct {
    unsigned int magic_number; /* 0x5253434d */
    unsigned int version;      /* for now is 1 */
    unsigned int dtbo_index;
    unsigned char full_project_name[FULL_PRJ_NAME_LEN];
    unsigned char op_name[OP_NAME_LEN];
  } rsc_info;
  unsigned char data[RSC_FIELD_SIZE];
} ex_rsc_field_t;
/******
!!!!!DO NOT CHANGE THIS STRUCTURE, IT'S PROTOCOL WITH LK!!!!!!!!!
******/

typedef struct ex_da_rsc_field {
  uint64_t offset;
  unsigned char partition_name[PARTITION_NAME_LEN];
  ex_rsc_field_t select_rsc;
} ex_da_rsc_field_t;

int set_rsc_info();