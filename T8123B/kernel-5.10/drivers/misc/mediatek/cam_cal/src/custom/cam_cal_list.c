// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#include <linux/kernel.h>
#include "cam_cal_list.h"
#include "eeprom_i2c_common_driver.h"
#include "eeprom_i2c_custom_driver.h"
#include "eeprom_i2c_gc08a8main_driver.h"
#include "eeprom_i2c_gc08a8sub_driver.h"
#include "eeprom_i2c_gc08a8maincxt_driver.h"
#include "eeprom_i2c_gc08a8subcxt_driver.h"
#include "kd_imgsensor.h"

#define MAX_EEPROM_SIZE_64K 0x10000
#define MAX_EEPROM_SIZE_32K 0x8000
#define MAX_EEPROM_SIZE_16K 0x4000

struct stCAM_CAL_LIST_STRUCT g_camCalList[] = {
	{GC08A8MAIN_SENSOR_ID, 0x24, gc08a8main_read_region, MAX_EEPROM_SIZE_64K},
	{GC08A8SUB_SENSOR_ID, 0x62, gc08a8sub_read_region, MAX_EEPROM_SIZE_64K},
	{GC08A8MAINCXT_SENSOR_ID, 0x20, gc08a8maincxt_read_region, MAX_EEPROM_SIZE_64K},
	{GC08A8SUBCXT_SENSOR_ID, 0x22, gc08a8subcxt_read_region, MAX_EEPROM_SIZE_64K},
	/*Below is commom sensor */
	{GC5035_SENSOR_ID,  0x7E, Common_read_region},
	/*  ADD before this line */
	{0, 0, 0}       /*end of list */
};

unsigned int cam_cal_get_sensor_list(
	struct stCAM_CAL_LIST_STRUCT **ppCamcalList)
{
	if (ppCamcalList == NULL)
		return 1;

	*ppCamcalList = &g_camCalList[0];
	return 0;
}


