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

#include "usb_comm.h"
#include "usb_drv.h"
#include "usb.h"
#include "usb_resource.h"
#include "usbacm_drv.h"

#include "usb_mode.h"
#include "usb_custom.h"
#include "usb_debug_tool.h"



/* static functions */

/************************************************************
configure mode  functions
*************************************************************/

/* The enable parameter decides whether to turn on D+ at this time */
void USB_Config_Type(USB_DEVICE_TYPE type, kal_bool b_enable, kal_uint32 *parameter)
{
   gUsbDevice.usb_config_result = KAL_TRUE;

   /* initailize CDC_ACM */
   USB_Register_CreateFunc("ACM DATA", USB_Acm_DataIf_Create, USB_Acm_DataIf_Reset,
      USB_Acm_DataIf_Speed_Reset);
   USB_Register_CreateFunc("ACM COMMU.", USB_Acm_CommIf_Create, USB_Acm_CommIf_Reset,
      USB_Acm_CommIf_Speed_Reset);
   USB_Register_Device_Code(USB_ACM_DEVICE_CODE, USB_ACM_SUBCLASS_CODE,
      USB_ACM_PROTOCOL_CODE, g_UsbACM.acm_param->desc_product);

   USB_Init(USB_CDC_ACM, b_enable);
}

/* different order for COMMU / DATA interface */
void USB_Change_Interface_Sequence(void)
{
   USB_DeRegister_CreateFunc();
   USB_Register_CreateFunc("ACM COMMU.", USB_Acm_CommIf_Create, USB_Acm_CommIf_Reset,
      USB_Acm_CommIf_Speed_Reset);
   USB_Register_CreateFunc("ACM DATA", USB_Acm_DataIf_Create, USB_Acm_DataIf_Reset,
      USB_Acm_DataIf_Speed_Reset);
   USB_Software_Create();
}

#if 1 // obseleted
Usb_Mode_Struct g_UsbMode;
#endif
