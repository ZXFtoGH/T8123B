/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2022 MediaTek Inc.
 * Author: Kevin Huang <shang-ming.huang@mediatek.com>
 */

#ifndef _ACCDET_H_
#define _ACCDET_H_

//extern int hotplug_status;
//extern wait_queue_head_t hotplug_waitq;

extern void audio_en_hp(bool en);
extern void typec_accdet_irq_handle(int insert);

#endif
