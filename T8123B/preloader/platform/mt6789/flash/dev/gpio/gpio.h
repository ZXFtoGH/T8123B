#pragma once

#if (DA_MODE_FPGA)
#define BOOT_TRAP_MASK  (1U<<11)
#define TRAP_TEST_RD (0x100056f0)
#define BOOT_FROM_UFS (0)
#define BOOT_FROM_EMMC  (1U<<11)
#define BOOT_FROM_EMMC_UFS  (0xFFFF)

#else
#define BOOT_TRAP_MASK  (1U<<10)
#define TRAP_TEST_RD (0x100056f0)
#define BOOT_FROM_UFS (0)
#define BOOT_FROM_EMMC  (1U<<10)
#define BOOT_FROM_EMMC_UFS  (0xFFFF)
#endif

