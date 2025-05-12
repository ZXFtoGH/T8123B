#include <platform.h>
#include <spm_mtcmos.h>
#include <spm_mtcmos_internal.h>
#include <timer.h>  //udelay
#include <pll.h>

#if CFG_FPGA_PLATFORM
#define IGNORE_MTCMOS_CHECK	1 //for FPGA
#endif

#define POWERON_CONFIG_EN	(SPM_BASE + 0x0000)
#define PWR_STATUS		(SPM_BASE + 0x0EF0)
#define PWR_STATUS_2ND		(SPM_BASE + 0x0EF4)
#define XPU_PWR_STATUS		(SPM_BASE + 0x0EF8)
#define XPU_PWR_STATUS_2ND	(SPM_BASE + 0x0EFC)


#define MFG0_PWR_CON		(SPM_BASE + 0x0E80)
#define MFG1_PWR_CON		(SPM_BASE + 0x0E84)
#define DIS0_PWR_CON		(SPM_BASE + 0x0E48)

/* Define MTCMOS power control */
#define PWR_RST_B		(0x1 << 0)
#define PWR_ISO			(0x1 << 1)
#define PWR_ON			(0x1 << 2)
#define PWR_ON_2ND		(0x1 << 3)
#define PWR_CLK_DIS		(0x1 << 4)
#define SRAM_CKISO		(0x1 << 5)
#define DORMANT_ENABLE		(0x1 << 6)
#define SRAM_ISOINT_B		(0x1 << 6)
#define VPROC_EXT_OFF		(0x1 << 7)
#define SLPB_CLAMP		(0x1 << 7)

/* Define MTCMOS Bus Protect Mask */
#define DIS0_PROT_STEP1_0_MASK           ((0x1 << 0) \
            |(0x1 << 2) \
            |(0x1 << 10) \
            |(0x1 << 12) \
            |(0x1 << 14) \
            |(0x1 << 16) \
            |(0x1 << 24) \
            |(0x1 << 26))
#define DIS0_PROT_STEP1_0_ACK_MASK       ((0x1 << 0) \
            |(0x1 << 2) \
            |(0x1 << 10) \
            |(0x1 << 12) \
            |(0x1 << 14) \
            |(0x1 << 16) \
            |(0x1 << 24) \
            |(0x1 << 26))
#define DIS0_PROT_STEP2_0_MASK           ((0x1 << 6))
#define DIS0_PROT_STEP2_0_ACK_MASK       ((0x1 << 6))
#define DIS0_PROT_STEP2_1_MASK           ((0x1 << 1) \
            |(0x1 << 3) \
            |(0x1 << 15) \
            |(0x1 << 17) \
            |(0x1 << 25) \
            |(0x1 << 27))
#define DIS0_PROT_STEP2_1_ACK_MASK       ((0x1 << 1) \
            |(0x1 << 3) \
            |(0x1 << 15) \
            |(0x1 << 17) \
            |(0x1 << 25) \
            |(0x1 << 27))

#define MFG1_PROT_STEP1_0_MASK           ((0x1 << 5) \
            |(0x1 << 6))
#define MFG1_PROT_STEP1_0_ACK_MASK       ((0x1 << 5) \
            |(0x1 << 6))
#define MFG1_PROT_STEP2_0_MASK           ((0x1 << 21) \
            |(0x1 << 22))
#define MFG1_PROT_STEP2_0_ACK_MASK       ((0x1 << 21) \
            |(0x1 << 22))

/* Define MTCMOS Power Status Mask */
#define MFG0_XPU_PWR_STA_MASK            (0x1 << 0)
#define MFG1_XPU_PWR_STA_MASK            (0x1 << 1)
#define DIS0_PWR_STA_MASK                (0x1 << 18)

/* Define Non-CPU SRAM Mask */
#define DIS0_SRAM_PDN                    (0x1 << 8)
#define DIS0_SRAM_PDN_ACK                (0x1 << 12)
#define MFG0_SRAM_PDN                    (0x1 << 8)
#define MFG0_SRAM_PDN_ACK                (0x1 << 12)
#define MFG1_SRAM_PDN                    (0x1 << 8)
#define MFG1_SRAM_PDN_ACK                (0x1 << 12)

/**************************************
 * for non-CPU MTCMOS
 **************************************/

int spm_mtcmos_ctrl_mfg0(int state)
{
	int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MFG0" */
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | MFG0_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MFG0_SRAM_PDN_ACK = 1" */
		while ((spm_read(MFG0_PWR_CON) & MFG0_SRAM_PDN_ACK) != MFG0_SRAM_PDN_ACK) {
				/* Need f_fmfg_core_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until XPU_PWR_STATUS = 0 and XPU_PWR_STATUS_2ND = 0" */
		while ((spm_read(XPU_PWR_STATUS) & MFG0_XPU_PWR_STA_MASK)
		       || (spm_read(XPU_PWR_STATUS_2ND) & MFG0_XPU_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Finish to turn off MFG0" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on MFG0" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until XPU_PWR_STATUS = 1 and XPU_PWR_STATUS_2ND = 1" */
		while (((spm_read(XPU_PWR_STATUS) & MFG0_XPU_PWR_STA_MASK)
				!= MFG0_XPU_PWR_STA_MASK)
				|| ((spm_read(XPU_PWR_STATUS_2ND) & MFG0_XPU_PWR_STA_MASK) != MFG0_XPU_PWR_STA_MASK)) {
			/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MFG0_PWR_CON, spm_read(MFG0_PWR_CON) & ~(MFG0_SRAM_PDN));
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MFG0_SRAM_PDN_ACK = 0" */
		while (spm_read(MFG0_PWR_CON) & MFG0_SRAM_PDN_ACK) {
				/* Need f_fmfg_core_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Finish to turn on MFG0" */
	}
	return err;
}

int spm_mtcmos_ctrl_mfg1(int state)
{
	int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MFG1" */
		/* TINFO="Set bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_2_SET, MFG1_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_STA1_2) & MFG1_PROT_STEP1_0_ACK_MASK) != MFG1_PROT_STEP1_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SET, MFG1_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_STA1) & MFG1_PROT_STEP2_0_ACK_MASK) != MFG1_PROT_STEP2_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | MFG1_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MFG1_SRAM_PDN_ACK = 1" */
		while ((spm_read(MFG1_PWR_CON) & MFG1_SRAM_PDN_ACK) != MFG1_SRAM_PDN_ACK) {
				/*  */
		}
#endif
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until XPU_PWR_STATUS = 0 and XPU_PWR_STATUS_2ND = 0" */
		while ((spm_read(XPU_PWR_STATUS) & MFG1_XPU_PWR_STA_MASK)
		       || (spm_read(XPU_PWR_STATUS_2ND) & MFG1_XPU_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Finish to turn off MFG1" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on MFG1" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until XPU_PWR_STATUS = 1 and XPU_PWR_STATUS_2ND = 1" */
		while (((spm_read(XPU_PWR_STATUS) & MFG1_XPU_PWR_STA_MASK)
				!= MFG1_XPU_PWR_STA_MASK)
				|| ((spm_read(XPU_PWR_STATUS_2ND) & MFG1_XPU_PWR_STA_MASK) != MFG1_XPU_PWR_STA_MASK)) {
			/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MFG1_PWR_CON, spm_read(MFG1_PWR_CON) & ~(MFG1_SRAM_PDN));
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MFG1_SRAM_PDN_ACK = 0" */
		while (spm_read(MFG1_PWR_CON) & MFG1_SRAM_PDN_ACK) {
				/*  */
		}
#endif
		/* TINFO="Release bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_CLR, MFG1_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_2_CLR, MFG1_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Finish to turn on MFG1" */
	}
	return err;
}

int spm_mtcmos_ctrl_disp(int state)
{
	int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off DIS0" */
		/* TINFO="Set bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_SET, DIS0_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_STA1) & DIS0_PROT_STEP1_0_ACK_MASK) != DIS0_PROT_STEP1_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SET, DIS0_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_STA1) & DIS0_PROT_STEP2_0_ACK_MASK) != DIS0_PROT_STEP2_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_SET, DIS0_PROT_STEP2_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_STA1) & DIS0_PROT_STEP2_1_ACK_MASK) != DIS0_PROT_STEP2_1_ACK_MASK) {
		}
#endif
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | DIS0_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until DIS0_SRAM_PDN_ACK = 1" */
		while ((spm_read(DIS0_PWR_CON) & DIS0_SRAM_PDN_ACK) != DIS0_SRAM_PDN_ACK) {
				/* Need hf_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & DIS0_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & DIS0_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Finish to turn off DIS0" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on DIS0" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (((spm_read(PWR_STATUS) & DIS0_PWR_STA_MASK)
				!= DIS0_PWR_STA_MASK)
				|| ((spm_read(PWR_STATUS_2ND) & DIS0_PWR_STA_MASK) != DIS0_PWR_STA_MASK)) {
			/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(DIS0_PWR_CON, spm_read(DIS0_PWR_CON) & ~(DIS0_SRAM_PDN));
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until DIS0_SRAM_PDN_ACK = 0" */
		while (spm_read(DIS0_PWR_CON) & DIS0_SRAM_PDN_ACK) {
				/* Need hf_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Release bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_CLR, DIS0_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step2 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_CLR, DIS0_PROT_STEP2_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_CLR, DIS0_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Finish to turn on DIS0" */
	}
	return err;
}

