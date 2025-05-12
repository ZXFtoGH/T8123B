/*
 * Copyright (c) [2021], MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * MediaTek Inc. and/or its licensors.
 * Except as otherwise provided in the applicable licensing terms with
 * MediaTek Inc. and/or its licensors, any reproduction, modification, use or
 * disclosure of MediaTek Software, and information contained herein, in whole
 * or in part, shall be strictly prohibited.
*/

#ifndef __MTSPMC_H__
#define __MTSPMC_H__

#define SPMC_DEBUG			0
#define CONFIG_SPMC_MODE	0 /* 0:cpc 1:spmc */

#if SPMC_DEBUG
#define PRINTF_SPMC(...)	print(__VA_ARGS__)
#else
#define PRINTF_SPMC(...)
#endif
#define INFO(...)			print(__VA_ARGS__)

#define MCUCFG_BASE			(0x0C530000)

#define SECURITY_AO_BASE	(0x1001A000)
#define BOOTROM_BOOT_ADDR	(SECURITY_AO_BASE + 0x0)
#define BOOTROM_SEC_CTRL	(SECURITY_AO_BASE + 0x10)
#define SW_ROM_PD			(1U << 31)
#define TS_CPU_MIGRATE		0x80000000

typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed int			int32_t;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;
typedef unsigned int		size_t;
typedef signed int			ssize_t;

int boot_secondary(unsigned int cpu);
int power_down_cpu(unsigned int cpu);

void mt_smp_prepare_cpus(void);
int spmc_init(unsigned int val);
void spm_poweron_cpu(unsigned int cluster, unsigned int cpu);
void spm_poweroff_cpu(unsigned int cluster, unsigned int cpu);

void cpc_hotplug_poweron_cpu(unsigned int cluster, unsigned int cpu);
void cpc_hotplug_poweroff_cpu(unsigned int cluster, unsigned int cpu);
void cpc_mcdi_poweron_cpu(unsigned int cluster, unsigned int cpu);
void sspm_hotplug_auto_poweron_cpu(unsigned int cluster, unsigned int cpu);
void sspm_hotplug_auto_poweroff_cpu(unsigned int cluster, unsigned int cpu);
void sspm_mcdi_auto_poweron_cpu(unsigned int cluster, unsigned int cpu);
void sspm_mcdi_auto_poweroff_cpu(unsigned int cluster, unsigned int cpu);

void sspm_noauto_poweron_cpu(unsigned int cluster, unsigned int cpu);
void sspm_noauto_poweroff_cpu(unsigned int cluster, unsigned int cpu);

void set_cpu_ignore_pactive(unsigned int cluster, unsigned int cpu);
void clear_cpu_ignore_pactive(unsigned int cluster, unsigned int cpu);
int spm_get_cpu_idle_to_pwr_off(unsigned int cluster, unsigned int cpu);
void set_CPUPWRCTLR_CORE_PWRDN_EN(void);

extern void boot_secondary_core17(void);
extern void boot_secondary_core13(void);
extern void boot_secondary_core47(void);

#endif /* __MTCMOS_H__ */