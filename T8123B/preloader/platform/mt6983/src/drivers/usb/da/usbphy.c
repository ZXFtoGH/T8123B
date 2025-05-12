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
#include "usb_hw.h"
#include <inc/register_act_util.h>

#define __raw_readb(reg)		(*(volatile unsigned char * const)(reg))
#define __raw_writeb(val, reg)		(*(volatile unsigned char * const)(reg)) = (val)
#define USBPHY_READ8(offset)		__raw_readb(USB_PHY_BASE+offset)
#define USBPHY_WRITE8(offset, value)	__raw_writeb(value, USB_PHY_BASE+offset)
#define USBPHY_SET8(offset, mask)	USBPHY_WRITE8(offset, USBPHY_READ8(offset) | mask)
#define USBPHY_CLR8(offset, mask)	USBPHY_WRITE8(offset, USBPHY_READ8(offset) & ~mask)

void
usb_phy_poweron (void)
{
   /* to be implemented */
}

void
usb_phy_savecurrent (void)
{
   /* to be implemented */
}

void
usb_phy_recover (void)
{
   /* to be implemented */
}

void
usb_phy_vusb10_on (void)
{
   /* If some errors occur in preloader stage, it will call save_current function to turn off the following register.
   And flash tool will jump back from preloader to brom -> DA, DA will get SSUSB_U3_MAC_RST_B_STS error.
   So we need to turn it on. ( Because its default value is 1, even when we don't encounter preloader errors,
   the following setting is always correct. )*/
}
