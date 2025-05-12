#include <platform.h>
#include <mtk_pcie.h>

#define PERICFG_AO_BASE		0x11036000

void mtk_pcie_reset(void)
{
    unsigned int val;

    /* reset PCIe mac and phy */
    val = readl(PERICFG_AO_BASE);
    val |= 0x0C000003;
    writel(val, PERICFG_AO_BASE);
}
