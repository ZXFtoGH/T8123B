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
#if defined(DRAM_SLT)
#include "dramc_int_slt.h"
#endif

#if FOR_DV_SIMULATION_USED == 1
#include "dramc_dv_init.h"
#endif

#ifdef __DPM__
#include "dramc_dpm.h"
#include "timer.h"
#elif __ETT__
#include "common.h"
    #if !QT_GUI_Tool
    #include <barriers.h>
    #endif
#endif

#ifndef __DPM__
#if ((__ETT__==0) && (FOR_DV_SIMULATION_USED==0))
#include "typedefs.h"
#endif
#endif
/***********************************************************************/
/*                  Public Types                                       */
/***********************************************************************/

/*------------------------------------------------------------*/
/*                  macros, defines, typedefs, enums          */
/*------------------------------------------------------------*/
/************************** Common Macro *********************/

// choose a proper mcDELAY
#if defined(__DPM__)
#define mcDELAY_US(x)		timer_busy_wait_us(TIMER3, x)
#define mcDELAY_XUS(x)		timer_busy_wait_us(TIMER3, x)
#define mcDELAY_XNS(x)		timer_busy_wait_us(TIMER3, 1)
#define mcDELAY_MS(x)		timer_busy_wait_ms(TIMER3, x)
#elif (FOR_DV_SIMULATION_USED == 1)
#if defined(DUMP_INIT_RG_LOG_TO_DE)
#define mcDELAY_US(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_US(%d);\n",x); delay_us((U32) (1))
#define mcDELAY_MS(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_MS(%d);\n",x); delay_us((U32) (x))
#define mcDELAY_XUS(x)      if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_US(%d);\n",x); delay_us((U32) (x))
#define mcDELAY_XNS(x)      if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_US(1);\n"); delay_ns((U32) (x))
#else
#define mcDELAY_US(x)       delay_us(1)
#define mcDELAY_XUS(x)      delay_us(x)
#define mcDELAY_XNS(x)      delay_ns(x)
#define mcDELAY_MS(x)       delay_us(x)
#endif
#elif __ETT__
    #if defined(DUMP_INIT_RG_LOG_TO_DE)
    #define mcDELAY_US(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_US(%d);\n",x); GPT_Delay_us((U32) (x))
    #define mcDELAY_MS(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_MS(%d);\n",x); GPT_Delay_ms((U32) (x))
    #define mcDELAY_XUS(x)      if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_US(%d);\n",x); GPT_Delay_us((U32) (x))
    #define mcDELAY_XNS(x)      if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG("DELAY_US(1);\n"); GPT_Delay_us((U32) (1))
    #elif QT_GUI_Tool
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

#if FOR_DV_SIMULATION_USED
  #define mcPRINTF printf
#elif defined(__DPM__)
  #define mcPRINTF
#elif __ETT__
  #if QT_GUI_Tool
    #define mcPRINTF print_log_QT
    #define mcPRINTF_RG print_rgdump_QT
  #else
    #define mcPRINTF opt_print
    #define mcPRINTF_RG opt_print
  #endif
#else
  #define mcPRINTF print
#endif

#if FOR_DV_SIMULATION_USED
#ifdef DUMP_INIT_RG_LOG_TO_DE
    #define mcSHOW_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG(_x_, ...)   
    #define mcSHOW_DBG_MSG2(_x_, ...)  
    #define mcSHOW_DBG_MSG3(_x_, ...)  
    #define mcSHOW_DBG_MSG4(_x_, ...)  
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)  
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)   
    #define mcDUMP_REG_MSG(_x_, ...)
    #define mcSHOW_DUMP_INIT_RG_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
#else
    #define mcSHOW_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG2(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG3(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG4(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG5(_x_, ...) 
    #define mcSHOW_DBG_MSG6(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_EYESCAN_MSG(_x_, ...) 
    #define mcSHOW_TIME_MSG(_x_, ...) 
    #define mcSHOW_ERR_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcDUMP_REG_MSG(_x_, ...) 
    #define mcSHOW_DUMP_INIT_RG_MSG(_x_, ...) 
#endif    
#elif __ETT__
    #if (defined(DDR_INIT_TIME_PROFILING))
    #define mcSHOW_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...)
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_ERR_MSG(_x_, ...)
    #define mcDUMP_REG_MSG(_x_, ...)
    #elif DUMP_ALLSHU_RG
    #define mcSHOW_MSG(_x_, ...)     {mcDELAY_US(50);mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG(_x_, ...)     {mcDELAY_US(50);mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG2(_x_, ...)    //{mcDELAY_US(50);mcPRINTF _x_;}
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...)
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)     {mcDELAY_US(50);mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcDUMP_REG_MSG(_x_, ...)     {mcDELAY_US(50);mcPRINTF(_x_,##__VA_ARGS__);}
    #elif defined(RELEASE)
    #define mcSHOW_MSG(_x_, ...)  //{mcPRINTF _x_;}
    #define mcSHOW_DBG_MSG(_x_, ...)  //{mcPRINTF _x_;}
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_EYESCAN_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)
    #define mcDUMP_REG_MSG(_x_, ...)
    #elif VENDER_JV_LOG
    #define mcSHOW_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV
    #define mcSHOW_EYESCAN_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)
    #define mcDUMP_REG_MSG(_x_, ...)
    #elif defined(DUMP_INIT_RG_LOG_TO_DE)
    #define mcSHOW_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x, ...)
    #define mcSHOW_DUMP_INIT_RG_MSG(_x_, ...) {GPT_Delay_us(50); mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)
    #define mcDUMP_REG_MSG(_x_, ...)
    #elif MRW_CHECK_ONLY
    #define mcSHOW_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...)
    #define mcSHOW_MRW_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...) {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcDUMP_REG_MSG(_x_, ...)
    #else   // ETT real chip
    #define mcSHOW_MSG(_x_, ...)       {mcDELAY_US(10); mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG(_x_, ...)   {mcDELAY_US(10); mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG2(_x_, ...)  {mcDELAY_US(10); mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG3(_x_, ...)  {mcDELAY_US(10); mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)  {mcDELAY_US(10); mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_JV_LOG_MSG(_x_, ...)
    #define mcSHOW_EYESCAN_MSG(_x_, ...) {if (gEye_Scan_color_flag) {mcDELAY_US(200);};mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)   {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcDUMP_REG_MSG(_x_, ...)
    #endif
#else  // preloader
    #if defined(DDR_INIT_TIME_PROFILING)
    #define mcSHOW_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...)
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...)   {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_ERR_MSG(_x_, ...)
    #define mcDUMP_REG_MSG(_x_, ...)
    #elif defined(__DRAM_FLASH_TOOL_DA__)
    #define mcSHOW_MSG(_x_, ...)        {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG(_x_, ...)    {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...)
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)    {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcDUMP_REG_MSG(_x_, ...)
    #elif defined(TARGET_BUILD_VARIANT_ENG) //&& !defined(MTK_EFUSE_WRITER_SUPPORT) && !CFG_TEE_SUPPORT && !MTK_EMMC_SUPPORT
    #define mcSHOW_MSG(_x_, ...)       {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG(_x_, ...)   {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG2(_x_, ...)  {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG3(_x_, ...)  {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)  {mcPRINTF(_x_,##__VA_ARGS__);}
        #if CFG_DRAM_LOG_TO_STORAGE
        #define mcSHOW_EYESCAN_MSG(_x_, ...)  {mcPRINTF(_x_,##__VA_ARGS__);}
        #define mcSHOW_JV_LOG_MSG(_x_, ...)   {mcPRINTF(_x_,##__VA_ARGS__);}
        #else
        #define mcSHOW_EYESCAN_MSG(_x_, ...)
        #define mcSHOW_JV_LOG_MSG(_x_, ...)
        #endif
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)   {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcDUMP_REG_MSG(_x_, ...)
    #else
    #define mcSHOW_MSG(_x_, ...)         {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcSHOW_DBG_MSG(_x_, ...)
    #define mcSHOW_DBG_MSG2(_x_, ...)
    #define mcSHOW_DBG_MSG3(_x_, ...)
    #define mcSHOW_DBG_MSG4(_x_, ...)
    #define mcSHOW_DBG_MSG5(_x_, ...)
    #define mcSHOW_DBG_MSG6(_x_, ...)
    #define mcSHOW_JV_LOG_MSG(_x_, ...)
    #define mcSHOW_EYESCAN_MSG(_x_, ...)
    #define mcSHOW_TIME_MSG(_x_, ...)
    #define mcSHOW_ERR_MSG(_x_, ...)     {mcPRINTF(_x_,##__VA_ARGS__);}
    #define mcDUMP_REG_MSG(_x_, ...)
    #endif
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)    (sizeof (x) / sizeof (x[0]))
#endif

#ifndef MIN
#define MIN(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#endif

#ifndef MAX
#define MAX(_x, _y) ((_x) > (_y) ? (_x) : (_y))
#endif

#define enter_function() \
	({mcSHOW_DBG_MSG("enter %s\n", __FUNCTION__);})

#define exit_function() \
	({mcSHOW_DBG_MSG("exit %s\n", __FUNCTION__);})

extern int dump_log;
#endif   // _DRAMC_COMMON_H_
