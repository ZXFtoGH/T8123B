#ifndef _MTK_CHARGER_H
#define _MTK_CHARGER_H

#include <typedefs.h>

#ifndef EBUSY
#define EBUSY  16
#endif

#define PMIC_EXCEED_I2C_FIFO_LENGTH	0xB002
#define PMIC_CHRDET_EXIST		0xB003
#define PMIC_CHRDET_NOT_EXIST		0xB004
#define PMIC_VBAT_DROP			0xB005
#define PMIC_VBAT_NOT_DROP		0xB006

typedef enum {
	CHARGER_UNKNOWN = 0,
	STANDARD_HOST,		// USB : 450mA
	CHARGING_HOST,
	NONSTANDARD_CHARGER,	// AC : 450mA~1A
	STANDARD_CHARGER,	// AC : ~1A
	APPLE_2_1A_CHARGER,	 // 2.1A apple charger
	APPLE_1_0A_CHARGER,	 // 1A apple charger
	APPLE_0_5A_CHARGER,	 // 0.5A apple charger
} CHARGER_TYPE;

#define MTK_UNLOCK_BAT_CV_LEVEL 4350000/* uV */

#ifdef MTK_UNLOCK_BAT_SUPPORT
extern int mtk_try_unlock_bat(unsigned int uV);
#endif /* MTK_UNLOCK_BAT_SUPPORT */

#ifdef MTK_EXT_CHGDET_SUPPORT
extern int mtk_ext_chgdet(CHARGER_TYPE *chg_type);
#endif /* MTK_EXT_CHGDET_SUPPORT */
extern CHARGER_TYPE mt_charger_type_detection(void);

#endif /* _MTK_CHARGER_H */
