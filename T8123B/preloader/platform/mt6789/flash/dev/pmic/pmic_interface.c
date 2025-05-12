#include "assert.h"
#include <debug.h>
#include <sys/types.h>

#if INTERFACE_PMIC_IMPL

extern int pwrap_init_preloader(void);
extern uint32_t pmic_init(void);
extern int mt6358_probe(void);
extern void pmic_init_setting(void);
extern void pmic_cold_reset(void);
extern void rtc_init(void);


void dev_pmic_init()
{
#if (!DA_MODE_FPGA)
	int ret = -1;

	pwrap_init_preloader();
	ret = pmic_init();
	if (0 != ret)
		ASSERT(0);

	rtc_init();
	pmic_init_setting();
#endif
}

void dev_pmic_reinit()
{
#if (!DA_MODE_FPGA)
	mt6358_probe();
#endif
}

void dev_pmic_cold_reset(void)
{
	pmic_cold_reset();
}
#else

void dev_pmic_init() {}
void dev_pmic_reinit() {}
void dev_pmic_cold_reset(void) {}

#endif
