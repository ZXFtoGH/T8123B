//=============================================================================
//  Include Files
//=============================================================================
//#include <common.h>
//#include <ett_common.h>
//#include <test_case_controller.h>
//#include <api.h>
//#include "gpio.h"
//#include "ett_cust.h"
//#include "emi_setting.h"
//#include "pll.h"
//#include "dramc_pi_api.h"
#include "dramc_common.h"
#include "dramc_int_global.h"


#if !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
#include "gpio.h"
#define GPIO_117  (0x80000000 | 117)
#include "dram_buffer.h"
#endif
#else
#include <barriers.h>
#endif

#if !__FLASH_TOOL_DA__ && !__ETT__
#include "string.h"
#if (FOR_DV_SIMULATION_USED==0)
   #include "platform.h"
#if defined(DEF_LAST_DRAMC)
   #include "plat_dbg_info.h"
#endif
#endif
#endif

#if !__FLASH_TOOL_DA__ && !__ETT__
#include "custom_emi.h"   // fix build error: emi_settings
#endif


#include <emi_hw.h>
#include <emi.h>
#include "voltage.h"


#define bootarg g_dram_buf->bootarg

#if (FOR_DV_SIMULATION_USED==0)
#include <pmic.h>
/* now we can use definition MTK_PMIC_MT6359
 * ==============================================================
 * PMIC   |Power |Dflt. Volt. |Step   |Support FPWM |Cmt.
 * --------------------------------------------------------------
 * MT6359 |Vcore |0.8v        |6.25mV |Yes          |
 *        |Vm18  |1.8v        |0.1V   |No           |
 * --------------------------------------------------------------
 * MT6362 |Vdram |1.125v      |5mV    |Yes          |(DRAM Vdram)
 *        |Vmddr |0.75v       |10mV   |No           |(AP Vdram)
 *        |Vddq  |0.6v        |10mV   |No           |
 * ==============================================================
 */
#define MTK_PMIC_MT6359
#endif

#if !__ETT__
#define mt_reg_sync_write(x,y) mt_reg_sync_writel(y,x)
#endif

#ifdef MTK_PMIC_MT6359
#include <regulator/mtk_regulator.h>
#include <mt6359.h>
#endif

#if !__ETT__
#define CQ_DMA_BASE (0x10212000)
#endif

#if !__ETT__
#include "sec_devinfo.h"
#endif

#if !__ETT__
#define DRAMC_ASSERT(expr)                                            \
    do {                                                        \
        if (!(expr)) {                                          \
      plat_clean_invalidate_dcache();														\
	    ASSERT(expr);					\
	}							\
    } while (0)
#else
#define DRAMC_ASSERT(expr)         ASSERT(expr)
#endif

//=============================================================================
//  Definition
//=============================================================================

int doe_get_config(char* feature);

//=============================================================================
//  Global Variables
//=============================================================================
static unsigned char auto_detect_done;
static int enable_combo_dis = 0;
static unsigned short mr5;
static unsigned short mr6;
static unsigned short mr7;
static unsigned short mr8;
extern DRAMC_CTX_T *psCurrDramCtx;
extern EMI_SETTINGS g_default_emi_setting;
#ifdef MTK_PMIC_MT6359
static struct mtk_regulator reg_vio18, reg_vdram, reg_vcore, reg_vddq, reg_vmddr;
#endif

#ifdef LAST_DRAMC
static LAST_DRAMC_INFO_T* last_dramc_info_ptr;
#endif

#ifdef VOLTAGE_SEL
static VOLTAGE_SEL_INFO_T voltage_sel_info_ptr;
#endif

#if ( !__ETT__ && (FOR_DV_SIMULATION_USED==0))
#define DRAM_3733_OFF 0
#define DRAM_3733_ON 1
#define DRAM_3733_DEFAULT 2

static unsigned int gpio_3733_detected = DRAM_3733_DEFAULT;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
#if (!__ETT__)
#include <storage_api.h>
#endif
extern u64 get_part_addr(const char *name);
static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo);
static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo);
static u64 part_dram_data_addr = 0;
static unsigned int dram_offline_data_flags = 0;
#endif

#if defined(SLT) && (!__ETT__)
#include <pl_version.h>
static u64 part_dram_data_addr_slt = 0;
int read_slt_data(DRAM_SLT_DATA_T *data);
int write_slt_data(DRAM_SLT_DATA_T *data);
int clean_slt_data(void);
#endif
//=============================================================================
//  External references
//=============================================================================
extern DRAMC_CTX_T *psCurrDramCtx;
extern char* opt_dle_value;

void print_DBG_info(DRAMC_CTX_T *p);
unsigned int mt_get_dram_type_from_hw_trap(void);

#if (!__FLASH_TOOL_DA__ && !__ETT__)
static PN_NEED_TMRS_T pn_list_need_tmrs[] = {
	"KM8F9001MM-B830",
	"KM8F9001JM-B813",
	"KM8L9001JM-B624",
	"KM8V9001JM-B813",
	"KM2L9001CM-B518",
	"KM5L9001DM-B424",
	"KM2P9001CM-B518",
	"KM5P9001DM-B424",
	"KM3V6001CB-B708",
	"KMDL6001DM-B425",
	"KMDV6001DB-B625",
	"KMDP6001DB-B425",
	"KM4X60002M-B321",
	"KM4D6001KM-B217",
};
#endif

#if ENABLE_PINMUX_FOR_RANK_SWAP
static void EMI_rank_swap_emi_setting(EMI_SETTINGS *emi_set)
{
	static unsigned int temp;

	if (emi_set->EMI_CONA_VAL & 0x20000) {
		temp = emi_set->EMI_CONA_VAL;
		emi_set->EMI_CONA_VAL &= ~(0xF3F0F0F0);
		emi_set->EMI_CONA_VAL |= (temp & 0xC0C0C0C0) >> 2;
		emi_set->EMI_CONA_VAL |= (temp & 0x30303030) << 2;
		emi_set->EMI_CONA_VAL |= (temp & 0x02000000) >> 1;
		emi_set->EMI_CONA_VAL |= (temp & 0x01000000) << 1;

		temp = emi_set->EMI_CONH_VAL;
		emi_set->EMI_CONH_VAL &= ~(0xFFFF0030);
		emi_set->EMI_CONH_VAL |= (temp & 0xF0F00000) >> 4;
		emi_set->EMI_CONH_VAL |= (temp & 0x0F0F0000) << 4;
		emi_set->EMI_CONH_VAL |= (temp & 0x00000020) >> 1;
		emi_set->EMI_CONH_VAL |= (temp & 0x00000010) << 1;

		temp = emi_set->CHN0_EMI_CONA_VAL;
		emi_set->CHN0_EMI_CONA_VAL &= ~(0x00FFF0FC);
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00F00000) >> 4;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x000F0000) << 4;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x0000C0C0) >> 2;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00003030) << 2;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00000008) >> 1;
		emi_set->CHN0_EMI_CONA_VAL |= (temp & 0x00000004) << 1;

		temp = emi_set->CHN1_EMI_CONA_VAL;
		emi_set->CHN1_EMI_CONA_VAL &= ~(0x00FFF0FC);
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00F00000) >> 4;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x000F0000) << 4;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x0000C0C0) >> 2;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00003030) << 2;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00000008) >> 1;
		emi_set->CHN1_EMI_CONA_VAL |= (temp & 0x00000004) << 1;

		emi_set->CHN0_EMI_CONA_VAL |= 0x80000000;
		emi_set->CHN1_EMI_CONA_VAL |= 0x80000000;

		temp = emi_set->DRAM_RANK_SIZE[0];
		emi_set->DRAM_RANK_SIZE[0] = emi_set->DRAM_RANK_SIZE[1];
		emi_set->DRAM_RANK_SIZE[1] = temp;

		if (emi_set->dram_cbt_mode_extern == CBT_R0_NORMAL_R1_BYTE)
			emi_set->dram_cbt_mode_extern = CBT_R0_BYTE_R1_NORMAL;
		else if (emi_set->dram_cbt_mode_extern == CBT_R0_BYTE_R1_NORMAL)
			emi_set->dram_cbt_mode_extern = CBT_R0_NORMAL_R1_BYTE;
	}
}

void EMI_rank_swap_handle(void)
{
	static unsigned int handled = 0;
	int i;

	if (!handled) {
		EMI_rank_swap_emi_setting(&g_default_emi_setting);
		handled = 1;
	}
}
#endif

void mdl_setting(DRAMC_CTX_T *p)
{
	EMI_SETTINGS *emi_set = &g_default_emi_setting;
	
	emi_init();

	//The following is MDL settings
	set_cen_emi_cona(emi_set->EMI_CONA_VAL);
	set_cen_emi_conf(emi_set->EMI_CONF_VAL);
	set_cen_emi_conh(emi_set->EMI_CONH_VAL);

	// CHNA and CHNB uses the same CH0 setting
	set_chn_emi_cona(emi_set->CHN0_EMI_CONA_VAL);
	//set_chn_emi_conc(0x4);

	p->vendor_id = emi_set->iLPDDR3_MODE_REG_5;
}

unsigned int check_gating_error(void)
{
	unsigned int ret = 0, i, phy_base, err_code = 0;
	unsigned int misc_stberr_all, misc_stberr_rk0_r, misc_stberr_rk0_f, misc_stberr_rk1_r, misc_stberr_rk1_f;

	phy_base = Channel_A_DDRPHY_AO_BASE_ADDRESS;

	for (i = 0; i < CHANNEL_NUM; ++i, phy_base += 0x10000, err_code = 0) {
		misc_stberr_all   = *(volatile unsigned int*)(phy_base + 0x1500);
		misc_stberr_rk0_r = *(volatile unsigned int*)(phy_base + 0x1504);
		misc_stberr_rk0_f = *(volatile unsigned int*)(phy_base + 0x1508);
		misc_stberr_rk1_r = *(volatile unsigned int*)(phy_base + 0x150c);
		misc_stberr_rk1_f = *(volatile unsigned int*)(phy_base + 0x1510);

		if (misc_stberr_all & (1 << 16)) {
			ret |= (1 << i);
#ifdef LAST_DRAMC
			if ((misc_stberr_rk0_r & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK0_R;
			}
			if ((misc_stberr_rk0_f & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK0_F;
			}
			if ((misc_stberr_rk1_r & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK1_R;
			}
			if ((misc_stberr_rk1_f & 0xffff) != 0) {
				err_code |= ERR_DRAM_GATING_RK1_F;
			}
			dram_fatal_set_gating_err(i, err_code);
			dram_fatal_set_stberr(i, 0, (misc_stberr_rk0_r & 0xffff) | ((misc_stberr_rk0_f & 0xffff) << 16));
			dram_fatal_set_stberr(i, 1, (misc_stberr_rk1_r & 0xffff) | ((misc_stberr_rk1_f & 0xffff) << 16));
		} else {
			dram_fatal_set_gating_err(i, 0);
			dram_fatal_set_stberr(i, 0, 0);
			dram_fatal_set_stberr(i, 1, 0);
#endif
		}
	}

	return ret;
}

void print_DBG_info(DRAMC_CTX_T *p)
{
#ifndef OLYMPUS_TO_BE_PORTING
    unsigned int addr = 0x0;
    U32 u4value;

#ifdef DDR_INIT_TIME_PROFILING
    return;
#endif

    mcSHOW_DBG_MSG(("EMI_CONA=%x\n", get_cen_emi_cona()));
    mcSHOW_DBG_MSG(("EMI_CONH=%x\n", get_cen_emi_conh()));

    //RISCReadAll();
#endif
}

int mt_get_dram_type(void)
{
	unsigned int dtype = mt_get_dram_type_from_hw_trap();

	if (dtype == TYPE_LPDDR4X)
		return DTYPE_LPDDR4X;
	else
		DRAMC_ASSERT(0);

	return 0;
}

int mt_get_freq_setting(DRAMC_CTX_T *p)
{
    return p->frequency;
}

#ifdef DDR_RESERVE_MODE
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;
#define TIMEOUT 3
extern void before_Dramc_DDR_Reserved_Mode_setting(void);

#define	CHAN_DRAMC_NAO_MISC_STATUSA(base)	(base + 0x80)
#define SREF_STATE				(1 << 16)

static unsigned int is_dramc_exit_slf(void)
{
	unsigned int ret;

	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_A_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-A is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(Channel_B_DRAMC_NAO_BASE_ADDRESS);
	if ((ret & SREF_STATE) != 0) {
		dramc_crit("DRAM CHAN-B is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

	dramc_crit("ALL DRAM CHAN is not in self-refresh\n");
	return 1;
}

#endif

unsigned int dramc_set_vcore_voltage(unsigned int vcore)
{
#ifdef MTK_PMIC_MT6359
	return mtk_regulator_set_voltage(&reg_vcore, vcore, MAX_VCORE);
#endif
	return 0;
}

unsigned int dramc_get_vcore_voltage(void)
{
#ifdef MTK_PMIC_MT6359
	return mtk_regulator_get_voltage(&reg_vcore);
#else
	return 0;
#endif
}

unsigned int dramc_set_vdram_voltage(unsigned int ddr_type, unsigned int vdram)
{
#ifdef MTK_PMIC_MT6359
	mtk_regulator_set_voltage(&reg_vdram, vdram, MAX_VDRAM);
#endif
	return 0;
}

unsigned int dramc_get_vdram_voltage(unsigned int ddr_type)
{
#ifdef MTK_PMIC_MT6359
	return mtk_regulator_get_voltage(&reg_vdram);
#else
	return 0;
#endif
}

unsigned int dramc_set_vddq_voltage(unsigned int ddr_type, unsigned int vddq)
{
#ifdef MTK_PMIC_MT6359
	mtk_regulator_set_voltage(&reg_vddq, vddq, MAX_VDDQ);
#endif
	return 0;
}

unsigned int dramc_get_vddq_voltage(unsigned int ddr_type)
{
#ifdef MTK_PMIC_MT6359
	return mtk_regulator_get_voltage(&reg_vddq);
#else
	return 0;
#endif
}

unsigned int dramc_set_vmddr_voltage(unsigned int vmddr)
{
#ifdef MTK_PMIC_MT6359
	return mtk_regulator_set_voltage(&reg_vmddr, vmddr, MAX_VMDDR);
#endif
	return 0;
}

unsigned int dramc_get_vmddr_voltage(void)
{
#ifdef MTK_PMIC_MT6359
	return mtk_regulator_get_voltage(&reg_vmddr);
#else
	return 0;
#endif
}

unsigned int dramc_set_vio18_voltage(unsigned int vio18)
{
#ifdef MTK_PMIC_MT6359
	unsigned int twist = vio18 % UNIT_VIO18_STEP / UNIT_VIO18;
	vio18 -= vio18 % UNIT_VIO18_STEP;
	pmic_config_interface(PMIC_RG_VM18_VOCAL_ADDR, twist, PMIC_RG_VM18_VOCAL_MASK, PMIC_RG_VM18_VOCAL_SHIFT);
	return mtk_regulator_set_voltage(&reg_vio18, vio18, MAX_VIO18);
#else
	return 0;
#endif
}


unsigned int dramc_get_vio18_voltage(void)
{
#ifdef MTK_PMIC_MT6359
	unsigned int twist = 0;
	pmic_read_interface(PMIC_RG_VM18_VOCAL_ADDR, &twist, PMIC_RG_VM18_VOCAL_MASK, PMIC_RG_VM18_VOCAL_SHIFT);
	return mtk_regulator_get_voltage(&reg_vio18) + twist * UNIT_VIO18;
#else
	return 0;
#endif
}

#define GPIO_TRAPPING_REG (0x100056f0)
unsigned int is_discrete_lpddr4(void)
{
	unsigned int value = 0;

	/*  bit 19 & bit 26 latch GPIO156,  GPIO155 will latch 0 value ;mcp lp4x:0x0040 8100, dis lp4x :0x0448 8100*/
	value = ((*((volatile unsigned int *)GPIO_TRAPPING_REG) >> 19) & 0x1) & ((*((volatile unsigned int *)GPIO_TRAPPING_REG) >> 26) & 0x1);
	dramc_debug("%s : %d \n", __func__, value);
	return value;
}

unsigned int get_dram_frequency_from_hw_trap(void)
{
#if ( !__ETT__ && (FOR_DV_SIMULATION_USED==0))
	
	if(gpio_3733_detected == DRAM_3733_DEFAULT){
	
		mt_set_gpio_mode(GPIO_117,GPIO_MODE_GPIO);
		mt_set_gpio_dir(GPIO_117,GPIO_DIR_IN);

		if (mt_get_gpio_in(GPIO_117) == DRAM_3733_ON){
		        /*dram should use 3733Mhz */
				dramc_crit("gpio 117 is high , set 3733 \n");
				gpio_3733_detected = DRAM_3733_ON;
				g_default_emi_setting.highest_freq = 3733;
			}else{
                /*dram should use 4266Mhz */
				dramc_crit("gpio 117 is low ,set 4266\n");
				gpio_3733_detected = DRAM_3733_OFF;
				g_default_emi_setting.highest_freq = 4266;
			}		
	}

#if MTK_LP4X_HIGHSET_DDR4266
    g_default_emi_setting.highest_freq = 4266;
    dramc_crit("MTK_LP4X_HIGHSET_DDR4266 mandatorily set 4266\n");
#endif
#if MTK_LP4X_HIGHSET_DDR3733
    g_default_emi_setting.highest_freq = 3733; 
    dramc_crit("MTK_LP4X_HIGHSET_DDR3733 mandatorily set 3733\n");
#endif
#endif
    return 0;
}

unsigned int mt_get_dram_type_from_hw_trap(void)
{

	#if  1 //for bring up use
		return TYPE_LPDDR4X;
	#else
	unsigned int trap = get_dram_type() & 0x7;
	switch (trap) {
		case 0:
			return TYPE_LPDDR4X;
		//case 1:
		//case 2:
		//case 3:
		//case 4:
		//case 5:
		//	return TYPE_LPDDR3;
		//case 6:
		//	return TYPE_LPDDR4;
		default:
			printf("[dramc] Wrond HW Trapping.\n");
			DRAMC_ASSERT(0);
			break;
	}
	#endif
	return 0;

}

void setup_dramc_voltage_by_pmic(void)
{
#ifdef VOLTAGE_SEL
	int vcore;
#endif
#ifdef MTK_PMIC_MT6359
	int ret;

	ret = mtk_regulator_get("vm18", &reg_vio18);
	if (ret)
		dramc_debug("mtk_regulator_get vio18 fail\n");
  
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		dramc_debug("mtk_regulator_get vcore fail\n");

	ret = mtk_regulator_get("VDRAM1", &reg_vdram);
	if (ret)
		printf("mtk_regulator_get vdram fail\n");

	ret = mtk_regulator_get("VDRAM2", &reg_vddq);
	if (ret)
		printf("mtk_regulator_get vddq fail\n");

	ret = mtk_regulator_get("VMDDR", &reg_vmddr);
	if (ret)
		printf("mtk_regulator_get vmddr fail\n");

	mtk_regulator_set_mode(&reg_vcore, 0x1);
	mtk_regulator_set_mode(&reg_vdram, 0x1);

	#ifdef VOLTAGE_SEL
	dramc_set_vio18_voltage(vio18_voltage_select());
	#else
	dramc_set_vio18_voltage(SEL_VIO18);
	#endif
#if defined(VCORE_BIN)
	#ifdef VOLTAGE_SEL
	vcore = vcore_voltage_select(KSHU0);
	if (vcore)
		dramc_set_vcore_voltage(vcore);
	else
	#endif
	dramc_set_vcore_voltage(get_vcore_uv_table(0));
#else
	#ifdef VOLTAGE_SEL
	dramc_set_vcore_voltage(vcore_voltage_select(KSHU0));
	#else
	dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0));
	#endif
#endif
	#ifdef VOLTAGE_SEL
	dramc_set_vdram_voltage(TYPE_LPDDR4, vdram_voltage_select());
	#else
	dramc_set_vdram_voltage(TYPE_LPDDR4, SEL_PREFIX_VDRAM(LP4));
	#endif

	#ifdef VOLTAGE_SEL
	dramc_set_vddq_voltage(TYPE_LPDDR4, vddq_voltage_select());
	#else
	dramc_set_vddq_voltage(TYPE_LPDDR4, SEL_PREFIX_VDDQ);
	#endif

	#ifdef VOLTAGE_SEL
	dramc_set_vmddr_voltage(vmddr_voltage_select());
	#else
	dramc_set_vmddr_voltage(SEL_PREFIX_VMDDR);
	#endif

	dramc_debug("Vio18 = %d\n", dramc_get_vio18_voltage());
	dramc_debug("Vcore = %d\n", dramc_get_vcore_voltage());
	dramc_debug("Vdram = %d\n", dramc_get_vdram_voltage(TYPE_LPDDR4));
	dramc_debug("Vddq = %d\n", dramc_get_vddq_voltage(TYPE_LPDDR4));
	dramc_debug("Vmddr = %d\n", dramc_get_vmddr_voltage());
#endif
}

static void restore_vcore_setting(void)
{
#ifdef VOLTAGE_SEL
	int vcore;
#endif
#ifdef MTK_PMIC_MT6359
	int ret;

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		printf("mtk_regulator_get vcore fail\n");

#if defined(VCORE_BIN)
	#ifdef VOLTAGE_SEL
	vcore = vcore_voltage_select(KSHU0);
	if ((doe_get_config("dram_fix_3094_0825")) || (doe_get_config("dram_all_3094_0825")) || (doe_get_config("dram_opp0_3733_others_3094_0825")))
			dramc_set_vcore_voltage(825000);
	else if (doe_get_config("dram_fix_3094_0725") || (doe_get_config("dram_fix_2400_0725")) || (doe_get_config("dram_fix_1534_0725")) || (doe_get_config("dram_fix_1200_0725")) || (doe_get_config("dram_all_3094_0725")) || (doe_get_config("dram_all_1534_0725")) || (doe_get_config("dram_opp0_3094_others_1534_0725")) || (doe_get_config("dram_opp0_2400_others_1534_0725")))
			dramc_set_vcore_voltage(725000);
	else if ((doe_get_config("dram_fix_1200_065")) || (doe_get_config("dram_fix_800_065")))
			dramc_set_vcore_voltage(650000);
	else if (vcore)
			dramc_set_vcore_voltage(vcore);
	else
	#endif
	dramc_set_vcore_voltage(get_vcore_uv_table(0));
#else
	#ifdef VOLTAGE_SEL
	dramc_set_vcore_voltage(vcore_voltage_select(KSHU0));
	#else
	dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0));
	#endif
#endif

	dramc_debug("Vcore = %d\n", dramc_get_vcore_voltage());
#endif
}

static void restore_pmic_setting(void)
{
#ifdef MTK_PMIC_MT6359
	int ret;

	restore_vcore_setting();
	
	ret = mtk_regulator_get("VDRAM1", &reg_vdram);
	if (ret) {
		printf("mtk_regulator_get vdram fail\n");
		return;
	}

	ret = mtk_regulator_get("VDRAM2", &reg_vddq);
	if (ret) {
		printf("mtk_regulator_get vddq fail\n");
		return;
	}

	ret = mtk_regulator_get("VMDDR", &reg_vmddr);
	if (ret) {
		printf("mtk_regulator_get vmddr fail\n");
		return;
	}
	
	ret = mtk_regulator_get("vm18", &reg_vio18);
	if (ret) {
		printf("mtk_regulator_get vio18 fail\n");
		return;
	}

#if 0 //for bring-up
	dramc_set_vdram_voltage(TYPE_LPDDR4, 1125000);
	dramc_set_vddq_voltage(TYPE_LPDDR4, 600000);
	dramc_set_vmddr_voltage(750000);
#else
	#ifdef VOLTAGE_SEL
	dramc_set_vdram_voltage(TYPE_LPDDR4, vdram_voltage_select());
	#else
	dramc_set_vdram_voltage(TYPE_LPDDR4, SEL_PREFIX_VDRAM(LP4));
	#endif

	#ifdef VOLTAGE_SEL
	dramc_set_vddq_voltage(TYPE_LPDDR4, vddq_voltage_select());
	#else
	dramc_set_vddq_voltage(TYPE_LPDDR4, SEL_PREFIX_VDDQ);
	#endif

	#ifdef VOLTAGE_SEL
	dramc_set_vmddr_voltage(vmddr_voltage_select());
	#else
	dramc_set_vmddr_voltage(SEL_PREFIX_VMDDR);
	#endif

	#ifdef VOLTAGE_SEL
	dramc_set_vio18_voltage(vio18_voltage_select());
	#else
	dramc_set_vio18_voltage(SEL_VIO18);
	#endif
#endif
	dramc_debug("Vdram = %d\n", dramc_get_vdram_voltage(TYPE_LPDDR4));
	dramc_debug("Vddq = %d\n", dramc_get_vddq_voltage(TYPE_LPDDR4));
	dramc_debug("Vmddr = %d\n", dramc_get_vmddr_voltage());
	dramc_debug("Vio18 = %d\n", dramc_get_vio18_voltage());
#endif
}

void switch_dramc_voltage_to_auto_mode(void)
{
#ifdef MTK_PMIC_MT6359
	mtk_regulator_set_mode(&reg_vcore, 0x0);
	mtk_regulator_set_mode(&reg_vdram, 0x0);
#endif
}

void release_dram(void)
{
#ifdef DDR_RESERVE_MODE
    int i;
    int counter = TIMEOUT;

    // scy: restore pmic setting (VCORE, VDRAM, VSRAM, VDDQ)
    restore_pmic_setting();

    drm_release_rg_dramc_conf_iso();//Release DRAMC/PHY conf ISO

#if DDR_RESERVE_NEW_MODE //new modw
    ASVA5_8_New_Mode_1();
    Dramc_DDR_Reserved_Mode_setting();       
    drm_release_rg_dramc_iso();//Release PHY IO ISO
    ASVA5_8_New_Mode_2();  
#else //old mode
    Dramc_DDR_Reserved_Mode_setting();       
    ASVA5_8_CSCA_Pull_Down_EN();
    drm_release_rg_dramc_iso();//Release PHY IO ISO
    ASVA5_8_CSCA_Pull_Down_DIS(); 
#endif

    drm_release_rg_dramc_sref();//Let DRAM Leave SR

    while(counter)
    {
        if(is_dramc_exit_slf() == 1) /* expect to exit dram-self-refresh */
            break;
        counter--;
    }

    if(counter == 0)
    {
        if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
        {
            dramc_crit("[DDR Reserve] release dram from self-refresh FAIL!\n");
            g_ddr_reserve_success = 0;
        }
    }
    else
    {
         dramc_crit("[DDR Reserve] release dram from self-refresh PASS!\n");
    }
    Dramc_DDR_Reserved_Mode_AfterSR();

#if DDR_RESERVE_NEW_MODE //new modw
    ASVA5_8_New_Mode_3(); 
#endif

    //Expect to Use LPDDR3200 and PHYPLL as output, so no need to handle
    //shuffle status since the status will be reset by system reset
    //There is an PLLL_SHU_GP in SPM which will reset by system reset

    // setup for EMI: touch center EMI and channel EMI to enable CLK
    dramc_crit("[DDR reserve] EMI CEN CONA: %x\n", get_cen_emi_cona());
    dramc_crit("[DDR reserve] EMI CHN CONA: %x\n", get_chn_emi_cona());
    for (i=0;i<10;i++);

    return;
#endif
}

void check_ddr_reserve_status(void)
{
	/* get status of DCS and DVFSRC */
	int dvfsrc_success = drm_is_dvfsrc_success();
	int dvfsrc_en = drm_is_dvfsrc_enable();

#ifdef DDR_RESERVE_MODE
	int counter = TIMEOUT;
	if(drm_is_reserve_ddr_enabled()) {
		g_ddr_reserve_enable = 1;
#ifdef LAST_DRAMC
		dram_fatal_set_ddr_rsv_mode_flow();
#endif

		if(drm_is_reserve_ddr_mode_success()) {
			while(counter) {
				if(drm_is_dram_slf()) {
					g_ddr_reserve_success = 1;
					break;
				}
				counter--;
			}
			if(counter == 0) {
				dramc_crit("[DDR Reserve] ddr reserve mode success but DRAM not in self-refresh!\n");
				g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
				dram_fatal_set_ddr_rsv_mode_err();
#endif
			}
		} else {
			dramc_crit("[DDR Reserve] ddr reserve mode FAIL!\n");
			drm_dram_reserved(0);
			g_ddr_reserve_success = 0;
#ifdef LAST_DRAMC
			dram_fatal_set_ddr_rsv_mode_err();
#endif
		}

		/* overwrite g_ddr_reserve_success if dvfsrc failed */
		if (dvfsrc_en == 1 && dvfsrc_success == 0) {
			dramc_crit("[DDR Reserve] DVFSRC fail!\n");
			drm_dram_reserved(0);
			g_ddr_reserve_success = 0;
#if 0//def LAST_DRAMC
			dram_fatal_set_dvfsrc_err();
#endif
		} else {
			dramc_crit("[DDR Reserve] DVFSRC success! (dvfsrc_en=%d)\n", dvfsrc_en);
		}

		/* release dram, no matter success or failed */
		release_dram();
	} else {
		dramc_crit("[DDR Reserve] ddr reserve mode not be enabled yet\n");
		g_ddr_reserve_enable = 0;
	}
#endif
}

unsigned int DRAM_MRR(int MRR_num)
{
    u16 MRR_value = 0x0;
    DRAMC_CTX_T *p = psCurrDramCtx;

    DramcModeRegRead(p, MRR_num, &MRR_value);
    return MRR_value;
}

static int mt_get_dram_type_for_dis(void)
{
    return TYPE_LPDDR4X;
}

#ifdef DRAM_QVL_CHECK
static int check_qvl(DRAM_INFO_BY_MRR_T *dram_info, unsigned int dram_type)
{
	unsigned int mr5;
	unsigned long long rank_size[2];
	char id[22];
	int emmc_nand_id_len=16;
	int fw_id_len;
	int result;
	int i;

	mr5 = dram_info->u2MR5VendorID & 0xFF;

	rank_size[0] = dram_info->u8MR8Density[0]; //now only K CHA to save time
	rank_size[1] = dram_info->u8MR8Density[1]; //now only K CHA to save time

        result = platform_get_mcp_id(id, emmc_nand_id_len,&fw_id_len);
	for (i = 0; i < num_of_emi_records; i++) {
		dramc_crit("[DRAMC] %s(%d),%s(%x),%s(%x),%s(0x%llx),%s(0x%llx)\n",
			"qvl", i,
			"type", qvl_list[i].type,
			"mr5", qvl_list[i].iLPDDR3_MODE_REG_5,
			"rank0_size", qvl_list[i].DRAM_RANK_SIZE[0], //DA need  (unsigned int) (qvl_list[i].DRAM_RANK_SIZE[0] & 0xFFFFFFFF), (unsigned int)(qvl_list[i].DRAM_RANK_SIZE[0] >> 32),
			"rank1_size", qvl_list[i].DRAM_RANK_SIZE[1]);//DA need (unsigned int) (qvl_list[i].DRAM_RANK_SIZE[1] & 0xFFFFFFFF), (unsigned int)(qvl_list[i].DRAM_RANK_SIZE[1] >> 32));
		/* check DRAM type */
		if ((qvl_list[i].type & 0xF) != (dram_type & 0xF))
			continue;

		/* check MR5 */
		if (qvl_list[i].iLPDDR3_MODE_REG_5 != mr5)
			continue;

		/* check rank size */
		if (qvl_list[i].DRAM_RANK_SIZE[0] != rank_size[0])
			continue;
		if (qvl_list[i].DRAM_RANK_SIZE[1] != rank_size[1])
			continue;

		/* check storage ID if MCP */
		if (qvl_list[i].type & 0xF00) {
			if (!result) {
				if (memcmp(id, qvl_list[i].ID, qvl_list[i].id_length)) {
					dramc_crit("[DRAMC] storage id mismatch\n", i);
					continue;
				} else{				    
					return 0;
				}
			}
		} else
			return 0;
	}

	return -1;
}
#endif

int get_dram_channel_support_nr(void)
{
    return DRAMC_MAX_CH;
}

int get_dram_channel_nr(void)
{
	return get_channel_nr_by_emi();
}

int get_dram_rank_nr(void)
{
	int cen_emi_cona;

#ifdef DDR_RESERVE_MODE
	if (g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
		return get_rank_nr_by_emi();
#endif   	

#ifdef DRAM_ADAPTIVE
	if (!auto_detect_done)
		DRAMC_ASSERT(0);
#endif

	cen_emi_cona = g_default_emi_setting.EMI_CONA_VAL;

	if ((cen_emi_cona & (1 << 17)) != 0 || //for channel 0  
	    (cen_emi_cona & (1 << 16)) != 0 )  //for channel 1
		return 2;
	else
		return 1;
}

int get_dram_mr_cnt(void)
{
    return DRAMC_MR_CNT;
}

int get_dram_freq_cnt(void)
{
    return DRAMC_FREQ_CNT;
}

#if (FOR_DV_SIMULATION_USED==0)
#if !__FLASH_TOOL_DA__ && !__ETT__ 
/* 
 * setup block
 */
void get_orig_dram_rank_info(dram_info_t *orig_dram_info)
{
	int i, j;
	u64 base = DRAM_BASE;
	u64 rank_size[DRAMC_MAX_RK];

	i = get_dram_rank_nr();

	orig_dram_info->rank_num = (i > 0) ? i : 0;
	get_dram_rank_size(rank_size);

	orig_dram_info->rank_info[0].start = base;
	for (i = 0; i < orig_dram_info->rank_num; i++) {

		orig_dram_info->rank_info[i].size = (u64)rank_size[i];

		if (i > 0) {
			orig_dram_info->rank_info[i].start =
				orig_dram_info->rank_info[i - 1].start +
				orig_dram_info->rank_info[i - 1].size;
		}
		dramc_debug("orig_dram_info[%d] start: 0x%llx, size: 0x%llx\n",
				i, orig_dram_info->rank_info[i].start,
				orig_dram_info->rank_info[i].size);
	}
	
	for(j=i; j<4; j++)
	{
	  		orig_dram_info->rank_info[j].start = 0;
	  		orig_dram_info->rank_info[j].size = 0;	
	}
#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
	for (i = 1; i < orig_dram_info->rank_num; i++) {
		if (orig_dram_info->rank_info[i].start >= (get_config_max_dram_size() + DRAM_BASE)) {
			orig_dram_info->rank_num = i;
			dramc_crit("[dramc] reduce rank size = %u\n", (unsigned int) orig_dram_info->rank_num);
			break;
		}
	}
#endif
}

void get_dram_rank_size(u64 dram_rank_size[DRAMC_MAX_RK])
{
	int rank_nr, i;

#ifdef DDR_RESERVE_MODE
	if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1){
		get_rank_size_by_emi(dram_rank_size);
		return;
	}
#endif  	

#ifdef DRAM_ADAPTIVE
	if (!auto_detect_done)
		DRAMC_ASSERT(0);
#endif

	rank_nr = get_dram_rank_nr();

	for(i = 0; i < rank_nr; i++) {
		dram_rank_size[i] = g_default_emi_setting.DRAM_RANK_SIZE[i];
		dramc_debug("%d:dram_rank_size:%llx\n",i,dram_rank_size[i]);
	}
}

void get_dram_freq_step(u32 dram_freq_step[])
{
	unsigned int i;
	unsigned int defined_step[DRAMC_FREQ_CNT] = {
		4266, 3200, 2400, 1866, 1600, 1200, 800};

	get_dram_frequency_from_hw_trap();


	if( g_default_emi_setting.highest_freq == 3733){
		defined_step[0] = 3733;
	}

	if (is_discrete_lpddr4()) {
		defined_step[0] = 3200;
	}
	for (i = 0; i < DRAMC_FREQ_CNT; i++) {
		dram_freq_step[i] = defined_step[i];
	}
}

void set_dram_mr(unsigned int index, unsigned short value)
{
#if 0
	unsigned short value_2rk;

	value_2rk = value & 0xFF;
	value_2rk |= (value_2rk << 8);

	switch (index) {
	case 5:
		mr5 = value_2rk;
		break;
	case 6:
		mr6 = value_2rk;
		break;
	case 7:
		mr7 = value_2rk;
		break;
	case 8:
		mr8 = value;
	default:
		break;
	}
#endif
}

unsigned short get_dram_mr(unsigned int index)
{
	unsigned int value = 0;
#if 0
	switch (index) {
	case 5:
		value = last_dramc_info_ptr->mr5;
		break;
	case 6:
		value = last_dramc_info_ptr->mr6;
		break;
	case 7:
		value = last_dramc_info_ptr->mr7;
		break;
	case 8:
		value = last_dramc_info_ptr->mr8;
	default:
		break;
	}
	return (unsigned short)(value & 0xFFFF);
#else
	return (unsigned short)(value & 0xFFFF);
#endif
}

void get_dram_mr_info(struct mr_info_t mr_info[])
{
#if 0
	unsigned int i;
	unsigned int mr_list[DRAMC_MR_CNT] = {5, 6, 7, 8};

	for (i = 0; i < DRAMC_MR_CNT; i++) {
		mr_info[i].mr_index = mr_list[i];
		mr_info[i].mr_value = get_dram_mr(mr_list[i]);
	}
#endif
}

void reserve_dramc_dummy_read(void)
{
	unsigned long long reserve_start;
	char *reserve_name[4] = {"dramc-rk0", "dramc-rk1", "dramc-rk2", "dramc-rk3"};
	unsigned int i;
	int rank_nr;
	dram_addr_t dram_addr;

	dram_addr.ch = 0;

	rank_nr = get_dram_rank_nr();
	if (rank_nr <= 0) {
		dramc_crit("[DRAMC] reserve dummy read fail\n");
		DRAMC_ASSERT(0);
	}

	for (i = 0; i < (unsigned int)rank_nr; i++) {
		dram_addr.rk = i;
		get_dramc_addr(&dram_addr, 0x0);
#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
		if (dram_addr.full_sys_addr > get_config_max_dram_size())
			break;
#endif
		reserve_start = mblock_reserve_ext(&bootarg.mblock_info, 0x1000, 0x1000, dram_addr.full_sys_addr, 0, reserve_name[i]);
		if (reserve_start != (dram_addr.full_sys_addr - 0x1000)) {
			dramc_crit("[DRAMC] dummy read fail (0x%llx)\n", reserve_start);
			DRAMC_ASSERT(0);
		}
	}
}
#endif //#if !__FLASH_TOOL_DA__ && !__ETT__
#endif

void freq_table_are_all_3094(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR3200;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].SRAMIdx= SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR3200;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].SRAMIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR3200;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].SRAMIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR3200;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].SRAMIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR3200;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].SRAMIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR3200;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].SRAMIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR3200;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].SRAMIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;

}

void freq_table_are_all_1534(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR1600;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].SRAMIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR1600;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].SRAMIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR1600;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].SRAMIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR1600;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].SRAMIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR1600;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].SRAMIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR1600;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].SRAMIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR1600;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].SRAMIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;

}

void freq_table_opp0_3733_others_3094(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR3200;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].SRAMIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR3200;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].SRAMIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR3200;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].SRAMIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR3733;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].SRAMIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR3200;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].SRAMIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR3200;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].SRAMIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR3200;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].SRAMIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;
}

void freq_table_opp0_3094_others_1534(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR1600;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].SRAMIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR1600;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].SRAMIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR1600;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].SRAMIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR3200;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].SRAMIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR1600;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].SRAMIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR1600;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].SRAMIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR1600;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].SRAMIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;
}

void freq_table_opp0_2400_others_1534(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR1600;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].SRAMIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR1600;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].SRAMIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR1600;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].SRAMIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR2400;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].SRAMIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR1600;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].SRAMIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR1600;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].SRAMIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR1600;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].SRAMIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;
}
#if (CFG_DRAM_LOG_TO_STORAGE)
#include <blkdev.h>
#include <partition.h>
#include <storage_api.h>

extern u64 get_part_addr(const char *name);
u64 part_dram_data_addr_uart = 0;
u32 log_start = 0;
static char logbuf[1024];
static int logcount;
#endif

#ifdef VOLTAGE_SEL
void update_voltage_select_info(void)
{
	voltage_sel_info_ptr.vcore = doe_get_config("vcore");
	voltage_sel_info_ptr.vdram = doe_get_config("vdram");
	voltage_sel_info_ptr.vddq = doe_get_config("vddq");
	voltage_sel_info_ptr.vmddr = doe_get_config("vmddr");
	voltage_sel_info_ptr.vio18 = doe_get_config("vio18");

	print("DOE setting: vcore %d, vdram %d, vddq %d, vmddr %d, vio18 %d \n",
		voltage_sel_info_ptr.vcore, voltage_sel_info_ptr.vdram,
		voltage_sel_info_ptr.vddq, voltage_sel_info_ptr.vmddr,
		voltage_sel_info_ptr.vio18);
}

int vio18_voltage_select()
{
	if (voltage_sel_info_ptr.vio18 == LEVEL_LV) {
		return HQA_VIO18_LV;	
	} else if (voltage_sel_info_ptr.vio18 == LEVEL_HV) {
		return HQA_VIO18_HV;
	} else {
		return HQA_VIO18_NV;
	}
}

int vmddr_voltage_select()
{
	if (voltage_sel_info_ptr.vmddr == LEVEL_LV) {
		return HQA_VMDDR_LV_LP4;	
	} else if (voltage_sel_info_ptr.vmddr == LEVEL_HV) {
		return HQA_VMDDR_HV_LP4;
	} else {
		return HQA_VMDDR_NV_LP4;
	}
}

int vddq_voltage_select()
{
	if (voltage_sel_info_ptr.vddq == LEVEL_LV) {
		return HQA_VDDQ_LV_LP4;	
	} else if (voltage_sel_info_ptr.vddq == LEVEL_HV) {
		return HQA_VDDQ_HV_LP4;
	} else {
		return HQA_VDDQ_NV_LP4;
	}
}

int vdram_voltage_select(void)
{
	if (voltage_sel_info_ptr.vdram == LEVEL_LV) {
		return HQA_VDRAM_LV_LP4;
	} else if (voltage_sel_info_ptr.vdram == LEVEL_HV) {
		return HQA_VDRAM_HV_LP4;
	} else {
		return HQA_VDRAM_NV_LP4;
	}
}

int vcore_voltage_select(DRAM_KSHU kshu)
{
	int ret = 0;
	if (voltage_sel_info_ptr.vcore == LEVEL_LV) {
		switch(kshu) {
			case KSHU0:
				ret = HQA_VCORE_LV_LP4_KSHU0_PL;
				break;
			case KSHU1:
				ret = HQA_VCORE_LV_LP4_KSHU1_PL;
				break;
			case KSHU2:
				ret = HQA_VCORE_LV_LP4_KSHU2_PL;
				break;
			case KSHU3:
				ret = HQA_VCORE_LV_LP4_KSHU3_PL;
				break;
			case KSHU4:
				ret = HQA_VCORE_LV_LP4_KSHU4_PL;
				break;
			case KSHU5:
				ret = HQA_VCORE_LV_LP4_KSHU5_PL;
				break;
			case KSHU6:
				ret = HQA_VCORE_LV_LP4_KSHU6_PL;
				break;
		};
	} else if (voltage_sel_info_ptr.vcore == LEVEL_HV) {
		switch(kshu) {
			case KSHU0:
				ret = HQA_VCORE_HV_LP4_KSHU0_PL;
				break;
			case KSHU1:
				ret = HQA_VCORE_HV_LP4_KSHU1_PL;
				break;
			case KSHU2:
				ret = HQA_VCORE_HV_LP4_KSHU2_PL;
				break;
			case KSHU3:
				ret = HQA_VCORE_HV_LP4_KSHU3_PL;
				break;
			case KSHU4:
				ret = HQA_VCORE_HV_LP4_KSHU4_PL;
				break;
			case KSHU5:
				ret = HQA_VCORE_HV_LP4_KSHU5_PL;
				break;
			case KSHU6:
				ret = HQA_VCORE_HV_LP4_KSHU6_PL;
				break;
		};
	} else {
#if defined(VCORE_BIN)
		ret = 0;
#else
		switch(kshu) {
			case KSHU0:
				ret = HQA_VCORE_NV_LP4_KSHU0_PL;
				break;
			case KSHU1:
				ret = HQA_VCORE_NV_LP4_KSHU1_PL;
				break;
			case KSHU2:
				ret = HQA_VCORE_NV_LP4_KSHU2_PL;
				break;
			case KSHU3:
				ret = HQA_VCORE_NV_LP4_KSHU3_PL;
				break;
			case KSHU4:
				ret = HQA_VCORE_NV_LP4_KSHU4_PL;
				break;
			case KSHU5:
				ret = HQA_VCORE_NV_LP4_KSHU5_PL;
				break;
			case KSHU6:
				ret = HQA_VCORE_NV_LP4_KSHU6_PL;
				break;
		};
#endif
	}

	return ret;
}

#endif

#ifdef DRAM_ADAPTIVE
static int update_dram_setting(EMI_SETTINGS *default_emi_setting, unsigned int dram_type, DRAM_INFO_BY_MRR_T *dram_info)
{
	default_emi_setting->type = dram_type;

	if (dram_info != NULL) {
		default_emi_setting->DRAM_RANK_SIZE[0] = (u64)dram_info->u8MR8Density[0];
		default_emi_setting->DRAM_RANK_SIZE[1] = (u64)dram_info->u8MR8Density[1];

		default_emi_setting->iLPDDR3_MODE_REG_5 = dram_info->u2MR5VendorID;

		if (dram_info->u4RankNum == 1) { // single rank
			if (dram_info->u1DieNum[RANK_0] == 1)
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_NORMAL;
			else if (dram_info->u1DieNum[RANK_0] == 2)
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_BYTE;
			else
				return -1;
		} else if (dram_info->u4RankNum == 2) { // dual rank
			if ((dram_info->u1DieNum[RANK_0] == 1) && (dram_info->u1DieNum[RANK_1] == 1))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_NORMAL;
			else if ((dram_info->u1DieNum[RANK_0] == 1) && (dram_info->u1DieNum[RANK_1] == 2))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_NORMAL_R1_BYTE;
			else if ((dram_info->u1DieNum[RANK_0] == 2) && (dram_info->u1DieNum[RANK_1] == 1))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_BYTE_R1_NORMAL;
			else if ((dram_info->u1DieNum[RANK_0] == 2) && (dram_info->u1DieNum[RANK_1] == 2))
				default_emi_setting->dram_cbt_mode_extern = CBT_R0_R1_BYTE;
			else
				return -2;
		} else
			return -3;
	} else
		return -4;

	return 0;
}

static int decode_emi_info(EMI_INFO_T *emi_info, unsigned int dram_type, DRAM_INFO_BY_MRR_T *dram_info)
{
	unsigned int i;
	unsigned long long die_size;

	emi_info->dram_type = dram_type;
	emi_info->ch_num = 2;
	emi_info->bank_width[0] = 3;
	emi_info->bank_width[1] = 3;
	emi_info->col_width[0] = 10;
	emi_info->col_width[1] = 10;

	if (dram_info != NULL) {
		emi_info->rank_size[0] = (u64)dram_info->u8MR8Density[0];
		emi_info->rank_size[1] = (u64)dram_info->u8MR8Density[1];

		emi_info->rk_num = dram_info->u4RankNum;

		for (i = 0; i < emi_info->rk_num; i++) {
			die_size = emi_info->rank_size[i] / dram_info->u1DieNum[i];
			switch (die_size | (dram_info->u1DieNum[i] << 4) | u1IsLP4Family(dram_type)) {
			case 0x20000011ULL: // 4Gb, x16, LP4
				emi_info->row_width[i] = 14;
				break;
			case 0x20000021ULL: // 4Gb, x8, LP4
			case 0x40000021ULL: // 8Gb, x8, LP4
			case 0x30000011ULL: // 6Gb, x16, LP4
			case 0x40000011ULL: // 8Gb, x16, LP4
				emi_info->row_width[i] = 15;
				break;
			case 0x30000021ULL: // 6Gb, x8, LP4
			case 0x60000011ULL: // 12Gb, x16, LP4
			case 0x80000011ULL: // 16Gb, x16, LP4
				emi_info->row_width[i] = 16;
				break;
			case 0x060000021ULL: // 12Gb, x8, LP4
			case 0x080000021ULL: // 16Gb, x8, LP4
			case 0x0C0000011ULL: // 24Gb, x16, LP4
			case 0x100000011ULL: // 32Gb, x16, LP4
				emi_info->row_width[i] = 17;
				break;
			case 0x0C0000021ULL: // 24Gb, x8, LP4
			case 0x100000021ULL: // 32Gb, x8, LP4
				emi_info->row_width[i] = 18;
				break;
			default:
				return -1;
			}
		}
	} else
		return -1;

	return 0;
}
#endif

#if (!__FLASH_TOOL_DA__ && !__ETT__)
int is_need_update_tmrs_for_samsung(void)
{
	char ufs_id[UFS_EMMC_ID_LEN + 1];
	int fw_id_len;
	int result;
	unsigned int i;

	result = platform_get_mcp_id(ufs_id, UFS_EMMC_ID_LEN, &fw_id_len);
	if (!result) {
		for (i = 0; i < sizeof(pn_list_need_tmrs)/sizeof(PN_NEED_TMRS_T); i++) {
			if (!strncmp(ufs_id, pn_list_need_tmrs[i].ufs_id, strlen(pn_list_need_tmrs[i].ufs_id))) {
				return 0;
			}
		}
	} else {
		dramc_crit("[DRAMC] get ufs/emmc id fail!\n");
		DRAMC_ASSERT(0);
	}
	return -1;
}
#endif

#if (FOR_DV_SIMULATION_USED==0)
void dram_auto_detection(void)
{
	DRAM_INFO_BY_MRR_T dram_info;
	EMI_INFO_T emi_info;
	DRAM_CBT_MODE_EXTERN_T dram_mode;
	unsigned int dram_type;
	int ret;

	memset((void *)&dram_info, 0, sizeof(dram_info));
	dram_type = (unsigned int)mt_get_dram_type_for_dis();
	g_default_emi_setting.type &= ~0xFF;
	g_default_emi_setting.type |= (dram_type & 0xFF);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if (!u1IsLP4Family(dram_type) ||
		read_offline_dram_mdl_data(&dram_info) < 0) {
#endif
		dram_mode = (u1IsLP4Family(dram_type))?
			CBT_BYTE_MODE1 : CBT_NORMAL_MODE;
#if defined(SLT)
		SLT_Test_Main_Flow(dram_type, dram_mode, &dram_info, SLT_USED);
#endif
		Init_DRAM(dram_type, dram_mode, &dram_info, GET_MDL_USED);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
		if (u1IsLP4Family(dram_type))
			write_offline_dram_mdl_data(&dram_info);
	}
#endif

#ifdef DRAM_QVL_CHECK
	ret = check_qvl(&dram_info, dram_type);
	if (ret) {
		dramc_crit("[DRAMC] check_qvl err %d\n", ret);
		DRAMC_ASSERT(0);
	}
#endif

	ret = update_dram_setting(&g_default_emi_setting, dram_type, &dram_info);
	if (ret) {
		dramc_crit("[DRAMC] update_dram_setting err %d\n", ret);
		DRAMC_ASSERT(0);
	}

	ret = decode_emi_info(&emi_info, dram_type, &dram_info);
	if (ret) {
		dramc_crit("[DRAMC] decode_emi_info err %d\n", ret);
		DRAMC_ASSERT(0);
	}

	ret = update_emi_setting(&g_default_emi_setting, &emi_info);
	if (ret) {
		dramc_crit("[DRAMC] update_emi_setting err %d\n", ret);
		DRAMC_ASSERT(0);
	}

	auto_detect_done = 1;
}

void mt_set_emi(void)
{
	int index;
	/*unsigned int SW_CTRL_VC, HW_CTRL_VC;*/
	EMI_SETTINGS *emi_set;

#ifdef VOLTAGE_SEL
	update_voltage_select_info();
#endif

#if ENABLE_PINMUX_FOR_RANK_SWAP
	EMI_rank_swap_handle();
#endif

	// set voltage and hw trapping before mdl
	setup_dramc_voltage_by_pmic();

	if ((doe_get_config("dram_all_3094_0825")) || (doe_get_config("dram_all_3094_0725")))
		freq_table_are_all_3094(); 
	else if	(doe_get_config("dram_all_1534_0725"))
		freq_table_are_all_1534();
	else if	(doe_get_config("dram_opp0_3733_others_3094_0825"))
		freq_table_opp0_3733_others_3094();
	else if	(doe_get_config("dram_opp0_3094_others_1534_0725"))
		freq_table_opp0_3094_others_1534();
	else if	(doe_get_config("dram_opp0_2400_others_1534_0725"))
		freq_table_opp0_2400_others_1534();

#ifdef DRAM_ADAPTIVE
	dram_auto_detection();
#endif
	emi_set = &g_default_emi_setting;

#ifdef DDR_RESERVE_MODE
	if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==0)
		Before_Init_DRAM_While_Reserve_Mode_fail(emi_set->type & 0xF);
#endif

#if (CFG_DRAM_LOG_TO_STORAGE)
		log_start = 1;
		print("log_start=0x%x part_dram_data_addr_uart=0x%llx \n",log_start,part_dram_data_addr_uart);
#endif
#if defined(SLT)
    SLT_Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL, NORMAL_USED);
#else
	Init_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode_extern, NULL, NORMAL_USED);
#endif
	switch_dramc_voltage_to_auto_mode();
	restore_vcore_setting();

#if (CFG_DRAM_LOG_TO_STORAGE)
    log_start = 0;
    print("log_start=0x%x part_dram_data_addr_uart=0x%llx \n",log_start,part_dram_data_addr_uart);
#endif
#if 0
	{
		DRAMC_CTX_T * p = psCurrDramCtx;
		DramcRegDump(p);
	}
#endif
}
#endif

#define DRAMC_ADDR_SHIFT_CHN(addr, channel) (addr + (channel * 0x10000))

void put_dummy_read_pattern(unsigned long long dst_pa, unsigned int src_pa, unsigned int len)
{
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x018)) = 7 << 16;

	*((volatile unsigned int *)(CQ_DMA_BASE + 0x01c)) = src_pa;
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x060)) = 0;

	*((volatile unsigned int *)(CQ_DMA_BASE + 0x020)) = dst_pa & 0xffffffff;
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x064)) = dst_pa >> 32;

	*((volatile unsigned int *)(CQ_DMA_BASE + 0x024)) = len;
	dsb();
	*((volatile unsigned int *)(CQ_DMA_BASE + 0x008)) = 0x1;

	while(*((volatile unsigned int *)(CQ_DMA_BASE + 0x008)));
}

unsigned int get_dramc_addr(dram_addr_t *dram_addr, unsigned int offset)
{
	static char init_pattern = 0;
	unsigned int channel_num, rank_num;
	unsigned long long dummy_read_addr;
	unsigned long long rank_size[DRAMC_MAX_RK];
	unsigned int index;
	unsigned int *src_addr;

	channel_num = (unsigned int) get_dram_channel_nr();
	rank_num = (unsigned int) get_dram_rank_nr();
	get_rank_size_by_emi(rank_size);
	dummy_read_addr = 0x40000000;
	src_addr = (unsigned int *) 0x40000000;

	if (dram_addr->ch >= channel_num) {
		mcSHOW_DBG_MSG(("[DRAMC] invalid channel: %d\n", dram_addr->ch));
		return 0;
	}

	if (dram_addr->rk >= rank_num) {
		mcSHOW_DBG_MSG(("[DRAMC] invalid rank: %d\n", dram_addr->rk));
		return 0;
	}

	for (index = 0; index <= dram_addr->rk; index++)
		dummy_read_addr += rank_size[index];
	dummy_read_addr -= offset;
	dummy_read_addr &= ~(0x300);

	if (offset == 0x20) {
		for (index = 0; index < 4; index++)
			*(src_addr + index) = 0xAAAA5555;

		if (!init_pattern) {
			for (index = 0; index < channel_num; index++) {
				put_dummy_read_pattern(dummy_read_addr | (index << 8),
					(unsigned int) src_addr, 16);
			}

			init_pattern = 1;
		}
	}

	dram_addr->full_sys_addr = dummy_read_addr;
	phy_addr_to_dram_addr(dram_addr, dummy_read_addr);

	return dram_addr->addr;
}

unsigned int get_dummy_read_addr(dram_addr_t *dram_addr)
{
	return get_dramc_addr(dram_addr, 0x20); // 32-byte align for dummy RW pattern
}

unsigned int get_ta2_addr(dram_addr_t *dram_addr)
{
	return (get_dramc_addr(dram_addr, 0x1000) >> 2) & 0xFFFFFFF8;
}

void init_ta2_single_channel(unsigned int channel)
{
	unsigned int temp;
	dram_addr_t dram_addr;
	DRAMC_CTX_T *p = psCurrDramCtx;
	int test_cnt;

	// disable self test engine1 and self test engine2
	temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A3, channel)) & 0x1FFFFFFF;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A3, channel), temp);

	// set rank address for test agent to auto
	temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A4, channel)) & 0x8FFFFFFF;
	temp |= (0x4 << 28);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A4, channel), temp);

	// set test for both rank0 and rank1
	temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A3, channel)) & 0xFFFFFFF0;
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A3, channel), temp | 0x1);

	// set base address for test agent to reserved space
	dram_addr.ch = channel;
	dram_addr.rk = 0;
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_RK_TEST2_A1, channel)) & 0x00000007);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_RK_TEST2_A1, channel), temp | get_ta2_addr(&dram_addr));
	dram_addr.rk = 1;
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_RK_TEST2_A1+0x200, channel)) & 0x00000007);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_RK_TEST2_A1+0x200, channel), temp | get_ta2_addr(&dram_addr));

	// set test length (offset) to 0x20
	temp = (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A2, channel)) & 0x0000000F) | (0x20 << 4);
	vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A2, channel), temp);

	// set TA2 pattern to the worst case
	test_cnt = (get_dram_rank_nr() > 1) ? 1 : 0;
	vIO32WriteFldAlign(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A3, channel), 0, TEST2_A3_TESTAUDPAT);
	vIO32WriteFldAlign(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A3, channel), test_cnt, TEST2_A3_TESTCNT);
	vIO32WriteFldAlign(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_TEST2_A4, channel), 1, TEST2_A4_TESTXTALKPAT);

	return;
}

#ifdef LAST_DRAMC

static unsigned int is_last_dramc_initialized(void)
{
	if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
		return 0;
	} else {
		return 1;
	}
}

void update_last_dramc_info(void)
{
	unsigned int chn;
	unsigned int latch_result = 0;
	unsigned int temp;
	unsigned int *curr;
	DRAMC_CTX_T *p = psCurrDramCtx;

	// init checksum and magic pattern
	if(last_dramc_info_ptr->ta2_result_magic != LAST_DRAMC_MAGIC_PATTERN) {
		last_dramc_info_ptr->ta2_result_magic = LAST_DRAMC_MAGIC_PATTERN;
		last_dramc_info_ptr->ta2_result_last = 0;
		last_dramc_info_ptr->ta2_result_past = 0;
		last_dramc_info_ptr->ta2_result_checksum = LAST_DRAMC_MAGIC_PATTERN;
		last_dramc_info_ptr->reboot_count = 0;
		last_dramc_info_ptr->mr5 = mr5;
		last_dramc_info_ptr->mr6 = mr6;
		last_dramc_info_ptr->mr7 = mr7;
		last_dramc_info_ptr->mr8 = mr8;
	} else {
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
		last_dramc_info_ptr->reboot_count++;
		last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->reboot_count;
	}

	// TODO: check DCS status

	// read data from latch register and reset
	for (chn = 0; chn < CHANNEL_NUM; ++chn) {
		//dramc_crit("[LastDRAMC] latch result before RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
		latch_result = (latch_result << 16) | u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)) & 0xFFFF;
		temp = u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_RST, chn));
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_RST, chn), temp | 0x00000001);
		vIO32Write4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_RST, chn), temp & 0xFFFFFFFE);
		//dramc_crit("[LastDRAMC] latch result after RST: %x\n", u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
	}

	last_dramc_info_ptr->ta2_result_checksum ^= last_dramc_info_ptr->ta2_result_past ^ latch_result;
	last_dramc_info_ptr->ta2_result_past = last_dramc_info_ptr->ta2_result_last;
	last_dramc_info_ptr->ta2_result_last = latch_result;
	for (temp = 0; temp < sizeof(LAST_DRAMC_INFO_T) / sizeof(temp); temp++) {
		curr = (unsigned int *)last_dramc_info_ptr + temp;
		dramc_crit("[LastDRAMC] 0x%x: 0x%x\n", curr, *curr);
	}

	return;
}

void init_ta2_all_channel(void)
{
	unsigned int chn;

	update_last_dramc_info();

	//cache flush after update dramc info
	plat_clean_invalidate_dcache();

	// TODO: consider DCS
	for (chn = 0; chn < CHANNEL_NUM; ++chn)
		init_ta2_single_channel(chn);
}


unsigned int check_gating_err_in_dramc_latch(void)
{
	unsigned int chn, ret = 0;
	DRAMC_CTX_T *p = psCurrDramCtx;

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		dramc_crit("for cold boot, always return 0\n");
		return 0;
	}

	for (chn = 0; chn <= 3; ++chn) {
		dramc_crit("[dramc] latch check in channel %d (0x%x)\n",
				chn, u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
		if (u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)) & 0x4000) {
			dramc_crit("[dramc] found gating error in channel %d (0x%x)\n",
					chn, u4IO32Read4B(DRAMC_ADDR_SHIFT_CHN(DRAMC_REG_WDT_DBG_SIGNAL, chn)));
			ret |= (1 << chn);
		}
	}

	return ret;
}

void dram_fatal_exception_detection_start(void)
{
	last_dramc_info_ptr = (LAST_DRAMC_INFO_T *) get_dbg_info_base(KEY_LAST_DRAMC);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	part_dram_data_addr = get_part_addr("boot_para") + 0x100000; // addr = 0x108000
	if (part_dram_data_addr != 0x0)
		dramc_crit("[dramc] init partition address is 0x%llx\n", part_dram_data_addr);
	else {
		dramc_crit("[dramc] init partition address is incorrect !!!\n");
	}
#endif

#if defined(SLT)
	part_dram_data_addr_slt = get_part_addr("boot_para") + 0x100000;
#endif

	if ((g_boot_reason == BR_POWER_KEY) || (g_boot_reason == BR_USB)
			|| mtk_wdt_is_pmic_full_reset() || (is_last_dramc_initialized() == 0)){
		/* cold boot: initialize last_dram_fatal_err_flag and dram_fatal_err_flag */
		dramc_crit("[dramc] init SRAM region for DRAM exception detection\n");
		last_dramc_info_ptr->last_fatal_err_flag = 0x0;
		last_dramc_info_ptr->storage_api_err_flag = 0x0;
		dram_fatal_init_stberr();
	} else {
		last_dramc_info_ptr->last_fatal_err_flag = last_dramc_info_ptr->fatal_err_flag;
		last_dramc_info_ptr->storage_api_err_flag = 0x0;
		dram_fatal_backup_stberr();
		dram_fatal_init_stberr();
	}

	last_dramc_info_ptr->fatal_err_flag = 1 << OFFSET_DRAM_FATAL_ERR;
	dsb();
}

void dram_fatal_exception_detection_end(void)
{
	last_dramc_info_ptr->fatal_err_flag = 0x0;
	dsb();
}

unsigned int check_dram_fatal_exception(void)
{
	dramc_crit("[dramc] DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->fatal_err_flag);

	return ((last_dramc_info_ptr->fatal_err_flag & ~((1 << OFFSET_DRAM_FATAL_ERR)|DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

unsigned int check_last_dram_fatal_exception(void)
{
	dramc_crit("[dramc] LAST_DRAM_FATAL_ERR_FLAG = 0x%x\n", last_dramc_info_ptr->last_fatal_err_flag);

	return ((last_dramc_info_ptr->last_fatal_err_flag & ~(DDR_RSV_MODE_ERR_MASK)) != 0x0) ? 1 : 0;
}

void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_TA2_ERR + 2 * chn, ret;

	if (chn > 3)
		return;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(0x7 << shift);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & 0x7) << shift);
	dsb();
}

void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code)
{
	unsigned int shift = OFFSET_DRAM_GATING_ERR + 4 * chn, ret;

	if (chn > 3)
		return;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(0xf << shift);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & 0xf) << shift);
	dsb();
}

void dram_fatal_init_stberr(void)
{
	last_dramc_info_ptr->gating_err[0][0] = 0x0;
	last_dramc_info_ptr->gating_err[0][1] = 0x0;
	last_dramc_info_ptr->gating_err[1][0] = 0x0;
	last_dramc_info_ptr->gating_err[1][1] = 0x0;

	dsb();
}

void dram_fatal_backup_stberr(void)
{
	last_dramc_info_ptr->last_gating_err[0][0] = last_dramc_info_ptr->gating_err[0][0];
	last_dramc_info_ptr->last_gating_err[0][1] = last_dramc_info_ptr->gating_err[0][1];
	last_dramc_info_ptr->last_gating_err[1][0] = last_dramc_info_ptr->gating_err[1][0];
	last_dramc_info_ptr->last_gating_err[1][1] = last_dramc_info_ptr->gating_err[1][1];

	dsb();
}

void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code)
{
	if ((chn > 1) || (rk > 1))
		return;

	last_dramc_info_ptr->gating_err[chn][rk] = err_code;

	dsb();
}

void dram_fatal_set_err(unsigned int err_code, unsigned int mask, unsigned int offset)
{
	unsigned int ret;

	ret = last_dramc_info_ptr->fatal_err_flag & ~(mask << offset);
	last_dramc_info_ptr->fatal_err_flag = ret | ((err_code & mask) << offset);
	dsb();
}

#endif

#if (FOR_DV_SIMULATION_USED==0) // for DV sim build pass
int doe_get_config(char* feature)
{
#if defined(ENABLE_DOE)
	char *doe_feature = dconfig_getenv(feature);
	int doe_result = atoi(doe_feature);
	dramc_debug("DOE force setting %s=%d\n", feature, doe_result);
	return doe_result;
#else
	return 0;
#endif
}
#endif

#if (CFG_DRAM_LOG_TO_STORAGE)
void log_to_storage(const char c)
{
	int ret, clr_count;
	blkdev_t *bootdev = NULL;
	static u8 logen = 0;

	bootdev = blkdev_get(CFG_BOOT_DEV);

	if (log_start && (!logen)) {
		logen = 1;
		logcount = 0;
		part_dram_data_addr_uart = get_part_addr("boot_para") + 0x100000; // addr = 0x1f300000, the first 1MB for debug
		memset(&logbuf, 0, sizeof(logbuf));
		for (clr_count = 0; clr_count < 3072 ; clr_count++) //3M
			ret = blkdev_write(bootdev, (part_dram_data_addr_uart + (1024 * clr_count)), 1024, (u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));
	}

	if (log_start) {
    	if (((((char) c >> 4) & 0x7) > 1) & ((((char) c >> 4) & 0x7) < 7))
        logbuf[logcount] = ((char) c & 0xF0) | (((char) c >> 2) & 0x03) | (((char) c << 2) & 0x0C);
			else
				logbuf[logcount] = (char) c;
    	logcount = logcount + 1;
    	//write to storage
    	if (logcount==1024) {
    		logcount = 0;
    		ret = blkdev_write(bootdev, part_dram_data_addr_uart, 1024, (u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));   
    		part_dram_data_addr_uart = part_dram_data_addr_uart + 1024;
    	}
    }
}
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#if !__ETT__
#include <blkdev.h>
#include <partition.h>
#include <pl_version.h>
#else
#include "ett_common.h"
#include "emi.h"
#endif

u32 g_dram_storage_api_err_code;

static u16 crc16(const u8* data, u32 length){
	u8 x;
	u16 crc = 0xFFFF;

	while (length--) {
		x = crc >> 8 ^ *data++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((u8)(x << 12)) ^ ((u8)(x <<5)) ^ ((u8)x);
	}
	return crc;
}

static void assign_checksum_for_dram_data(DRAM_CALIBRATION_SHU_DATA_T *shu_data)
{
	/* need to initialize checksum to 0 before calculation */
	shu_data->checksum = 0;
	shu_data->checksum = crc16((u8*)shu_data, sizeof(*shu_data));
}

static int check_checksum_for_dram_data(DRAM_CALIBRATION_SHU_DATA_T *shu_data)
{
	u16 checksum_in_storage = shu_data->checksum;

	assign_checksum_for_dram_data(shu_data);

	return (shu_data->checksum == checksum_in_storage) ? 1 : 0;
}

#if !__ETT__
static void assign_checksum_for_mdl_data(DRAM_CALIBRATION_MRR_DATA_T *mrr_info)
{
	/* need to initialize checksum to 0 before calculation */
	mrr_info->checksum = 0;
	mrr_info->checksum = crc16((u8*)mrr_info, sizeof(*mrr_info));
}

static int check_checksum_for_mdl_data(DRAM_CALIBRATION_MRR_DATA_T *mrr_info)
{
	u16 checksum_in_storage = mrr_info->checksum;

	assign_checksum_for_mdl_data(mrr_info);

	return (mrr_info->checksum == checksum_in_storage) ? 1 : 0;
}

static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	int i, ret;
	u16 emi_checksum;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_MRR_DATA_T mrr_info;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (dram_offline_data_flags)
		goto exit;

	if (DramInfo == NULL) {
		dramc_crit("[dramc] DramInfo == NULL, skip\n");
		dram_offline_data_flags = ERR_NULL_POINTER;
		goto exit;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		dram_offline_data_flags = ERR_BLKDEV_NOT_FOUND;
		goto exit;
	}

	if (!part_dram_data_addr) {
		dram_offline_data_flags = ERR_BLKDEV_NO_PART;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "hdr");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	if (hdr.pl_version != PL_VERSION) {
		/* current preloader version does not match the calibration hdr in storage -> erase the partition */
		dramc_crit("[dramc] PL_VERSION is updated, erase the DRAM shu_data\n");

		shu_data.checksum = 0;

		/* clear each shuffle */
		for (i = 0; i < DRAM_DFS_SRAM_MAX; i++) {
			ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
			if (ret != 0) {
				dramc_crit("[dramc] blkdev_write failed\n");
				dram_offline_data_flags = ERR_BLKDEV_WRITE_FAIL;
				goto exit;
			}
		}

		dram_offline_data_flags = ERR_PL_UPDATED;
		goto exit;
	}

	if ((doe_get_config("dram_all_3094_0825")) || (doe_get_config("dram_all_3094_0725")) || (doe_get_config("dram_all_1534_0725")) || (doe_get_config("dram_opp0_3733_others_3094_0825")) || (doe_get_config("dram_opp0_3094_others_1534_0725")) || (doe_get_config("dram_opp0_2400_others_1534_0725"))) {

		shu_data.checksum = 0;

		/* clear each shuffle */
		for (i = 0; i < DRAM_DFS_SRAM_MAX; i++) {
			ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
			if (ret != 0) {
				dramc_crit("[dramc] blkdev_write failed\n");
				dram_offline_data_flags = ERR_BLKDEV_WRITE_FAIL;
				goto exit;
			}
		}

		dram_offline_data_flags = ERR_PL_UPDATED;
		goto exit;
	}

	/* check magic number */
	if (hdr.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		dramc_crit("[dramc] magic number mismatch\n");
		dram_offline_data_flags = ERR_MAGIC_NUMBER;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) &datap->mrr_info), sizeof(mrr_info), (u8*)&mrr_info, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "data");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	/* check checksum */
	if (check_checksum_for_mdl_data(&mrr_info) != 1) {
		dramc_crit("[dramc] checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;

		goto exit;
	}

	emi_checksum = crc16((u8*)qvl_list, sizeof(qvl_list));

	if (emi_checksum != mrr_info.emi_checksum) {
		dramc_crit("[dramc] emi checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;

		goto exit;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(DramInfo, &(mrr_info.DramInfo), sizeof(*DramInfo));

exit:
	if (dram_offline_data_flags)
		SET_DRAM_STORAGE_API_ERR(dram_offline_data_flags, DRAM_STORAGE_API_READ);

	return 0 - dram_offline_data_flags;
}

static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_MRR_DATA_T mrr_info;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (DramInfo == NULL) {
		dramc_crit("[dramc] DramInfo == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memcpy(&(mrr_info.DramInfo), DramInfo, sizeof(*DramInfo));

#if 0
    /* assign PL version */
    hdr.pl_version = PL_VERSION;

    /* assign magic number */
    hdr.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

    /* assign api error code */
    hdr.calib_err_code = g_dram_storage_api_err_code;

    ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, PART_ID_DRAM_DATA);
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }
#endif

    /* calculate and assign checksum */
    mrr_info.emi_checksum = crc16((u8*)qvl_list, sizeof(qvl_list));
    assign_checksum_for_mdl_data(&mrr_info);

    ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->mrr_info), sizeof(mrr_info), (u8*)&mrr_info, storage_get_part_id(STORAGE_PHYS_PART_USER));
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }

    return 0;
}

int read_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int i, ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;
	unsigned char *ch;

	if (dram_offline_data_flags)
		goto exit;

	if (offLine_SaveData == NULL) {
		dramc_crit("[dramc] offLine_SaveData == NULL, skip\n");
		dram_offline_data_flags = ERR_NULL_POINTER;
		goto exit;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		dram_offline_data_flags = ERR_BLKDEV_NOT_FOUND;
		goto exit;
	}

	if (!part_dram_data_addr) {
		dram_offline_data_flags = ERR_BLKDEV_NO_PART;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "hdr");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

    	/* check preloader version */
	if (hdr.pl_version != PL_VERSION) {
		/* current preloader version does not match the calibration hdr in storage -> erase the partition */
		dramc_crit("[dramc] PL_VERSION is updated, erase the DRAM shu_data\n");

		shu_data.checksum = 0;

		/* clear each shuffle */
		for (i = 0; i < DRAM_DFS_SRAM_MAX; i++) {
			ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
			if (ret != 0) {
				dramc_crit("[dramc] blkdev_write failed\n");
				dram_offline_data_flags = ERR_BLKDEV_WRITE_FAIL;
				goto exit;
			}
		}

		dram_offline_data_flags = ERR_PL_UPDATED;
		goto exit;
	}

	/* check magic number */
	if (hdr.magic_number != DRAM_CALIBRATION_DATA_MAGIC) {
		dramc_crit("[dramc] magic number mismatch\n");
		dram_offline_data_flags = ERR_MAGIC_NUMBER;
		goto exit;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[shuffle]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_read %s failed\n", "data");
		dram_offline_data_flags = ERR_BLKDEV_READ_FAIL;
		goto exit;
	}

	/* check checksum */
	if (check_checksum_for_dram_data(&shu_data) != 1) {
		dramc_crit("[dramc] checksum failed\n");
		dram_offline_data_flags = ERR_CHECKSUM;
		goto exit;
	}

	/* copy the data stored in storage to the data structure for calibration */
	memcpy(offLine_SaveData, &(shu_data.calibration_data), sizeof(*offLine_SaveData));

exit:
	if (dram_offline_data_flags)
		SET_DRAM_STORAGE_API_ERR(dram_offline_data_flags, DRAM_STORAGE_API_READ);

	return 0 - dram_offline_data_flags;
}

int write_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	int ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	if (offLine_SaveData == NULL) {
		dramc_crit("[dramc] offLine_SaveData == NULL, skip\n");
		SET_DRAM_STORAGE_API_ERR(ERR_NULL_POINTER, DRAM_STORAGE_API_WRITE);
		return -ERR_NULL_POINTER;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find boot device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_WRITE);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memcpy(&(shu_data.calibration_data), offLine_SaveData, sizeof(*offLine_SaveData));
    
    /* assign PL version */
    hdr.pl_version = PL_VERSION;

    /* assign magic number */
    hdr.magic_number = DRAM_CALIBRATION_DATA_MAGIC;

    /* assign api error code */
    hdr.calib_err_code = g_dram_storage_api_err_code;

    ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }

    /* calculate and assign checksum */
    assign_checksum_for_dram_data(&shu_data);

    ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[shuffle]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }

    return 0;
}

int clean_dram_calibration_data(void)
{
	int i, ret;
	blkdev_t *bootdev = NULL;
	DRAM_CALIBRATION_HEADER_T hdr;
	DRAM_CALIBRATION_SHU_DATA_T shu_data;
	DRAM_CALIBRATION_DATA_T *datap = NULL;

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc] can't find block device(%d)\n", CFG_BOOT_DEV);
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_NOT_FOUND, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_NOT_FOUND;
	}

	if (!part_dram_data_addr) {
		return -ERR_BLKDEV_NO_PART;
	}

	memset(&hdr, 0, sizeof(hdr));

	ret = blkdev_write(bootdev, part_dram_data_addr, sizeof(hdr), (u8*)&hdr, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc] blkdev_write failed\n");
		SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_CLEAN);
		return -ERR_BLKDEV_WRITE_FAIL;
	}
    
    shu_data.checksum = 0;

    /* clear each shuffle */
    for (i = 0; i < DRAM_DFS_SRAM_MAX; i++) {
        ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->data[i]), sizeof(shu_data), (u8*)&shu_data, storage_get_part_id(STORAGE_PHYS_PART_USER));
        if (ret != 0) {
            dramc_crit("[dramc] blkdev_write failed\n");
            SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
            return -ERR_BLKDEV_WRITE_FAIL;
        }
    }

    return 0;
}

#else

DRAM_CALIBRATION_DATA_T dram_data; // using global variable to avoid stack overflow

static int read_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
    return -1;	
}
static int write_offline_dram_mdl_data(DRAM_INFO_BY_MRR_T *DramInfo)
{
   return -1;
}

int read_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	return 0;
}

int write_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	return 0;
}

int clean_dram_calibration_data(void)
{
	return;
}
#endif


void set_err_code_for_storage_api(void)
{
#ifdef LAST_DRAMC
	last_dramc_info_ptr->storage_api_err_flag = g_dram_storage_api_err_code;
	dsb();
#endif
}

#endif

#if defined(SLT) && (!__ETT__)
#include <storage_api.h>
#include <emi.h>
int clean_slt_data(void)
{
	DRAM_SLT_DATA_T data;
	data.header.stage_status = -1;
	data.header.pl_version = PL_VERSION;
	return write_slt_data(&data);
}

int read_slt_data(DRAM_SLT_DATA_T *data)
{
	int i, ret;
	blkdev_t *bootdev = NULL;

	if (data == NULL) {
		dramc_crit("[dramc_slt] SLT data == NULL, skip\n");
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc_slt] can't find boot device(%d)\n", CFG_BOOT_DEV);
		return SLT_ERR_NO_DEV;
	}

	if (!part_dram_data_addr_slt) {
		return SLT_ERR_NO_ADDR;
	}

	ret = blkdev_read(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));

	if (ret != 0) {
		return SLT_ERR_READ_FAIL;
	}

	if (data->header.pl_version != PL_VERSION) {
		dramc_crit("[dramc_slt] PL_VERSION mismatch\n");
		clean_slt_data();
		blkdev_read(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	}

	return 0;
}

int write_slt_data(DRAM_SLT_DATA_T *data)
{
	int ret;
	blkdev_t *bootdev = NULL;

	if (data == NULL) {
		dramc_crit("[dramc_slt] data == NULL, skip\n");
		return SLT_ERR_NO_DATA;
	}

	bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		dramc_crit("[dramc_slt] can't find boot device(%d)\n", CFG_BOOT_DEV);
		return SLT_ERR_NO_DEV;
	}

	if (!part_dram_data_addr_slt) {
		return SLT_ERR_NO_ADDR;
	}

	data->header.pl_version = PL_VERSION;

	ret = blkdev_write(bootdev, part_dram_data_addr_slt, sizeof(DRAM_SLT_DATA_T), (u8*)data, storage_get_part_id(STORAGE_PHYS_PART_USER));
	if (ret != 0) {
		dramc_crit("[dramc_slt] blkdev_write failed\n");
		return SLT_ERR_WRITE_FAIL;
	}

	return 0;
}
#endif

#if __FLASH_TOOL_DA__
unsigned int get_mr8_by_mrr(U8 channel, U8 rank)
{
	DRAMC_CTX_T *p = psCurrDramCtx;
	unsigned int mr8_value;

	p->channel = channel;

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), rank, SWCMD_CTRL0_MRRRK);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), 8, SWCMD_CTRL0_MRSMA);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_MRREN);
	while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRR_RESPONSE) ==0)
		mcDELAY_US(1);
	mr8_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRR_STATUS), MRR_STATUS_MRR_REG);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_MRREN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), 0, SWCMD_CTRL0_MRRRK);

	return (mr8_value & 0xff);
}
#endif

