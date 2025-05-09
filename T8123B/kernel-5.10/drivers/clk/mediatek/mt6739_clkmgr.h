/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/

#ifndef _MT_CLKMGR_6739_H
#define _MT_CLKMGR_6739_H

extern unsigned int mt_get_abist_freq(unsigned int ID);
extern unsigned int mt_get_ckgen_freq(unsigned int ID);
extern unsigned int __clk_get_enable_count(struct clk *clk);
extern struct clk *__clk_lookup(const char *name);
#endif
