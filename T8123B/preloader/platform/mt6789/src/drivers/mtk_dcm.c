#include "mtk_dcm_busdvt.h"
#include <typedefs.h>
#include <platform.h>
#include <mtk_dcm_busdvt.h>

void dcm_dump_regs(void)
{
    dcm_pr_info("\n******** Preloader dcm dump register ********\n");

    REG_DUMP(INFRA_BUS_DCM_CTRL);
    REG_DUMP(P2P_RX_CLK_ON);
    REG_DUMP(INFRA_AXIMEM_IDLE_BIT_EN_0);
    REG_DUMP(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0);
    REG_DUMP(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1);
    REG_DUMP(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2);
    REG_DUMP(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0);
    REG_DUMP(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1);
}

static int dcm_infra(int on)
{
    dcm_infracfg_ao_aximem_bus_dcm(on);
    dcm_infracfg_ao_infra_bus_dcm(on);
    dcm_infracfg_ao_infra_rx_p2p_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_fmem_sub_dcm(on);
    return 0;
}

static int dcm_peri(int on)
{
    dcm_peri_ao_bcrm_peri_bus_dcm(on);
    return 0;
}

static int dcm_busdvt(int on)
{
    dcm_pr_info("\n******** before busdvt dcm enable ********\n");

    dcm_infracfg_ao_aximem_bus_dcm(on);
    dcm_infracfg_ao_infra_bus_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_dcm(on);
    dcm_infra_ao_bcrm_infra_bus_fmem_sub_dcm(on);

    dcm_pr_info("\n******** after busdvt dcm enable ********\n");
    dcm_dump_regs();
    return 0;
}

int mtk_dcm_init(void)
{
    int ret = 0;
#if !defined(DCM_BRINGUP)
    ret = dcm_infra(1);
    ret |= dcm_peri(1);

#if defined(MTK_DCM_BUSDVT)
    ret |= dcm_busdvt(1);
#endif /* defined(MTK_DCM_BUSDVT) */
#endif /* !defined(DCM_BRINGUP) */
    return ret;
}
