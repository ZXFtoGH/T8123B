#include <platform.h>
#include <spm_mtcmos.h>
#include <spm_mtcmos_internal.h>
#include <timer.h>  //udelay
#include <pll.h>

#if CFG_FPGA_PLATFORM
#define IGNORE_MTCMOS_CHECK	1 //for FPGA
#endif

#define POWERON_CONFIG_EN	(SPM_BASE + 0x0000)

/* Define MTCMOS power control */
#define PWR_RST_B                        (0x1 << 0)
#define PWR_ISO                          (0x1 << 1)
#define PWR_ON                           (0x1 << 2)
#define PWR_ON_2ND                       (0x1 << 3)
#define PWR_CLK_DIS                      (0x1 << 4)
#define SRAM_CKISO                       (0x1 << 5)
#define SRAM_ISOINT_B                    (0x1 << 6)

/* Define MTCMOS Bus Protect Mask */

#define DIS_PROT_STEP1_0_MASK	((0x1 << 10) \
			| (0x1 << 12))
#define DIS_PROT_STEP1_0_ACK_MASK	((0x1 << 10) \
			| (0x1 << 12))
#define DIS_PROT_STEP2_0_MASK	((0x1 << 6) \
			| (0x1 << 23))
#define DIS_PROT_STEP2_0_ACK_MASK	((0x1 << 6) \
			| (0x1 << 23))

#define PWR_STATUS		(SPM_BASE + 0x16C)
#define PWR_STATUS_2ND		(SPM_BASE + 0x170)
#define DIS_PWR_CON		(SPM_BASE + 0x354)

/* Define MTCMOS Power Status Mask */

#define DIS_PWR_STA_MASK			(0x1 << 21)

/* Define Non-CPU SRAM Mask */

#define DIS_SRAM_PDN		(0x1 << 8)
#define DIS_SRAM_PDN_ACK		(0x1 << 12)

enum {
	DBG_ID_DISP =		0,
	DBG_ID_NUM =		1,
};

#define INCREASE_STEPS \
	do { \
		DBG_STEP++; \
		loop_cnt = 0; \
	} while (0)

static int DBG_ID;
static int DBG_STA;
static int DBG_STEP;
static unsigned int loop_cnt = 0;

#ifndef IGNORE_MTCMOS_CHECK

static void ram_console_update(void)
{
	if (loop_cnt > 10000)
		dbg_print("%s %d MTCMOS hang at %s flow step %d\n",
				"[clkmgr]",
				DBG_ID,
				DBG_STA ? "pwron":"pdn",
				DBG_STEP);

	loop_cnt++;
}

#endif

/**************************************
 * for non-CPU MTCMOS
 **************************************/

void spm_mtcmos_ctrl_disp(int state)
{
	DBG_ID = DBG_ID_DISP;
	DBG_STA = state;
	DBG_STEP = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* STA_POWER_DOWN */
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_SET, DIS_PROT_STEP1_0_MASK);
		#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_STA1) & DIS_PROT_STEP1_0_ACK_MASK) != DIS_PROT_STEP1_0_ACK_MASK)
			ram_console_update();

		INCREASE_STEPS;
		#endif
		/* TINFO="Set bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SET, DIS_PROT_STEP2_0_MASK);
		#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_STA1) & DIS_PROT_STEP2_0_ACK_MASK) != DIS_PROT_STEP2_0_ACK_MASK)
			ram_console_update();

		INCREASE_STEPS;
		#endif
		/* TINFO="Set DIS_SRAM_PDN = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | DIS_SRAM_PDN);
		#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until DIS_SRAM_PDN_ACK = 1" */
		while ((spm_read(DIS_PWR_CON) & DIS_SRAM_PDN_ACK) != DIS_SRAM_PDN_ACK)
			ram_console_update();

		INCREASE_STEPS;
		#endif
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ON_2ND);
		#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & DIS_PWR_STA_MASK)
				|| (spm_read(PWR_STATUS_2ND) & DIS_PWR_STA_MASK))
			ram_console_update();

		INCREASE_STEPS;
		#endif
	} else {
		/* STA_POWER_ON */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ON_2ND);
		#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (((spm_read(PWR_STATUS) & DIS_PWR_STA_MASK) != DIS_PWR_STA_MASK)
				|| ((spm_read(PWR_STATUS_2ND) & DIS_PWR_STA_MASK) != DIS_PWR_STA_MASK))
			ram_console_update();

		INCREASE_STEPS;
		#endif
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_RST_B);
		/* TINFO="Set DIS_SRAM_PDN = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~DIS_SRAM_PDN);
		#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until DIS_SRAM_PDN_ACK = 0" */
		while (spm_read(DIS_PWR_CON) & DIS_SRAM_PDN_ACK)
			ram_console_update();

		INCREASE_STEPS;
		#endif
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_CLR, DIS_PROT_STEP2_0_MASK);
		/* TINFO="Release bus protect" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_CLR, DIS_PROT_STEP1_0_MASK);
	}

	INCREASE_STEPS;
}