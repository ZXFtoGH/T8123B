#ifndef _MTK_SUBPMIC_H
#define _MTK_SUBPMIC_H

#include <typedefs.h>
#include <mtk_charger.h>

#define MTK_UNLOCK_BAT_CV_LEVEL 4350000	/* uV */

extern int mtk_try_unlock_bat(unsigned int uV);
extern int mtk_ext_chgdet(CHARGER_TYPE *chg_type);
extern int mtk_subpmic_init(void);

extern int mtk_da_chg_set_aicr(unsigned int mA);
extern int mtk_da_chg_enable_charging(bool en);

#endif /* _MTK_SUBPMIC_H */
