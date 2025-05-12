/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    usbacm_drv.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This file implements usb CDC ACM class driver
 *
 * Author:
 * -------
 *   Jensen Hu
 *   CJung Chen  
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:   01 Jul 2005 10:51:08  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/usb/src/usbacm_drv.c-arc  $
 *
 * Feb 25 2009 MTK02172
 * [STP100001912] [FlashTool] v3.0908.00 release
 * 
 *
 * May 16 2008 mtk01413
 * [STP100001691] [FlashTool] v3.0808.0 release
 * New features:
 *  1. USB Download for End User (MT6230)
 *
 * Dec 2 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. USB Download on MT6225/MT6226/MT6227 Series BB
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/


#include "kal_release.h"
#include "./register_act_util.h"

#include "usb_hw.h"
#include "usb_comm.h"
#include "usb_drv.h"
#include "usb.h"

#include "usb_resource.h"
#include "usbacm_drv.h"
#include "usbacm_adap.h"
#include "usb_custom.h"
#include "usb_debug_tool.h"
#include "assert.h"
#include "lib/string.h"

/* global variables */
UARTStruct USB2UARTPort;
UsbAcm_Struct g_UsbACM;

kal_bool gUSBAcm_IsInEPComplete;
/* Exception flag */
extern kal_uint8 INT_Exception_Enter; 
//extern kal_uint32 USB_BULK_MAXP;  

/* static functions */
static void USB_Acm_Ep0_SetLineCoding(void *data);
static void USB_Acm_Ep0_Command(Usb_Ep0_Status* pep0state,Usb_Command* pcmd);
static void USB_Acm_BulkOut_Hdr(void);
static void USB_Acm_BulkIn_Reset(void);
static void USB_Acm_BulkOut_Reset(void);
//static void USB_Acm_FT_BulkOut_Hdr(void );
static void USB_Acm_BulkIn_Hdr(void);

/*************************************************************
  translate ACM and UART setting functions
**************************************************************/

/* translate ACM line coding to UART DCB */
/*
void USB2UART_LineCoding2DCB(UsbAcm_Line_Coding *pline_coding, UARTDCBStruct *pDCB)
{
	 pDCB->baud = pline_coding->dwDTERate;
	 pDCB->stopBits = pline_coding->bCharFormat+1; 
	 pDCB->parity = pline_coding->bParityType;
	 pDCB->dataBits = pline_coding->bDataBits;
}
*/
/* translate UART DCB to ACM line coding */
/*
void UART2USB_DCB2LineCoding(UARTDCBStruct *pDCB, UsbAcm_Line_Coding *pline_coding)
{
	  pline_coding->dwDTERate = pDCB->baud;
	  pline_coding->bCharFormat = pDCB->stopBits-1;
	  pline_coding->bParityType = pDCB->parity;
	  pline_coding->bDataBits = pDCB->dataBits;
}
*/
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

	/* standard interface descriptor */
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
//	g_UsbACM.comm_ep_intr_info->epdesc.stdep.wMaxPacketSize[1] = (USB_EP_INTR_MAXP_FS>>8)&0xff;
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
	g_UsbACM.data_if_info->ep_info[0] = g_UsbACM.data_ep_in_info;
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
	g_UsbACM.data_if_info->ifdscr.stdif.iInterface = USB_Get_String_Number((void *)g_UsbACM.acm_param->data_interface_string);   

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
	g_UsbACM.data_ep_in_info->epdesc.stdep.bInterval = 0;
	g_UsbACM.data_ep_in_info->ep_status.epin_status.byEP = ep_in_id;
	g_UsbACM.data_ep_in_info->ep_status.epin_status.nBytesLeft = USB_EP_NODATA;
	g_UsbACM.data_ep_out_info->epdesc.stdep.bLength = USB_EPDSC_LENGTH;
	g_UsbACM.data_ep_out_info->epdesc.stdep.bDescriptorType = USB_ENDPOINT;
	g_UsbACM.data_ep_out_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_OUT | ep_out_id);   /*OutPipe*/
	g_UsbACM.data_ep_out_info->epdesc.stdep.bmAttributes = USB_EP_BULK;
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
	g_UsbACM.data_ep_out_info->epdesc.stdep.bInterval = 0;
	g_UsbACM.data_ep_out_info->ep_status.epout_status.byEP = ep_out_id;
}

/* data interface reset function, enable EP*/
void USB_Acm_DataIf_Reset(void)
{
	g_UsbACM.txpipe = &g_UsbACM.data_ep_in_info->ep_status.epin_status;
	g_UsbACM.rxpipe = &g_UsbACM.data_ep_out_info->ep_status.epout_status;

	/*DMA*/
	USB_InEPEn(g_UsbACM.data_ep_in_info->ep_status.epin_status.byEP, KAL_FALSE);
	/*Non-DMA*/
	USB_OutEPEn(g_UsbACM.data_ep_out_info->ep_status.epout_status.byEP, KAL_FALSE);
}


/* data interface speed reset function, enable EP's speed-specific descriptor */
void USB_Acm_DataIf_Speed_Reset(kal_bool b_other_speed)
{
#ifdef __USB_HS_ENABLE__
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
//			g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = (USB_EP_BULK_MAXP_FS>>8)&0xff;
			g_UsbACM.data_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = 0x00;
			g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[0] = USB_BULK_MAXP&0xff;
//			g_UsbACM.data_ep_out_info->epdesc.stdep.wMaxPacketSize[1] = (USB_EP_BULK_MAXP_FS>>8)&0xff;
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
//	g_UsbACM.break_detect = KAL_FALSE;
//	g_UsbACM.break_number = 0;
//	g_UsbACM.out_pipe_race = KAL_FALSE;
//	g_UsbACM.send_Txilm = KAL_FALSE;
//	g_UsbACM.send_Rxilm = KAL_TRUE;
//	g_UsbACM.send_UARTilm = KAL_FALSE;
//	g_UsbACM.ring_buffer_timer_counting = KAL_FALSE;
}

#if 0
/* release global variable g_UsbACM */
void USB_Release_Acm_Status(void)
{
	g_UsbACM.txpipe = NULL;
	g_UsbACM.rxpipe = NULL;
	g_UsbACM.intrpipe = NULL;
	g_UsbACM.data_interface_id = 0;
//	g_UsbACM.break_detect = KAL_FALSE;
//	g_UsbACM.break_number = 0;
//	g_UsbACM.out_pipe_race = KAL_FALSE;
//	g_UsbACM.send_Txilm = KAL_FALSE;
//	g_UsbACM.send_Rxilm = KAL_TRUE;
//	g_UsbACM.send_UARTilm = KAL_FALSE;
//	g_UsbACM.config_send_Txilm = KAL_FALSE;
}

#endif

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
   		gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
   		USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, KAL_FALSE, KAL_TRUE);
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
		gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
		USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, bError, KAL_TRUE);
	}
	else
		USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, bError, KAL_FALSE);
}


/************************************************************
	Bulk EP OUT handle functions
*************************************************************/

/* EP Bulk Out interrupt handler, called by EP interrupt */
#if 1
static void USB_Acm_BulkOut_Hdr(void )
{
	extern usbacm_buf_struct usbacm_rx_buf;
	kal_uint32 nCount;

	ASSERT(usbacm_rx_buf.len == 0);
	nCount = USB_EP_Out_Pkt_Len(g_UsbACM.rxpipe->byEP);
	if (nCount <= USB_BULK_HS_MAXP)
	{
		usbacm_rx_buf.len += nCount;
		USB_EPFIFORead(g_UsbACM.rxpipe->byEP, nCount, usbacm_rx_buf.data);
		USB_EP_Bulk_Out_Ready(g_UsbACM.rxpipe->byEP);
	}
}
#else
static void USB_Acm_BulkOut_Hdr(void )
{
	kal_int32        RoomLeft;
	kal_uint32      nCount;


	/* check data length and ring buffer empty length*/
	nCount = USB_EP_Out_Pkt_Len(g_UsbACM.rxpipe->byEP);
	Buf_GetRoomLeft(&(USB2UARTPort.Rx_Buffer),RoomLeft);

	#ifdef USBDL_DEBUG
	bulkarray[bulkindex].roomleft = RoomLeft;
	bulkarray[bulkindex++].count = nCount;
	total_count += nCount;
	#endif

	if(RoomLeft < (nCount))
		return;
	
	//if(nCount > 0)
	{
		kal_int32 remain;
		BUFFER_INFO *rx = &(USB2UARTPort.Rx_Buffer);

		remain = (BWrite(rx)+nCount) - BLength(rx);
		if(remain < 0)
		{
			USB_EPFIFORead(g_UsbACM.rxpipe->byEP, nCount, BuffWrite(rx));
			BWrite(rx) += nCount;
		}
		else
		{
			USB_EPFIFORead(g_UsbACM.rxpipe->byEP, (nCount-remain), BuffWrite(rx));
			USB_EPFIFORead(g_UsbACM.rxpipe->byEP, remain, B_StartAddr(rx));
			BWrite(rx) = remain;
		}
		/* Clear OutPktRdy */
		USB_EP_Bulk_Out_Ready(g_UsbACM.rxpipe->byEP);
		//Buf_Pushs(&(USB2UARTPort.Rx_Buffer), data, nCount);
	}

}
#endif


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
