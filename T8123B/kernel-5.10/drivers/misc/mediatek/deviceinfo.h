#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

struct dev_info_item {
   	char model[24];	
	char supplier[16];	
	char param[16];	
	char tclid[24];
	char manufacture_date[24];
	char tuning_ver[16];	
};

struct dev_info_hw {
	struct dev_info_item dev_CamNameB;
	struct dev_info_item dev_CamNameB2;
	struct dev_info_item dev_CamNameF;
	struct dev_info_item dev_CamOTPB;
	struct dev_info_item dev_CamOTPF;
	struct dev_info_item dev_LCM;
	struct dev_info_item dev_ctp;
	struct dev_info_item dev_eMMC;
	struct dev_info_item dev_gsensor;
	struct dev_info_item dev_psensor;
	struct dev_info_item dev_lsensor;
	struct dev_info_item dev_gyroscope;
	struct dev_info_item dev_compass;
	struct dev_info_item dev_NFC;
	struct dev_info_item dev_battery_info;
	struct dev_info_item dev_speaker1;
	struct dev_info_item dev_speaker2;
	struct dev_info_item dev_receiver1;
	struct dev_info_item dev_receiver2;
	struct dev_info_item dev_FM;
	struct dev_info_item dev_hall1;
	struct dev_info_item dev_hall2;
	struct dev_info_item dev_bluetooth;
	struct dev_info_item dev_wifi;
	struct dev_info_item dev_gps;
	struct dev_info_item dev_DTV;
	struct dev_info_item dev_ATV;
	struct dev_info_item dev_CPU;
	struct dev_info_item dev_FP;
	struct dev_info_item dev_CamNameB3;
	struct dev_info_item dev_CamNameB4;
	struct dev_info_item dev_DDR;
	struct dev_info_item dev_LCMB;
	struct dev_info_item dev_PMIC;
	struct dev_info_item dev_Barometer;
	struct dev_info_item dev_battery_manufacture_date;
};

#endif
