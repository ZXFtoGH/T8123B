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
#include "usb_custom.h"
#include "usb_debug_tool.h"
#include "usb_hw.h"
#include "assert.h"
#include "lib/string.h"

static const kal_uint16 USB_Language_String[] =
{
   0x0304,
   0x0409
};

/* interface specific create and init functions */
static Usb_IfCreate_Info usb_ifcreate_tbl[USB_MAX_INTERFACE];
static kal_uint8 usb_ifcreate_number = 0;
/* device code */
static Usb_Device_Code usb_device_code;

#define USB_CONF_SIZE	512
static kal_uint8 usb_conf_desc[USB_CONF_SIZE];

/* static functions */
static void USB_Resource_Reset(void);

/* endpoint start number */
static kal_uint8 usb_intr_number = 0;
static kal_uint8 usb_bulkin_number = 0;
static kal_uint8 usb_bulkout_number = 0;

/* register class specific create functions,  class specific create function should create interface descriptor*/
void USB_Register_CreateFunc(kal_char* if_name, usb_create_if_func_ptr if_create_func, VOID_FUNC if_init_func,
                             usb_speed_if_func_ptr if_speed_func)
{
   if((if_create_func == NULL) || (if_init_func == NULL)||(usb_ifcreate_number>=USB_MAX_INTERFACE))
      EXT_ASSERT(0, usb_ifcreate_number, (kal_uint32)if_create_func, (kal_uint32)if_init_func);

   usb_ifcreate_tbl[usb_ifcreate_number].if_name_ptr = if_name;
   usb_ifcreate_tbl[usb_ifcreate_number].if_create_func = if_create_func;
   usb_ifcreate_tbl[usb_ifcreate_number].if_init_func = if_init_func;

#ifdef __USB_HS_ENABLE__
   usb_ifcreate_tbl[usb_ifcreate_number].if_speed_func = if_speed_func;
#endif
   usb_ifcreate_number++;
}


/* register class specific device and product code*/
void USB_Register_Device_Code(kal_uint8 device_code, kal_uint8 subclass_code,
                              kal_uint8 protocol_code, kal_uint16 product_code)
{
   ASSERT(usb_device_code.b_registerd == KAL_FALSE);
   usb_device_code.device_code = device_code;
   usb_device_code.subclass_code = subclass_code;
   usb_device_code.protocol_code = protocol_code;
   usb_device_code.product_code= product_code;
   usb_device_code.b_registerd= KAL_TRUE;
}


/* deregister class specific create functions, should be called when cable plug out */
void USB_DeRegister_CreateFunc(void)
{
   usb_ifcreate_number = 0;
}


/* deregister class specific device and product code, should be called when cable plug out */
void USB_DeRegister_Device_Code(void)
{
   usb_device_code.b_registerd = KAL_FALSE;
}


/* get interrupt endpoint number and endpoint info pointer */
Usb_Ep_Info* USB_Get_Intr_Ep(kal_uint8 *p_num)
{
   /* interrupt must EP 3*/
   gUsbDevice.resource_ep_intr_number++;
   if(gUsbDevice.resource_ep_intr_number > USB_MAX_EP_INTR)
   {
      EXT_ASSERT(0, gUsbDevice.resource_ep_intr_number, USB_MAX_EP_INTR, 0);
   }

   if (usb_intr_number)
      *p_num = usb_intr_number;
   else
      *p_num = 4;

   return (&gUsbDevice.ep_intr_info[0]);
}


/* get bulk in endpoint number and endpoint info pointer */
Usb_Ep_Info* USB_Get_BulkIn_Ep(kal_uint8 *p_num)
{
#ifdef __ENDPOINT_TWO_TEST__
   gUsbDevice.resource_ep_bulkin_number = 2;
   *p_num = gUsbDevice.resource_ep_bulkin_number;
   return (&gUsbDevice.ep_bulkin_info[gUsbDevice.resource_ep_bulkin_number - 1]);
#else
   gUsbDevice.resource_ep_bulkin_number++;
   if(gUsbDevice.resource_ep_bulkin_number > USB_MAX_EP_BULK_IN)
   {
      EXT_ASSERT(0, gUsbDevice.resource_ep_bulkin_number, USB_MAX_EP_BULK_IN, 0);
   }

   if (usb_bulkin_number)
      *p_num = usb_bulkin_number;
   else
      *p_num = gUsbDevice.resource_ep_bulkin_number;

   return (&gUsbDevice.ep_bulkin_info[gUsbDevice.resource_ep_bulkin_number - 1]);
#endif
}


/* get bulk out endpoint number and endpoint info pointer */
Usb_Ep_Info* USB_Get_BulkOut_Ep(kal_uint8 *p_num)
{
#ifdef __ENDPOINT_TWO_TEST__
   gUsbDevice.resource_ep_bulkout_number = 2;
   *p_num = gUsbDevice.resource_ep_bulkout_number;
   return (&gUsbDevice.ep_bulkout_info[gUsbDevice.resource_ep_bulkout_number - 1]);
#else
   gUsbDevice.resource_ep_bulkout_number++;
   if(gUsbDevice.resource_ep_bulkout_number > USB_MAX_EP_BULK_OUT)
   {
      EXT_ASSERT(0, gUsbDevice.resource_ep_bulkout_number, USB_MAX_EP_BULK_OUT, 0);
   }

   if (usb_bulkout_number)
      *p_num = usb_bulkout_number;
   else
      *p_num = gUsbDevice.resource_ep_bulkout_number;

   return (&gUsbDevice.ep_bulkout_info[gUsbDevice.resource_ep_bulkout_number - 1]);
#endif
}

/* set interrupt endpoint number */
void USB_Set_Intr_Ep(kal_uint8 p_num)
{
   usb_intr_number = p_num;
}

/* set bulk in endpoint number */
void USB_Set_BulkIn_Ep(kal_uint8 p_num)
{
   usb_bulkin_number = p_num;
}

/* set bulk out endpoint number */
void USB_Set_BulkOut_Ep(kal_uint8 p_num)
{
   usb_bulkout_number = p_num;
}

/* get interface number and interface info pointer */
Usb_Interface_Info* USB_Get_Interface_Number(kal_uint8 *p_num)
{
   gUsbDevice.resource_interface_number++;
   if(gUsbDevice.resource_interface_number > USB_MAX_INTERFACE)
   {
      EXT_ASSERT(0, gUsbDevice.resource_interface_number, USB_MAX_INTERFACE, 0);
   }
   *p_num = gUsbDevice.resource_interface_number - 1;
   return (&gUsbDevice.if_info[gUsbDevice.resource_interface_number - 1]);
}

/* get string number and point device recource_string to real string pointer */
kal_uint8 USB_Get_String_Number(void *usb_string)
{
   Usb_String_Dscr *string = (Usb_String_Dscr *)usb_string;
   gUsbDevice.resource_string_number++;

   if(gUsbDevice.resource_string_number <= 0)
   {
      EXT_ASSERT(0, gUsbDevice.resource_string_number, USB_MAX_STRING, 0);
   }

   if(gUsbDevice.resource_string_number > USB_MAX_STRING)
   {
      EXT_ASSERT(0, gUsbDevice.resource_string_number, USB_MAX_STRING, 0);
   }

   gUsbDevice.resource_string[gUsbDevice.resource_string_number - 1] = string;
   return (gUsbDevice.resource_string_number - 1);
}

/* create all descriptors, including device and class specific */

void USB_Software_Create(void)
{
   kal_uint8 index_if,index;
   kal_uint8 index_ep;
   kal_uint16 Conf_index = 0;
   kal_uint16 if_len = 0;

   /* initial resource number */
   USB_Resource_Reset();

   /*   string descriptor    */
   USB_Get_String_Number((void *)USB_Language_String);   /*  index 0 */

   /* check class has registered create function and device code */
   if((usb_ifcreate_number == 0) || (usb_device_code.b_registerd == KAL_FALSE))
      EXT_ASSERT(0, usb_ifcreate_number, usb_device_code.b_registerd, 0);

   /* call the create function for each class */
   for(index_if=0; index_if<usb_ifcreate_number; index_if++)
   {
      if(usb_ifcreate_tbl[index_if].if_create_func == NULL)
         EXT_ASSERT(0, 0, 0, 0);
      usb_ifcreate_tbl[index_if].if_create_func(usb_ifcreate_tbl[index_if].if_name_ptr);

      if(gUsbDevice.if_info[index_if].ifdscr_size == 0)
         EXT_ASSERT(0, 0, 0, 0);
   }

   /* Initialise the Standard Device Descriptor */
   gUsbDevice.devdscr.bLength = USB_DEVDSC_LENGTH;
   gUsbDevice.devdscr.bDescriptorType = USB_DEVICE;
   gUsbDevice.devdscr.bcdUSB = 0x0110; /* USB spec rev is 1.1 */
#ifdef USB_IP_U3
   if(USB_IS_Super_Speed() == KAL_TRUE)
   {
      gUsbDevice.devdscr.bcdUSB = 0x0300;
   }
#endif
   gUsbDevice.devdscr.bDeviceClass = usb_device_code.device_code;
   gUsbDevice.devdscr.bDeviceSubClass = usb_device_code.subclass_code;
   gUsbDevice.devdscr.bDeviceProtocol = usb_device_code.protocol_code;
   gUsbDevice.devdscr.bMaxPacketSize0 = USB_EP0_MAXP; /* USB_EP0_MAXP == 8 or 64*/
#ifdef USB_IP_U3
   if(USB_IS_Super_Speed() == KAL_TRUE)
   {
      gUsbDevice.devdscr.bMaxPacketSize0 = USB_EP0_SS_MAXP_EXP;
   }
#endif


   if(gUsbSetupInfo.pid == 0&& gUsbSetupInfo.vid == 0){
      gUsbDevice.devdscr.idVendor = 0x0e8d;//gUsbDevice.device_param->desc_vendor;
      gUsbDevice.devdscr.idProduct = 0x2001;//usb_device_code.product_code;
   }
   else{
      gUsbDevice.devdscr.idVendor = gUsbSetupInfo.vid;//gUsbDevice.device_param->desc_vendor;
      gUsbDevice.devdscr.idProduct = gUsbSetupInfo.pid;//usb_device_code.product_code;
   }

   gUsbDevice.devdscr.bcdDevice = USB_DEVDSC_BCDDEVICE;
   gUsbDevice.devdscr.iManufacturer = 0x00;//USB_Get_String_Number((void *)gUsbDevice.device_param->manufacture_string);
   gUsbDevice.devdscr.iProduct = 0x00;//USB_Get_String_Number((void *)gUsbDevice.device_param->product_string);
   /* serial string is got during initialization procedure */
   //	gUsbDevice.devdscr.iSerialNumber = USB_Get_String_Number((void *)gUsbDevice.serial_string);
   gUsbDevice.devdscr.iSerialNumber = 0x00;
   gUsbDevice.devdscr.bNumConfigurations = 1;

   /* Initialise the descriptors for main configuration 1 */
   gUsbDevice.cfg_info.cfg_name_ptr = "USB Main Config";
   gUsbDevice.cfg_info.stdcfg.bLength = USB_CFGDSC_LENGTH;
   gUsbDevice.cfg_info.stdcfg.bDescriptorType = USB_CONFIG;
   gUsbDevice.cfg_info.stdcfg.bNumInterfaces = gUsbDevice.resource_interface_number;
   gUsbDevice.cfg_info.stdcfg.bConfigurationValue = 0+1;   /*0+1=1, the value = (cfg index+1), select this config's number*/
   gUsbDevice.cfg_info.stdcfg.iConfiguration = 0;

   gUsbDevice.cfg_info.stdcfg.bmAttributes = USB_CFGDSC_ATTR_NATURE;
   if(gUsbDevice.remoteWk == KAL_TRUE)
      gUsbDevice.cfg_info.stdcfg.bmAttributes |= USB_CFGDSC_ATTR_REMOTEWAKEUP;

   if(gUsbDevice.self_powered == KAL_TRUE)
      gUsbDevice.cfg_info.stdcfg.bmAttributes |= USB_CFGDSC_ATTR_SELFPOWER;

#ifdef USB_IP_U3
   /*BOS DESC*/
   gUsbDevice.bos_info.bos_dscr.bLength = USB_DT_BOS_SIZE;
   gUsbDevice.bos_info.bos_dscr.bDescriptorType = USB_BOS;
   gUsbDevice.bos_info.bos_dscr.wTotalLength = USB_DT_BOS_SIZE +
      USB_DT_USB_EXT_CAP_SIZE +
      USB_DT_USB_SS_CAP_SIZE; 							//le16
   gUsbDevice.bos_info.bos_dscr.bNumDeviceCaps = 2;

   /*USB_EXT DESC*/
   gUsbDevice.bos_info.usb_ext_cap_dscr.bLength = USB_DT_USB_EXT_CAP_SIZE;
   gUsbDevice.bos_info.usb_ext_cap_dscr.bDescriptorType = USB_DEVICE_CAPABILITY;
   gUsbDevice.bos_info.usb_ext_cap_dscr.bDevCapabilityType = USB_CAP_TYPE_EXT;
   gUsbDevice.bos_info.usb_ext_cap_dscr.bmAttributes = USB_LPM_SUPPORT;					// le16

   /*SS_CAP DESC*/
   gUsbDevice.bos_info.ss_cap_dscr.bLength = USB_DT_USB_SS_CAP_SIZE;
   gUsbDevice.bos_info.ss_cap_dscr.bDescriptorType = USB_DEVICE_CAPABILITY;
   gUsbDevice.bos_info.ss_cap_dscr.bDevCapabilityType = USB_SS_CAP_TYPE;
   gUsbDevice.bos_info.ss_cap_dscr.bmAttributes = 0;
   gUsbDevice.bos_info.ss_cap_dscr.wSpeedSupported = (	USB_LOW_SPEED_OPERATION |
      USB_FULL_SPEED_OPERATION |
      USB_HIGH_SPEED_OPERATION |
      USB_5GBPS_OPERATION);				// le16
   gUsbDevice.bos_info.ss_cap_dscr.bFunctionalitySupport = USB_LOW_SPEED_OPERATION;

   /* 0 means not support */
   gUsbDevice.bos_info.ss_cap_dscr.bU1devExitLat = 0;
   gUsbDevice.bos_info.ss_cap_dscr.bU2DevExitLat = 0;					// le16
#endif


   /* configuration length */
   //	for(index_if = 0; index_if < gUsbDevice.resource_iad_number; index_if++)
   //	{
   //		if_len += gUsbDevice.iad_desc[index_if].bLength;
   //	}
   for(index_if = 0; index_if < gUsbDevice.resource_interface_number; index_if++)
   {
      if_len += gUsbDevice.if_info[index_if].ifdscr_size;
      for(index_ep = 0; index_ep<gUsbDevice.if_info[index_if].ifdscr.stdif.bNumEndpoints; index_ep++)
      {
         if_len += gUsbDevice.if_info[index_if].ep_info[index_ep]->epdscr_size;
      }
  	}

   gUsbDevice.cfg_info.stdcfg.wTotalLength = (USB_CFGDSC_LENGTH + if_len);
   // use array instead of get buffer
   ASSERT(gUsbDevice.cfg_info.stdcfg.wTotalLength <= USB_CONF_SIZE);
   gUsbDevice.conf = usb_conf_desc;

#ifdef __USB_HS_ENABLE__
   /* decide EP is high speed or full speed descriptors */
   for(index = 0; index < usb_ifcreate_number; index++)
   {
      if(usb_ifcreate_tbl[index].if_speed_func != NULL)
         usb_ifcreate_tbl[index].if_speed_func(KAL_FALSE);
      else
         ASSERT(0);
   }
#endif

   /* construct configuration descriptor */
   /* memcpy(dst,src,len) */
   memcpy( (kal_uint8 *)(gUsbDevice.conf+Conf_index), (kal_uint8*) &gUsbDevice.cfg_info.stdcfg, USB_CFGDSC_LENGTH );
   Conf_index += USB_CFGDSC_LENGTH;

   //	for(index_if = 0; index_if < gUsbDevice.resource_iad_number; index_if++)
   //	{
   //		memcpy((kal_uint8 *)(gUsbDevice.conf + Conf_index),
   //				(kal_uint8*)&gUsbDevice.iad_desc[index_if], gUsbDevice.iad_desc[index_if].bLength);
   //		Conf_index += gUsbDevice.iad_desc[index_if].bLength;
   //	}

   for(index_if = 0; index_if < gUsbDevice.resource_interface_number; index_if++)
   {
      {
         memcpy( (kal_uint8 *)(gUsbDevice.conf + Conf_index),
            (kal_uint8*)&gUsbDevice.if_info[index_if].ifdscr.stdif, gUsbDevice.if_info[index_if].ifdscr_size );
         Conf_index += gUsbDevice.if_info[index_if].ifdscr_size;

         for(index_ep = 0; index_ep < gUsbDevice.if_info[index_if].ifdscr.stdif.bNumEndpoints; index_ep++)
         {
            memcpy((kal_uint8 *)(gUsbDevice.conf + Conf_index),
               (kal_uint8 *)&(gUsbDevice.if_info[index_if].ep_info[index_ep]->epdesc.stdep),
               gUsbDevice.if_info[index_if].ep_info[index_ep]->epdscr_size);
            Conf_index += gUsbDevice.if_info[index_if].ep_info[index_ep]->epdscr_size;
#ifdef USB_IP_U3
            if(USB_IS_Super_Speed() == KAL_TRUE)
            {
               /*ep company desc here */
               memcpy((kal_uint8 *)(gUsbDevice.conf + Conf_index),
                  (kal_uint8 *)&(gUsbDevice.if_info[index_if].ep_info[index_ep]->stdep_ss_comp),
                  USB_SS_COM_EPDSC_LENGTH);
               Conf_index += USB_SS_COM_EPDSC_LENGTH;
            }
#endif

         }
      }
   }

#ifdef USB_IP_U3
   gUsbDevice.cfg_info.stdcfg.wTotalLength = Conf_index;
   //LOGD("USB_Software_Speed_Init, conf_index : %d\n", Conf_index);
   memcpy( (kal_uint8 *)(gUsbDevice.conf), (kal_uint8*) &gUsbDevice.cfg_info.stdcfg, USB_CFGDSC_LENGTH);
   if(USB_IS_Super_Speed() == KAL_TRUE)
   {
      LOGD("USB speed : SS\n");
   }
#endif


   if(USB_IS_High_Speed() == KAL_TRUE)
   {
      if(0)
         LOGD("USB speed: HS\n");
   }
   else
   {
      if(0)
         LOGD("USB speed: non-HS\n");
   }

   ASSERT(Conf_index == gUsbDevice.cfg_info.stdcfg.wTotalLength);
}


void USB_Software_Speed_Init(kal_bool b_other_speed)
{
   kal_uint32  index_if;
   kal_uint32  index_ep;
   kal_uint32  Conf_index;

   kal_uint32  index;
   /* decide EP is high speed or full speed descriptors */
   for(index = 0; index < usb_ifcreate_number && index < USB_MAX_INTERFACE; index++)
   {
      if(usb_ifcreate_tbl[index].if_speed_func != NULL)
         usb_ifcreate_tbl[index].if_speed_func(b_other_speed);
      else
         ASSERT(0);
   }


   /* construct configuration descriptor */
   /* memcpy(dst,src,len) */
   memcpy( (kal_uint8 *)(gUsbDevice.conf), (kal_uint8*) &gUsbDevice.cfg_info.stdcfg, USB_CFGDSC_LENGTH);
   Conf_index = USB_CFGDSC_LENGTH;

   for(index_if = 0; index_if < gUsbDevice.resource_interface_number && index_if < USB_MAX_INTERFACE; index_if++)
   {
      {
         memcpy( (kal_uint8 *)(gUsbDevice.conf + Conf_index),
            (kal_uint8*)&gUsbDevice.if_info[index_if].ifdscr.stdif, gUsbDevice.if_info[index_if].ifdscr_size );
         Conf_index += gUsbDevice.if_info[index_if].ifdscr_size;

         for(index_ep = 0; index_ep < gUsbDevice.if_info[index_if].ifdscr.stdif.bNumEndpoints && index_ep < USB_MAX_EP_PER_IF; index_ep++)
         {
            memcpy((kal_uint8 *)(gUsbDevice.conf + Conf_index),
               (kal_uint8 *)&(gUsbDevice.if_info[index_if].ep_info[index_ep]->epdesc.stdep),
               gUsbDevice.if_info[index_if].ep_info[index_ep]->epdscr_size);
            Conf_index += gUsbDevice.if_info[index_if].ep_info[index_ep]->epdscr_size;
#ifdef USB_IP_U3
            if(USB_IS_Super_Speed() == KAL_TRUE)
            {
               /*ep company desc here */
               memcpy((kal_uint8 *)(gUsbDevice.conf + Conf_index),
                  (kal_uint8 *)&(gUsbDevice.if_info[index_if].ep_info[index_ep]->stdep_ss_comp),
                  USB_SS_COM_EPDSC_LENGTH);
               Conf_index += USB_SS_COM_EPDSC_LENGTH;
            }
#endif
         }
      }
   }

#ifdef USB_IP_U3
   gUsbDevice.cfg_info.stdcfg.wTotalLength = Conf_index;		//04404_test
   //LOGD("USB_Software_Speed_Init, conf_index : %d\n", Conf_index);		//04404_test
   memcpy( (kal_uint8 *)(gUsbDevice.conf), (kal_uint8*) &gUsbDevice.cfg_info.stdcfg, USB_CFGDSC_LENGTH);
#endif

   ASSERT(Conf_index == gUsbDevice.cfg_info.stdcfg.wTotalLength);

}

/* init EP of each interface (class specific) */
void USB_Software_Init(void)
{
   kal_uint8 index;

   /* check class has registered init function*/
   ASSERT(usb_ifcreate_number !=0);

   for(index=0; index<usb_ifcreate_number && index < USB_MAX_INTERFACE;index++)
   {
      if(usb_ifcreate_tbl[index].if_init_func != NULL)
         usb_ifcreate_tbl[index].if_init_func();
      else
         ASSERT(0);
   }
}


/* reset the resource maintain in this file */
static void USB_Resource_Reset(void)
{
   gUsbDevice.resource_ep_bulkin_number = 0;
   gUsbDevice.resource_ep_bulkout_number = 0;
   gUsbDevice.resource_ep_intr_number = 0;
   gUsbDevice.resource_interface_number = 0;
   //gUsbDevice.resource_config_number = 0;
   gUsbDevice.resource_string_number = 0;
   gUsbDevice.resource_iad_number = 0;
}


#if 1 // obseleted

/* get string number and point device recource_string to real string pointer */
kal_uint16 USB_Get_VID(void)
{
   kal_uint16 value = 0;

   //	value = Efuse_Vid(); //*EFUSE_USB_PID

   if (value == 0)
      value = 0x0e8d; //MTK number

   return value;
}
/* get string number and point device recource_string to real string pointer */
kal_uint16 USB_Get_PID(void)
{
   kal_uint16 value = 0;

   //	value = Efuse_Pid(); //*EFUSE_USB_PID

   if (value == 0)
      value = 0x0003; //MTK number

   return value;
}
#endif
