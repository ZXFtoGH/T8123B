#ifndef _SPM_MTCMOS_
#define _SPM_MTCMOS_

#define STA_POWER_DOWN  0
#define STA_POWER_ON    1

/*
 * 1. for non-CPU MTCMOS: VDEC, VENC, ISP, DISP, MFG, INFRA, DDRPHY, MDSYS1, MDSYS2
 * 2. call spm_mtcmos_noncpu_lock/unlock() before/after any operations
 */
int spm_mtcmos_ctrl_mfg0(int state);
int spm_mtcmos_ctrl_mfg1(int state);
int spm_mtcmos_ctrl_disp(int state);
#endif
