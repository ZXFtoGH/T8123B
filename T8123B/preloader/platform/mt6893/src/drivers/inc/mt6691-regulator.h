#ifndef __LINUX_REGULATOR_MT6691_H_
#define __LINUX_REGULATOR_MT6691_H_

#include <regulator/mtk_regulator_core.h>
#include "mt6359.h"
#include "mt6691-i2c.h"

/*--abstrac the same parameter--*/
struct mt6691_regulator_info {
	unsigned char vol_reg;
	unsigned char enable_reg;
	unsigned char mode_reg;
};

extern int mt6691_probe(struct mt6691_chip *mt6691_chip, unsigned char num_chip);
extern int hl7593_probe(struct mt6691_chip *mt6691_chip, unsigned char num_chip);
#endif /* __LINUX_REGULATOR_MT6691_H_ */
