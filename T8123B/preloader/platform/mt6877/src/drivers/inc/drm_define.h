#ifndef __DRM_DEFINE_H
#define __DRM_DEFINE_H

#include "dramc_pi_api.h"

#if __ETT__
//#define DDR_RESERVE_MODE
#endif

#ifdef  DDR_RESERVE_MODE
#define DRM_FAIL_RE_INTDRAMC_TEST 0
#if DRM_FAIL_RE_INTDRAMC_TEST
#define DRM_RESET_CONF_HW 0 // 1: HW Reset CONF 0: SW reset // >=IPM2.2 support HW Reset
#endif

extern void ETT_DRM(DRAMC_CTX_T *p);
#endif

#endif
