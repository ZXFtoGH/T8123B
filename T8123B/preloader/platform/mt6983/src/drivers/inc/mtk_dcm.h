#ifndef MTK_DCM_H
#define MTK_DCM_H

#ifndef __raw_readl
#define __raw_readl(addr)		DRV_Reg32(addr)
#endif
#define reg_read(addr)	 __raw_readl(addr)
#define dcm_pr_info(fmt, args...)	pal_log_info(fmt, ##args)
#define REG_DUMP(addr) pal_log_info("%s(0x%x): 0x%x\n", #addr, addr, reg_read(addr))

/* #define DCM_BRINGUP */

static int dcm_busdvt(int on);
static int dcm_peri(int on);
static int dcm_infra(int on);
int mtk_dcm_init(void);
#endif
