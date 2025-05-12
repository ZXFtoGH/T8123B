#include "boot/dev_interface/partial_protect.h"

#define PROTECT_UNIT_NUM 1

/*
* struct protect_unit
* {
*     unsigned int storage; // 0x1:eMMC 0x30: UFS
*     unsigned int region;   // 1:BOOT0; 2:BOOT1; 8:USER
*     unsigned long long addr;  //start offset of the specified region
*     unsigned long long length;  //length of the specified region
*  };
*/

struct protect_unit partial_regions[PROTECT_UNIT_NUM] = { 
 /*  {1, 1, 0x0, 0x1000},
   {1, 8, 0xe200000, 0x1000},
   {1, 8, 0x6688000, 0x4000000}*/
 	{1, 8, 0x0, 0x0}
};

struct protect_unit *get_protect_info(void)
{
   return &partial_regions[0];
}

unsigned int get_partial_number()
{
   return PROTECT_UNIT_NUM;
}

int check_register_read_permission(unsigned int address) { return 0; }

int check_register_write_permission(unsigned int address) { return 0; }

