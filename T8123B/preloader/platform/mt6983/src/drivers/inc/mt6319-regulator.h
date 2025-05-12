#ifndef __MT6319_REGULATOR_H_
#define __MT6319_REGULATOR_H_

#include <pal_typedefs.h>

/*--abstrac the same parameter--*/
struct mt6319_regulator_info {
	uint8_t power_idx;
	uint8_t buck_id;
	uint8_t enable_shift;
	uint8_t mode_mask;
	unsigned int vol_reg;
	unsigned int da_vol_reg;
	unsigned int latch_vol_reg;
	unsigned int enable_reg;
	unsigned int da_enable_reg;
	unsigned int mode_reg;
};

extern int mt6319_regulator_init(void);
#endif /* __MT6319_REGULATOR_H_ */
