#ifndef _SPM_MTCMOS_INTERNAL_
#define _SPM_MTCMOS_INTERNAL_

#include "sync_write.h"

#define SPM_PROJECT_CODE	0xb16
#define SPM_REGWR_CFG_KEY	(SPM_PROJECT_CODE << 16)
#define SPM_REGWR_EN		0x1


#define spm_read(addr)                  __raw_readl(addr)
#define spm_write(addr, val)            mt_reg_sync_writel(val, addr)

#endif //#ifndef _SPM_MTCMOS_INTERNAL_
