#include <typedefs.h>
#include <mtk_charger.h>
#include <mtk_subpmic.h>

#define __WEAK __attribute__((weak))

__WEAK int mtk_try_unlock_bat(unsigned int uV)
{
	return 0;
}

__WEAK int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	return 0;
}

__WEAK int mtk_da_chg_set_aicr(unsigned int mA)
{
	return 0;
}

__WEAK int mtk_da_chg_enable_charging(bool en)
{
	return 0;
}

__WEAK int mtk_subpmic_init(void)
{
	return 0;
}
