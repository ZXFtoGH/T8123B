#ifndef _DCC_H_
#define _DCC_H_

#include "platform.h"

#define MCUSYS_PAR_WRAP      (0x0C000000)
#define CPU_PLLDIV_0_CFG1    (MCUSYS_PAR_WRAP + 0x0110) /* LCPU */
//1	1	rg_cpu_plldiv_0_bypass
//20	16	rg_cpu_plldiv_0_calin
#define CPU_PLLDIV_1_CFG1    (MCUSYS_PAR_WRAP + 0x011C) /* MCPU */
//1	1	rg_cpu_plldiv_1_bypass
//20	16	rg_cpu_plldiv_1_calin
#define CPU_PLLDIV_2_CFG1    (MCUSYS_PAR_WRAP + 0x0128) /* BCPU */
//1	1	rg_cpu_plldiv_2_bypass
//20	16	rg_cpu_plldiv_2_calin

extern void armplldiv_dcc_init(void);

#endif
