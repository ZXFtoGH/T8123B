#ifndef __EMI_MPU_MT_H__
#define __EMI_MPU_MT_H__

#include "emi.h"

#define ENABLE_MPU	0

#define EMI_MPU_ALIGN_BITS	12
#define EMI_MPU_DOMAIN_NUM	16
#define EMI_MPU_REGION_NUM	32
#define DRAM_OFFSET		(0x40000000 >> EMI_MPU_ALIGN_BITS)
#define MIU_PERM_NUM		6
#define REGION_MAX		48

#define SSPM_MPU_REGION_ID	4
#define MPU_REGION_NUM		8

#define FORBIDDEN       0
#define WRITE           1
#define READ            2
#define RW              3

#endif /* __EMI_MPU_MT_H__ */
