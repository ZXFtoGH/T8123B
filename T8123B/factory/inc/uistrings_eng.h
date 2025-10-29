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
 * MediaTek Inc. (C) 2010. All rights reserved.
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

#ifndef _UISTRINGS_ENG_H_
#define _UISTRINGS_ENG_H_

/*use define rather than variable. because initializer element must be constant*/
#define uistr_factory_mode_eng 		 "Factory Mode"
#define uistr_full_test_eng 		  "Manual Test"
#define uistr_auto_test_eng 		  "Auto Test"
#define uistr_item_test_eng 		  "Item Test"
#define uistr_test_report_eng		  "Test Report"
#define uistr_debug_test_eng 		  "Debug Test"
#define uistr_clear_flash_eng 	  "Clear Flash"
#define uistr_clear_storage_eng 		  "Clear Stroage"
#define uistr_version_eng 			  "Version"
#define uistr_reboot_eng 				  "Reboot"
#define uistr_pass_eng					  "Test Pass"
#define uistr_fail_eng 					  "Test Fail"
#define uistr_retest_eng 					"Retest"
#define uistr_key_back_eng 					 "Back"
#define uistr_key_pass_eng 					 "Pass"
#define uistr_key_fail_eng 					 "Fail"
#define uistr_key_pre_eng 					 "Prev"
#define uistr_key_next_eng 					 "Next"
#define uistr_keys_eng 					  "Keys"
#define uistr_jogball_eng 			  "Jogball"
#define uistr_ofn_eng 					  "OFN"
#define uistr_touch_eng 				  "Touch Panel"
#define uistr_touch_auto_eng 				  "Touch Panel Auto Test"
#define uistr_backlight_level_eng 				  "Backlight Level"
#define uistr_lcm_test_eng 				  "LCM Test"
#define uistr_lcm_eng 				  "LCM"
#define uistr_nand_flash_eng 		  "NAND Flash"
#define uistr_storage_eng 					  "Storage"
#define uistr_memory_card_eng 	  "Memory Card"
#define uistr_sim_card_eng 			  "SIM Card"
#define uistr_sim_detect_eng 		  "SIM Detect"
#define uistr_sig_test_eng 			  "Signaling Test"
#define uistr_vibrator_eng 			  "Vibrator"
#define uistr_led_eng 					  "LED"
#define uistr_rtc_eng 					  "RTC"
#define uistr_system_stability_eng 	"System Stability"
#define uistr_nfc_eng 					 "NFC"
#define uistr_cmmb_eng              "CMMB"
#define uistr_gps_eng               "GPS"
#define uistr_atv_eng                   "MATV Autoscan"
#define uistr_wifi_eng						"Wi-Fi"
#define uistr_bluetooth_eng       "Bluetooth"
#define uistr_idle_eng       "Idle Current"

/* AP AuxADC */
#define uistr_info_ap_auxadc_eng		"AP-AuxADC Test"
#define uistr_info_auxadc_chn_eng	"AP-AuxADC Channel Initial"

/*audio*/
#define uistr_info_audio_yes_eng							                "Yes"
#define uistr_info_audio_no_eng							                  "No"
#define uistr_info_audio_press_eng							              "Press"
#define uistr_info_audio_release_eng							            "Release"
#define uistr_info_audio_ringtone_eng							            "Ringtone"
#define uistr_info_audio_receiver_eng							            "Receiver"
#define uistr_info_audio_loopback_eng							            "Loopback"
#define uistr_info_audio_loopback_phone_mic_headset_eng				"Loopback-PhoneMic_HesdsetLR"
#define uistr_info_audio_loopback_phone_mic_speaker_eng				"Mic-Spk loopback"
#define uistr_info_audio_loopback_headset_mic_speaker_eng			"Loopback-HeadsetMic_SpeakerLR"
#define uistr_info_audio_loopback_waveplayback_eng						"WavePlayback"
#define uistr_info_audio_speaker_rdc_calibration_eng					"Speaker Rdc Calibration"
#define uistr_info_audio_loopback_note_eng							      "If HeadsetMic Loopback, \n Insert Headset... \n\n\n"
#define uistr_info_audio_headset_note_eng                     "Insert Headset... \n\n"
#define uistr_info_audio_headset_avail_eng                    "Headset Avail : "
#define uistr_info_audio_headset_mic_avail_eng                "Headset Mic : "
#define uistr_info_audio_headset_Button_eng                   "Headset Button: "
#define uistr_info_audio_loopback_complete_eng							  "Test Loopback Case In-Complete"
#define uistr_info_audio_loopback_headset_mic_eng							"Test Headset-Mic"
#define uistr_info_audio_loopback_dualmic_mic_eng							"Test Mic loopback"
#define uistr_info_audio_loopback_dualmic_mi1_eng							"Test Mic1 loopback"
#define uistr_info_audio_loopback_dualmic_mi2_eng							"Test Mic2 loopback"
#define uistr_info_audio_acoustic_loopback_eng							  "Dual-Mic Loopback"
#define uistr_info_audio_acoustic_loopback_DMNR_eng           "With DMNR"
#define uistr_info_audio_acoustic_loopback_without_DMNR_eng		"Without DMNR"
#define uistr_info_audio_headset_debug_eng "Headset Debug"
#define uistr_info_audio_receiver_debug_eng "Receiver Debug"
#define uistr_info_audio_micbias_eng                          "Micbias Test"
#define uistr_info_audio_micbias_on_eng                       "Micbias ON"
#define uistr_info_audio_micbias_off_eng                      "Micbias OFF"
#define uistr_info_audio_speaker_monitor_test_eng             "Speaker Monitor"
#define uistr_info_audio_speaker_monitor_set_temperature_eng  "Speaker Set Temperature"
#define uistr_info_audio_speaker_add_temperature_eng          "Speaker Temperature +0.5"
#define uistr_info_audio_speaker_sub_temperature_eng          "Speaker Temperature -0.5"
#define uistr_info_audio_speaker_exit_and_save_temperature_eng  "Exit And Save"
#define uistr_info_audio_speaker_exit_not_save_temperature_eng  "Exit"

#define uistr_info_audio_receiver_freq_response_eng           "Receiver Frequency Response Test"
#define uistr_info_audio_speaker_freq_response_eng            "Speaker Frequency Response Test"
#define uistr_info_audio_receiver_thd_eng                     "Receiver THD Test"
#define uistr_info_audio_speaker_thd_eng                      "Speaker THD Test"
#define uistr_info_audio_headset_thd_eng                      "Headset THD Test"

/*phone*/
#define uistr_info_audio_receiver_phone_eng							            "Receiver(Phone)"
#define uistr_info_headset_phone_eng							                  "Headset(Phone)"
#define uistr_info_audio_loopback_phone_mic_speaker_phone_eng				"Loopback-PhoneMic_SpeakerLR(Phone)"
#define uistr_vibrator_phone_eng 			  "Vibrator(Phone)"

/*speaker oc*/
#define uistr_info_speaker_oc_eng							                "Speaker OC Test"
#define uistr_info_speaker_oc_pass_eng							          "Test is passed"
#define uistr_info_speaker_oc_fail_eng							          "Test is failed"
#define uistr_info_speaker_oc_retest_eng							        "SPK OC Test is retesting \n\n"

/*headset*/
#define uistr_info_headset_eng							                  "Headset"
#define uistr_info_avail_eng						                  "Avail"
#define uistr_info_button_eng						                  "button"
#define uistr_info_press_eng						                  "press"
#define uistr_info_release_eng						                  "release"


#define uistr_g_sensor_eng			"G-Sensor"
#define uistr_g_sensor_c_eng		"G-Sensor cali"
#define uistr_m_sensor_eng			"M-Sensor"
#define uistr_als_ps_eng				"ALS/PS"
#define uistr_gyroscope_eng			"Gyroscope"
#define uistr_gyroscope_c_eng		"Gyroscope cali"
#define uistr_barometer_eng			"Barometer"
#define uistr_humidity_eng			"Humidity"
#define uistr_heart_monitor_eng		"Heart Rate Monitor"
#define uistr_bio_sensor_eng		"Bio-Sensor"
#define uistr_bio_sensor_c_eng		"Bio-Sensor cali"
#define uistr_rf_test_eng    "RF Test"
#define uistr_rf_c2k_test_eng    "C2K RF test"
#define uistr_ant_test_eng    "Universal ANT Test"
/*emergency_call*/
#define uistr_info_emergency_call_eng							  "Emergency Call"
#define uistr_info_emergency_call_not_start_eng		  "Emergency Call is not start."
#define uistr_info_emergency_call_testing_eng      "signal testing"
#define uistr_info_emergency_call_success_eng			  "dial 112 success!"
#define uistr_info_emergency_call_fail_eng			  "dial 112 failed!"
#define uistr_info_emergency_call_success_in_modem1_eng           "dail 112 success in modem1! "
#define uistr_info_emergency_call_fail_in_modem1_eng           "dail 112 failed in modem1!"
#define uistr_info_emergency_call_success_in_modem2_eng           "dail 112 success in modem2! "
#define uistr_info_emergency_call_fail_in_modem2_eng           "dail 112 failed in modem2!"
#define uistr_info_emergency_call_success_in_modem5_eng           "dail 112 success in modem5! "
#define uistr_info_emergency_call_fail_in_modem5_eng           "dail 112 failed in modem5!"

#define uistr_info_emergency_call_success_in_modem_eng			 "dial 112 success in modem"
#define uistr_info_emergency_call_fail_in_modem_eng			 "dial 112 failed in modem"

#define uistr_info_emergency_call_in_modem1_eng        "modem1 dial 112"
#define uistr_info_emergency_call_in_modem2_eng        "modem2 dial 112"
#define uistr_info_emergency_call_in_modem5_eng        "modem5 dial 112"


/*CMMB*/
#define uistr_info_cmmb_autoscan_eng             "Auto Scan"
#define uistr_info_cmmb_channellist_eng          "Channel List"
#define uistr_info_cmmb_init_ok_eng              "CMMB driver initialization OK."
#define uistr_info_cmmb_init_fail_eng            "CMMB driver initialization failed!"
#define uistr_info_cmmb_scanning_eng             "Scanning channels..."
#define uistr_info_cmmb_scan_ok_eng              "Channels auto scanning OK."
#define uistr_info_cmmb_scan_fail_eng            "Channels auto scanning failed!"
#define uistr_info_cmmb_tune_channel_eng         "Please select a channel to tune,and wait..."
#define uistr_info_cmmb_servicelist_eng          "CMMB Service List"
#define uistr_info_cmmb_selectstream_eng         "Please select a stream to record"
#define uistr_info_cmmb_recording_eng            "CMMB Recording Stream"
#define uistr_info_cmmb_recording_to_eng         "Recording stream to "
#define uistr_info_cmmb_stop_eng                 "Stop"
#define uistr_info_cmmb_stop_to_eng              "Record stopped, file is saved to "


/*eMMC-SD*/
#define uistr_info_emmc_eng			"eMMC"
#define uistr_info_sd_eng			"SD Card"
#define uistr_info_emmc_fat_eng		"eMMC FAT"
#define uistr_info_emmc_format_item_eng		"Format eMMC FAT (Recommend in product line)"
#define uistr_info_emmc_format_stat_eng		"Format Stat"
#define uistr_info_emmc_format_stat_start_eng		"Formatting eMMC FAT..."
#define uistr_info_emmc_format_data_start_eng   "Formatting /data..."
#define uistr_info_reboot_eng		"Rebooting..."
#define uistr_info_emmc_format_stat_success_eng		"Format eMMC FAT Successfully!"
#define uistr_info_emmc_format_stat_fail_eng		"Format eMMC FAT FAILED!"
#define uistr_info_emmc_sd_avail_eng		"Card Avail "
#define uistr_info_emmc_sd_yes_eng		"Yes"
#define uistr_info_emmc_sd_no_eng		"No"
#define uistr_info_emmc_sd_total_size_eng	"Total Size "
#define uistr_info_emmc_sd_free_size_eng	"Free Size  "
#define uistr_info_emmc_sd_checksum_eng		"Checksum   "
#define uistr_info_sd1_eng			"SD Card 1"
#define uistr_info_sd2_eng			"SD Card 2"

/* UFS */
#define uistr_info_ufs_eng			"UFS"

/* MNTL */
#define uistr_info_mntl_eng			"MNTL"

/* Bluetooth */
#define uistr_info_bt_init_eng            "Status: Initializing"
#define uistr_info_bt_init_fail_eng       "Status: INIT FAILED"
#define uistr_info_bt_init_ok_eng         "Status: Initialized"
#define uistr_info_bt_inquiry_start_eng   "Status: Start inquiring...\n"
#define uistr_info_bt_inquiry_1_eng       "Status: Inquiring ----- \n"
#define uistr_info_bt_inquiry_2_eng       "Status: Inquiring +++++ \n"
#define uistr_info_bt_scan_1_eng          "Status: Scanning ----- \n"
#define uistr_info_bt_scan_2_eng          "Status: Scanning +++++ \n"
#define uistr_info_bt_scan_complete_eng   "Status: Scan Completed\n"
#define uistr_info_bt_no_dev_eng          "----End of Device List  No dev found ----\n"
#define uistr_info_bt_dev_list_end_eng    "----End of Device List----\n"
#define uistr_info_bt_scan_list_end_eng   "----End of Scan List----\n"

/*Wi-Fi*/
#define uistr_info_wifi_test_pass_eng   "Test Pass"
#define uistr_info_wifi_test_fail_eng   "Test Fail"
#define uistr_info_wifi_renew_eng       "Renew"
#define uistr_info_wifi_error_eng       "[ERROR]"
#define uistr_info_wifi_warn_eng        "[WARN]"
#define uistr_info_wifi_status_eng      "Status"
#define uistr_info_wifi_start_eng       "Start"
#define uistr_info_wifi_init_fail_eng   "INIT FAILED"
#define uistr_info_wifi_scanning_eng    "Scanning"
#define uistr_info_wifi_timeout_eng     "Connection timed-out"
#define uistr_info_wifi_disconnect_eng  "Disconnect"
#define uistr_info_wifi_connecting_eng  "Connecting"
#define uistr_info_wifi_connected_eng  "Connected"
#define uistr_info_wifi_unknown_eng     "Unknown"
#define uistr_info_wifi_mode_eng        "Mode"
#define uistr_info_wifi_infra_eng       "Infrastructure"
#define uistr_info_wifi_adhoc_eng       "Ad-Hoc"
#define uistr_info_wifi_channel_eng     "Channel"
#define uistr_info_wifi_rssi_eng        "RSSI"
#define uistr_info_wifi_rate_eng        "RATE"
#define uistr_info_wifi_iface_err_eng   "read interface error"
#define uistr_info_wifi_fail_scan_eng   "failed to scan"
#define uistr_info_wifi_no_scan_res_eng "no APs found"
#define uistr_info_wifi_connect_err_eng "connect AP error"
#define uistr_info_wifi_no_ap_eng       "no AP to connect"


/* camera */
#define uistr_main_sensor_eng 	"Main Camera"
#define uistr_main2_sensor_eng 	"Main2 Camera"
#define uistr_main3_sensor_eng 	"Main3 Camera"
#define uistr_main4_sensor_eng 	"Main4 Camera"
#define uistr_sub_sensor_eng 	"Sub Camera"
#define uistr_sub2_sensor_eng 	"Sub2 Camera"
#define uistr_main_lens_eng 	"Main Lens"
#define uistr_main2_lens_eng 	"Main2 Lens"
#define uistr_main3_lens_eng 	"Main3 Lens"
#define uistr_sub_lens_eng 	 	"Sub Lens"
#define uistr_camera_prv_cap_strobe_eng "Preview/Capture/Strobe"
#define uistr_camera_prv_strobe_eng 	"Preview/Strobe"
#define uistr_camera_prv_cap_eng 	"Preview/Capture"
#define uistr_camera_back_eng       "Back"
#define uistr_camera_capture_eng    "Capture"
#define uistr_camera_preview_eng    "Preview"

/* Strobe */
#define uistr_strobe_eng 	"Strobe"
#define uistr_strobe_test_eng "Strobe Test"



/* USB */
#define uistr_info_usb_connect_eng 		  "Connect"
#define uistr_info_usb_disconnect_eng 	  "Disonnect"

/* battery&charging */
#define uistr_info_title_battery_charger_eng "Battery & Charger"
#define uistr_info_title_ac_charger_eng "AC Charger"
#define uistr_info_title_usb_charger_eng "USB Charger"
#define uistr_info_title_battery_yes_eng "YES"
#define uistr_info_title_battery_no_eng "NO"
#define uistr_info_title_battery_connect_eng "Connect"
#define uistr_info_title_battery_no_connect_eng "NO connect"
#define uistr_info_title_battery_cal_eng "BAT Cal."
#define uistr_info_title_battery_val_eng "BAT Volt."
#define uistr_info_title_battery_temp_eng "BAT Temp."
#define uistr_info_title_battery_chr_eng "CHGR"
#define uistr_info_title_battery_chr_val_eng "CHGR Volt."
#define uistr_info_title_battery_chr_current_eng "CHGR Curr."
#define uistr_info_title_battery_ad32_eng "AD32"
#define uistr_info_title_battery_ad42_eng "AD42"
#define uistr_info_title_battery_curad_eng "CurAD"
#define uistr_info_title_battery_fg_cur_eng "FG_Current"
#define uistr_info_title_battery_pmic_chip_eng "PMIC Chip"
#define uistr_info_title_battery_mv_eng "mV"
#define uistr_info_title_battery_ma_eng "mA"
#define uistr_info_title_battery_c_eng "Celsius"

/* slave charger */
#define uistr_info_title_slave_charger_eng "Slave charger"
#define uistr_info_title_slave_charger_connect_eng "Connect"
#define uistr_info_title_slave_charger_no_connect_eng "NO connect"

/* ext buck */
#define uistr_info_title_ext_buck_item_eng "EXT BUCK"
#define uistr_info_title_ext_buck_chip_eng "EXT BUCK"
#define uistr_info_title_ext_buck2_chip_eng "EXT BUCK2"
#define uistr_info_title_ext_buck3_chip_eng "EXT BUCK3"
#define uistr_info_title_ext_buck_connect_eng "Connect"
#define uistr_info_title_ext_buck_no_connect_eng "NO connect"

/* ext vbat boost */
#define uistr_info_title_ext_vbat_boost_item_eng "EXT VBAT BOOST"
#define uistr_info_title_ext_vbat_boost_chip_eng "EXT VBAT BOOST"
#define uistr_info_title_ext_vbat_boost_connect_eng "Connect"
#define uistr_info_title_ext_vbat_boost_no_connect_eng "NO connect"

/* OTG */
#define uistr_info_otg_status_eng "OTG status"
#define uistr_info_otg_status_device_eng "Device"
#define uistr_info_otg_status_host_eng "Host"

/* LED */
#define uistr_info_nled_test_eng			"NLED test"
#define uistr_info_keypad_led_test_eng		"Keypad LED test"

/* Backlight */
#define uistr_info_show_test_images_eng		"Show Test Images"
#define uistr_info_change_contrast_eng		"Change Contrast"

/* System Stability */
#define uistr_info_stress_test_result_eng		"Stress Test Result"

/* GPS */
#define uistr_info_gps_hot_restart_eng    "Hot Restart"
#define uistr_info_gps_cold_restart_eng   "Cold Restart"
#define uistr_info_gps_error_eng          "GPS failed!"
#define uistr_info_gps_fixed_eng          "GPS fixed TTFF(s)"
#define uistr_info_gps_ttff_eng           "GPS TTFF(s)"
#define uistr_info_gps_svid_eng          "SVid"
#define uistr_info_gps_init_eng           "GPS initializing..."

/*NAND*/
#define uistr_info_nand_clear_flash_eng		"Clear Flash"
#define uistr_info_nand_Manufacturer_ID_eng		"Manufacturer ID"
#define uistr_info_nand_Device_ID_eng		"Device ID"
#define uistr_info_nand_R_W_tests_result_eng		"R/W tests result"
#define uistr_info_nand_erase_info_eng		"\n\nClearing /data(%d).\n\n\nPlease wait !\n\n\nOnce clear is done.\n\nsystem will REBOOT!\n"

/*mATV*/
#define uistr_info_atv_autoscan_eng             "Autoscan"
#define uistr_info_atv_channellist_eng          "Channel List"
#define uistr_info_atv_init_ok_eng              "initilize ok"
#define uistr_info_atv_initizling_eng              "opening, please wait"
#define uistr_info_atv_init_fail_eng            "initilize fail"
#define uistr_info_atv_previewCH_eng                 "Preview Channel"
#define uistr_info_atv_switchCH_eng                 "Switch Channel"
#define uistr_info_atv_refreshCH_eng                 "Refresh Channel"
#define uistr_info_atv_CH_eng				 "CH"
#define	Country_AFGHANISTAN_eng		"Afghanistan"
#define	Country_ARGENTINA_eng		"Argentina"
#define Country_AUSTRALIA_eng		"Australia"
#define Country_BRAZIL_eng		"Brazil"
#define Country_BURMA_eng		"Burma"
#define Country_CAMBODIA_eng		"Cambodia"
#define Country_CANADA_eng		"Canada"
#define Country_CHILE_eng		"Chile"
#define Country_CHINA_eng		"Mainland China"
#define Country_CHINA_HONGKONG_eng		"Chinese Hong Kong"
#define Country_CHINA_SHENZHEN_eng		"Chinese Shenzhen"
#define Country_EUROPE_EASTERN_eng		"Eastern Europe"
#define Country_EUROPE_WESTERN_eng		"Western Europe"
#define Country_FRANCE_eng			"France"
#define Country_FRENCH_COLONIE_eng		"French Colonie"
#define Country_INDIA_eng		"India"
#define Country_INDONESIA_eng		"Indonesia"
#define Country_IRAN_eng		"Iran"
#define Country_ITALY_eng		"Italy"
#define Country_JAPAN_eng		"Japan"
#define Country_KOREA_eng		"Korea"
#define Country_LAOS_eng		"Laos"
#define Country_MALAYSIA_eng		"Malaysia"
#define Country_MEXICO_eng		"Mexico"
#define Country_NEWZEALAND_eng		"NewZealand"
#define Country_PAKISTAN_eng		"Pakistan"
#define Country_PARAGUAY_eng		"Paraguay"
#define Country_PHILIPPINES_eng		"Philippines"
#define Country_PORTUGAL_eng		"Portugal"
#define Country_RUSSIA_eng			"Russia"
#define Country_SINGAPORE_eng		"Singapore"
#define Country_SOUTHAFRICA_eng		"South Africa"
#define Country_SPAIN_eng				"Spain"
#define Country_TAIWAN_eng			"Taiwan"
#define Country_THAILAND_eng		"Thailand"
#define Country_TURKEY_eng		"Turkey"
#define Country_UNITED_ARAB_EMIRATES_eng	"United Arab Emirates"
#define Country_UNITED_KINGDOM_eng		"United Kingdom"
#define Country_USA_eng		"United State of America"
#define Country_URUGUAY_eng		"Uruguay"
#define Country_VENEZUELA_eng		"Venezuela"
#define Country_VIETNAM_eng		"Vietnam"
#define Country_IRELAND_eng		"Ireland"
#define Country_MOROCCO_eng		"Morocco"
#define Country_BANGLADESH_eng	"Bangladesh"
#define Country_EXIT_eng 				"Exit"


/* TV out */

#define uistr_info_tvout_plugin_eng 				 "TV cable is plugged in"
#define uistr_info_tvout_checkifplugin_eng 	 "Check if color bar shown on the TV"
#define uistr_info_tvout_notplugin_eng       "TV cable is NOT plugged in."
#define uistr_info_tvout_item_eng       		 "TV OUT"

/* SIM detect */
#define uistr_info_sim_detect_item_sim_1_eng	"Detect SIM1"
#define uistr_info_sim_detect_item_sim_2_eng	"Detect SIM2"
#define uistr_info_sim_detect_item_pass_eng		"Test Pass"
#define uistr_info_sim_detect_item_fail_eng		"Test Fail"
#define uistr_info_sim_detect_test_result_eng	"Detect SIM%d: %s\n"
#define uistr_info_sim_detect_result_pass_eng	"PASS"
#define uistr_info_sim_detect_result_fail_eng	"FAIL"

/* FM Radio */
#define uistr_info_fmr_title_eng                "FM Radio"
#define uistr_info_fmr_no_headset_warning_eng   "Please Insert Headset.\n"
#define uistr_info_fmr_open_fail_eng            "FM open failed\n"
#define uistr_info_fmr_poweron_fail_eng         "FM power on failed\n"
#define uistr_info_fmr_mute_fail_eng            "FM mute after on failed\n"
#define uistr_info_fmr_poweron_ok_eng           "FM power on successfully\n"
#define uistr_info_fmr_setfreq_eng              "FM set Freq:"
#define uistr_info_fmr_mhz_eng                 "MHz"
#define uistr_info_fmr_fail_eng                 "Failed\n"
#define uistr_info_fmr_success_eng              "Successfully\n"
#define uistr_info_fmr_rssi_eng                 "RSSI: %d(dBm)\n"
#define uistr_info_fmr_freq0_eng                "Test Freq 0"
#define uistr_info_fmr_freq1_eng                "Test Freq 1"
#define uistr_info_fmr_freq2_eng               "Test Freq 2"
#define uistr_info_fmr_freq3_eng                "Test Freq 3"
#define uistr_info_fmr_pass_eng                 "RSSI>=%d(dBm)Test Pass"
#define uistr_info_fmr_failed_eng               "RSSI< %d(dBm)Test Fail,try again"

/* FM Transmitter */
#define uistr_info_fmt_title_eng                "FM Transmitter"
#define uistr_info_fmt_open_fail_eng            "FM TX open failed\n"
#define uistr_info_fmt_poweron_fail_eng         "FM TX power on failed\n"
#define uistr_info_fmt_poweron_ok_eng           "FM TX power on successfully\n"
#define uistr_info_fmt_audio_out_eng            "1K Hz tone output\n"
#define uistr_info_fmt_setfreq_eng              "FM set Freq: "
#define uistr_info_fmt_mhz_eng                  "MHz"
#define uistr_info_fmt_fail_eng                 "Failed\n"
#define uistr_info_fmt_success_eng              "Successfully\n"
#define uistr_info_fmt_check_rds_fail_eng       "Check RDS TX Failed\n"
#define uistr_info_fmt_enable_rds_fail_eng      "Enable RDS TX Failed\n"
#define uistr_info_fmt_set_rds_fail_eng         "set rds faield\n"
#define uistr_info_fmt_rds_eng                  "RDS TX Data:"
#define uistr_info_fmt_freq0_eng               "Test Freq 0"
#define uistr_info_fmt_freq1_eng                "Test Freq 1"
#define uistr_info_fmt_freq2_eng                "Test Freq 2"
#define uistr_info_fmt_freq3_eng                "Test Freq 3"
#define uistr_info_fmt_freq4_eng                "Test Freq 4"
#define uistr_info_fmt_freq5_eng                "Test Freq 5"
#define uistr_info_fmt_freq6_eng                "Test Freq 6"
#define uistr_info_fmt_freq7_eng                "Test Freq 7"
#define uistr_info_fmt_freq8_eng                "Test Freq 8"
#define uistr_info_fmt_freq9_eng                "Test Freq 9"
#define uistr_info_fmt_pass_eng                 "Test Pass"
#define uistr_info_fmt_failed_eng               "Test Fail"

/* Touchpanel */
#define uistr_info_touch_ctp_main_eng			"CTP Test"
#define uistr_info_touch_calibration_eng			"Calibration"
#define uistr_info_touch_rtp_linearity_eng			"RTP Linearity"
#define uistr_info_touch_ctp_linearity_eng			"CTP Linearity"
#define uistr_info_touch_rtp_accuracy_eng		"RTP Accuracy"
#define uistr_info_touch_ctp_accuracy_eng		"CTP Accuracy"
#define uistr_info_touch_sensitivity_eng			"Sensitivity"
#define uistr_info_touch_deadzone_eng				"Deadzone"
#define uistr_info_touch_zoom_eng					"PinchToZoom"
#define uistr_info_touch_freemode_eng				"FreeMode"
#define uistr_info_touch_start_eng					"touch screen to start!"
#define uistr_info_touch_red_cross_eng			"touch the red cross!"
#define uistr_info_touch_pass_continue_eng		"pass! tap to continue"

/* OFN */
#define uistr_info_ofn_fail_eng							"Fail"
#define uistr_info_ofn_pass_eng						"Pass"
#define uistr_info_ofn_back_eng						"Back"
#define uistr_info_ofn_free_mode_item_eng		"Free Mode"
#define uistr_info_ofn_pass_item_eng				"Test Pass"
#define uistr_info_ofn_fail_item_eng					"Test Fail"
#define uistr_info_ofn_return_item_eng				"Return"

/* G/M-Sensor & ALS/PS & Gyroscope */
#define uistr_info_sensor_back_eng							"back"
#define uistr_info_sensor_pass_eng							"pass"
#define uistr_info_sensor_init_fail_eng						"Initialize fail\n"
#define uistr_info_sensor_initializing_eng					"Initializing...\n"
#define uistr_info_sensor_fail_eng								"fail"

#define uistr_info_g_sensor_testing_eng						"in testing"
#define uistr_info_g_sensor_unknow_eng						"unknown"
#define uistr_info_g_sensor_doing_eng							"Ongoing"
#define uistr_info_g_sensor_done_eng							"done"
#define uistr_info_g_sensor_max_eng							"MAX"
#define uistr_info_g_sensor_min_eng							"MIN"
#define uistr_info_g_sensor_range_eng							"Max-Min should be in the range of 0 ~ 0.2\n"
#define uistr_info_g_sensor_selftest_eng						"selftest:"
#define uistr_info_g_sensor_statistic_eng						"statistic:"
#define uistr_info_g_sensor_notsupport_eng					"not supported"
#define uistr_info_g_sensor_avg_eng							"AVG"
#define uistr_info_g_sensor_std_eng								"STD"

#define uistr_info_m_sensor_self_eng								"shipment test"
#define uistr_info_m_sensor_notsupport_eng							"Not support,please ignore"
#define uistr_info_m_sensor_ok_eng									"OK"
#define uistr_info_m_sensor_testing_eng								"testing"
#define uistr_info_m_sensor_fail_eng								"FAIL"
#define uistr_info_m_sensor_status_eng								"status"
#define uistr_info_m_sensor_data_eng								"Msensor Raw Data"

#define uistr_info_sensor_cali_clear_eng								"Clear Cali Data"
#define uistr_info_sensor_cali_do_eng									"Do Calibration"
#define uistr_info_sensor_cali_do_20_eng								"Do Calibration(20%)"
#define uistr_info_sensor_cali_do_40_eng								"Do Calibration(40%)"
#define uistr_info_sensor_cali_ok_eng									"Cali done\n"
#define uistr_info_sensor_cali_fail_eng								"Cali fail\n"
#define uistr_info_sensor_cali_ongoing_eng							"Cali ongoing, don't touch!\n"

#define uistr_info_sensor_alsps_thres_high_eng					"PS THRESHOLD HIGH"
#define uistr_info_sensor_alsps_thres_low_eng					"PS THRESHOLD LOW"
#define uistr_info_sensor_alsps_check_command_eng					"commond not support, please ignore"
#define uistr_info_sensor_alsps_result_eng							"TEST RESULT"

#define uistr_info_sensor_pressure_value_eng							"PRESS"
#define uistr_info_sensor_temperature_value_eng							"TEMP"

#define uistr_info_bio_sensor_max_eng							"MAX"
#define uistr_info_bio_sensor_min_eng							"MIN"
#define uistr_info_bio_sensor_thres_eng						"THRESHOLD"
#define uistr_info_bio_sensor_result_eng						"TEST RESULT"
/*test report*/
#define uistr_info_test_report_back_eng                        "Back"

#define uistr_info_detect_sim1_eng                     "Detect SIM1"
#define uistr_info_detect_sim2_eng                     "Detect SIM2"
#define uistr_info_test_pass_eng                     "Test Pass"
#define uistr_info_test_fail_eng                     "Test Fail"
#define uistr_info_detect_sim_eng                     "Detect SIM"
#define uistr_info_yes_eng                     "Yes"
#define uistr_info_no_eng                     "No"
#define uistr_info_fail_eng                     "Fail"
#define uistr_info_pass_eng                     "Pass"

/* BTS */
#define uistr_bts_eng                           "BTS"

/* NFC */
#define uistr_info_nfc_swp_test_eng       "NFC_SWP_TEST"
#define uistr_info_nfc_tag_dep_eng        "NFC_TAG_DEP"
#define uistr_info_nfc_card_mode_eng      "NFC_NFC_CARD_MODE"
#define uistr_info_nfc_vcard_mode_eng     "NFC_NFC_VIRTUAL_CARD_MODE"
#define uistr_info_nfc_testing_eng      "Test..."
#define uistr_info_nfc_colse2reader_eng      "CLOSE TO READER"
#define uistr_info_nfc_init_eng              "initializing..."
#define uistr_info_nfc_vcard_removedSIM_eng     "Please Remove SIM or uSD"
#define uistr_info_nfc_put_dut2reader_cm_eng     "card mode configure ok. Please put DUT close to reader device"
#define uistr_info_nfc_put_dut2reader_vcm_eng     "virtual card mode configure ok and please put DUT close to reader device"
/*Hotknot*/
#define uistr_hotknot_eng       "Hotknot"
#define uistr_info_hotknot_master_eng				   "master device"
#define uistr_info_hotknot_slave_eng				   "slave device"
#define uistr_info_hotknot_fail_eng				   "Fail"
#define uistr_info_hotknot_pass_eng				   "Pass"
#define uistr_info_hotknot_mode_select_eng         "Select Master or slave mode"
#define uistr_info_hotknot_info_eng						"Do Hotknot again than will exit"

/* IrTx */
#define uistr_info_irtx_led_test_eng "IrTx LED Test"
#define uistr_info_irtx_open_device_eng "Status: Open devcie node"
#define uistr_info_irtx_call_ioctl_eng "Status: Call ioctl"
#define uistr_info_irtx_led_enable_eng "Enable IRTX LED"
#define uistr_info_irtx_led_disable_eng "Disable IRTX LED"

/* Fingerprint */
/* goodix begin */
#define uistr_goodix_fingerprint_eng "Fingerprint"
/* goodix end */

/* efuse */
#define uistr_info_efuse_test_eng   "EFUSE"
#define uistr_info_efuse_result_eng "EFUSE Self-blow result"
#define uistr_info_efuse_success_eng "SUCCESS"
#define uistr_info_efuse_reblow_eng "REBLOW"
#define uistr_info_efuse_broken_eng "BROKEN"
#define uistr_info_efuse_unknown_eng "UNKNOWN"

/*NTC*/
#define uistr_ntc_eng "NTC test"

/*KEYBOX*/
#define uistr_keybox_eng "Keybox test"


/*DCDC*/
#define uistr_dcdc_eng "DCDC test"

/*GMS*/
#define uistr_gms_eng "GMS Info test"
#define uistr_hall_eng "Hall"

#endif

