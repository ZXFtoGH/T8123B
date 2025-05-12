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

#include <typedefs.h>
#include <gpio.h>
#include <gpio_ext_api.h>

#include "gpio_ext.h"
#include "mt6373_gpio.h"

/*---------------------------------------------------------------------------*/
struct mt_gpio_ext_dev *mt_gpio_ext_get_dev(int dev_id)
{
    if (dev_id == MT_GPIO_EXT_MT6373)
        return mt_gpio_get_dev_mt6373();

    return NULL;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_set_drv(int dev_id, u32 pin, u32 drv)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_DRV];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_DRV_UNSUPPORTED;

    if (drv > gpio_dev->drv_mask)
        return -ERINVAL;

    ret = (S32)gpio_dev->write_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                (u8)drv, gpio_dev->drv_mask,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    return ret;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_get_drv(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_DRV];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_DRV_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, gpio_dev->drv_mask,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_set_dir(int dev_id, u32 pin, u32 dir)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = (u8)!!dir;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_DIR];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_DIR_UNSUPPORTED;

    ret = (S32)gpio_dev->write_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    return ret;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_get_dir(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_DIR];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_DIR_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_set_out(int dev_id, u32 pin, u32 output)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = (u8)!!output;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_DO];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_OUT_UNSUPPORTED;

    ret = (S32)gpio_dev->write_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    return ret;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_get_out(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_DO];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_OUT_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_get_in(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_DI];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_IN_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_set_mode(int dev_id, u32 pin, u32 mode)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_MODE];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_MODE_UNSUPPORTED;

    if (mode > gpio_dev->mode_mask)
        return -ERINVAL;

    ret = (S32)gpio_dev->write_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                (u8)mode, gpio_dev->mode_mask,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    return ret;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_get_mode(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_MODE];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_MODE_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, gpio_dev->mode_mask,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_set_smt(int dev_id, u32 pin, u32 enable)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = (u8)!!enable;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_SMT];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_SMT_UNSUPPORTED;

    ret = (S32)gpio_dev->write_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    return ret;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_get_smt(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_SMT];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_SMT_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_set_pull_enable(int dev_id, u32 pin, u32 en)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = (u8)!!en;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_PULLEN];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_PULL_EN_UNSUPPORTED;

    ret = (S32)gpio_dev->write_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    return ret;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_get_pull_enable(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_PULLEN];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_PULL_EN_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
/*---------------------------------------------------------------------------*/
s32 mt_gpio_ext_set_pull_select(int dev_id, u32 pin, u32 sel)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = (u8)!!sel;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_PULLSEL];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_PULL_UNSUPPORTED;

    ret = (S32)gpio_dev->write_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    return ret;
}
/*---------------------------------------------------------------------------*/
/* get pull-up or pull-down, regardless of resistor value */
s32 mt_gpio_ext_get_pull_select(int dev_id, u32 pin)
{
    const struct gpio_attr_addr *attr_addr;
    struct mt_gpio_ext_dev *gpio_dev;
    u8 val = 0;
    s32 ret;

    gpio_dev = mt_gpio_ext_get_dev(dev_id);

    if ((pin < gpio_dev->min_pin_no) || (pin > gpio_dev->max_pin_no))
        return -ERINVAL;

    attr_addr = &gpio_dev->attr_addr[GPIO_ATTR_PULLSEL];

    if (attr_addr->offset[pin - gpio_dev->min_pin_no] == (s8)-1)
        return GPIO_PULL_UNSUPPORTED;

    ret = (S32)gpio_dev->read_ops(
                attr_addr->addr[pin - gpio_dev->min_pin_no],
                &val, 0x1,
                attr_addr->offset[pin - gpio_dev->min_pin_no]);

    if (ret < 0)
        return ret;

    return (s32)val;
}
