/*
 *
 * DRAMC_COMMON.H
 *
 */

#ifndef _DRAMC_COMMON_H_
#define _DRAMC_COMMON_H_

#include <string.h>
#include "dramc_register.h"
#include "dramc_pi_api.h"
#ifdef __MD32__
#include "dramc_dpm.h"
#endif

#if FOR_DV_SIMULATION_USED == 1
#include "dramc_dv_init.h"
#endif

#if __ETT__
#include "common.h"
#endif

/***********************************************************************/
/*                  Public Types                                       */
/***********************************************************************/

/*------------------------------------------------------------*/
/*                  macros, defines, typedefs, enums          */
/*------------------------------------------------------------*/
/************************** Common Macro *********************/

// choose a proper mcDELAY
#if defined(__MD32__)
#define mcDELAY_US(x)		timer_busy_wait_us(TIMER3, x)
#define mcDELAY_XUS(x)		timer_busy_wait_us(TIMER3, x)
#define mcDELAY_XNS(x)		timer_busy_wait_us(TIMER3, 1)
#define mcDELAY_MS(x)		timer_busy_wait_ms(TIMER3, x)
#elif __ETT__
#if defined(DUMP_INIT_RG_LOG_TO_DE)
#define mcDELAY_US(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_US(%d);\n",x)); GPT_Delay_us((U32) (x))
#define mcDELAY_MS(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_MS(%d);\n",x)); GPT_Delay_ms((U32) (x))
#define mcDELAY_XUS(x)      if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_US(%d);\n",x)); GPT_Delay_us((U32) (x))
#define mcDELAY_XNS(x)      if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_US(1);\n")); GPT_Delay_us((U32) (1))
#elif (FOR_DV_SIMULATION_USED == 1)
#define mcDELAY_US(x)       delay_us(1)
#define mcDELAY_XUS(x)      delay_us(x)
#define mcDELAY_XNS(x)      delay_ns(x)
#define mcDELAY_MS(x)       delay_us(x)
#elif QT_GUI_Tool==1
#include "windows.h"
#define mcDELAY_US(x)       Sleep((UINT32) (1))
#define mcDELAY_XUS(x)      Sleep((UINT32) (1))
#define mcDELAY_XNS(x)      Sleep((UINT32) (1))
#define mcDELAY_MS(x)       Sleep((UINT32) (x))
#else
#define mcDELAY_US(x)       GPT_Delay_us((U32) (x))
#define mcDELAY_MS(x)       GPT_Delay_ms((U32) (x))
#define mcDELAY_XUS(x)      GPT_Delay_us((U32) (x))
#define mcDELAY_XNS(x)      GPT_Delay_us((U32) (1))
#endif
#else // preloader
#define mcDELAY_US(x)       gpt_busy_wait_us(x)
#define mcDELAY_MS(x)       gpt_busy_wait_us(x*1000)
#define mcDELAY_XUS(x)      gpt_busy_wait_us(x)
#define mcDELAY_XNS(x)      gpt_busy_wait_us(1)
#endif //endif __ETT__


/**********************************************/
/* Priority of debug log                      */
/*--------------------------------------------*/
/* mcSHOW_DBG_MSG: High                       */
/* mcSHOW_DBG_MSG2: Medium High               */
/* mcSHOW_DBG_MSG3: Medium Low                */
/* mcSHOW_DBG_MSG4: Low                       */
/**********************************************/
#if __FLASH_TOOL_DA__
  #define printf DBG_MSG
  #define print DBG_MSG
#elif defined(RELEASE)
#if !__ETT__
  #undef printf
  #define printf
  #undef print
  #define print
#endif
#endif

#if __ETT__
    #if QT_GUI_Tool
        #if MRW_CHECK_ONLY
        #define mcSHOW_DBG_MSG_tmp(...) {printf (__VA_ARGS__);  if(fp_A60868){fprintf (fp_A60868,__VA_ARGS__);}}
        #define mcSHOW_DBG_MSG_Dump(...)
        #define mcSHOW_DBG_MSG(_x_)
        #define mcSHOW_DBG_MSG2(_x_)
        #define mcSHOW_DBG_MSG3(_x_)
        #define mcSHOW_DBG_MSG4(_x_)
        #define mcSHOW_DBG_MSG5(_x_)
        #define mcSHOW_EYESCAN_MSG(_x_)
        #define mcSHOW_TIME_MSG(_x_)
        #define mcSHOW_ERR_MSG(_x_)
        #define mcDUMP_REG_MSG(_x_)
        #define mcSHOW_DUMP_INIT_RG_MSG(_x_)
        #define mcSHOW_MRW_MSG(_x_)    {mcSHOW_DBG_MSG_tmp _x_;}
        #else
        #define mcSHOW_DBG_MSG_tmp(...)   {printf (__VA_ARGS__);  if(fp_A60868){fprintf (fp_A60868,__VA_ARGS__);}}
        #define mcSHOW_DBG_MSG_Dump(...)  {if(fp_A60868_RGDump){fprintf (fp_A60868_RGDump,__VA_ARGS__);}}
        #define mcSHOW_DBG_MSG(_x_)   {mcSHOW_DBG_MSG_tmp _x_;}
        #define mcSHOW_DBG_MSG2(_x_)  {mcSHOW_DBG_MSG_tmp _x_;}
        #define mcSHOW_DBG_MSG3(_x_)  {mcSHOW_DBG_MSG_tmp _x_;}
        #define mcSHOW_DBG_MSG4(_x_)  {mcSHOW_DBG_MSG_tmp _x_;}
        #define mcSHOW_DBG_MSG5(_x_)
        #define mcSHOW_EYESCAN_MSG(_x_) {mcSHOW_DBG_MSG_tmp _x_;}
        #define mcSHOW_TIME_MSG(_x_)
        #define mcSHOW_ERR_MSG(_x_)   {mcSHOW_DBG_MSG_tmp _x_;}
        #define mcDUMP_REG_MSG(_x_)   {mcSHOW_DBG_MSG_Dump _x_;}
        #define mcSHOW_DUMP_INIT_RG_MSG(_x_)
        #endif
    #elif (defined(DDR_INIT_TIME_PROFILING))
    #define mcSHOW_DBG_MSG(_x_)
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)   {opt_print _x_;}
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #elif DUMP_ALLSUH_RG
    #define mcSHOW_DBG_MSG(_x_)     {mcDELAY_US(10); opt_print _x_;}
    #define mcSHOW_DBG_MSG2(_x_)    {mcDELAY_US(10); opt_print _x_;}
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_) {mcDELAY_US(50);opt_print _x_;}
    #define mcDUMP_REG_MSG(_x_) {mcDELAY_US(50);opt_print _x_;}
    #elif defined(RELEASE)
    #define mcSHOW_DBG_MSG(_x_)  //{opt_print _x_;}
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_) {opt_print _x_;}
    #define mcSHOW_EYESCAN_MSG(_x_) {opt_print _x_;}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #elif VENDER_JV_LOG
    #define mcSHOW_DBG_MSG(_x_)
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_)    {opt_print _x_;}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV
    #define mcSHOW_EYESCAN_MSG(_x_) {opt_print _x_;}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #elif FOR_DV_SIMULATION_USED
    #define mcSHOW_DBG_MSG(_x_)   {printf _x_;}
    #define mcSHOW_DBG_MSG2(_x_)  {printf _x_;}
    #define mcSHOW_DBG_MSG3(_x_)  {printf _x_;}
    #define mcSHOW_DBG_MSG4(_x_)  {printf _x_;}
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)   {printf _x_;}
    #define mcDUMP_REG_MSG(_x_)
    #define mcSHOW_DUMP_INIT_RG_MSG(_x_)
    #elif SW_CHANGE_FOR_SIMULATION
    #define mcSHOW_DBG_MSG(_x_)
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #elif defined(DUMP_INIT_RG_LOG_TO_DE)
    #define mcSHOW_DBG_MSG(_x_)
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_DUMP_INIT_RG_MSG(_x_) {GPT_Delay_us(50); EdbgOutputDebugString _x_;}
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #elif MRW_CHECK_ONLY
    #define mcSHOW_DBG_MSG(_x_)
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_MRW_MSG(_x_)    {printf _x_;}
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #else   // ETT real chip
    #define mcSHOW_DBG_MSG(_x_)   {mcDELAY_US(10); opt_print _x_;}
    #define mcSHOW_DBG_MSG2(_x_)  {mcDELAY_US(10); opt_print _x_;}
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_) {if (gEye_Scan_color_flag) {mcDELAY_US(200);};opt_print _x_;}
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)   {opt_print _x_;}
    #define mcDUMP_REG_MSG(_x_)
    #endif
#else  // preloader
    #if defined(DDR_INIT_TIME_PROFILING)
    #define mcSHOW_DBG_MSG(_x_)
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)   {print _x_;}
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #elif defined(TARGET_BUILD_VARIANT_ENG) //&& !defined(MTK_EFUSE_WRITER_SUPPORT) && !CFG_TEE_SUPPORT && !MTK_EMMC_SUPPORT
    #define mcSHOW_DBG_MSG(_x_)   {print _x_;}
    #define mcSHOW_DBG_MSG2(_x_)  {print _x_;}
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)
        #if (CFG_DRAM_LOG_TO_STORAGE)
        #define mcSHOW_EYESCAN_MSG(_x_)  {print _x_;}
        #define mcSHOW_JV_LOG_MSG(_x_)   {print _x_;}
        #else
        #define mcSHOW_EYESCAN_MSG(_x_)
        #define mcSHOW_JV_LOG_MSG(_x_)
        #endif
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)   {print _x_;}
    #define mcDUMP_REG_MSG(_x_)
    #else
    #define mcSHOW_DBG_MSG(_x_)
    #define mcSHOW_DBG_MSG2(_x_)
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)     {print _x_;}
    #define mcDUMP_REG_MSG(_x_)
    #endif
#endif



#if QT_GUI_Tool ==1
#define mcFPRINTF(_x_)     fprintf _x_;
#else
#define mcFPRINTF(_x_)
#endif

#define ARRAY_SIZE(x)    (sizeof (x) / sizeof (x[0]))

#define enter_function() \
	({mcSHOW_DBG_MSG(("enter %s\n", __FUNCTION__));})

#define exit_function() \
	({mcSHOW_DBG_MSG(("exit %s\n", __FUNCTION__));})

extern int dump_log;
#endif   // _DRAMC_COMMON_H_
