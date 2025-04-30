// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#include "kd_imgsensor.h"
#include "imgsensor_sensor_list.h"

/* Add Sensor Init function here
 * Note:
 * 1. Add by the resolution from ""large to small"", due to large sensor
 *    will be possible to be main sensor.
 *    This can avoid I2C error during searching sensor.
 * 2. This file should be the same as
 *    mediatek\custom\common\hal\imgsensor\src\sensorlist.cpp
 */
struct IMGSENSOR_SENSOR_LIST
	gimgsensor_sensor_list[MAX_NUM_OF_SUPPORT_SENSOR] = {

/* emdoor start*/
#if defined(GC08A8MAIN_MIPI_RAW)
	{GC08A8MAIN_SENSOR_ID, SENSOR_DRVNAME_GC08A8MAIN_MIPI_RAW, GC08A8MAIN_MIPI_RAW_SensorInit},
#endif

#if defined(GC08A8SUB_MIPI_RAW)
	{GC08A8SUB_SENSOR_ID, SENSOR_DRVNAME_GC08A8SUB_MIPI_RAW, GC08A8SUB_MIPI_RAW_SensorInit},
#endif

#if defined(GC08A8MAINCXT_MIPI_RAW)
	{GC08A8MAINCXT_SENSOR_ID, SENSOR_DRVNAME_GC08A8MAINCXT_MIPI_RAW, GC08A8MAINCXT_MIPI_RAW_SensorInit},
#endif

#if defined(GC08A8SUBCXT_MIPI_RAW)
	{GC08A8SUBCXT_SENSOR_ID, SENSOR_DRVNAME_GC08A8SUBCXT_MIPI_RAW, GC08A8SUBCXT_MIPI_RAW_SensorInit},
#endif

#if defined(OV8856MAIN_MIPI_RAW)
{OV8856MAIN_SENSOR_ID, SENSOR_DRVNAME_OV8856MAIN_MIPI_RAW, OV8856MAIN_MIPI_RAW_SensorInit},
#endif

#if defined(OV8856SUB_MIPI_RAW)
{OV8856SUB_SENSOR_ID, SENSOR_DRVNAME_OV8856SUB_MIPI_RAW, OV8856SUB_MIPI_RAW_SensorInit},
#endif

#if defined(HI846_MIPI_RAW)
{HI846_SENSOR_ID, SENSOR_DRVNAME_HI846_MIPI_RAW, HI846_MIPI_RAW_SensorInit},
#endif

#if defined(GC08A3MAIN_MIPI_RAW)
{GC08A3MAIN_SENSOR_ID, SENSOR_DRVNAME_GC08A3MAIN_MIPI_RAW, GC08A3MAIN_MIPI_RAW_SensorInit},
#endif

#if defined(OV13850MAIN_MIPI_RAW)
{OV13850MAIN_SENSOR_ID, SENSOR_DRVNAME_OV13850MAIN_MIPI_RAW, OV13850MAIN_MIPI_RAW_SensorInit},
#endif

#if defined(GC13A0MAIN_MIPI_RAW)
{GC13A0MAIN_SENSOR_ID, SENSOR_DRVNAME_GC13A0MAIN_MIPI_RAW, GC13A0MAIN_MIPI_RAW_SensorInit},
#endif

#if defined(HI846FRONT_MIPI_RAW)
{HI846FRONT_SENSOR_ID, SENSOR_DRVNAME_HI846FRONT_MIPI_RAW, HI846_F_MIPI_RAW_SensorInit},
#endif

#if defined(GC08A3FRONT_MIPI_RAW)
{GC08A3FRONT_SENSOR_ID, SENSOR_DRVNAME_GC08A3FRONT_MIPI_RAW, GC08A3FRONT_MIPI_RAW_SensorInit},
#endif

#if defined(GC05A2FRONT_MIPI_RAW)
{GC05A2FRONT_SENSOR_ID, SENSOR_DRVNAME_GC05A2FRONT_MIPI_RAW, GC05A2FRONT_MIPI_RAW_SensorInit},
#endif

/* emdoor end*/

	/*  ADD sensor driver before this line */
	{0, {0}, NULL}, /* end of list */
};

/* e_add new sensor driver here */


