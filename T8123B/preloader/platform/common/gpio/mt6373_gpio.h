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

#ifndef _MT6373_GPIO_H_
#define _MT6373_GPIO_H_

#include <typedefs.h>
#include <mt6373_pmic.h>

#include "gpio_ext.h"

#define MT6373_DRV_MASK         0xf
#define MT6373_MODE_MASK        0xf
#define MT6373_MIN_GPIO_PIN     1
#define MT6373_MAX_GPIO_PIN     13

#if (SECOND_PMIC_CHIP == 0x6373)
static u16 MODE_addr[] = {
    /* 1 */ 0xc4,
    /* 2 */ 0xc7,
    /* 3 */ 0xc7,
    /* 4 */ 0xca,
    /* 5 */ 0xca,
    /* 6 */ 0xcd,
    /* 7 */ 0xcd,
    /* 8 */ 0xd0,
    /* 9 */ 0xd0,
    /* 10 */ 0xd3,
    /* 11 */ 0xd3,
    /* 12 */ 0xd6,
    /* 13 */ 0xd6,
};

static s8 MODE_offset[] = {
    /* 1 */ 3,
    /* 2 */ 0,
    /* 3 */ 3,
    /* 4 */ 0,
    /* 5 */ 3,
    /* 6 */ 0,
    /* 7 */ 3,
    /* 8 */ 0,
    /* 9 */ 3,
    /* 10 */ 0,
    /* 11 */ 3,
    /* 12 */ 0,
    /* 13 */ 3,
};

static u16 DIR_addr[] = {
    /* 1 */ 0x88,
    /* 2 */ 0x8b,
    /* 3 */ 0x8b,
    /* 4 */ 0x8b,
    /* 5 */ 0x8b,
    /* 6 */ 0x8b,
    /* 7 */ 0x8b,
    /* 8 */ 0x8b,
    /* 9 */ 0x8b,
    /* 10 */ 0x8e,
    /* 11 */ 0x8e,
    /* 12 */ 0x8e,
    /* 13 */ 0x8e,
};

static s8 DIR_offset[] = {
    /* 1 */ 7,
    /* 2 */ 0,
    /* 3 */ 1,
    /* 4 */ 2,
    /* 5 */ 3,
    /* 6 */ 4,
    /* 7 */ 5,
    /* 8 */ 6,
    /* 9 */ 7,
    /* 10 */ 0,
    /* 11 */ 1,
    /* 12 */ 2,
    /* 13 */ 3,
};

static u16 DO_addr[] = {
    /* 1 */ 0xac,
    /* 2 */ 0xaf,
    /* 3 */ 0xaf,
    /* 4 */ 0xaf,
    /* 5 */ 0xaf,
    /* 6 */ 0xaf,
    /* 7 */ 0xaf,
    /* 8 */ 0xaf,
    /* 9 */ 0xaf,
    /* 10 */ 0xb2,
    /* 11 */ 0xb2,
    /* 12 */ 0xb2,
    /* 13 */ 0xb2,
};

static s8 DO_offset[] = {
    /* 1 */ 7,
    /* 2 */ 0,
    /* 3 */ 1,
    /* 4 */ 2,
    /* 5 */ 3,
    /* 6 */ 4,
    /* 7 */ 5,
    /* 8 */ 6,
    /* 9 */ 7,
    /* 10 */ 0,
    /* 11 */ 1,
    /* 12 */ 2,
    /* 13 */ 3,
};

static u16 DI_addr[] = {
    /* 1 */ 0xb5,
    /* 2 */ 0xb6,
    /* 3 */ 0xb6,
    /* 4 */ 0xb6,
    /* 5 */ 0xb6,
    /* 6 */ 0xb6,
    /* 7 */ 0xb6,
    /* 8 */ 0xb6,
    /* 9 */ 0xb6,
    /* 10 */ 0xb7,
    /* 11 */ 0xb7,
    /* 12 */ 0xb7,
    /* 13 */ 0xb7,
};

static s8 DI_offset[] = {
    /* 1 */ 7,
    /* 2 */ 0,
    /* 3 */ 1,
    /* 4 */ 2,
    /* 5 */ 3,
    /* 6 */ 4,
    /* 7 */ 5,
    /* 8 */ 6,
    /* 9 */ 7,
    /* 10 */ 0,
    /* 11 */ 1,
    /* 12 */ 2,
    /* 13 */ 3,
};

static u16 SMT_addr[] = {
    /* 1 */ 0x22,
    /* 2 */ 0x23,
    /* 3 */ 0x23,
    /* 4 */ 0x23,
    /* 5 */ 0x23,
    /* 6 */ 0x23,
    /* 7 */ 0x23,
    /* 8 */ 0x23,
    /* 9 */ 0x23,
    /* 10 */ 0x24,
    /* 11 */ 0x24,
    /* 12 */ 0x24,
    /* 13 */ 0x24,
};

static s8 SMT_offset[] = {
    /* 1 */ 7,
    /* 2 */ 0,
    /* 3 */ 1,
    /* 4 */ 2,
    /* 5 */ 3,
    /* 6 */ 4,
    /* 7 */ 5,
    /* 8 */ 6,
    /* 9 */ 7,
    /* 10 */ 0,
    /* 11 */ 1,
    /* 12 */ 2,
    /* 13 */ 3,
};

static u16 DRV_addr[] = {
    /* 1 */ 0x29,
    /* 2 */ 0x2a,
    /* 3 */ 0x2a,
    /* 4 */ 0x2b,
    /* 5 */ 0x2b,
    /* 6 */ 0x2c,
    /* 7 */ 0x2c,
    /* 8 */ 0x2d,
    /* 9 */ 0x2d,
    /* 10 */ 0x2e,
    /* 11 */ 0x2e,
    /* 12 */ 0x2f,
    /* 13 */ 0x2f,
};

static s8 DRV_offset[] = {
    /* 1 */ 4,
    /* 2 */ 0,
    /* 3 */ 4,
    /* 4 */ 0,
    /* 5 */ 4,
    /* 6 */ 0,
    /* 7 */ 4,
    /* 8 */ 0,
    /* 9 */ 4,
    /* 10 */ 0,
    /* 11 */ 4,
    /* 12 */ 0,
    /* 13 */ 4,
};

static u16 PULLEN_addr[] = {
    /* 1 */ 0x91,
    /* 2 */ 0x94,
    /* 3 */ 0x94,
    /* 4 */ 0x94,
    /* 5 */ 0x94,
    /* 6 */ 0x94,
    /* 7 */ 0x94,
    /* 8 */ 0x94,
    /* 9 */ 0x94,
    /* 10 */ 0x97,
    /* 11 */ 0x97,
    /* 12 */ 0x97,
    /* 13 */ 0x97,
};

static s8 PULLEN_offset[] = {
    /* 1 */ 7,
    /* 2 */ 0,
    /* 3 */ 1,
    /* 4 */ 2,
    /* 5 */ 3,
    /* 6 */ 4,
    /* 7 */ 5,
    /* 8 */ 6,
    /* 9 */ 7,
    /* 10 */ 0,
    /* 11 */ 1,
    /* 12 */ 2,
    /* 13 */ 3,
};

static u16 PULLSEL_addr[] = {
    /* 1 */ 0x9a,
    /* 2 */ 0x9d,
    /* 3 */ 0x9d,
    /* 4 */ 0x9d,
    /* 5 */ 0x9d,
    /* 6 */ 0x9d,
    /* 7 */ 0x9d,
    /* 8 */ 0x9d,
    /* 9 */ 0x9d,
    /* 10 */ 0xa0,
    /* 11 */ 0xa0,
    /* 12 */ 0xa0,
    /* 13 */ 0xa0,
};

static s8 PULLSEL_offset[] = {
    /* 1 */ 7,
    /* 2 */ 0,
    /* 3 */ 1,
    /* 4 */ 2,
    /* 5 */ 3,
    /* 6 */ 4,
    /* 7 */ 5,
    /* 8 */ 6,
    /* 9 */ 7,
    /* 10 */ 0,
    /* 11 */ 1,
    /* 12 */ 2,
    /* 13 */ 3,
};

const struct gpio_attr_addr mt6373_attr_addr[GPIO_ATTR_MAX] = {
    [GPIO_ATTR_MODE] = { .addr = MODE_addr, .offset = MODE_offset, },
    [GPIO_ATTR_DIR] = { .addr = DIR_addr, .offset = DIR_offset, },
    [GPIO_ATTR_DI] = { .addr = DI_addr, .offset = DI_offset, },
    [GPIO_ATTR_DO] = { .addr = DO_addr, .offset = DO_offset, },
    [GPIO_ATTR_SMT] = { .addr = SMT_addr, .offset = SMT_offset, },
    [GPIO_ATTR_DRV] = { .addr = DRV_addr, .offset = DRV_offset, },
    [GPIO_ATTR_PULLSEL] = { .addr = PULLSEL_addr, .offset = PULLSEL_offset, },
    [GPIO_ATTR_PULLEN] = { .addr = PULLEN_addr, .offset = PULLEN_offset, },
};

struct mt_gpio_ext_dev mt6373_dev = {
    .attr_addr = mt6373_attr_addr,
    .min_pin_no = MT6373_MIN_GPIO_PIN,
    .max_pin_no = MT6373_MAX_GPIO_PIN,
    .drv_mask = MT6373_DRV_MASK,
    .mode_mask = MT6373_MODE_MASK,
    .read_ops = second_pmic_read_interface,
    .write_ops = second_pmic_config_interface,
};

struct mt_gpio_ext_dev *mt_gpio_get_dev_mt6373(void)
{
    return &mt6373_dev;
}

#else
struct mt_gpio_ext_dev *mt_gpio_get_dev_mt6373(void)
{
    return NULL;
}

#endif
#endif

