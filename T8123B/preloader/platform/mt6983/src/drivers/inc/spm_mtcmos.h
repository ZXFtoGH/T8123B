#ifndef _SPM_MTCMOS_
#define _SPM_MTCMOS_

#define SPM_PROJECT_CODE      0xB16

#define STA_POWER_DOWN	      0
#define STA_POWER_ON          1

#define SET_BUS_PROTECT	      1
#define RELEASE_BUS_PROTECT   0

/*
 * 1. for non-CPU MTCMOS: VDEC, VENC, ISP, DISP, MFG, INFRA, DDRPHY, MDSYS1, MDSYS2
 * 2. call spm_mtcmos_noncpu_lock/unlock() before/after any operations
 */
 
int spm_mtcmos_ctrl_mfg0_shut_down(int state);
int spm_mtcmos_ctrl_mfg1(int state);
int spm_mtcmos_ctrl_mm_infra(int state);
int spm_mtcmos_ctrl_dis0(int state);
int spm_mtcmos_ctrl_dis1(int state);
int spm_mtcmos_ctrl_audio(int state);
int spm_mtcmos_ctrl_adsp_infra(int state);
int spm_mtcmos_ctrl_adsp_ao(int state);
#endif
