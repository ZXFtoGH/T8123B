#ifndef __LINUX_REGULATOR_MT6359_H_
#define __LINUX_REGULATOR_MT6359_H_

#include <typedefs.h>
#include "pal_log.h"

#define PMIC_PRELOADER 1
#define LDO_SUPPORT
#define LDO_VOTRIM_SUPPORT
#define MT6359_DEBUG 1

#if MT6359_DEBUG
#define PMUTAG                "[MT6359] "
#define mreg_dbg_print(fmt, arg...) pal_log_info(PMUTAG fmt, ##arg)
#else
#define mreg_dbg_print(...) do { } while(0)
#endif

enum {
	MTK_REGULATOR_VSRAM_OTHERS,
	MTK_REGULATOR_VSRAM_PROC1,
	MTK_REGULATOR_VSRAM_PROC2,
	MTK_REGULATOR_VSRAM_MD,
	MTK_REGULATOR_VPROC1,
	MTK_REGULATOR_VPROC2,
	MTK_REGULATOR_VGPU11,
#ifdef LDO_SUPPORT
	MTK_REGULATOR_VIO18,
	MTK_REGULATOR_LDO_SUPPORT = MTK_REGULATOR_VIO18,
#endif /*--LDO_SUPPORT--*/
#ifdef MT6360_PMIC
	MTK_REGULATOR_SUBPMIC_BUCK1,
	MTK_REGULATOR_SUBPMIC_BUCK2,
	MTK_REGULATOR_SUBPMIC_LDO6,
	MTK_REGULATOR_SUBPMIC_LDO7,
#endif
	MTK_REGULATOR_MT6315_6_VBUCK1,
	MTK_REGULATOR_MT6315_6_VBUCK3,
	MTK_REGULATOR_MT6315_7_VBUCK1,
	MTK_REGULATOR_MT6315_7_VBUCK3,
	MTK_REGULATOR_MT6315_3_VBUCK1,
	MTK_REGULATOR_MT6315_3_VBUCK3,
	MTK_REGULATOR_MT6315_3_VBUCK4,
	MTK_REGULATOR_VMDDR,
	MTK_REGULATOR_VUFS12,
	MTK_REGULATOR_MAX_NR,
};

typedef enum {
	DISABLE_REGULATOR,
	ENABLE_REGULATOR,
} enset;

typedef enum {
	AUTOMODE,
	PWMMODE,
} modeset;

#ifdef LDO_SUPPORT
typedef enum {
	VOLTOSEL,
	SELTOVOL,
} volseltran;
#endif /*--LDO_SUPPORT--*/

#ifdef LDO_VOTRIM_SUPPORT
typedef enum {
	TRIMTOSEL,
	SELTOTRIM,
} trimseltran;
#endif /*--LDO_VOTRIM_SUPPORT--*/

typedef enum {
	NON_REGULAR_VOLTAGE,
	FIXED_REGULAR_VOLTAGE,
	REGULAR_VOLTAGE,
} regulator_type;

#ifdef LDO_VOTRIM_SUPPORT
struct mt6359_ldo_trim_info {
	unsigned short trim_reg;
	unsigned short trim_mask;
	unsigned short trim_shift;
	const void *trim_voltages;
	unsigned int trim_size;
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static unsigned int mt6359_ldo_convert_data(unsigned char id,
	unsigned int cnvdata, volseltran transtype);

struct mt6359_ldo_info {
	const void *pvoltages;
	const void *idxs;
	unsigned int n_size;
};
#endif /*--LDO_SUPPORT--*/

/*--abstrac the same parameter--*/
struct mt6359_regulator_info {
	unsigned int min_uV;
	unsigned int max_uV;
	unsigned short vol_reg;
	unsigned short vol_mask;
	unsigned short vol_shift;
	unsigned short da_vol_reg;
	unsigned short da_vol_mask;
	unsigned short da_vol_shift;
	unsigned short enable_reg;
	unsigned short enable_shift;
	unsigned short mode_reg;
	unsigned short mode_shift;
#ifdef LDO_SUPPORT
	struct mt6359_ldo_info *extinfo;
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
	struct mt6359_ldo_trim_info *triminfo;
#endif /*--LDO_VOTRIM_SUPPORT--*/
	unsigned short step_uV;
	regulator_type rtype;
};

extern int mt6359_tracking_set(const char *name, bool en,
			       int offset, unsigned int lb, unsigned int hb);
extern void mt6359_wdtdbg_vosel(void);
extern int mt6359_probe(void);

#ifdef LDO_VOTRIM_SUPPORT
#define mt6359_ldo_trim_decl(_name, trim_array)\
{ \
	.trim_reg = _name##_trim_reg, \
	.trim_mask = _name##_trim_mask, \
	.trim_shift = _name##_trim_shift, \
	.trim_voltages = (void *)(trim_array), \
	.trim_size = ARRAY_SIZE(trim_array),	\
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
#define mt6359_ldo_decl(volt_array, idx_array)\
{ \
	.pvoltages = (void *)(volt_array), \
	.idxs = (void *)(idx_array), \
	.n_size = ARRAY_SIZE(volt_array),	\
}
#endif /*--LDO_SUPPORT--*/

#define mt6359_decl(_name)\
{ \
	.min_uV = _name##_min_uV, \
	.max_uV = _name##_max_uV, \
	.vol_reg = _name##_vol_reg, \
	.vol_mask = _name##_vol_mask, \
	.vol_shift = _name##_vol_shift, \
	.da_vol_reg = _name##_da_vol_reg, \
	.da_vol_mask = _name##_da_vol_mask, \
	.da_vol_shift = _name##_da_vol_shift, \
	.enable_reg = _name##_enable_reg, \
	.enable_shift = _name##_enable_shift, \
	.mode_reg = _name##_mode_reg, \
	.mode_shift = _name##_mode_shift, \
	.step_uV = _name##_step_uV, \
	.rtype = _name##_volt_type, \
}

/* -------Code Gen Start-------*/
#define vsram_others_vol_reg            PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_ADDR
#define vsram_others_vol_mask           PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_MASK
#define vsram_others_vol_shift          PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_SHIFT
#define vsram_others_da_vol_reg         PMIC_DA_VSRAM_OTHERS_VOSEL_ADDR
#define vsram_others_da_vol_mask        PMIC_DA_VSRAM_OTHERS_VOSEL_MASK
#define vsram_others_da_vol_shift       PMIC_DA_VSRAM_OTHERS_VOSEL_SHIFT
#define vsram_others_enable_reg         PMIC_RG_LDO_VSRAM_OTHERS_EN_ADDR
#define vsram_others_enable_shift       PMIC_RG_LDO_VSRAM_OTHERS_EN_SHIFT
#define vsram_others_mode_reg           0
#define vsram_others_mode_shift         0
#define vsram_others_trim_reg           0
#define vsram_others_trim_mask          0
#define vsram_others_trim_shift         0
#define vsram_others_min_uV             500000
#define vsram_others_max_uV             1293750
#define vsram_others_step_uV            6250
#define vsram_others_volt_type          REGULAR_VOLTAGE
#define vsram_others_stb                240


#define vsram_proc1_vol_reg             PMIC_RG_LDO_VSRAM_PROC1_VOSEL_ADDR
#define vsram_proc1_vol_mask            PMIC_RG_LDO_VSRAM_PROC1_VOSEL_MASK
#define vsram_proc1_vol_shift           PMIC_RG_LDO_VSRAM_PROC1_VOSEL_SHIFT
#define vsram_proc1_da_vol_reg          PMIC_DA_VSRAM_PROC1_VOSEL_ADDR
#define vsram_proc1_da_vol_mask         PMIC_DA_VSRAM_PROC1_VOSEL_MASK
#define vsram_proc1_da_vol_shift        PMIC_DA_VSRAM_PROC1_VOSEL_SHIFT
#define vsram_proc1_enable_reg          PMIC_RG_LDO_VSRAM_PROC1_EN_ADDR
#define vsram_proc1_enable_shift        PMIC_RG_LDO_VSRAM_PROC1_EN_SHIFT
#define vsram_proc1_mode_reg            0
#define vsram_proc1_mode_shift          0
#define vsram_proc1_trim_reg            0
#define vsram_proc1_trim_mask           0
#define vsram_proc1_trim_shift          0
#define vsram_proc1_min_uV              500000
#define vsram_proc1_max_uV              1293750
#define vsram_proc1_step_uV             6250
#define vsram_proc1_volt_type           REGULAR_VOLTAGE
#define vsram_proc1_stb                 240


#define vsram_proc2_vol_reg             PMIC_RG_LDO_VSRAM_PROC2_VOSEL_ADDR
#define vsram_proc2_vol_mask            PMIC_RG_LDO_VSRAM_PROC2_VOSEL_MASK
#define vsram_proc2_vol_shift           PMIC_RG_LDO_VSRAM_PROC2_VOSEL_SHIFT
#define vsram_proc2_da_vol_reg          PMIC_DA_VSRAM_PROC2_VOSEL_ADDR
#define vsram_proc2_da_vol_mask         PMIC_DA_VSRAM_PROC2_VOSEL_MASK
#define vsram_proc2_da_vol_shift        PMIC_DA_VSRAM_PROC2_VOSEL_SHIFT
#define vsram_proc2_enable_reg          PMIC_RG_LDO_VSRAM_PROC2_EN_ADDR
#define vsram_proc2_enable_shift        PMIC_RG_LDO_VSRAM_PROC2_EN_SHIFT
#define vsram_proc2_mode_reg            0
#define vsram_proc2_mode_shift          0
#define vsram_proc2_trim_reg            0
#define vsram_proc2_trim_mask           0
#define vsram_proc2_trim_shift          0
#define vsram_proc2_min_uV              500000
#define vsram_proc2_max_uV              1293750
#define vsram_proc2_step_uV             6250
#define vsram_proc2_volt_type           REGULAR_VOLTAGE
#define vsram_proc2_stb                 240


#define vsram_md_vol_reg                PMIC_RG_LDO_VSRAM_MD_VOSEL_ADDR
#define vsram_md_vol_mask               PMIC_RG_LDO_VSRAM_MD_VOSEL_MASK
#define vsram_md_vol_shift              PMIC_RG_LDO_VSRAM_MD_VOSEL_SHIFT
#define vsram_md_da_vol_reg             PMIC_DA_VSRAM_MD_VOSEL_ADDR
#define vsram_md_da_vol_mask            PMIC_DA_VSRAM_MD_VOSEL_MASK
#define vsram_md_da_vol_shift           PMIC_DA_VSRAM_MD_VOSEL_SHIFT
#define vsram_md_enable_reg             PMIC_RG_LDO_VSRAM_MD_EN_ADDR
#define vsram_md_enable_shift           PMIC_RG_LDO_VSRAM_MD_EN_SHIFT
#define vsram_md_mode_reg               0
#define vsram_md_mode_shift             0
#define vsram_md_trim_reg               0
#define vsram_md_trim_mask              0
#define vsram_md_trim_shift             0
#define vsram_md_min_uV                 500000
#define vsram_md_max_uV                 1293750
#define vsram_md_step_uV                6250
#define vsram_md_volt_type              REGULAR_VOLTAGE
#define vsram_md_stb                    240


#define vproc1_vol_reg                  PMIC_RG_BUCK_VPROC1_VOSEL_ADDR
#define vproc1_vol_mask                 PMIC_RG_BUCK_VPROC1_VOSEL_MASK
#define vproc1_vol_shift                PMIC_RG_BUCK_VPROC1_VOSEL_SHIFT
#define vproc1_da_vol_reg               PMIC_DA_VPROC1_VOSEL_ADDR
#define vproc1_da_vol_mask              PMIC_DA_VPROC1_VOSEL_MASK
#define vproc1_da_vol_shift             PMIC_DA_VPROC1_VOSEL_SHIFT
#define vproc1_enable_reg               PMIC_RG_BUCK_VPROC1_EN_ADDR
#define vproc1_enable_shift             PMIC_RG_BUCK_VPROC1_EN_SHIFT
#define vproc1_mode_reg                 PMIC_RG_VPROC1_FCCM_ADDR
#define vproc1_mode_shift               PMIC_RG_VPROC1_FCCM_SHIFT
#define vproc1_trim_reg                 0
#define vproc1_trim_mask                0
#define vproc1_trim_shift               0
#define vproc1_min_uV                   400000
#define vproc1_max_uV                   1193750
#define vproc1_step_uV                  6250
#define vproc1_volt_type                REGULAR_VOLTAGE
#define vproc1_stb                      200


#define vproc2_vol_reg                  PMIC_RG_BUCK_VPROC2_VOSEL_ADDR
#define vproc2_vol_mask                 PMIC_RG_BUCK_VPROC2_VOSEL_MASK
#define vproc2_vol_shift                PMIC_RG_BUCK_VPROC2_VOSEL_SHIFT
#define vproc2_da_vol_reg               PMIC_DA_VPROC2_VOSEL_ADDR
#define vproc2_da_vol_mask              PMIC_DA_VPROC2_VOSEL_MASK
#define vproc2_da_vol_shift             PMIC_DA_VPROC2_VOSEL_SHIFT
#define vproc2_enable_reg               PMIC_RG_BUCK_VPROC2_EN_ADDR
#define vproc2_enable_shift             PMIC_RG_BUCK_VPROC2_EN_SHIFT
#define vproc2_mode_reg                 PMIC_RG_VPROC2_FCCM_ADDR
#define vproc2_mode_shift               PMIC_RG_VPROC2_FCCM_SHIFT
#define vproc2_trim_reg                 0
#define vproc2_trim_mask                0
#define vproc2_trim_shift               0
#define vproc2_min_uV                   400000
#define vproc2_max_uV                   1193750
#define vproc2_step_uV                  6250
#define vproc2_volt_type                REGULAR_VOLTAGE
#define vproc2_stb                      200


#define vgpu11_vol_reg                  PMIC_RG_BUCK_VGPU11_VOSEL_ADDR
#define vgpu11_vol_mask                 PMIC_RG_BUCK_VGPU11_VOSEL_MASK
#define vgpu11_vol_shift                PMIC_RG_BUCK_VGPU11_VOSEL_SHIFT
#define vgpu11_da_vol_reg               PMIC_DA_VGPU11_VOSEL_ADDR
#define vgpu11_da_vol_mask              PMIC_DA_VGPU11_VOSEL_MASK
#define vgpu11_da_vol_shift             PMIC_DA_VGPU11_VOSEL_SHIFT
#define vgpu11_enable_reg               PMIC_RG_BUCK_VGPU11_EN_ADDR
#define vgpu11_enable_shift             PMIC_RG_BUCK_VGPU11_EN_SHIFT
#define vgpu11_mode_reg                 PMIC_RG_VGPU11_FCCM_ADDR
#define vgpu11_mode_shift               PMIC_RG_VGPU11_FCCM_SHIFT
#define vgpu11_trim_reg                 0
#define vgpu11_trim_mask                0
#define vgpu11_trim_shift               0
#define vgpu11_min_uV                   400000
#define vgpu11_max_uV                   1193750
#define vgpu11_step_uV                  6250
#define vgpu11_volt_type                REGULAR_VOLTAGE
#define vgpu11_stb                      200


#define vio18_vol_reg                    PMIC_RG_VIO18_VOSEL_ADDR
#define vio18_vol_mask                   PMIC_RG_VIO18_VOSEL_MASK
#define vio18_vol_shift                  PMIC_RG_VIO18_VOSEL_SHIFT
#define vio18_da_vol_reg                 0
#define vio18_da_vol_mask                0
#define vio18_da_vol_shift               0
#define vio18_enable_reg                 PMIC_RG_LDO_VIO18_EN_ADDR
#define vio18_enable_shift               PMIC_RG_LDO_VIO18_EN_SHIFT
#define vio18_mode_reg                   0
#define vio18_mode_shift                 0
#define vio18_trim_reg                   PMIC_RG_VIO18_VOTRIM_ADDR
#define vio18_trim_mask                  PMIC_RG_VIO18_VOTRIM_MASK
#define vio18_trim_shift                 PMIC_RG_VIO18_VOTRIM_SHIFT
#define vio18_min_uV                     1700000
#define vio18_max_uV                     1900000
#define vio18_step_uV                    0
#define vio18_volt_type                  NON_REGULAR_VOLTAGE
#define vio18_stb                        960

#endif /* __LINUX_REGULATOR_MT6359_H_ */
