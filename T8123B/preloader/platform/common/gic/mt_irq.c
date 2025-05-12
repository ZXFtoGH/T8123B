/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2021. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */
#include <pal_typedefs.h>
#include <typedefs.h>
#include "inc/mt_irq.h"

struct irq_desc irq_handlers_array[MAX_IRQ_LINE_NUM];

void  dump_handler_table(void)
{
    uint32_t i;

    print("dump handler table:\n");
    for (i = 0 ; i < MAX_IRQ_LINE_NUM ; i++) {
	print ("%s=%d, %s=0x%x, %s=0x%x, %s=%s\n",
		"irqnr", irq_handlers_array[i].irqnr,
		"irq handler", irq_handlers_array[i].irq_num_handler,
		"cookie", irq_handlers_array[i].cookie,
		"description", irq_handlers_array[i].description);
    }
}

extern uint32_t request_irq(uint32_t irq_num,
	uint32_t sensitive_type,
	irq_handler_t handler,
	void *cookie,
	BOOL auto_unmask,
	char *description)
{
    uint32_t i;

    ASSERT(handler != NULL);
    for (i = 0 ; i < MAX_IRQ_LINE_NUM ; i++) {
	if ((irq_handlers_array[i].irq_num_handler != NULL) &&
		(irq_handlers_array[i].irqnr != irq_num))
	    continue;
	irq_handlers_array[i].irqnr = irq_num;
	irq_handlers_array[i].irq_num_handler = handler;
	irq_handlers_array[i].cookie = cookie;
	irq_handlers_array[i].description = description;
	mt_irq_set_sens(irq_num, sensitive_type);
	if (auto_unmask == TRUE)
	    mt_irq_unmask(irq_num);
	break;
    }

    if(i >= MAX_IRQ_LINE_NUM) {
	print("[%s]%s:%d, %s:0x%lx, %s:0x%x, %s=%d, %s=%s\n",
		__func__,
		"irq_num", irq_num,
		"handler", handler,
		"cookie", cookie,
		"auto_unmask", auto_unmask,
		"description", description);
	dump_handler_table();
    }

    ASSERT(i < MAX_IRQ_LINE_NUM);

    return 0;
}

int32_t irq_handler(void)
{
    uint32_t i, irqnr;
    int32_t ret = -1;

    irqnr = mt_irq_get();

    for (i = 0 ; i < MAX_IRQ_LINE_NUM ; i++) {
	if (irq_handlers_array[i].irqnr == irqnr &&
	    irq_handlers_array[i].irq_num_handler) {
	    irq_handlers_array[i].irq_num_handler(irq_handlers_array[i].cookie);
	    mt_irq_ack(irqnr);
	    ret = 0;
	    break;
	}
    }

    return ret;
}
