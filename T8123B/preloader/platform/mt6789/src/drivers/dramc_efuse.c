/*
 * Copyright (c) 2020 MediaTek Inc.
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
#include <typedefs.h>

#include <sec_devinfo.h>
#include "dramc_efuse.h"


void get_chip_sec_devinfo(EFUSE_CHIP_ID_T *chipid)
{
    chipid->u4segment_code = seclib_get_devinfo_with_index(E_AREA7);
    chipid->u4HRID0 = seclib_get_devinfo_with_index(E_AREA12);
    chipid->u4HRID1 = seclib_get_devinfo_with_index(E_AREA13);
    chipid->u4HRID2 = seclib_get_devinfo_with_index(E_AREA14);
    chipid->u4HRID3 = seclib_get_devinfo_with_index(E_AREA15);

    return;
}
