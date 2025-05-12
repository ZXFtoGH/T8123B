#include "assert.h"
#include <debug.h>
#include <sys/types.h>

#if INTERFACE_PMIC_IMPL

extern int pmifclkmgr_init(void);
extern int pmif_spmi_init(int mstid);
extern uint32_t pmic_init(void);
extern int pmic_init_sdev(void);
extern int second_pmic_init_sdev(void);
extern int mt6363_regulator_probe(void);
extern int mt6373_regulator_probe(void);
extern void pmic_init_setting(void);
extern void second_pmic_init_setting(void);
extern void rtc_init(void);


void dev_pmic_init()
{
#if (!DA_MODE_FPGA)
	int ret = -1;

	pmifclkmgr_init();
	pmif_spmi_init(1);
	ret = pmic_init();
	if (0 != ret)
		ASSERT(0);
	rtc_init();
	pmic_init_setting();
	second_pmic_init_setting();
#endif
}

void dev_pmic_reinit()
{
#if (!DA_MODE_FPGA)
	pmifclkmgr_init();
	pmif_spmi_init(1);
	pmic_init_sdev();
	second_pmic_init_sdev();
	mt6363_regulator_probe();
	mt6373_regulator_probe();
#endif
}

void dev_pmic_cold_reset(void)
{
  // Move this to Preloader side.
  /*
      pmic_config_interface(PMIC_RG_CRST_ADDR, 1,
                            PMIC_RG_CRST_MASK,
                            PMIC_RG_CRST_SHIFT);
  */
}

#else

void dev_pmic_init() {}
void dev_pmic_reinit() {}
void dev_pmic_cold_reset(void) {}

#endif
