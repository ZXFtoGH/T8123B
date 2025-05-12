#include <emi_mpu_v2.h>
#include <emi_hw.h>
#include <emi_mpu_mt.h>
#include "platform.h"

int emi_set_protection(struct emi_region_info_t *region_info)
{
#if ENABLE_MPU
	unsigned int start, end, region, region_set, region_bit, val;

	start = (unsigned int)(region_info->start >> EMI_MPU_ALIGN_BITS);
	end = (unsigned int)(region_info->end >> EMI_MPU_ALIGN_BITS);
	region = (unsigned int)(region_info->region);

	region_set = region/MPU_REGION_NUMBER_PER_SET;
	region_bit = region%MPU_REGION_NUMBER_PER_SET;

	if ((start >= DRAM_OFFSET) && (end >= start)) {
		start -= DRAM_OFFSET;
		end -= DRAM_OFFSET;
	} else
		return -1;

	if (region_bit == 0) {
		*((volatile unsigned int *)SECURE_REGION_SA0((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = start & ADDR_MASK;
		*((volatile unsigned int *)SECURE_REGION_SA1((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = start >> ADDR_BIT;
		*((volatile unsigned int *)SECURE_REGION_EA0((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = end & ADDR_MASK;
		*((volatile unsigned int *)SECURE_REGION_EA1((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = (end >> ADDR_BIT) | REGION_ENABLE;
		*((volatile unsigned int *)SECURE_REGION_ENABLE((region_set-1))) |= (1 << (MPU_REGION_NUMBER_PER_SET-1));

		*((volatile unsigned int *)SUB_SECURE_REGION_SA0((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = start & ADDR_MASK;
		*((volatile unsigned int *)SUB_SECURE_REGION_SA1((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = start >> ADDR_BIT;
		*((volatile unsigned int *)SUB_SECURE_REGION_EA0((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = end & ADDR_MASK;
		*((volatile unsigned int *)SUB_SECURE_REGION_EA1((region_set-1), (MPU_REGION_NUMBER_PER_SET-1))) = (end >> ADDR_BIT) | REGION_ENABLE;
		*((volatile unsigned int *)SUB_SECURE_REGION_ENABLE((region_set-1))) |= (1 << (MPU_REGION_NUMBER_PER_SET-1));
	} else {
		*((volatile unsigned int *)SECURE_REGION_SA0(region_set, (region_bit-1))) = start & ADDR_MASK;
		*((volatile unsigned int *)SECURE_REGION_SA1(region_set, (region_bit-1))) = start >> ADDR_BIT;
		*((volatile unsigned int *)SECURE_REGION_EA0(region_set, (region_bit-1))) = end & ADDR_MASK;
		*((volatile unsigned int *)SECURE_REGION_EA1(region_set, (region_bit-1))) = (end >> ADDR_BIT) | REGION_ENABLE;
		*((volatile unsigned int *)SECURE_REGION_ENABLE(region_set)) |= (1 << (region_bit-1));

		*((volatile unsigned int *)SUB_SECURE_REGION_SA0(region_set, (region_bit-1))) = start & ADDR_MASK;
		*((volatile unsigned int *)SUB_SECURE_REGION_SA1(region_set, (region_bit-1))) = start >> ADDR_BIT;
		*((volatile unsigned int *)SUB_SECURE_REGION_EA0(region_set, (region_bit-1))) = end & ADDR_MASK;
		*((volatile unsigned int *)SUB_SECURE_REGION_EA1(region_set, (region_bit-1))) = (end >> ADDR_BIT) | REGION_ENABLE;
		*((volatile unsigned int *)SUB_SECURE_REGION_ENABLE(region_set)) |= (1 << (region_bit-1));
	}
#endif
	return 0;
}
