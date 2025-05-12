/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Delete unused code.
*
*
*******************************************************************************/

#include "kal_release.h"
#include "register_act_util.h"
#include "usb_debug_tool.h"

#if 0
static  Usb_Dbg_Struct  USB_Dbg_Info;
#endif

/* debug trace */
void USB_Dbg_Trace(USB_DBG_INFO tag, kal_uint32 time, kal_uint32 data1, kal_uint32 data2)
{
#if 0
   USB_Dbg_Info.dbg_data[USB_Dbg_Info.dbg_data_idx&(USB_DBG_DATA_SIZE-1)].tag = tag;
   USB_Dbg_Info.dbg_data[USB_Dbg_Info.dbg_data_idx&(USB_DBG_DATA_SIZE-1)].time = time;
   USB_Dbg_Info.dbg_data[USB_Dbg_Info.dbg_data_idx&(USB_DBG_DATA_SIZE-1)].data1 = data1;
   USB_Dbg_Info.dbg_data[USB_Dbg_Info.dbg_data_idx&(USB_DBG_DATA_SIZE-1)].data2 = data2;
   USB_Dbg_Info.dbg_data_idx++;
#endif
}


