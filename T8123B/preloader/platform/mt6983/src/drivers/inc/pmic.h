#ifndef _PMIC_H_
#define _PMIC_H_

#include <typedefs.h>
#include <upmu_hw.h>
#include <mt6363-regulator.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define OP_MODE_MU	(0)
#define OP_MODE_LP	(1)

#define HW_OFF		(0)
#define HW_ON		(0)
#define HW_LP		(1)

enum {
	RC0 = 0,
	RC1,
	RC2,
	RC3,
	RC4,
	RC5,
	RC6,
	RC7,
	RC8 = 0,
	RC9,
	RC10,
	RC11,
	RC12,
	RC13,
	HW0 = 0,
	HW1,
	HW2,
	HW3,
	HW4,
	HW5,
	HW6,
};


//==============================================================================
// PMIC define
//==============================================================================
extern int pmic_init_sdev(void);
extern int pmic_init(void);
extern void pmic_init_setting(void);
extern U16 pmic_read_efuse_nolock(int i);
extern void pmic_efuse_sw_load(void);

/*
 * PMIC Access APIs
 */
extern U32 pmic_read_interface(U32 RegNum, U8 *val, U8 MASK, U8 SHIFT);
extern U32 pmic_config_interface(U32 RegNum, U8 val, U8 MASK, U8 SHIFT);
extern U32 pmic_read_interface_nolock(U32 RegNum, U8 *val, U8 MASK, U8 SHIFT);
extern U32 pmic_config_interface_nolock(U32 RegNum, U8 val, U8 MASK, U8 SHIFT);
extern U8 upmu_get_reg_value(U32 reg);
extern U16 upmu_get_reg_value16(U32 reg);
extern U32 upmu_set_reg_value(U32 reg, U8 reg_val);
extern U32 upmu_set_reg_value16(U32 reg, U16 reg_val);

/*
 * PMIC Export APIs
 */
extern U32 is_pmic_rtc_alarm(void);
extern U32 is_pmic_spar(void);
extern U32 is_pmic_cold_reset(void);
extern U32 is_pmic_spt(void);
extern U32 get_pmic_boot_status(void);
extern U32 is_pmic_long_press_reset(void);
extern U32 is_pwrkey_short_press(void);
extern int pmic_detect_powerkey(void);
extern int pmic_detect_homekey(void);
extern int pmic_detect_homekey2(void);
extern U32 upmu_is_chr_det(void);
extern void PMIC_enable_long_press_reboot(void);
extern void pl_power_off(void);
extern void mt6363_dump_record_reg(void);

#endif // #ifndef _PMIC_H_

