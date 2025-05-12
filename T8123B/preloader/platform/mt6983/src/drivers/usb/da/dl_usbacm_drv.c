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

//#define USB_DBG_ON

#include "kal_release.h"
#include "register_act_util.h"
#include "usb_hw.h"
#include "usb_comm.h"
#include "usb_drv.h"
#include "usb.h"
#include "usb_resource.h"
#include "usbacm_drv.h"
#include "usbacm_adap.h"
#include "usb_custom.h"
#include "usb_debug_tool.h"
#include "debug.h"
#include "assert.h"
#include "lib/string.h"


/* global variables */
UARTStruct USB2UARTPort;
UsbAcm_Struct g_UsbACM;

kal_bool gUSBAcm_IsInEPComplete;


/* static functions */
static void USB_Acm_Ep0_SetLineCoding(void *data);
static void USB_Acm_Ep0_Command(Usb_Ep0_Status* pep0state,Usb_Command* pcmd);
static void USB_Acm_BulkOut_Hdr(void);
static void USB_Acm_BulkIn_Reset(void);
static void USB_Acm_BulkOut_Reset(void);
//static void USB_Acm_FT_BulkOut_Hdr(void );
static void USB_Acm_BulkIn_Hdr(void);

/************************************************************
interface initialization functions
*************************************************************/

/* communication interface create function, prepare descriptor */
void USB_Acm_CommIf_Create(void *ifname)
{
   kal_uint8 ep_id;
   kal_uint8 if_id;
   Usb_Cdc_If_Dscr Dscr_temp;

   /* get resource number and register to gUsbDevice */
   g_UsbACM.comm_if_info = USB_Get_Interface_Number(&if_id);
   g_UsbACM.comm_ep_intr_info = USB_Get_Intr_Ep(&ep_id); /* number is 3  */

   /* record interface name and interface descriptor length */
   g_UsbACM.comm_if_info->interface_name_ptr = (kal_char *)ifname;
   g_UsbACM.comm_if_info->ifdscr_size = USB_CDC_IF_LENGTH;
   g_UsbACM.comm_ep_intr_info->epdscr_size = USB_EPDSC_LENGTH;

   /* related endpoint info and class specific command handler*/
   g_UsbACM.comm_if_info->ep_info[0] = (Usb_Ep_Info*)g_UsbACM.comm_ep_intr_info;
   g_UsbACM.comm_if_info->if_class_specific_hdlr = USB_Acm_Ep0_Command;

   memset((void*)&Dscr_temp,0x0,sizeof(Usb_Cdc_If_Dscr));

   Dscr_temp.bLength = USB_IFDSC_LENGTH;
   Dscr_temp.bDescriptorType = USB_INTERFACE;
   Dscr_temp.bInterfaceNumber = if_id;
   Dscr_temp.bAlternateSetting = 0;
   Dscr_temp.bNumEndpoints = USB_CDC_ACM_COMM_EP_NUMBER;
   Dscr_temp.bInterfaceClass = USB_ACM_COMM_INTERFACE_CLASS_CODE;
   Dscr_temp.bInterfaceSubClass = USB_ACM_COMM_INTERFACE_SUBCLASS_CODE;
   Dscr_temp.bInterfaceProtocol = USB_ACM_COMM_INTERFACE_PROTOCOL_CODE;
   Dscr_temp.iInterface = 0x00;

   Dscr_temp.HFbFunctionLength = 0x05;
   Dscr_temp.HFbDescriptorType = 0x24;
   Dscr_temp.HFbDescriptorSubtype = 0x00;
   Dscr_temp.bcdCDC = 0x0110;

   Dscr_temp.ACMFbFunctionLength = 0x04;
   Dscr_temp.ACMFbDescriptorType = 0x24;
   Dscr_temp.ACMFbDescriptorSubtype = 0x02;
   Dscr_temp.ACMFbmCapabilities = 0x0f;

   Dscr_temp.UFbFunctionLength = 0x05;
   Dscr_temp.UFbDescriptorType = 0x24;
   Dscr_temp.UFbDescriptorSubtype = 0x06;
   Dscr_temp.bMasterInterface = if_id;
   Dscr_temp.bSlaveInterface0 = g_UsbACM.data_interface_id;

   Dscr_temp.CMFbFunctionLength = 0x05;
   Dscr_temp.CMFbDescriptorType = 0x24;
   Dscr_temp.CMFbDescriptorSubtype = 0x01;
   Dscr_temp.CMFbmCapabilities = 0x03;
   Dscr_temp.bDataInterface = g_UsbACM.data_interface_id;

   memcpy(&g_UsbACM.comm_if_info->ifdscr.classif,&Dscr_temp,sizeof(Usb_Cdc_If_Dscr));

   /* endpoint handler*/
   USB_Register_Drv_Info(USB_DRV_HDLR_EP_IN, ep_id, NULL);
   g_UsbACM.comm_ep_intr_info->ep_reset = NULL;

   /* endpoint descriptor */
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.bLength = USB_EPDSC_LENGTH;
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.bDescriptorType = USB_ENDPOINT;
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_IN | ep_id);	/*InPipe*/
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.bmAttributes = USB_EP_INTR;

#ifdef USB_IP_U3
   /* endpoint company descriptor */
   g_UsbACM.comm_ep_intr_info->stdep_ss_comp.bLength = USB_SS_COM_EPDSC_LENGTH;
   g_UsbACM.comm_ep_intr_info->stdep_ss_comp.bDescriptorType = USB_SS_ENDPOINT_COMP;

   /* need to confirm value, 04404 */
   g_UsbACM.comm_ep_intr_info->stdep_ss_comp.bMaxBurst = 0;
   g_UsbACM.comm_ep_intr_info->stdep_ss_comp.bmAttributes = 0;
   g_UsbACM.comm_ep_intr_info->stdep_ss_comp.wBytesPerInterval = 0;
#endif

#ifndef __USB_HS_ENABLE__
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.wMaxPacketSize[0] = USB_INTR_MAXP&0xff; 		/*HW-dependent*/
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.wMaxPacketSize[1] = (USB_INTR_MAXP>>8)&0xff;
#endif

   g_UsbACM.comm_ep_intr_info->epdesc.stdep.bInterval = 1;
   g_UsbACM.comm_ep_intr_info->ep_status.epin_status.byEP = ep_id;
   g_UsbACM.comm_ep_intr_info->ep_status.epin_status.nBytesLeft = USB_EP_NODATA;
}

/* communication interface reset function, should enable EP, but we do not use this interrupt EP so not enable it */
void USB_Acm_CommIf_Reset(void)
{
   g_UsbACM.intrpipe = &g_UsbACM.comm_ep_intr_info->ep_status.epin_status;
}
/* communication interface speed reset function, enable EP's speed-specific descriptor */
void USB_Acm_CommIf_Speed_Reset(kal_bool b_other_speed)
{
#ifdef __USB_HS_ENABLE__
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.wMaxPacketSize[0] = USB_EP_INTR_MAXP_FS&0xff;
   g_UsbACM.comm_ep_intr_info->epdesc.stdep.wMaxPacketSize[1] = 0x00;
#endif
}
/* data interface create function, prepare descriptor */
void USB_Acm_DataIf_Create(void *ifname)
{
   kal_uint8 ep_in_id;
   kal_uint8 ep_out_id;
   kal_uint8 if_id;

   /* get resource number and register to gUsbDevice */
   g_UsbACM.data_if_info = USB_Get_Interface_Number(&if_id);
   g_UsbACM.data_interface_id = if_id;
   g_UsbACM.data_ep_in_info = USB_Get_BulkIn_Ep(&ep_in_id);
   g_UsbACM.data_ep_out_info = USB_Get_BulkOut_Ep(&ep_out_id);

   /* record interface name and interface descriptor length */
   g_UsbACM.data_if_info->interface_name_ptr = (kal_char *)ifname;
   g_UsbACM.data_if_info->ifdscr_size = USB_IFDSC_LENGTH;
   g_UsbACM.data_ep_in_info->epdscr_size = USB_EPDSC_LENGTH;
   g_UsbACM.data_ep_out_info->epdscr_size = USB_EPDSC_LENGTH;

   /* related endpoint info and class specific command handler*/
   g_UsbACM.data_if_info->ep_info[0] = (Usb_Ep_Info*)g_UsbACM.data_ep_in_info;
   g_UsbACM.data_if_info->ep_info[1] = (Usb_Ep_Info*)g_UsbACM.data_ep_out_info;
   g_UsbACM.data_if_info->if_class_specific_hdlr = NULL;

   /* standard interface descriptor */
   g_UsbACM.data_if_info->ifdscr.stdif.bLength = USB_IFDSC_LENGTH;
   g_UsbACM.data_if_info->ifdscr.stdif.bDescriptorType = USB_INTERFACE;
   g_UsbACM.data_if_info->ifdscr.stdif.bInterfaceNumber = if_id;
   g_UsbACM.data_if_info->ifdscr.stdif.bAlternateSetting = 0;
   g_UsbACM.data_if_info->ifdscr.stdif.bNumEndpoints = USB_CDC_ACM_DATA_EP_NUMBER;
   g_UsbACM.data_if_info->ifdscr.stdif.bInterfaceClass = USB_ACM_DATA_INTERFACE_CLASS_CODE;
   g_UsbACM.data_if_info->ifdscr.stdif.bInterfaceSubClass = USB_ACM_DATA_INTERFACE_SUBCLASS_CODE;
   g_UsbACM.data_if_info->ifdscr.stdif.bInterfaceProtocol = USB_ACM_DATA_INTERFACE_PROTOCOL_CODE;
   g_UsbACM.data_if_info->ifdscr.stdif.iInterface = 0x00;
   //USB_Get_String_Number((void *)g_UsbACM.acm_param->data_interface_string);

   /* endpoint handler */
   USB_Register_Drv_Info(USB_DRV_HDLR_EP_IN, ep_in_id, USB_Acm_BulkIn_Hdr);
   g_UsbACM.data_ep_in_info->ep_reset = USB_Acm_BulkIn_Reset;

   USB_Register_Drv_Info(USB_DRV_HDLR_EP_OUT, ep_out_id, USB_Acm_BulkOut_Hdr);
   g_UsbACM.data_ep_out_info->ep_reset = USB_Acm_BulkOut_Reset;


   /* endpoint descriptor */
   g_UsbACM.data_ep_in_info->epdesc.stdep.bLength = USB_EPDSC_LENGTH;
   g_UsbACM.data_ep_in_info->epdesc.stdep.bDescriptorType = USB_ENDPOINT;
   g_UsbACM.data_ep_in_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_IN | ep_in_id);   /*InPipe*/
   g_UsbACM.data_ep_in_info->epdesc.stdep.bmAttributes = USB_EP_BULK;

#ifdef USB_IP_U3
   /* endpoint company descriptor */
   g_UsbACM.data_ep_in_info->stdep_ss_comp.bLength = USB_SS_COM_EPDSC_LENGTH;
   g_UsbACM.data_ep_in_info->stdep_ss_comp.bDescriptorType = USB_SS_ENDPOINT_COMP;

   /* need to confirm value, 04404 */
   g_UsbACM.data_ep_in_info->stdep_ss_comp.bMaxBurst = EPX_BURST;
   g_UsbACM.data_ep_in_info->stdep_ss_comp.bmAttributes = 0;
   g_UsbACM.data_ep_in_info->stdep_ss_comp.wBytesPerInterval = 0;
#endif

   if (USB_IS_High_Speed() == KAL_FALSE)
   {
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_MAXP & 0xff;
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_MAXP >> 8)&0xff;
   }
   else
   {
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_HS_MAXP & 0xff;
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_HS_MAXP >> 8)&0xff;
   }

   //LOGD("maxp in: %x, %x\n", g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[0], g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1]);

   g_UsbACM.data_ep_in_info->epdesc.stdep.bInterval = 0;
   g_UsbACM.data_ep_in_info->ep_status.epin_status.byEP = ep_in_id;
   g_UsbACM.data_ep_in_info->ep_status.epin_status.nBytesLeft = USB_EP_NODATA;


   g_UsbACM.data_ep_out_info->epdesc.stdep.bLength = USB_EPDSC_LENGTH;
   g_UsbACM.data_ep_out_info->epdesc.stdep.bDescriptorType = USB_ENDPOINT;
   g_UsbACM.data_ep_out_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_OUT | ep_out_id);   /*OutPipe*/
   g_UsbACM.data_ep_out_info->epdesc.stdep.bmAttributes = USB_EP_BULK;

#ifdef USB_IP_U3
   g_UsbACM.data_ep_out_info->stdep_ss_comp.bLength = USB_SS_COM_EPDSC_LENGTH;
   g_UsbACM.data_ep_out_info->stdep_ss_comp.bDescriptorType = USB_SS_ENDPOINT_COMP;

   /* need to confirm value, 04404 */
   g_UsbACM.data_ep_out_info->stdep_ss_comp.bMaxBurst = EPX_BURST;
   g_UsbACM.data_ep_out_info->stdep_ss_comp.bmAttributes = 0;
   g_UsbACM.data_ep_out_info->stdep_ss_comp.wBytesPerInterval = 0;
#endif


   if (USB_IS_High_Speed() == KAL_FALSE)
   {
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_MAXP & 0xff;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_MAXP >> 8)&0xff;
   }
   else
   {
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_HS_MAXP & 0xff;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_HS_MAXP >> 8)&0xff;
   }

   //LOGD("maxp out: %x, %x\n", g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[0], g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[1]);

   g_UsbACM.data_ep_out_info->epdesc.stdep.bInterval = 0;
   g_UsbACM.data_ep_out_info->ep_status.epout_status.byEP = ep_out_id;

}

/* data interface reset function, enable EP*/
void USB_Acm_DataIf_Reset(void)
{
   g_UsbACM.txpipe = &g_UsbACM.data_ep_in_info->ep_status.epin_status;
   g_UsbACM.rxpipe = &g_UsbACM.data_ep_out_info->ep_status.epout_status;


   USB_InEPEn(g_UsbACM.data_ep_in_info->ep_status.epin_status.byEP, KAL_FALSE);
   USB_OutEPEn(g_UsbACM.data_ep_out_info->ep_status.epout_status.byEP, KAL_FALSE);
}


/* data interface speed reset function, enable EP's speed-specific descriptor */
void USB_Acm_DataIf_Speed_Reset(kal_bool b_other_speed)
{
#ifdef __USB_HS_ENABLE__

#ifdef USB_IP_U3
   if(USB_IS_Super_Speed() == KAL_TRUE)
   {
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_SS_MAXP&0xff;
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_SS_MAXP>>8)&0xff;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_SS_MAXP&0xff;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_SS_MAXP>>8)&0xff;
      return;
   }
#endif

   if(USB_IS_High_Speed() == KAL_TRUE)
   {
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_HS_MAXP&0xff;
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_HS_MAXP>>8)&0xff;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_HS_MAXP&0xff;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[1] = (USB_BULK_HS_MAXP>>8)&0xff;
   }
   else
   {
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_MAXP&0xff;
      g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = 0x00;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_MAXP&0xff;
      g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[1] = 0x00;

   }
#endif
}

/************************************************************
global variable g_UsbACM initialize and release functions
*************************************************************/

/* initialize global variable g_UsbACM */
void USB_Init_Acm_Status(void)
{
   g_UsbACM.txpipe = NULL;
   g_UsbACM.rxpipe = NULL;
   g_UsbACM.intrpipe = NULL;
   g_UsbACM.data_interface_id = 0;
}



/************************************************************
EP0 handle functions
************************************************************/

/* set USB request line coding to UART DCB structure */
static void USB_Acm_Ep0_SetLineCoding(void *data)
{
   kal_uint32 nCount;

   /* read setting and set to UART structure*/
   nCount = USB_EP0_Pkt_Len();
   if(nCount == 7)
   {
      USB_EPFIFORead(0, nCount, &g_UsbACM.line_coding);
#if defined(USB_IP_U3)
      //u3 chip no RX status, so we change to IDLE state directly
      gUsbDevice.ep0_state = USB_EP0_IDLE;
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_RX_END, KAL_FALSE, KAL_TRUE);
#else
      gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
      USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, KAL_FALSE, KAL_TRUE);
#endif

   }

}


/* parse class specific request */
static void USB_Acm_Ep0_Command(Usb_Ep0_Status* pep0state,Usb_Command* pcmd)
{
   kal_uint8 bError = KAL_FALSE;

   MSG(USB, "USB ACM EP0 Class Request: %x\n", pcmd->bRequest);
   switch (pcmd->bRequest)
   {
   case USB_ACM_SEND_ENCAPSULATED_COMMAND:
      MSG(USB, "   SEND_ENCAPSULATED_COMMAND\n");
      /* not support */
      ASSERT(0);
      bError = KAL_TRUE;
      break;
   case USB_ACM_GET_ENCAPSULATED_RESPONSE:
      MSG(USB, "   SEND_ENCAPSULATED_RESPONSE\n");
      /* not support */
      ASSERT(0);
      bError = KAL_TRUE;
      break;
   case USB_ACM_SET_LINE_CODING:
      MSG(USB, "   SET_LINE_CODING\n");
      /* register handler to handle the get data*/
      USB_Dbg_Trace(DEBUG_USB_ACM_SET_LINE_CODING, 0, 0,0);
      USB_Register_EP0_RxHdlr(USB_Acm_Ep0_SetLineCoding);
      gUsbDevice.ep0_state =USB_EP0_RX;
      break;
   case USB_ACM_GET_LINE_CODING:
      MSG(USB, "   GET_LINE_CODING\n");
      /* tell host the current setting */
      USB_Dbg_Trace(DEBUG_USB_ACM_GET_LINE_CODING, 0, 0,0);
      g_UsbACM.line_coding.bDataBits = 8;
      USB_Generate_EP0Data(pep0state, pcmd, &g_UsbACM.line_coding, 7);
      break;
   case USB_ACM_SET_CONTROL_LINE_STATE:
      MSG(USB, "   SET_CONTROL_LINE_STATE\n");
      MSG(USB, "   bmRequestType = %x\n", pcmd->bmRequestType);
      MSG(USB, "   bRequest      = %x\n", pcmd->bRequest);
      MSG(USB, "   wValue        = %x\n", pcmd->wValue);
      MSG(USB, "   wIndex        = %x\n", pcmd->wIndex);
      MSG(USB, "   wLength       = %x\n", pcmd->wLength);
      /* do not need to do anything */
      break;
   case USB_ACM_SEND_BREAK:
      MSG(USB, "   SEND_BREAK\n");
      /* do not need to do anything */
      break;
   case USB_ACM_SET_COMM_FEATURE:
      MSG(USB, "   SET_COMM_FEATURE\n");
      /* not support */
      ASSERT(0);
      bError = KAL_TRUE;
      break;
   case USB_ACM_GET_COMM_FEATURE:
      MSG(USB, "   GET_COMM_FEATURE\n");
      ASSERT(0);
      bError = KAL_TRUE;
      break;
   case USB_ACM_CLEAR_COMM_FEATURE:
      MSG(USB, "   CLEAR_COMM_FEATURE\n");
      ASSERT(0);
      bError = KAL_TRUE;
      break;
   default:
      MSG(USB, "   NOT RECOGNIZED\n");
      ASSERT(0);
      bError = KAL_TRUE;
      break;
   }

   /* Stall command if an error occured */
   if (gUsbDevice.ep0_state == USB_EP0_IDLE)
   {
#if defined(USB_IP_U3)
      gUsbDevice.ep0_state = USB_EP0_IDLE;
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_END, bError, KAL_TRUE);
#else
      gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
      USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, bError, KAL_TRUE);
#endif
   }
   else
   {
#if defined(USB_IP_U3)
      if( gUsbDevice.ep0_state == USB_EP0_RX)
         USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_TO_RX, bError, KAL_FALSE);
      else
         USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_TO_TX, bError, KAL_FALSE);
#else
      USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, bError, KAL_FALSE);
#endif
   }
}


/************************************************************
Bulk EP OUT handle functions
*************************************************************/

kal_bool USB_LOG_EN = KAL_FALSE;
#define DATA_BYTE_SHOWN 	4

/* EP Bulk Out interrupt handler, called by EP interrupt */
static void USB_Acm_BulkOut_Hdr(void )
{
   extern usbacm_buf_struct usbacm_rx_buf;
   kal_uint32 nCount, i;	//04404_test

#ifdef USB_IP_U3
   if(usbacm_rx_buf.len == 0)
   {
#endif
      nCount = USB_EP_Out_Pkt_Len(g_UsbACM.rxpipe->byEP);
#ifdef USB_IP_U3
      if (nCount <= USB_BULK_SS_MAXP)
#else
      if (nCount <= USB_BULK_HS_MAXP)
#endif
      {
         usbacm_rx_buf.len += nCount;

         //USB_DBG("USB_Acm_BulkOut_Hdr, nCount:%x\n", nCount);	//04404_test
#ifndef USB_IP_U3
         USB_EPFIFORead(g_UsbACM.rxpipe->byEP, nCount, usbacm_rx_buf.data);
         USB_EP_Bulk_Out_Ready(g_UsbACM.rxpipe->byEP);
      }
#else
         if(USB_IS_QMU_Init_Done() == KAL_TRUE)
         {
            USB_QMU_Read(g_UsbACM.rxpipe->byEP, usbacm_rx_buf.data, nCount);	//04404
         }
         else
         {
            USB_EPFIFORead(g_UsbACM.rxpipe->byEP, nCount, usbacm_rx_buf.data);
            USB_EP_Bulk_Out_Ready(g_UsbACM.rxpipe->byEP);
         }
         for(i = 0; i < nCount; i++)
         {
            if(USB_LOG_EN != KAL_TRUE
               || i > DATA_BYTE_SHOWN)
               continue;
            USB_DBG("usbacm_rx_buf.data[%d]:%x, %c\n", i, usbacm_rx_buf.data[i], usbacm_rx_buf.data[i]);
            //LOGD("rx data[%d]:%c\n", i, usbacm_rx_buf.data[i]);
         }
         //LOGD("pretend consume data\n"); //04404_test
         //usbacm_rx_buf.len = 0; //04404_test
      }
#endif
#ifdef USB_IP_U3
   }
   else
   {
      //LOGD("USB_Acm_BulkOut_Hdr, usbacm_rx_buf.len != 0\n");	//04404_test
   }
#endif



}



/* EP Bulk In reset handler */
static void USB_Acm_BulkIn_Reset(void)
{
   g_UsbACM.txpipe = &g_UsbACM.data_ep_in_info->ep_status.epin_status;
}

static void USB_Acm_BulkIn_Hdr(void)
{
   gUSBAcm_IsInEPComplete = KAL_TRUE;
}

/* EP Bulk Out reset handler */
static void USB_Acm_BulkOut_Reset(void)
{
   g_UsbACM.rxpipe = &g_UsbACM.data_ep_out_info->ep_status.epout_status;
}

#if 1 // obseleted
/* Exception flag */
extern kal_uint8 INT_Exception_Enter;
#endif
