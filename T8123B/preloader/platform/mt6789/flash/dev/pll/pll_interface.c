#include "error_code.h"
#include <debug.h>
#include <sys/types.h>

#if INTERFACE_PLL_IMPL

extern void mt_pll_init(void);
void dev_pll_init() { mt_pll_init(); }

#else

void dev_pll_init() {}

#endif