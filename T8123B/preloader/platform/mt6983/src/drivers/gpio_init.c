/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

/******************************************************************************
 * gpio_init.c - MT67xx Preloader GPIO Initialization
 *
 * Copyright 2021 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     default GPIO init
 *
 ******************************************************************************/

#include <gpio.h>

#if !defined(FPGA_PLATFORM)
#include <cust_gpio_boot.h>
#endif

#include <platform.h>

/*----------------------------------------------------------------------------*/
#define GPIOTAG "[GPIO] "
#define GPIODBG(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)
#define GPIOERR(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)
#define GPIOVER(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)

#define GPIO_WR32(addr, data)   DRV_WriteReg32(addr,data)
#define GPIO_RD32(addr)         DRV_Reg32(addr)
/*----------------------------------------------------------------------------*/
#ifdef FPGA_PLATFORM
void mt_gpio_set_default(void)
{
	return;
}
#else

#include <gpio_init.h>

void mt_gpio_set_default_chip(void)
{
	unsigned pin = 0;
	for (pin = 0; pin < MT_GPIO_BASE_MAX; pin++) {
		/* set GPIOx_MODE*/
		mt_set_gpio_mode(0x80000000 + pin , gpio_array[pin].mode);

		/* set GPIOx_DIR*/
		if (gpio_array[pin].dir != 0xFF)
			mt_set_gpio_dir(0x80000000 + pin, gpio_array[pin].dir);

		/* set GPIOx_PULL*/
		if (gpio_array[pin].pull != 0xFF)
			mt_set_gpio_pull_select(0x80000000 + pin, gpio_array[pin].pull);

		/* set GPIOx_PULLEN*/
		if (gpio_array[pin].pullen != 0xFF)
			mt_set_gpio_pull_enable(0x80000000 + pin , gpio_array[pin].pullen);

		/* set GPIOx_DATAOUT*/
		if (gpio_array[pin].dataout != 0xFF)
			mt_set_gpio_out(0x80000000 + pin, gpio_array[pin].dataout);

		/* set GPIO0_SMT */
		if (gpio_array[pin].smt != 0xFF)
			mt_set_gpio_smt(0x80000000 + pin, gpio_array[pin].smt);
	}
}

void mt_gpio_set_default(void)
{
	mt_gpio_set_default_chip();

	return;
}

#endif
