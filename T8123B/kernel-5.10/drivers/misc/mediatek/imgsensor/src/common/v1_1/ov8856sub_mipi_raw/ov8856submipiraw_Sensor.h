/*
 * Copyright (C) 2017 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */
/*****************************************************************************
 *
 * Filename:
 * ---------
 *	 OV8856SUBmipi_Sensor.h
 *
 * Project:
 * --------
 *	 ALPS
 *	Dream
 * Description:
 * ------------
 *	 CMOS sensor header file
 *
 ****************************************************************************/
#ifndef _OV8856SUBMIPI_SENSOR_H
#define _OV8856SUBMIPI_SENSOR_H


enum IMGSENSOR_MODE {
	IMGSENSOR_MODE_INIT,
	IMGSENSOR_MODE_PREVIEW,
	IMGSENSOR_MODE_CAPTURE,
	IMGSENSOR_MODE_VIDEO,
	IMGSENSOR_MODE_HIGH_SPEED_VIDEO,
	IMGSENSOR_MODE_SLIM_VIDEO,
};


struct imgsensor_mode_struct {
	kal_uint32 pclk;
	kal_uint32 linelength;
	kal_uint32 framelength;
	kal_uint8 startx;
	kal_uint8 starty;

	kal_uint16 grabwindow_width;
	kal_uint16 grabwindow_height;
	kal_uint32 mipi_pixel_rate;
	/*	 following for MIPIDataLowPwr2HighSpeedSettleDelayCount by different scenario	*/
	kal_uint8 mipi_data_lp2hs_settle_dc;

	/*	 following for GetDefaultFramerateByScenario()	*/
	kal_uint16 max_framerate;
};


/* SENSOR PRIVATE STRUCT FOR VARIABLES*/
struct imgsensor_struct {
	kal_uint8 mirror;				/*mirrorflip information*/

	kal_uint8 sensor_mode;			/*record IMGSENSOR_MODE enum value*/

	kal_uint32 shutter;				/*current shutter*/
	kal_uint16 gain;				/*current gain*/

	kal_uint32 pclk;				/*current pclk*/

	kal_uint32 frame_length;		/*current framelength*/
	kal_uint32 line_length;			/*current linelength*/

	kal_uint32 min_frame_length;	/*current min  framelength to max framerate*/
	kal_uint16 dummy_pixel;			/*current dummypixel*/
	kal_uint16 dummy_line;			/*current dummline*/

	kal_uint16 current_fps;			/*current max fps*/
	kal_bool   autoflicker_en;		/*record autoflicker enable or disable*/
	kal_bool test_pattern;			/*record test pattern mode or not*/
	enum MSDK_SCENARIO_ID_ENUM current_scenario_id;/*current scenario id*/
	kal_uint8  ihdr_en;				/*ihdr enable or disable*/

	kal_uint8 i2c_write_id;			/*record current sensor's i2c write id*/
};

/* SENSOR PRIVATE STRUCT FOR CONSTANT*/
struct imgsensor_info_struct {
	kal_uint16 sensor_id;			/*record sensor id defined in Kd_imgsensor.h*/
	kal_uint32 checksum_value;		/*checksum value for Camera Auto Test*/
	struct imgsensor_mode_struct pre;		/*preview scenario relative information*/
	struct imgsensor_mode_struct cap;		/*capture scenario relative information*/
	struct imgsensor_mode_struct cap1;
	/*capture for PIP 24fps relative information,
	 *capture1 mode must use same framelength, linelength with Capture mode for shutter calculate
	 */
	struct imgsensor_mode_struct normal_video;/*normal video  scenario relative information*/
	struct imgsensor_mode_struct hs_video;	/*high speed video scenario relative information*/
	struct imgsensor_mode_struct slim_video;	/*slim video for VT scenario relative information*/

	kal_uint8  ae_shut_delay_frame;	/*shutter delay frame for AE cycle*/
	kal_uint8  ae_sensor_gain_delay_frame;	/*sensor gain delay frame for AE cycle*/
	kal_uint8  ae_ispGain_delay_frame;	/*isp gain delay frame for AE cycle*/
	kal_uint8  ihdr_support;		/*1, support; 0,not support*/
	kal_uint8  ihdr_le_firstline;	/*1,le first ; 0, se first*/
	kal_uint8  sensor_mode_num;		/*support sensor mode num*/

	kal_uint8  cap_delay_frame;		/*enter capture delay frame num*/
	kal_uint8  pre_delay_frame;		/*enter preview delay frame num*/
	kal_uint8  video_delay_frame;	/*enter video delay frame num*/
	kal_uint8  hs_video_delay_frame;	/*enter high speed video  delay frame num*/
	kal_uint8  slim_video_delay_frame;	/*enter slim video delay frame num*/
	// huangjiwu for  captrue black --begin 
	kal_uint8 temperature_support;	/* 1, support; 0,not support */
	kal_uint32 min_gain;
	kal_uint32 max_gain;
	kal_uint32 min_gain_iso;
	kal_uint32 gain_step;
	kal_uint32 exp_step;
	kal_uint32 gain_type;
	//huangjiwu for  captrue black --end
	kal_uint8  margin;				/*sensor framelength & shutter margin */
	kal_uint32 min_shutter;			/*min shutter*/
	kal_uint32 max_frame_length;	/*max framelength by sensor register's limitation*/
	kal_uint8  isp_driving_current;	/*clk driving current*/
	kal_uint8  sensor_interface_type;/*sensor_interface_type*/
	kal_uint8  mipi_sensor_type; /*0,MIPI_OPHY_NCSI2; 1,MIPI_OPHY_CSI2, default is NCSI2, don't modify this para*/
	kal_uint8  mipi_settle_delay_mode;
	/*0, high speed signal auto detect; 1, use settle delay,unit is ns,
	 *default is auto detect, don't modify this para
	 */
	kal_uint8  sensor_output_dataformat;/*sensor output first pixel color*/
	kal_uint8  mclk;				/*mclk value, suggest 24 or 26 for 24Mhz or 26Mhz*/
	kal_uint32 i2c_speed;
	kal_uint8  mipi_lane_num;		/*mipi lane num*/
	kal_uint8  i2c_addr_table[5];	/*record sensor support all write id addr, only supprt 4must end with 0xff*/
};


extern int iReadRegI2C(u8 *a_pSendData, u16 a_sizeSendData, u8 *a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData, u16 a_sizeSendData, u16 i2cId);
extern int iBurstWriteReg_multi(u8 *pData, u32 bytes, u16 i2cId, u16 transfer_length, u16 timing);
#endif
