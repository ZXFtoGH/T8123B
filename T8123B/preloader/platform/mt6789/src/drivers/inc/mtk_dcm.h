#ifndef MTK_DCM_H
#define MTK_DCM_H

#define __raw_readl(addr)		DRV_Reg32(addr)
#define reg_read(addr)	 __raw_readl(addr)
#define dcm_pr_info(fmt, args...)	print(fmt, ##args)
#define REG_DUMP(addr) print("%s(0x%lx): 0x%x\n", #addr, addr, reg_read(addr))

#define DCM_BRINGUP

static int dcm_busdvt(int on);
static int dcm_peri(int on);
static int dcm_infra(int on);
static int mtk_dcm_init(void);
#endif
