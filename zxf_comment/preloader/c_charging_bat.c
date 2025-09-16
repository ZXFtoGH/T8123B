/*
整体结构概览
功能	                   对应函数/变量
充电器类型检测	            hw_charger_type_detection() / mt_charger_type_detection()
外部 PMIC 检测支持	        mtk_ext_chgdet()（通过 MTK_EXT_CHGDET_SUPPORT 宏控制）
电池电压检测	            get_bat_sense_volt()
充电电压检测	            get_charger_volt()
启动充电	                pchr_turn_on_charging()
看门狗喂狗	                kick_charger_wdt()
电池保护检查	            pl_check_bat_protect_status()

函数调用关系图：
                          ┌──────────────────────┐
                          │   mt_charger_type_detection()   ← 入口
                          └──────────────────────┘
                                      │
                                      ▼
                   ┌────────────────────────────────────┐
                   │ 是否首次检测？ (g_first_check == 0) │
                   └────────────────────────────────────┘
                                      │
                                      ├─ 否 → 直接返回 g_ret
                                      │
                                      ▼ 是
                   ┌────────────────────────────────────┐
                   │ 是否检测到充电器？ upmu_is_chr_det() │
                   └────────────────────────────────────┘
                                      │
                                      ├─ 否 → 返回 CHARGER_UNKNOWN
                                      │
                                      ▼ 是
              ┌──────────────────────────────────────────────────┐
              │ 是否定义 MTK_EXT_CHGDET_SUPPORT？                 │
              └──────────────────────────────────────────────────┘
                         │                      │
             是 ↓                        否 ↓
     ┌─────────────────┐        ┌──────────────────────────────┐
     │ mtk_ext_chgdet() │        │ hw_charger_type_detection() │
     └─────────────────┘        └──────────────────────────────┘
           │                                 │
           │                                 ├─ hw_bc11_init()
           │                                 ├─ hw_bc11_DCD()
           │                                 ├─ hw_bc11_stepA1()
           │                                 ├─ hw_bc11_stepA2()
           │                                 ├─ hw_bc11_stepB2()
           │                                 └─ hw_bc11_done()
           │                                 └─ 结果 → g_ret
           │
           ▼
     [结果保存到 g_ret]

           ▼
┌──────────────────────────────┐
│ pl_check_bat_protect_status() │
└──────────────────────────────┘
           │
           ├─ get_bat_sense_volt()
           ├─ get_charger_volt()
           ├─ get_charging_current()
           └─ pchr_turn_on_charging()

           ▼
   kick_charger_wdt() ← 喂狗
*/
#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <mtk_charger.h>
#include <pmic.h>
#include "cust_usb.h"
#include "pal_log.h"

enum auxadc_channel {
	AUXADC_CHAN_BATADC,
	AUXADC_CHAN_ISENSE,
	AUXADC_CHAN_VCDT,
	AUXADC_CHAN_BAT_TEMP,
	AUXADC_CHAN_BATID,
	AUXADC_CHAN_CHIP_TEMP,
	AUXADC_CHAN_VCORE_TEMP,
	AUXADC_CHAN_VPROC_TEMP,
	AUXADC_CHAN_VGPU_TEMP,
	AUXADC_CHAN_ACCDET,
	AUXADC_CHAN_VDCXO,
	AUXADC_CHAN_TSX_TEMP,
	AUXADC_CHAN_HPOFS_CAL,
	AUXADC_CHAN_DCXO_TEMP,
	AUXADC_CHAN_VBIF,
	AUXADC_CHAN_VTREF,
	AUXADC_CHAN_IMP,
	AUXADC_CHAN_VSYSSNS,
	AUXADC_CHAN_VIN1,
	AUXADC_CHAN_VIN2,
	AUXADC_CHAN_VIN3,
	AUXADC_CHAN_VIN4,
	AUXADC_CHAN_VIN5,
	AUXADC_CHAN_VIN6,
	AUXADC_CHAN_VIN7,
	AUXADC_CHAN_MAX,
};

enum auxadc_val_type {
	AUXADC_VAL_PROCESSED,
	AUXADC_VAL_RAW,
};

//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
#define V_CHARGER_MAX 6500				// 6.5 V
#define SWCHR_POWER_PATH 1
#ifdef MT6360_PMU
extern int mtk_get_vbus_adc(int *vbus);
#endif

#if SWCHR_POWER_PATH
#define BATTERY_LOWVOL_THRESOLD 0
#else
#define BATTERY_LOWVOL_THRESOLD 3300
#endif

CHARGER_TYPE g_ret = CHARGER_UNKNOWN;
int g_charger_in_flag = 0;
int g_first_check = 0;
static int vbat_status = PMIC_VBAT_NOT_DROP;

#ifdef PMIC_FORCE_CHARGER_TYPE
int hw_charger_type_detection(void)
{
    pal_log_info("force STANDARD_HOST\r\n");
    return STANDARD_HOST;
}
#else

extern void Charger_Detect_Init(void);
extern void Charger_Detect_Release(void);
extern void mdelay (unsigned long msec);

static void hw_bc11_dump_register(void)
{
/*
	kal_uint32 reg_val = 0;
	kal_uint32 reg_num = CHR_CON18;
	kal_uint32 i = 0;

	for(i=reg_num ; i<=CHR_CON19 ; i+=2)
	{
		reg_val = upmu_get_reg_value(i);
		pal_log_info("Chr Reg[0x%x]=0x%x \r\n", i, reg_val);
	}
*/
}

static void hw_bc11_init(void)
{
    mdelay(200);
    Charger_Detect_Init();      // 充电检测通用初始化
#if TBD
    //RG_bc11_BIAS_EN=1
    pmic_config_interface(PMIC_RG_BC11_BIAS_EN_ADDR, 1, PMIC_RG_BC11_BIAS_EN_MASK, PMIC_RG_BC11_BIAS_EN_SHIFT);
    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR,0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //bc11_RST=1
    pmic_config_interface(PMIC_RG_BC11_RST_ADDR, 1, PMIC_RG_BC11_RST_MASK, PMIC_RG_BC11_RST_SHIFT);
    //bc11_BB_CTRL=1
    pmic_config_interface(PMIC_RG_BC11_BB_CTRL_ADDR, 1, PMIC_RG_BC11_BB_CTRL_MASK, PMIC_RG_BC11_BB_CTRL_SHIFT);
#endif
    mdelay(50);
}

static U32 hw_bc11_DCD(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_IPU_EN[1.0] = 10
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x2, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x1, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=01
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x1, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 10
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x2, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
#endif /* TBD */
    return wChargerAvail;
}

static U32 hw_bc11_stepA1(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_IPD_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x1, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x1, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
#endif /* TBD */

    return  wChargerAvail;
}


static U32 hw_bc11_stepA2(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_VSRC_EN[1.0] = 10
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x2, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_IPD_EN[1:0] = 01
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x1, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x1, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x0, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
#endif /* TBD */
    return  wChargerAvail;
}


static U32 hw_bc11_stepB2(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_IPU_EN[1:0]=10
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x2, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=01
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x1, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x1, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    if (!wChargerAvail)
    {
        //RG_bc11_VSRC_EN[1.0] = 10
        //mt6325_upmu_set_rg_bc11_vsrc_en(0x2);
        pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x2, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    }
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
#endif /* TBD */
    return  wChargerAvail;
}


static void hw_bc11_done(void)
{
#if TBD
    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x0, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=0
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_BIAS_EN=0
    pmic_config_interface(PMIC_RG_BC11_BIAS_EN_ADDR, 0x0, PMIC_RG_BC11_BIAS_EN_MASK, PMIC_RG_BC11_BIAS_EN_SHIFT);
#endif /* TBD */
    Charger_Detect_Release();
}

void pmic_DetectVbatDrop (void)
{

	U32 ret=0;
	U32 just_rst=0;

	pmic_read_interface(PMIC_JUST_PWRKEY_RST_ADDR, (&just_rst), PMIC_JUST_PWRKEY_RST_MASK, PMIC_JUST_PWRKEY_RST_SHIFT);
	pmic_config_interface(PMIC_CLR_JUST_RST_ADDR, 1, PMIC_CLR_JUST_RST_MASK, PMIC_CLR_JUST_RST_SHIFT);

	pal_log_info("[PMIC]just_rst = %d\n", just_rst);
	if(just_rst)
		vbat_status = PMIC_VBAT_DROP;
	else
		vbat_status = PMIC_VBAT_NOT_DROP;

}

int pmic_IsVbatDrop(void)
{
   /* used by RTC to check power on reason */
   return vbat_status;
}


void hw_set_cc(int cc_val)
{
    //TBD
}

/*
MTK 平台中用于检测 USB 充电器类型的底层硬件函数，它实现了 USB BC1.2（Battery Charging v1.2）协议 的检测流程，通过一系列电压和电阻的测量来判断插入的是哪种类型的充电设备。
检测流程（经典四步法）
步骤	操作	判断结果
hw_bc11_init()	初始化 PMIC 的 BC1.2 模块	准备开始检测
hw_bc11_DCD()	做 Data Contact Detect (DCD)：检测 D+ D- 是否接触良好	若失败 → 可能是浮空或坏线
hw_bc11_stepA1()	测试 D- 上拉、D+ 下拉	若导通 → 可能是 Apple 充电器
hw_bc11_stepA2()	D+ 上拉，测 D- 电压	若导通 → 进入 B2
hw_bc11_stepB2()	D- 上拉，测 D+ 电压	若导通 → DCP（快充头）<br>否则 → Charging Host（支持充电的电脑口）
默认	都不导通 → SDP（标准 USB 口）	

BC1.1检测步骤详解
hw_bc11_DCD() - 数据接触检测
    在D+和D-上施加测试电流
    检测数据线是否短路
    判断是否为专用充电端口

hw_bc11_stepA1() - 苹果充电器检测
    检测特定的电压特性
    识别苹果2.1A充电器的特殊特征

hw_bc11_stepA2() - USB数据线检测
    检测D+和D-线上的电压
    判断USB主机类型

hw_bc11_stepB2() - 充电器/主机区分
    进一步检测电流输出能力
    区分标准充电器和充电USB主机
*/
int hw_charger_type_detection(void)
{
    CHARGER_TYPE charger_tye = CHARGER_UNKNOWN;

    /********* Step initial  **************
     * 配置PMIC相关寄存器  设置BC1.1检测所需的偏置电压和比较器
    */
    hw_bc11_init();

    /********* Step DCD **************
     * DCD功能：检测数据线是否短路，判断是否为专用充电端口（DCP）
     * 原理：
            给 D+ 施加一个微小电流（IDP_SRC）。
            测量 D+ 上的电压。
            如果电压上升明显 → 说明 D+ 是开路的（没接好）→ 返回 0（失败）。
            如果电压变化小 → 说明 D+ 接地了（正常连接）→ 返回 1（成功）。
        返回1：数据线短路，可能是专用充电器
        返回0：数据线未短路，可能是标准USB主机
    */
    if(1 == hw_bc11_DCD())
    {
         /********* Step A1 ***************/
         if(1 == hw_bc11_stepA1())
         {
             charger_tye = APPLE_2_1A_CHARGER;
             pal_log_info("step A1 : Apple 2.1A CHARGER!\r\n");
         }
         else
         {
             charger_tye = NONSTANDARD_CHARGER;
             pal_log_info("step A1 : Non STANDARD CHARGER!\r\n");
         }
    }
    else
    {
         /********* Step A2 ***************/
         if(1 == hw_bc11_stepA2())
         {
             /********* Step B2 ***************/
             if(1 == hw_bc11_stepB2())
             {
                 charger_tye = STANDARD_CHARGER;
                 pal_log_info("step B2 : STANDARD CHARGER!\r\n");
             }
             else
             {
                 charger_tye = CHARGING_HOST;
                 pal_log_info("step B2 :  Charging Host!\r\n");
             }
         }
         else
         {
             charger_tye = STANDARD_HOST;
             pal_log_info("step A2 : Standard USB Host!\r\n");
         }
    }

    /********* Finally setting ******************************
     * 关闭BC1.1相关电路
        释放资源，恢复默认状态
    */
    hw_bc11_done();

    return charger_tye;
}
#endif

/*
本驱动文件的入口
为什么它是入口？
它是唯一返回 CHARGER_TYPE 的公共函数。
其他模块（如 Bootloader 的电源初始化、Kernel 的 charger manager）会调用它来获取当前充电器类型。
它封装了内部检测逻辑，对外提供统一接口。

这个函数的作用是：在系统启动时，安全、准确地识别当前插入的充电器类型，只检测一次，并把结果缓存起来供后续使用，为正常充电打下基础。
*/
CHARGER_TYPE mt_charger_type_detection(void)
{
    if( g_first_check == 0 )
    {
        g_first_check = 1;

        /*
        upmu_is_chr_det(): 检测物理上是否有充电器插入
        如果没有检测到充电器，直接返回 CHARGER_UNKNOWN
        */
        if(upmu_is_chr_det() == KAL_FALSE)
            return CHARGER_UNKNOWN;

#ifdef MTK_EXT_CHGDET_SUPPORT
        mtk_ext_chgdet(&g_ret); //如果定义了 MTK_EXT_CHGDET_SUPPORT，说明使用外部充电检测芯片（如：UPM6910、BQ24196 等）来进行更精确的充电器识别。
#ifdef MULTI_EXT_MAIN_CHARGER
        pal_log_info("first chg get g_ret = %d\n",g_ret);
        if(!g_ret){
            second_mainchg_chgdet(&g_ret);
            pal_log_info("second chg get g_ret = %d\n",g_ret);
        }
#endif

#else
        g_ret = hw_charger_type_detection();
#endif
    }
    else
    {
        pal_log_info("[mt_charger_type_detection] Got data !!, %d, %d\r\n", g_charger_in_flag, g_first_check);
    }

    return g_ret;
}

#ifndef MT6375_BATTERY
int hw_check_battery(void)
{
#if CFG_FPGA_PLATFORM
    pal_log_info("FPGA ignore bat check !\n");
    return 1;
#else

#ifdef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
    pal_log_info("ignore bat check !\n");
    return 1;
#else
    U32 val = 0;
    U32 ret_val;
    ret_val = pmic_config_interface(PMIC_RG_LDO_VBIF28_EN_ADDR, 1,
                                    PMIC_RG_LDO_VBIF28_EN_MASK,
                                    PMIC_RG_LDO_VBIF28_EN_SHIFT);

    if (drdi_get_hw_ver() == HW_VER_V0) {
        pal_log_info("ignore bat check\n");
        return 1;
    } else {
        pmic_upmu_set_rg_baton_en(1);
        val = pmic_upmu_get_rgs_baton_undet();

        if (val == 0) {
            pal_log_info("bat is exist.\n");
            return 1;
        } else {
            pal_log_info("bat NOT exist.\n");
            return 0;
	}
    }
#endif /* MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION */
#endif /* CFG_FPGA_PLATFORM */
}
#endif /* MT6375_BATTERY */

void pl_charging(int en_chr)
{
    //TBD
}

void pl_kick_chr_wdt(void)
{
    //TBD
}

void pl_close_pre_chr_led(void)
{
    //no charger feature
}

int get_bat_sense_volt(int times)
{
	return pmic_get_auxadc_value(AUXADC_LIST_BATADC);
}

int get_i_sense_volt(int times)
{
	/*TODO: MT6355 doesn't have isense auxadc */
	return pmic_get_auxadc_value(AUXADC_LIST_BATADC);
}

#define R_CHARGER_1 330
#define R_CHARGER_2 39


int get_charger_volt(int times)
{
	int val = 0;
#ifdef MT6360_PMU
	mtk_get_vbus_adc(&val);
	val /= 1000;
#else
	val = pmic_get_auxadc_value(AUXADC_LIST_VCDT);
	val = (((R_CHARGER_1+R_CHARGER_2)*100*val)/R_CHARGER_2)/100;
#endif
	return val;
}

void kick_charger_wdt(void)
{
#if TBD
	pmic_config_interface(PMIC_RG_CHRWDT_TD_ADDR,3,PMIC_RG_CHRWDT_TD_MASK,PMIC_RG_CHRWDT_TD_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_WR_ADDR,1,PMIC_RG_CHRWDT_WR_MASK,PMIC_RG_CHRWDT_WR_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_INT_EN_ADDR,1,PMIC_RG_CHRWDT_INT_EN_MASK,PMIC_RG_CHRWDT_INT_EN_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_EN_ADDR,1,PMIC_RG_CHRWDT_EN_MASK,PMIC_RG_CHRWDT_EN_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_FLAG_WR_ADDR,1,PMIC_RG_CHRWDT_FLAG_WR_MASK,PMIC_RG_CHRWDT_FLAG_WR_SHIFT);
#endif /* TBD */
/*
	pmic_set_register_value(PMIC_RG_CHRWDT_TD,3);  // CHRWDT_TD, 32s for keep charging for lk to kernel
	pmic_set_register_value(PMIC_RG_CHRWDT_WR,1); // CHRWDT_WR
	pmic_set_register_value(PMIC_RG_CHRWDT_INT_EN,1);	// CHRWDT_INT_EN
	pmic_set_register_value(PMIC_RG_CHRWDT_EN,1);		// CHRWDT_EN
	pmic_set_register_value(PMIC_RG_CHRWDT_FLAG_WR,1);// CHRWDT_WR
*/
}

void pchr_turn_on_charging(kal_bool bEnable)
{
	/*pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 1, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);*/
	//pmic_set_register_value(PMIC_RG_USBDL_RST,1);//force leave USBDL mode
	//mt6325_upmu_set_rg_usbdl_rst(1);       //force leave USBDL mode
#if TBD
	pmic_config_interface(PMIC_RG_BC11_RST_ADDR, 1, PMIC_RG_BC11_RST_MASK, PMIC_RG_BC11_RST_SHIFT);

	kick_charger_wdt();

	pmic_config_interface(PMIC_RG_CS_VTH_ADDR,0xc,PMIC_RG_CS_VTH_MASK,PMIC_RG_CS_VTH_SHIFT);
#if MT6351
	pmic_config_interface(PMIC_RG_NORM_CS_VTH_ADDR,0xc,PMIC_RG_NORM_CS_VTH_MASK,PMIC_RG_NORM_CS_VTH_SHIFT);
#endif

	//pmic_set_register_value(PMIC_RG_CS_VTH,0xC);	// CS_VTH, 450mA
	//mt6325_upmu_set_rg_cs_vth(0xC);             // CS_VTH, 450mA

	pmic_config_interface(PMIC_RG_CSDAC_EN_ADDR,1,PMIC_RG_CSDAC_EN_MASK,PMIC_RG_CSDAC_EN_SHIFT);
	//pmic_set_register_value(PMIC_RG_CSDAC_EN,1);
	//mt6325_upmu_set_rg_csdac_en(1);				// CSDAC_EN

	pmic_config_interface(PMIC_RG_CHR_EN_ADDR,1,PMIC_RG_CHR_EN_MASK,PMIC_RG_CHR_EN_SHIFT);

#if MT6351
	pmic_config_interface(PMIC_RG_NORM_CHR_EN_ADDR,1,PMIC_RG_NORM_CHR_EN_MASK,PMIC_RG_NORM_CHR_EN_SHIFT);
#endif
	//pmic_set_register_value(PMIC_RG_CHR_EN,1);
	//mt6325_upmu_set_rg_chr_en(1);				// CHR_EN

        pmic_config_interface(PMIC_RG_CSDAC_MODE_ADDR,1,PMIC_RG_CSDAC_MODE_MASK,PMIC_RG_CSDAC_MODE_SHIFT);
        pmic_config_interface(PMIC_RG_CSDAC_EN_ADDR,1,PMIC_RG_CSDAC_EN_MASK,PMIC_RG_CSDAC_EN_SHIFT);
#endif /* TBD */
}

#define CUST_R_SENSE         68
int get_charging_current(int times)
{
	int ret;
	kal_int32 ADC_I_SENSE=1;   // 1 measure time
	kal_int32 ADC_BAT_SENSE=1; // 1 measure time
	int ICharging=0;

	ADC_I_SENSE=get_i_sense_volt(1);
	ADC_BAT_SENSE=get_bat_sense_volt(1);

	ICharging = (ADC_I_SENSE - ADC_BAT_SENSE )*1000/CUST_R_SENSE;

	return ICharging;
}

/*
调用时机：通常在 mt_charger_type_detection() 成功识别出充电器类型后调用。
目标场景：设备电池电量极低（甚至关机）时插入充电器，需要“先充一点电才能开机”。
核心任务：
检测当前电池电压是否太低（< 阈值）
如果太低，就主动开启充电
监控充电电流和电压是否正常
如果充电异常（如电压过高、电流过小），则停止充电并保护系统
等待电池电压回升到安全水平后再继续启动流程

                     [pl_check_bat_protect_status()]
                                      │
                      ┌───────────────┴───────────────┐
                      │   读取电池电压 bat_val         │
                      └───────────────┬───────────────┘
                                      ▼
                ┌──────────────────────────────────────┐
                │ bat_val < BATTERY_LOWVOL_THRESOLD ? │
                └────────────────────┬─────────────────┘
                                     │
                   否 ┌─────────────┴──────────────┐ 是
                      ▼                            ▼
             [结束，准备启动系统]     [进入充电保护循环]
                                              │
                        ┌─────────────────────┼─────────────────────┐
                        ▼                     ▼                     ▼
               [喂看门狗]      [是否有充电器？否→退出]   [VBUS电压过高？是→退出]
                                              │
                                    [开启充电 pchr_turn_on_charging(TRUE)]
                                              │
                              ┌───────────────┴───────────────┐
                              │       连续10次检测电流/电压     │
                              └───────────────┬───────────────┘
                                              │
                               多次电流<100mA?┌┴┐ 是
                                          ↓   │ │ 否
                                   [cnt >= 8?] │
                                          │   ↓
                                      是  ▼   [cnt=0, 继续]
                                    [关闭充电，退出]
                                          │
                                          ▼
                                 [延时50ms，更新bat_val]
                                          │
                                          └─────┐
                                                │
                                                ▼
                                       [回到 while 判断]

*/
void pl_check_bat_protect_status(void)
{
    kal_int32 bat_val = 0;
	int current,chr_volt,cnt=0,i;

    /*
    根据充电路径选择不同的电压检测函数
    参数5表示测量5次取平均值
    */
#if SWCHR_POWER_PATH
    bat_val = get_i_sense_volt(5);
#else
    bat_val = get_bat_sense_volt(5);
#endif

    chr_volt= get_charger_volt(1);
    pal_log_info( "[%s]: check VBAT=%d mV with %d mV, VCHR %d mV ,VCHR_HV=%d  start charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD,chr_volt,V_CHARGER_MAX);


    //VCDT_HV_VTH, 7V
    //pmic_config_interface(PMIC_RG_VCDT_HV_VTH_ADDR, V_CHARGER_MAX, PMIC_RG_VCDT_HV_VTH_MASK, PMIC_RG_VCDT_HV_VTH_SHIFT);
    //VCDT_HV_EN=1
   	//pmic_config_interface(PMIC_RG_VCDT_HV_EN_ADDR, 0x01, PMIC_RG_VCDT_HV_EN_MASK, PMIC_RG_VCDT_HV_EN_SHIFT);


    while (bat_val < BATTERY_LOWVOL_THRESOLD)
    {
        /*
        喂看门狗（防止死机）
        在长时间操作中必须定期喂狗，否则硬件看门狗会触发复位。
        因为此循环可能持续几秒甚至十几秒，所以每次循环都重启看门狗。
        */
        mtk_wdt_restart();
        if(upmu_is_chr_det() == KAL_FALSE)
        {
            pal_log_info( "[PL][BATTERY] No Charger, Power OFF !\n");
            break;
        }

		chr_volt= get_charger_volt(1);
		if(chr_volt>V_CHARGER_MAX)
		{
            pal_log_info( "[PL][BATTERY] charger voltage is too high :%d , threshold is %d !\n",chr_volt,V_CHARGER_MAX);
            break;
		}


        pchr_turn_on_charging(KAL_TRUE);


        /*
        监测充电电流和电压10次
    如果电流<100mA且电压<4400mV，认为是异常状态
        */
		cnt=0;
		for(i=0;i<10;i++)
		{
			current=get_charging_current(1);
			chr_volt=get_charger_volt(1);
			if(current<100 && chr_volt<4400)
			{
				cnt++;
				pal_log_info( "[PL][BATTERY] charging current=%d charger volt=%d\n\r",current,chr_volt);
			}
			else
			{
				pal_log_info( "[PL][BATTERY] charging current=%d charger volt=%d\n\r",current,chr_volt);
				cnt=0;
			}
		}

        /*
        如果10次检测中有8次异常，判定为充电故障
        关闭充电功能并退出循环
        */
		if(cnt>=8)
		{

	            pal_log_info( "[PL][BATTERY] charging current and charger volt too low !! \n\r",cnt);

	            pchr_turn_on_charging(KAL_FALSE);
	   			break;
		}
		mdelay(50);

#if SWCHR_POWER_PATH
		bat_val = get_i_sense_volt(5);
#else
		bat_val = get_bat_sense_volt(5);
#endif
		 pal_log_info( "[%s]: check VBAT=%d mV  chr=%d\n", __FUNCTION__, bat_val, chr_volt);
    }

    pal_log_info( "[%s]: check VBAT=%d mV with %d mV, stop charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD);
}
