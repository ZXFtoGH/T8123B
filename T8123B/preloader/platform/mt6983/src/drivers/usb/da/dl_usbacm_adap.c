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

#include "debug.h"
#include "assert.h"
#include <string.h>

static uint32 err_threshold;	// define in BLOADER.C
usbacm_buf_struct usbacm_tx_buf;
usbacm_buf_struct usbacm_rx_buf;
extern USB_Drv_Info g_UsbDrvInfo;
extern UARTStruct USB2UARTPort;
static kal_bool InheritedUSB = KAL_FALSE;


kal_bool gUSBAcm_IsQMUDone;
kal_bool gUSBAcm_IsZLPErr;
kal_bool gUSBAcm_IsAcmOpen;

//setup up a new comport
extern kal_bool USB_LOG_EN; //04404_test
void USB2COM_Setup_PORT(void)
{
   // pull down DP
   USB_Set_DP_Pull_High(KAL_FALSE);

   USB_Dbg_Trace(DEBUG_USB2COM_Init, 0, 0,0);
   USB_Config_Type(USB_CDC_ACM, KAL_TRUE, NULL);
   usbacm_rx_buf.len = 0;
   usbacm_rx_buf.index = 0;
   USB2UARTPort.initialized = KAL_TRUE;

   // wait USB Config
   while (gUsbDevice.nDevState != DEVSTATE_CONFIG)
   {
      USB_HISR();
   }

   //USB_LOG_EN = KAL_TRUE;

#if 0
   LOGD("USB2COM_Setup_PORT delay 5 sec begin\n");		//04404_test
   mdelay(5000);
   LOGD("USB2COM_Setup_PORT delay 5 sec end\n");
#endif
#ifdef USB_UT
   USB_TEST();
#endif

}

/* dymanic switch endpoint if need */
void Check_And_Change_Endpoint(void)
{
   // special case for endpoint 4
   if ((USB_OutEP_PK_SIZE(4)== USB_BULK_HS_MAXP))
   {
      LOGD("Switch endpoints configuratio\n");
      USB_Set_BulkIn_Ep(4);
      USB_Set_BulkOut_Ep(4);
      USB_Set_Intr_Ep(1);
      InheritedUSB = KAL_TRUE;
   }

}

//04404
void USB_QMU_Init_and_Start(void);
/* initialize USB2UART setting, called when driver initialize, no matter user select as UART or not */
void USB2COM_Init(void)
{
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

   //04404
   if (USB_IS_Super_Speed() == KAL_TRUE)
   {
      if ((USB_OutEP_PK_SIZE(g_UsbACM.rxpipe->byEP)!= USB_BULK_SS_MAXP))
         ASSERT(0);
   }
   else if (USB_IS_High_Speed() == KAL_TRUE)
   {
      if ((USB_OutEP_PK_SIZE(g_UsbACM.rxpipe->byEP)!= USB_BULK_HS_MAXP))
         ASSERT(0);
   }
   else
   {
      if ((USB_OutEP_PK_SIZE(g_UsbACM.rxpipe->byEP)!= USB_BULK_MAXP))
         ASSERT(0);
   }
}


/*
Used to retreive exception log, all interrupts are disabled
Note that this is special for sending exception log since interrupt is disabled when exception occurs
It must not be used in normal time
*/


void USB_Debug(kal_uint8 flag)
{
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

   if (USB_IS_Super_Speed() == KAL_TRUE)
      packet_length = USB_BULK_SS_MAXP;
   else if (USB_IS_High_Speed() == KAL_TRUE)
      packet_length = USB_BULK_HS_MAXP;
   else
      packet_length = USB_BULK_MAXP;

   return packet_length;
}

extern kal_bool gUSBAcm_IsInEPComplete; //04404_test
extern void print_data(kal_uint8* data, kal_int32 len);


#if 1

void USBDL_PutUARTByte(kal_uint8 data)
{
   kal_uint8 	ep_num;
   kal_uint32      Data_Packet_Length;

   Data_Packet_Length = USB2COM_Get_USB_MAX_PACKET();

   if(gUsbDevice.nDevState == DEVSTATE_CONFIG)
   {
      usbacm_tx_buf.data[usbacm_tx_buf.len] = data;
      usbacm_tx_buf.len++;

      if(usbacm_tx_buf.len < Data_Packet_Length )
      {
         /* gather the data until one USB MAX packet size */
         return;
      }
      else
      {

         ep_num = g_UsbACM.txpipe->byEP;
         gUSBAcm_IsInEPComplete = KAL_FALSE;
         //LOGD("USBDL_PutUARTByte send\n"); //04404_test
         //print_data((kal_uint8*)usbacm_tx_buf.data, Data_Packet_Length);		//04404_test
         USB_EPFIFOWrite(ep_num, Data_Packet_Length , usbacm_tx_buf.data);
         USB_EP_Bulk_In_Ready(ep_num);
         //LOGD("ep_num : %d\n", ep_num);

         while(gUSBAcm_IsInEPComplete == KAL_FALSE)
         {
            USB_HISR();
         }
         usbacm_tx_buf.len = 0;
      }
   }
}

void USBDL_PutUARTByte_Complete(void)
{
   kal_uint8 	ep_num;
   ep_num = g_UsbACM.txpipe->byEP;

   if ((gUsbDevice.nDevState == DEVSTATE_CONFIG)&&(usbacm_tx_buf.len != 0))
   {
      gUSBAcm_IsInEPComplete = KAL_FALSE;
      USB_EPFIFOWrite(ep_num, (kal_uint16)usbacm_tx_buf.len, usbacm_tx_buf.data);
      USB_EP_Bulk_In_Ready(ep_num);

      while(gUSBAcm_IsInEPComplete == KAL_FALSE)
      {
         USB_HISR();
      }
   }
   else
   {//send ZLP
      USB_EP_Bulk_In_Ready(ep_num);
      USB_EP_Poll_Bulk_In_Ready(ep_num);
   }

   usbacm_tx_buf.len = 0;

}

void USBDL_adpt_PutData(kal_uint8* buf, kal_uint32 len)
{
   kal_uint32 i;
   for(i = 0; i < len; i++)
   {
      USBDL_PutUARTByte(*(buf+i));
   }

}

static kal_bool USBDL_GetByteFromRxBuf(kal_uint8* data)
{
   static kal_uint32 index = 0;			// note this static
   kal_bool    bResult=KAL_FALSE;

   // get data from buf
   if( 0 < usbacm_rx_buf.len )
   {
     // LOGI("%s: rx index:%d\n", __func__, index);
      *data = usbacm_rx_buf.data[index++];
      bResult = KAL_TRUE;
   //   LOGI("%s: copy done\n", __func__);
      if(index == usbacm_rx_buf.len)
      {
         /* All data have been read */
         usbacm_rx_buf.len = 0;
         index = 0;
      }
   }

   return bResult;
}


kal_uint8 USBDL_GetUARTByte(void)
{
   kal_uint8   data = 0;
   kal_uint32 recv_len;

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

            USB_QMU_Prepare_GPD(ep_num, (kal_uint8*)&usbacm_rx_buf.data, USB_BULK_SS_MAXP, USB_RX);
            USB_QMU_Resume(ep_num, USB_RX);
            while(gUSBAcm_IsQMUDone == KAL_FALSE)
            {
               USB_HISR();
               if(gUSBAcm_IsZLPErr == KAL_TRUE)
                  USB_QMU_Resume(ep_num, USB_RX);

            }

            recv_len = USB_QMU_Update_Gpd(ep_num);
            //LOGD("recv len from QMU : %d\n", recv_len);		//04404_test
            //print_data(usbacm_rx_buf.data, recv_len);
            usbacm_rx_buf.len += recv_len;

         }
      }
      //USBDL_GetByteFromRxBuf(&data);
      return data;
   }


   // if RX buf is empty, polling until receive data
   while( 0 == usbacm_rx_buf.len )
   {
      USB_HISR();
   }

   // get data from buf
   USBDL_GetByteFromRxBuf(&data);

   return data;
}

/* transfer one byte data into an IN packet  */
void USB2COM_PutByte(kal_uint8 data)
{
   USB_DBG("%s begin\n", __func__);
   /* QMU mode */
   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
   {
      //LOGD("USB2COM_PutByte QMU, data : %x\n", data); //04404_test
      /* use QMU TX transfer*/
      USB_QMU_Write(g_UsbACM.txpipe->byEP, &data, 1);
      //LOGD("USB2COM_PutByte end\n"); //04404_test
      return;
   }

   /* BROM PIO mode */
   USBDL_adpt_PutData((kal_uint8*)&data, 1);
   USBDL_PutUARTByte_Complete();
   return;

}


void USB2COM_PutByte_Buffer(kal_uint32* addr, kal_uint32 length)
{
   kal_uint32     len = length;
   kal_uint8      *ptr = (kal_uint8 *)addr;


   USB_DBG("%s begin\n", __func__);
   /*QMU mode */
   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
   {
      arch_clean_invalidate_cache_range((addr_t)ptr, len);
      /* use QMU TX transfer*/

      kal_uint32 desire_len;

      while (len > 0){

         if(len > QMU_MAX_SZ_PER_GPD)
         {
            desire_len = QMU_MAX_SZ_PER_GPD;
         }
         else
         {
            desire_len = len;
         }

         USB_QMU_Write(g_UsbACM.txpipe->byEP, ptr, desire_len);
         len -= desire_len;
         ptr += desire_len;
      }

      return;
   }

   /*BROM PIO MODE*/
   USBDL_adpt_PutData((kal_uint8*)addr, len);
   // flush data
   USBDL_PutUARTByte_Complete();

   return;
}
/*
It is called by bootloader to flush the tx buffer into USB FIFO
*/
void USB2COM_PutByte_Complete(kal_uint8 data)
{
   USB_DBG("%s begin\n", __func__);
   USB2COM_PutByte(data);
}

kal_uint8 USB2COM_GetByte(void)
{

   USB_DBG("%s begin, usbacm_rx_buf.len : %d\n", __func__, usbacm_rx_buf.len);
   return USBDL_GetUARTByte();
}




kal_uint16 USB2COM_GetData16(void)			//ok, high byte is first
{
   uint8	tmp,index;
   uint16 	tmp16;
   uint16  result =0;

   USB_DBG("%s begin, usbacm_rx_buf.len : %d\n", __func__, usbacm_rx_buf.len);


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

   USB_DBG("%s begin\n", __func__);

   for (index =0;index < 2;index++)
   {
      tmp16 = (data >> (8-8*index));
      tmp = (kal_uint8)tmp16;
      put_data[index] = tmp;

   }

   /* QMU mode */
   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
   {
      /* use QMU TX transfer*/
      USB_QMU_Write(g_UsbACM.txpipe->byEP, (kal_uint8*)&put_data, 2);
      return;
   }

   // BROM PIO MODE
   for (index =0;index < 2;index++)
   {
      tmp16 = (data >> (8-8*index));
      tmp = (uint8)tmp16;
      USBDL_adpt_PutData((kal_uint8 *)&tmp, 1);
   }

   USBDL_PutUARTByte_Complete();
   return;

}

kal_uint32 USB2COM_GetData32(void)			//ok, high byte is first
{
   kal_uint8	tmp,index;
   kal_uint32 tmp32;
   kal_uint32  result =0;

   USB_DBG("%s begin, usbacm_rx_buf.len : %d\n", __func__, usbacm_rx_buf.len);

   for (index =0;index < 4;index++)
   {
      tmp = USB2COM_GetByte();	//04404_test
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

   USB_DBG("%s begin\n", __func__);
   for (index =0;index < 4;index++)
   {
      tmp32 = (data >> (24-8*index));
      tmp = (kal_uint8)tmp32;
      put_data[index] = tmp;
   }

   /* QMU mode */
   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
   {
      /* use QMU TX transfer*/
      USB_QMU_Write(g_UsbACM.txpipe->byEP, (kal_uint8*)&put_data, 4);
      return;
   }

   //BROM PIO MODE
   for (index =0;index < 4;index++)
   {
      tmp32 = (data >> (24-8*index));
      tmp = (uint8)tmp32;
      USBDL_adpt_PutData(&tmp, 1);
   }

   USBDL_PutUARTByte_Complete();
   return;


}

kal_uint64 USB2COM_GetData64(void)			//ok, high byte is first
{
   kal_uint32	tmp1,tmp2;
   kal_uint64  result =0;

   USB_DBG("%s begin, usbacm_rx_buf.len : %d\n", __func__, usbacm_rx_buf.len);
   tmp1 = USB2COM_GetData32();
   tmp2 = USB2COM_GetData32();

   result = ((kal_uint64)tmp1<<32)|tmp2;

   return result;
}

void USB2COM_PutData64(kal_uint64 data)		//ok, high byte is first
{
   USB_DBG("%s begin\n", __func__);

   kal_uint8	tmp,index;
   kal_uint64 tmp64;
   kal_uint8   put_data[8];

   for (index =0;index < 8;index++)
   {
      tmp64 = (data >> (56-8*index));
      tmp = (kal_uint8)tmp64;
      put_data[index] = tmp;
   }
   /* QMU mode */
   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
   {
      /* use QMU TX transfer*/
      USB_QMU_Write(g_UsbACM.txpipe->byEP, (kal_uint8*)&put_data, 8);
      return;
   }

   //BROM PIO MODE
   for (index =0;index < 8;index++)
   {
      tmp64 = (data >> (56-8*index));
      tmp = (uint8)tmp64;
      USBDL_adpt_PutData(&tmp, 1);
   }

   USBDL_PutUARTByte_Complete();

}

/* Try to get a byte until err_threshold is reached */
kal_bool USB2COM_GetByteStatus(kal_uint8 *data)
{
   USB_DBG("%s begin, usbacm_rx_buf.len : %d\n", __func__, usbacm_rx_buf.len);
   *data = USBDL_GetUARTByte();
   return KAL_TRUE;
}



kal_bool USB2COM_GetByte_Buffer(kal_uint32* addr, kal_uint32 length)
{
   kal_uint8      *ptr = (kal_uint8 *)addr;
   kal_uint32	org_length = length;
   kal_uint32 i;

   USB_DBG("%s begin, addr : %x, length : %d, usbacm_rx_buf.len : %d\n", __func__, addr, length, usbacm_rx_buf.len);

   /* QMU mode */
   if(USB_IS_QMU_Init_Done() == KAL_TRUE)
   {
      arch_clean_invalidate_cache_range((addr_t)ptr, org_length);
      while (org_length > 0){

         kal_uint32 ep_num, recv_len, desire_len;
         ep_num = g_UsbACM.rxpipe->byEP;

         /* use QMU RX transfer*/
         gUSBAcm_IsQMUDone = KAL_FALSE;
         gUSBAcm_IsZLPErr = KAL_FALSE;

         if(org_length > QMU_MAX_SZ_PER_GPD)
         {
            desire_len = QMU_MAX_SZ_PER_GPD;
         }
         else
         {
            desire_len = org_length;
         }

         //LOGD("desire_len : %d\n", desire_len);
         USB_QMU_Prepare_GPD(ep_num, ptr, desire_len, USB_RX);
         USB_QMU_Resume(ep_num, USB_RX);
         while(gUSBAcm_IsQMUDone == KAL_FALSE)
         {
            USB_HISR();
            if(gUSBAcm_IsZLPErr == KAL_TRUE)
            {
               USB_QMU_Resume(ep_num, USB_RX);
            }
         }

         recv_len = USB_QMU_Update_Gpd(ep_num);
         //LOGD("USB2COM_GetByte_Buffer, recv len from QMU : %d\n", recv_len);		//04404_test

         org_length -= recv_len;
         ptr += recv_len;
      }
      return KAL_TRUE;
   }

   //BROM Style
   for(i = 0; i < org_length; i++)
   {
     if (org_length >= 512) {
         i = 0;
         while(1)
         {
            if (usbacm_rx_buf.len == 0)
               USB_HISR();
		//USBDL_GetUARTByte();

            if (usbacm_rx_buf.len) {
               memcpy(ptr + i,  usbacm_rx_buf.data, sizeof(kal_uint8)*usbacm_rx_buf.len);
               i = i + usbacm_rx_buf.len;
               usbacm_rx_buf.len = 0;
            }

            if (org_length == i) {
               usbacm_rx_buf.len = 0;
               break;
            }
         }
      }
      else {
      *(ptr+i) = USBDL_GetUARTByte();
   }
     //   *(ptr+i) = USBDL_GetUARTByte();
 /*      while( 0 == usbacm_rx_buf.len )
      {
      USB_HISR();
      }
      //   LOGI("%s: i[%d]\n", __func__, i);

      // get data from buf
      USBDL_GetByteFromRxBuf(ptr+i);

      //	  LOGI("%s: done\n", __func__);*/

   }
   return KAL_TRUE;

}

#else		// old DA PIO style, work around in it
/* transfer one byte data into an IN packet  */
void USB2COM_PutByte(kal_uint8 data)
{
   extern		USB_Drv_Info g_UsbDrvInfo;

   kal_uint8 	ep_num;


   /* PIO mode */
   ep_num = g_UsbACM.txpipe->byEP;

   USB_EPFIFOWrite(ep_num, 1, &data);
   USB_EP_Bulk_In_Ready(ep_num);
   //for delay .Because IN sysPC() call this function continuous
   USB_EP_Poll_Bulk_In_Ready(ep_num);
   mdelay(10);		//04404_test, 2013.06.26, dont know work or not

}


void USB2COM_PutByte_Buffer(kal_uint32* addr, kal_uint32 length)
{
   kal_uint32     packet_length, len = length;
   kal_uint32     cur_length = 0;
   kal_uint8      *ptr = (kal_uint8 *)addr;

   //print_data(ptr, length);

   /* PIO mode */
   if (USB_IS_Super_Speed() == KAL_TRUE)
   {
      packet_length = USB_BULK_SS_MAXP;
   }
   else if (USB_IS_High_Speed() == KAL_TRUE)
   {
      packet_length = USB_BULK_HS_MAXP;
   }
   else
   {
      packet_length = USB_BULK_MAXP;
   }

   if (length > packet_length)
   {
      while (len > 0) {
         cur_length = (len >= packet_length) ? (packet_length) : (len);
         USB_EPFIFOWrite(g_UsbACM.txpipe->byEP, cur_length, ptr);
         USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
         USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);   // check if TX is completed
         mdelay(10);		//04404_test, 2013.06.26, dont know work or not
         len -= cur_length;
         ptr += cur_length;
      }
      if (cur_length == packet_length) {  // sending ZLP
         USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
         USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);   // check if TX is completed
         mdelay(10);		//04404_test, 2013.06.26, dont know work or not
         LOGD("ZLP sent(%d,%d)\n", length, packet_length);
      }
      usbacm_tx_buf.len = 0;
   }
   else
   {

      USB_EPFIFOWrite(g_UsbACM.txpipe->byEP, length, addr);
      USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
      USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
      mdelay(10);		//04404_test, 2013.06.26, dont know work or not

      if (length == packet_length) {  // sending ZLP
         USB_EP_Bulk_In_Ready(g_UsbACM.txpipe->byEP);
         USB_EP_Poll_Bulk_In_Ready(g_UsbACM.txpipe->byEP);   // check if TX is completed
         mdelay(10);		//04404_test, 2013.06.26, dont know work or not
         LOGD("ZLP sent(%d,%d)\n", length, packet_length);
      }
      usbacm_tx_buf.len = 0;
   }

   LOGD("end of USB2COM_PutByte_Buffer\n");

}
/*
It is called by bootloader to flush the tx buffer into USB FIFO
*/
void USB2COM_PutByte_Complete(kal_uint8 data)
{
   USB2COM_PutByte(data);
}

kal_uint8 USB2COM_GetByte(void)
{

#if 0		//04404_test, 2013.06.26
   kal_uint32  cur_length, data;
   LOGD("usbacm_rx_buf.len :%d, 1\n", usbacm_rx_buf.len);
   if(usbacm_rx_buf.index == usbacm_rx_buf.len)
   {
      usbacm_rx_buf.len = 0;
      usbacm_rx_buf.index = 0;
   }

   LOGD("usbacm_rx_buf.len :%d, 2\n", usbacm_rx_buf.len);
   if(usbacm_rx_buf.len == 0)
   {
      while((USB_EP_Poll_Bulk_Out_Intr(g_UsbACM.rxpipe->byEP)==KAL_FALSE));

      mdelay(10);	//04404_test

      cur_length = USB_EP_Out_Pkt_Len(g_UsbACM.rxpipe->byEP);
#ifdef USB_IP_U3
      if(cur_length && cur_length <= USB_BULK_SS_MAXP)
#else
      if(cur_length && cur_length <= USB_BULK_HS_MAXP)
#endif
         USB_EPFIFORead(g_UsbACM.rxpipe->byEP,cur_length ,usbacm_rx_buf.data);
      LOGD("USB2COM_GetByte, cur_length : %d\n", cur_length);
      USB_EP_Bulk_Out_Ready(g_UsbACM.rxpipe->byEP);
      usbacm_rx_buf.len += cur_length;
   }


   data = usbacm_rx_buf.data[usbacm_rx_buf.index];
   LOGD("USB2COM_GetByte, return data :%x, usbacm_rx_buf.index : %d\n", data, usbacm_rx_buf.index);
   usbacm_rx_buf.index++;
   return data;
#endif

   /*org*/

   if(usbacm_rx_buf.index == usbacm_rx_buf.len)
   {
      usbacm_rx_buf.len = 0;
      usbacm_rx_buf.index = 0;
   }



   while (usbacm_rx_buf.len == 0)
   {
      USB_HISR();
   }

   return usbacm_rx_buf.data[usbacm_rx_buf.index++];
}


kal_uint16 USB2COM_GetData16(void)			//ok, high byte is first
{
   uint8	tmp,index;
   uint16 	tmp16;
   uint16  result =0;

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


   extern		USB_Drv_Info g_UsbDrvInfo;
   kal_uint8 	ep_num;

   for (index =0;index < 2;index++)
   {
      tmp16 = (data >> (8-8*index));
      tmp = (kal_uint8)tmp16;
      put_data[index] = tmp;

   }

   /* PIO mode */
   ep_num = g_UsbACM.txpipe->byEP;
   USB_EPFIFOWrite(ep_num, 2, &put_data);
   USB_EP_Bulk_In_Ready(ep_num);
   //for delay .Because IN sysPC() call this function continuous
   USB_EP_Poll_Bulk_In_Ready(ep_num);
   mdelay(10);		//04404_test, 2013.06.26, dont know work or not

}

kal_uint32 USB2COM_GetData32(void)			//ok, high byte is first
{
   kal_uint8	tmp,index;
   kal_uint32 	tmp32;
   kal_uint32  result =0;

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

   extern		USB_Drv_Info g_UsbDrvInfo;
   kal_uint8 	ep_num;


   for (index =0;index < 4;index++)
   {
      tmp32 = (data >> (24-8*index));
      tmp = (kal_uint8)tmp32;
      put_data[index] = tmp;

   }


   /* PIO mode */
   ep_num = g_UsbACM.txpipe->byEP;
   USB_EPFIFOWrite(ep_num, 4, &put_data);
   USB_EP_Bulk_In_Ready(ep_num);
   //for delay .Because IN sysPC() call this function continuous
   USB_EP_Poll_Bulk_In_Ready(ep_num);
   mdelay(10);		//04404_test, 2013.06.26, dont know work or not

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
   kal_uint32 loop;

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
   kal_uint32     cur_length = 0, org_length = length;
   kal_uint8      *ptr = (kal_uint8 *)addr;


   /* PIO mode */
   while (length > 0){

      while((USB_EP_Poll_Bulk_Out_Intr(g_UsbACM.rxpipe->byEP)==KAL_FALSE));

      mdelay(10);	//04404_test, 2013.06.26

      cur_length = USB_EP_Out_Pkt_Len(g_UsbACM.rxpipe->byEP);
#ifdef USB_IP_U3
      if(cur_length && cur_length <= USB_BULK_SS_MAXP)
#else
      if(cur_length && cur_length <= USB_BULK_HS_MAXP)
#endif
         USB_EPFIFORead(g_UsbACM.rxpipe->byEP,cur_length ,ptr);

      //LOGD("USB2COM_GetByte_Buffer, cur_length : %d\n", cur_length);
      //print_data(ptr, cur_length);
      USB_EP_Bulk_Out_Ready(g_UsbACM.rxpipe->byEP);
      length -= cur_length;
      ptr += cur_length;

   }

   usbacm_rx_buf.len = 0;
   usbacm_rx_buf.index = 0;

   //print_data((kal_uint8 *)addr, org_length);
   return KAL_TRUE;
}
#endif



void USB2COM_SetupDL(bool usb_dl_mode, bool enable_hs)
{
#define WPLL_CON0       ((volatile unsigned short *)0xC00071C0)
#define UPLL_CON0       ((volatile unsigned short *)0xC0007160)

#define PMIC_BANK0_SLAVE_ADDR 0xC0
   USB_Enable_HS(enable_hs);
   if( !usb_dl_mode) { /* forcely switch to USB download mode */
#if !defined(SKIP_EMMC_FLASH_WRITE)
      //usb_phy_poweron();
#endif
      USB2COM_Setup_PORT();
   } else {
      if (InheritedUSB == KAL_FALSE)	{

         //04404, marked first for not doing any phy related action
#if 0
#if !defined(SKIP_EMMC_FLASH_WRITE)
         /* 04404, marked first, shouldn't disc here, it will be done in USB2COM_Setup_PORT */
         uint32 usbpower = DRV_Reg32(U3D_POWER_MANAGEMENT);
         usbpower &= ~SOFT_CONN;
         DRV_WriteReg32(U3D_POWER_MANAGEMENT, usbpower);
         gpt4_busy_wait_ms(200);
         usb_phy_savecurrent();
#endif
#endif

         USB_DeRegister_CreateFunc();
         USB_DeRegister_Device_Code();
#if 0
#if !defined(SKIP_EMMC_FLASH_WRITE)
         usb_phy_recover();
#endif
#endif

#ifndef CFG_FPGA_PLATFORM
         /* to prevent preloader fail back to BROM->DA case, use for real chip, temporaily marked for 6795 */
         //usb_phy_vusb10_on();
#endif
         USB2COM_Setup_PORT();
      } else {
         LOGD("Inherited USB from prelaoder, don't do enumeration\n");
      }
   }
}
//-----------------------------------------------------------------------------
void USB2COM_SetErrThreshold(uint32 err_thres)
{
   err_threshold = err_thres;
}
//-----------------------------------------------------------------------------
bool COM_Is_USB_Download_Mode(void)
{
   kal_uint32 dwTemp;

   dwTemp = DRV_Reg32(U3D_DEVICE_CONF) & SSUSB_DEV_SPEED;
   if(	dwTemp == SSUSB_SPEED_FULL ||
      dwTemp == SSUSB_SPEED_HIGH ||
      dwTemp == SSUSB_SPEED_SUPER )
      return TRUE;
   else
      return FALSE;

}

#if 1 // obseleted
void USB2UART_Polling_Recv_Data(void)
{
   LOGD("%s, currently not implemented\n", __func__);
}

void USB2COM_GetDatas(kal_uint32 length)
{
   LOGD("%s, currently not implemented\n", __func__);
}
void USB2COM_PutDatas(kal_uint32 length)
{
   LOGD("%s, currently not implemented\n", __func__);
}
#endif
