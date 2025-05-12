#include <mtk_subpmic.h>

#define __WEAK __attribute__((weak))

__WEAK int mtk_subpmic_init(void)
{
	return 0;
}

__WEAK int mtk_try_unlock_bat(unsigned int uV)
{
	return 0;
}

__WEAK int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	return 0;
}

__WEAK int mtk_subpmic_get_hw_trapping_type(void)
{
	return 0;
}

__WEAK int mtk_subpmic_read_interface(u8 addr, u8 *data, u8 mask, u8 shift)
{
	return 0;
}

__WEAK int mtk_subpmic_config_interface(u8 addr, u8 data, u8 mask, u8 shift)
{
	return 0;
}

__WEAK int mtk_subpmic_enable_poweroff_sequence(bool en)
{
	return 0;
}

__WEAK int mtk_get_vbus_adc(u32 *vbus)
{
	return 0;
}
