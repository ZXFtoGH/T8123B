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
 *    usbacm_adap.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This file implements usb adaption layer for UART API
 *
 * Author:
 * -------
 *  Jensen Hu
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:   01 Jul 2005 10:50:24  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/usb/src/usbacm_adap.c-arc  $
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
//#include "intrCtrl.h"
#include "./register_act_util.h"
#include "usb_comm.h"
#include "usb_drv.h"
#include "usb.h"
#include "usbacm_drv.h"
#include "usb_custom.h"
#include "usb_hw.h"
#include "usb_drv_pri.h"
#include "usbacm_adap.h"
#include "usb_debug_tool.h"
#include "usb_resource.h"
#include "assert.h"
#include "lib/string.h"
//#include "../gpt_timer/gpt_timer.h"

#if defined(BB_MT6235) || defined(BB_MT6238)
	#define TDMA_RTC_32K				((volatile kal_uint32 *)(0x82000230))
#else	// MT6225/26/27/28/29/30
	#define TDMA_RTC_32K				((volatile kal_uint32 *)(0x80200230))
#endif

static kal_uint32 err_threshold;	// define in BLOADER.C
usbacm_buf_struct usbacm_tx_buf;
usbacm_buf_struct usbacm_rx_buf;
extern USB_Drv_Info g_UsbDrvInfo;
extern UARTStruct USB2UARTPort;
static kal_bool InheritedUSB = KAL_FALSE;

extern kal_bool gUSBAcm_IsQMUDone;
extern kal_bool gUSBAcm_IsZLPErr; 

extern void USB_Set_DP_Pull_High(kal_bool bset);
extern void USB_Config_Type(USB_DEVICE_TYPE type, kal_bool b_enable, kal_uint32 *parameter);
extern kal_uint8 USB_ReadAddress(void);
extern kal_bool USB_IS_QMU_Init_Done(void);
extern kal_uint32 USB_QMU_Write(kal_uint8 ep_num, kal_uint8* addr, kal_uint32 length);
extern void USB_QMU_Prepare_GPD(kal_uint8 ep_num, kal_uint8* pBuf, kal_uint32 length, USB_DIR dir);
extern void arch_clean_invalidate_cache_range(addr_t start, size_t len);
extern void USB_QMU_Resume(kal_uint8 ep_num, USB_DIR dir);
extern kal_uint32 USB_QMU_Update_Gpd(kal_uint8 ep_num);
extern void USB_Poll_QMU_Done(void);
extern void USB_Enable_HS(int enable_hs);
extern void mt_usb_phy_poweron(void);
extern void mt_usb_phy_savecurrent(void);
extern void mt_usb_phy_recover(void);

//setup up a new comport
void USB2COM_Setup_PORT(void)
{
	// pull down DP 
	USB_Set_DP_Pull_High(KAL_FALSE);
		
	//USB_Dbg_Trace(DEBUG_USB2COM_Init, DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_Init, 0, 0,0);
	USB_Config_Type(USB_CDC_ACM, KAL_TRUE, NULL);   	
	usbacm_rx_buf.len = 0;
	usbacm_rx_buf.index = 0;
	USB2UARTPort.initialized = KAL_TRUE;
	
	// wait USB Config
	while (gUsbDevice.nDevState != DEVSTATE_CONFIG)
		USB_HISR();	
}

/* dymanic switch endpoint if need */
void Check_And_Change_Endpoint(void)
{
#if	defined(USB_IP_V3) || defined(USB_IP_V4)
	// special case for endpoint 4	
	DRV_WriteReg8(USB_INDEX_V3, 4);
	if ((USB_OutEP_PK_SIZE(4)== USB_BULK_HS_MAXP))
	{
		MSG(USB, "Switch endpoints configuratio\n");		
		USB_Set_BulkIn_Ep(4);
		USB_Set_BulkOut_Ep(4);	
		USB_Set_Intr_Ep(1);
		InheritedUSB = KAL_TRUE;
	}	
#endif	
}

/* initialize USB2UART setting, called when driver initialize, no matter user select as UART or not */
void USB2COM_Init(void)
{			
	//USB_Dbg_Trace(DEBUG_USB2COM_Init, DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_Init, 0, 0, 0);
	/* dymanic switch endpoint if need */
	Check_And_Change_Endpoint();
	USB_Config_Type(USB_CDC_ACM, KAL_FALSE, NULL);
	gUsbDevice.ep0info.byFAddr = USB_ReadAddress();	
	gUsbDevice.nDevState = DEVSTATE_CONFIG;	   	
	usbacm_rx_buf.len = 0;
	usbacm_rx_buf.index = 0;
	USB2UARTPort.initialized = KAL_TRUE;
	
//Check DA & BootRom or Bootloader data packet 	(only check RX -> to save memory(TX = RX))
	if (USB_IS_High_Speed() == KAL_FALSE)
	{
		if ((USB_OutEP_PK_SIZE(g_UsbACM.rxpipe->byEP)!= USB_BULK_MAXP))//||(USB_InEP_PK_SIZE(g_UsbACM.txpipe->byEP) != USB_BULK_MAXP))
			ASSERT(0);
	}
	else
	{
		if ((USB_OutEP_PK_SIZE(g_UsbACM.rxpipe->byEP)!= USB_BULK_HS_MAXP))//||(USB_InEP_PK_SIZE(g_UsbACM.txpipe->byEP) != USB_BULK_HS_MAXP))
			ASSERT(0);
	}

}


/* 
    Used to retreive exception log, all interrupts are disabled 
    Note that this is special for sending exception log since interrupt is disabled when exception occurs
    It must not be used in normal time 
*/
void USB2UART_Polling_Recv_Data(void)
{
	USB_Polling_Recv_Data(g_UsbACM.rxpipe->byEP);
}

void USB2COM_PurgeFIFO(void)
{
		USB_Dbg_Trace(DEBUG_USB2COM_PurgeFIFO, 0, 0,0);
}
#define  BBCHIP_TYPE kal_uint32
	
void USB2COM_SetBaudRate(kal_uint8 baud_rate, BBCHIP_TYPE bbchip_type)
{
	  USB_Dbg_Trace(DEBUG_USB2COM_SetBaudRate, 0, 0,0);
}

kal_uint32 USB2COM_Get_USB_MAX_PACKET(void)
{
	kal_uint32  packet_length;
	
	if (USB_IS_High_Speed() == KAL_FALSE)
		packet_length = USB_BULK_MAXP;
	else
		packet_length = USB_BULK_HS_MAXP;
	
	return packet_length;
}

void USB_Debug(kal_uint8 flag)
{
//	USB_Dbg_Trace(DEBUG_GetPacket,DRV_Reg32(TDMA_RTC_32K), 0, flag);
}

/* transfer one byte data into an IN packet  */
void USB2COM_PutByte(kal_uint8 data)
{
	extern		kal_bool gUSBAcm_IsInEPComplete;
//	extern		USB_Drv_Info g_UsbDrvInfo;
//	extern 		void USB_EP_Poll_Bulk_In_Ready(kal_uint8 ep_num);
  
	kal_uint8 	ep_num;

#ifdef  USB_UT
	 MSG(USB, "USB2COM_PutByte() begin, data : %x\n", data);
#endif
#if defined(USB_QMU)
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
	{
#ifdef  USB_UT
		MSG(USB, "USB2COM_PutByte QMU, data : %x\n", data);
#endif
		/* use QMU TX transfer*/
		USB_QMU_Write(g_UsbACM.txpipe->byEP, &data, 1);
		return;
	}
#endif
	//USB_Dbg_Trace(DEBUG_USB2COM_PutByte,DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_PutByte, 0, 0, 0);
	ep_num = g_UsbACM.txpipe->byEP;
	gUSBAcm_IsInEPComplete = KAL_FALSE;	
	USB_EPFIFOWrite(ep_num, 1, &data);
	USB_EP_Bulk_In_Ready(ep_num);
	//for delay .Because IN sysPC() call this function continuous
	USB_EP_Poll_Bulk_In_Ready(ep_num);
#ifdef  USB_UT
	 MSG(USB, "USB2COM_PutByte() end\n");
#endif
	
}


void USB2COM_PutByte_Buffer(kal_uint32* addr, kal_uint32 length)
{	
	kal_uint32     packet_length/*, len = length*/;
#ifdef COMPACT_DA	
	kal_uint32     loop = 0;
#endif
//	kal_uint32     cur_length = 0;    
	kal_uint8      *ptr = (kal_uint8 *)addr;    
									
#ifdef COMPACT_DA
    for(loop=0;loop<length;loop++)
        USB2COM_PutByte(*ptr++);
    return ;    
#endif


#ifdef  USB_UT
	 MSG(USB, "USB2COM_PutByte_Buffer() begin, length : %d\n ", length);
#endif
	//USB_Dbg_Trace(DEBUG_USB2COM_PutByte_buffer, DRV_Reg32(TDMA_RTC_32K), usbacm_tx_buf.len,0);
 	USB_Dbg_Trace(DEBUG_USB2COM_PutByte_buffer, 0, usbacm_tx_buf.len,0);
	 
#if defined(USB_QMU)
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
	{
		/* use QMU TX transfer*/		
		kal_uint32 desire_len;
		
		while (length > 0){
			
			if(length > QMU_MAX_SZ_PER_GPD)
			{
				desire_len = QMU_MAX_SZ_PER_GPD;
			}
			else
			{
				desire_len = length;
			}
			
			USB_QMU_Write(g_UsbACM.txpipe->byEP, ptr, desire_len);
			length -= desire_len;
			ptr += desire_len;
		}

		return;
	}
#endif
	 
	if (USB_IS_High_Speed() == KAL_FALSE)
	{
		packet_length = USB_BULK_MAXP;
	}
	else
	{
		packet_length = USB_BULK_HS_MAXP;
	}

     	if (length > packet_length) 
    	{
        	kal_uint32 len = length;
    		kal_uint32 cur_length = 0;    
    		kal_uint8  *ptr = (kal_uint8 *)addr;            
        	while (len > 0) {
           		 cur_length = (len >= packet_length) ? (packet_length) : (len);
    			USB_EPFIFOWrite(g_UsbACM.txpipe->byEP, cur_length, ptr);
    			USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
            		USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);   // check if TX is completed
            		len -= cur_length;
            		ptr += cur_length;        
        	}
        	if (cur_length == packet_length) {  // sending ZLP
    			USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
            		USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);   // check if TX is completed        
            		MSG(USB, "ZLP sent(%d,%d)\n", length, packet_length);
        	}                
    		usbacm_tx_buf.len = 0;	    
    	}
	else 
	{
       	if (length == packet_length)
    		{
    			USB_EPFIFOWrite(g_UsbACM.txpipe->byEP, packet_length, addr);
    			USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
    		}
    		else if (length < packet_length)
    		{
    			USB_EPFIFOWrite(g_UsbACM.txpipe->byEP, length, addr);
    			USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
    		}
    		else  //if Length > Max Packet sieze
    			ASSERT(0);

    		USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
		
		if (length == packet_length) {  // sending ZLP
    			USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
            		USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);   // check if TX is completed        
            		MSG(USB, "ZLP sent(%d,%d)\n", length, packet_length);
        	}  
    		usbacm_tx_buf.len = 0;	
	}

#ifdef  USB_UT
	 MSG(USB, "USB2COM_PutByte_Buffer() end\n ");
#endif
	//USB_Dbg_Trace(DEBUG_USB2COM_PutByte_buffer, DRV_Reg32(TDMA_RTC_32K), usbacm_tx_buf.len,1);
	USB_Dbg_Trace(DEBUG_USB2COM_PutByte_buffer, 0, usbacm_tx_buf.len,1);
}
/*
	It is called by bootloader to flush the tx buffer into USB FIFO
*/
void USB2COM_PutByte_Complete(kal_uint8 data)
{
	extern		kal_bool gUSBAcm_IsInEPComplete;
	//extern		USB_Drv_Info g_UsbDrvInfo;
	kal_uint8 	ep_num;
	
 	//USB_Dbg_Trace(DEBUG_USB2COM_PutByte_Complete, DRV_Reg32(TDMA_RTC_32K), usbacm_tx_buf.len,0);
 	USB_Dbg_Trace(DEBUG_USB2COM_PutByte_Complete, 0, usbacm_tx_buf.len,0);
 	 	 
#if defined(USB_QMU)
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
	{
		//MSG(USB, "USB2COM_PutByte_Complete QMU, data : %x\n", data);
		/* use QMU TX transfer*/
		USB_QMU_Write(g_UsbACM.txpipe->byEP, &data, 1);
		return;
	}
#endif
 	 	 
	if(usbacm_tx_buf.len)
	{
		ep_num = g_UsbACM.txpipe->byEP;
		gUSBAcm_IsInEPComplete = KAL_FALSE;		
		USB_EPFIFOWrite(ep_num, (kal_uint16)usbacm_tx_buf.len, usbacm_tx_buf.data);
		USB_EP_Bulk_In_Ready(ep_num);
		while(!gUSBAcm_IsInEPComplete )
		{
			USB_HISR();
		}
		usbacm_tx_buf.len = 0;
	}
	
	USB2COM_PutByte(data);
	
/*	
#if	defined(USB_IP_V3)
	else
	{	
	ep_num = g_UsbACM.txpipe->byEP;
	USB_EPFIFOWrite(ep_num, 0, NULL);
	USB_EP_Bulk_In_Ready(ep_num);
	}
#endif
*/
}

kal_uint8 USB2COM_GetByteQMU(void)
{
	kal_uint8   data = 0;
#if defined(USB_QMU)
	kal_uint32 recv_len;

#ifdef  USB_UT
	MSG(USB, "USB2COM_GetByteQMU usbacm_rx_buf.len:%d, usbacm_rx_buf.index:%d\n", usbacm_rx_buf.len, usbacm_rx_buf.index);
#endif
	
	/* QMU mode */
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
	{
		if( 0 == usbacm_rx_buf.len )
		{
			{
				kal_uint32 ep_num;
				ep_num = g_UsbACM.rxpipe->byEP;
				
				/* use QMU RX transfer*/
				gUSBAcm_IsQMUDone = KAL_FALSE;
				gUSBAcm_IsZLPErr = KAL_FALSE;

				if (USB_IS_High_Speed() == KAL_FALSE){
					USB_QMU_Prepare_GPD(ep_num, usbacm_rx_buf.data, USB_BULK_MAXP, USB_RX);
					arch_clean_invalidate_cache_range((addr_t) &usbacm_rx_buf.data, USB_BULK_MAXP);
				}else{
					USB_QMU_Prepare_GPD(ep_num, usbacm_rx_buf.data, USB_BULK_HS_MAXP, USB_RX);
					arch_clean_invalidate_cache_range((addr_t) &usbacm_rx_buf.data, USB_BULK_HS_MAXP);
				}
				
				USB_QMU_Resume(ep_num, USB_RX);
				while(gUSBAcm_IsQMUDone == KAL_FALSE)
				{
					USB_HISR();
					if(gUSBAcm_IsZLPErr == KAL_TRUE)
					{
						gUSBAcm_IsZLPErr = KAL_FALSE;
						USB_QMU_Resume(ep_num, USB_RX);
					}
						
				}

				recv_len = USB_QMU_Update_Gpd(ep_num);
//				MSG(USB, "recv len from QMU : %d\n", recv_len);
				usbacm_rx_buf.len += recv_len;
				
			}
		}
		data = usbacm_rx_buf.data[usbacm_rx_buf.index++];

		if(usbacm_rx_buf.index == usbacm_rx_buf.len)
		{
			usbacm_rx_buf.len = 0;
			usbacm_rx_buf.index = 0;
		}

		return data;
	}
#endif
	return data;
}

kal_uint8 USB2COM_GetByte(void)
{
	kal_uint8   data = 0;
#ifdef  USB_UT
	 MSG(USB, "USB2COM_GetByte() begin\n");
#endif
	
 	//USB_Dbg_Trace(DEBUG_USB2COM_GetByte, DRV_Reg32(TDMA_RTC_32K),usbacm_rx_buf.len, 0);
 	USB_Dbg_Trace(DEBUG_USB2COM_GetByte, 0, usbacm_rx_buf.len, 0);

#if defined(USB_QMU)
#ifdef  USB_UT
	MSG(USB, "USB2COM_GetByte() begin\n");
#endif
	if(USB_IS_QMU_Init_Done() == KAL_TRUE){
 		data = USB2COM_GetByteQMU();
		return data;
	}
#endif

	while (usbacm_rx_buf.len == 0)
	{
		USB_HISR();
	}

	data = usbacm_rx_buf.data[usbacm_rx_buf.index++];

	if(usbacm_rx_buf.index == usbacm_rx_buf.len)
	{
		usbacm_rx_buf.len = 0;
		usbacm_rx_buf.index = 0;
	}
#ifdef  USB_UT
	MSG(USB, "USB2COM_GetByte() end, data : %x\n", data);
#endif
	return data;
}


kal_uint16 USB2COM_GetData16(void)			//ok, high byte is first
{
	kal_uint8	tmp,index;
	kal_uint16 	tmp16;
	kal_uint16  result =0;
	//USB_Dbg_Trace(DEBUG_USB2COM_GetData16, DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_GetData16, 0, 0,0);
	for (index =0;index < 2;index++)
	{
		tmp = USB2COM_GetByte();
		tmp16 = (kal_uint16)tmp;
		result |= (tmp16 << (8-8*index));
	}
	return result;
}

void USB2COM_PutData16(kal_uint16 data)		//ok, high byte is first
{
	kal_uint8	tmp,index;
	kal_uint16 	tmp16;
	kal_uint8   put_data[2];

#if	defined(USB_IP_V3) || defined(USB_IP_V4)
	extern		kal_bool gUSBAcm_IsInEPComplete;
	//extern		USB_Drv_Info g_UsbDrvInfo;
	kal_uint8 	ep_num;
#endif	

 	//USB_Dbg_Trace(DEBUG_USB2COM_PutData16, DRV_Reg32(TDMA_RTC_32K), 0,0);
 	USB_Dbg_Trace(DEBUG_USB2COM_PutData16, 0, 0,0);
	
	for (index =0;index < 2;index++)
	{
		tmp16 = (data >> (8-8*index));
		tmp = (kal_uint8)tmp16;
		put_data[index] = tmp;
#if !defined(USB_IP_V3) && !defined(USB_IP_V4)
		USB2COM_PutByte(tmp);
#endif
	}
	
#if	defined(USB_IP_V3) || defined(USB_IP_V4)

#if defined(USB_QMU)
	/* QMU mode */
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
	{
		/* use QMU TX transfer*/
		USB_QMU_Write(g_UsbACM.txpipe->byEP, put_data, 2);
		return;
	}
#endif

	ep_num = g_UsbACM.txpipe->byEP;
	gUSBAcm_IsInEPComplete = KAL_FALSE;	
	USB_EPFIFOWrite(ep_num, 2, &put_data);
	USB_EP_Bulk_In_Ready(ep_num);
	//for delay .Because IN sysPC() call this function continuous
	USB_EP_Poll_Bulk_In_Ready(ep_num);
#endif
}

kal_uint32 USB2COM_GetData32(void)			//ok, high byte is first
{
	kal_uint8	tmp,index;
	kal_uint32 	tmp32;
	kal_uint32  result =0;
	
	//USB_Dbg_Trace(DEBUG_USB2COM_GetData32, DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_GetData32, 0, 0,0);
	for (index =0;index < 4;index++)
	{
		tmp = USB2COM_GetByte();
		tmp32 = (kal_uint32)tmp;
		result |= (tmp32 << (24-8*index));
	}
	return result;
}

void USB2COM_PutData32(kal_uint32 data)		//ok, high byte is first
{
	kal_uint8	tmp,index;
	kal_uint32 	tmp32;
	kal_uint8   put_data[4];
#if defined(USB_IP_V3) || defined(USB_IP_V4)
	extern		kal_bool gUSBAcm_IsInEPComplete;
	//extern		USB_Drv_Info g_UsbDrvInfo;
//	extern 		void USB_EP_Poll_Bulk_In_Ready(kal_uint8 ep_num);
	kal_uint8 	ep_num;  
#endif
	
    //USB_Dbg_Trace(DEBUG_USB2COM_PutData32, DRV_Reg32(TDMA_RTC_32K), 0,0);
    USB_Dbg_Trace(DEBUG_USB2COM_PutData32, 0, 0,0);
	for (index =0;index < 4;index++)
	{
		tmp32 = (data >> (24-8*index));
		tmp = (kal_uint8)tmp32;
		put_data[index] = tmp;
#if !defined(USB_IP_V3) && !defined(USB_IP_V4)
		USB2COM_PutByte(tmp);
#endif
	}
#if defined(USB_IP_V3) || defined(USB_IP_V4)

#if defined(USB_QMU)
	/* QMU mode */
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
	{
		/* use QMU TX transfer*/
		USB_QMU_Write(g_UsbACM.txpipe->byEP, put_data, 4);
		return;
	}
#endif
	
	ep_num = g_UsbACM.txpipe->byEP;
	gUSBAcm_IsInEPComplete = KAL_FALSE;	
	USB_EPFIFOWrite(ep_num, 4, &put_data);
	USB_EP_Bulk_In_Ready(ep_num);
	//for delay .Because IN sysPC() call this function continuous
	USB_EP_Poll_Bulk_In_Ready(ep_num);
#endif
}

kal_uint64 USB2COM_GetData64(void)			//ok, high byte is first
{
	kal_uint32	tmp1,tmp2;
	kal_uint64  result =0;
	
	tmp1 = USB2COM_GetData32();
	tmp2 = USB2COM_GetData32();

	result = ((kal_uint64)tmp1<<32)|tmp2;
	return result;
}

void USB2COM_PutData64(kal_uint64 data)		//ok, high byte is first
{
	kal_uint32	tmp1,tmp2;

	tmp1 = (kal_uint32)(data>>32);
	tmp2 = (kal_uint32)(data);
	USB2COM_PutData32(tmp1);
	USB2COM_PutData32(tmp2);
}

/* Try to get a byte until err_threshold is reached */
kal_bool USB2COM_GetByteStatus(kal_uint8 *data)
{
	kal_uint32 loop = 0;
//	kal_uint8 del = 0;
	
// 	USB_Dbg_Trace(DEBUG_USB2COM_GetByteStatus, DRV_Reg32(TDMA_RTC_32K), 0,0);

#if defined(USB_QMU)
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
 		return USB2COM_GetByteQMU();
#endif

	if(usbacm_rx_buf.index == usbacm_rx_buf.len)
	{
		usbacm_rx_buf.len = 0;
		usbacm_rx_buf.index = 0;
	}	

	while(usbacm_rx_buf.len == 0 && loop++ < err_threshold)
	{
		USB_HISR();
	}
	
	if(loop >= err_threshold)
		return KAL_FALSE;		
	*data = usbacm_rx_buf.data[usbacm_rx_buf.index++];
	return KAL_TRUE;

}

kal_bool USB2COM_GetByte_Buffer(kal_uint32* addr, kal_uint32 length)
{	
//	kal_uint32     packet_length;
#ifdef COMPACT_DA
	kal_uint32     loop = 0;
#endif
	kal_uint32     cur_length = 0;
	kal_uint8      *ptr = (kal_uint8 *)addr;    

#ifdef COMPACT_DA
    for(loop=0;loop<length;loop++)
        *ptr++ = USB2COM_GetByte();
	return KAL_TRUE;	
#endif


#if	defined(USB_IP_V2)	
	kal_bool       result;
#endif

#ifdef  USB_UT
	MSG(USB, "USB2COM_GetByte_Buffer() begin, length : %d\n", length);
#endif

	//USB_Dbg_Trace(DEBUG_USB2COM_GetByte_Buffer, DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_GetByte_Buffer, 0, 0, 0);
    
#if defined(USB_QMU)
	/* QMU mode */
	if(USB_IS_QMU_Init_Done() == KAL_TRUE)
	{
		while (length > 0){
		
			kal_uint32 ep_num, recv_len, desire_len;
			ep_num = g_UsbACM.rxpipe->byEP;
			
			/* use QMU RX transfer*/
			gUSBAcm_IsQMUDone = KAL_FALSE;
			gUSBAcm_IsZLPErr = KAL_FALSE;
			
			if(length > QMU_MAX_SZ_PER_GPD)
			{
				desire_len = QMU_MAX_SZ_PER_GPD;
			}
			else
			{
				desire_len = length;
			}

//			MSG(USB, "desire_len : %d\n", desire_len);
			USB_QMU_Prepare_GPD(ep_num, ptr, desire_len, USB_RX);
			arch_clean_invalidate_cache_range((addr_t) ptr, desire_len);
			USB_QMU_Resume(ep_num, USB_RX);
			while(gUSBAcm_IsQMUDone == KAL_FALSE)
			{
				USB_Poll_QMU_Done();
				if(gUSBAcm_IsZLPErr == KAL_TRUE)
				{
					gUSBAcm_IsZLPErr = KAL_FALSE;
					USB_QMU_Resume(ep_num, USB_RX);
				}
					
			}

			recv_len = USB_QMU_Update_Gpd(ep_num);
//			MSG(USB, "USB2COM_GetByte_Buffer, recv len from QMU : %d length %d\n", recv_len, length);
			
			length -= recv_len;
			ptr += recv_len;
			
		}
		return KAL_TRUE;
	}
#endif

	while (length > 0){
#ifdef  USB_UT
	   	MSG(USB, "USB2COM_GetByte_Buffer(), before USB_EP_Poll_Bulk_Out_Intr()");
#endif
        	while((USB_EP_Poll_Bulk_Out_Intr(g_UsbACM.rxpipe->byEP)==KAL_FALSE));
		
#ifdef  USB_UT
	  	MSG(USB, "USB2COM_GetByte_Buffer(), after USB_EP_Poll_Bulk_Out_Intr()");
#endif
	  
        	cur_length = USB_EP_Out_Pkt_Len(g_UsbACM.rxpipe->byEP);
       	 if(cur_length)
    			USB_EPFIFORead(g_UsbACM.rxpipe->byEP,cur_length ,ptr);


	 	USB_EP_Bulk_Out_Ready(g_UsbACM.rxpipe->byEP);
        	length -= cur_length;
        	ptr += cur_length;
#ifdef  USB_UT
	 	MSG(USB, "USB2COM_GetByte_Buffer(), cur_length :%x, length:%x, ptr:%x\n ", cur_length, length, ptr);
#endif
	}

	usbacm_rx_buf.len = 0;
	usbacm_rx_buf.index = 0;	
#ifdef  USB_UT
	MSG(USB, "USB2COM_GetByte_Buffer() end\n");
#endif
	return KAL_TRUE;
}

void USB2COM_GetDatas(kal_uint32 length)
{	
	//USB_Dbg_Trace(DEBUG_USB2COM_GetDatas, DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_GetDatas, 0, 0, 0);
	
	USB_DMA_Setup(g_UsbACM.rxpipe->byEP,USB_OUT_EP_TYPE, (kal_uint32)usbacm_rx_buf.data,length);
	USBDMA_HISR();

}
void USB2COM_PutDatas(kal_uint32 length)
{
	//USB_Dbg_Trace(DEBUG_USB2COM_PutDatas, DRV_Reg32(TDMA_RTC_32K), 0,0);
	USB_Dbg_Trace(DEBUG_USB2COM_PutDatas, 0, 0, 0);

	USB_DMA_Setup(g_UsbACM.txpipe->byEP,USB_IN_EP_TYPE, (kal_uint32)usbacm_tx_buf.data,length);
	USBDMA_HISR();

}


void USB2COM_SetupDL(int usb_dl_mode, int enable_hs)
{
    #define WPLL_CON0       ((volatile unsigned short *)0xC00071C0)
    #define UPLL_CON0       ((volatile unsigned short *)0xC0007160)

    #define PMIC_BANK0_SLAVE_ADDR 0xC0
    USB_Enable_HS(enable_hs);
  
	if( !usb_dl_mode) { /* forcely switch to USB download mode */
#if !defined(FPGA_FEATURE_ONLY)
        mt_usb_phy_poweron();
#endif
        USB2COM_Setup_PORT();
	} else {
		if (InheritedUSB == KAL_FALSE)	{
			
#if !defined(FPGA_FEATURE_ONLY)
        		kal_uint8 usbpower = DRV_Reg8(USB_POWER_V3);
        		usbpower &= ~USB_POWER_SOFTCONN;
        		DRV_WriteReg8(USB_POWER_V3, usbpower);
        		gpt_busy_wait_ms(200);
        		mt_usb_phy_savecurrent();
#endif

        		USB_DeRegister_CreateFunc();
        		USB_DeRegister_Device_Code();
				
#if !defined(FPGA_FEATURE_ONLY)
        		mt_usb_phy_recover();
#endif
        		USB2COM_Setup_PORT();
        	} else {
        		MSG(USB, "Inherited USB from prelaoder, don't do enumeration\n");
        	}
	}
}
//-----------------------------------------------------------------------------
void USB2COM_SetErrThreshold(kal_uint32 err_thres)
{
    err_threshold = err_thres;
}
//-----------------------------------------------------------------------------
int COM_Is_USB_Download_Mode(void)
{
#ifndef FPGA_FEATURE_ONLY
    /*#define WPLL_CON0   ((volatile kal_uint16 *)0xC00071C0)
    #define USB_61M     0x4001

    int i = 0;
    // in MT6575E1, if PLL is off, device hangs while accessing USB MAC
    if (USB_61M != (*WPLL_CON0 & USB_61M))    
        return FALSE;*/
#endif
    
    return (DRV_Reg8(USB_POWER_V3) & USB_POWER_SOFTCONN) ? TRUE : FALSE;
}
