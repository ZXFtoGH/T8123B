#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include "ufs_aio_platform.h"
#include "srclken_rc.h"

#define IS_TRUE		(1)
#define IS_FALSE		(0)

/* RC settle time setting */
/*#define NO_DCXO_XO_CHN		((1 << CHN_COANT) | (1 << CHN_SCP) | (1 << CHN_RESERVE))*/
//#define BT_ONLY_HW_MODE	((1 << CHN_BT))	/* only BT use HW side-band signal */
#define FULL_SET_HW_MODE	0	/* dcxo mode use pmrc_en */
#define VCORE_SETTLE_TIME	1	/* ~= 30us */
#define ULPOSC_SETTLE_TIME	4	/* ~= ? 150us */
#define NON_DCXO_SETTLE_TIME	0x1	/* 2^(step_sz+5) * 0x33 *30.77ns~=400us */
#define DCXO_SETTLE_TIME	0x41	/* 2^(step_sz+5) * 0x87 *30.77ns~= 1063us */
#define CENTROL_CNT_STEP	0x3	/* Fix in 3, central align with Mxx Channel */
#define DCXO_STABLE_TIME 	0x70
#define XO_STABLE_TIME 		0x29
#define KEEP_RC_SPI_ACTIVE 	1
#define SRCLKEN_RC_EN_SEL 	0

#define INIT_SUBSYS_FPM_TO_LPM ( 1 << CHN_RF | 1 << CHN_DEEPIDLE \
				| 1 << CHN_MD | 1 << CHN_GPS | 1 <<CHN_BT \
				| 1 << CHN_WIFI | 1 << CHN_MCU | 1 << CHN_COANT \
				| 1 << CHN_NFC | 1 << CHN_UFS | 1 << CHN_SCP | 1 << CHN_RESERVE)

#define INIT_SUBSYS_FPM_TO_BBLPM ( 1 << CHN_DEEPIDLE)

#define INIT_SUBSYS_TO_HW ( 1 << CHN_SUSPEND | 1 << CHN_RF | 1 << CHN_DEEPIDLE \
				| 1 << CHN_GPS | 1 <<CHN_BT \
				| 1 << CHN_WIFI | 1 << CHN_MCU | 1 << CHN_COANT \
				| 1 << CHN_NFC )

/* RC central setting */
#define RC_CENTRAL_ENABLE	1
#define RC_CENTRAL_DISABLE	0
#if MTK_SRCLKEN_RC_FULL_SET
#define SPI_TRIG_MODE		SRCLKENAO_MODE		/* use srlckenao to set vcore */
#define IS_SPI_DONE_RELEASE	IS_FALSE		/* release vcore when spi request done */
#define IS_SPI2PMIC_SET_CLR	IS_FALSE		/* register direct write */
#define SRCLKENO_0_CTRL_M	MERGE_OR_MODE 		/* merge with spm */
#define VREQ_CTRL_M		BYPASS_MODE		/* merge with vreq */
#define ULPOSC_CTRL_M		BYPASS_MODE		/* merge with ulposc */
#define PWRAP_CTRL_M		MERGE_OR_MODE		/* merge with pwrap_scp */
#endif
#define SPI_CLK_SRC		RC_32K	/* pmic spec under 200us */

/* other setting */
#define PMRC_CON0		0x1A6	/* default use this reg direct write */
#define PMRC_CON0_SET		0x1A8
#define PMRC_CON0_CLR		0x1AA

#define DCXO_FPM_CTRL_MODE	MERGE_OR_MODE | ASYNC_MODE /* merge with spm */

#define PWRAP_TMOUT_VAL	0xA	/* 31us * 0xa ~= 310us */

#if MTK_SRCLKEN_RC_FULL_SET
#define FPM_MSK_B		FULL_SET_HW_MODE
#define MD0_SRCLKENO_0_MASK_B	IS_FALSE		/* md0 control by pmrc */

#define SUB_BBLPM_SET		( 1 << CHN_COANT | 1 << CHN_DEEPIDLE )
#define SUB_FPM_SET		(1 << CHN_SUSPEND | 1 << CHN_RF \
				| 1 << CHN_MD | 1 << CHN_GPS | 1 << CHN_BT \
				| 1 << CHN_WIFI | 1 << CHN_MCU \
				| 1 << CHN_NFC | 1 << CHN_UFS | 1 << CHN_SCP | 1 << CHN_RESERVE)

#define SW_BBLPM_HIGH  1
#define SW_BBLPM_LOW 0
#define SW_FPM_HIGH  1
#define SW_FPM_LOW 0
#define DXCO_SETTLE_BLK_EN 1
#define DXCO_SETTLE_BLK_DIS 0

#define SUB_CTRL_CON(_id, _dcxo_prd, _xo_prd \
		, _sw_bblpm, _sw_fpm, _sw_rc, _bypass_cmd, _dcxo_settle_blk_en) {	\
		.id = _id,		\
		.dcxo_prd = _dcxo_prd,		\
		.xo_prd = _xo_prd,		\
		.cnt_step = CENTROL_CNT_STEP,	\
		.track_en = 0x0,		\
		.req_ack_imd_en = 0x1,		\
		.xo_soc_link_en = 0x0,		\
		.sw_bblpm = _sw_bblpm,		\
		.sw_fpm = _sw_fpm,		\
		.sw_rc = _sw_rc, 	\
		.bypass_cmd = _bypass_cmd, 	\
		.dcxo_settle_blk_en = _dcxo_settle_blk_en,		\
	}

static struct subsys_rc_con rc_ctrl[MAX_CHN_NUM] = {
	SUB_CTRL_CON(CHN_SUSPEND, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_RF, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_DEEPIDLE, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_MD, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_GPS, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_BT, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_WIFI, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_MCU, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_COANT, 0x0, 0x0,  SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x1 /*bypass*/,DXCO_SETTLE_BLK_DIS),
	SUB_CTRL_CON(CHN_NFC, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_UFS, DCXO_STABLE_TIME, XO_STABLE_TIME, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x0 /*bypass*/,DXCO_SETTLE_BLK_EN),
	SUB_CTRL_CON(CHN_SCP, 0x0, 0x0, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x1 /*bypass*/,DXCO_SETTLE_BLK_DIS),
	SUB_CTRL_CON(CHN_RESERVE, 0x0, 0x0, SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE, 0x1 /*bypass*/,DXCO_SETTLE_BLK_DIS),
};
#endif

int ulposc_init(void)
{
	/* calibrate ULPOSC1 */
	/* use scp's calibration function if need */
	return 0;
}

void rc_dump_reg_info(void){
#if SRCLKEN_DBG
	unsigned int chn_n;

	print("SRCLKEN_RC_CFG:0x%x\n", DRV_Reg32(SRCLKEN_RC_CFG));
	print("RC_CENTRAL_CFG1:0x%x\n", DRV_Reg32(RC_CENTRAL_CFG1));
	print("RC_CENTRAL_CFG2:0x%x\n", DRV_Reg32(RC_CENTRAL_CFG2));
	print("RC_CENTRAL_CFG3:0x%x\n", DRV_Reg32(RC_CENTRAL_CFG3));
	print("RC_CENTRAL_CFG4:0x%x\n", DRV_Reg32(RC_CENTRAL_CFG4));
	print("RC_DCXO_FPM_CFG:0x%x\n", DRV_Reg32(RC_DCXO_FPM_CFG));
	print("SUBSYS_INTF_CFG = 0x%x\n", DRV_Reg32(SUBSYS_INTF_CFG));
	print("RC_SPI_STA_0:0x%x\n", DRV_Reg32(RC_SPI_STA_0));
	print("RC_PI_PO_STA:0x%x\n", DRV_Reg32(RC_PI_PO_STA));

	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++){
		print("M0%d: 0x%x\n", chn_n, DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * chn_n)));
	}

#endif
}

/* RC initial flow and relative setting */
static void __rc_ctrl_mode_switch(enum chn_id id, enum rc_ctrl_m mode)
{
#ifdef MTK_SRCLKEN_RC_SUPPORT

	unsigned int value = 0;

	ASSERT(id >= CHN_SUSPEND && id < MAX_CHN_NUM);

	if(mode == INIT_MODE){
		value = ((rc_ctrl[id].dcxo_settle_blk_en & DCXO_SETTLE_BLK_EN_MSK) << DCXO_SETTLE_BLK_EN_SHFT) |
				((rc_ctrl[id].bypass_cmd & BYPASS_CMD_EN_MSK) << BYPASS_CMD_EN_SHFT) |
				((rc_ctrl[id].sw_rc & SW_SRCLKEN_RC_MSK) << SW_SRCLKEN_RC_SHFT) |
				((rc_ctrl[id].sw_fpm & SW_SRCLKEN_FPM_MSK) << SW_SRCLKEN_FPM_SHFT) |
				((rc_ctrl[id].sw_bblpm & SW_SRCLKEN_BBLPM_MSK) << SW_SRCLKEN_BBLPM_SHFT) |
				((rc_ctrl[id].xo_soc_link_en & XO_SOC_LINK_EN_MSK) << XO_SOC_LINK_EN_SHFT) |
				((rc_ctrl[id].req_ack_imd_en & REQ_ACK_LOW_IMD_EN_MSK) << REQ_ACK_LOW_IMD_EN_SHFT) |
				((rc_ctrl[id].track_en & SRCLKEN_TRACK_M_EN_MSK) << SRCLKEN_TRACK_M_EN_SHFT) |
				((rc_ctrl[id].cnt_step & CNT_PRD_STEP_MSK) << CNT_PRD_STEP_SHFT) |
				((rc_ctrl[id].xo_prd & XO_STABLE_PRD_MSK) << XO_STABLE_PRD_SHFT) |
				((rc_ctrl[id].dcxo_prd & DCXO_STABLE_PRD_MSK) << DCXO_STABLE_PRD_SHFT) ;
	}else if (mode == SW_MODE){
		value = DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)) | ( 0x1 << SW_SRCLKEN_RC_SHFT);
	}else if (mode == HW_MODE){
		value = DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)) & ~(SW_SRCLKEN_RC_MSK << SW_SRCLKEN_RC_SHFT);
	}else
		return;

	DRV_WriteReg32(RC_MXX_SRCLKEN_CFG + (0x4 * id),	value);

	print("M0%d: 0x%x\n", id, DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)));
#endif
}


/* RC subsys FPM control*/
static void __rc_ctrl_fpm_switch(enum chn_id id, unsigned int mode)
{
#ifdef MTK_SRCLKEN_RC_SUPPORT

	unsigned int value = 0;

	ASSERT(id >= CHN_SUSPEND && id < MAX_CHN_NUM);

	if (mode == SW_FPM_HIGH){
		value = DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)) | ( 0x1 << SW_SRCLKEN_FPM_SHFT);
	}else if (mode == SW_FPM_LOW){
		value = DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)) & ~(SW_SRCLKEN_FPM_MSK << SW_SRCLKEN_FPM_SHFT);
	}else
		return;

	rc_ctrl[id].sw_fpm = mode;
	DRV_WriteReg32(RC_MXX_SRCLKEN_CFG + (0x4 * id), value);

	print("M0%d FPM SWITCH: 0x%x\n", id, DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)));
#endif
}

static void __rc_ctrl_bblpm_switch(enum chn_id id, unsigned int mode)
{
#ifdef MTK_SRCLKEN_RC_SUPPORT

	unsigned int value = 0;

	ASSERT(id >= CHN_SUSPEND && id < MAX_CHN_NUM);

	if (mode == SW_BBLPM_HIGH){
		value = DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)) | ( 0x1 << SW_SRCLKEN_BBLPM_SHFT);
	}else if (mode == SW_BBLPM_LOW){
		value = DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)) & ~(SW_SRCLKEN_BBLPM_MSK << SW_SRCLKEN_BBLPM_SHFT);
	}else
		return;

	rc_ctrl[id].sw_bblpm = mode;
	DRV_WriteReg32(RC_MXX_SRCLKEN_CFG + (0x4 * id), value);

	print("M0%d BBLPM SWITCH: 0x%x\n", id, DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)));
#endif
}

void rc_init_subsys_hw_mode(void){
#ifdef MTK_SRCLKEN_RC_SUPPORT
	unsigned int chn_n;
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++){
		if( INIT_SUBSYS_TO_HW & (1 << chn_n))
			__rc_ctrl_mode_switch(chn_n, HW_MODE);
	}
#endif
}

void rc_init_subsys_lpm(void){
#ifdef MTK_SRCLKEN_RC_SUPPORT
	unsigned int chn_n;
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++){
		if( INIT_SUBSYS_FPM_TO_LPM & (1 << chn_n))
			__rc_ctrl_fpm_switch(chn_n, SW_FPM_LOW);
	}
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++){
		if( INIT_SUBSYS_FPM_TO_BBLPM & (1 << chn_n))
			__rc_ctrl_bblpm_switch(chn_n, SW_BBLPM_HIGH);
	}
#endif
}

void rc_ctrl_mode_switch_hw(enum chn_id id)
{
	__rc_ctrl_mode_switch(id, HW_MODE);
}

void rc_ctrl_mode_switch_sw(enum chn_id id)
{
	__rc_ctrl_mode_switch(id, SW_MODE);
}

void rc_ctrl_mode_switch_init(enum chn_id id)
{
	__rc_ctrl_mode_switch(id, INIT_MODE);
}

extern void pmwrap_interface_init(void);

static int srclken_rc_chk_init_cfg(void)
{
	int ret = 0;

#if MTK_SRCLKEN_RC_BRINGUP
	print("Bring-UP : skip srclken_rc init\n");

	return SRLCKEN_RC_BRINGUP;
#else
#if defined(SLT)
	print("%s: is skipped for SLT\n", __func__);
	return SRCLKEN_RC_SKIP;
#endif
	pmwrap_interface_init();

	/*enable debug trace*/
	#if SRCLKEN_DBG
	DRV_WriteReg32(RC_DEBUG_TRACE, 1);
	DRV_WriteReg32(RC_DEBUG_CFG, DRV_Reg32(RC_DEBUG_CFG) | (TRACE_MODE_EN_MSK << TRACE_MODE_EN_SHFT));
	#endif
#ifndef MTK_SRCLKEN_RC_SUPPORT
	return SRCLKEN_RC_DISABLE;
#else
	return SRCLKEN_RC_ENABLE;
#endif
#endif
}

int srclken_rc_init(void)
{
#if MTK_SRCLKEN_RC_BRINGUP
	print("%s skip for bring up\n");
	return 0;
#else
	/* new co-clock architecture srclkenrc implement here */
	unsigned int chn_n;
	int ret = 0;

	/* check platform config to proceed init flow */
	if (srclken_rc_chk_init_cfg() != SRCLKEN_RC_ENABLE)
		return ret;

	/* Enable ulposc1 first */
	ret = ulposc_init();

	/* Set SW RESET 1 */
	DRV_WriteReg32(SRCLKEN_RC_CFG, 1 << SW_RESET_SHFT);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW CG 1 */
	DRV_WriteReg32(SRCLKEN_RC_CFG,
		(1 << SW_RESET_SHFT) | (1 << CG_32K_EN_SHFT)
		| (1 << CG_FCLK_EN_SHFT) | (1 << CG_FCLK_FR_EN_SHFT));

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set Clock Mux*/
	DRV_WriteReg32(SRCLKEN_RC_CFG,
		(1 << SW_RESET_SHFT) |(1 << CG_32K_EN_SHFT)
		| (1 << CG_FCLK_EN_SHFT) | (1 << CG_FCLK_FR_EN_SHFT)
		| (1 << MUX_FCLK_FR_SHFT));

	/* Set req_filter m00~m12 as default SW_FPM */
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++)
		rc_ctrl_mode_switch_init(chn_n);

	/* Set PMIC addr for SPI CMD */
	DRV_WriteReg32(RC_PMIC_RCEN_ADDR, PMRC_CON0);

	DRV_WriteReg32(RC_PMIC_RCEN_SET_CLR_ADDR,
			(PMRC_CON0_CLR << 16 | PMRC_CON0_SET));

	DRV_WriteReg32(RC_CMD_ARB_CFG, 0);

	/* CFG1 setting for spi cmd config */
	DRV_WriteReg32(RC_CENTRAL_CFG1,
		DCXO_SETTLE_TIME << DCXO_SETTLE_T_SHFT |
		NON_DCXO_SETTLE_TIME << NON_DCXO_SETTLE_T_SHFT |
		ULPOSC_SETTLE_TIME << ULPOSC_SETTLE_T_SHFT |
		VCORE_SETTLE_TIME << VCORE_SETTLE_T_SHFT |
		SRCLKEN_RC_EN_SEL << SRCLKEN_RC_EN_SEL_SHFT |
		KEEP_RC_SPI_ACTIVE << RC_SPI_ACTIVE_SHFT |
		IS_SPI2PMIC_SET_CLR << RCEN_ISSUE_M_SHFT |
		RC_CENTRAL_DISABLE << SRCLKEN_RC_EN_SHFT);

	/* CFG2 setting for signal mode of each control mux */
	DRV_WriteReg32(RC_CENTRAL_CFG2,
		SPI_CLK_SRC << PWRAP_SLP_MUX_SEL_SHFT |
		PWRAP_CTRL_M << PWRAP_SLP_CTRL_M_SHFT |
		ULPOSC_CTRL_M << ULPOSC_CTRL_M_SHFT |
		IS_SPI_DONE_RELEASE << SRCVOLTEN_VREQ_M_SHFT |
		SPI_TRIG_MODE << SRCVOLTEN_VREQ_SEL_SHFT |
		VREQ_CTRL_M << VREQ_CTRL_SHFT |
		SRCLKENO_0_CTRL_M << SRCVOLTEN_CTRL_SHFT);

	DRV_WriteReg32(RC_CENTRAL_CFG3,
		0x4 << TO_LPM_SETTLE_T_SHFT |
		1 << TO_BBLPM_SETTLE_EN_SHFT |
		1 << BLK_COANT_DXCO_MD_TARGET_SHFT |
		1 << BLK_SCP_DXCO_MD_TARGET_SHFT |
		1 << TO_LPM_SETTLE_EN_SHFT);

	/* Set srclkeno_0/conn_bt as factor to allow dcxo change to FPM */
	DRV_WriteReg32(RC_DCXO_FPM_CFG,
		FPM_MSK_B << SUB_SRCLKEN_FPM_MSK_B_SHFT |
		MD0_SRCLKENO_0_MASK_B << SRCVOLTEN_FPM_MSK_B_SHFT |
		DCXO_FPM_CTRL_MODE << DCXO_FPM_CTRL_M_SHFT);

	/* Set bblpm/fpm channel */
	DRV_WriteReg32(SUBSYS_INTF_CFG,
		SUB_BBLPM_SET << SRCLKEN_BBLPM_MASK_B_SHFT|
		SUB_FPM_SET << SRCLKEN_FPM_MASK_B_SHFT);

	/* Trigger srclken_rc enable */
	DRV_WriteReg32(RC_CENTRAL_CFG1, DRV_Reg32(RC_CENTRAL_CFG1) | (RC_CENTRAL_ENABLE << SRCLKEN_RC_EN_SHFT));

	DRV_WriteReg32(RC_CENTRAL_CFG4,
		0x1 << SLEEP_VLD_MODE_SHFT |
		0x1 << PWRAP_VLD_FORCE_SHFT |
		0x800 << KEEP_RC_SPI_ACTIVE_SHFT);


	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW RESET 0 */
	DRV_WriteReg32(SRCLKEN_RC_CFG,
		(1 << CG_32K_EN_SHFT) | (1 << CG_FCLK_EN_SHFT) |
		(1 << CG_FCLK_FR_EN_SHFT) | (1 << MUX_FCLK_FR_SHFT));

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW CG 0 */
	DRV_WriteReg32(SRCLKEN_RC_CFG, 1 << MUX_FCLK_FR_SHFT);

	/* Wait 500us */
	gpt_busy_wait_us(500);

	/* Set req_filter m00~m12 FPM to LPM*/
	rc_init_subsys_lpm();

	/* Polling ACK of Initial Subsys Input */
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++) {
		unsigned int chk_sta, shift_chn_n = 0;
		int i = 0, temp = 0;

		if(chn_n > 0)
			shift_chn_n = 1;

		chk_sta = (rc_ctrl[chn_n].sw_fpm & SW_SRCLKEN_FPM_MSK) << 1 |
			(rc_ctrl[chn_n].sw_bblpm & SW_SRCLKEN_BBLPM_MSK) << 3;
		; /* Fix RC_MXX_REQ_STA_0 register shift */
		while((DRV_Reg32(RC_MXX_REQ_STA_0 + (chn_n + shift_chn_n) * 4) & 0xa) != chk_sta) {
			udelay(10);
			i++;
			if(i > 200) {
				#if 1
				pmic_read_interface(MT6359_PMRC_CON0, &temp,
					PMIC_PMRC_EN_MASK, PMIC_PMRC_EN_SHIFT);
				#endif

				print("%s: polling M%d status fail.(R:0x%x)(C:0x%x)(PMRC:0x%x)\n", __func__, chn_n,
					DRV_Reg32(RC_MXX_REQ_STA_0 + (chn_n + shift_chn_n) * 4),DRV_Reg32(RC_MXX_SRCLKEN_CFG + chn_n * 4), temp);
				ret = -1;
				break;
			}
		}
	}

	/* Set req_filter m00~m12 */
	rc_init_subsys_hw_mode();

	/* release force pmic req signal*/
	DRV_WriteReg32(RC_CENTRAL_CFG4,
		0x1 << SLEEP_VLD_MODE_SHFT |
		0x800 << KEEP_RC_SPI_ACTIVE_SHFT);

	rc_dump_reg_info();

	return ret;
#endif
}
