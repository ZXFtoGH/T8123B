#ifndef USB_TYPE_H
#define USB_TYPE_H

#include <sys/types.h>

/* define usb device type*/
typedef enum
{
   USB_UNKOWN=0,
   USB_CDC_ACM,
   USB_MASS_STORAGE,
   USB_VIDEO,
   USB_COMPOSITE,
   USB_STOP_MS,
   USB_CDC_ACM_IRDBG,
   USB_IMAGE,
   USB_MAX_DEVICE_TYPE
} USB_DEVICE_TYPE;

/* usb standard command structure */
typedef struct
{
   kal_uint8	bmRequestType;
   kal_uint8	bRequest;
   kal_uint16	wValue;
   kal_uint16	wIndex;
   kal_uint16	wLength;
} Usb_Command;

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

typedef struct
{
   kal_uint16 pid;
   kal_uint16 vid;
} Usb_SetUp_Info;

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
extern bool USB2COM_GetByte_Buffer(kal_uint32* addr, kal_uint32 length);
extern bool COM_Is_USB_Download_Mode(void);
extern void USB_QMU_Init_and_Start(void);

#endif /* USB_H */
