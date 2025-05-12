
#pragma once

#include "common_struct.h"
#include "boot/arguments.h"
#include "boot/dev_interface/mmc_interface.h"
#include "boot/dev_interface/nand_interface.h"
#include "boot/dev_interface/ufs_interface.h"
#include "boot/dev_interface/nor_interface.h"
#include "boot/dev_interface/emi_interface.h"

extern struct runtime_parameters sysob_runtime_params;

extern struct mmc_info_struct sysob_mmc;
extern struct ufs_info_struct sysob_ufs;
extern struct nand_info_struct sysob_nand;
extern struct nor_info_struct sysob_nor;

extern struct bootstrap1_arg sysob_bootstrap1_arg;
extern struct bootstrap2_arg sysob_bootstrap2_arg;

extern struct host_info_struct *sysob_host_info;

extern struct ram_info_struct sysob_ram;
void init_system_objects();



