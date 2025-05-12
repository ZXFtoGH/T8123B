#ifndef __DRM_DEFINE_H
#define __DRM_DEFINE_H

#include "dramc_pi_api.h"

#if __ETT__
//#define DDR_RESERVE_MODE
#endif

#ifdef  DDR_RESERVE_MODE
#define DRM_FAIL_RE_INTDRAMC_TEST 0

extern void ETT_DRM(DRAMC_CTX_T *p);
#endif

#endif
