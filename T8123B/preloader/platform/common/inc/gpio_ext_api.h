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

#ifndef _MT_GPIO_EXT_API_H_
#define _MT_GPIO_EXT_API_H_

#include <typedefs.h>
#include <gpio.h>

#ifdef FPGA_PLATFORM
S32 mt_gpio_ext_set_dir(int dev_id, u32 pin, u32 dir)             {return RSUCCESS;}
S32 mt_gpio_ext_get_dir(int dev_id, u32 pin)                      {return GPIO_DIR_UNSUPPORTED;}
S32 mt_gpio_ext_set_drv(int dev_id, u32 pin, u32 drv)             {return RSUCCESS;}
S32 mt_gpio_ext_get_drv(int dev_id, u32 pin)                      {return GPIO_DIR_UNSUPPORTED;}
S32 mt_gpio_ext_set_pull_enable(int dev_id, u32 pin, u32 en)      {return RSUCCESS;}
S32 mt_gpio_ext_get_pull_enable(int dev_id, u32 pin)              {return GPIO_PULL_EN_UNSUPPORTED;}
S32 mt_gpio_ext_set_pull_select(int dev_id, u32 pin, u32 sel)     {return RSUCCESS;}
S32 mt_gpio_ext_get_pull_select(int dev_id, u32 pin)              {return GPIO_PULL_UNSUPPORTED;}
S32 mt_gpio_ext_set_smt(int dev_id, u32 pin, u32 enable)          {return RSUCCESS;}
S32 mt_gpio_ext_get_smt(int dev_id, u32 pin)                      {return GPIO_SMT_UNSUPPORTED;}
S32 mt_gpio_ext_set_out(int dev_id, u32 pin, u32 output)          {return RSUCCESS;}
S32 mt_gpio_ext_get_out(int dev_id, u32 pin)                      {return GPIO_OUT_UNSUPPORTED;}
S32 mt_gpio_ext_get_in(int dev_id, u32 pin)                       {return GPIO_IN_UNSUPPORTED;}
S32 mt_gpio_ext_set_mode(int dev_id, u32 pin, u32 mode)           {return RSUCCESS;}
S32 mt_gpio_ext_get_mode(int dev_id, u32 pin)                     {return GPIO_MODE_UNSUPPORTED;}
#endif

#if !defined(FPGA_PLATFORM) && !defined(SECOND_PMIC_CHIP)
S32 mt_gpio_ext_set_dir(int dev_id, u32 pin, u32 dir)             {return GPIO_DIR_UNSUPPORTED;}
S32 mt_gpio_ext_get_dir(int dev_id, u32 pin)                      {return GPIO_DIR_UNSUPPORTED;}
S32 mt_gpio_ext_set_drv(int dev_id, u32 pin, u32 drv)             {return GPIO_DIR_UNSUPPORTED;}
S32 mt_gpio_ext_get_drv(int dev_id, u32 pin)                      {return GPIO_DIR_UNSUPPORTED;}
S32 mt_gpio_ext_set_pull_enable(int dev_id, u32 pin, u32 en)      {return GPIO_PULL_EN_UNSUPPORTED;}
S32 mt_gpio_ext_get_pull_enable(int dev_id, u32 pin)              {return GPIO_PULL_EN_UNSUPPORTED;}
S32 mt_gpio_ext_set_pull_select(int dev_id, u32 pin, u32 sel)     {return GPIO_PULL_UNSUPPORTED;}
S32 mt_gpio_ext_get_pull_select(int dev_id, u32 pin)              {return GPIO_PULL_UNSUPPORTED;}
S32 mt_gpio_ext_set_smt(int dev_id, u32 pin, u32 enable)          {return GPIO_SMT_UNSUPPORTED;}
S32 mt_gpio_ext_get_smt(int dev_id, u32 pin)                      {return GPIO_SMT_UNSUPPORTED;}
S32 mt_gpio_ext_set_out(int dev_id, u32 pin, u32 output)          {return GPIO_OUT_UNSUPPORTED;}
S32 mt_gpio_ext_get_out(int dev_id, u32 pin)                      {return GPIO_OUT_UNSUPPORTED;}
S32 mt_gpio_ext_get_in(int dev_id, u32 pin)                       {return GPIO_IN_UNSUPPORTED;}
S32 mt_gpio_ext_set_mode(int dev_id, u32 pin, u32 mode)           {return GPIO_MODE_UNSUPPORTED;}
S32 mt_gpio_ext_get_mode(int dev_id, u32 pin)                     {return GPIO_MODE_UNSUPPORTED;}
#endif

#if !defined(FPGA_PLATFORM) && defined(SECOND_PMIC_CHIP)
enum {
    MT_GPIO_EXT_MT6373,
    MT_GPIO_EXT_MAX,
};

/*direction*/
S32 mt_gpio_ext_set_dir(int dev_id, u32 pin, u32 dir);
S32 mt_gpio_ext_get_dir(int dev_id, u32 pin);

/*driving*/
S32 mt_gpio_ext_set_drv(int dev_id, u32 pin, u32 drv);
S32 mt_gpio_ext_get_drv(int dev_id, u32 pin);

/*pull enable*/
S32 mt_gpio_ext_set_pull_enable(int dev_id, u32 pin, u32 en);
S32 mt_gpio_ext_get_pull_enable(int dev_id, u32 pin);

/*pull select*/
S32 mt_gpio_ext_set_pull_select(int dev_id, u32 pin, u32 sel);
S32 mt_gpio_ext_get_pull_select(int dev_id, u32 pin);

/*schmitt trigger*/
S32 mt_gpio_ext_set_smt(int dev_id, u32 pin, u32 enable);
S32 mt_gpio_ext_get_smt(int dev_id, u32 pin);

/*input/output*/
S32 mt_gpio_ext_set_out(int dev_id, u32 pin, u32 output);
S32 mt_gpio_ext_get_out(int dev_id, u32 pin);
S32 mt_gpio_ext_get_in(int dev_id, u32 pin);

/*mode control*/
S32 mt_gpio_ext_set_mode(int dev_id, u32 pin, u32 mode);
S32 mt_gpio_ext_get_mode(int dev_id, u32 pin);

#endif

#endif
