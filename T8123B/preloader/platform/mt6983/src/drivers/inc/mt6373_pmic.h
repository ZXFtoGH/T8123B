#ifndef _MT6373_PMIC_H_
#define _MT6373_PMIC_H_

#include <pal_log.h>
#include <platform.h>
#include <typedefs.h>

#include <mt6373_upmu_hw.h>
#include <pmic.h>
#include <pmic_auxadc.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_errno.h>

//==============================================================================
// Second PMIC define
//==============================================================================
extern int second_pmic_init_sdev(void);
extern int second_pmic_init(void);
extern void second_pmic_init_setting(void);
extern U16 second_pmic_read_efuse_nolock(int i);
extern void second_pmic_efuse_sw_load(void);
/* Second PMIC Regulator define */
extern int mt6373_regulator_probe(void);
extern void mt6373_wdtdbg_vosel(void);

/*
 * Second PMIC Access APIs
 */
extern U32 second_pmic_read_interface(U32 RegNum, U8 *val, U8 MASK, U8 SHIFT);
extern U32 second_pmic_config_interface(U32 RegNum, U8 val, U8 MASK, U8 SHIFT);
extern U32 second_pmic_read_interface_nolock(U32 RegNum, U8 *val, U8 MASK, U8 SHIFT);
extern U32 second_pmic_config_interface_nolock(U32 RegNum, U8 val, U8 MASK, U8 SHIFT);
extern U8 second_upmu_get_reg_value(U32 reg);
extern U16 second_upmu_get_reg_value16(U32 reg);
extern U32 second_upmu_set_reg_value(U32 reg, U8 reg_val);
extern U32 second_upmu_set_reg_value16(U32 reg, U16 reg_val);

/*
 * Second PMIC Auxadc define
 */
extern void mt6373_auxadc_init(void);
extern void mt6373_sdmadc_init(void);

/*
 * Second PMIC Export APIs
 */
extern U32 second_pmic_get_chip_version(void);
extern int second_pmic_get_dram_type(void);
extern void second_pmic_seq_off(int en);
extern void mt6373_dump_record_reg(void);
extern bool is_second_pmic_pp_swap(void);

/* wk_buck_autok */
extern void wk_second_pmic_buck_autok(bool is_trimh_2);

#endif // #ifndef _MT6373_PMIC_H_

