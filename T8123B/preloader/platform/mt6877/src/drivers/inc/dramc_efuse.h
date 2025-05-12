/*
 * Copyright (c) 2020 MediaTek Inc.
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */

#ifndef __DRAMC_EFUSE_H__
#define __DRAMC_EFUSE_H__

typedef struct _EFUSE_CHIP_ID_T
{
    unsigned int u4segment_code;
    unsigned int u4HRID0;
    unsigned int u4HRID1;
    unsigned int u4HRID2;
    unsigned int u4HRID3;
} EFUSE_CHIP_ID_T;

void get_chip_sec_devinfo(EFUSE_CHIP_ID_T *chipid);
#endif
