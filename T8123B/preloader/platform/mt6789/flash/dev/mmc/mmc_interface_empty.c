#include "boot/dev_interface/mmc_interface.h"
#include "boot/dev_interface/partial_protect.h"
#include "lib/string.h"
#include "debug.h"
#include "error_code.h"
#include "lib/string.h"
#include "boot/system_objects.h"
#include "mmc_drv_interface.h"

status_t interface_mmc_init()
{
	return 0;
}

status_t interface_switch_mmc_section(uint32_t section)
{
	return 0;
}

status_t interface_mmc_write(uint64_t address,  uint8_t* buffer, uint64_t length)
{
	return 0;
}

status_t interface_mmc_read(uint64_t address,  uint8_t* buffer, uint64_t length)
{
	return 0;
}

status_t interface_mmc_erase(uint64_t address, uint64_t length)
{
	return 0;
}

status_t interface_get_mmc_info(struct mmc_info_struct* info)
{
	return 0;
}

status_t interface_set_mmc_ext_csd(uint32_t index, uint32_t value)
{
	return 0;
}

status_t interface_mmc_device_ctrl(uint32_t ctrl_code, void* in, uint32_t in_len, void* out, uint32_t out_len, uint32_t* ret_len)
{
	return 0;
}

status_t interface_get_mmc_ext_csd(uint32_t index, uint32_t* value)
{
	return 0;
}

status_t  interface_set_emmc_boot_ext_csd(uint32_t index)
{
	return 0;
}

status_t interface_check_RPMB_key_status()
{
	return 0;
}

status_t interface_send_pon()
{
	return 0;
}

status_t interface_emmc_set_wp(uint64_t start_address, uint32_t length,uint32_t type)
{
	return 0;
}

uint32_t interface_mmc_get_defined_irq_id()
{
	return 0;
}

status_t interface_mmc_irq_handler(uint32_t irq_id)
{
	return 0;
}
