#pragma once

enum BOOT_TRAP_E
{
   BOOT_TRAP_NOT_SET = 0,
   BOOT_TRAP_EMMC,
   BOOT_TRAP_UFS,
   BOOT_TRAP_EMMC_UFS,
   BOOT_TRAP_PNAND,
   BOOT_TRAP_SNAND,
   BOOT_TRAP_NOR,
};
enum BOOT_TRAP_E gpio_get_boot_trap();

