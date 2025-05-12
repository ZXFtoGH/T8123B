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
#include "usb_comm.h"
#include "usb_drv.h"
#include "usb_hw.h"
#include "usb.h"
#include "usb_mode.h"
#include "usb_resource.h"
#include "usbacm_drv.h"
#include "usb_custom.h"
#include "usb_debug_tool.h"
#include "debug.h"
#include "assert.h"
#include "lib/string.h"

/* Device structure, handle usb level data */
Usb_Device gUsbDevice;
Usb_SetUp_Info gUsbSetupInfo;

extern void USB_Initialize_Drv(void);
extern void USB_Set_DP_Pull_High(kal_bool bset);
extern void	mem_set(void *dest, uint8 default_value, uint32 len);

#if defined(USB_IP_V3) || defined(USB_IP_V4) || defined(USB_IP_U3)
extern void USB_Reset_FIFO(void);
#endif


void USB_Reset(void);
/* static fuctions */
static void USB_Initialize(kal_bool setup);
static kal_bool USB_Cmd_SetAddress(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);
static kal_bool USB_Cmd_GetDescriptor(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);
static kal_bool USB_Cmd_SetConfiguration(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);
static kal_bool USB_Cmd_GetConfiguration(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);
static kal_bool USB_Cmd_SetFeature(Usb_Command *pcmd, kal_bool bset);
static kal_bool USB_Cmd_GetStatus(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);
static void USB_Stdcmd(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);
static void USB_Endpoint0_Idle (void);
static void USB_Endpoint0_Rx(void);
static void USB_Endpoint0_Tx(void);
static void USB_Endpoint0_Hdlr(void);


/************************************************************
gUsbDevice initialize and release functions
*************************************************************/
#ifdef USB_UT
kal_uint8 g_iobuf[1024*10];
kal_uint32 loopback_cnt = 0;
extern usbacm_buf_struct usbacm_rx_buf;
void print_data(kal_uint8* data, kal_int32 len);
#ifdef USB_IP_U3
extern kal_bool gUSBAcm_IsAcmOpen;
#endif
kal_uint32 usb_loopback(void)
{
   kal_uint32 *addr = (kal_uint32 *)g_iobuf;
   kal_uint32 len;
   kal_uint32 status=0;
   kal_uint32 csr, EPno, ret_len;

   static kal_uint32 first_shot = 1;

   EPno = 1;

   //dump_csr();
#if 0
   gUSBAcm_IsAcmOpen = KAL_FALSE;

   while(gUSBAcm_IsAcmOpen == KAL_FALSE)
   {
      USB_HISR();
   }
   LOGD("open com port ok\n");

   len = USB2COM_GetData32();
   LOGD("len:%x\n", len);


   LOGD("special test\n");
   USB2COM_GetByte_Buffer(addr, len);
   LOGD("special test 1\n");
   USB2COM_GetByte_Buffer(addr, len);
   return 0;
#endif

#if 0
   if(first_shot)
   {

      gUSBAcm_IsAcmOpen = KAL_FALSE;

      while(gUSBAcm_IsAcmOpen == KAL_FALSE)
      {
         USB_HISR();
      }
      LOGD("open com port ok\n");
      first_shot = 0;
   }
#endif

   // get length
   len = USB2COM_GetData32();
   LOGD("len:%x\n", len);


#if 0
   // receive data
   LOGD("just before block read\n");
   COM_GetByte_Buffer(addr, len);
   LOGD("just after block read\n");
   return 0;
#endif

   USB2COM_PutData32(len);


   LOGD("after put data 32, len : %x\n", len);

   //return 0;	//04404_test


   LOGD("reply status:%x\n", status);

   USB2COM_PutData16(status);

   // receive data
   LOGD("just before block read\n");
   USB2COM_GetByte_Buffer(addr, len);
   LOGD("just after block read\n");
   //print_data((kal_uint8 *)addr, len);


   // receive data again
   LOGD("just before block read1\n");
   USB2COM_GetByte_Buffer(addr, len);
   LOGD("just after block read1\n");
   //print_data((kal_uint8 *)addr, len);

   // return ok

   USB2COM_PutData16(status);		//04404

   // loopback data
   USB2COM_PutByte_Buffer(addr, len);


   // return ok
   USB2COM_PutData16(status);

#if 0
   loopback_cnt += 1;
   if(loopback_cnt == 2)
      USB_QMU_Init_and_Start();
#endif


   //dump_csr();
   LOGD("Loopback: %d, usbacm_rx_buf.len : %d\n", len, usbacm_rx_buf.len);
   return status;
}


void USB_TEST(void)
{
#if 1
   uint32 cmd;

   LOGD("wait 3 sec to test USB mode...\n");
   mdelay(3000);
   LOGD("3 sec done\n");

   LOGD("wait d9\n");
   cmd = USB2COM_GetByte();
   LOGD("got d9\n");
#endif

   LOGD("Entering test USB mode...\n");

   while(1)
   {
      usb_loopback();
   }
}

void ENUM_TEST(void)
{
   USB_Set_DP_Pull_High(KAL_FALSE);
   LOGD("wait 1000 msec\n");
   mdelay(1000);
   USB_Set_DP_Pull_High(KAL_TRUE);

   gUsbDevice.nDevState = DEVSTATE_DEFAULT;
   while (gUsbDevice.nDevState != DEVSTATE_CONFIG)
      USB_HISR();

   LOGD("enumeration done\n");
   while(1);

}
#endif


/* initialize the  global variable gUsbDevice */
void USB_Init_Device_Status(void)
{
   kal_uint8 index = 0;

   memset(&gUsbDevice.cfg_info, 0, sizeof(Usb_Config_Info));

   for( index = 0; index < USB_MAX_INTERFACE; index++)
   {
      memset(&gUsbDevice.if_info[index], 0, sizeof(Usb_Interface_Info));
   }
   for( index = 0; index < USB_MAX_EP_BULK_IN; index++)
   {
      memset(&gUsbDevice.ep_bulkin_info[index], 0, sizeof(Usb_Ep_Info));
   }
   for( index = 0; index < USB_MAX_EP_BULK_OUT; index++)
   {
      memset(&gUsbDevice.ep_bulkout_info[index], 0, sizeof(Usb_Ep_Info));
   }
   for( index = 0; index < USB_MAX_EP_INTR; index++)
   {
      memset(&gUsbDevice.ep_intr_info[index], 0, sizeof(Usb_Ep_Info));
   }
   for( index = 0; index < USB_MAX_STRING; index++)
   {
      gUsbDevice.resource_string[index] = NULL;
   }
   gUsbDevice.conf = NULL;
   gUsbDevice.device_type = USB_UNKOWN;
   gUsbDevice.nDevState = DEVSTATE_DEFAULT;
   gUsbDevice.remoteWk = KAL_FALSE;
   gUsbDevice.self_powered = KAL_FALSE;
   gUsbDevice.config_num = 0;
   gUsbDevice.interface_num = 0;
   gUsbDevice.ep0_rx_handler = NULL;
   gUsbDevice.ep0_class_cmd_handler.b_enable = KAL_FALSE;

   gUsbDevice.resource_ep_bulkin_number = 0;
   gUsbDevice.resource_ep_bulkout_number = 0;
   gUsbDevice.resource_ep_intr_number = 0;
   gUsbDevice.resource_interface_number = 0;
   gUsbDevice.resource_string_number = 0;
}



/************************************************************
EP0 functions
*************************************************************/

/* register EP0 rx OUT token data handler */
void USB_Register_EP0_RxHdlr(usb_ep0_rx_ptr handler)
{
   gUsbDevice.ep0_rx_handler = handler;
}



/* prepare TX data infomration for pep0state, data is actually sent out in TX state handler */
void USB_Generate_EP0Data(Usb_Ep0_Status*pep0state, Usb_Command*pcmd, void *data, kal_int32 len)
{
   pep0state->nBytesLeft = len;
   pep0state->pData = (void*)data;
   /* only transmit at most command request */
   if (pcmd->wLength < pep0state->nBytesLeft)
      pep0state->nBytesLeft = pcmd->wLength;

   //LOGD("USB_Generate_EP0Data, nBytesLeft : %d, wLength : %d\n", pep0state->nBytesLeft, pcmd->wLength);//04404_test

   /* EP0 go to TX state */
   gUsbDevice.ep0_state = USB_EP0_TX;
}

/************************************************************
system ctrl functions
*************************************************************/

/* init function, called when user select usb type,
entry function for task , B_eanble is D+ detection enable */
void USB_Init(USB_DEVICE_TYPE type, kal_bool b_enable)
{
   USB_Init_Drv_Info();			/*initialize driver parameters*/
   USB_Init_Device_Status();	/*initialize global variable gUsbDevice*/

   USB_Init_Acm_Status();
   gUsbDevice.device_type = USB_CDC_ACM;

   /* register reset and ep0 interrupt handler to driver info*/
   USB_Register_Drv_Info(USB_DRV_HDLR_RESET, 0, USB_Reset);
   USB_Register_Drv_Info(USB_DRV_HDLR_EP0, 0, USB_Endpoint0_Hdlr);

   /* create descriptors */
   USB_Software_Create();
   if (b_enable == KAL_TRUE)
  	{
      /* reset and initialize system initial value and registers*/
      USB_Reset_Drv();  /* reset hw power register */
      USB_Initialize_Drv();

      /* this function must be called after vbus come in, i.e. cable is connected to host */
      /* becoz after this function, device has attempt to negotiate with host */
      USB_Set_DP_Pull_High(KAL_TRUE);
  	}
   else
      /* initailize according to application*/
      USB_Initialize(KAL_TRUE);	   // FOR brom usb2.0
}


/* reset device, called when receive reset interrupt*/
void USB_Reset(void)
{
   kal_uint8 ep_num;

   gUsbDevice.is_configured_now = KAL_FALSE;

   // initialize driver info and system interrupt
   USB_ResetDrv_Info();
   USB_Initialize_Drv();
   USB_Initialize(KAL_TRUE);        /* call class reset function */
   for(ep_num=0;ep_num<gUsbDevice.resource_ep_bulkin_number;ep_num++)
   {
      if(gUsbDevice.ep_bulkin_info[ep_num].ep_reset)
         gUsbDevice.ep_bulkin_info[ep_num].ep_reset();
   }
   for(ep_num=0;ep_num<gUsbDevice.resource_ep_bulkout_number;ep_num++)
   {
      if(gUsbDevice.ep_bulkout_info[ep_num].ep_reset)
         gUsbDevice.ep_bulkout_info[ep_num].ep_reset();
   }
   for(ep_num=0;ep_num<gUsbDevice.resource_ep_intr_number;ep_num++)
   {
      if(gUsbDevice.ep_intr_info[ep_num].ep_reset)
         gUsbDevice.ep_intr_info[ep_num].ep_reset();
   }
}

/* initialize system */
static void USB_Initialize(kal_bool setup)
{
   kal_uint8 index_ep;
   kal_uint16 wMaxPacketSize;

   if (setup == KAL_TRUE)
   {
#ifdef __USB_HS_ENABLE__
      /* initial class specific speed-related endpoint, and re-prepare descriptor */
      USB_Software_Speed_Init(KAL_FALSE);
#endif


#if defined(USB_IP_V3) || defined(USB_IP_V4) || defined(USB_IP_U3)
      USB_Reset_FIFO();
#endif

      /* configure ep for hareware */
      for( index_ep = 0; index_ep < gUsbDevice.resource_ep_bulkin_number; index_ep++)
      {
         wMaxPacketSize = (kal_uint8)gUsbDevice.ep_bulkin_info[index_ep].epdesc.stdep.wMaxPacketSize[1];
         wMaxPacketSize <<= 8;
         wMaxPacketSize |= (kal_uint8)gUsbDevice.ep_bulkin_info[index_ep].epdesc.stdep.wMaxPacketSize[0];
#ifdef USB_IP_U3
         //LOGD("index_ep : %d, wMaxPacketSize : %d, USB_InEPInit\n", index_ep, wMaxPacketSize);//04404_test
         USB_InEPInit((gUsbDevice.ep_bulkin_info[index_ep].epdesc.stdep.bEndpointAddress&(~USB_EP_DIR)),
            wMaxPacketSize, USB_ENDPT_BULK); //wMaxPacketSize
#else
         USB_InEPInit((gUsbDevice.ep_bulkin_info[index_ep].epdesc.stdep.bEndpointAddress&(~USB_EP_DIR)),
            512, USB_ENDPT_BULK); //wMaxPacketSize
#endif
         gUsbDevice.ep_bulkin_info[index_ep].ep_status.epin_status.nBytesLeft = USB_EP_NODATA;
      }
      for( index_ep = 0; index_ep < gUsbDevice.resource_ep_bulkout_number; index_ep++)
      {
         wMaxPacketSize = (kal_uint8)gUsbDevice.ep_bulkout_info[index_ep].epdesc.stdep.wMaxPacketSize[1];
         wMaxPacketSize <<= 8;
         wMaxPacketSize |= (kal_uint8)gUsbDevice.ep_bulkout_info[index_ep].epdesc.stdep.wMaxPacketSize[0];

         //LOGD("index_ep : %d, wMaxPacketSize : %d, USB_OutEPInit\n", index_ep, wMaxPacketSize);//04404_test
         USB_OutEPInit((gUsbDevice.ep_bulkout_info[index_ep].epdesc.stdep.bEndpointAddress&(~USB_EP_DIR)),
            wMaxPacketSize, USB_ENDPT_BULK); //wMaxPacketSize
         gUsbDevice.ep_bulkout_info[index_ep].ep_status.epout_status.nBuffLen = 0;
         gUsbDevice.ep_bulkout_info[index_ep].ep_status.epout_status.nBytesRecv = 0;
      }

      for( index_ep = 0; index_ep < gUsbDevice.resource_ep_intr_number; index_ep++)
      {
         wMaxPacketSize =  (kal_uint8)gUsbDevice.ep_intr_info[index_ep].epdesc.stdep.wMaxPacketSize[1];
         wMaxPacketSize <<= 8;
         wMaxPacketSize |= (kal_uint8)gUsbDevice.ep_intr_info[index_ep].epdesc.stdep.wMaxPacketSize[0];

         USB_InEPInit((gUsbDevice.ep_intr_info[index_ep].epdesc.stdep.bEndpointAddress&(~USB_EP_DIR)),
            wMaxPacketSize, USB_ENDPT_INTR); //wMaxPacketSize
         gUsbDevice.ep_intr_info[index_ep].ep_status.epin_status.nBytesLeft = USB_EP_NODATA;
      }
   }

   /* Clear current configuration pointer */
   //gUsbDevice.pCurCfg = &gUsbDevice.cfg_info[0].stdcfg;  /* pointer point to configuration descriptor */
   gUsbDevice.nDevState = DEVSTATE_DEFAULT;
   gUsbDevice.self_powered = KAL_FALSE;
   gUsbDevice.remoteWk = KAL_FALSE;
   gUsbDevice.config_num = 0;        /*  set configuration command value  */
   gUsbDevice.interface_num = 0;    /*  set inferface command value  */
   gUsbDevice.ep0_state = USB_EP0_IDLE;
   gUsbDevice.ep0info.byFAddr = 0xff;   /*  device (function) address, no use, at HW still set 0x00 */

   /* initial class specific interface functions*/
   USB_Software_Init();
}

/************************************************************
EP0 functions
*************************************************************/

/* parse command Set Address */
static kal_bool USB_Cmd_SetAddress(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool bError = KAL_FALSE;

   /* Store device function address until status stage of request */
   if (pcmd->bmRequestType != USB_CMD_STDDEVOUT)
   {
      bError = KAL_TRUE;
   }
   else
   {
      if (gUsbDevice.nDevState <= DEVSTATE_ADDRESS)
      {
         pep0state->byFAddr = (kal_uint8)pcmd->wValue;
         if ((gUsbDevice.nDevState == DEVSTATE_DEFAULT) && (pep0state->byFAddr<=127))
         {
            gUsbDevice.nDevState = DEVSTATE_SET_ADDRESS;
            //LOGD("set addr : %d\n", pep0state->byFAddr);//04404_test
            USB_SetAddress(pep0state->byFAddr, USB_SET_ADDR_DATA);
         }
         else
         {
            gUsbDevice.nDevState = DEVSTATE_DEFAULT;
         }
      }
      else
      {
         bError = KAL_TRUE;
      }
   }

   return bError;
}

/* parse command Get Descriptor */
static kal_bool USB_Cmd_GetDescriptor(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool bError = KAL_FALSE;

   kal_uint8 byConfig;
   kal_uint8 bystr;
   Usb_Cfg_Dscr *pcfg;

#ifdef USB_IP_U3
   kal_uint8 bos_dscr[USB_DT_BOS_SIZE + USB_DT_USB_EXT_CAP_SIZE + USB_DT_USB_SS_CAP_SIZE];
#endif

   //LOGD("pcmd->wValue : %x\n", pcmd->wValue);	//04404_test
   /* Decode the required descriptor from the command */
   if (pcmd->bmRequestType != USB_CMD_STDDEVIN)
   {
      bError = KAL_TRUE;
   }
   else
   {
      switch (pcmd->wValue & USB_CMD_DESCMASK)
      {
      case USB_CMD_DEVICE:
         /* Prepare to return Standard Device Descriptor */
#ifdef USB_IP_U3
         if(USB_IS_Super_Speed() == KAL_TRUE)
         {
            gUsbDevice.devdscr.bcdUSB = 0x0300;
         }
         else
         {
            gUsbDevice.devdscr.bcdUSB = 0x0110;
         }
#endif

         USB_Generate_EP0Data(pep0state, pcmd, &gUsbDevice.devdscr, sizeof(Usb_Dev_Dscr));
         break;
      case USB_CMD_CONFIG:

#ifdef __USB_HS_ENABLE__
         USB_Software_Speed_Init(KAL_FALSE);
#endif
         byConfig = (kal_uint8)(pcmd->wValue & 0x00FF);
         if (byConfig >= gUsbDevice.devdscr.bNumConfigurations || byConfig >= USB_MAX_CONFIG)
         {
            bError = KAL_TRUE;
         }
         else
         {
            /* Get pointer to requested configuration descriptor */
            pcfg = (Usb_Cfg_Dscr*)gUsbDevice.conf;

#ifdef  __USB_MULTI_CHARGE_CURRENT__
#if ( defined(MT6305) || defined(MT6318) )
            /*  memory copy to modify descriptor */
            memcpy( (kal_uint8*) &(pcfg->bConfigurationValue),
               (kal_uint8*) &gUsbDevice.multi_configuration_value[byConfig] ,
               1);
            memcpy( (kal_uint8*) &(pcfg->bMaxPower),
               (kal_uint8*) &gUsbDevice.multi_Max_Power[byConfig] ,
               1);
#endif
#endif
            /* Prepare to return Configuration Descriptors */
            USB_Generate_EP0Data(pep0state, pcmd, pcfg, pcfg->wTotalLength);
         }
         break;
      case USB_CMD_STRING:
         bystr = (kal_uint8)(pcmd->wValue & 0x00FF);
         if (bystr >= gUsbDevice.resource_string_number || bystr >= USB_MAX_STRING)
         {
            bError = KAL_TRUE;
         }
         else
         {
            /* Get pointer to requested string descriptor */
            USB_Generate_EP0Data(pep0state, pcmd, (void *)gUsbDevice.resource_string[bystr],
               (*(kal_uint8 *)gUsbDevice.resource_string[bystr]));
         }
         break;
#ifdef USB_IP_U3
      case USB_CMD_BOS:
         /* Prepare to return BOS Descriptor */
         memcpy( (kal_uint8*) bos_dscr,
            &(gUsbDevice.bos_info.bos_dscr),
            USB_DT_BOS_SIZE);
         memcpy( (kal_uint8*) bos_dscr + USB_DT_BOS_SIZE,
            &(gUsbDevice.bos_info.usb_ext_cap_dscr),
            USB_DT_USB_EXT_CAP_SIZE);
         memcpy( (kal_uint8*) bos_dscr + USB_DT_BOS_SIZE + USB_DT_USB_EXT_CAP_SIZE,
            &(gUsbDevice.bos_info.ss_cap_dscr),
            USB_DT_USB_SS_CAP_SIZE);
         USB_Generate_EP0Data(pep0state, pcmd, bos_dscr, gUsbDevice.bos_info.bos_dscr.wTotalLength);
         break;
#endif
      default:
         bError = KAL_TRUE;
         break;
      }
   }

   return bError;
}


/* parse command Set Configuration */
static kal_bool USB_Cmd_SetConfiguration(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool bError = KAL_FALSE;
   kal_uint8 byConfig;

   /*mark for build warning
   #ifndef  __USB_MULTI_CHARGE_CURRENT__
   kal_bool bConfirm = KAL_FALSE;
   #endif
   */


   byConfig = (kal_uint8)(pcmd->wValue & 0x00FF);
   if (gUsbDevice.nDevState == DEVSTATE_DEFAULT)
   {
      bError = KAL_TRUE;
   }
   /* Assumes configurations are numbered from 1 to NumConfigurations */
   else
   {
      if (byConfig > gUsbDevice.devdscr.bNumConfigurations || byConfig > USB_MAX_CONFIG)
      {
         bError = KAL_TRUE;
      }
      else if (byConfig == 0)
      {
         gUsbDevice.nDevState = DEVSTATE_ADDRESS;
         gUsbDevice.config_num = byConfig;
      }
      else
      {
         gUsbDevice.nDevState = DEVSTATE_CONFIG;
         gUsbDevice.is_configured_now = KAL_TRUE;
         gUsbDevice.config_num = byConfig;

         /*mark for build warning
         #ifndef __USB_MULTI_CHARGE_CURRENT__
         bConfirm = KAL_TRUE;
         #endif
         */
      }
   }

   return bError;
}


/* parse command Get Configuration */
static kal_bool USB_Cmd_GetConfiguration(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool bError = KAL_FALSE;

   //	Usb_Cfg_Dscr *pcfg;

   if( (gUsbDevice.nDevState == DEVSTATE_ADDRESS) && (!pcmd->wValue))
   {
      /* Prepare to return zero */
      USB_Generate_EP0Data(pep0state, pcmd, &pcmd->wValue, 1);
   }
   else if( gUsbDevice.nDevState == DEVSTATE_CONFIG)
   {
      USB_Generate_EP0Data(pep0state, pcmd, &gUsbDevice.config_num, 1);
   }
   else
   {
      bError = KAL_TRUE;
   }
   return bError;
}

/* parse command Set Interface */
static kal_bool USB_Cmd_SetInterface(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool bError = KAL_FALSE;
   kal_uint8 intf = pcmd->wValue & 0xff;

   if (intf < USB_MAX_INTERFACE)
      gUsbDevice.interface_num = intf;
   else
      bError = KAL_TRUE;
   return bError;
}

/* parse command Get Interface */
static kal_bool USB_Cmd_GetInterface(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool bError = KAL_FALSE;

   USB_Generate_EP0Data(pep0state, pcmd, &gUsbDevice.interface_num, 1);

   return bError;
}


/* parse command Set/Clear Feature */
/* bset  true means command SET_FETURE, false means command CLEAR_FEATURE */
static kal_bool USB_Cmd_SetFeature(Usb_Command *pcmd, kal_bool bset)
{
   kal_bool bError = KAL_FALSE;

   switch (pcmd->bmRequestType)
   {
      /* device */
   case USB_CMD_STDDEVOUT:
      switch (pcmd->wValue)
      {
      case USB_FTR_DEVREMWAKE:
         gUsbDevice.remoteWk = bset;
         break;
      default:
         bError = KAL_TRUE;
         break;
      }
      break;
      /* endpoint */
   case USB_CMD_STDEPOUT:
      switch (pcmd->wValue)
      {
      case USB_FTR_EPHALT:
         if (pcmd->wIndex == 0)
         {
            /* endpoint 0 */
            ;
         }
         else
         {
            /* command EP direction zero indicate OUT EP */
            if(pcmd->wIndex & 0x80)
            {
               /* In EP*/
               if(((pcmd->wIndex & 0x000f) > MAX_INTR_EP_NUM) || ((pcmd->wIndex & 0x000f) == 0))
               {
                  bError = KAL_TRUE;
               }
               else
               {
                  USB_CtrlEPStall((pcmd->wIndex& 0x0f), USB_IN_EP_TYPE, bset, USB_CTRL_STALL_ENTRY_1);
               }
            }
            else
            {
               /* Out EP*/
               if(((pcmd->wIndex & 0x000f) > MAX_INTR_EP_NUM) || ((pcmd->wIndex & 0x000f) == 0))
               {
                  bError = KAL_TRUE;
               }
               else
               {
                  USB_CtrlEPStall((pcmd->wIndex& 0x0f), USB_OUT_EP_TYPE, bset, USB_CTRL_STALL_ENTRY_1);
               }
            }
         }
         break;
      default:
         bError = KAL_TRUE;
         break;
      }
      break;
   case USB_CMD_STDIFOUT:
   default:
      bError = KAL_TRUE;
      break;
   }

   return bError;
}

/* parse command Get Status */
static kal_bool USB_Cmd_GetStatus(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool bError = KAL_FALSE;
   kal_uint16 status = 0;
   switch (pcmd->bmRequestType)
   {
   case USB_CMD_STDDEVIN:
      status = ( (kal_uint16)(gUsbDevice.remoteWk << 1) | (kal_uint16)(gUsbDevice.self_powered) );
      USB_Generate_EP0Data( pep0state, pcmd, &status, 2);
      break;
   case USB_CMD_STDIFIN:
      USB_Generate_EP0Data( pep0state, pcmd, &status, 2);
      break;
   case USB_CMD_STDEPIN:
      if(pcmd->wIndex & 0x80)
      {
         /* TX EP*/
         if(((pcmd->wIndex & 0x000f) > MAX_INTR_EP_NUM) || ((pcmd->wIndex & 0x000f) == 0))
         {
            bError = KAL_TRUE;
         }
         else
         {
            /* In EP*/
            status = (kal_uint16)USB_Get_EP_Stall_Status((pcmd->wIndex & 0x000f), USB_IN_EP_TYPE);
         }
      }
      else
      {
         /* RX EP*/
         if(((pcmd->wIndex & 0x000f) > MAX_INTR_EP_NUM) || ((pcmd->wIndex & 0x000f) == 0))
         {
            bError = KAL_TRUE;
         }
         else
         {
            /* Out EP*/
            status = (kal_uint16)USB_Get_EP_Stall_Status((pcmd->wIndex & 0x000f), USB_OUT_EP_TYPE);
         }
      }
      USB_Generate_EP0Data( pep0state, pcmd, &status, 2);
      break;
   default:
      bError = KAL_TRUE;
      break;
   }

   return bError;
}

#if defined(USB_IP_U3)
#define USB_SET_SEL 0x30
static void USB_SET_SEL_HDLR(void *data)
{
   kal_uint32 nCount;
   kal_int32 tmp_buf[64];

   /* read setting and set to UART structure*/
   nCount = USB_EP0_Pkt_Len();
   ;
   if((nCount > 0) && (nCount <= sizeof(tmp_buf)))
   {
      USB_EPFIFORead(0, nCount, tmp_buf);
      //u3 chip no RX status, so we change to IDLE state directly
      gUsbDevice.ep0_state = USB_EP0_IDLE;
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_RX_END, KAL_FALSE, KAL_TRUE);
   }

}
#endif

/* parse usb standard command */
static void USB_Stdcmd(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
   kal_bool   bError = KAL_FALSE;

   //LOGD("pcmd->bRequest: %x, pcmd->wValue : %x\n", pcmd->bRequest, pcmd->wValue);
   switch (pcmd->bRequest)
   {
   case USB_SET_ADDRESS:
      bError = USB_Cmd_SetAddress(pep0state, pcmd);
      break;
   case USB_GET_DESCRIPTOR:
      bError = USB_Cmd_GetDescriptor(pep0state, pcmd);
      break;
   case USB_SET_CONFIGURATION:
      bError = USB_Cmd_SetConfiguration(pep0state, pcmd);
      break;
   case USB_SET_INTERFACE:
      bError = USB_Cmd_SetInterface(pep0state, pcmd);
      break;
   case USB_GET_CONFIGURATION:
      bError = USB_Cmd_GetConfiguration(pep0state, pcmd);
      break;
   case USB_GET_INTERFACE:
      bError = USB_Cmd_GetInterface(pep0state, pcmd);
      break;
   case USB_SET_FEATURE:
      //bError = USB_Cmd_SetFeature(pcmd, KAL_TRUE);
      /* to pretend deal with feature done to avoid error */
      bError = KAL_FALSE;
      break;
   case USB_CLEAR_FEATURE:
      bError = USB_Cmd_SetFeature(pcmd, KAL_FALSE);
      break;
   case USB_GET_STATUS:
      bError = USB_Cmd_GetStatus(pep0state, pcmd);
      break;
#if defined(USB_IP_U3)
   case USB_SET_SEL:													////04404_test
      //LOGD("usb_stdcmd, USB_SET_SEL\n");
      USB_Register_EP0_RxHdlr(USB_SET_SEL_HDLR);
      gUsbDevice.ep0_state = USB_EP0_RX;
      break;
#endif
      /* Stall the command if an unrecognized request is received */
   case USB_SYNCH_FRAME:   /*Only support for Isoc traffic*/
   case USB_SET_DESCRIPTOR:
   default:
      LOGD("usb_stdcmd, in default case, ready to stall, set bError = true\n");//04404_test
      bError = KAL_TRUE;
      ASSERT(0);
      break;
   }


   if (gUsbDevice.ep0_state == USB_EP0_IDLE)	/* no data to transmit */
   {
#if defined(USB_IP_U3)

      //remove the RX STATUS, remain to idle state

      //set the address in this state
      if(gUsbDevice.nDevState == DEVSTATE_SET_ADDRESS)
      {
         USB_SetAddress(gUsbDevice.ep0info.byFAddr, USB_SET_ADDR_STATUS);
         gUsbDevice.nDevState = DEVSTATE_ADDRESS;
      }
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_END, bError, KAL_TRUE);

      if((gUsbDevice.ep0_class_cmd_handler.b_enable == KAL_TRUE) &&
         (gUsbDevice.ep0_class_cmd_handler.cmd == pcmd->bRequest) )
      {
         gUsbDevice.ep0_class_cmd_handler.ep0_cmd_hdlr(pcmd);/* Besides standard cmd, still need to do extra this*/
      }
#else
      gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
      USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, bError, KAL_TRUE);
      if((gUsbDevice.ep0_class_cmd_handler.b_enable == KAL_TRUE) &&
         (gUsbDevice.ep0_class_cmd_handler.cmd == pcmd->bRequest) )
      {
         gUsbDevice.ep0_class_cmd_handler.ep0_cmd_hdlr(pcmd);/* Besides standard cmd, still need to do extra this*/
      }
#endif
   }
   else /* has data stage */
   {
#if defined(USB_IP_U3)
      USB_U3_Update_EP0_State((gUsbDevice.ep0_state==USB_EP0_TX)?USB_EP0_DRV_STATE_SETUP_TO_TX:USB_EP0_DRV_STATE_SETUP_TO_RX, bError, KAL_FALSE);
#else
      USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, bError, KAL_FALSE);
#endif
   }
}


/* parse EP0 requested command */
static void USB_Endpoint0_Idle (void)
{
   kal_uint8 wIndex;
   /* read ep0 data*/
   USB_EPFIFORead(0, 8, &gUsbDevice.cmd);

   /* Check request type */
   switch (gUsbDevice.cmd.bmRequestType & USB_CMD_TYPEMASK)
   {
   case USB_CMD_STDREQ:
      /* standard request */
      MSG(USB, "USB STANDARD REQUEST\n");
      USB_Stdcmd(&gUsbDevice.ep0info, &gUsbDevice.cmd);
      break;
   case USB_CMD_CLASSREQ:
      wIndex = (kal_uint8)(gUsbDevice.cmd.wIndex & 0xff);
      /* class specific request */
      MSG(USB, "USB CLASS REQUEST\n");
      if((gUsbDevice.cmd.bmRequestType == USB_CMD_CLASSIFIN) || (gUsbDevice.cmd.bmRequestType == USB_CMD_CLASSIFOUT))
      {
         if(wIndex < gUsbDevice.resource_interface_number && wIndex < USB_MAX_INTERFACE &&
		         gUsbDevice.if_info[wIndex].if_class_specific_hdlr != NULL)
         {
            gUsbDevice.if_info[wIndex].if_class_specific_hdlr(&gUsbDevice.ep0info, &gUsbDevice.cmd);
         }
         else
         {
            // for interface sequence check
            USB_Change_Interface_Sequence();
            if(wIndex < gUsbDevice.resource_interface_number && wIndex < USB_MAX_INTERFACE &&
		gUsbDevice.if_info[wIndex].if_class_specific_hdlr != NULL)
            {
               gUsbDevice.if_info[wIndex].if_class_specific_hdlr(&gUsbDevice.ep0info, &gUsbDevice.cmd);
            }
            else/* error occur, stall endpoint*/
            {
#if defined(USB_IP_U3)
               USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_END, KAL_TRUE, KAL_FALSE);
#else
               USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, KAL_TRUE, KAL_FALSE);
#endif
            }
         }
      }
      else
      {
         /* error occur, stall endpoint*/
#if defined(USB_IP_U3)
         USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_END, KAL_TRUE, KAL_FALSE);
#else
         USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, KAL_TRUE, KAL_FALSE);
#endif
      }
      break;
   case USB_CMD_VENDREQ:
      MSG(USB, "USB VENDOR REQUEST\n");
#if defined(USB_IP_U3)
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_END, KAL_TRUE, KAL_FALSE);
#else
      USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, KAL_TRUE, KAL_FALSE);
#endif
      break;
   default:
      MSG(USB, "USB UNKNOWN REQUEST\n");
      /* Stall the command if a reserved request is received */
#if defined(USB_IP_U3)
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_SETUP_END, KAL_TRUE, KAL_FALSE);
#else
      USB_Update_EP0_State(USB_EP0_DRV_STATE_READ_END, KAL_TRUE, KAL_FALSE);
#endif
      break;
   }
}


/* EP0 RX handler, called when EP0 interrupt happened and in RX state */
static void USB_Endpoint0_Rx(void)
{
   if (gUsbDevice.ep0_rx_handler != NULL)
   {
      /* called rx handler to get data*/
      gUsbDevice.ep0_rx_handler(&gUsbDevice.ep0info);
   }
   else
   {
      /* this should not happened, user should register rx handler when set EP0 into RX state */
      /* error occur, stall endpoint*/
      ASSERT(0);
   }
}

void print_data(kal_uint8* data, kal_int32 len)			// 04404_test
{
   kal_int32 i;
   for(i =0 ;i < len ; i++)
   {
      LOGD("data[%d]:%x, %c\n", i, data[i], data[i]);
   }
}


/* EP0 TX handler, called when EP0 interrupt happened and in TX state,
or EP0 just translate from IDLE to TX state */
static void USB_Endpoint0_Tx(void)
{
   kal_int32 nBytes;
   kal_uint32 EP0_MAXP = USB_EP0_MAXP;
#ifdef USB_IP_U3
   if(USB_IS_Super_Speed() == KAL_TRUE)
   {
      EP0_MAXP = USB_EP0_SS_MAXP;
   }
#endif


   /* Determine number of bytes to send */
   if (gUsbDevice.ep0info.nBytesLeft <= EP0_MAXP)
   {
      nBytes = gUsbDevice.ep0info.nBytesLeft;
      gUsbDevice.ep0info.nBytesLeft = 0;
   }
   else
   {
      nBytes = EP0_MAXP;
      gUsbDevice.ep0info.nBytesLeft -= EP0_MAXP;
   }

   /* send out data */
   //LOGD("USB_Endpoint0_Tx nBytes:%x, EP0_MAXP : %x\n", nBytes, EP0_MAXP);		//04404_test
   USB_EPFIFOWrite (0, nBytes, gUsbDevice.ep0info.pData);
   //print_data((kal_uint8 *)gUsbDevice.ep0info.pData, nBytes);	//04404_test


   /* update data pointer and  prepare for next transaction */
   gUsbDevice.ep0info.pData = (kal_uint8 *)gUsbDevice.ep0info.pData + nBytes;

   if (nBytes < EP0_MAXP)
   {


      /* last data, set end as true*/
#if defined(USB_IP_U3)
      if(nBytes == 0)
      {
         gUsbDevice.ep0_state = USB_EP0_IDLE;
         //LOGD("USB_Endpoint0_Tx data end\n");		//04404_test

         USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_TX_END, KAL_FALSE, KAL_TRUE);
      }
      else
         USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_TX, KAL_FALSE, KAL_FALSE);
#else
      gUsbDevice.ep0_state = USB_EP0_IDLE;
      USB_Update_EP0_State(USB_EP0_DRV_STATE_WRITE_RDY, KAL_FALSE, KAL_TRUE);
#endif
   }
   else
   {
#if defined(USB_IP_U3)
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_TX, KAL_FALSE, KAL_FALSE);
#else
      USB_Update_EP0_State(USB_EP0_DRV_STATE_WRITE_RDY, KAL_FALSE, KAL_FALSE);
#endif
   }
}


/* EP0 interrupt handler called by USB_HISR */
static void USB_Endpoint0_Hdlr(void)
{
   kal_bool b_transaction_end;
   kal_bool b_sent_stall;
   kal_uint32 nCount;

   USB_Get_EP0_Status(&b_transaction_end, &b_sent_stall);

   /* Check for SentStall */
   /* SentStall && SetupEnd are impossible to occur together*/
   if (b_sent_stall == KAL_TRUE)
   {
#if defined(USB_IP_U3)
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_CLEAR_SENT_STALL, KAL_FALSE, KAL_FALSE);
#else
      USB_Update_EP0_State(USB_EP0_DRV_STATE_CLEAR_SENT_STALL, KAL_FALSE, KAL_FALSE);
#endif
      gUsbDevice.ep0_state = USB_EP0_IDLE;
   }

   /* Check for SetupEnd */
   if (b_transaction_end == KAL_TRUE)
   {
#if defined(USB_IP_U3)
      USB_U3_Update_EP0_State(USB_EP0_DRV_STATE_TRANSACTION_END, KAL_FALSE, KAL_FALSE);
#else
      USB_Update_EP0_State(USB_EP0_DRV_STATE_TRANSACTION_END, KAL_FALSE, KAL_FALSE);
#endif
      gUsbDevice.ep0_state = USB_EP0_IDLE;
   }

   /* Call relevant routines for endpoint 0 state */
   if (gUsbDevice.ep0_state == USB_EP0_IDLE)
   {
      /* receive command request */
      nCount = USB_EP0_Pkt_Len();
      if (nCount > 0)
      {
         /* idle state handler */
         USB_Endpoint0_Idle();
      }
   }
   else if (gUsbDevice.ep0_state == USB_EP0_RX)
   {
      /* Rx state handler */
      USB_Endpoint0_Rx();
   }
#ifndef USB_IP_U3
   else if (gUsbDevice.ep0_state == USB_EP0_RX_STATUS)
   {
      /* Data stage is RX, status stage is TX*/
      if(gUsbDevice.nDevState == DEVSTATE_SET_ADDRESS)
      {
         USB_SetAddress(gUsbDevice.ep0info.byFAddr, USB_SET_ADDR_STATUS);
         gUsbDevice.nDevState = DEVSTATE_ADDRESS;
      }
      gUsbDevice.ep0_state = USB_EP0_IDLE;

      /* In case next setup followed the previous status very fast and interrupt only happens once*/
      /* receive command request */
      nCount = USB_EP0_Pkt_Len();
      if (nCount > 0)
      {
         /* idle state handler */
         USB_Endpoint0_Idle();
      }
   }
#endif
   /* must use if, not else if, EP0 may enter TX state in previous IDLE state handler */
   if (gUsbDevice.ep0_state == USB_EP0_TX)
   {
      /* Tx state handler */
      USB_Endpoint0_Tx();
   }

   return;
}


#if 1 // obseleted
void USB_Register_EP0_Class_CmdHdlr(kal_uint8 cmd, usb_ep0_cmd_ptr handler)
{
   gUsbDevice.ep0_class_cmd_handler.cmd = cmd;
   gUsbDevice.ep0_class_cmd_handler.ep0_cmd_hdlr = handler;
   gUsbDevice.ep0_class_cmd_handler.b_enable = KAL_TRUE;
}
#endif
