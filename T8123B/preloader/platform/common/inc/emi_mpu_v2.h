#ifndef __EMI_MPU_H__
#define __EMI_MPU_H__

#define LOCK                1
#define UNLOCK              0

#define EMI_MPU_DGROUP_NUM	(EMI_MPU_DOMAIN_NUM / 8)
#if (EMI_MPU_DGROUP_NUM == 1)
#define SET_ACCESS_PERMISSION(apc_ary, lock, d7, d6, d5, d4, d3, d2, d1, d0) \
do { \
	apc_ary[0] = \
		(((unsigned int)  d7) << 21) | (((unsigned int)  d6) << 18) | (((unsigned int)  d5) << 15) | \
		(((unsigned int)  d4) << 12) | (((unsigned int)  d3) <<  9) | (((unsigned int)  d2) <<  6) | \
		(((unsigned int)  d1) <<  3) |  ((unsigned int)  d0) | ((unsigned int) lock << 31); \
} while (0)
#elif (EMI_MPU_DGROUP_NUM == 2)
#define SET_ACCESS_PERMISSION(apc_ary, lock, d15, d14, d13, d12, d11, d10, d9, d8, d7, d6, d5, d4, d3, d2, d1, d0) \
do { \
	apc_ary[1] = \
		(((unsigned int) d15) << 21) | (((unsigned int) d14) << 18) | (((unsigned int) d13) << 15) | \
		(((unsigned int) d12) << 12) | (((unsigned int) d11) <<  9) | (((unsigned int) d10) <<  6) | \
		(((unsigned int)  d9) <<  3) |  ((unsigned int)  d8); \
	apc_ary[0] = \
		(((unsigned int)  d7) << 21) | (((unsigned int)  d6) << 18) | (((unsigned int)  d5) << 15) | \
		(((unsigned int)  d4) << 12) | (((unsigned int)  d3) <<  9) | (((unsigned int)  d2) <<  6) | \
		(((unsigned int)  d1) <<  3) |  ((unsigned int)  d0) | ((unsigned int) lock << 31); \
} while (0)
#endif

struct emi_region_info_t {
	unsigned long long start;
	unsigned long long end;
	unsigned int region;
};

extern int emi_set_protection(struct emi_region_info_t *region_info);

#endif /* __EMI_MPU_H__ */
