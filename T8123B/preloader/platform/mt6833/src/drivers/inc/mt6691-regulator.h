#ifndef __LINUX_REGULATOR_MT6691_H_
#define __LINUX_REGULATOR_MT6691_H_

#include <regulator/mtk_regulator_core.h>
#include "mt6359.h"

/*--abstrac the same parameter--*/
struct mt6691_regulator_info {
	unsigned char vol_reg;
	unsigned char enable_reg;
	unsigned char mode_reg;
};

#endif /* __LINUX_REGULATOR_MT6691_H_ */
