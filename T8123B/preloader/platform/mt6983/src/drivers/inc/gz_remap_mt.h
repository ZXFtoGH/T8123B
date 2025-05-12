#ifndef __GZ_REMAP_MT_H__
#define __GZ_REMAP_MT_H__

#include <mt6983.h>

/* Note: GZ SEC remap excluded */
#define GZ_USE_DEFAULT_REMAP_TABLE  1

/* Coda regs */
#define GZ_SEC_COMMON_REG0          (uintptr_t)(SECURITY_AO + 0x690)
#define GZ_SEC_COMMON_REG1          (uintptr_t)(SECURITY_AO + 0x694)
#define GZ_SEC_COMMON_LOCK          (uintptr_t)(SECURITY_AO + 0x698)

/* HW registers */
#define GZ_REMAP_DOMAIN_EMI         GZ_SEC_COMMON_REG0
#define GZ_EMI_REMAP_EN             (0x1 << 5) /* Enable GZ EMI Remap */
/* User Defined REMAP Table */
#define GZ_EMI_REMAP_TABLE0_SBIT    (6)
#define GZ_EMI_REMAP_TABLE0_EN      (0x1 << 10)
#define GZ_EMI_REMAP_TABLE1_SBIT    (11)
#define GZ_EMI_REMAP_TABLE1_EN      (0x1 << 15)
#define GZ_EMI_REMAP_TABLE2_SBIT    (16)
#define GZ_EMI_REMAP_TABLE2_EN      (0x1 << 20)
#define GZ_EMI_REMAP_TABLE3_SBIT    (21)
#define GZ_EMI_REMAP_TABLE3_EN      (0x1 << 25)

#define GZ_REMAP_DOMAIN_INFRA_IO    GZ_SEC_COMMON_REG1
/* Enable GZ INFRA Remap */
#define GZ_INFRA_REMAP_EN           (0x1 << 5)
/* User Defined REMAP Table */
#define GZ_INFRA_REMAP_TABLE0_SBIT  (6)
#define GZ_INFRA_REMAP_TABLE0_EN    (0x1 << 10)
#define GZ_INFRA_REMAP_TABLE1_SBIT  (11)
#define GZ_INFRA_REMAP_TABLE1_EN    (0x1 << 15)
#define GZ_INFRA_REMAP_TABLE2_SBIT  (16)
#define GZ_INFRA_REMAP_TABLE2_EN    (0x1 << 20)
#define GZ_INFRA_REMAP_TABLE3_SBIT  (21)
#define GZ_INFRA_REMAP_TABLE3_EN    (0x1 << 25)

/* GZ_Remap Lock bit */
#define GZ_SEC_COMMON_LOCK_EN       (0x1U << 31)

/**
 * VM used/max number and per-VM offset
 *
 * The whole VM offset depends on overall addressing bits
 * and the secure IO remap is activated by accessing peripherals
 * through Bank 3 instead of Bank 1 addresses
 */
#define GZ_REMAP_VM_NUM             2
#define GZ_REMAP_VM_MAX             4
#define GZ_REMAP_VM_OFFSET          (0x1ULL << 35)
#define GZ_REMAP_VM_SEC_IO_OFFSET   0x20000000

/* VM domain mapping */
#define GZ_REMAP_VMDOMAIN_AP 0
#define GZ_REMAP_VMDOMAIN_GZ 11
#define GZ_REMAP_VMDOMAIN_MAP_BITS  0x5

#endif /* end of __GZ_REMAP_MT_H__ */
