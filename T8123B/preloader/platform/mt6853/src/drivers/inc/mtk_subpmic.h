#ifndef _MTK_SUBPMIC_H
#define _MTK_SUBPMIC_H

#include <typedefs.h>
#include <pmic.h>

#define MTK_UNLOCK_BAT_CV_LEVEL 4350000	/* uV */

extern int mtk_subpmic_init(void);
extern int mtk_try_unlock_bat(unsigned int uV);
extern int mtk_ext_chgdet(CHARGER_TYPE *chg_type);
extern int mtk_subpmic_get_hw_trapping_type(void);
extern int mtk_subpmic_read_interface(u8 addr, u8 *data, u8 mask, u8 shift);
extern int mtk_subpmic_config_interface(u8 addr, u8 data, u8 mask, u8 shift);
extern int mtk_subpmic_enable_poweroff_sequence(bool en);
extern int mtk_get_vbus_adc(u32 *vbus);

#endif /* _MTK_SUBPMIC_H */
