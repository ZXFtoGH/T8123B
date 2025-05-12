#include <typedefs.h>
#include <mtk_drm.h>
#include <timer.h>

#define udelay(x)	gpt_busy_wait_us(x)
#define mdelay(x)	gpt_busy_wait_us(x*1000)

static unsigned int mtk_drm_get_debug_ctl(void)
{
	static unsigned int drm_debug_ctl = 0;

	/*
	 * Note:
	 * Because some bits in register DEBUG_CTL will be cleared after
	 * writing DRM_MODE, we use a static variable to keep original DEBUG_CTL.
	 *
	 * Effected Bits:
	 * 16: ddr_reserve_success
	 * 18: emi_dcs_success
	 * 20: dvfsrc_success
	 */
	if (drm_debug_ctl == 0)
		drm_debug_ctl = READ_REG(MTK_DRM_DEBUG_CTL);

	return drm_debug_ctl;
}

static void mtk_drm_keep_essential_info(void)
{
	mtk_drm_get_debug_ctl();
}

int drm_dram_reserved(int enable)
{
	volatile unsigned int tmp = 0, ret = 0;

	/*
	 * DDR reserved mode may be switched on/off anytime by caller.
	 *
	 * DDR reserved mode switch on/off flow will modify register MODE.
	 * Since register STATUS and some bits in DEBUG_CTL will be reset
	 * if register MODE has any changes. Keep those register values first
	 * before register MODE is written later.
	 */
	mtk_drm_keep_essential_info();

	if (enable == 1) {
		/* enable ddr reserved mode */
		tmp = READ_REG(MTK_DRM_MODE);
		tmp |= (MTK_DRM_MODE_DDR_RESERVE|MTK_DRM_MODE_KEY);
		WRITE_REG(tmp, MTK_DRM_MODE);
	} else if (enable == 0) {
		/* disable ddr reserved mode, set reset mode,
				disable watchdog output reset signal */
		tmp = READ_REG(MTK_DRM_MODE);
		tmp &= (~MTK_DRM_MODE_DDR_RESERVE);
		tmp |= MTK_DRM_MODE_KEY;
		WRITE_REG(tmp, MTK_DRM_MODE);
	} else {
		printf("Wrong input %d, should be 1(enable) or 0(disable) in %s\n", enable, __func__);
		ret = -1;
	}

	printf("%s: MTK_DRM_MODE(%x)\n", __func__, tmp);
	return ret;
}

int drm_is_reserve_ddr_enabled(void)
{
	unsigned int drm_mode;

	drm_mode = READ_REG(MTK_DRM_MODE);

	if (drm_mode & 0x1)
		return 1;
	else
		return 0;
}

int drm_is_dram_slf(void)
{
	unsigned int drm_dbg_ctrl;

	drm_dbg_ctrl = READ_REG(MTK_DRM_DEBUG_CTL);

	if (drm_dbg_ctrl & MTK_DRM_DDR_SREF_STA) {
		printf("DDR is in self-refresh. %x\n", drm_dbg_ctrl);
		return 1;
	} else {
		printf("DDR is not in self-refresh. %x\n", drm_dbg_ctrl);
		return 0;
	}
}

int drm_wait_for_reg_update_done(unsigned int reg_addr, unsigned mask, unsigned target_val)
{
	unsigned timeout_ms = 5;
	unsigned int read_val;
	unsigned int read_times = 0;
	unsigned int elapsed_ms = 0;

	while (1) {
		read_val = READ_REG(reg_addr);

		if (target_val == (read_val & mask))
			break;

		read_times++;

		if (read_times && 0 == (read_times % 100)) {

			mdelay(1);
			elapsed_ms += 1;

			if (elapsed_ms > timeout_ms) {
				printf("%s: ERROR! Update reg 0x%x timeout! Mask: 0x%x Target: 0x%x\n",
				       __func__, reg_addr, mask, target_val);

				return -1;
			}
		}
	}

	return 0;
}

int drm_update_reg(unsigned int reg, unsigned int func, unsigned int bits)
{
	volatile unsigned int val;
	int ret;

	val = READ_REG(reg);

	if (func == DRM_REG_SET)
		val |= bits;
	else if (func == DRM_REG_CLR)
		val &= (~bits);
	else
		return -1;

	/* add unlock key */
	if (reg == MTK_DRM_DEBUG_CTL)
		val |= MTK_DRM_DEBUG_CTL_KEY;
	else if (reg == MTK_DRM_DEBUG_CTL2)
		val |= MTK_DRM_DEBUG_CTL2_KEY;
	else {
		printf("DRM: Invalid! Set bit 0x%x in reg 0x%x fail!\n", bits, reg);
		return -1;
	}

	/* update register */
	WRITE_REG(val, reg);

	/* wait until register updating done */
	if (func == DRM_REG_SET)
		val = bits;
	else
		val = 0;

	/*
	 * Must ensure waiting-done API has timeout mechanism
	 * because in some scenarios with security feature toggled,
	 * below registers will be read-only. Writting operation
	 * will get timeout here.
	 *
	 * DEBUG_CTL / DEBUG_CLT2 / DDR_RESERVE_MODE bit in MODE
	 */
	ret = drm_wait_for_reg_update_done(reg, bits, val);

	printf("%s: %d, bits: 0x%x, addr: 0x%x, val: 0x%x\n", __func__,
		func, bits, reg, READ_REG(reg));

	return ret;
}

int drm_is_reserve_ddr_mode_success(void)
{
	unsigned int drm_dbg_ctrl;

	/*
	 * MTK_DRM_DDR_RESERVE_RTA bit will be reset by modifying register MODE.
	 * Read DEBUG_CTL value kept by mtk_drm_get_debug_ctl().
	 */
	drm_dbg_ctrl = mtk_drm_get_debug_ctl();

	if (drm_dbg_ctrl & MTK_DRM_DDR_RESERVE_STA) {
		printf("DRM DDR reserve mode success! %x\n", drm_dbg_ctrl);
		return 1;
	} else {
		printf("DRM DDR reserve mode FAIL! %x\n", drm_dbg_ctrl);
		return 0;
	}
}

int drm_release_rg_dramc_conf_iso(void)
{
	int ret;
	int success;

	ret = drm_update_reg(MTK_DRM_DEBUG_CTL,
		DRM_REG_CLR, MTK_DRM_DRAMC_CONF_ISO);

	success = drm_is_reserve_ddr_mode_success();

	printf("DDR RESERVE Success %d\n", success);

	return ret;
}

int drm_release_rg_dramc_iso(void)
{
	return drm_update_reg(MTK_DRM_DEBUG_CTL, DRM_REG_CLR, MTK_DRM_DRAMC_ISO);
}

int drm_release_rg_dramc_sref(void)
{
	return drm_update_reg(MTK_DRM_DEBUG_CTL, DRM_REG_CLR, MTK_DRM_DRAMC_SREF);
}

int drm_is_dvfsrc_success(void)
{
	unsigned int drm_dbg_ctrl;
	unsigned int success;

	drm_dbg_ctrl = mtk_drm_get_debug_ctl();
	success = (drm_dbg_ctrl & MTK_DRM_DVFSRC_SUCCESS)? 1 : 0;

	printf("DVFSRC_SUCCESS %d\n", success);

	return success;
}

int drm_is_dvfsrc_enable(void)
{
	unsigned int reg;

	reg = READ_REG(MTK_DRM_DEBUG_CTL2);

	if (reg & MTK_DRM_DVFSRC_EN)
		return 1;
	else
		return 0;
}

void mtk_drm_init(void)
{
	unsigned int latch_ctrl;

	/* Save DRM debug info */
	printf("Save DRM_DEBUG_CTL(0x%x)\n", mtk_drm_get_debug_ctl());

	/* Configure DRM_LATCH_CTL */
	latch_ctrl = READ_REG(MTK_DRM_LATCH_CTL);
	latch_ctrl |= MTK_DRM_LATCH_CTL_KEY;
	latch_ctrl |= MTK_DRM_LATCH_EN;
	latch_ctrl |= MTK_DRM_MCU_LATCH_EN;
	latch_ctrl |= MTK_DRM_SPM_LATCH_EN;
	latch_ctrl |= MTK_DRM_GPU_EXT_OFF_EN;
	latch_ctrl |= MTK_DRM_MD_EXT_OFF_EN;
	latch_ctrl &= ~(MTK_DRM_MCU_LATCH_SELECT | MTK_DRM_SPM_LATCH_SELECT);
	latch_ctrl |= MTK_DRM_DRAMC_LATCH_EN;
	latch_ctrl |= MTK_DRM_EMI_LATCH_EN;
	latch_ctrl |= MTK_DRM_DVFSRC_LATCH_EN;
	latch_ctrl |= MTK_DRM_DEBUGSYS_LATCH_EN;
	latch_ctrl |= (MTK_DRM_DRAMC_RD_TEST_EN | MTK_DRM_DRAMC_RDWT_TEST_EN);
	WRITE_REG(latch_ctrl, MTK_DRM_LATCH_CTL);

	/* Configure DRM_LATCH_CTL2 */
	latch_ctrl = READ_REG(MTK_DRM_LATCH_CTL2);
	latch_ctrl |= MTK_DRM_LATCH_CTL2_KEY;

	/* DFD feature */
	latch_ctrl |= MTK_DRM_MCU_DFD_EN;
	latch_ctrl &= ~(MTK_DRM_MCU_DFD_TIMEOUT_MASK << MTK_DRM_MCU_DFD_TIMEOUT_OFS);
	latch_ctrl |= (MTK_DRM_MCU_DFD_TIMEOUT_VALUE << MTK_DRM_MCU_DFD_TIMEOUT_OFS);

	WRITE_REG(latch_ctrl, MTK_DRM_LATCH_CTL2);

	/* Dump DRM control regs */
	printf("DRM_LATCH_CTL : 0x%x\n", READ_REG(MTK_DRM_LATCH_CTL));
	printf("DRM_LATCH_CTL2: 0x%x\n", READ_REG(MTK_DRM_LATCH_CTL2));

	/*
	 * Configure DRM_DEBUG_CTL
	 *
	 * Release DDR reserve mode related control.
	 *
	 * NOTE: This job must be done earlier than DVS/DVFSRC initialization.
	 */
	drm_update_reg(MTK_DRM_DEBUG_CTL, DRM_REG_SET,
		MTK_DRM_DRM_LITE_MODE);

	drm_update_reg(MTK_DRM_DEBUG_CTL, DRM_REG_CLR,
		MTK_DRM_DVFSRC_PAUSE);

	drm_update_reg(MTK_DRM_DEBUG_CTL2, DRM_REG_CLR,
		MTK_DRM_DVFSRC_EN);

	drm_update_reg(MTK_DRM_DEBUG_CTL2, DRM_REG_CLR,
		MTK_DRM_EMI_DCS_PAUSE);

	drm_update_reg(MTK_DRM_DEBUG_CTL2, DRM_REG_CLR,
		MTK_DRM_EMI_DCS_EN);

	printf("MTK_DRM_DEBUG_CTL : 0x%x\n", READ_REG(MTK_DRM_DEBUG_CTL));
	printf("MTK_DRM_DEBUG_CTL2: 0x%x\n", READ_REG(MTK_DRM_DEBUG_CTL2));
}

