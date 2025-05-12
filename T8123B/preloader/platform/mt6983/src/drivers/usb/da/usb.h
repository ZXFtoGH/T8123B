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


#ifndef USB_H
#define USB_H

#include "usb_custom.h"
#include "usb_drv.h"
#include "usb_comm.h"

/***********************************************
standard command definition
************************************************/

/* Standard Request Codes */
#define USB_GET_STATUS			0x00
#define USB_CLEAR_FEATURE		0x01
#define USB_SET_FEATURE			0x03
#define USB_SET_ADDRESS			0x05
#define USB_GET_DESCRIPTOR		0x06
#define USB_SET_DESCRIPTOR		0x07
#define USB_GET_CONFIGURATION	0x08
#define USB_SET_CONFIGURATION	0x09
#define USB_GET_INTERFACE		0x0A
#define USB_SET_INTERFACE		0x0B
#define USB_SYNCH_FRAME			0x0C

/* Command bit fields */
#define USB_CMD_DATADIR			0x80

/* Request Type Field */
#define USB_CMD_TYPEMASK		0x60
#define USB_CMD_STDREQ			0x00
#define USB_CMD_CLASSREQ		0x20
#define USB_CMD_VENDREQ			0x40
#define USB_CMD_STDDEVIN		0x80
#define USB_CMD_STDDEVOUT		0x00
#define USB_CMD_STDIFIN			0x81
#define USB_CMD_STDIFOUT		0x01
#define USB_CMD_STDEPIN			0x82
#define USB_CMD_STDEPOUT		0x02
#define USB_CMD_CLASSIFIN		0xa1
#define USB_CMD_CLASSIFOUT		0x21

/* Standard command descriptor type*/
#define USB_DEVICE				0x01
#define USB_CONFIG				0x02
#define USB_STRING				0x03
#define USB_INTERFACE			0x04
#define USB_ENDPOINT			0x05
#define USB_DEVICE_QUALIFIER 		0x06
#define USB_OTHER_SPEED			0x07
#define USB_INTERFACE_POWER		0x08
#define USB_OTG_DESC			0x09
#define USB_INTERFACE_ASSOCIATION		0x0b
#ifdef USB_IP_U3
#define USB_BOS					0x0f
#define USB_DEVICE_CAPABILITY   0x10
#define USB_SS_ENDPOINT_COMP	0x30
#endif

#define USB_CMD_DESCMASK		0xFF00
#define USB_CMD_DEVICE			(USB_DEVICE<<8)
#define USB_CMD_CONFIG			(USB_CONFIG<<8)
#define USB_CMD_STRING			(USB_STRING<<8)
#define USB_CMD_INTERFACE		(USB_INTERFACE<<8)
#define USB_CMD_ENDPOINT		(USB_ENDPOINT<<8)
#define USB_CMD_DEVICE_QUALIFIER		(USB_DEVICE_QUALIFIER<<8)
#define USB_CMD_OTHER_SPEED			(USB_OTHER_SPEED<<8)
#define USB_CMD_INTERFACE_POWER		(USB_INTERFACE_POWER<<8)
#ifdef USB_IP_U3
#define USB_CMD_BOS					(USB_BOS<<8)
#endif

/* Standard Device Feature Selectors */
#define USB_FTR_DEVREMWAKE			0x0001
#define USB_FTR_EPHALT				0x0000
#define USB_FTR_B_HNP_ENB			0x0003
#define USB_FTR_A_HNP_SUPPORT		0x0004
#define USB_FTR_A_ALT_HNP_SUPPORT	0x0005

/***********************************************
standard descriptor definition
************************************************/

/*Scriptor length*/
#define USB_DEVDSC_LENGTH			18
#define USB_CFGDSC_LENGTH			9
#define USB_IFDSC_LENGTH			9
//#define USB_MAX_IFDSC_LENGTH		28	/* this will change depend on different class interface */
#define USB_MAX_IFDSC_LENGTH		258	/* this will change depend on different class interface */
/*descriptor requirment*/
/* 28 is for CDC ACM communication class interface */
/* 258 is for video streaming class interface */
#define USB_EPDSC_LENGTH			7
#define USB_OTGDSC_LENGTH			3
#define USB_MAX_CLASS_EPDESC_LENGTH	12	/* this will change depend on different class interface */
/*descriptor requirment*/
/* 12 is for video control interrupt ep */
#define USB_IAD_LENGTH				8
#define USB_DEV_QUA_DSC_LENGTH		10
#define USB_OTHER_CFGDSC_LENGTH			9

/* Endpoint direction */
#define USB_EP_DIR				0x80
#define USB_EP_DIR_IN			0x80
#define USB_EP_DIR_OUT			0x00

/* Endpoint transfer types */
#define USB_EP_TFMASK			0x03
#define USB_EP_CONTROL			0x00
#define USB_EP_ISO				0x01
#define USB_EP_BULK				0x02
#define USB_EP_INTR				0x03

/*****Descriptor Element*****/
/* Define values for Standard Device Descriptor */
/*For Device descriptor*/
#define USB_DEVDSC_PRODUCT_COMPOSITE	0x0004   /* Product ID */
#define USB_DEVDSC_MAXPKTSIZE			0x08     /* Packet Size */
#define USB_DEVDSC_BCDDEVICE			0x0100   /*Release number of Device*/

#ifdef   __USB_MULTI_CHARGE_CURRENT__
#if ( defined(MT6305) || defined(MT6318) )
#define USB_DEVDSC_CONFIGS    			USB_GetChargeCurrentTableSize()
#else
#define USB_DEVDSC_CONFIGS    			1
#endif
#else
#define USB_DEVDSC_CONFIGS    			1  /* Number of possible configurations */
#endif


/*For Configuration descriptor*/
#define USB_CFGDSC_ATTR_NATURE			0x80
#define USB_CFGDSC_ATTR_SELFPOWER		0x40
#define USB_CFGDSC_ATTR_REMOTEWAKEUP	0x20
#define USB_CFGDSC_MAXPOWER				(500/2)

#define USB_OTG_HNP_SUPPORT				0x02
#define USB_OTG_SRP_SUPPORT				0x01

/***********************************************
implement definition
************************************************/

/* define configuration,interface, ep number */
#define USB_MAX_CONFIG				1  /* configuration number */
#define USB_MAX_IAD					1
#define USB_MAX_INTERFACE			3  /* interface number */

#ifdef __ENDPOINT_TWO_TEST__
#define USB_MAX_EP_BULK_IN			2  /* max bulk in ep number */
#define USB_MAX_EP_BULK_OUT		2  /* max bulk in ep number */
#else
#define USB_MAX_EP_BULK_IN			1  /* max bulk in ep number */
#define USB_MAX_EP_BULK_OUT			1  /* max bulk in ep number */
#endif
#define USB_MAX_EP_INTR				1  /* max interrupt ep number */
#define USB_MAX_STRING				10 /* string number */

#define USB_MAX_EP_PER_IF			3 /* 3 is for image class */

/* serial string array size  for that unique Serial number*/
#define USB_SERIAL_STRING_LEN		16

/* define endpoint data parameters */
#define USB_EP_NODATA				-1  /* no more data for endpoint to send */


/***********************************************
standard command structure
************************************************/

/* usb standard command structure */
typedef struct
{
   kal_uint8	bmRequestType;
   kal_uint8	bRequest;
   kal_uint16	wValue;
   kal_uint16	wIndex;
   kal_uint16	wLength;
} Usb_Command;



/***********************************************
standard descriptor structure
************************************************/

/* standard device descriptor */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint16   bcdUSB;
   kal_uint8    bDeviceClass;
   kal_uint8    bDeviceSubClass;
   kal_uint8    bDeviceProtocol;
   kal_uint8    bMaxPacketSize0;
   kal_uint16   idVendor;
   kal_uint16   idProduct;
   kal_uint16   bcdDevice;
   kal_uint8    iManufacturer;
   kal_uint8    iProduct;
   kal_uint8    iSerialNumber;
   kal_uint8    bNumConfigurations;
} Usb_Dev_Dscr;

/* standard configuration descriptor */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint16   wTotalLength;
   kal_uint8    bNumInterfaces;
   kal_uint8    bConfigurationValue;
   kal_uint8    iConfiguration;
   kal_uint8    bmAttributes;
   kal_uint8    bMaxPower;
} Usb_Cfg_Dscr;

/* standard IAD descriptor */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint8    bFirstInterface;
   kal_uint8    bInterfaceCount;
   kal_uint8    bFunctionClass;
   kal_uint8    bFunctionSubClass;
   kal_uint8    bFunctionProtocol;
   kal_uint8    iFunction;
} Usb_IAD_Dscr;

/* standard interface descriptor */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint8    bInterfaceNumber;
   kal_uint8    bAlternateSetting;
   kal_uint8    bNumEndpoints;
   kal_uint8    bInterfaceClass;
   kal_uint8    bInterfaceSubClass;
   kal_uint8    bInterfaceProtocol;
   kal_uint8    iInterface;
} Usb_If_Dscr;

/* standard endpoint descriptor */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint8    bEndpointAddress;
   kal_uint8    bmAttributes;
   kal_uint8    wMaxPacketSize[2];
   kal_uint8    bInterval;
} Usb_Ep_Dscr;


/* standard string descriptor */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint16   wData[1];
}Usb_String_Dscr;

/* standard string descriptor */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint8    bAttribute;
}Usb_Otg_Dscr;

typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint16  bcdUSB;
   kal_uint8    bDeviceClass;
   kal_uint8    bDeviceSubClass;
   kal_uint8    bDeviceProtocol;
   kal_uint8    bMaxPacketSize0;
   kal_uint8    bNumConfigurations;
   kal_uint8    bReserved;
}Usb_Dev_Qual_Dscr;

typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint16   wTotalLength;
   kal_uint8    bNumInterfaces;
   kal_uint8    bConfigurationValue;
   kal_uint8    iConfiguration;
   kal_uint8    bmAttributes;
   kal_uint8    MaxPower;
}Usb_Other_Speed_Cfg_Dscr;


#ifdef USB_IP_U3
#define USB_SS_COM_EPDSC_LENGTH		6
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint8    bMaxBurst;
   kal_uint8    bmAttributes;
   kal_uint16    wBytesPerInterval;
} Usb_SS_Ep_Comp_Dscr;

#define USB_DT_BOS_SIZE         5
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint16   wTotalLength;
   kal_uint8    bNumDeviceCaps;
}Usb_Bos_Dscr;


#define USB_DT_USB_EXT_CAP_SIZE 		7
#define USB_CAP_TYPE_EXT                2
#define USB_LPM_SUPPORT                 (1 << 1)        /* supports LPM */
#define USB_BESL_SUPPORT                (1 << 2)        /* supports BESL */
#define USB_BESL_BASELINE_VALID         (1 << 3)        /* Baseline BESL valid*/
#define USB_BESL_DEEP_VALID             (1 << 4)        /* Deep BESL valid */
#define USB_GET_BESL_BASELINE(p)        (((p) & (0xf << 8)) >> 8)
#define USB_GET_BESL_DEEP(p)            (((p) & (0xf << 12)) >> 12)
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint8   bDevCapabilityType;
   kal_uint32    bmAttributes;



}Usb_Usb_Ext_Cap_Dscr;


#define USB_DT_USB_SS_CAP_SIZE  10
#define USB_SS_CAP_TYPE         3
#define USB_LOW_SPEED_OPERATION         (1)      /* Low speed operation */
#define USB_FULL_SPEED_OPERATION        (1 << 1) /* Full speed operation */
#define USB_HIGH_SPEED_OPERATION        (1 << 2) /* High speed operation */
#define USB_5GBPS_OPERATION             (1 << 3) /* Operation at 5Gbps */
#define USB_DEFAULT_U1_DEV_EXIT_LAT     0x01	/* Less then 1 microsec */
#define USB_DEFAULT_U2_DEV_EXIT_LAT     0x1F4	/* Less then 500 microsec */
typedef struct
{
   kal_uint8    bLength;
   kal_uint8    bDescriptorType;
   kal_uint8    bDevCapabilityType;
   kal_uint8    bmAttributes;
   kal_uint16   wSpeedSupported;



   kal_uint8    bFunctionalitySupport;


   kal_uint8    bU1devExitLat;
   kal_uint16   bU2DevExitLat;
}Usb_Ss_Cap_Dscr;

#endif
/***********************************************
implement enum and structure
************************************************/

/* device states */
typedef enum
{
   DEVSTATE_DEFAULT = 0,
   DEVSTATE_SET_ADDRESS,
   DEVSTATE_ADDRESS,
   DEVSTATE_CONFIG
} USB_DEVICE_STATE;


/* Endpoint 0 state */
typedef enum
{
   USB_EP0_IDLE=0,
   USB_EP0_RX,
   USB_EP0_TX,
   USB_EP0_RX_STATUS
} USB_EP0_STATE;


/* EP0 status */
typedef struct
{
   kal_uint8	byFAddr;	/* new function address */
   kal_int32	nBytesLeft;	/* number of bytes left to send in TX mode */
   void		*pData;		/* pointer to data to transmit/receive */
   kal_int32	nBytesRecv;	/* number of bytes received in RX mode */
} Usb_Ep0_Status;

/* Bulk IN endpoint status */
typedef struct
{
   kal_uint8	byEP;		/* endpoint number */
   void		*pData;		/* pointer to data buffer */
   kal_int32	nBytesLeft;	/* number of bytes left to send */
} Usb_EpBIn_Status;

/* Bulk OUT endpoint status */
typedef struct
{
   kal_uint8	byEP;		/* endpoint number */
   void		*pData;		/* pointer to data buffer */
   kal_int32	nBuffLen;	/* length of data buffer */
   kal_int32	nBytesRecv;	/* number of bytes received */
} Usb_EpBOut_Status;


/* interface create function pointer */
typedef void (*usb_create_if_func_ptr)(void *);
/* interface speed reset function pointer */
typedef void (* usb_speed_if_func_ptr)(kal_bool bset);
/* EP0 rx interrupt handler */
typedef void (*usb_ep0_rx_ptr)(void *);
/* class specific command interrupt handler */
typedef void (*usb_class_specific_handler_ptr)(Usb_Ep0_Status *, Usb_Command*);
/* endpoint interrupt handler */
typedef void (*usb_ep_handler_ptr)(void);
/* EP0 command  interrupt handler */
typedef void (*usb_ep0_cmd_ptr)(Usb_Command *pcmd);

/* endpoint information, including endpoint status */
typedef struct
{
   union
   {
      Usb_Ep_Dscr			stdep;			/* ep descriptor */
      kal_uint8			classep[USB_MAX_CLASS_EPDESC_LENGTH];
   }epdesc;
#ifdef USB_IP_U3
   Usb_SS_Ep_Comp_Dscr		stdep_ss_comp;
#endif

   kal_uint16				epdscr_size;		/* descriptor length */
   usb_ep_handler_ptr		ep_reset;		/* ep reset handler */
   union
   {
      Usb_EpBIn_Status	epin_status;
      Usb_EpBOut_Status	epout_status;
   }ep_status;						/* ep status */
} Usb_Ep_Info;


/* interface information, including endpoint information and interface information*/
//extern struct Usb_Cdc_If_Dscr;
typedef struct
{
   kal_char			*interface_name_ptr;	/* interface name */
   usb_class_specific_handler_ptr	if_class_specific_hdlr;	/* interface specific handler, handle ep0 class specific request */
   kal_uint16			ifdscr_size;		/* descriptor length */
   union
   {
      Usb_If_Dscr		stdif;
      kal_uint8		classif[USB_MAX_IFDSC_LENGTH];
   }ifdscr;
   Usb_Ep_Info			*ep_info[USB_MAX_EP_PER_IF];/* endpoint attach to the interface info */
} Usb_Interface_Info;

/* configuration information */
typedef struct
{
   kal_char			*cfg_name_ptr;		/* config name */
   Usb_Cfg_Dscr		stdcfg;			/* config descriptor */
}Usb_Config_Info;

#ifdef USB_IP_U3
typedef struct
{
   Usb_Bos_Dscr			bos_dscr;
   Usb_Usb_Ext_Cap_Dscr	usb_ext_cap_dscr;
   Usb_Ss_Cap_Dscr			ss_cap_dscr;

}Usb_Bos_Info;

#endif
typedef struct
{
   kal_bool			b_enable;
   kal_uint8			cmd;
   usb_ep0_cmd_ptr	ep0_cmd_hdlr;
}Usb_EPo_Cmd_Hdler;

/* device information, used for usb level */
typedef struct
{
   Usb_Dev_Dscr		devdscr;
   /* Usb_Config_Info		cfg_info[USB_MAX_CONFIG]; */
   /*  the configuration only one for saving memory  */
   Usb_Config_Info		cfg_info;
#ifdef USB_IP_U3
   Usb_Bos_Info 		bos_info;
#endif
   Usb_Interface_Info	if_info[USB_MAX_INTERFACE];
   Usb_Ep_Info			ep_bulkin_info[USB_MAX_EP_BULK_IN];
   Usb_Ep_Info			ep_bulkout_info[USB_MAX_EP_BULK_OUT];
   Usb_Ep_Info			ep_intr_info[USB_MAX_EP_INTR];
   Usb_String_Dscr		*resource_string[USB_MAX_STRING];
   //	Usb_IAD_Dscr		iad_desc[USB_MAX_IAD];

#ifdef USB_HS_ENABLE
   Usb_Dev_Qual_Dscr			dev_qual_dscr;
   Usb_Other_Speed_Cfg_Dscr		other_speed_cfg_dscr;
   USB_TEST_MODE_TYPE			usb_test_type;
   kal_bool				usb_enter_test_mode;
#endif
   kal_uint8 			*conf;				/*just for descriptor transfer*/
   Usb_Command			cmd;				/*USB_COMMAND*/
   USB_DEVICE_TYPE 	device_type;
   USB_DEVICE_STATE	nDevState;			/*DEVSTATE_CONFIG,DEVSTATE_ADDRESS,DEVSTATE_DEFAULT*/
   kal_bool			remoteWk;
   kal_bool			self_powered;
   kal_bool  		usb_comport_boot;		/* a flag to check if usb boot as com port */
   kal_bool			is_configured_now;		/* a flag to check that after reset and before set configuration, the all DMA call back messages will be ignored */
   kal_uint8			config_num;
   kal_uint8			interface_num;
   USB_EP0_STATE 		ep0_state;
   Usb_Ep0_Status    	ep0info;
   usb_ep0_rx_ptr  	ep0_rx_handler;
   Usb_EPo_Cmd_Hdler	ep0_class_cmd_handler; /* class specific ep0 cmd */
   kal_bool 			usb_config_result; 	/* configure may be fail */

   /* used for resource management*/
   kal_uint8			resource_ep_bulkin_number;
   kal_uint8			resource_ep_bulkout_number;
   kal_uint8			resource_ep_intr_number;
   kal_uint8			resource_interface_number;
   kal_uint8			resource_string_number;
   kal_uint8			resource_iad_number;

#ifdef __USB_MULTI_CHARGE_CURRENT__
#if ( defined(MT6305) || defined(MT6318) )
   /* add for multi-configurations*/
   kal_uint8			multi_configuration_value[USB_MAX_CONFIG];
   kal_uint8			multi_Max_Power[USB_MAX_CONFIG];
#endif
#endif
   /* serial string unique for each phone*/
   kal_uint16			serial_string[USB_SERIAL_STRING_LEN];

   /* customizable variables*/
   const USB_DEVICE_PARAM	*device_param;
} Usb_Device;

/*add for setup preloader comport when scan comport*/
typedef struct
{
   kal_uint16 pid;
   kal_uint16 vid;
} Usb_SetUp_Info;


/***********************************************
function and global variable
************************************************/
extern Usb_Device gUsbDevice;
extern Usb_SetUp_Info gUsbSetupInfo;

extern void USB_Init_Device_Status(void);
extern void USB_Release_Device_Status(void);
extern void USB_Register_EP0_RxHdlr(usb_ep0_rx_ptr handler);
extern void USB_Register_EP0_Class_CmdHdlr(kal_uint8 cmd, usb_ep0_cmd_ptr handler);
extern void USB_Init(USB_DEVICE_TYPE type, kal_bool b_enable);
extern void USB_Release(void);

extern void USB_Generate_EP0Data(Usb_Ep0_Status*pep0state, Usb_Command*pcmd, void *data, kal_int32 len);
extern void USB2COM_SetupDL(bool usb_dl_mode, bool enable_hs);
extern void USB2COM_Init(void);
extern void USB2COM_PutByte_Buffer(kal_uint32* addr, kal_uint32 length);
extern kal_bool USB2COM_GetByte_Buffer(kal_uint32* addr, kal_uint32 length);
extern bool COM_Is_USB_Download_Mode(void);
extern void USB_QMU_Init_and_Start(void);


#ifdef  __USB_MULTI_CHARGE_CURRENT__
#if ( defined(MT6305) || defined(MT6318) )
extern const kal_uint16  USB_CHARGE_CURRENT[];
extern const kal_uint8  USB_GetChargeCurrentTableSize(void);
#endif
#endif

#endif /* USB_H */

