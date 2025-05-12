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
#include "emi_hw.h"
#include "dramc_common.h"
#include "dramc_int_global.h"
#if !__ETT__
#include <emi-addr2dram.h>
#include "emi_mpu_mt.h"
#endif
unsigned int mt_get_dram_type_from_hw_trap(void);
#if !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
#include "dram_buffer.h"
#endif
#else
#include <barriers.h>
#endif

#if !__FLASH_TOOL_DA__ && !__ETT__
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

#include "emi.h"
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
 * MT6360 |Vdram |1.125v      |5mV    |Yes          |(DRAM Vdram)
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
#ifdef LAST_EMI
static LAST_EMI_INFO_T* last_emi_info_ptr;
#endif
#endif

#if !__ETT__
#include "sec_devinfo.h"
#endif
//=============================================================================
//  Definition
//=============================================================================

static unsigned int get_dramc_addr(dram_addr_t *dram_addr, unsigned int offset);
int doe_get_config(char* feature);

//=============================================================================
//  Global Variables
//=============================================================================
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

#ifdef LAST_EMI
static LAST_EMI_INFO_T* last_emi_info_ptr;
#endif

#ifdef VOLTAGE_SEL
static VOLTAGE_SEL_INFO_T voltage_sel_info_ptr;
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

//This is for Chip, Please don't remove it.
#define EMI_APB_BASE 0x10219000
#define EMI_MPU_APB_BASE 0x10226000
#define SUB_EMI_MPU_APB_BASE 0x10225000
#define SUB_EMI_APB_BASE 0x1021D000
#define EMI_CHANNEL_APB_BASE 0x10235000
#define CHN0_EMI_APB_BASE 0x10235000
#define CHN2_EMI_APB_BASE 0x10245000
#define CHN1_EMI_APB_BASE 0x10255000
#define CHN3_EMI_APB_BASE 0x10265000
#define MCUSYS_PAR_WRAP_BASE 0x0C530000
//#define INFRACFG_AO_BASE 0x10001000
#define INFRACFG_AO_MEM_BASE 0x10002000
#define SUB_INFRACFG_AO_MEM_BASE 0x1030E000

void mt_emi_sync_write(unsigned int addr, unsigned int value)
{
                *((volatile unsigned int *)addr) = value;
                dsb();
}
 
unsigned int mt_emi_sync_read(unsigned int addr)
{
                dsb();
                return *((volatile unsigned int *)addr);
}
 
void mt_emi_sync_write_or(unsigned int addr, unsigned int or_value)
{
                unsigned int value = mt_emi_sync_read(addr);
                mt_emi_sync_write(addr, value | or_value);
}

void EMI_ESL_Setting1(void)
{
  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast

  mt_emi_sync_write(    EMI_APB_BASE+0x000007a4, 0xc0000000); // config north emi for 2+2CH
  mt_emi_sync_write(SUB_EMI_APB_BASE+0x000007a4, 0xd0000000); // config south emi for 2+2CH

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
//C:-------------------BEGIN))= EMI Setting--------------------
//))= Row = 15-bit
#ifndef ONE_CH
  #ifdef RANK_512MB  // => 2channel , dual rank , total=2G
    mt_emi_sync_write(EMI_APB_BASE+0x00000000,0xa053a154); // 2CH
  #else  //RANK_1G  => 2channel , dual rank , total=4G
    mt_emi_sync_write(EMI_APB_BASE+0x00000000,0xf053f154); // 2CH
  #endif
#else
  #ifdef RANK_512MB  
    mt_emi_sync_write(EMI_APB_BASE+0x00000000,0xa053a054); // 1CH
  #else
    mt_emi_sync_write(EMI_APB_BASE+0x00000000,0xf053f054); // 1CH
  #endif
#endif

  mt_emi_sync_write(EMI_APB_BASE+0x00000004,0x182e2d33); //4266 (1:8) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000008,0x0f251025); //4266 (1:8) r8  - r5  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000000c,0x122a1027); //4266 (1:8) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000010,0x1a31162d); //4266 (1:8) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000008b0,0x182e2d33); //3733 (1:8) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000008b4,0x0f251025); //3733 (1:8) r8  - r5  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000001c,0x122a1027); //3733 (1:8) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000024,0x1a31162d); //3733 (1:8) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000034,0x182e2d33); //3200 (1:8) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000006c,0x0f251025); //3200 (1:8) r8  - r5  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000013c,0x122a1027); //3200 (1:8) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000640,0x1a31162d); //3200 (1:8) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000044,0x1024202c); //2400 (1:8) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000074,0x0b210c21); //2400 (1:8) r8  - r5  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000001e0,0x0f250d23); //2400 (1:8) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000644,0x152b1228); //2400 (1:8) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000004c,0x0c201a28); //1866 (1:8) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000084,0x0d230a20); //1866 (1:8) r8  - r5  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000001e4,0x0e260d24); //1866 (1:8) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000648,0x132d1229); //1866 (1:8) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000054,0x0c201a28); //1600 (1:8) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000008c,0x0d230a20); //1600 (1:8) r8  - r5  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000001e8,0x0e260d24); //1600 (1:8) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000064c,0x132d1229); //1600 (1:8) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000005c,0x0c201a28); //1200 (1:8) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000094,0x0d230a20); //1200 (1:8) r8  - r5  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000001c8,0x0e260d24); //1200 (1:8) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000660,0x132d1229); //1200 (1:8) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000064,0x0e290e28); //800  (1:4) r12 - r9  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000009c,0x091e1322); //800  (1:4) r4  - r1  overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000001f4,0x0f29112a); //800  (1:4) r16 - r13 overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000664,0x0c240a1f); //800  (1:4) r8  - r5  overhead

  mt_emi_sync_write(EMI_APB_BASE+0x00000030,0x37373a57); //4266 (1:8) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000014,0x3f3f3c39); //4266 (1:8) r16 - r10 non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000008b8,0x37373a57); //3733 (1:8) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000002c,0x3f3f3c39); //3733 (1:8) r16 - r10 non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000000c4,0x3836374e); //3200 (1:8) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000668,0x41413d3a); //3200 (1:8) r16 - r10 non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000000c8,0x33313241); //2400 (1:8) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000066c,0x3a3a3835); //2400 (1:8) r16 - r10 non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000000cc,0x34343542); //1866 (1:8) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000694,0x3b3b3835); //1866 (1:8) r16 - r10 non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000000e4,0x34343542); //1600 (1:8) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000708,0x3b3b3835); //1600 (1:8) r16 - r10 non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x000000f4,0x34343542); //1200 (1:8) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000070c,0x3b3b3835); //1200 (1:8) r16 - r10 non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x0000012c,0x37333034); //800  (1:4) r8  - r2  non-align overhead
  mt_emi_sync_write(EMI_APB_BASE+0x00000748,0x39393a39); //800  (1:4) r16 - r10 non-align overhead

  mt_emi_sync_write(EMI_APB_BASE+0x00000018,0x3657587a);
  mt_emi_sync_write(EMI_APB_BASE+0x00000020,0x0000c042);
  mt_emi_sync_write(EMI_APB_BASE+0x00000028,0x08421000);
  mt_emi_sync_write(EMI_APB_BASE+0x00000038,0x00000083);
  mt_emi_sync_write(EMI_APB_BASE+0x0000003c,0x00073210);
  mt_emi_sync_write(EMI_APB_BASE+0x00000040,0x00008802);
  mt_emi_sync_write(EMI_APB_BASE+0x00000048,0x00000000);
  mt_emi_sync_write(EMI_APB_BASE+0x00000060,0x007812ff); // reserved buffer to normal read/write :8/7
  mt_emi_sync_write(EMI_APB_BASE+0x00000068,0x00000000);
  mt_emi_sync_write(EMI_APB_BASE+0x00000078,0x11120c1f);  //22:20=ultra_w=1
  mt_emi_sync_write(EMI_APB_BASE+0x00000710,0x11120c1f);  //22:20=ultra_w=1
  mt_emi_sync_write(EMI_APB_BASE+0x0000007c,0x00001123);
  mt_emi_sync_write(EMI_APB_BASE+0x00000718,0x00001123);
  mt_emi_sync_write(EMI_APB_BASE+0x000000d0,0xa8a8a8a8);
  mt_emi_sync_write(EMI_APB_BASE+0x000000d4,0x25252525);
  mt_emi_sync_write(EMI_APB_BASE+0x000000d8,0xa8a8a8a8);
  mt_emi_sync_write(EMI_APB_BASE+0x000000dc,0x25252525);
  mt_emi_sync_write(EMI_APB_BASE+0x000000e8,0x00060037); // initial starvation counter div2, [4]=1
  mt_emi_sync_write(EMI_APB_BASE+0x000000f0,0x384a0034);
  mt_emi_sync_write(EMI_APB_BASE+0x000000f8,0xa0000000);
  mt_emi_sync_write(EMI_APB_BASE+0x00000100,0x20107244);
  mt_emi_sync_write(EMI_APB_BASE+0x00000108,0x10107044);
  mt_emi_sync_write(EMI_APB_BASE+0x00000110,0x343450df);
  mt_emi_sync_write(EMI_APB_BASE+0x00000118,0x0000f0d0);
  mt_emi_sync_write(EMI_APB_BASE+0x00000120,0x10106048);
  mt_emi_sync_write(EMI_APB_BASE+0x00000128,0x343450df);
  mt_emi_sync_write(EMI_APB_BASE+0x00000130,0x83837044);
  mt_emi_sync_write(EMI_APB_BASE+0x00000138,0x83837044);
  mt_emi_sync_write(EMI_APB_BASE+0x00000140,0x00007108);
  mt_emi_sync_write(EMI_APB_BASE+0x00000144,0x00007108);
  mt_emi_sync_write(EMI_APB_BASE+0x00000150,0x090a0000);
  mt_emi_sync_write(EMI_APB_BASE+0x00000158,0xff0bff00);
  mt_emi_sync_write(EMI_APB_BASE+0x00000400,0x00ff0001); //[27:20] enable monitor
  mt_emi_sync_write(EMI_APB_BASE+0x0000071c,0x10000008);
  mt_emi_sync_write(EMI_APB_BASE+0x00000800,0x77777777);
  mt_emi_sync_write(EMI_APB_BASE+0x00000808,0x0a0a0a0a);
  mt_emi_sync_write(EMI_APB_BASE+0x0000080c,0x0a0a0a0a);
  mt_emi_sync_write(EMI_APB_BASE+0x00000820,0x24240101); //wostd throttle
  mt_emi_sync_write(EMI_APB_BASE+0x00000824,0x01012424); //wostd throttle
  mt_emi_sync_write(EMI_APB_BASE+0x00000828,0x50500101); //rostd throttle
  mt_emi_sync_write(EMI_APB_BASE+0x0000082c,0x01015050); //rostd throttle
  mt_emi_sync_write(EMI_APB_BASE+0x00000830,0x0fc39a70);
  mt_emi_sync_write(EMI_APB_BASE+0x00000834,0x05050003);
  mt_emi_sync_write(EMI_APB_BASE+0x00000838,0x254dffff);
  mt_emi_sync_write(EMI_APB_BASE+0x0000083c,0x5a788ca0); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000840,0x00000478); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000844,0x000003e8); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000848,0x0000036b); //update
  mt_emi_sync_write(EMI_APB_BASE+0x0000084c,0x00000290); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000850,0x00000000);
  mt_emi_sync_write(EMI_APB_BASE+0x00000854,0x00000000);
  mt_emi_sync_write(EMI_APB_BASE+0x00000858,0x02531cff); //ignore rff threshold
  mt_emi_sync_write(EMI_APB_BASE+0x0000085c,0x00002785); //disable internal MD latency urgent mask
  mt_emi_sync_write(EMI_APB_BASE+0x00000874,0x00000200); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000878,0x00460000); //update
  mt_emi_sync_write(EMI_APB_BASE+0x0000087c,0x0255250d);
  mt_emi_sync_write(EMI_APB_BASE+0x00000890,0xffff3c59);
  mt_emi_sync_write(EMI_APB_BASE+0x00000894,0xffff00ff);
  mt_emi_sync_write(EMI_APB_BASE+0x000008a0,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000008a4,0x0000ffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000008c0,0x000001b5); //update
  mt_emi_sync_write(EMI_APB_BASE+0x000008c4,0x003c0000); //update
  mt_emi_sync_write(EMI_APB_BASE+0x000008c8,0x0000014b); //update
  mt_emi_sync_write(EMI_APB_BASE+0x000008cc,0x002d0000); //update
  mt_emi_sync_write(EMI_APB_BASE+0x000008d0,0x00000185); //update
  mt_emi_sync_write(EMI_APB_BASE+0x000008d4,0x003c0000); //update
  mt_emi_sync_write(EMI_APB_BASE+0x000008e0,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000008e4,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000008e8,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000920,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000924,0x0000ffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000930,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000934,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000938,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000009f0,0x41547082);
  mt_emi_sync_write(EMI_APB_BASE+0x000009f4,0x38382a38);
  mt_emi_sync_write(EMI_APB_BASE+0x000009f8,0x00000217); //update
  mt_emi_sync_write(EMI_APB_BASE+0x000009fc,0x000001d4); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b00,0x00000190); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b04,0x0000012c); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b08,0x000000ed); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b0c,0x000000c8); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b10,0x00000096); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b14,0x000000c8); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b28,0x30404850); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b2c,0x20182026); //update

  mt_emi_sync_write(EMI_APB_BASE+0x00000b60,0x181e282f);
  mt_emi_sync_write(EMI_APB_BASE+0x00000b64,0x14140f18);
  mt_emi_sync_write(EMI_APB_BASE+0x00000b98,0x96c8e9ff); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000b9c,0x644b6474); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000bd0,0x01010101);
  mt_emi_sync_write(EMI_APB_BASE+0x00000bd4,0x01010101);
  mt_emi_sync_write(EMI_APB_BASE+0x00000c08,0x96c8e9ff); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c0c,0x644b6474); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c40,0x01010101);
  mt_emi_sync_write(EMI_APB_BASE+0x00000c44,0x01010101);
  mt_emi_sync_write(EMI_APB_BASE+0x00000c4c,0x300ff025);	//ignore wff threshold
  mt_emi_sync_write(EMI_APB_BASE+0x00000c80,0x00000478); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c84,0x000003e8); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c88,0x0000036b); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c8c,0x00000290); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c90,0x00000200); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c94,0x000001b5); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c98,0x0000014b); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000c9c,0x00000185); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000cb0,0x698ca0bb); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000cb4,0x46354652); //update
  mt_emi_sync_write(EMI_APB_BASE+0x00000cf8,0x01010101);
  mt_emi_sync_write(EMI_APB_BASE+0x00000cfc,0x01010101);

  mt_emi_sync_write(EMI_APB_BASE+0x00000d04,0x00000009); //MDR shf0 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d0c,0x00000000); //MDR shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d14,0x00730000); //MDR shf0
  mt_emi_sync_write(EMI_APB_BASE+0x00000d18,0x00000808); //MDR shf1
  mt_emi_sync_write(EMI_APB_BASE+0x00000d1c,0x00000028); //MDW shf0 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d24,0x00000000); //MDW shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d2c,0x00730000); //MDW shf0
  mt_emi_sync_write(EMI_APB_BASE+0x00000d30,0x00000808); //MDW shf1
  mt_emi_sync_write(EMI_APB_BASE+0x00000d34,0x00000080); //APR shf0 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d3c,0x00000000); //APR shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d44,0x30201008); //APR shf0/shf1
  mt_emi_sync_write(EMI_APB_BASE+0x00000d48,0x00000800); //APW shf0 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d50,0x00000000); //APW shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d58,0x00008000); //MMR shf0 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d60,0x00020000); //MMR shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d64,0x00001000); //MMR shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d68,0x00010000); //MMR shf2 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d6c,0x00000800); //MMR shf2 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d70,0x08080000); //MMR shf0
  mt_emi_sync_write(EMI_APB_BASE+0x00000d74,0x00073030); //MMR shf1
  mt_emi_sync_write(EMI_APB_BASE+0x00000d78,0x00040000); //MMW shf0 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d80,0x00100000); //MMW shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d84,0x00004000); //MMW shf1 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d88,0x00080000); //MMW shf2 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d8c,0x00002000); //MMW shf2 event selet
  mt_emi_sync_write(EMI_APB_BASE+0x00000d90,0x08080000); //MMW shf0
  mt_emi_sync_write(EMI_APB_BASE+0x00000d94,0x00074040); //MMW shf1
  mt_emi_sync_write(EMI_APB_BASE+0x00000d98,0x00400000); //MDHWR sh0 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000da0,0x00200000); //MDHWR sh1 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000da8,0x10100404); //MDHWWR sh
  mt_emi_sync_write(EMI_APB_BASE+0x00000dac,0x01000000); //MDHWW sh0 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000db4,0x00800000); //MDHWW sh1 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000dbc,0x04000000); //GPUR sh0 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000dc4,0x02000000); //GPUR sh1 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000dcc,0x60602010); //GPUR
  mt_emi_sync_write(EMI_APB_BASE+0x00000dd0,0x10000000); //GPUW sh0 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000dd8,0x08000000); //GPUW sh1 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000de0,0x00000009); //ARBR sh0 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000de8,0x04400080); //ARBR sh1 event select
  mt_emi_sync_write(EMI_APB_BASE+0x00000df0,0x0f170f11); //ARB
  mt_emi_sync_write(EMI_APB_BASE+0x00000df4,0x0303f7f7); //QOS control

  mt_emi_sync_write(EMI_APB_BASE+0x00000e04,0x00000166);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e08,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e0c,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e14,0x00400166);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e18,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e1c,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e24,0x00000266);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e28,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e2c,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e34,0x00400266);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e38,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000e3c,0xffffffff);

  // Added by Wei-Lun - START
  // prtcl chker - golden setting
  mt_emi_sync_write(EMI_APB_BASE+0x00000304,0xffffffff); // cyc
  mt_emi_sync_write(EMI_APB_BASE+0x0000030c,0x001ffc84); // ctl
  mt_emi_sync_write(EMI_APB_BASE+0x0000034c,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000354,0x001ffc84);
  mt_emi_sync_write(EMI_APB_BASE+0x0000039c,0x001ffc84);
  mt_emi_sync_write(EMI_APB_BASE+0x00000394,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000003d8,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000003dc,0x001ffc84);
  mt_emi_sync_write(EMI_APB_BASE+0x000003fc,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x0000040c,0x001ffc84);
  mt_emi_sync_write(EMI_APB_BASE+0x0000044c,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000454,0x001ffc84);
  mt_emi_sync_write(EMI_APB_BASE+0x0000049c,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x000004a4,0x001ffc84);
  mt_emi_sync_write(EMI_APB_BASE+0x0000050c,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000514,0x001ffc84);

  // maxbw monitor - golden setting
  mt_emi_sync_write(EMI_APB_BASE+0x00000960,0x3bfd24ae);
  mt_emi_sync_write(EMI_APB_BASE+0x00000964,0x00001818);
  mt_emi_sync_write(EMI_APB_BASE+0x00000968,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x0000096c,0x21087fff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000970,0xffffffff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000974,0x10080808);

  // enable dbw0 cnter for max bw correlation (m3/4 no filters are enabled)
  mt_emi_sync_write(EMI_APB_BASE+0x00000f00,0x0000181f);
  mt_emi_sync_write(EMI_APB_BASE+0x00000f2c,0xf000f000);
  // Added by Wei-Lun - END
  //
  // Added by Fumin - START
  mt_emi_sync_write(EMI_APB_BASE+0x0000014c,0xc0000100);
  mt_emi_sync_write(EMI_APB_BASE+0x00000990,0x00000fff);
  mt_emi_sync_write(EMI_APB_BASE+0x00000994,0x00000000);
  // Added by Fumin - END

//C:-------------------END))= EMI Setting--------------------;
//C:-------------------BEGIN))= CHANNEL EMI Setting--------------------;

#ifdef RANK_512MB  // => 2channel , dual rank , total=2G
  #ifdef BANK_INTERLEAVE_ON
    mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000000,0x2400a051);
  #else
    mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000000,0x0400a051);
  #endif
#else  //RANK_1G  => 2channel , dual rank , total=4G
  #ifdef BANK_INTERLEAVE_ON
    mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000000,0x2400f051);
  #else
    mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000000,0x0400f051);
  #endif
#endif

  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000008,0x00ff6048);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000010,0x00000004);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000018,0x99f08c03);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000710,0x9a508c17); // [24:20] = 0x2 : bank throttling (default=0x01f00000)
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000048,0x00038137); //RD_INORDER_THR[20:16]=2
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000050,0x38460002); // [1] : MD_RD_AFT_WR_EN
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000058,0x00000000);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000090,0x000002ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000098,0x00003111);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000140,0x22607188);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000144,0x22607188);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000148,0x3719595e);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000014c,0x2719595e);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000150,0x64f3fc79);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000154,0x64f3fc79);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000158,0x011b0868);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000015c,0x884102a2); // Stop urgent read first when write command buffer remain < 8
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000016c,0x0000f801);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000170,0x40000000);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b0,0x000c802f); // Rank-Aware arbitration
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b4,0xbd3f3f7e); // Rank-Aware arbitration
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b8,0x7e003d7e); // Rank-Aware arbitration
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000003fc,0x00000000); // Write M17_toggle_mask = 0
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000080,0x990188ff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000088,0x9961acff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000404,0xaa41beff);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x0000040c,0x9f650633);

//C:-------------------END))= CHANNEL EMI Setting--------------------;
  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
}

void EMI_ESL_Setting2(void)
{
  unsigned int emi_temp_data;

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH

  // CHN_EMI enable after DRAMC setting done
  mt_emi_sync_write_or(EMI_CHANNEL_APB_BASE+0x00000010,0x00000001); // [0] EMI enable
  // CEN_EMI enable after CHN_EMI setting done
  mt_emi_sync_write_or(EMI_APB_BASE+0x00000060,0x00000400); // [26] Disable read data DCM, [10] EMI enable
  // EMI_MPU_REG interrupt workaround for mt6885
  mt_emi_sync_write_or(EMI_MPU_APB_BASE+0x00000000,0x00000010); // [4] Disable emi_mpu_reg interrupt
  
  // Clear rank_arb_en
  emi_temp_data = mt_emi_sync_read(EMI_CHANNEL_APB_BASE+0x000001b0); // read ch0
  emi_temp_data = emi_temp_data & ~(0x1);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x000001b0, emi_temp_data); // broadcast to all channel
  // auto-config rank_arb_en according to dual_rank_en setting
  // assume all channel with same configuration
  emi_temp_data = mt_emi_sync_read(EMI_CHANNEL_APB_BASE+0x00000000); // read ch0
  emi_temp_data = emi_temp_data & 0x1;
  mt_emi_sync_write_or(EMI_CHANNEL_APB_BASE+0x000001b0, emi_temp_data); // broadcast to all channel

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast

  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000028, 0x0000002f); // INFRA_EMI_DCM_CFG0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000030, 0x001F0044); // INFRA_EMI_DCM_CFG2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000038, 0xA00000ff); // TOP_CK_ANCHOR_CFG
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000028, 0x0005007f); // SUB_INFRA_EMI_DCM_CFG0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x0000002c, 0x40000000); // SUB_INFRA_EMI_DCM_CFG1
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000030, 0x001F0044); // SUB_INFRA_EMI_DCM_CFG2

  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000100, 0xffffffff); // INFRA_EMI_IDLE_BIT_EN_0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000104, 0xffffffff); // INFRA_EMI_IDLE_BIT_EN_1
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000108, 0xffffffff); // INFRA_EMI_IDLE_BIT_EN_2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x0000010c, 0xffffffff); // INFRA_EMI_IDLE_BIT_EN_3
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000110, 0x01f00000); // INFRA_EMI_M0M1_IDLE_BIT_EN_0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000114, 0xc0040180); // INFRA_EMI_M0M1_IDLE_BIT_EN_1
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000118, 0x00000000); // INFRA_EMI_M0M1_IDLE_BIT_EN_2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x0000011c, 0x00000003); // INFRA_EMI_M0M1_IDLE_BIT_EN_3
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000120, 0x0c000000); // INFRA_EMI_M2M5_IDLE_BIT_EN_0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000124, 0x00c00000); // INFRA_EMI_M2M5_IDLE_BIT_EN_1
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000128, 0x01f08000); // INFRA_EMI_M2M5_IDLE_BIT_EN_2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x0000012c, 0x00000000); // INFRA_EMI_M2M5_IDLE_BIT_EN_3
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000130, 0x20003040); // INFRA_EMI_M3_IDLE_BIT_EN_0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000134, 0x00000000); // INFRA_EMI_M3_IDLE_BIT_EN_1
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000138, 0x00001000); // INFRA_EMI_M3_IDLE_BIT_EN_2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x0000013c, 0x00000000); // INFRA_EMI_M3_IDLE_BIT_EN_3
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000140, 0x10020f20); // INFRA_EMI_M4_IDLE_BIT_EN_0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000144, 0x00019000); // INFRA_EMI_M4_IDLE_BIT_EN_1
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000148, 0x040a0818); // INFRA_EMI_M4_IDLE_BIT_EN_2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x0000014c, 0x00000370); // INFRA_EMI_M4_IDLE_BIT_EN_3
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000150, 0xc001c080); // INFRA_EMI_M6M7_IDLE_BIT_EN_0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000154, 0x33000e01); // INFRA_EMI_M6M7_IDLE_BIT_EN_1
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000158, 0x180067e1); // INFRA_EMI_M6M7_IDLE_BIT_EN_2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x0000015c, 0x0000008c); // INFRA_EMI_M6M7_IDLE_BIT_EN_3
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000160, 0x020c0008); // INFRA_EMI_SRAM_IDLE_BIT_EN_0
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000164, 0x0c00007e); // INFRA_EMI_SRAM_IDLE_BIT_EN_1
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x00000168, 0x80050006); // INFRA_EMI_SRAM_IDLE_BIT_EN_2
  mt_emi_sync_write(INFRACFG_AO_MEM_BASE+0x0000016c, 0x00000000); // INFRA_EMI_SRAM_IDLE_BIT_EN_3

  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000020, 0xffffffff); // SUB_INFRA_EMI_IDLE_BIT_EN_0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000024, 0xffffffff); // SUB_INFRA_EMI_IDLE_BIT_EN_1
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000210, 0x00000000); // SUB_INFRA_EMI_M0M1_IDLE_BIT_EN_0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000214, 0x06007fe0); // SUB_INFRA_EMI_M0M1_IDLE_BIT_EN_1
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000218, 0x0000003f); // SUB_INFRA_EMI_M2M5_IDLE_BIT_EN_0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x0000021c, 0x0003000c); // SUB_INFRA_EMI_M2M5_IDLE_BIT_EN_1
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000220, 0x200003c0); // SUB_INFRA_EMI_M3_IDLE_BIT_EN_0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000224, 0x00000000); // SUB_INFRA_EMI_M3_IDLE_BIT_EN_1
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000228, 0x1000fc00); // SUB_INFRA_EMI_M4_IDLE_BIT_EN_0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x0000022c, 0x01000000); // SUB_INFRA_EMI_M4_IDLE_BIT_EN_1
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000230, 0xcfff0000); // SUB_INFRA_EMI_M6M7_IDLE_BIT_EN_0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000234, 0x30008013); // SUB_INFRA_EMI_M6M7_IDLE_BIT_EN_1
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x00000238, 0x00000000); // SUB_INFRA_EMI_VPU_IDLE_BIT_EN_0
  mt_emi_sync_write(SUB_INFRACFG_AO_MEM_BASE+0x0000023c, 0x00000000); // SUB_INFRA_EMI_VPU_IDLE_BIT_EN_1

	mt_emi_sync_write_or(INFRACFG_AO_BASE+0x00000078, 0x08000000); // enable infra_local_cg

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH

  // Enable rdata_prty_gen & wdata_prty_chk
  mt_emi_sync_write_or(EMI_APB_BASE+0x00000068,0x00400000); // enable cen_emi parity

  // emi bus parity workaround
  emi_temp_data = mt_emi_sync_read(0x40000000);
  mt_emi_sync_write(0x40000000, emi_temp_data);
  emi_temp_data = mt_emi_sync_read(0x40000100);
  mt_emi_sync_write(0x40000100, emi_temp_data);
  emi_temp_data = mt_emi_sync_read(0x40000200);
  mt_emi_sync_write(0x40000200, emi_temp_data);
  emi_temp_data = mt_emi_sync_read(0x40000300);
  mt_emi_sync_write(0x40000300, emi_temp_data);

  mt_emi_sync_write_or(EMI_CHANNEL_APB_BASE+0x00000050,0x00000004); // enable chn_emi parity

  // Enable APMCU Early CKE
  // set reg_chn_en
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >>  6) & (0x1<<2);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_chn_pos
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data <<  2) & (0x3<<4);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_chn_loc
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >>  4) & (0x1<<6);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_dual_rank_en
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >> 10) & (0x1<<7);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_cas_size[1:0]
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >>  9) & (0x3<<9);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_cas_size[2]
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data << 11) & (0x1<<11);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_cas_size[3]
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >> 14) & (0x1<<12);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_remap_shift
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, 0x00006000);
  // set reg_rank_dec0
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000038);
  emi_temp_data = (emi_temp_data <<  2) & (0x1f<<18);
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, emi_temp_data);
  // set reg_rank_cke_ext and reg_enable
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007f4, 0x11000001);
  // set to mcusys
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x000007f4);
  mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0000a490, emi_temp_data);
  mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0000a494, emi_temp_data);
  mt_emi_sync_write(EMI_CHANNEL_APB_BASE+0x00000090, 0x00000200);

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast

  // Enable EBG
  emi_temp_data = mt_emi_sync_read(INFRACFG_AO_MEM_BASE+0x00000050);
  if (emi_temp_data & 0x01ff0000)
  	emi_temp_data = emi_temp_data >> 16;
  emi_temp_data &= 0xf;
  mt_emi_sync_write_or(EMI_APB_BASE+0x000007a4, emi_temp_data);
  // set addr_decode_0
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x000007c0);
  mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0000a498, emi_temp_data);
  // set addr_decode_1
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x000007c4);
  mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0000a49c, emi_temp_data);
  // set addr_decode_2
  // set reg_chn_row[1:0]
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >> 12) & (0x3<<0);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set reg_chn_row[2]
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >> 22) & (0x1<<2);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set reg_chn_col_2nd
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >>  3) & (0x3<<3);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set reg_chn_row_2nd[1:0]
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >>  9) & (0x3<<5);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set reg_chn_row_2nd[2]
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >> 18) & (0x1<<7);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set reg_dual_rank_en
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >>  9) & (0x1<<8);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set reg_rank_pos
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000000);
  emi_temp_data = (emi_temp_data >> 18) & (0x1<<9);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set reg_rank_decode
  emi_temp_data = mt_emi_sync_read(EMI_APB_BASE+0x00000038);
  emi_temp_data = (emi_temp_data >>  5) & (0x1f<<11);
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, emi_temp_data);
  // set EBG enable
  mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, 0x00010000);

  //emi_temp_data = mt_emi_sync_read(SUB_EMI_APB_BASE+0x000007c0);
  //mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0000a4a4, emi_temp_data);
  //emi_temp_data = mt_emi_sync_read(SUB_EMI_APB_BASE+0x000007c4);
  //mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0000a4a8, emi_temp_data);
  //mt_emi_sync_write(MCUSYS_PAR_WRAP_BASE+0x0000a4ac, 0x0000418c);
  //mt_emi_sync_write_or(MCUSYS_PAR_WRAP_BASE+0x0000a4a0, 0x00010000);

  mt_emi_sync_write(CHN0_EMI_APB_BASE+0x00000020, 0x3fff02c4);
  mt_emi_sync_write(CHN2_EMI_APB_BASE+0x00000020, 0x3fff02c4);
  mt_emi_sync_write(CHN1_EMI_APB_BASE+0x00000020, 0x3fff02c4);
  mt_emi_sync_write(CHN3_EMI_APB_BASE+0x00000020, 0x3fff02c4);

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH

  mt_emi_sync_write_or(EMI_APB_BASE+0x00000068,0x00200000); // enable cen_emi wdata bus encode
  mt_emi_sync_write_or(EMI_CHANNEL_APB_BASE+0x00000050,0x00000010); // enable chn_emi wdata bus encode

  mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast

}

void EMI_Patch(void)
{

	/* Enable MPU violation interrupt to MD for D1 and D7 */
	*((volatile unsigned int *)EMI_MPU_CTRL_D(1)) |= 0x10;
	*((volatile unsigned int *)EMI_MPU_CTRL_D(7)) |= 0x10;
#if defined(SUB_EMI_MPU_BASE)
	*((volatile unsigned int *)SUB_EMI_MPU_CTRL_D(1)) |= 0x10;
	*((volatile unsigned int *)SUB_EMI_MPU_CTRL_D(7)) |= 0x10;
#endif

	/* for DVFS BW monitor */
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
	*((volatile unsigned int *) EMI_BWCT0) = 0x05000305;
	*((volatile unsigned int *) EMI_BWCT0_6TH) = 0x08FF0705;
	*((volatile unsigned int *) EMI_BWCT0_3RD) = 0x0DFF0A05;
	*((volatile unsigned int *) EMI_BWCT0_4TH) = 0x7FFF0F05;
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast

#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	last_emi_info_ptr->decs_magic = LAST_EMI_MAGIC_PATTERN;
	last_emi_info_ptr->isu_version = 0xFFFFFFFF;
#if CFG_LAST_EMI_BW_DUMP
	last_emi_info_ptr->decs_ctrl = 0xDECDDECD;
#else
	last_emi_info_ptr->decs_ctrl = 0;
#endif
	last_emi_info_ptr->decs_dram_type = 0;
	last_emi_info_ptr->decs_diff_us = 0;
	last_emi_info_ptr->mbw_buf_l = 0;
	last_emi_info_ptr->mbw_buf_h = 0;
#endif
}

void EMI_Init(DRAMC_CTX_T *p)
{
	EMI_SETTINGS *emi_set = &g_default_emi_setting;

#ifdef MTK_DOE_CONFIG_ENV_SUPPORT /* from platform/mtxxxx/feature.mak */
	unsigned int emi_mpu_slverr = 0;
	unsigned int emi_dcm = 0;
	unsigned int domain = 0;
	char *str;
#endif

	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
	EMI_ESL_Setting1(); //Copy Paste from DE
	
	dsb();
	
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
	
	//The following is MDL settings
	*(volatile unsigned *)EMI_CONA = emi_set->EMI_CONA_VAL;
	*(volatile unsigned *)EMI_CONH = emi_set->EMI_CONH_VAL;

	// CHNA and CHNB uses the same CH0 setting
	*(volatile unsigned *)CHN_EMI_CONA(CHN0_EMI_APB_BASE) = emi_set->CHN0_EMI_CONA_VAL;
	//*(volatile unsigned *)CHN_EMI_CONA(CHN1_EMI_APB_BASE) = emi_set->CHN1_EMI_CONA_VAL;

	*(volatile unsigned *)EMI_CONF = emi_set->EMI_CONF_VAL;
	//*(volatile unsigned *)CHN_EMI_CONC(CHN0_EMI_APB_BASE) = 0x4;
	//*(volatile unsigned *)CHN_EMI_CONC(CHN1_EMI_APB_BASE) = 0x4;

	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
	dsb();

#ifdef MTK_DOE_CONFIG_ENV_SUPPORT /* from platform/mtxxxx/feature.mak */
	/* EMI doeapp for MPU slaver error */
	str = dconfig_getenv("emi_mpu_slverr");
	emi_mpu_slverr = (str) ? atoi(str) : 0;

	if (emi_mpu_slverr == 1) {
		for (domain = 0; domain < EMI_MPU_DOMAIN_NUM; domain++) {
			*((volatile unsigned int *)EMI_MPU_CTRL_D(domain)) |= 0x2;
			print("EMI SLVERR ENABLED\n");
		}
	} else if (emi_mpu_slverr == 2) {
		for (domain = 0; domain < EMI_MPU_DOMAIN_NUM; domain++) {
			*((volatile unsigned int *)EMI_MPU_CTRL_D(domain)) &= ~0x2;
			print("EMI SLVERR DISABLED\n");
		}
	}
	/* EMI doeapp for DCM */
	str = dconfig_getenv("emi_dcm");
	emi_dcm = (str) ? atoi(str) : 0;

        if (emi_dcm == 1) {
                *((volatile unsigned int *)EMI_CONM) &= ~0xFF000000;//Enable DCM
                *((volatile unsigned int *)EMI_CONN) &= ~0xFF000000;//Enable DCM2
		print("EMI DCM ENABLED\n");
        } else if (emi_dcm == 2) {
                *((volatile unsigned int *)EMI_CONM) |= 0xFF000000;
                *((volatile unsigned int *)EMI_CONN) |= 0xFF000000;
		print("EMI DCM DISABLED\n");
        }
#endif

	p->vendor_id = emi_set->iLPDDR3_MODE_REG_5;
}

void EMI_Init2(void)
{
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
	EMI_ESL_Setting2(); //Copy Paste from DE
#if !__ETT__
	emi_addr2dram_disph(14);// Set dispatch for 4-channels
#endif
	EMI_Patch(); //Please add the EMI patch here
}

static unsigned int emi_conh = 0;
static unsigned int chn_emi_cona[2] = {0, 0};

/* return the start address of rank1 */
unsigned int set_emi_before_rank1_mem_test(void)
{
	unsigned int emi_cona;

	emi_cona = *(volatile unsigned int*)EMI_CONA;
	emi_conh = *(volatile unsigned int*)EMI_CONH;
	chn_emi_cona[0] = *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_APB_BASE);
	chn_emi_cona[1] = *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_APB_BASE);

	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH

	if (emi_cona & 0x100) {
		/* set all the rank size of all the channels to the minimum value */
		*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x11110000));

		/* set all the rank size of all channel-0 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_APB_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_APB_BASE) & 0xff00ffff ) | (0x00110000));

		/* set all the rank size of all channel-1 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_APB_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_APB_BASE) & 0xff00ffff ) | (0x00110000));
	} else {
		/* set all the rank size of all the channels to the minimum value */
		*(volatile unsigned int*)EMI_CONH = ((*(volatile unsigned int*)EMI_CONH & 0x0000ffff) | (0x22220000));

		/* set all the rank size of all channel-0 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_APB_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_APB_BASE) & 0xff00ffff ) | (0x00220000));

		/* set all the rank size of all channel-1 to the minimum value */
		*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_APB_BASE)
			= ((*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_APB_BASE) & 0xff00ffff ) | (0x00220000));
	}

	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
	dsb();
	return 0x40000000;
}

void restore_emi_after_rank1_mem_test(void)
{
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
	*(volatile unsigned int*)EMI_CONH = emi_conh;
	*(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_APB_BASE) = chn_emi_cona[0];
	*(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_APB_BASE) = chn_emi_cona[1];
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
	dsb();
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

    mcSHOW_DBG_MSG(("EMI_CONA=%x\n",*(volatile unsigned *)(EMI_APB_BASE+0x00000000)));
    mcSHOW_DBG_MSG(("EMI_CONH=%x\n",*(volatile unsigned *)(EMI_APB_BASE+0x00000038)));

    //RISCReadAll();
#endif
}

int mt_get_dram_type(void)
{
#if 1
	unsigned int dtype = mt_get_dram_type_from_hw_trap();

	if (dtype == TYPE_LPDDR4X)
		return DTYPE_LPDDR4X;
	else
		 ASSERT(0);

	return 0;
#else
#if (fcFOR_CHIP_ID == fcPetrus)
    return (*((volatile unsigned *)(Channel_A_DRAMC_AO_BASE_ADDRESS+0x10)) >> 10) & 0x7;
#else
    #error No defined mt_get_dram_type for your chip !!!
#endif
#endif
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
	pmic_config_interface(PMIC_RG_VIO18_VOCAL_ADDR, twist, PMIC_RG_VIO18_VOCAL_MASK, PMIC_RG_VIO18_VOCAL_SHIFT);
	return mtk_regulator_set_voltage(&reg_vio18, vio18, MAX_VIO18);
#else
	return 0;
#endif
}

unsigned int dramc_get_vio18_voltage(void)
{
#ifdef MTK_PMIC_MT6359
	unsigned int twist = 0;
	pmic_read_interface(PMIC_RG_VIO18_VOCAL_ADDR, &twist, PMIC_RG_VIO18_VOCAL_MASK, PMIC_RG_VIO18_VOCAL_SHIFT);
	return mtk_regulator_get_voltage(&reg_vio18) + twist * UNIT_VIO18;
#else
	return 0;
#endif
}

unsigned int mt_get_dram_type_from_hw_trap(void)
{
	unsigned int trap = get_dram_type() & 0x7;
	switch (trap) {
		case 0:
		case 1:
		//case 2:
		//case 3:
			return TYPE_LPDDR4X;
		//case 4:
		//case 5:
		//	return TYPE_LPDDR3;
		//case 6:
		//	return TYPE_LPDDR4;
		default:
			printf("[dramc] Wrond HW Trapping.\n");
			ASSERT(0);
			break;
	}
	return 0;

}

void setup_dramc_voltage_by_pmic(void)
{
#ifdef VOLTAGE_SEL
	int vcore;
#endif
	int efuse;

#if !__ETT__
	efuse = seclib_get_devinfo_with_index(21);
	if (efuse > 2)
		efuse = 2;
#else
	efuse = 0;
#endif
#ifdef MTK_PMIC_MT6359
	int ret;

	ret = mtk_regulator_get("vio18", &reg_vio18);
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
	mtk_regulator_set_mode(&reg_vddq, 0x1);
	mtk_regulator_set_mode(&reg_vmddr, 0x1);

	#ifdef VOLTAGE_SEL
	dramc_set_vio18_voltage(vio18_voltage_select());
	#else
	dramc_set_vio18_voltage(SEL_VIO18);
	#endif
#if defined(VCORE_BIN)
	#ifdef VOLTAGE_SEL
	vcore = vcore_voltage_select(KSHU0, efuse);
	if (vcore)
		dramc_set_vcore_voltage(vcore);
	else
	#endif
	dramc_set_vcore_voltage(get_vcore_uv_table(0));
#else
	#ifdef VOLTAGE_SEL
	dramc_set_vcore_voltage(vcore_voltage_select(KSHU0, efuse));
	#else
	if (efuse == 1)
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0, 25));
	else if (efuse == 2)
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0, 50));
	else
		dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0, 0));
		
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
	int efuse;

#if !__ETT__
	efuse = seclib_get_devinfo_with_index(21);
	if (efuse > 2)
		efuse = 2;
#else
	efuse = 0;
#endif
#ifdef MTK_PMIC_MT6359
	int ret;

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		printf("mtk_regulator_get vcore fail\n");

#if defined(SLT)
	dramc_set_vcore_voltage(725000);  //2400@0.725V from CK Tseng
#else
#if defined(VCORE_BIN)
	#ifdef VOLTAGE_SEL
	vcore = vcore_voltage_select(KSHU0, efuse);
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
	dramc_set_vcore_voltage(vcore_voltage_select(KSHU0, efuse));
	#else
        if (efuse == 1)
                dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0, 25));
        else if (efuse == 2)
                dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0, 50));
        else
                dramc_set_vcore_voltage(SEL_PREFIX_VCORE(LP4, KSHU0, 0));
	#endif
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
	
	ret = mtk_regulator_get("vio18", &reg_vio18);
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
	int force_pwm_vcore = doe_get_config("force_pwm_vcore");
	int force_pwm_vdram = doe_get_config("force_pwm_vdram");
	int force_pwm_vddq = doe_get_config("force_pwm_vddq");
	int force_pwm_vmddr = doe_get_config("force_pwm_vmddr");

	if (!force_pwm_vcore) {
		mtk_regulator_set_mode(&reg_vcore, 0x0);
		dramc_debug("Vcore auto mode\n");
	} else {
		dramc_debug("Vcore PWM mode\n");
	}

	if (!force_pwm_vdram) {
		mtk_regulator_set_mode(&reg_vdram, 0x0);
		dramc_debug("Vdram auto mode\n");
	} else {
		dramc_debug("Vdram PWM mode\n");
	}

	if (!force_pwm_vddq) {
		mtk_regulator_set_mode(&reg_vddq, 0x0);
		dramc_debug("Vddq auto mode\n");
	} else {
		dramc_debug("Vddq PWM mode\n");
	}

	if (!force_pwm_vmddr) {
		mtk_regulator_set_mode(&reg_vmddr, 0x0);
		dramc_debug("Vmddr auto mode\n");
	} else {
		dramc_debug("Vmddr PWM mode\n");
	}
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
    dramc_crit("[DDR reserve] EMI CONA: %x\n", *(volatile unsigned int*)EMI_CONA);
    dramc_crit("[DDR reserve] EMI CHA CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN0_EMI_APB_BASE));
    dramc_crit("[DDR reserve] EMI CHB CONA: %x\n", *(volatile unsigned int*)CHN_EMI_CONA(CHN1_EMI_APB_BASE));
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
		dramc_debug("[DRAMC] %s(%d),%s(%x),%s(%x),%s(0x%llx),%s(0x%llx)\n",
			"qvl", i,
			"type", qvl_list[i].type,
			"mr5", qvl_list[i].iLPDDR3_MODE_REG_5,
			"rank0_size", qvl_list[i].DRAM_RANK_SIZE[0],
			"rank1_size", qvl_list[i].DRAM_RANK_SIZE[1]);

		#ifdef DRAM_QVL_CHECK
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
				} else
					return 0;
			}
		} else
			return 0;
		#endif
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
    int channel_nr;

    channel_nr = (*((volatile unsigned int*)(EMI_CONA)) >> 8) & 0x3;

#ifdef SUB_EMI_APB_BASE
    channel_nr++;
#endif

    return (0x1 << channel_nr);
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
		ASSERT(0);
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

int get_row_width_by_emi(unsigned int rank)
{
	unsigned int emi_cona;
	unsigned int shift_row, shift_ext;
	int row_width;

	if (rank == 0) {
		shift_row = 12;
		shift_ext = 22;
	} else if (rank == 1) {
		shift_row = 14;
		shift_ext = 23;
	} else
		return -1;

	emi_cona = mt_emi_sync_read((unsigned int)EMI_CONA);
	row_width =
		((emi_cona >> shift_row) & 0x3) |
		((emi_cona >> shift_ext) & 0x4);

	return (row_width + 13);
}

int get_channel_nr_by_emi(void)
{
	int channel_nr;

	channel_nr = 0x1 << ((mt_emi_sync_read((unsigned int)EMI_CONA) >> 8) & 0x3);

	return channel_nr;
}

int get_rank_nr_by_emi(void)
{
	unsigned int cen_emi_cona = mt_emi_sync_read((unsigned int)EMI_CONA);

	if (cen_emi_cona & (0x3 << 16))
		return 2;
	else
		return 1;
}

_Static_assert(DRAMC_MAX_RK > 1, "rank number is violated");
void get_rank_size_by_emi(unsigned long long dram_rank_size[DRAMC_MAX_RK])
{
	int i;
	unsigned int quad_ch_ratio;
	unsigned int col_bit, row_bit;
	unsigned long long ch0_rank0_size, ch0_rank1_size;
	unsigned long long ch1_rank0_size, ch1_rank1_size;
	unsigned int cen_emi_conh = mt_emi_sync_read((unsigned int)EMI_CONH);
	unsigned long long dq_width;

	switch (mt_get_dram_type_from_hw_trap()) {
		case TYPE_LPDDR4X:
		case TYPE_LPDDR4:
			dq_width = 2;
			break;
		default:
			ASSERT(0);
	}

	dram_rank_size[0] = 0;
	dram_rank_size[1] = 0;

	ch0_rank0_size = (cen_emi_conh >> 16) & 0xF;
	ch0_rank1_size = (cen_emi_conh >> 20) & 0xF;
	ch1_rank0_size = (cen_emi_conh >> 24) & 0xF;
	ch1_rank1_size = (cen_emi_conh >> 28) & 0xF;

	quad_ch_ratio = (get_rank_nr_by_emi() == 4)? 2 : 1;
	ch0_rank0_size = (ch0_rank0_size * quad_ch_ratio) << 28;
	ch0_rank1_size = (ch0_rank1_size * quad_ch_ratio) << 28;
	ch1_rank0_size = (ch1_rank0_size * quad_ch_ratio) << 28;
	ch1_rank1_size = (ch1_rank1_size * quad_ch_ratio) << 28;

	if(ch0_rank0_size == 0) {
		dramc_crit("[EMI] undefined CONH for CH0 RANK0\n");
		ASSERT(0);
        }
	dram_rank_size[0] += ch0_rank0_size * 2;

        if (get_rank_nr_by_emi() > 1) {
		if(ch0_rank1_size == 0) {
			dramc_crit("[EMI] undefined CONH for CH0 RANK1\n");
			ASSERT(0);
		}
		dram_rank_size[1] += ch0_rank1_size * 2;
        }

	if(get_channel_nr_by_emi() > 1) {
		if(ch1_rank0_size == 0) {
			dramc_crit("[EMI] undefined CONH for CH1 RANK0\n");
			ASSERT(0);
		}
		dram_rank_size[0] += ch1_rank0_size * 2;

		if (get_rank_nr_by_emi() > 1) {
			if(ch1_rank1_size == 0) {
				dramc_crit("[EMI] undefined CONH for CH1 RANK1\n");
				ASSERT(0);
            		}
			dram_rank_size[1] += ch1_rank1_size * 2;
		}
        }

	dramc_crit("DRAM rank0 size:0x%llx,\nDRAM rank1 size=0x%llx\n",
		dram_rank_size[0], dram_rank_size[1]);
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
	u64 rank_size[4];

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

void get_dram_rank_size(u64 dram_rank_size[])
{
	int rank_nr, i;

#ifdef DDR_RESERVE_MODE
	if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1) {
		get_rank_size_by_emi(dram_rank_size);
		return;
	}
#endif  	

#ifdef DRAM_ADAPTIVE
	if (!auto_detect_done)
		ASSERT(0);
#endif

	rank_nr = get_dram_rank_nr();

	for(i = 0; i < rank_nr; i++) {
		dram_rank_size[i] = g_default_emi_setting.DRAM_RANK_SIZE[i];
		dram_rank_size[i] <<= 1;
		dramc_debug("%d:dram_rank_size:%llx\n",i,dram_rank_size[i]);
	}
}

void get_dram_freq_step(u32 dram_freq_step[])
{
	unsigned int i;
	unsigned int defined_step[DRAMC_FREQ_CNT] = {
		4266, 3733, 3068, 2366, 1866, 1534, 1200, 800};

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

void get_emi_isu_info(struct isu_info_t *isu_info_ptr)
{
#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	isu_info_ptr->buf_size = EMI_ISU_BUF_SIZE;
	isu_info_ptr->buf_addr = (u64)last_emi_info_ptr->mbw_buf_h;
	isu_info_ptr->buf_addr <<= 32;
	isu_info_ptr->buf_addr += (u64)last_emi_info_ptr->mbw_buf_l;
	isu_info_ptr->ver_addr = (uintptr_t)(&(last_emi_info_ptr->isu_version));
	isu_info_ptr->con_addr = (uintptr_t)(&(last_emi_info_ptr->decs_ctrl));
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
		ASSERT(0);
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
			dramc_crit("[DRAMC] dummy read fail reserve:(0x%llx)\n", reserve_start);
			dramc_crit("[DRAMC] dummy read fail dram_addr:(0x%llx)\n", dram_addr.full_sys_addr);
			ASSERT(0);
		}
	}
}
#endif //#if !__FLASH_TOOL_DA__ && !__ETT__
#endif

void freq_table_are_all_3094(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR3200;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].shuffleIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR3200;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].shuffleIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR3200;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].shuffleIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR3200;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].shuffleIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR3200;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].shuffleIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR3200;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].shuffleIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR3200;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].shuffleIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[7].freq_sel = LP4_DDR3200;
   gFreqTbl[7].divmode = DIV8_MODE;
   gFreqTbl[7].shuffleIdx = SRAM_SHU7;
   gFreqTbl[7].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[7].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[7].ddr_loop_mode = CLOSE_LOOP_MODE;
}

void freq_table_are_all_1534(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR1600;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].shuffleIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR1600;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].shuffleIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR1600;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].shuffleIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR1600;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].shuffleIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR1600;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].shuffleIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR1600;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].shuffleIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR1600;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].shuffleIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[7].freq_sel = LP4_DDR1600;
   gFreqTbl[7].divmode = DIV8_MODE;
   gFreqTbl[7].shuffleIdx = SRAM_SHU7;
   gFreqTbl[7].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[7].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[7].ddr_loop_mode = CLOSE_LOOP_MODE;

}

///TODO: 6893+
void freq_table_opp0_3733_others_3094(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR3200;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].shuffleIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR3200;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].shuffleIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR3200;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].shuffleIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR3733;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].shuffleIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR3200;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].shuffleIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR3200;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].shuffleIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR3200;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].shuffleIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[7].freq_sel = LP4_DDR3200;
   gFreqTbl[7].divmode = DIV8_MODE;
   gFreqTbl[7].shuffleIdx = SRAM_SHU7;
   gFreqTbl[7].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[7].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[7].ddr_loop_mode = CLOSE_LOOP_MODE;
}

void freq_table_opp0_3094_others_1534(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR1600;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].shuffleIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR1600;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].shuffleIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR1600;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].shuffleIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR3200;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].shuffleIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR1600;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].shuffleIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR1600;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].shuffleIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR1600;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].shuffleIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[7].freq_sel = LP4_DDR1600;
   gFreqTbl[7].divmode = DIV8_MODE;
   gFreqTbl[7].shuffleIdx = SRAM_SHU7;
   gFreqTbl[7].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[7].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[7].ddr_loop_mode = CLOSE_LOOP_MODE;

}

void freq_table_opp0_2400_others_1534(void)
{
   gFreqTbl[0].freq_sel = LP4_DDR1600;
   gFreqTbl[0].divmode = DIV8_MODE;
   gFreqTbl[0].shuffleIdx = SRAM_SHU1;
   gFreqTbl[0].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[0].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[0].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[1].freq_sel = LP4_DDR1600;
   gFreqTbl[1].divmode = DIV8_MODE;
   gFreqTbl[1].shuffleIdx = SRAM_SHU3;
   gFreqTbl[1].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[1].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[1].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[2].freq_sel = LP4_DDR1600;
   gFreqTbl[2].divmode = DIV8_MODE;
   gFreqTbl[2].shuffleIdx = SRAM_SHU2;
   gFreqTbl[2].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[2].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[2].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[3].freq_sel = LP4_DDR2400;
   gFreqTbl[3].divmode = DIV8_MODE;
   gFreqTbl[3].shuffleIdx = SRAM_SHU0;
   gFreqTbl[3].duty_calibration_mode = DUTY_NEED_K;
   gFreqTbl[3].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[3].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[4].freq_sel = LP4_DDR1600;
   gFreqTbl[4].divmode = DIV8_MODE;
   gFreqTbl[4].shuffleIdx = SRAM_SHU5;
   gFreqTbl[4].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[4].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[4].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[5].freq_sel = LP4_DDR1600;
   gFreqTbl[5].divmode = DIV8_MODE;
   gFreqTbl[5].shuffleIdx = SRAM_SHU4;
   gFreqTbl[5].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[5].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[5].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[6].freq_sel = LP4_DDR1600;
   gFreqTbl[6].divmode = DIV8_MODE;
   gFreqTbl[6].shuffleIdx = SRAM_SHU6;
   gFreqTbl[6].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[6].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[6].ddr_loop_mode = CLOSE_LOOP_MODE;

   gFreqTbl[7].freq_sel = LP4_DDR1600;
   gFreqTbl[7].divmode = DIV8_MODE;
   gFreqTbl[7].shuffleIdx = SRAM_SHU7;
   gFreqTbl[7].duty_calibration_mode = DUTY_DEFAULT;
   gFreqTbl[7].vref_calibartion_enable = VREF_CALI_ON;
   gFreqTbl[7].ddr_loop_mode = CLOSE_LOOP_MODE;

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

	dramc_crit("DOE setting: vcore %d, vdram %d, vddq %d, vmddr %d, vio18 %d",
		voltage_sel_info_ptr.vcore, voltage_sel_info_ptr.vdram,
		voltage_sel_info_ptr.vddq, voltage_sel_info_ptr.vmddr,
		voltage_sel_info_ptr.vio18);
}

int vio18_voltage_select(void)
{
	if (voltage_sel_info_ptr.vio18 == LEVEL_LV) {
		return HQA_VIO18_LV;	
	} else if (voltage_sel_info_ptr.vio18 == LEVEL_HV) {
		return HQA_VIO18_HV;
	} else {
		return HQA_VIO18_NV;
	}
}

int vmddr_voltage_select(void)
{
	if (voltage_sel_info_ptr.vmddr == LEVEL_LV) {
		return HQA_VMDDR_LV_LP4;	
	} else if (voltage_sel_info_ptr.vmddr == LEVEL_HV) {
		return HQA_VMDDR_HV_LP4;
	} else {
		return HQA_VMDDR_NV_LP4;
	}
}

int vddq_voltage_select(void)
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

int vcore_voltage_select(DRAM_KSHU kshu, int efuse)
{
	int ret = 0;
	if (voltage_sel_info_ptr.vcore == LEVEL_LV) {
		if (efuse == 2) {
			switch(kshu) {
				case KSHU0:
					ret = HQA_VCORE_LV50_LP4_KSHU0_PL;
					break;
				case KSHU1:
					ret = HQA_VCORE_LV50_LP4_KSHU1_PL;
					break;
				case KSHU2:
					ret = HQA_VCORE_LV50_LP4_KSHU2_PL;
					break;
				case KSHU3:
					ret = HQA_VCORE_LV50_LP4_KSHU3_PL;
					break;
				case KSHU4:
					ret = HQA_VCORE_LV50_LP4_KSHU4_PL;
					break;
				case KSHU5:
					ret = HQA_VCORE_LV50_LP4_KSHU5_PL;
					break;
				case KSHU6:
					ret = HQA_VCORE_LV50_LP4_KSHU6_PL;
					break;
				case KSHU7:
					ret = HQA_VCORE_LV50_LP4_KSHU7_PL;
					break;
			};
		} else if (efuse == 1) {
			switch(kshu) {
				case KSHU0:
					ret = HQA_VCORE_LV25_LP4_KSHU0_PL;
					break;
				case KSHU1:
					ret = HQA_VCORE_LV25_LP4_KSHU1_PL;
					break;
				case KSHU2:
					ret = HQA_VCORE_LV25_LP4_KSHU2_PL;
					break;
				case KSHU3:
					ret = HQA_VCORE_LV25_LP4_KSHU3_PL;
					break;
				case KSHU4:
					ret = HQA_VCORE_LV25_LP4_KSHU4_PL;
					break;
				case KSHU5:
					ret = HQA_VCORE_LV25_LP4_KSHU5_PL;
					break;
				case KSHU6:
					ret = HQA_VCORE_LV25_LP4_KSHU6_PL;
					break;
				case KSHU7:
					ret = HQA_VCORE_LV25_LP4_KSHU7_PL;
					break;
			};
		} else {
			switch(kshu) {
				case KSHU0:
					ret = HQA_VCORE_LV0_LP4_KSHU0_PL;
					break;
				case KSHU1:
					ret = HQA_VCORE_LV0_LP4_KSHU1_PL;
					break;
				case KSHU2:
					ret = HQA_VCORE_LV0_LP4_KSHU2_PL;
					break;
				case KSHU3:
					ret = HQA_VCORE_LV0_LP4_KSHU3_PL;
					break;
				case KSHU4:
					ret = HQA_VCORE_LV0_LP4_KSHU4_PL;
					break;
				case KSHU5:
					ret = HQA_VCORE_LV0_LP4_KSHU5_PL;
					break;
				case KSHU6:
					ret = HQA_VCORE_LV0_LP4_KSHU6_PL;
					break;
				case KSHU7:
					ret = HQA_VCORE_LV0_LP4_KSHU7_PL;
					break;
			};
		}
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
			case KSHU7:
				ret = HQA_VCORE_HV_LP4_KSHU7_PL;
				break;
		};
	} else {
#if defined(VCORE_BIN)
		ret = 0;
#else
		if (efuse == 2) {
			switch(kshu) {
				case KSHU0:
					ret = HQA_VCORE_NV50_LP4_KSHU0_PL;
					break;
				case KSHU1:
					ret = HQA_VCORE_NV50_LP4_KSHU1_PL;
					break;
				case KSHU2:
					ret = HQA_VCORE_NV50_LP4_KSHU2_PL;
					break;
				case KSHU3:
					ret = HQA_VCORE_NV50_LP4_KSHU3_PL;
					break;
				case KSHU4:
					ret = HQA_VCORE_NV50_LP4_KSHU4_PL;
					break;
				case KSHU5:
					ret = HQA_VCORE_NV50_LP4_KSHU5_PL;
					break;
				case KSHU6:
					ret = HQA_VCORE_NV50_LP4_KSHU6_PL;
					break;
				case KSHU7:
					ret = HQA_VCORE_NV50_LP4_KSHU7_PL;
					break;
			};
		} else if (efuse == 1) {
			switch(kshu) {
				case KSHU0:
					ret = HQA_VCORE_NV25_LP4_KSHU0_PL;
					break;
				case KSHU1:
					ret = HQA_VCORE_NV25_LP4_KSHU1_PL;
					break;
				case KSHU2:
					ret = HQA_VCORE_NV25_LP4_KSHU2_PL;
					break;
				case KSHU3:
					ret = HQA_VCORE_NV25_LP4_KSHU3_PL;
					break;
				case KSHU4:
					ret = HQA_VCORE_NV25_LP4_KSHU4_PL;
					break;
				case KSHU5:
					ret = HQA_VCORE_NV25_LP4_KSHU5_PL;
					break;
				case KSHU6:
					ret = HQA_VCORE_NV25_LP4_KSHU6_PL;
					break;
				case KSHU7:
					ret = HQA_VCORE_NV25_LP4_KSHU7_PL;
					break;
			};
		} else {
			switch(kshu) {
				case KSHU0:
					ret = HQA_VCORE_NV0_LP4_KSHU0_PL;
					break;
				case KSHU1:
					ret = HQA_VCORE_NV0_LP4_KSHU1_PL;
					break;
				case KSHU2:
					ret = HQA_VCORE_NV0_LP4_KSHU2_PL;
					break;
				case KSHU3:
					ret = HQA_VCORE_NV0_LP4_KSHU3_PL;
					break;
				case KSHU4:
					ret = HQA_VCORE_NV0_LP4_KSHU4_PL;
					break;
				case KSHU5:
					ret = HQA_VCORE_NV0_LP4_KSHU5_PL;
					break;
				case KSHU6:
					ret = HQA_VCORE_NV0_LP4_KSHU6_PL;
					break;
				case KSHU7:
					ret = HQA_VCORE_NV0_LP4_KSHU7_PL;
					break;
			};
		}
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

#if (FOR_DV_SIMULATION_USED==0)
int update_emi_setting(EMI_SETTINGS *default_emi_setting, EMI_INFO_T *emi_info)
{
	unsigned int ddr_type;
	unsigned int cen_cona_val, cen_conf_val, cen_conh_val, cen_conk_val;
	unsigned int chn_cona_val;
	unsigned int cen_conf_shf;
	unsigned int col, row, row_ext, rk_size, rk_size_ext, rk_size_chn_ext;
	unsigned int temp_val;
	unsigned int i;

	ddr_type = default_emi_setting->type & 0xFF;
	cen_conh_val = 0x00000003;
	cen_conk_val = mt_emi_sync_read(EMI_APB_BASE+0x050);
	cen_conk_val = cen_conk_val & 0x0000FFFF;
	if (u1IsLP4Family(ddr_type)) {
		cen_cona_val = 0x00000104;
		chn_cona_val = 0x04000000;
	} else {
		cen_cona_val = 0x00000006;
		chn_cona_val = 0x00000002;
	}

	for (i = 0; i < emi_info->rk_num; i++) {
		row = emi_info->row_width[i] - 13;
		row_ext = row >> 2;
		row &= 0x3;

		col = emi_info->col_width[i] - 9;

		rk_size = emi_info->rank_size[i] >> 28;
		if (u1IsLP4Family(ddr_type))
			rk_size >>= 1;
		rk_size_ext = rk_size >> 4;
		rk_size_chn_ext = rk_size_ext & 0x1;
		rk_size &= 0xF;

		if (i == 1) {
			col <<= 2;
			row <<= 2;
			row_ext <<= 1;
			rk_size <<= 4;
			rk_size_ext <<= 4;
			rk_size_chn_ext <<= 1;
			cen_cona_val |= 0x00030000;
			chn_cona_val |= 0x00000001;
		}

		cen_cona_val |=
			(col << 4) | (row << 12) | (col << 20) |
			(row_ext << 24) | (row << 28);
		cen_conh_val |=
			(row_ext << 4) | (rk_size << 16) | (rk_size << 24);
		cen_conk_val |=
			(rk_size_ext << 16) | (rk_size_ext << 24);
		chn_cona_val |=
			(row_ext << 2) | (col << 4) | (row << 12) |
			(rk_size << 16) | (rk_size_chn_ext << 8);
	}

	cen_conf_shf = 0;
	for (i = 0; i < emi_info->rk_num; i++) {
		temp_val = 2 + emi_info->col_width[i];
		if (cen_conf_shf < temp_val)
			cen_conf_shf = temp_val;
	}
	if (((cen_conf_shf - 9) * 4) < 21)
		cen_conf_val = 0x421 << ((cen_conf_shf - 9) * 4);
	else
		return -1;

	dramc_crit("[EMI] %s(0x%x),%s(0x%x),%s(0x%x),%s(0x%x),%s(0x%x)\n",
		"CEN_CONA", cen_cona_val,
		"CEN_CONF", cen_conf_val,
		"CEN_CONH", cen_conh_val,
		"CEN_CONK", cen_conk_val,
		"CHN_CONA", chn_cona_val);

	default_emi_setting->EMI_CONA_VAL = cen_cona_val;
	default_emi_setting->EMI_CONF_VAL = cen_conf_val;
	default_emi_setting->EMI_CONH_VAL = cen_conh_val;
	default_emi_setting->EMI_CONK_VAL = cen_conk_val;
	default_emi_setting->CHN0_EMI_CONA_VAL = chn_cona_val;
	default_emi_setting->CHN1_EMI_CONA_VAL = chn_cona_val;

	return 0;
}

void dram_auto_detection(void)
{
	DRAM_INFO_BY_MRR_T dram_info;
	EMI_INFO_T emi_info;
	DRAM_CBT_MODE_EXTERN_T dram_mode;
	unsigned int dram_type;
	int ret;

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
		ASSERT(0);
	}
#endif

	ret = update_dram_setting(&g_default_emi_setting, dram_type, &dram_info);
	if (ret) {
		dramc_crit("[DRAMC] update_dram_setting err %d\n", ret);
		ASSERT(0);
	}

	ret = decode_emi_info(&emi_info, dram_type, &dram_info);
	if (ret) {
		dramc_crit("[DRAMC] decode_emi_info err %d\n", ret);
		ASSERT(0);
	}

	ret = update_emi_setting(&g_default_emi_setting, &emi_info);
	if (ret) {
		dramc_crit("[DRAMC] update_emi_setting err %d\n", ret);
		ASSERT(0);
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

void phy_addr_to_dram_addr(dram_addr_t *dram_addr, unsigned long long phy_addr)
{
	unsigned int emi_cona, emi_conf;
	unsigned long long rank_size[4];
	unsigned int channel_num, rank_num;
	unsigned int bit_scramble, bit_xor, bit_shift, channel_pos, channel_width;
	unsigned int temp;
	unsigned int index;

	emi_cona = *((volatile unsigned int *)EMI_CONA);
	emi_conf = *((volatile unsigned int *)EMI_CONF) >> 8;
	get_rank_size_by_emi(rank_size);
	rank_num = (unsigned int) get_dram_rank_nr();
	channel_num = (unsigned int) get_dram_channel_nr();

	phy_addr -= 0x40000000;
	for (index = 0; index < rank_num; index++) {
		if (phy_addr >= rank_size[index])
			phy_addr -= rank_size[index];
		else
			break;
	}

	for (bit_scramble = 11; bit_scramble < 17; bit_scramble++) {
		bit_xor = (emi_conf >> (4 * (bit_scramble - 11))) & 0xf;
		bit_xor &= phy_addr >> 16;
		for (bit_shift = 0; bit_shift < 4; bit_shift++)
			phy_addr ^= ((bit_xor>>bit_shift)&0x1) << bit_scramble;
	}

	if (channel_num > 1) {
		channel_pos = ((emi_cona >> 2) & 0x3) + 7;

		for (channel_width = bit_shift = 0; bit_shift < 4; bit_shift++) {
			if ((unsigned int)(1 << bit_shift) >= channel_num)
				break;
			channel_width++;
		}

		switch (channel_width) {
		case 2:
			dram_addr->addr = ((phy_addr & ~(((0x1 << 2) << channel_pos) - 1)) >> 2);
			break;
		default:
			dram_addr->addr = ((phy_addr & ~(((0x1 << 1) << channel_pos) - 1)) >> 1);
			break;
		}
		dram_addr->addr |= (phy_addr & ((0x1 << channel_pos) - 1));
	}

	temp = dram_addr->addr >> 1;
	switch ((emi_cona >> 4) & 0x3) {
	case 0:
		dram_addr->col = temp & 0x1FF;
		temp = temp >> 9;
		break;
	case 1:
		dram_addr->col = temp & 0x3FF;
		temp = temp >> 10;
		break;
	case 2:
	default:
		dram_addr->col = temp & 0x7FF;
		temp = temp >> 11;
		break;
	}
	dram_addr->bk = temp & 0x7;
	temp = temp >> 3;

	dram_addr->row = temp;

	//mcSHOW_DBG_MSG(("ch%d, rk%d, dram addr: %x\n", dram_addr->ch, dram_addr->rk, dram_addr->addr));
	//mcSHOW_DBG_MSG(("bk%x, row%x, col%x\n", dram_addr->bk, dram_addr->row, dram_addr->col));
}

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

static unsigned int get_dramc_addr(dram_addr_t *dram_addr, unsigned int offset)
{
	static char init_pattern = 0;
	unsigned int channel_num, rank_num;
	unsigned long long dummy_read_addr;
	unsigned long long rank_size[4];
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
	unsigned long long latch_result = 0;
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
#if CFG_ENABLE_DCACHE
	plat_clean_invalidate_dcache();
#endif

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
#if (CFG_DRAM_PARTITION)
	part_dram_data_addr = get_part_addr("dram_para"); // addr = 0x108000
#else
	part_dram_data_addr = get_part_addr("boot_para") + 0x100000; // addr = 0x108000
#endif
	if (part_dram_data_addr != 0x0)
		dramc_crit("[dramc] init partition address is 0x%llx\n", part_dram_data_addr);
	else {
		dramc_crit("[dramc] init partition address is incorrect !!!\n");
	}
#endif

#if defined(SLT)
#if (CFG_DRAM_PARTITION)
	part_dram_data_addr_slt = get_part_addr("dram_para");
#else
	part_dram_data_addr_slt = get_part_addr("boot_para") + 0x100000;
#endif
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
	last_dramc_info_ptr->gating_err[2][0] = 0x0;
	last_dramc_info_ptr->gating_err[2][1] = 0x0;
	last_dramc_info_ptr->gating_err[3][0] = 0x0;
	last_dramc_info_ptr->gating_err[3][1] = 0x0;

	dsb();
}

void dram_fatal_backup_stberr(void)
{
	last_dramc_info_ptr->last_gating_err[0][0] = last_dramc_info_ptr->gating_err[0][0];
	last_dramc_info_ptr->last_gating_err[0][1] = last_dramc_info_ptr->gating_err[0][1];
	last_dramc_info_ptr->last_gating_err[1][0] = last_dramc_info_ptr->gating_err[1][0];
	last_dramc_info_ptr->last_gating_err[1][1] = last_dramc_info_ptr->gating_err[1][1];
	last_dramc_info_ptr->last_gating_err[2][0] = last_dramc_info_ptr->gating_err[2][0];
	last_dramc_info_ptr->last_gating_err[2][1] = last_dramc_info_ptr->gating_err[2][1];
	last_dramc_info_ptr->last_gating_err[3][0] = last_dramc_info_ptr->gating_err[3][0];
	last_dramc_info_ptr->last_gating_err[3][1] = last_dramc_info_ptr->gating_err[3][1];

	dsb();
}

void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code)
{
	if ((chn > 3) || (rk > 1))
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
	char *doe_feature =(char *)dconfig_getenv(feature);
	int doe_result = atoi(doe_feature);
	dramc_debug("DOE force setting %s=%d\n", feature, doe_result);
	return doe_result;
#else
	return 0;
#endif
}
#endif

#if (CFG_DRAM_LOG_TO_STORAGE)

#if (CFG_K_LOG_TO_STORAGE)
#define GATING_ERR_MAGIC 0x9a712931

int set_err_flag_to_storage(void)
{
	int ret;
	unsigned int ge_magic_base;
	unsigned int ge_magic = GATING_ERR_MAGIC;
	blkdev_t *bootdev = NULL;

	bootdev = blkdev_get(CFG_BOOT_DEV);
#if (CFG_DRAM_PARTITION)
	ge_magic_base = get_part_addr("dram_para") + 0x200000;
#else
	ge_magic_base = get_part_addr("boot_para") + 0x300000;
#endif

	ret = blkdev_write(bootdev, ge_magic_base, sizeof(ge_magic), (u8*)&ge_magic, storage_get_part_id(STORAGE_PHYS_PART_USER));

	return ret;
}
#else
int set_err_flag_to_storage(void)
{
	return 0;
}
#endif

void log_to_storage(const char c)
{
	int ret, clr_count;
	blkdev_t *bootdev = NULL;
	static u8 logen = 0;
#if (CFG_K_LOG_TO_STORAGE)
	static unsigned int gating_error;
	unsigned int ge_check;
	unsigned int ge_magic_base;
#endif

	bootdev = blkdev_get(CFG_BOOT_DEV);

	if (log_start && (!logen)) {
		logen = 1;
		logcount = 0;
#if (CFG_K_LOG_TO_STORAGE)
#if (CFG_DRAM_PARTITION)
		part_dram_data_addr_uart = get_part_addr("dram_para") + 0x100000; // 1M ~ 2M for fastk info 
#else
		part_dram_data_addr_uart = get_part_addr("boot_para") + 0x200000; // 1M ~ 2M for fastk info 
#endif
#else
#if (CFG_DRAM_PARTITION)
		part_dram_data_addr_uart = get_part_addr("dram_para") ; // addr = 0x1f300000, the first 1MB for debug
#else
		part_dram_data_addr_uart = get_part_addr("boot_para") + 0x100000; // addr = 0x1f300000, the first 1MB for debug
#endif
#endif
		memset(&logbuf, 0, sizeof(logbuf));
#if (CFG_K_LOG_TO_STORAGE)
		for (clr_count = 2772; clr_count < 3072; clr_count++) //300k
#else
		for (clr_count = 0; clr_count < 3072; clr_count++) //3M
#endif
			ret = blkdev_write(bootdev, (part_dram_data_addr_uart + (1024 * clr_count)), 1024, (u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));
#if (CFG_K_LOG_TO_STORAGE)
		// check gating error occur
#if (CFG_DRAM_PARTITION)
		ge_magic_base = get_part_addr("dram_para") + 0x200000;
#else
		ge_magic_base = get_part_addr("boot_para") + 0x300000;
#endif
		ret = blkdev_read(bootdev, ge_magic_base, sizeof(ge_check), (u8*)&ge_check, storage_get_part_id(STORAGE_PHYS_PART_USER));
		if (ge_check != GATING_ERR_MAGIC) {
			gating_error = 0;
		} else {
			//clear GE flag
			gating_error = 1;
			ge_check = 0;
			ret = blkdev_write(bootdev, ge_magic_base, sizeof(ge_check), (u8*)&ge_check, storage_get_part_id(STORAGE_PHYS_PART_USER));
		}
#endif
	}

	if (log_start) {
#if (CFG_K_LOG_TO_STORAGE)
		if (!gating_error) {
#endif
#if (!CFG_K_LOG_TO_STORAGE)
			if (((((char) c >> 4) & 0x7) > 1) & ((((char) c >> 4) & 0x7) < 7))
				logbuf[logcount] = ((char) c & 0xF0) | (((char) c >> 2) & 0x03) | (((char) c << 2) & 0x0C);
			else
#endif
				logbuf[logcount] = (char) c;
    			logcount = logcount + 1;
    			//write to storage
    			if (logcount==1024) {
    				logcount = 0;
    				ret = blkdev_write(bootdev, part_dram_data_addr_uart, 1024, (u8*)&logbuf, storage_get_part_id(STORAGE_PHYS_PART_USER));   
    				part_dram_data_addr_uart = part_dram_data_addr_uart + 1024;
    			}
#if (CFG_K_LOG_TO_STORAGE)
		}
#endif
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
		for (i = 0; i < DRAM_DFS_SHUFFLE_MAX; i++) {
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
		for (i = 0; i < DRAM_DFS_SHUFFLE_MAX; i++) {
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

#ifdef DRAM_QVL_CHECK
	emi_checksum = crc16((u8*)qvl_list, sizeof(qvl_list));
#endif

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

#ifdef DRAM_QVL_CHECK
    /* calculate and assign checksum */
    mrr_info.emi_checksum = crc16((u8*)qvl_list, sizeof(qvl_list));
    assign_checksum_for_mdl_data(&mrr_info);
#endif

    ret = blkdev_write(bootdev, part_dram_data_addr + ((unsigned long) &datap->mrr_info), sizeof(mrr_info), (u8*)&mrr_info, storage_get_part_id(STORAGE_PHYS_PART_USER));
    if (ret != 0) {
        dramc_crit("[dramc] blkdev_write failed\n");
        SET_DRAM_STORAGE_API_ERR(ERR_BLKDEV_WRITE_FAIL, DRAM_STORAGE_API_WRITE);
        return -ERR_BLKDEV_WRITE_FAIL;
    }

    return 0;
}

int read_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
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
		for (i = 0; i < DRAM_DFS_SHUFFLE_MAX; i++) {
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

int write_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
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
    for (i = 0; i < DRAM_DFS_SHUFFLE_MAX; i++) {
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

int read_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
{
	return 0;
}

int write_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData)
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

#if !__ETT__
void reserve_emi_mbw_buf(void)
{
	unsigned long long rsv_start;
	dram_addr_t dram_addr;

	dram_addr.ch = 0;
	dram_addr.rk = 0;
	get_dramc_addr(&dram_addr, 0x0);

#if CFG_LAST_EMI_BW_DUMP
	/* to avoid CMA reserve conflict, need to allocate before 0x80000000 */
	rsv_start = mblock_reserve_ext(&bootarg.mblock_info, EMI_ISU_BUF_SIZE,
					EMI_ISU_BUF_SIZE, 0x80000000, 0, "emi_mbw_buf");
#else
	rsv_start = 0ULL;
#endif

#ifdef LAST_EMI
	last_emi_info_ptr = (LAST_EMI_INFO_T *) get_dbg_info_base(KEY_LAST_EMI);
	last_emi_info_ptr->mbw_buf_l = (unsigned int) (rsv_start & 0xFFFFFFFF);
	last_emi_info_ptr->mbw_buf_h = (unsigned int) (rsv_start >> 32);
#endif
}

void reserve_emi_mbist_buf(void)
{
	unsigned long long rsv_start;
	dram_addr_t dram_addr;

	dram_addr.ch = 0;
	dram_addr.rk = 0;
	get_dramc_addr(&dram_addr, 0x0);

	/* to avoid CMA reserve conflict, need to allocate before 0x80000000 */
	rsv_start = mblock_reserve_ext(&bootarg.mblock_info, 0x1000,
					0x1000, 0x80000000, 0, "emi_mbist_buf");
	//print("reserve_emi_mbist_buf rsv_start=0x%llx  \n",rsv_start);
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x000f7f7f); // enable broadcast for 2+2CH
	mt_emi_sync_write(EMI_APB_BASE+0x0000098c,rsv_start - 0x40000000);
	mt_emi_sync_write(EMI_APB_BASE+0x00000990,rsv_start - 0x40000000 + 0xFFF);
	mt_emi_sync_write(INFRACFG_AO_BASE+0x000000b4, 0x00000000); // disable broadcast
}

u64 platform_memory_size(void)
{
    static u64 mem_size = 0;
    int nr_rank;
    int i;
    u64 rank_size[4], *rksize = &rank_size[0];
    u64 size = 0;

    if (!mem_size) {
        nr_rank = get_dram_rank_nr();

        get_dram_rank_size(rank_size);

        for (i = 0; i < nr_rank; i++)
            size += *rksize++;
        mem_size = size;
    }

    return mem_size;
}

#endif

