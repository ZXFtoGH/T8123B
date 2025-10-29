#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/properties.h>
#include <utils/Log.h>

#include "ftm.h"
#include "uistrings.h"
#include "uistrings_chn.h"
#include "uistrings_eng.h"


#define TAG "[FACTORY] "
#define FONT_GB2312_PROP "persist.vendor.factory.GB2312"

char uistr_factory_mode[SHORT_STRING] = {0};
char uistr_full_test[SHORT_STRING] = {0};
char uistr_auto_test[SHORT_STRING] = {0};
char uistr_item_test[SHORT_STRING] = {0};
char uistr_test_report[SHORT_STRING] = {0};
char uistr_debug_test[SHORT_STRING] = {0};
char uistr_clear_flash[SHORT_STRING] = {0};
char uistr_clear_storage[SHORT_STRING] = {0};
char uistr_version[SHORT_STRING] = {0};
char uistr_reboot[SHORT_STRING] = {0};
char uistr_pass[SHORT_STRING] = {0};
char uistr_fail[SHORT_STRING] = {0};
char uistr_retest[SHORT_STRING] = {0};
char uistr_key_back[SHORT_STRING] = {0};
char uistr_key_pass[SHORT_STRING] = {0};
char uistr_key_fail[SHORT_STRING] = {0};
char uistr_key_pre[SHORT_STRING] = {0};
char uistr_key_next[SHORT_STRING] = {0};
char uistr_keys[SHORT_STRING] = {0};
char uistr_jogball[SHORT_STRING] = {0};
char uistr_ofn[SHORT_STRING] = {0};
char uistr_touch[SHORT_STRING] = {0};
char uistr_touch_auto[SHORT_STRING] = {0};
char uistr_backlight_level[SHORT_STRING] = {0};
char uistr_lcm_test[SHORT_STRING] = {0};
char uistr_lcm[SHORT_STRING] = {0};
char uistr_nand_flash[SHORT_STRING] = {0};
char uistr_storage[SHORT_STRING] = {0};
char uistr_memory_card[SHORT_STRING] = {0};
char uistr_sim_card[SHORT_STRING] = {0};
char uistr_sim_detect[SHORT_STRING] = {0};
char uistr_sig_test[SHORT_STRING] = {0};
char uistr_vibrator[SHORT_STRING] = {0};
char uistr_led[SHORT_STRING] = {0};
char uistr_rtc[SHORT_STRING] = {0};
char uistr_system_stability[SHORT_STRING] = {0};
char uistr_nfc[SHORT_STRING] = {0};
char uistr_cmmb[SHORT_STRING] = {0};
char uistr_gps[SHORT_STRING] = {0};
char uistr_atv[SHORT_STRING] = {0};
char uistr_wifi[SHORT_STRING] = {0};
char uistr_bluetooth[SHORT_STRING] = {0};
char uistr_idle[SHORT_STRING] = {0};
char uistr_g_sensor[SHORT_STRING] = {0};
char uistr_g_sensor_c[SHORT_STRING] = {0};
char uistr_bio_sensor[SHORT_STRING] = {0};
char uistr_bio_sensor_c[SHORT_STRING] = {0};
char uistr_m_sensor[SHORT_STRING] = {0};
char uistr_ant_test[SHORT_STRING] = {0};
char uistr_rf_test[SHORT_STRING] = {0};
char uistr_rf_c2k_test[SHORT_STRING] = {0};
char uistr_als_ps[SHORT_STRING] = {0};
char uistr_gyroscope[SHORT_STRING] = {0};
char uistr_gyroscope_c[SHORT_STRING] = {0};
char uistr_barometer[SHORT_STRING] = {0};
char uistr_humidity[SHORT_STRING] = {0};
char uistr_heart_monitor[SHORT_STRING] = {0};

/* AP AuxADC */
char uistr_info_ap_auxadc[SHORT_STRING] = {0};
char uistr_info_auxadc_chn[SHORT_STRING] = {0};

/*audio*/
char uistr_info_audio_yes[SHORT_STRING] = {0};
char uistr_info_audio_no[SHORT_STRING] = {0};
char uistr_info_audio_press[SHORT_STRING] = {0};
char uistr_info_audio_release[SHORT_STRING] = {0};
char uistr_info_audio_ringtone[SHORT_STRING] = {0};
char uistr_info_audio_receiver[SHORT_STRING] = {0};
char uistr_info_audio_loopback[SHORT_STRING] = {0};
char uistr_info_audio_loopback_phone_mic_headset[SHORT_STRING] = {0};
char uistr_info_audio_loopback_phone_mic_speaker[SHORT_STRING] = {0};
char uistr_info_audio_loopback_headset_mic_speaker[SHORT_STRING] = {0};
char uistr_info_audio_loopback_waveplayback[SHORT_STRING] = {0};
char uistr_info_audio_speaker_rdc_calibration[SHORT_STRING] = {0};
char uistr_info_audio_loopback_note[LONG_STRING] = {0};
char uistr_info_audio_headset_note[SHORT_STRING] = {0};
char uistr_info_audio_headset_avail[SHORT_STRING] = {0};
char uistr_info_audio_headset_mic_avail[SHORT_STRING] = {0};
char uistr_info_audio_headset_Button[SHORT_STRING] = {0};
char uistr_info_audio_loopback_complete[SHORT_STRING] = {0};
char uistr_info_audio_loopback_headset_mic[SHORT_STRING] = {0};
char uistr_info_audio_loopback_dualmic_mic[SHORT_STRING] = {0};
char uistr_info_audio_loopback_dualmic_mi1[SHORT_STRING] = {0};
char uistr_info_audio_loopback_dualmic_mi2[SHORT_STRING] = {0};
char uistr_info_audio_acoustic_loopback[SHORT_STRING] = {0};
char uistr_info_audio_acoustic_loopback_DMNR[SHORT_STRING] = {0};
char uistr_info_audio_acoustic_loopback_without_DMNR[SHORT_STRING] = {0};
char uistr_info_audio_headset_debug[SHORT_STRING] = {0};
char uistr_info_audio_receiver_debug[SHORT_STRING] = {0};
char uistr_info_audio_micbias[SHORT_STRING] = {0};
char uistr_info_audio_micbias_on[SHORT_STRING] = {0};
char uistr_info_audio_micbias_off[SHORT_STRING] = {0};
char uistr_info_audio_speaker_monitor_test[SHORT_STRING] = {0};
char uistr_info_audio_speaker_monitor_set_temperature[SHORT_STRING] = {0};
char uistr_info_audio_speaker_add_temperature[SHORT_STRING] = {0};
char uistr_info_audio_speaker_sub_temperature[SHORT_STRING] = {0};
char uistr_info_audio_speaker_exit_and_save_temperature[SHORT_STRING] = {0};
char uistr_info_audio_speaker_exit_not_save_temperature[SHORT_STRING] = {0};

char uistr_info_audio_receiver_freq_response[SHORT_STRING] = {0};
char uistr_info_audio_speaker_freq_response[SHORT_STRING] = {0};
char uistr_info_audio_receiver_thd[SHORT_STRING] = {0};
char uistr_info_audio_speaker_thd[SHORT_STRING] = {0};
char uistr_info_audio_headset_thd[SHORT_STRING] = {0};

/*phone*/
char uistr_info_audio_receiver_phone[SHORT_STRING] = {0};
char uistr_info_headset_phone[SHORT_STRING] = {0};
char uistr_info_audio_loopback_phone_mic_speaker_phone[SHORT_STRING] = {0};
char uistr_vibrator_phone[SHORT_STRING] = {0};

/*speaker oc*/
char uistr_info_speaker_oc[SHORT_STRING] = {0};
char uistr_info_speaker_oc_pass[SHORT_STRING] = {0};
char uistr_info_speaker_oc_fail[SHORT_STRING] = {0};
char uistr_info_speaker_oc_retest[SHORT_STRING] = {0};

/*headset*/
char uistr_info_headset[SHORT_STRING] = {0};
char uistr_info_avail[SHORT_STRING] = {0};
char uistr_info_button[SHORT_STRING] = {0};
char uistr_info_press[SHORT_STRING] = {0};
char uistr_info_release[SHORT_STRING] = {0};



/*emergency_call*/
char uistr_info_emergency_call[SHORT_STRING] = {0};
char uistr_info_emergency_call_not_start[SHORT_STRING] = {0};
char uistr_info_emergency_call_testing[SHORT_STRING] = {0};
char uistr_info_emergency_call_success[SHORT_STRING] = {0};
char uistr_info_emergency_call_fail[SHORT_STRING] = {0};
char uistr_info_emergency_call_success_in_modem1  [SHORT_STRING] = {0};
char uistr_info_emergency_call_fail_in_modem1[SHORT_STRING] = {0};
char uistr_info_emergency_call_success_in_modem2[SHORT_STRING] = {0};
char uistr_info_emergency_call_fail_in_modem2[SHORT_STRING] = {0};
char uistr_info_emergency_call_success_in_modem5[SHORT_STRING] = {0};
char uistr_info_emergency_call_fail_in_modem5[SHORT_STRING] = {0};

char uistr_info_emergency_call_success_in_modem[SHORT_STRING] = {0};
char uistr_info_emergency_call_fail_in_modem[SHORT_STRING] = {0};

char uistr_info_emergency_call_in_modem1[SHORT_STRING] = {0};
char uistr_info_emergency_call_in_modem2[SHORT_STRING] = {0};
char uistr_info_emergency_call_in_modem5[SHORT_STRING] = {0};

/*Gyro*/

/*CMMB*/
char uistr_info_cmmb_autoscan[SHORT_STRING] = {0};
char uistr_info_cmmb_channellist[SHORT_STRING] = {0};
char uistr_info_cmmb_init_ok[SHORT_STRING] = {0};
char uistr_info_cmmb_init_fail[SHORT_STRING] = {0};
char uistr_info_cmmb_scanning[SHORT_STRING] = {0};
char uistr_info_cmmb_scan_ok[SHORT_STRING] = {0};
char uistr_info_cmmb_scan_fail[SHORT_STRING] = {0};
char uistr_info_cmmb_tune_channel[SHORT_STRING] = {0};
char uistr_info_cmmb_servicelist[SHORT_STRING] = {0};
char uistr_info_cmmb_selectstream[SHORT_STRING] = {0};
char uistr_info_cmmb_recording[SHORT_STRING] = {0};
char uistr_info_cmmb_recording_to[SHORT_STRING] = {0};
char uistr_info_cmmb_stop[SHORT_STRING] = {0};
char uistr_info_cmmb_stop_to[SHORT_STRING] = {0};

/*eMMC-SD*/
char uistr_info_emmc[SHORT_STRING] = {0};
char uistr_info_sd[SHORT_STRING] = {0};
char uistr_info_emmc_fat[SHORT_STRING] = {0};
char uistr_info_emmc_format_item[SHORT_STRING] = {0};
char uistr_info_emmc_format_stat[SHORT_STRING] = {0};
char uistr_info_emmc_format_stat_start[SHORT_STRING] = {0};
char uistr_info_emmc_format_data_start[SHORT_STRING] = {0};
char uistr_info_reboot[SHORT_STRING] = {0};
char uistr_info_emmc_format_stat_success[SHORT_STRING] = {0};
char uistr_info_emmc_format_stat_fail[SHORT_STRING] = {0};
char uistr_info_emmc_sd_avail[SHORT_STRING] = {0};
char uistr_info_emmc_sd_yes[SHORT_STRING] = {0};
char uistr_info_emmc_sd_no[SHORT_STRING] = {0};
char uistr_info_emmc_sd_total_size[SHORT_STRING] = {0};
char uistr_info_emmc_sd_free_size[SHORT_STRING] = {0};
char uistr_info_emmc_sd_checksum	[SHORT_STRING] = {0};
char uistr_info_sd1[SHORT_STRING] = {0};
char uistr_info_sd2[SHORT_STRING] = {0};

/* UFS */
char uistr_info_ufs[SHORT_STRING] = {0};

/* MNTL */
char uistr_info_mntl[SHORT_STRING] = {0};

/* Bluetooth */
char uistr_info_bt_init[SHORT_STRING] = {0};
char uistr_info_bt_init_fail[SHORT_STRING] = {0};
char uistr_info_bt_init_ok[SHORT_STRING] = {0};
char uistr_info_bt_inquiry_start[SHORT_STRING] = {0};
char uistr_info_bt_inquiry_1[SHORT_STRING] = {0};
char uistr_info_bt_inquiry_2[SHORT_STRING] = {0};
char uistr_info_bt_scan_1[SHORT_STRING] = {0};
char uistr_info_bt_scan_2[SHORT_STRING] = {0};
char uistr_info_bt_scan_complete[SHORT_STRING] = {0};
char uistr_info_bt_no_dev[SHORT_STRING] = {0};
char uistr_info_bt_dev_list_end[SHORT_STRING] = {0};
char uistr_info_bt_scan_list_end[SHORT_STRING] = {0};

/*Wi-Fi*/
char uistr_info_wifi_test_pass[SHORT_STRING] = {0};
char uistr_info_wifi_test_fail[SHORT_STRING] = {0};
char uistr_info_wifi_renew[SHORT_STRING] = {0};
char uistr_info_wifi_error[SHORT_STRING] = {0};
char uistr_info_wifi_warn[SHORT_STRING] = {0};
char uistr_info_wifi_status[SHORT_STRING] = {0};
char uistr_info_wifi_start[SHORT_STRING] = {0};
char uistr_info_wifi_init_fail[SHORT_STRING] = {0};
char uistr_info_wifi_scanning[SHORT_STRING] = {0};
char uistr_info_wifi_timeout[SHORT_STRING] = {0};
char uistr_info_wifi_disconnect[SHORT_STRING] = {0};
char uistr_info_wifi_connecting[SHORT_STRING] = {0};
char uistr_info_wifi_connected[SHORT_STRING] = {0};
char uistr_info_wifi_unknown[SHORT_STRING] = {0};
char uistr_info_wifi_mode[SHORT_STRING] = {0};
char uistr_info_wifi_infra[SHORT_STRING] = {0};
char uistr_info_wifi_adhoc[SHORT_STRING] = {0};
char uistr_info_wifi_channel[SHORT_STRING] = {0};
char uistr_info_wifi_rssi[SHORT_STRING] = {0};
char uistr_info_wifi_rate[SHORT_STRING] = {0};
char uistr_info_wifi_iface_err[SHORT_STRING] = {0};
char uistr_info_wifi_fail_scan[SHORT_STRING] = {0};
char uistr_info_wifi_no_scan_res[SHORT_STRING] = {0};
char uistr_info_wifi_connect_err[SHORT_STRING] = {0};
char uistr_info_wifi_no_ap[SHORT_STRING] = {0};





/* camera */
char uistr_main_sensor[SHORT_STRING] = {0};
char uistr_main2_sensor[SHORT_STRING] = {0};
char uistr_main3_sensor[SHORT_STRING] = {0};
char uistr_main4_sensor[SHORT_STRING] = {0};
char uistr_sub_sensor[SHORT_STRING] = {0};
char uistr_sub2_sensor[SHORT_STRING] = {0};
char uistr_main_lens[SHORT_STRING] = {0};
char uistr_main2_lens[SHORT_STRING] = {0};
char uistr_main3_lens[SHORT_STRING] = {0};
char uistr_sub_lens[SHORT_STRING] = {0};
char uistr_camera_prv_cap_strobe[SHORT_STRING] = {0};
char uistr_camera_prv_strobe[SHORT_STRING] = {0};
char uistr_camera_prv_cap[SHORT_STRING] = {0};
char uistr_camera_back[SHORT_STRING] = {0};
char uistr_camera_capture[SHORT_STRING] = {0};
char uistr_camera_preview[SHORT_STRING] = {0};
char uistr_strobe[SHORT_STRING] = {0};


/* USB */
char uistr_info_usb_connect[SHORT_STRING] = {0};
char uistr_info_usb_disconnect[SHORT_STRING] = {0};

/* battery&charging */
char uistr_info_title_battery_charger[SHORT_STRING] = {0};
char uistr_info_title_ac_charger[SHORT_STRING] = {0};
char uistr_info_title_usb_charger[SHORT_STRING] = {0};
char uistr_info_title_battery_yes[SHORT_STRING] = {0};
char uistr_info_title_battery_no[SHORT_STRING] = {0};
char uistr_info_title_battery_connect[SHORT_STRING] = {0};
char uistr_info_title_battery_no_connect[SHORT_STRING] = {0};
char uistr_info_title_battery_cal[SHORT_STRING] = {0};
char uistr_info_title_battery_val[SHORT_STRING] = {0};
char uistr_info_title_battery_temp[SHORT_STRING] = {0};
char uistr_info_title_battery_chr[SHORT_STRING] = {0};
char uistr_info_title_battery_chr_val[SHORT_STRING] = {0};
char uistr_info_title_battery_chr_current[SHORT_STRING] = {0};
char uistr_info_title_battery_ad32[SHORT_STRING] = {0};
char uistr_info_title_battery_ad42[SHORT_STRING] = {0};
char uistr_info_title_battery_curad[SHORT_STRING] = {0};
char uistr_info_title_battery_fg_cur[SHORT_STRING] = {0};
char uistr_info_title_battery_pmic_chip[SHORT_STRING] = {0};
char uistr_info_title_battery_mv[SHORT_STRING] = {0};
char uistr_info_title_battery_ma[SHORT_STRING] = {0};
char uistr_info_title_battery_c[SHORT_STRING] = {0};

/* slave charger */
char uistr_info_title_slave_charger[SHORT_STRING] = {0};
char uistr_info_title_slave_charger_connect[SHORT_STRING] = {0};
char uistr_info_title_slave_charger_no_connect[SHORT_STRING] = {0};

/* ext buck */
char uistr_info_title_ext_buck_item[SHORT_STRING] = {0};
char uistr_info_title_ext_buck_chip[SHORT_STRING] = {0};
char uistr_info_title_ext_buck2_chip[SHORT_STRING] = {0};
char uistr_info_title_ext_buck3_chip[SHORT_STRING] = {0};
char uistr_info_title_ext_buck_connect[SHORT_STRING] = {0};
char uistr_info_title_ext_buck_no_connect[SHORT_STRING] = {0};

/* ext vbat boost */
char uistr_info_title_ext_vbat_boost_item[SHORT_STRING] = {0};
char uistr_info_title_ext_vbat_boost_chip[SHORT_STRING] = {0};
char uistr_info_title_ext_vbat_boost_connect[SHORT_STRING] = {0};
char uistr_info_title_ext_vbat_boost_no_connect[SHORT_STRING] = {0};

/* OTG */
char uistr_info_otg_status[SHORT_STRING] = {0};
char uistr_info_otg_status_device[SHORT_STRING] = {0};
char uistr_info_otg_status_host[SHORT_STRING] = {0};

/* LED */
char uistr_info_nled_test[SHORT_STRING] = {0};
char uistr_info_keypad_led_test[SHORT_STRING] = {0};

/* Backlight */
char uistr_info_show_test_images[SHORT_STRING] = {0};
char uistr_info_change_contrast[SHORT_STRING] = {0};

/* System Stability */
char uistr_info_stress_test_result[SHORT_STRING] = {0};

/* GPS */
char uistr_info_gps_hot_restart[SHORT_STRING] = {0};
char uistr_info_gps_cold_restart[SHORT_STRING] = {0};
char uistr_info_gps_error[SHORT_STRING] = {0};
char uistr_info_gps_fixed[SHORT_STRING] = {0};
char uistr_info_gps_ttff[SHORT_STRING] = {0};
char uistr_info_gps_svid[SHORT_STRING] = {0};
char uistr_info_gps_init[SHORT_STRING] = {0};

/*NAND*/
char uistr_info_nand_clear_flash[SHORT_STRING] = {0};
char uistr_info_nand_Manufacturer_ID[SHORT_STRING] = {0};
char uistr_info_nand_Device_ID[SHORT_STRING] = {0};
char uistr_info_nand_R_W_tests_result[SHORT_STRING] = {0};
char uistr_info_nand_erase_info[LONG_STRING] = {0};

/*mATV*/
char uistr_info_atv_autoscan[SHORT_STRING] = {0};
char uistr_info_atv_channellist[SHORT_STRING] = {0};
char uistr_info_atv_init_ok[SHORT_STRING] = {0};
char uistr_info_atv_initizling[SHORT_STRING] = {0};
char uistr_info_atv_init_fail[SHORT_STRING] = {0};
char uistr_info_atv_previewCH[SHORT_STRING] = {0};
char uistr_info_atv_switchCH [SHORT_STRING] = {0};
char uistr_info_atv_refreshCH[SHORT_STRING] = {0};
char uistr_info_atv_CH[SHORT_STRING] = {0};
char Country_AFGHANISTAN[SHORT_STRING] = {0};
char Country_ARGENTINA[SHORT_STRING] = {0};
char Country_AUSTRALIA[SHORT_STRING] = {0};
char Country_BRAZIL[SHORT_STRING] = {0};
char Country_BURMA[SHORT_STRING] = {0};
char Country_CAMBODIA[SHORT_STRING] = {0};
char Country_CANADA[SHORT_STRING] = {0};
char Country_CHILE[SHORT_STRING] = {0};
char Country_CHINA[SHORT_STRING] = {0};
char Country_CHINA_HONGKONG[SHORT_STRING] = {0};
char Country_CHINA_SHENZHEN[SHORT_STRING] = {0};
char Country_EUROPE_EASTERN[SHORT_STRING] = {0};
char Country_EUROPE_WESTERN[SHORT_STRING] = {0};
char Country_FRANCE[SHORT_STRING] = {0};
char Country_FRENCH_COLONIE[SHORT_STRING] = {0};
char Country_INDIA[SHORT_STRING] = {0};
char Country_INDONESIA[SHORT_STRING] = {0};
char Country_IRAN[SHORT_STRING] = {0};
char Country_ITALY[SHORT_STRING] = {0};
char Country_JAPAN[SHORT_STRING] = {0};
char Country_KOREA[SHORT_STRING] = {0};
char Country_LAOS[SHORT_STRING] = {0};
char Country_MALAYSIA[SHORT_STRING] = {0};
char Country_MEXICO[SHORT_STRING] = {0};
char Country_NEWZEALAND[SHORT_STRING] = {0};
char Country_PAKISTAN[SHORT_STRING] = {0};
char Country_PARAGUAY[SHORT_STRING] = {0};
char Country_PHILIPPINES[SHORT_STRING] = {0};
char Country_PORTUGAL[SHORT_STRING] = {0};
char Country_RUSSIA[SHORT_STRING] = {0};
char Country_SINGAPORE[SHORT_STRING] = {0};
char Country_SOUTHAFRICA[SHORT_STRING] = {0};
char Country_SPAIN[SHORT_STRING] = {0};
char Country_TAIWAN[SHORT_STRING] = {0};
char Country_THAILAND[SHORT_STRING] = {0};
char Country_TURKEY[SHORT_STRING] = {0};
char Country_UNITED_ARAB_EMIRATES[SHORT_STRING] = {0};
char Country_UNITED_KINGDOM[SHORT_STRING] = {0};
char Country_USA[SHORT_STRING] = {0};
char Country_URUGUAY[SHORT_STRING] = {0};
char Country_VENEZUELA[SHORT_STRING] = {0};
char Country_VIETNAM[SHORT_STRING] = {0};
char Country_IRELAND[SHORT_STRING] = {0};
char Country_MOROCCO[SHORT_STRING] = {0};
char Country_BANGLADESH[SHORT_STRING] = {0};
char Country_EXIT[SHORT_STRING] = {0};


/* TV out */

char uistr_info_tvout_plugin[SHORT_STRING] = {0};
char uistr_info_tvout_checkifplugin[SHORT_STRING] = {0};
char uistr_info_tvout_notplugin[SHORT_STRING] = {0};
char uistr_info_tvout_item[SHORT_STRING] = {0};


/* SIM detect */
char uistr_info_sim_detect_item_sim_1[SHORT_STRING] = {0};
char uistr_info_sim_detect_item_sim_2[SHORT_STRING] = {0};
char uistr_info_sim_detect_item_pass[SHORT_STRING] = {0};
char uistr_info_sim_detect_item_fail[SHORT_STRING] = {0};
char uistr_info_sim_detect_test_result[SHORT_STRING] = {0};
char uistr_info_sim_detect_result_pass[SHORT_STRING] = {0};
char uistr_info_sim_detect_result_fail[SHORT_STRING] = {0};

/* FM Radio */
char uistr_info_fmr_title[SHORT_STRING] = {0};
char uistr_info_fmr_no_headset_warning[SHORT_STRING] = {0};
char uistr_info_fmr_open_fail[SHORT_STRING] = {0};
char uistr_info_fmr_poweron_fail[SHORT_STRING] = {0};
char uistr_info_fmr_mute_fail[SHORT_STRING] = {0};
char uistr_info_fmr_poweron_ok[SHORT_STRING] = {0};
char uistr_info_fmr_setfreq[SHORT_STRING] = {0};
char uistr_info_fmr_mhz[SHORT_STRING] = {0};
char uistr_info_fmr_fail[SHORT_STRING] = {0};
char uistr_info_fmr_success[SHORT_STRING] = {0};
char uistr_info_fmr_rssi[SHORT_STRING] = {0};
char uistr_info_fmr_freq0[SHORT_STRING] = {0};
char uistr_info_fmr_freq1[SHORT_STRING] = {0};
char uistr_info_fmr_freq2[SHORT_STRING] = {0};
char uistr_info_fmr_freq3[SHORT_STRING] = {0};
char uistr_info_fmr_pass[SHORT_STRING] = {0};
char uistr_info_fmr_failed[SHORT_STRING] = {0};

/* FM Transmitter */
char uistr_info_fmt_title[SHORT_STRING] = {0};
char uistr_info_fmt_open_fail[SHORT_STRING] = {0};
char uistr_info_fmt_poweron_fail[SHORT_STRING] = {0};
char uistr_info_fmt_poweron_ok[SHORT_STRING] = {0};
char uistr_info_fmt_audio_out[SHORT_STRING] = {0};
char uistr_info_fmt_setfreq[SHORT_STRING] = {0};
char uistr_info_fmt_mhz[SHORT_STRING] = {0};
char uistr_info_fmt_fail[SHORT_STRING] = {0};
char uistr_info_fmt_success[SHORT_STRING] = {0};
char uistr_info_fmt_check_rds_fail[SHORT_STRING] = {0};
char uistr_info_fmt_enable_rds_fail[SHORT_STRING] = {0};
char uistr_info_fmt_set_rds_fail[SHORT_STRING] = {0};
char uistr_info_fmt_rds[SHORT_STRING] = {0};
char uistr_info_fmt_freq0[SHORT_STRING] = {0};
char uistr_info_fmt_freq1[SHORT_STRING] = {0};
char uistr_info_fmt_freq2[SHORT_STRING] = {0};
char uistr_info_fmt_freq3[SHORT_STRING] = {0};
char uistr_info_fmt_freq4[SHORT_STRING] = {0};
char uistr_info_fmt_freq5[SHORT_STRING] = {0};
char uistr_info_fmt_freq6[SHORT_STRING] = {0};
char uistr_info_fmt_freq7[SHORT_STRING] = {0};
char uistr_info_fmt_freq8[SHORT_STRING] = {0};
char uistr_info_fmt_freq9[SHORT_STRING] = {0};
char uistr_info_fmt_pass[SHORT_STRING] = {0};
char uistr_info_fmt_failed[SHORT_STRING] = {0};
/*Wi-Fi*/
/*
char uistr_info_wifi_test_fail[SHORT_STRING] = {0};
char uistr_info_wifi_renew[SHORT_STRING] = {0};
char uistr_info_wifi_error[SHORT_STRING] = {0};
char uistr_info_wifi_warn[SHORT_STRING] = {0};
char uistr_info_wifi_status[SHORT_STRING] = {0};
char uistr_info_wifi_start[SHORT_STRING] = {0};
char uistr_info_wifi_init_fail[SHORT_STRING] = {0};
char uistr_info_wifi_scanning[SHORT_STRING] = {0};
char uistr_info_wifi_timeout[SHORT_STRING] = {0};
char uistr_info_wifi_disconnect[SHORT_STRING] = {0};
char uistr_info_wifi_connecting[SHORT_STRING] = {0};
char uistr_info_wifi_connected[SHORT_STRING] = {0};
char uistr_info_wifi_unknown[SHORT_STRING] = {0};
char uistr_info_wifi_mode[SHORT_STRING] = {0};
char uistr_info_wifi_infra[SHORT_STRING] = {0};
char uistr_info_wifi_adhoc[SHORT_STRING] = {0};
char uistr_info_wifi_channel[SHORT_STRING] = {0};
char uistr_info_wifi_rssi[SHORT_STRING] = {0};
char uistr_info_wifi_rate[SHORT_STRING] = {0};
*/
/* Touchpanel */
char uistr_info_touch_ctp_main[SHORT_STRING] = {0};
char uistr_info_touch_calibration[SHORT_STRING] = {0};
char uistr_info_touch_rtp_linearity[SHORT_STRING] = {0};
char uistr_info_touch_ctp_linearity[SHORT_STRING] = {0};
char uistr_info_touch_rtp_accuracy[SHORT_STRING] = {0};
char uistr_info_touch_ctp_accuracy[SHORT_STRING] = {0};
char uistr_info_touch_sensitivity[SHORT_STRING] = {0};
char uistr_info_touch_deadzone[SHORT_STRING] = {0};
char uistr_info_touch_zoom[SHORT_STRING] = {0};
char uistr_info_touch_freemode[SHORT_STRING] = {0};
char uistr_info_touch_start[SHORT_STRING] = {0};
char uistr_info_touch_red_cross[SHORT_STRING] = {0};
char uistr_info_touch_pass_continue[SHORT_STRING] = {0};

/* OFN */
char uistr_info_ofn_fail[SHORT_STRING] = {0};
char uistr_info_ofn_pass[SHORT_STRING] = {0};
char uistr_info_ofn_back[SHORT_STRING] = {0};
char uistr_info_ofn_free_mode_item[SHORT_STRING] = {0};
char uistr_info_ofn_pass_item[SHORT_STRING] = {0};
char uistr_info_ofn_fail_item[SHORT_STRING] = {0};
char uistr_info_ofn_return_item[SHORT_STRING] = {0};

/* G/M-Sensor & ALS/PS & Gyroscope */
char uistr_info_sensor_back[SHORT_STRING] = {0};
char uistr_info_sensor_pass[SHORT_STRING] = {0};
char uistr_info_sensor_init_fail[SHORT_STRING] = {0};
char uistr_info_sensor_initializing[SHORT_STRING] = {0};
char uistr_info_sensor_fail[SHORT_STRING] = {0};

char uistr_info_g_sensor_testing[SHORT_STRING] = {0};
char uistr_info_g_sensor_unknow[SHORT_STRING] = {0};
char uistr_info_g_sensor_doing[SHORT_STRING] = {0};
char uistr_info_g_sensor_done[SHORT_STRING] = {0};
char uistr_info_g_sensor_max[SHORT_STRING] = {0};
char uistr_info_g_sensor_min[SHORT_STRING] = {0};
char uistr_info_g_sensor_range[SHORT_STRING] = {0};
char uistr_info_g_sensor_selftest[SHORT_STRING] = {0};
char uistr_info_g_sensor_statistic[SHORT_STRING] = {0};
char uistr_info_g_sensor_notsupport[SHORT_STRING] = {0};
char uistr_info_g_sensor_avg[SHORT_STRING] = {0};
char uistr_info_g_sensor_std[SHORT_STRING] = {0};

char uistr_info_m_sensor_self[SHORT_STRING] = {0};
char uistr_info_m_sensor_notsupport[SHORT_STRING] = {0};
char uistr_info_m_sensor_ok[SHORT_STRING] = {0};
char uistr_info_m_sensor_testing[SHORT_STRING] = {0};
char uistr_info_m_sensor_fail[SHORT_STRING] = {0};
char uistr_info_m_sensor_status[SHORT_STRING] = {0};
char uistr_info_m_sensor_data[SHORT_STRING] = {0};

char uistr_info_sensor_cali_clear[SHORT_STRING] = {0};
char uistr_info_sensor_cali_do[SHORT_STRING] = {0};
char uistr_info_sensor_cali_do_20[SHORT_STRING] = {0};
char uistr_info_sensor_cali_do_40[SHORT_STRING] = {0};
char uistr_info_sensor_cali_ok[SHORT_STRING] = {0};
char uistr_info_sensor_cali_fail[SHORT_STRING] = {0};
char uistr_info_sensor_cali_ongoing[SHORT_STRING] = {0};

char uistr_info_sensor_alsps_thres_high[SHORT_STRING] = {0};
char uistr_info_sensor_alsps_thres_low[SHORT_STRING] = {0};
char uistr_info_sensor_alsps_check_command[SHORT_STRING] = {0};
char uistr_info_sensor_alsps_result[SHORT_STRING] = {0};

char uistr_info_sensor_pressure_value[SHORT_STRING] = {0};
char uistr_info_sensor_temperature_value[SHORT_STRING] = {0};

char uistr_info_bio_sensor_max[SHORT_STRING] = {0};
char uistr_info_bio_sensor_min[SHORT_STRING] = {0};
char uistr_info_bio_sensor_thres[SHORT_STRING] = {0};
char uistr_info_bio_sensor_result[SHORT_STRING] = {0};
/*test report*/
char uistr_info_test_report_back[SHORT_STRING] = {0};

char uistr_info_detect_sim1[SHORT_STRING] = {0};
char uistr_info_detect_sim2[SHORT_STRING] = {0};
char uistr_info_test_pass[SHORT_STRING] = {0};
char uistr_info_test_fail[SHORT_STRING] = {0};
char uistr_info_detect_sim[SHORT_STRING] = {0};
char uistr_info_yes[SHORT_STRING] = {0};
char uistr_info_no[SHORT_STRING] = {0};
char uistr_info_fail[SHORT_STRING] = {0};
char uistr_info_pass[SHORT_STRING] = {0};

/* BTS */
char uistr_bts[SHORT_STRING] = {0};

/* NFC */
char uistr_info_nfc_swp_test[SHORT_STRING] = {0};
char uistr_info_nfc_tag_dep[SHORT_STRING] = {0};
char uistr_info_nfc_card_mode[SHORT_STRING] = {0};
char uistr_info_nfc_vcard_mode[SHORT_STRING] = {0};
char uistr_info_nfc_colse2reader[SHORT_STRING] = {0};
char uistr_info_nfc_testing[SHORT_STRING] = {0};
char uistr_info_nfc_init[SHORT_STRING] = {0};
char uistr_info_nfc_vcard_removedSIM[SHORT_STRING] = {0};
char uistr_info_nfc_put_dut2reader_cm[SHORT_STRING] = {0};
char uistr_info_nfc_put_dut2reader_vcm[SHORT_STRING] = {0};
/*Hotknot*/
char uistr_hotknot[SHORT_STRING] = {0};
char uistr_info_hotknot_master[SHORT_STRING] = {0};
char uistr_info_hotknot_slave[SHORT_STRING] = {0};
char uistr_info_hotknot_fail[SHORT_STRING] = {0};
char uistr_info_hotknot_pass[SHORT_STRING] = {0};
char uistr_info_hotknot_mode_select[SHORT_STRING] = {0};
char uistr_info_hotknot_info[SHORT_STRING] = {0};

/* IrTx */
char uistr_info_irtx_led_test[SHORT_STRING] = {0};
char uistr_info_irtx_open_device[SHORT_STRING] = {0};
char uistr_info_irtx_call_ioctl[SHORT_STRING] = {0};
char uistr_info_irtx_led_enable[SHORT_STRING] = {0};
char uistr_info_irtx_led_disable[SHORT_STRING] = {0};

/* Fingerprint */
/* goodix begin */
char uistr_goodix_fingerprint[SHORT_STRING] = {0};
/* goodix end */

/* efuse */
char uistr_info_efuse_test[SHORT_STRING] = {0};
char uistr_info_efuse_result[SHORT_STRING] = {0};
char uistr_info_efuse_success[SHORT_STRING] = {0};
char uistr_info_efuse_reblow[SHORT_STRING] = {0};
char uistr_info_efuse_broken[SHORT_STRING] = {0};
char uistr_info_efuse_unknown[SHORT_STRING] = {0};
char uistr_ntc[SHORT_STRING] = {0};
char uistr_keybox[SHORT_STRING] = {0};
char uistr_dcdc[SHORT_STRING] = {0};
char uistr_gms[SHORT_STRING] = {0};
char uistr_hall[SHORT_STRING] = {0};



static void init_chn_string()
{
	snprintf(uistr_factory_mode, SHORT_STRING, "%s", uistr_factory_mode_chn);
	snprintf(uistr_full_test, SHORT_STRING, "%s", uistr_full_test_chn);
	snprintf(uistr_auto_test, SHORT_STRING, "%s", uistr_auto_test_chn);
	snprintf(uistr_item_test,   SHORT_STRING, "%s", uistr_item_test_chn);
	snprintf(uistr_test_report, SHORT_STRING, "%s", uistr_test_repor_chn);
	snprintf(uistr_debug_test,  SHORT_STRING, "%s", uistr_debug_test_chn);
	snprintf(uistr_clear_flash, SHORT_STRING, "%s", uistr_clear_flash_chn);
	snprintf(uistr_clear_storage,  SHORT_STRING, "%s", uistr_clear_storage_chn);
	snprintf(uistr_version,     SHORT_STRING, "%s", uistr_version_chn);
	snprintf(uistr_reboot,      SHORT_STRING, "%s", uistr_reboot_chn);
	snprintf(uistr_pass,        SHORT_STRING, "%s", uistr_pass_chn);
	snprintf(uistr_fail,        SHORT_STRING, "%s", uistr_fail_chn);
	snprintf(uistr_retest,      SHORT_STRING, "%s", uistr_retest_chn);


	snprintf(uistr_key_back, SHORT_STRING, "%s", uistr_key_back_chn);
	snprintf(uistr_key_pass, SHORT_STRING, "%s", uistr_key_pass_chn);
	snprintf(uistr_key_fail, SHORT_STRING, "%s", uistr_key_fail_chn);
	snprintf(uistr_key_pre, SHORT_STRING, "%s", uistr_key_pre_chn);
	snprintf(uistr_key_next, SHORT_STRING, "%s", uistr_key_next_chn);
	snprintf(uistr_keys, SHORT_STRING, "%s", uistr_keys_chn);
	snprintf(uistr_jogball, SHORT_STRING, "%s", uistr_jogball_chn);
	snprintf(uistr_ofn, SHORT_STRING, "%s", uistr_ofn_chn);
	snprintf(uistr_touch, SHORT_STRING, "%s", uistr_touch_chn);
	snprintf(uistr_touch_auto, SHORT_STRING, "%s", uistr_touch_auto_chn);
	snprintf(uistr_backlight_level, SHORT_STRING, "%s", uistr_backlight_level_chn);
	snprintf(uistr_lcm_test, SHORT_STRING, "%s", uistr_lcm_test_chn);
	snprintf(uistr_lcm, SHORT_STRING, "%s", uistr_lcm_chn);
	snprintf(uistr_nand_flash, SHORT_STRING, "%s", uistr_nand_flash_chn);
	snprintf(uistr_storage, SHORT_STRING, "%s", uistr_storage_chn);
	snprintf(uistr_memory_card, SHORT_STRING, "%s", uistr_memory_card_chn);
	snprintf(uistr_sim_card, SHORT_STRING, "%s", uistr_sim_card_chn);
	snprintf(uistr_sim_detect, SHORT_STRING, "%s", uistr_sim_detect_chn);
	snprintf(uistr_sig_test, SHORT_STRING, "%s", uistr_sig_test_chn);
	snprintf(uistr_vibrator, SHORT_STRING, "%s", uistr_vibrator_chn);
	snprintf(uistr_led, SHORT_STRING, "%s", uistr_led_chn);
	snprintf(uistr_rtc, SHORT_STRING, "%s", uistr_rtc_chn);
	snprintf(uistr_system_stability, SHORT_STRING, "%s", uistr_system_stability_chn);
	snprintf(uistr_nfc, SHORT_STRING, "%s", uistr_nfc_chn);
	snprintf(uistr_cmmb, SHORT_STRING, "%s", uistr_cmmb_chn);
	snprintf(uistr_gps, SHORT_STRING, "%s", uistr_gps_chn);
	snprintf(uistr_atv, SHORT_STRING, "%s", uistr_atv_chn);
	snprintf(uistr_wifi, SHORT_STRING, "%s", uistr_wifi_chn);
	snprintf(uistr_bluetooth, SHORT_STRING, "%s", uistr_bluetooth_chn);
	snprintf(uistr_idle, SHORT_STRING, "%s", uistr_idle_chn);
	snprintf(uistr_g_sensor, SHORT_STRING, "%s", uistr_g_sensor_chn);
	snprintf(uistr_g_sensor_c, SHORT_STRING, "%s", uistr_g_sensor_c_chn);
	snprintf(uistr_bio_sensor, SHORT_STRING, "%s", uistr_bio_sensor_chn);
	snprintf(uistr_bio_sensor_c, SHORT_STRING, "%s", uistr_bio_sensor_c_chn);
	snprintf(uistr_m_sensor, SHORT_STRING, "%s", uistr_m_sensor_chn);
	snprintf(uistr_ant_test, SHORT_STRING, "%s", uistr_ant_test_chn);
	snprintf(uistr_rf_test, SHORT_STRING, "%s", uistr_rf_test_chn);
	snprintf(uistr_rf_c2k_test, SHORT_STRING, "%s", uistr_rf_c2k_test_chn);
	snprintf(uistr_als_ps, SHORT_STRING, "%s", uistr_als_ps_chn);
	snprintf(uistr_gyroscope, SHORT_STRING, "%s", uistr_gyroscope_chn);
	snprintf(uistr_gyroscope_c, SHORT_STRING, "%s", uistr_gyroscope_c_chn);
	snprintf(uistr_barometer, SHORT_STRING, "%s", uistr_barometer_chn);
	snprintf(uistr_humidity, SHORT_STRING, "%s", uistr_humidity_chn);
	snprintf(uistr_heart_monitor, SHORT_STRING, "%s", uistr_heart_monitor_chn);

	/* AP AuxADC */
	snprintf(uistr_info_ap_auxadc, SHORT_STRING, "%s", uistr_info_ap_auxadc_chn);
	snprintf(uistr_info_auxadc_chn, SHORT_STRING, "%s", uistr_info_auxadc_chn_chn);

	/*audio*/
	snprintf(uistr_info_audio_yes, SHORT_STRING, "%s", uistr_info_audio_yes_chn);
	snprintf(uistr_info_audio_no, SHORT_STRING, "%s", uistr_info_audio_no_chn);
	snprintf(uistr_info_audio_press, SHORT_STRING, "%s", uistr_info_audio_press_chn);
	snprintf(uistr_info_audio_release, SHORT_STRING, "%s", uistr_info_audio_release_chn);
	snprintf(uistr_info_audio_ringtone, SHORT_STRING, "%s", uistr_info_audio_ringtone_chn);
	snprintf(uistr_info_audio_receiver, SHORT_STRING, "%s", uistr_info_audio_receiver_chn);
	snprintf(uistr_info_audio_loopback, SHORT_STRING, "%s", uistr_info_audio_loopback_chn);
	snprintf(uistr_info_audio_loopback_phone_mic_headset, SHORT_STRING, "%s", uistr_info_audio_loopback_phone_mic_headset_chn);
	snprintf(uistr_info_audio_loopback_phone_mic_speaker, SHORT_STRING, "%s", uistr_info_audio_loopback_phone_mic_speaker_chn);
	snprintf(uistr_info_audio_loopback_headset_mic_speaker, SHORT_STRING, "%s", uistr_info_audio_loopback_headset_mic_speaker_chn);
	snprintf(uistr_info_audio_loopback_waveplayback, SHORT_STRING, "%s", uistr_info_audio_loopback_waveplayback_chn);
	snprintf(uistr_info_audio_speaker_rdc_calibration, SHORT_STRING, "%s", uistr_info_audio_speaker_rdc_calibration_chn);
	snprintf(uistr_info_audio_loopback_note, LONG_STRING, "%s", uistr_info_audio_loopback_note_chn);
	snprintf(uistr_info_audio_headset_note, SHORT_STRING, "%s", uistr_info_audio_headset_note_chn);
	snprintf(uistr_info_audio_headset_avail, SHORT_STRING, "%s", uistr_info_audio_headset_avail_chn);
	snprintf(uistr_info_audio_headset_mic_avail, SHORT_STRING, "%s", uistr_info_audio_headset_mic_avail_chn);
	snprintf(uistr_info_audio_headset_Button, SHORT_STRING, "%s", uistr_info_audio_headset_Button_chn);
	snprintf(uistr_info_audio_loopback_complete, SHORT_STRING, "%s", uistr_info_audio_loopback_complete_chn);
	snprintf(uistr_info_audio_loopback_headset_mic, SHORT_STRING, "%s", uistr_info_audio_loopback_headset_mic_chn);
	snprintf(uistr_info_audio_loopback_dualmic_mic, SHORT_STRING, "%s", uistr_info_audio_loopback_dualmic_mic_chn);
	snprintf(uistr_info_audio_loopback_dualmic_mi1, SHORT_STRING, "%s", uistr_info_audio_loopback_dualmic_mi1_chn);
	snprintf(uistr_info_audio_loopback_dualmic_mi2, SHORT_STRING, "%s", uistr_info_audio_loopback_dualmic_mi2_chn);
	snprintf(uistr_info_audio_acoustic_loopback, SHORT_STRING, "%s", uistr_info_audio_acoustic_loopback_chn);
	snprintf(uistr_info_audio_acoustic_loopback_DMNR, SHORT_STRING, "%s", uistr_info_audio_acoustic_loopback_DMNR_chn);
	snprintf(uistr_info_audio_acoustic_loopback_without_DMNR, SHORT_STRING, "%s", uistr_info_audio_acoustic_loopback_without_DMNR_chn);
	snprintf(uistr_info_audio_headset_debug, SHORT_STRING, "%s", uistr_info_audio_headset_debug_chn);
	snprintf(uistr_info_audio_receiver_debug, SHORT_STRING, "%s", uistr_info_audio_receiver_debug_chn);
	snprintf(uistr_info_audio_micbias, SHORT_STRING, "%s", uistr_info_audio_micbias_chn);
	snprintf(uistr_info_audio_micbias_on, SHORT_STRING, "%s", uistr_info_audio_micbias_on_chn);
	snprintf(uistr_info_audio_micbias_off, SHORT_STRING, "%s", uistr_info_audio_micbias_off_chn);
	snprintf(uistr_info_audio_speaker_monitor_test, SHORT_STRING, "%s", uistr_info_audio_speaker_monitor_test_chn);
	snprintf(uistr_info_audio_speaker_monitor_set_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_monitor_set_temperature_chn);
	snprintf(uistr_info_audio_speaker_add_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_add_temperature_chn);
	snprintf(uistr_info_audio_speaker_sub_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_sub_temperature_chn);
	snprintf(uistr_info_audio_speaker_exit_and_save_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_exit_and_save_temperature_chn);
	snprintf(uistr_info_audio_speaker_exit_not_save_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_exit_not_save_temperature_chn);

	snprintf(uistr_info_audio_receiver_freq_response, SHORT_STRING, "%s", uistr_info_audio_receiver_freq_response_chn);
	snprintf(uistr_info_audio_speaker_freq_response, SHORT_STRING, "%s", uistr_info_audio_speaker_freq_response_chn);
	snprintf(uistr_info_audio_receiver_thd, SHORT_STRING, "%s", uistr_info_audio_receiver_thd_chn);
	snprintf(uistr_info_audio_speaker_thd, SHORT_STRING, "%s", uistr_info_audio_speaker_thd_chn);
	snprintf(uistr_info_audio_headset_thd, SHORT_STRING, "%s", uistr_info_audio_headset_thd_chn);

	/*phone*/
	snprintf(uistr_info_audio_receiver_phone, SHORT_STRING, "%s", uistr_info_audio_receiver_phone_chn);
	snprintf(uistr_info_headset_phone, SHORT_STRING, "%s", uistr_info_headset_phone_chn);
	snprintf(uistr_info_audio_loopback_phone_mic_speaker_phone, SHORT_STRING, "%s", uistr_info_audio_loopback_phone_mic_speaker_phone_chn);
	snprintf(uistr_vibrator_phone, SHORT_STRING, "%s", uistr_vibrator_phone_chn);

	/*speaker oc*/
	snprintf(uistr_info_speaker_oc, SHORT_STRING, "%s", uistr_info_speaker_oc_chn);
	snprintf(uistr_info_speaker_oc_pass, SHORT_STRING, "%s", uistr_info_speaker_oc_pass_chn);
	snprintf(uistr_info_speaker_oc_fail, SHORT_STRING, "%s", uistr_info_speaker_oc_fail_chn);
	snprintf(uistr_info_speaker_oc_retest, SHORT_STRING, "%s", uistr_info_speaker_oc_retest_chn);

	/*headset*/
	snprintf(uistr_info_headset, SHORT_STRING, "%s", uistr_info_headset_chn);
	snprintf(uistr_info_avail, SHORT_STRING, "%s", uistr_info_avail_chn);
	snprintf(uistr_info_button, SHORT_STRING, "%s", uistr_info_button_chn);
	snprintf(uistr_info_press, SHORT_STRING, "%s", uistr_info_press_chn);
	snprintf(uistr_info_release, SHORT_STRING, "%s", uistr_info_release_chn);



	/*emergency_call*/
	snprintf(uistr_info_emergency_call, SHORT_STRING, "%s", uistr_info_emergency_call_chn);
	snprintf(uistr_info_emergency_call_not_start, SHORT_STRING, "%s", uistr_info_emergency_call_not_start_chn);
	snprintf(uistr_info_emergency_call_testing, SHORT_STRING, "%s", uistr_info_emergency_call_testing_chn);
	snprintf(uistr_info_emergency_call_success, SHORT_STRING, "%s", uistr_info_emergency_call_success_chn);
	snprintf(uistr_info_emergency_call_fail, SHORT_STRING, "%s", uistr_info_emergency_call_fail_chn);
	snprintf(uistr_info_emergency_call_success_in_modem1  , SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem1_chn);
	snprintf(uistr_info_emergency_call_fail_in_modem1, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem1_chn);
	snprintf(uistr_info_emergency_call_success_in_modem2, SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem2_chn);
	snprintf(uistr_info_emergency_call_fail_in_modem2, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem2_chn);
	snprintf(uistr_info_emergency_call_success_in_modem5, SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem5_chn);
	snprintf(uistr_info_emergency_call_fail_in_modem5, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem5_chn);

	snprintf(uistr_info_emergency_call_success_in_modem, SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem_chn);
	snprintf(uistr_info_emergency_call_fail_in_modem, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem_chn);

	snprintf(uistr_info_emergency_call_in_modem1, SHORT_STRING, "%s", uistr_info_emergency_call_in_modem1_chn);
	snprintf(uistr_info_emergency_call_in_modem2, SHORT_STRING, "%s", uistr_info_emergency_call_in_modem2_chn);
	snprintf(uistr_info_emergency_call_in_modem5, SHORT_STRING, "%s", uistr_info_emergency_call_in_modem5_chn);

	/*Gyro*/

	/*CMMB*/
	snprintf(uistr_info_cmmb_autoscan, SHORT_STRING, "%s", uistr_info_cmmb_autoscan_chn);
	snprintf(uistr_info_cmmb_channellist, SHORT_STRING, "%s", uistr_info_cmmb_channellist_chn);
	snprintf(uistr_info_cmmb_init_ok, SHORT_STRING, "%s", uistr_info_cmmb_init_ok_chn);
	snprintf(uistr_info_cmmb_init_fail, SHORT_STRING, "%s", uistr_info_cmmb_init_fail_chn);
	snprintf(uistr_info_cmmb_scanning, SHORT_STRING, "%s", uistr_info_cmmb_scanning_chn);
	snprintf(uistr_info_cmmb_scan_ok, SHORT_STRING, "%s", uistr_info_cmmb_scan_ok_chn);
	snprintf(uistr_info_cmmb_scan_fail, SHORT_STRING, "%s", uistr_info_cmmb_scan_fail_chn);
	snprintf(uistr_info_cmmb_tune_channel, SHORT_STRING, "%s", uistr_info_cmmb_tune_channel_chn);
	snprintf(uistr_info_cmmb_servicelist, SHORT_STRING, "%s", uistr_info_cmmb_servicelist_chn);
	snprintf(uistr_info_cmmb_selectstream, SHORT_STRING, "%s", uistr_info_cmmb_selectstream_chn);
	snprintf(uistr_info_cmmb_recording, SHORT_STRING, "%s", uistr_info_cmmb_recording_chn);
	snprintf(uistr_info_cmmb_recording_to, SHORT_STRING, "%s", uistr_info_cmmb_recording_to_chn);
	snprintf(uistr_info_cmmb_stop, SHORT_STRING, "%s", uistr_info_cmmb_stop_chn);
	snprintf(uistr_info_cmmb_stop_to, SHORT_STRING, "%s", uistr_info_cmmb_stop_to_chn);

	/*eMMC-SD*/
	snprintf(uistr_info_emmc, SHORT_STRING, "%s", uistr_info_emmc_chn);
	snprintf(uistr_info_sd, SHORT_STRING, "%s", uistr_info_sd_chn);
	snprintf(uistr_info_emmc_fat, SHORT_STRING, "%s", uistr_info_emmc_fat_chn);
	snprintf(uistr_info_emmc_format_item, SHORT_STRING, "%s", uistr_info_emmc_format_item_chn);
	snprintf(uistr_info_emmc_format_stat, SHORT_STRING, "%s", uistr_info_emmc_format_stat_chn);
	snprintf(uistr_info_emmc_format_stat_start, SHORT_STRING, "%s", uistr_info_emmc_format_stat_start_chn);
	snprintf(uistr_info_emmc_format_data_start, SHORT_STRING, "%s", uistr_info_emmc_format_data_start_chn);
	snprintf(uistr_info_reboot, SHORT_STRING, "%s", uistr_info_reboot_chn);
	snprintf(uistr_info_emmc_format_stat_success, SHORT_STRING, "%s", uistr_info_emmc_format_stat_success_chn);
	snprintf(uistr_info_emmc_format_stat_fail, SHORT_STRING, "%s", uistr_info_emmc_format_stat_fail_chn);
	snprintf(uistr_info_emmc_sd_avail, SHORT_STRING, "%s", uistr_info_emmc_sd_avail_chn);
	snprintf(uistr_info_emmc_sd_yes, SHORT_STRING, "%s", uistr_info_emmc_sd_yes_chn);
	snprintf(uistr_info_emmc_sd_no, SHORT_STRING, "%s", uistr_info_emmc_sd_no_chn);
	snprintf(uistr_info_emmc_sd_total_size, SHORT_STRING, "%s", uistr_info_emmc_sd_total_size_chn);
	snprintf(uistr_info_emmc_sd_free_size, SHORT_STRING, "%s", uistr_info_emmc_sd_free_size_chn);
	snprintf(uistr_info_emmc_sd_checksum	, SHORT_STRING, "%s", uistr_info_emmc_sd_checksum_chn);
	snprintf(uistr_info_sd1, SHORT_STRING, "%s", uistr_info_sd1_chn);
	snprintf(uistr_info_sd2, SHORT_STRING, "%s", uistr_info_sd2_chn);

	/* UFS */
	snprintf(uistr_info_ufs, SHORT_STRING, "%s", uistr_info_ufs_chn);

	/* MNTL */
	snprintf(uistr_info_mntl, SHORT_STRING, "%s", uistr_info_mntl_chn);

	/* Bluetooth */
	snprintf(uistr_info_bt_init, SHORT_STRING, "%s", uistr_info_bt_init_chn);
	snprintf(uistr_info_bt_init_fail, SHORT_STRING, "%s", uistr_info_bt_init_fail_chn);
	snprintf(uistr_info_bt_init_ok, SHORT_STRING, "%s", uistr_info_bt_init_ok_chn);
	snprintf(uistr_info_bt_inquiry_start, SHORT_STRING, "%s", uistr_info_bt_inquiry_start_chn);
	snprintf(uistr_info_bt_inquiry_1, SHORT_STRING, "%s", uistr_info_bt_inquiry_1_chn);
	snprintf(uistr_info_bt_inquiry_2, SHORT_STRING, "%s", uistr_info_bt_inquiry_2_chn);
	snprintf(uistr_info_bt_scan_1, SHORT_STRING, "%s", uistr_info_bt_scan_1_chn);
	snprintf(uistr_info_bt_scan_2, SHORT_STRING, "%s", uistr_info_bt_scan_2_chn);
	snprintf(uistr_info_bt_scan_complete, SHORT_STRING, "%s", uistr_info_bt_scan_complete_chn);
	snprintf(uistr_info_bt_no_dev, SHORT_STRING, "%s", uistr_info_bt_no_dev_chn);
	snprintf(uistr_info_bt_dev_list_end, SHORT_STRING, "%s", uistr_info_bt_dev_list_end_chn);
	snprintf(uistr_info_bt_scan_list_end, SHORT_STRING, "%s", uistr_info_bt_scan_list_end_chn);

	/*Wi-Fi*/
	snprintf(uistr_info_wifi_test_pass, SHORT_STRING, "%s", uistr_info_wifi_test_pass_chn);
	snprintf(uistr_info_wifi_test_fail, SHORT_STRING, "%s", uistr_info_wifi_test_fail_chn);
	snprintf(uistr_info_wifi_renew, SHORT_STRING, "%s", uistr_info_wifi_renew_chn);
	snprintf(uistr_info_wifi_error, SHORT_STRING, "%s", uistr_info_wifi_error_chn);
	snprintf(uistr_info_wifi_warn, SHORT_STRING, "%s", uistr_info_wifi_warn_chn);
	snprintf(uistr_info_wifi_status, SHORT_STRING, "%s", uistr_info_wifi_status_chn);
	snprintf(uistr_info_wifi_start, SHORT_STRING, "%s", uistr_info_wifi_start_chn);
	snprintf(uistr_info_wifi_init_fail, SHORT_STRING, "%s", uistr_info_wifi_init_fail_chn);
	snprintf(uistr_info_wifi_scanning, SHORT_STRING, "%s", uistr_info_wifi_scanning_chn);
	snprintf(uistr_info_wifi_timeout, SHORT_STRING, "%s", uistr_info_wifi_timeout_chn);
	snprintf(uistr_info_wifi_disconnect, SHORT_STRING, "%s", uistr_info_wifi_disconnect_chn);
	snprintf(uistr_info_wifi_connecting, SHORT_STRING, "%s", uistr_info_wifi_connecting_chn);
	snprintf(uistr_info_wifi_connected, SHORT_STRING, "%s", uistr_info_wifi_connected_chn);
	snprintf(uistr_info_wifi_unknown, SHORT_STRING, "%s", uistr_info_wifi_unknown_chn);
	snprintf(uistr_info_wifi_mode, SHORT_STRING, "%s", uistr_info_wifi_mode_chn);
	snprintf(uistr_info_wifi_infra, SHORT_STRING, "%s", uistr_info_wifi_infra_chn);
	snprintf(uistr_info_wifi_adhoc, SHORT_STRING, "%s", uistr_info_wifi_adhoc_chn);
	snprintf(uistr_info_wifi_channel, SHORT_STRING, "%s", uistr_info_wifi_channel_chn);
	snprintf(uistr_info_wifi_rssi, SHORT_STRING, "%s", uistr_info_wifi_rssi_chn);
	snprintf(uistr_info_wifi_rate, SHORT_STRING, "%s", uistr_info_wifi_rate_chn);
	snprintf(uistr_info_wifi_iface_err, SHORT_STRING, "%s", uistr_info_wifi_iface_err_chn);
	snprintf(uistr_info_wifi_fail_scan, SHORT_STRING, "%s", uistr_info_wifi_fail_scan_chn);
	snprintf(uistr_info_wifi_no_scan_res, SHORT_STRING, "%s", uistr_info_wifi_no_scan_res_chn);
	snprintf(uistr_info_wifi_connect_err, SHORT_STRING, "%s", uistr_info_wifi_connect_err_chn);
	snprintf(uistr_info_wifi_no_ap, SHORT_STRING, "%s", uistr_info_wifi_no_ap_chn);





	/* camera */
	snprintf(uistr_main_sensor, SHORT_STRING, "%s", uistr_main_sensor_chn);
	snprintf(uistr_main2_sensor, SHORT_STRING, "%s", uistr_main2_sensor_chn);
	snprintf(uistr_main3_sensor, SHORT_STRING, "%s", uistr_main3_sensor_chn);
	snprintf(uistr_main4_sensor, SHORT_STRING, "%s", uistr_main4_sensor_chn);
	snprintf(uistr_sub_sensor, SHORT_STRING, "%s", uistr_sub_sensor_chn);
	snprintf(uistr_sub2_sensor, SHORT_STRING, "%s", uistr_sub2_sensor_chn);
	snprintf(uistr_main_lens, SHORT_STRING, "%s", uistr_main_lens_chn);
	snprintf(uistr_main2_lens, SHORT_STRING, "%s", uistr_main2_lens_chn);
	snprintf(uistr_main3_lens, SHORT_STRING, "%s", uistr_main3_lens_chn);
	snprintf(uistr_sub_lens, SHORT_STRING, "%s", uistr_sub_lens_chn);
	snprintf(uistr_camera_prv_cap_strobe, SHORT_STRING, "%s", uistr_camera_prv_cap_strobe_chn);
	snprintf(uistr_camera_prv_strobe, SHORT_STRING, "%s", uistr_camera_prv_strobe_chn);
	snprintf(uistr_camera_prv_cap, SHORT_STRING, "%s", uistr_camera_prv_cap_chn);
	snprintf(uistr_camera_back, SHORT_STRING, "%s", uistr_camera_back_chn);
	snprintf(uistr_camera_capture, SHORT_STRING, "%s", uistr_camera_capture_chn);
	snprintf(uistr_camera_preview, SHORT_STRING, "%s", uistr_camera_preview_chn);
	snprintf(uistr_strobe, SHORT_STRING, "%s", uistr_strobe_chn);


	/* USB */
	snprintf(uistr_info_usb_connect, SHORT_STRING, "%s", uistr_info_usb_connect_chn);
	snprintf(uistr_info_usb_disconnect, SHORT_STRING, "%s", uistr_info_usb_disconnect_chn);

	/* battery&charging */
	snprintf(uistr_info_title_battery_charger, SHORT_STRING, "%s", uistr_info_title_battery_charger_chn);
	snprintf(uistr_info_title_ac_charger, SHORT_STRING, "%s", uistr_info_title_ac_charger_chn);
	snprintf(uistr_info_title_usb_charger, SHORT_STRING, "%s", uistr_info_title_usb_charger_chn);
	snprintf(uistr_info_title_battery_yes, SHORT_STRING, "%s", uistr_info_title_battery_yes_chn);
	snprintf(uistr_info_title_battery_no, SHORT_STRING, "%s", uistr_info_title_battery_no_chn);
	snprintf(uistr_info_title_battery_connect, SHORT_STRING, "%s", uistr_info_title_battery_connect_chn);
	snprintf(uistr_info_title_battery_no_connect, SHORT_STRING, "%s", uistr_info_title_battery_no_connect_chn);
	snprintf(uistr_info_title_battery_cal, SHORT_STRING, "%s", uistr_info_title_battery_cal_chn);
	snprintf(uistr_info_title_battery_val, SHORT_STRING, "%s", uistr_info_title_battery_val_chn);
	snprintf(uistr_info_title_battery_temp, SHORT_STRING, "%s", uistr_info_title_battery_temp_chn);
	snprintf(uistr_info_title_battery_chr, SHORT_STRING, "%s", uistr_info_title_battery_chr_chn);
	snprintf(uistr_info_title_battery_chr_val, SHORT_STRING, "%s", uistr_info_title_battery_chr_val_chn);
	snprintf(uistr_info_title_battery_chr_current, SHORT_STRING, "%s", uistr_info_title_battery_chr_current_chn);
	snprintf(uistr_info_title_battery_ad32, SHORT_STRING, "%s", uistr_info_title_battery_ad32_chn);
	snprintf(uistr_info_title_battery_ad42, SHORT_STRING, "%s", uistr_info_title_battery_ad42_chn);
	snprintf(uistr_info_title_battery_curad, SHORT_STRING, "%s", uistr_info_title_battery_curad_chn);
	snprintf(uistr_info_title_battery_fg_cur, SHORT_STRING, "%s", uistr_info_title_battery_fg_cur_chn);
	snprintf(uistr_info_title_battery_pmic_chip, SHORT_STRING, "%s", uistr_info_title_battery_pmic_chip_chn);
	snprintf(uistr_info_title_battery_mv, SHORT_STRING, "%s", uistr_info_title_battery_mv_chn);
	snprintf(uistr_info_title_battery_ma, SHORT_STRING, "%s", uistr_info_title_battery_ma_chn);
	snprintf(uistr_info_title_battery_c, SHORT_STRING, "%s", uistr_info_title_battery_c_chn);

	/* slave charger */
	snprintf(uistr_info_title_slave_charger, SHORT_STRING, "%s", uistr_info_title_slave_charger_chn);
	snprintf(uistr_info_title_slave_charger_connect, SHORT_STRING, "%s", uistr_info_title_slave_charger_connect_chn);
	snprintf(uistr_info_title_slave_charger_no_connect, SHORT_STRING, "%s", uistr_info_title_slave_charger_no_connect_chn);

	/* ext buck */
	snprintf(uistr_info_title_ext_buck_item, SHORT_STRING, "%s", uistr_info_title_ext_buck_item_chn);
	snprintf(uistr_info_title_ext_buck_chip, SHORT_STRING, "%s", uistr_info_title_ext_buck_chip_chn);
	snprintf(uistr_info_title_ext_buck2_chip, SHORT_STRING, "%s", uistr_info_title_ext_buck2_chip_chn);
	snprintf(uistr_info_title_ext_buck3_chip, SHORT_STRING, "%s", uistr_info_title_ext_buck3_chip_chn);
	snprintf(uistr_info_title_ext_buck_connect, SHORT_STRING, "%s", uistr_info_title_ext_buck_connect_chn);
	snprintf(uistr_info_title_ext_buck_no_connect, SHORT_STRING, "%s", uistr_info_title_ext_buck_no_connect_chn);

	/* ext vbat boost */
	snprintf(uistr_info_title_ext_vbat_boost_item, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_item_chn);
	snprintf(uistr_info_title_ext_vbat_boost_chip, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_chip_chn);
	snprintf(uistr_info_title_ext_vbat_boost_connect, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_connect_chn);
	snprintf(uistr_info_title_ext_vbat_boost_no_connect, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_no_connect_chn);

	/* OTG */
	snprintf(uistr_info_otg_status, SHORT_STRING, "%s", uistr_info_otg_status_chn);
	snprintf(uistr_info_otg_status_device, SHORT_STRING, "%s", uistr_info_otg_status_device_chn);
	snprintf(uistr_info_otg_status_host, SHORT_STRING, "%s", uistr_info_otg_status_host_chn);

	/* LED */
	snprintf(uistr_info_nled_test, SHORT_STRING, "%s", uistr_info_nled_test_chn);
	snprintf(uistr_info_keypad_led_test, SHORT_STRING, "%s", uistr_info_keypad_led_test_chn);

	/* Backlight */
	snprintf(uistr_info_show_test_images, SHORT_STRING, "%s", uistr_info_show_test_images_chn);
	snprintf(uistr_info_change_contrast, SHORT_STRING, "%s", uistr_info_change_contrast_chn);

	/* System Stability */
	snprintf(uistr_info_stress_test_result, SHORT_STRING, "%s", uistr_info_stress_test_result_chn);

	/* GPS */
	snprintf(uistr_info_gps_hot_restart, SHORT_STRING, "%s", uistr_info_gps_hot_restart_chn);
	snprintf(uistr_info_gps_cold_restart, SHORT_STRING, "%s", uistr_info_gps_cold_restart_chn);
	snprintf(uistr_info_gps_error, SHORT_STRING, "%s", uistr_info_gps_error_chn);
	snprintf(uistr_info_gps_fixed, SHORT_STRING, "%s", uistr_info_gps_fixed_chn);
	snprintf(uistr_info_gps_ttff, SHORT_STRING, "%s", uistr_info_gps_ttff_chn);
	snprintf(uistr_info_gps_svid, SHORT_STRING, "%s", uistr_info_gps_svid_chn);
	snprintf(uistr_info_gps_init, SHORT_STRING, "%s", uistr_info_gps_init_chn);

	/*NAND*/
	snprintf(uistr_info_nand_clear_flash, SHORT_STRING, "%s", uistr_info_nand_clear_flash_chn);
	snprintf(uistr_info_nand_Manufacturer_ID, SHORT_STRING, "%s", uistr_info_nand_Manufacturer_ID_chn);
	snprintf(uistr_info_nand_Device_ID, SHORT_STRING, "%s", uistr_info_nand_Device_ID_chn);
	snprintf(uistr_info_nand_R_W_tests_result, SHORT_STRING, "%s", uistr_info_nand_R_W_tests_result_chn);
	snprintf(uistr_info_nand_erase_info, LONG_STRING, "%s", uistr_info_nand_erase_info_chn);

	/*mATV*/
	snprintf(uistr_info_atv_autoscan, SHORT_STRING, "%s", uistr_info_atv_autoscan_chn);
	snprintf(uistr_info_atv_channellist, SHORT_STRING, "%s", uistr_info_atv_channellist_chn);
	snprintf(uistr_info_atv_init_ok, SHORT_STRING, "%s", uistr_info_atv_init_ok_chn);
	snprintf(uistr_info_atv_initizling, SHORT_STRING, "%s", uistr_info_atv_initizling_chn);
	snprintf(uistr_info_atv_init_fail, SHORT_STRING, "%s", uistr_info_atv_init_fail_chn);
	snprintf(uistr_info_atv_previewCH, SHORT_STRING, "%s", uistr_info_atv_previewCH_chn);
	snprintf(uistr_info_atv_switchCH , SHORT_STRING, "%s", uistr_info_atv_switchCH_chn);
	snprintf(uistr_info_atv_refreshCH, SHORT_STRING, "%s", uistr_info_atv_refreshCH_chn);
	snprintf(uistr_info_atv_CH, SHORT_STRING, "%s", uistr_info_atv_CH_chn);
	snprintf(Country_AFGHANISTAN, SHORT_STRING, "%s", Country_AFGHANISTAN_chn);
	snprintf(Country_ARGENTINA, SHORT_STRING, "%s", Country_ARGENTINA_chn);
	snprintf(Country_AUSTRALIA, SHORT_STRING, "%s", Country_AUSTRALIA_chn);
	snprintf(Country_BRAZIL, SHORT_STRING, "%s", Country_BRAZIL_chn);
	snprintf(Country_BURMA, SHORT_STRING, "%s", Country_BURMA_chn);
	snprintf(Country_CAMBODIA, SHORT_STRING, "%s", Country_CAMBODIA_chn);
	snprintf(Country_CANADA, SHORT_STRING, "%s", Country_CANADA_chn);
	snprintf(Country_CHILE, SHORT_STRING, "%s", Country_CHILE_chn);
	snprintf(Country_CHINA, SHORT_STRING, "%s", Country_CHINA_chn);
	snprintf(Country_CHINA_HONGKONG, SHORT_STRING, "%s", Country_CHINA_HONGKONG_chn);
	snprintf(Country_CHINA_SHENZHEN, SHORT_STRING, "%s", Country_CHINA_SHENZHEN_chn);
	snprintf(Country_EUROPE_EASTERN, SHORT_STRING, "%s", Country_EUROPE_EASTERN_chn);
	snprintf(Country_EUROPE_WESTERN, SHORT_STRING, "%s", Country_EUROPE_WESTERN_chn);
	snprintf(Country_FRANCE, SHORT_STRING, "%s", Country_FRANCE_chn);
	snprintf(Country_FRENCH_COLONIE, SHORT_STRING, "%s", Country_FRENCH_COLONIE_chn);
	snprintf(Country_INDIA, SHORT_STRING, "%s", Country_INDIA_chn);
	snprintf(Country_INDONESIA, SHORT_STRING, "%s", Country_INDONESIA_chn);
	snprintf(Country_IRAN, SHORT_STRING, "%s", Country_IRAN_chn);
	snprintf(Country_ITALY, SHORT_STRING, "%s", Country_ITALY_chn);
	snprintf(Country_JAPAN, SHORT_STRING, "%s", Country_JAPAN_chn);
	snprintf(Country_KOREA, SHORT_STRING, "%s", Country_KOREA_chn);
	snprintf(Country_LAOS, SHORT_STRING, "%s", Country_LAOS_chn);
	snprintf(Country_MALAYSIA, SHORT_STRING, "%s", Country_MALAYSIA_chn);
	snprintf(Country_MEXICO, SHORT_STRING, "%s", Country_MEXICO_chn);
	snprintf(Country_NEWZEALAND, SHORT_STRING, "%s", Country_NEWZEALAND_chn);
	snprintf(Country_PAKISTAN, SHORT_STRING, "%s", Country_PAKISTAN_chn);
	snprintf(Country_PARAGUAY, SHORT_STRING, "%s", Country_PARAGUAY_chn);
	snprintf(Country_PHILIPPINES, SHORT_STRING, "%s", Country_PHILIPPINES_chn);
	snprintf(Country_PORTUGAL, SHORT_STRING, "%s", Country_PORTUGAL_chn);
	snprintf(Country_RUSSIA, SHORT_STRING, "%s", Country_RUSSIA_chn);
	snprintf(Country_SINGAPORE, SHORT_STRING, "%s", Country_SINGAPORE_chn);
	snprintf(Country_SOUTHAFRICA, SHORT_STRING, "%s", Country_SOUTHAFRICA_chn);
	snprintf(Country_SPAIN, SHORT_STRING, "%s", Country_SPAIN_chn);
	snprintf(Country_TAIWAN, SHORT_STRING, "%s", Country_TAIWAN_chn);
	snprintf(Country_THAILAND, SHORT_STRING, "%s", Country_THAILAND_chn);
	snprintf(Country_TURKEY, SHORT_STRING, "%s", Country_TURKEY_chn);
	snprintf(Country_UNITED_ARAB_EMIRATES, SHORT_STRING, "%s", Country_UNITED_ARAB_EMIRATES_chn);
	snprintf(Country_UNITED_KINGDOM, SHORT_STRING, "%s", Country_UNITED_KINGDOM_chn);
	snprintf(Country_USA, SHORT_STRING, "%s", Country_USA_chn);
	snprintf(Country_URUGUAY, SHORT_STRING, "%s", Country_URUGUAY_chn);
	snprintf(Country_VENEZUELA, SHORT_STRING, "%s", Country_VENEZUELA_chn);
	snprintf(Country_VIETNAM, SHORT_STRING, "%s", Country_VIETNAM_chn);
	snprintf(Country_IRELAND, SHORT_STRING, "%s", Country_IRELAND_chn);
	snprintf(Country_MOROCCO, SHORT_STRING, "%s", Country_MOROCCO_chn);
	snprintf(Country_BANGLADESH, SHORT_STRING, "%s", Country_BANGLADESH_chn);
	snprintf(Country_EXIT, SHORT_STRING, "%s", Country_EXIT_chn);


	/* TV out */

	snprintf(uistr_info_tvout_plugin, SHORT_STRING, "%s", uistr_info_tvout_plugin_chn);
	snprintf(uistr_info_tvout_checkifplugin, SHORT_STRING, "%s", uistr_info_tvout_checkifplugin_chn);
	snprintf(uistr_info_tvout_notplugin, SHORT_STRING, "%s", uistr_info_tvout_notplugin_chn);
	snprintf(uistr_info_tvout_item, SHORT_STRING, "%s", uistr_info_tvout_item_chn);


	/* SIM detect */
	snprintf(uistr_info_sim_detect_item_sim_1, SHORT_STRING, "%s", uistr_info_sim_detect_item_sim_1_chn);
	snprintf(uistr_info_sim_detect_item_sim_2, SHORT_STRING, "%s", uistr_info_sim_detect_item_sim_2_chn);
	snprintf(uistr_info_sim_detect_item_pass, SHORT_STRING, "%s", uistr_info_sim_detect_item_pass_chn);
	snprintf(uistr_info_sim_detect_item_fail, SHORT_STRING, "%s", uistr_info_sim_detect_item_fail_chn);
	snprintf(uistr_info_sim_detect_test_result, SHORT_STRING, "%s", uistr_info_sim_detect_test_result_chn);
	snprintf(uistr_info_sim_detect_result_pass, SHORT_STRING, "%s", uistr_info_sim_detect_result_pass_chn);
	snprintf(uistr_info_sim_detect_result_fail, SHORT_STRING, "%s", uistr_info_sim_detect_result_fail_chn);

	/* FM Radio */
	snprintf(uistr_info_fmr_title, SHORT_STRING, "%s", uistr_info_fmr_title_chn);
	snprintf(uistr_info_fmr_no_headset_warning, SHORT_STRING, "%s", uistr_info_fmr_no_headset_warning_chn);
	snprintf(uistr_info_fmr_open_fail, SHORT_STRING, "%s", uistr_info_fmr_open_fail_chn);
	snprintf(uistr_info_fmr_poweron_fail, SHORT_STRING, "%s", uistr_info_fmr_poweron_fail_chn);
	snprintf(uistr_info_fmr_mute_fail, SHORT_STRING, "%s", uistr_info_fmr_mute_fail_chn);
	snprintf(uistr_info_fmr_poweron_ok, SHORT_STRING, "%s", uistr_info_fmr_poweron_ok_chn);
	snprintf(uistr_info_fmr_setfreq, SHORT_STRING, "%s", uistr_info_fmr_setfreq_chn);
	snprintf(uistr_info_fmr_mhz, SHORT_STRING, "%s", uistr_info_fmr_mhz_chn);
	snprintf(uistr_info_fmr_fail, SHORT_STRING, "%s", uistr_info_fmr_fail_chn);
	snprintf(uistr_info_fmr_success, SHORT_STRING, "%s", uistr_info_fmr_success_chn);
	snprintf(uistr_info_fmr_rssi, SHORT_STRING, "%s", uistr_info_fmr_rssi_chn);
	snprintf(uistr_info_fmr_freq0, SHORT_STRING, "%s", uistr_info_fmr_freq0_chn);
	snprintf(uistr_info_fmr_freq1, SHORT_STRING, "%s", uistr_info_fmr_freq1_chn);
	snprintf(uistr_info_fmr_freq2, SHORT_STRING, "%s", uistr_info_fmr_freq2_chn);
	snprintf(uistr_info_fmr_freq3, SHORT_STRING, "%s", uistr_info_fmr_freq3_chn);
	snprintf(uistr_info_fmr_pass, SHORT_STRING, "%s", uistr_info_fmr_pass_chn);
	snprintf(uistr_info_fmr_failed, SHORT_STRING, "%s", uistr_info_fmr_failed_chn);

	/* FM Transmitter */
	snprintf(uistr_info_fmt_title, SHORT_STRING, "%s", uistr_info_fmt_title_chn);
	snprintf(uistr_info_fmt_open_fail, SHORT_STRING, "%s", uistr_info_fmt_open_fail_chn);
	snprintf(uistr_info_fmt_poweron_fail, SHORT_STRING, "%s", uistr_info_fmt_poweron_fail_chn);
	snprintf(uistr_info_fmt_poweron_ok, SHORT_STRING, "%s", uistr_info_fmt_poweron_ok_chn);
	snprintf(uistr_info_fmt_audio_out, SHORT_STRING, "%s", uistr_info_fmt_audio_out_chn);
	snprintf(uistr_info_fmt_setfreq, SHORT_STRING, "%s", uistr_info_fmt_setfreq_chn);
	snprintf(uistr_info_fmt_mhz, SHORT_STRING, "%s", uistr_info_fmt_mhz_chn);
	snprintf(uistr_info_fmt_fail, SHORT_STRING, "%s", uistr_info_fmt_fail_chn);
	snprintf(uistr_info_fmt_success, SHORT_STRING, "%s", uistr_info_fmt_success_chn);
	snprintf(uistr_info_fmt_check_rds_fail, SHORT_STRING, "%s", uistr_info_fmt_check_rds_fail_chn);
	snprintf(uistr_info_fmt_enable_rds_fail, SHORT_STRING, "%s", uistr_info_fmt_enable_rds_fail_chn);
	snprintf(uistr_info_fmt_set_rds_fail, SHORT_STRING, "%s", uistr_info_fmt_set_rds_fail_chn);
	snprintf(uistr_info_fmt_rds, SHORT_STRING, "%s", uistr_info_fmt_rds_chn);
	snprintf(uistr_info_fmt_freq0, SHORT_STRING, "%s", uistr_info_fmt_freq0_chn);
	snprintf(uistr_info_fmt_freq1, SHORT_STRING, "%s", uistr_info_fmt_freq1_chn);
	snprintf(uistr_info_fmt_freq2, SHORT_STRING, "%s", uistr_info_fmt_freq2_chn);
	snprintf(uistr_info_fmt_freq3, SHORT_STRING, "%s", uistr_info_fmt_freq3_chn);
	snprintf(uistr_info_fmt_freq4, SHORT_STRING, "%s", uistr_info_fmt_freq4_chn);
	snprintf(uistr_info_fmt_freq5, SHORT_STRING, "%s", uistr_info_fmt_freq5_chn);
	snprintf(uistr_info_fmt_freq6, SHORT_STRING, "%s", uistr_info_fmt_freq6_chn);
	snprintf(uistr_info_fmt_freq7, SHORT_STRING, "%s", uistr_info_fmt_freq7_chn);
	snprintf(uistr_info_fmt_freq8, SHORT_STRING, "%s", uistr_info_fmt_freq8_chn);
	snprintf(uistr_info_fmt_freq9, SHORT_STRING, "%s", uistr_info_fmt_freq9_chn);
	snprintf(uistr_info_fmt_pass, SHORT_STRING, "%s", uistr_info_fmt_pass_chn);
	snprintf(uistr_info_fmt_failed, SHORT_STRING, "%s", uistr_info_fmt_failed_chn);
	/*Wi-Fi*/
	snprintf(uistr_info_wifi_test_fail, SHORT_STRING, "%s", uistr_info_wifi_test_fail_chn);
	snprintf(uistr_info_wifi_renew, SHORT_STRING, "%s", uistr_info_wifi_renew_chn);
	snprintf(uistr_info_wifi_error, SHORT_STRING, "%s", uistr_info_wifi_error_chn);
	snprintf(uistr_info_wifi_warn, SHORT_STRING, "%s", uistr_info_wifi_warn_chn);
	snprintf(uistr_info_wifi_status, SHORT_STRING, "%s", uistr_info_wifi_status_chn);
	snprintf(uistr_info_wifi_start, SHORT_STRING, "%s", uistr_info_wifi_start_chn);
	snprintf(uistr_info_wifi_init_fail, SHORT_STRING, "%s", uistr_info_wifi_init_fail_chn);
	snprintf(uistr_info_wifi_scanning, SHORT_STRING, "%s", uistr_info_wifi_scanning_chn);
	snprintf(uistr_info_wifi_timeout, SHORT_STRING, "%s", uistr_info_wifi_timeout_chn);
	snprintf(uistr_info_wifi_disconnect, SHORT_STRING, "%s", uistr_info_wifi_disconnect_chn);
	snprintf(uistr_info_wifi_connecting, SHORT_STRING, "%s", uistr_info_wifi_connecting_chn);
	snprintf(uistr_info_wifi_connected, SHORT_STRING, "%s", uistr_info_wifi_connected_chn);
	snprintf(uistr_info_wifi_unknown, SHORT_STRING, "%s", uistr_info_wifi_unknown_chn);
	snprintf(uistr_info_wifi_mode, SHORT_STRING, "%s", uistr_info_wifi_mode_chn);
	snprintf(uistr_info_wifi_infra, SHORT_STRING, "%s", uistr_info_wifi_infra_chn);
	snprintf(uistr_info_wifi_adhoc, SHORT_STRING, "%s", uistr_info_wifi_adhoc_chn);
	snprintf(uistr_info_wifi_channel, SHORT_STRING, "%s", uistr_info_wifi_channel_chn);
	snprintf(uistr_info_wifi_rssi, SHORT_STRING, "%s", uistr_info_wifi_rssi_chn);
	snprintf(uistr_info_wifi_rate, SHORT_STRING, "%s", uistr_info_wifi_rate_chn);

	/* Touchpanel */
	snprintf(uistr_info_touch_ctp_main, SHORT_STRING, "%s", uistr_info_touch_ctp_main_chn);
	snprintf(uistr_info_touch_calibration, SHORT_STRING, "%s", uistr_info_touch_calibration_chn);
	snprintf(uistr_info_touch_rtp_linearity, SHORT_STRING, "%s", uistr_info_touch_rtp_linearity_chn);
	snprintf(uistr_info_touch_ctp_linearity, SHORT_STRING, "%s", uistr_info_touch_ctp_linearity_chn);
	snprintf(uistr_info_touch_rtp_accuracy, SHORT_STRING, "%s", uistr_info_touch_rtp_accuracy_chn);
	snprintf(uistr_info_touch_ctp_accuracy, SHORT_STRING, "%s", uistr_info_touch_ctp_accuracy_chn);
	snprintf(uistr_info_touch_sensitivity, SHORT_STRING, "%s", uistr_info_touch_sensitivity_chn);
	snprintf(uistr_info_touch_deadzone, SHORT_STRING, "%s", uistr_info_touch_deadzone_chn);
	snprintf(uistr_info_touch_zoom, SHORT_STRING, "%s", uistr_info_touch_zoom_chn);
	snprintf(uistr_info_touch_freemode, SHORT_STRING, "%s", uistr_info_touch_freemode_chn);
	snprintf(uistr_info_touch_start, SHORT_STRING, "%s", uistr_info_touch_start_chn);
	snprintf(uistr_info_touch_red_cross, SHORT_STRING, "%s", uistr_info_touch_red_cross_chn);
	snprintf(uistr_info_touch_pass_continue, SHORT_STRING, "%s", uistr_info_touch_pass_continue_chn);

	/* OFN */
	snprintf(uistr_info_ofn_fail, SHORT_STRING, "%s", uistr_info_ofn_fail_chn);
	snprintf(uistr_info_ofn_pass, SHORT_STRING, "%s", uistr_info_ofn_pass_chn);
	snprintf(uistr_info_ofn_back, SHORT_STRING, "%s", uistr_info_ofn_back_chn);
	snprintf(uistr_info_ofn_free_mode_item, SHORT_STRING, "%s", uistr_info_ofn_free_mode_item_chn);
	snprintf(uistr_info_ofn_pass_item, SHORT_STRING, "%s", uistr_info_ofn_pass_item_chn);
	snprintf(uistr_info_ofn_fail_item, SHORT_STRING, "%s", uistr_info_ofn_fail_item_chn);
	snprintf(uistr_info_ofn_return_item, SHORT_STRING, "%s", uistr_info_ofn_return_item_chn);

	/* G/M-Sensor & ALS/PS & Gyroscope */
	snprintf(uistr_info_sensor_back, SHORT_STRING, "%s", uistr_info_sensor_back_chn);
	snprintf(uistr_info_sensor_pass, SHORT_STRING, "%s", uistr_info_sensor_pass_chn);
	snprintf(uistr_info_sensor_init_fail, SHORT_STRING, "%s", uistr_info_sensor_init_fail_chn);
	snprintf(uistr_info_sensor_initializing, SHORT_STRING, "%s", uistr_info_sensor_initializing_chn);
	snprintf(uistr_info_sensor_fail, SHORT_STRING, "%s", uistr_info_sensor_fail_chn);

	snprintf(uistr_info_g_sensor_testing, SHORT_STRING, "%s", uistr_info_g_sensor_testing_chn);
	snprintf(uistr_info_g_sensor_unknow, SHORT_STRING, "%s", uistr_info_g_sensor_unknow_chn);
	snprintf(uistr_info_g_sensor_doing, SHORT_STRING, "%s", uistr_info_g_sensor_doing_chn);
	snprintf(uistr_info_g_sensor_done, SHORT_STRING, "%s", uistr_info_g_sensor_done_chn);
	snprintf(uistr_info_g_sensor_max, SHORT_STRING, "%s", uistr_info_g_sensor_max_chn);
	snprintf(uistr_info_g_sensor_min, SHORT_STRING, "%s", uistr_info_g_sensor_min_chn);
	snprintf(uistr_info_g_sensor_range, SHORT_STRING, "%s", uistr_info_g_sensor_range_chn);
	snprintf(uistr_info_g_sensor_selftest, SHORT_STRING, "%s", uistr_info_g_sensor_selftest_chn);
	snprintf(uistr_info_g_sensor_statistic, SHORT_STRING, "%s", uistr_info_g_sensor_statistic_chn);
	snprintf(uistr_info_g_sensor_notsupport, SHORT_STRING, "%s", uistr_info_g_sensor_notsupport_chn);
	snprintf(uistr_info_g_sensor_avg, SHORT_STRING, "%s", uistr_info_g_sensor_avg_chn);
	snprintf(uistr_info_g_sensor_std, SHORT_STRING, "%s", uistr_info_g_sensor_std_chn);

	snprintf(uistr_info_m_sensor_self, SHORT_STRING, "%s", uistr_info_m_sensor_self_chn);
	snprintf(uistr_info_m_sensor_notsupport, SHORT_STRING, "%s", uistr_info_m_sensor_notsupport_chn);
	snprintf(uistr_info_m_sensor_ok, SHORT_STRING, "%s", uistr_info_m_sensor_ok_chn);
	snprintf(uistr_info_m_sensor_testing, SHORT_STRING, "%s", uistr_info_m_sensor_testing_chn);
	snprintf(uistr_info_m_sensor_fail, SHORT_STRING, "%s", uistr_info_m_sensor_fail_chn);
	snprintf(uistr_info_m_sensor_status, SHORT_STRING, "%s", uistr_info_m_sensor_status_chn);
	snprintf(uistr_info_m_sensor_data, SHORT_STRING, "%s", uistr_info_m_sensor_data_chn);

	snprintf(uistr_info_sensor_cali_clear, SHORT_STRING, "%s", uistr_info_sensor_cali_clear_chn);
	snprintf(uistr_info_sensor_cali_do, SHORT_STRING, "%s", uistr_info_sensor_cali_do_chn);
	snprintf(uistr_info_sensor_cali_do_20, SHORT_STRING, "%s", uistr_info_sensor_cali_do_20_chn);
	snprintf(uistr_info_sensor_cali_do_40, SHORT_STRING, "%s", uistr_info_sensor_cali_do_40_chn);
	snprintf(uistr_info_sensor_cali_ok, SHORT_STRING, "%s", uistr_info_sensor_cali_ok_chn);
	snprintf(uistr_info_sensor_cali_fail, SHORT_STRING, "%s", uistr_info_sensor_cali_fail_chn);
	snprintf(uistr_info_sensor_cali_ongoing, SHORT_STRING, "%s", uistr_info_sensor_cali_ongoing_chn);

	snprintf(uistr_info_sensor_alsps_thres_high, SHORT_STRING, "%s", uistr_info_sensor_alsps_thres_high_chn);
	snprintf(uistr_info_sensor_alsps_thres_low, SHORT_STRING, "%s", uistr_info_sensor_alsps_thres_low_chn);
	snprintf(uistr_info_sensor_alsps_check_command, SHORT_STRING, "%s", uistr_info_sensor_alsps_check_command_chn);
	snprintf(uistr_info_sensor_alsps_result, SHORT_STRING, "%s", uistr_info_sensor_alsps_result_chn);

	snprintf(uistr_info_sensor_pressure_value, SHORT_STRING, "%s", uistr_info_sensor_pressure_value_chn);
	snprintf(uistr_info_sensor_temperature_value, SHORT_STRING, "%s", uistr_info_sensor_temperature_value_chn);

	snprintf(uistr_info_bio_sensor_max, SHORT_STRING, "%s", uistr_info_bio_sensor_max_chn);
	snprintf(uistr_info_bio_sensor_min, SHORT_STRING, "%s", uistr_info_bio_sensor_min_chn);
	snprintf(uistr_info_bio_sensor_thres, SHORT_STRING, "%s", uistr_info_bio_sensor_thres_chn);
	snprintf(uistr_info_bio_sensor_result, SHORT_STRING, "%s", uistr_info_bio_sensor_result_chn);
	/*test report*/
	snprintf(uistr_info_test_report_back, SHORT_STRING, "%s", uistr_info_test_report_back_chn);

	snprintf(uistr_info_detect_sim1, SHORT_STRING, "%s", uistr_info_detect_sim1_chn);
	snprintf(uistr_info_detect_sim2, SHORT_STRING, "%s", uistr_info_detect_sim2_chn);
	snprintf(uistr_info_test_pass, SHORT_STRING, "%s", uistr_info_test_pass_chn);
	snprintf(uistr_info_test_fail, SHORT_STRING, "%s", uistr_info_test_fail_chn);
	snprintf(uistr_info_detect_sim, SHORT_STRING, "%s", uistr_info_detect_sim_chn);
	snprintf(uistr_info_yes, SHORT_STRING, "%s", uistr_info_yes_chn);
	snprintf(uistr_info_no, SHORT_STRING, "%s", uistr_info_no_chn);
	snprintf(uistr_info_fail, SHORT_STRING, "%s", uistr_info_fail_chn);
	snprintf(uistr_info_pass, SHORT_STRING, "%s", uistr_info_pass_chn);

	/* BTS */
	snprintf(uistr_bts, SHORT_STRING, "%s", uistr_bts_chn);

	/* NFC */
	snprintf(uistr_info_nfc_swp_test, SHORT_STRING, "%s", uistr_info_nfc_swp_test_chn);
	snprintf(uistr_info_nfc_tag_dep, SHORT_STRING, "%s", uistr_info_nfc_tag_dep_chn);
	snprintf(uistr_info_nfc_card_mode, SHORT_STRING, "%s", uistr_info_nfc_card_mode_chn);
	snprintf(uistr_info_nfc_vcard_mode, SHORT_STRING, "%s", uistr_info_nfc_vcard_mode_chn);
	snprintf(uistr_info_nfc_colse2reader, SHORT_STRING, "%s", uistr_info_nfc_colse2reader_chn);
	snprintf(uistr_info_nfc_testing, SHORT_STRING, "%s", uistr_info_nfc_testing_chn);
	snprintf(uistr_info_nfc_init, SHORT_STRING, "%s", uistr_info_nfc_init_chn);
	snprintf(uistr_info_nfc_vcard_removedSIM, SHORT_STRING, "%s", uistr_info_nfc_vcard_removedSIM_chn);
	snprintf(uistr_info_nfc_put_dut2reader_cm, SHORT_STRING, "%s", uistr_info_nfc_put_dut2reader_cm_chn);
	snprintf(uistr_info_nfc_put_dut2reader_vcm, SHORT_STRING, "%s", uistr_info_nfc_put_dut2reader_vcm_chn);
	/*Hotknot*/
	snprintf(uistr_hotknot, SHORT_STRING, "%s", uistr_hotknot_chn);
	snprintf(uistr_info_hotknot_master, SHORT_STRING, "%s", uistr_info_hotknot_master_chn);
	snprintf(uistr_info_hotknot_slave, SHORT_STRING, "%s", uistr_info_hotknot_slave_chn);
	snprintf(uistr_info_hotknot_fail, SHORT_STRING, "%s", uistr_info_hotknot_fail_chn);
	snprintf(uistr_info_hotknot_pass, SHORT_STRING, "%s", uistr_info_hotknot_pass_chn);
	snprintf(uistr_info_hotknot_mode_select, SHORT_STRING, "%s", uistr_info_hotknot_mode_select_chn);
	snprintf(uistr_info_hotknot_info, SHORT_STRING, "%s", uistr_info_hotknot_info_chn);

	/* IrTx */
	snprintf(uistr_info_irtx_led_test, SHORT_STRING, "%s", uistr_info_irtx_led_test_chn);
	snprintf(uistr_info_irtx_open_device, SHORT_STRING, "%s", uistr_info_irtx_open_device_chn);
	snprintf(uistr_info_irtx_call_ioctl, SHORT_STRING, "%s", uistr_info_irtx_call_ioctl_chn);
	snprintf(uistr_info_irtx_led_enable, SHORT_STRING, "%s", uistr_info_irtx_led_enable_chn);
	snprintf(uistr_info_irtx_led_disable, SHORT_STRING, "%s", uistr_info_irtx_led_disable_chn);

	/* Fingerprint */
	/* goodix begin */
	snprintf(uistr_goodix_fingerprint, SHORT_STRING, "%s", uistr_goodix_fingerprint_chn);
	/* goodix end */

	/* efuse */
	snprintf(uistr_info_efuse_test, SHORT_STRING, "%s", uistr_info_efuse_test_chn);
	snprintf(uistr_info_efuse_result, SHORT_STRING, "%s", uistr_info_efuse_result_chn);
	snprintf(uistr_info_efuse_success, SHORT_STRING, "%s", uistr_info_efuse_success_chn);
	snprintf(uistr_info_efuse_reblow, SHORT_STRING, "%s", uistr_info_efuse_reblow_chn);
	snprintf(uistr_info_efuse_broken, SHORT_STRING, "%s", uistr_info_efuse_broken_chn);
	snprintf(uistr_info_efuse_unknown, SHORT_STRING, "%s", uistr_info_efuse_unknown_chn);
	snprintf(uistr_ntc, SHORT_STRING, "%s", uistr_ntc_chn);
	snprintf(uistr_keybox, SHORT_STRING, "%s", uistr_keybox_chn);
	snprintf(uistr_dcdc, SHORT_STRING, "%s", uistr_dcdc_chn);
	snprintf(uistr_gms, SHORT_STRING, "%s", uistr_gms_chn);
	snprintf(uistr_hall, SHORT_STRING, "%s", uistr_hall_chn);
}

static void init_eng_string()
{
	snprintf(uistr_factory_mode, SHORT_STRING, "%s", uistr_factory_mode_eng);
	snprintf(uistr_full_test, SHORT_STRING, "%s", uistr_full_test_eng);
	snprintf(uistr_auto_test, SHORT_STRING, "%s", uistr_auto_test_eng);
	snprintf(uistr_item_test,   SHORT_STRING, "%s", uistr_item_test_eng);
	snprintf(uistr_test_report, SHORT_STRING, "%s", uistr_test_report_eng);
	snprintf(uistr_debug_test,  SHORT_STRING, "%s", uistr_debug_test_eng);
	snprintf(uistr_clear_flash, SHORT_STRING, "%s", uistr_clear_flash_eng);
	snprintf(uistr_clear_storage,  SHORT_STRING, "%s", uistr_clear_storage_eng);
	snprintf(uistr_version,     SHORT_STRING, "%s", uistr_version_eng);
	snprintf(uistr_reboot,      SHORT_STRING, "%s", uistr_reboot_eng);
	snprintf(uistr_pass,        SHORT_STRING, "%s", uistr_pass_eng);
	snprintf(uistr_fail,        SHORT_STRING, "%s", uistr_fail_eng);
	snprintf(uistr_retest,      SHORT_STRING, "%s", uistr_retest_eng);


	snprintf(uistr_key_back, SHORT_STRING, "%s", uistr_key_back_eng);
	snprintf(uistr_key_pass, SHORT_STRING, "%s", uistr_key_pass_eng);
	snprintf(uistr_key_fail, SHORT_STRING, "%s", uistr_key_fail_eng);
	snprintf(uistr_key_pre, SHORT_STRING, "%s", uistr_key_pre_eng);
	snprintf(uistr_key_next, SHORT_STRING, "%s", uistr_key_next_eng);
	snprintf(uistr_keys, SHORT_STRING, "%s", uistr_keys_eng);
	snprintf(uistr_jogball, SHORT_STRING, "%s", uistr_jogball_eng);
	snprintf(uistr_ofn, SHORT_STRING, "%s", uistr_ofn_eng);
	snprintf(uistr_touch, SHORT_STRING, "%s", uistr_touch_eng);
	snprintf(uistr_touch_auto, SHORT_STRING, "%s", uistr_touch_auto_eng);
	snprintf(uistr_backlight_level, SHORT_STRING, "%s", uistr_backlight_level_eng);
	snprintf(uistr_lcm_test, SHORT_STRING, "%s", uistr_lcm_test_eng);
	snprintf(uistr_lcm, SHORT_STRING, "%s", uistr_lcm_eng);
	snprintf(uistr_nand_flash, SHORT_STRING, "%s", uistr_nand_flash_eng);
	snprintf(uistr_storage, SHORT_STRING, "%s", uistr_storage_eng);
	snprintf(uistr_memory_card, SHORT_STRING, "%s", uistr_memory_card_eng);
	snprintf(uistr_sim_card, SHORT_STRING, "%s", uistr_sim_card_eng);
	snprintf(uistr_sim_detect, SHORT_STRING, "%s", uistr_sim_detect_eng);
	snprintf(uistr_sig_test, SHORT_STRING, "%s", uistr_sig_test_eng);
	snprintf(uistr_vibrator, SHORT_STRING, "%s", uistr_vibrator_eng);
	snprintf(uistr_led, SHORT_STRING, "%s", uistr_led_eng);
	snprintf(uistr_rtc, SHORT_STRING, "%s", uistr_rtc_eng);
	snprintf(uistr_system_stability, SHORT_STRING, "%s", uistr_system_stability_eng);
	snprintf(uistr_nfc, SHORT_STRING, "%s", uistr_nfc_eng);
	snprintf(uistr_cmmb, SHORT_STRING, "%s", uistr_cmmb_eng);
	snprintf(uistr_gps, SHORT_STRING, "%s", uistr_gps_eng);
	snprintf(uistr_atv, SHORT_STRING, "%s", uistr_atv_eng);
	snprintf(uistr_wifi, SHORT_STRING, "%s", uistr_wifi_eng);
	snprintf(uistr_bluetooth, SHORT_STRING, "%s", uistr_bluetooth_eng);
	snprintf(uistr_idle, SHORT_STRING, "%s", uistr_idle_eng);
	snprintf(uistr_g_sensor, SHORT_STRING, "%s", uistr_g_sensor_eng);
	snprintf(uistr_g_sensor_c, SHORT_STRING, "%s", uistr_g_sensor_c_eng);
	snprintf(uistr_bio_sensor, SHORT_STRING, "%s", uistr_bio_sensor_eng);
	snprintf(uistr_bio_sensor_c, SHORT_STRING, "%s", uistr_bio_sensor_c_eng);
	snprintf(uistr_m_sensor, SHORT_STRING, "%s", uistr_m_sensor_eng);
	snprintf(uistr_ant_test, SHORT_STRING, "%s", uistr_ant_test_eng);
	snprintf(uistr_rf_test, SHORT_STRING, "%s", uistr_rf_test_eng);
	snprintf(uistr_rf_c2k_test, SHORT_STRING, "%s", uistr_rf_c2k_test_eng);
	snprintf(uistr_als_ps, SHORT_STRING, "%s", uistr_als_ps_eng);
	snprintf(uistr_gyroscope, SHORT_STRING, "%s", uistr_gyroscope_eng);
	snprintf(uistr_gyroscope_c, SHORT_STRING, "%s", uistr_gyroscope_c_eng);
	snprintf(uistr_barometer, SHORT_STRING, "%s", uistr_barometer_eng);
	snprintf(uistr_humidity, SHORT_STRING, "%s", uistr_humidity_eng);
	snprintf(uistr_heart_monitor, SHORT_STRING, "%s", uistr_heart_monitor_eng);

	/* AP AuxADC */
	snprintf(uistr_info_ap_auxadc, SHORT_STRING, "%s", uistr_info_ap_auxadc_eng);
	snprintf(uistr_info_auxadc_chn, SHORT_STRING, "%s", uistr_info_auxadc_chn_eng);

	/*audio*/
	snprintf(uistr_info_audio_yes, SHORT_STRING, "%s", uistr_info_audio_yes_eng);
	snprintf(uistr_info_audio_no, SHORT_STRING, "%s", uistr_info_audio_no_eng);
	snprintf(uistr_info_audio_press, SHORT_STRING, "%s", uistr_info_audio_press_eng);
	snprintf(uistr_info_audio_release, SHORT_STRING, "%s", uistr_info_audio_release_eng);
	snprintf(uistr_info_audio_ringtone, SHORT_STRING, "%s", uistr_info_audio_ringtone_eng);
	snprintf(uistr_info_audio_receiver, SHORT_STRING, "%s", uistr_info_audio_receiver_eng);
	snprintf(uistr_info_audio_loopback, SHORT_STRING, "%s", uistr_info_audio_loopback_eng);
	snprintf(uistr_info_audio_loopback_phone_mic_headset, SHORT_STRING, "%s", uistr_info_audio_loopback_phone_mic_headset_eng);
	snprintf(uistr_info_audio_loopback_phone_mic_speaker, SHORT_STRING, "%s", uistr_info_audio_loopback_phone_mic_speaker_eng);
	snprintf(uistr_info_audio_loopback_headset_mic_speaker, SHORT_STRING, "%s", uistr_info_audio_loopback_headset_mic_speaker_eng);
	snprintf(uistr_info_audio_loopback_waveplayback, SHORT_STRING, "%s", uistr_info_audio_loopback_waveplayback_eng);
	snprintf(uistr_info_audio_speaker_rdc_calibration, SHORT_STRING, "%s", uistr_info_audio_speaker_rdc_calibration_eng);
	snprintf(uistr_info_audio_loopback_note, LONG_STRING, "%s", uistr_info_audio_loopback_note_eng);
	snprintf(uistr_info_audio_headset_note, SHORT_STRING, "%s", uistr_info_audio_headset_note_eng);
	snprintf(uistr_info_audio_headset_avail, SHORT_STRING, "%s", uistr_info_audio_headset_avail_eng);
	snprintf(uistr_info_audio_headset_mic_avail, SHORT_STRING, "%s", uistr_info_audio_headset_mic_avail_eng);
	snprintf(uistr_info_audio_headset_Button, SHORT_STRING, "%s", uistr_info_audio_headset_Button_eng);
	snprintf(uistr_info_audio_loopback_complete, SHORT_STRING, "%s", uistr_info_audio_loopback_complete_eng);
	snprintf(uistr_info_audio_loopback_headset_mic, SHORT_STRING, "%s", uistr_info_audio_loopback_headset_mic_eng);
	snprintf(uistr_info_audio_loopback_dualmic_mic, SHORT_STRING, "%s", uistr_info_audio_loopback_dualmic_mic_eng);
	snprintf(uistr_info_audio_loopback_dualmic_mi1, SHORT_STRING, "%s", uistr_info_audio_loopback_dualmic_mi1_eng);
	snprintf(uistr_info_audio_loopback_dualmic_mi2, SHORT_STRING, "%s", uistr_info_audio_loopback_dualmic_mi2_eng);
	snprintf(uistr_info_audio_acoustic_loopback, SHORT_STRING, "%s", uistr_info_audio_acoustic_loopback_eng);
	snprintf(uistr_info_audio_acoustic_loopback_DMNR, SHORT_STRING, "%s", uistr_info_audio_acoustic_loopback_DMNR_eng);
	snprintf(uistr_info_audio_acoustic_loopback_without_DMNR, SHORT_STRING, "%s", uistr_info_audio_acoustic_loopback_without_DMNR_eng);
	snprintf(uistr_info_audio_headset_debug, SHORT_STRING, "%s", uistr_info_audio_headset_debug_eng);
	snprintf(uistr_info_audio_receiver_debug, SHORT_STRING, "%s", uistr_info_audio_receiver_debug_eng);
	snprintf(uistr_info_audio_micbias, SHORT_STRING, "%s", uistr_info_audio_micbias_eng);
	snprintf(uistr_info_audio_micbias_on, SHORT_STRING, "%s", uistr_info_audio_micbias_on_eng);
	snprintf(uistr_info_audio_micbias_off, SHORT_STRING, "%s", uistr_info_audio_micbias_off_eng);
	snprintf(uistr_info_audio_speaker_monitor_test, SHORT_STRING, "%s", uistr_info_audio_speaker_monitor_test_eng);
	snprintf(uistr_info_audio_speaker_monitor_set_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_monitor_set_temperature_eng);
	snprintf(uistr_info_audio_speaker_add_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_add_temperature_eng);
	snprintf(uistr_info_audio_speaker_sub_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_sub_temperature_eng);
	snprintf(uistr_info_audio_speaker_exit_and_save_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_exit_and_save_temperature_eng);
	snprintf(uistr_info_audio_speaker_exit_not_save_temperature, SHORT_STRING, "%s", uistr_info_audio_speaker_exit_not_save_temperature_eng);

	snprintf(uistr_info_audio_receiver_freq_response, SHORT_STRING, "%s", uistr_info_audio_receiver_freq_response_eng);
	snprintf(uistr_info_audio_speaker_freq_response, SHORT_STRING, "%s", uistr_info_audio_speaker_freq_response_eng);
	snprintf(uistr_info_audio_receiver_thd, SHORT_STRING, "%s", uistr_info_audio_receiver_thd_eng);
	snprintf(uistr_info_audio_speaker_thd, SHORT_STRING, "%s", uistr_info_audio_speaker_thd_eng);
	snprintf(uistr_info_audio_headset_thd, SHORT_STRING, "%s", uistr_info_audio_headset_thd_eng);

	/*phone*/
	snprintf(uistr_info_audio_receiver_phone, SHORT_STRING, "%s", uistr_info_audio_receiver_phone_eng);
	snprintf(uistr_info_headset_phone, SHORT_STRING, "%s", uistr_info_headset_phone_eng);
	snprintf(uistr_info_audio_loopback_phone_mic_speaker_phone, SHORT_STRING, "%s", uistr_info_audio_loopback_phone_mic_speaker_phone_eng);
	snprintf(uistr_vibrator_phone, SHORT_STRING, "%s", uistr_vibrator_phone_eng);

	/*speaker oc*/
	snprintf(uistr_info_speaker_oc, SHORT_STRING, "%s", uistr_info_speaker_oc_eng);
	snprintf(uistr_info_speaker_oc_pass, SHORT_STRING, "%s", uistr_info_speaker_oc_pass_eng);
	snprintf(uistr_info_speaker_oc_fail, SHORT_STRING, "%s", uistr_info_speaker_oc_fail_eng);
	snprintf(uistr_info_speaker_oc_retest, SHORT_STRING, "%s", uistr_info_speaker_oc_retest_eng);

	/*headset*/
	snprintf(uistr_info_headset, SHORT_STRING, "%s", uistr_info_headset_eng);
	snprintf(uistr_info_avail, SHORT_STRING, "%s", uistr_info_avail_eng);
	snprintf(uistr_info_button, SHORT_STRING, "%s", uistr_info_button_eng);
	snprintf(uistr_info_press, SHORT_STRING, "%s", uistr_info_press_eng);
	snprintf(uistr_info_release, SHORT_STRING, "%s", uistr_info_release_eng);



	/*emergency_call*/
	snprintf(uistr_info_emergency_call, SHORT_STRING, "%s", uistr_info_emergency_call_eng);
	snprintf(uistr_info_emergency_call_not_start, SHORT_STRING, "%s", uistr_info_emergency_call_not_start_eng);
	snprintf(uistr_info_emergency_call_testing, SHORT_STRING, "%s", uistr_info_emergency_call_testing_eng);
	snprintf(uistr_info_emergency_call_success, SHORT_STRING, "%s", uistr_info_emergency_call_success_eng);
	snprintf(uistr_info_emergency_call_fail, SHORT_STRING, "%s", uistr_info_emergency_call_fail_eng);
	snprintf(uistr_info_emergency_call_success_in_modem1  , SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem1_eng);
	snprintf(uistr_info_emergency_call_fail_in_modem1, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem1_eng);
	snprintf(uistr_info_emergency_call_success_in_modem2, SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem2_eng);
	snprintf(uistr_info_emergency_call_fail_in_modem2, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem2_eng);
	snprintf(uistr_info_emergency_call_success_in_modem5, SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem5_eng);
	snprintf(uistr_info_emergency_call_fail_in_modem5, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem5_eng);

	snprintf(uistr_info_emergency_call_success_in_modem, SHORT_STRING, "%s", uistr_info_emergency_call_success_in_modem_eng);
	snprintf(uistr_info_emergency_call_fail_in_modem, SHORT_STRING, "%s", uistr_info_emergency_call_fail_in_modem_eng);

	snprintf(uistr_info_emergency_call_in_modem1, SHORT_STRING, "%s", uistr_info_emergency_call_in_modem1_eng);
	snprintf(uistr_info_emergency_call_in_modem2, SHORT_STRING, "%s", uistr_info_emergency_call_in_modem2_eng);
	snprintf(uistr_info_emergency_call_in_modem5, SHORT_STRING, "%s", uistr_info_emergency_call_in_modem5_eng);

	/*Gyro*/

	/*CMMB*/
	snprintf(uistr_info_cmmb_autoscan, SHORT_STRING, "%s", uistr_info_cmmb_autoscan_eng);
	snprintf(uistr_info_cmmb_channellist, SHORT_STRING, "%s", uistr_info_cmmb_channellist_eng);
	snprintf(uistr_info_cmmb_init_ok, SHORT_STRING, "%s", uistr_info_cmmb_init_ok_eng);
	snprintf(uistr_info_cmmb_init_fail, SHORT_STRING, "%s", uistr_info_cmmb_init_fail_eng);
	snprintf(uistr_info_cmmb_scanning, SHORT_STRING, "%s", uistr_info_cmmb_scanning_eng);
	snprintf(uistr_info_cmmb_scan_ok, SHORT_STRING, "%s", uistr_info_cmmb_scan_ok_eng);
	snprintf(uistr_info_cmmb_scan_fail, SHORT_STRING, "%s", uistr_info_cmmb_scan_fail_eng);
	snprintf(uistr_info_cmmb_tune_channel, SHORT_STRING, "%s", uistr_info_cmmb_tune_channel_eng);
	snprintf(uistr_info_cmmb_servicelist, SHORT_STRING, "%s", uistr_info_cmmb_servicelist_eng);
	snprintf(uistr_info_cmmb_selectstream, SHORT_STRING, "%s", uistr_info_cmmb_selectstream_eng);
	snprintf(uistr_info_cmmb_recording, SHORT_STRING, "%s", uistr_info_cmmb_recording_eng);
	snprintf(uistr_info_cmmb_recording_to, SHORT_STRING, "%s", uistr_info_cmmb_recording_to_eng);
	snprintf(uistr_info_cmmb_stop, SHORT_STRING, "%s", uistr_info_cmmb_stop_eng);
	snprintf(uistr_info_cmmb_stop_to, SHORT_STRING, "%s", uistr_info_cmmb_stop_to_eng);

	/*eMMC-SD*/
	snprintf(uistr_info_emmc, SHORT_STRING, "%s", uistr_info_emmc_eng);
	snprintf(uistr_info_sd, SHORT_STRING, "%s", uistr_info_sd_eng);
	snprintf(uistr_info_emmc_fat, SHORT_STRING, "%s", uistr_info_emmc_fat_eng);
	snprintf(uistr_info_emmc_format_item, SHORT_STRING, "%s", uistr_info_emmc_format_item_eng);
	snprintf(uistr_info_emmc_format_stat, SHORT_STRING, "%s", uistr_info_emmc_format_stat_eng);
	snprintf(uistr_info_emmc_format_stat_start, SHORT_STRING, "%s", uistr_info_emmc_format_stat_start_eng);
	snprintf(uistr_info_emmc_format_data_start, SHORT_STRING, "%s", uistr_info_emmc_format_data_start_eng);
	snprintf(uistr_info_reboot, SHORT_STRING, "%s", uistr_info_reboot_eng);
	snprintf(uistr_info_emmc_format_stat_success, SHORT_STRING, "%s", uistr_info_emmc_format_stat_success_eng);
	snprintf(uistr_info_emmc_format_stat_fail, SHORT_STRING, "%s", uistr_info_emmc_format_stat_fail_eng);
	snprintf(uistr_info_emmc_sd_avail, SHORT_STRING, "%s", uistr_info_emmc_sd_avail_eng);
	snprintf(uistr_info_emmc_sd_yes, SHORT_STRING, "%s", uistr_info_emmc_sd_yes_eng);
	snprintf(uistr_info_emmc_sd_no, SHORT_STRING, "%s", uistr_info_emmc_sd_no_eng);
	snprintf(uistr_info_emmc_sd_total_size, SHORT_STRING, "%s", uistr_info_emmc_sd_total_size_eng);
	snprintf(uistr_info_emmc_sd_free_size, SHORT_STRING, "%s", uistr_info_emmc_sd_free_size_eng);
	snprintf(uistr_info_emmc_sd_checksum	, SHORT_STRING, "%s", uistr_info_emmc_sd_checksum_eng);
	snprintf(uistr_info_sd1, SHORT_STRING, "%s", uistr_info_sd1_eng);
	snprintf(uistr_info_sd2, SHORT_STRING, "%s", uistr_info_sd2_eng);

	/* UFS */
	snprintf(uistr_info_ufs, SHORT_STRING, "%s", uistr_info_ufs_eng);

	/* MNTL */
	snprintf(uistr_info_mntl, SHORT_STRING, "%s", uistr_info_mntl_eng);

	/* Bluetooth */
	snprintf(uistr_info_bt_init, SHORT_STRING, "%s", uistr_info_bt_init_eng);
	snprintf(uistr_info_bt_init_fail, SHORT_STRING, "%s", uistr_info_bt_init_fail_eng);
	snprintf(uistr_info_bt_init_ok, SHORT_STRING, "%s", uistr_info_bt_init_ok_eng);
	snprintf(uistr_info_bt_inquiry_start, SHORT_STRING, "%s", uistr_info_bt_inquiry_start_eng);
	snprintf(uistr_info_bt_inquiry_1, SHORT_STRING, "%s", uistr_info_bt_inquiry_1_eng);
	snprintf(uistr_info_bt_inquiry_2, SHORT_STRING, "%s", uistr_info_bt_inquiry_2_eng);
	snprintf(uistr_info_bt_scan_1, SHORT_STRING, "%s", uistr_info_bt_scan_1_eng);
	snprintf(uistr_info_bt_scan_2, SHORT_STRING, "%s", uistr_info_bt_scan_2_eng);
	snprintf(uistr_info_bt_scan_complete, SHORT_STRING, "%s", uistr_info_bt_scan_complete_eng);
	snprintf(uistr_info_bt_no_dev, SHORT_STRING, "%s", uistr_info_bt_no_dev_eng);
	snprintf(uistr_info_bt_dev_list_end, SHORT_STRING, "%s", uistr_info_bt_dev_list_end_eng);
	snprintf(uistr_info_bt_scan_list_end, SHORT_STRING, "%s", uistr_info_bt_scan_list_end_eng);

	/*Wi-Fi*/
	snprintf(uistr_info_wifi_test_pass, SHORT_STRING, "%s", uistr_info_wifi_test_pass_eng);
	snprintf(uistr_info_wifi_test_fail, SHORT_STRING, "%s", uistr_info_wifi_test_fail_eng);
	snprintf(uistr_info_wifi_renew, SHORT_STRING, "%s", uistr_info_wifi_renew_eng);
	snprintf(uistr_info_wifi_error, SHORT_STRING, "%s", uistr_info_wifi_error_eng);
	snprintf(uistr_info_wifi_warn, SHORT_STRING, "%s", uistr_info_wifi_warn_eng);
	snprintf(uistr_info_wifi_status, SHORT_STRING, "%s", uistr_info_wifi_status_eng);
	snprintf(uistr_info_wifi_start, SHORT_STRING, "%s", uistr_info_wifi_start_eng);
	snprintf(uistr_info_wifi_init_fail, SHORT_STRING, "%s", uistr_info_wifi_init_fail_eng);
	snprintf(uistr_info_wifi_scanning, SHORT_STRING, "%s", uistr_info_wifi_scanning_eng);
	snprintf(uistr_info_wifi_timeout, SHORT_STRING, "%s", uistr_info_wifi_timeout_eng);
	snprintf(uistr_info_wifi_disconnect, SHORT_STRING, "%s", uistr_info_wifi_disconnect_eng);
	snprintf(uistr_info_wifi_connecting, SHORT_STRING, "%s", uistr_info_wifi_connecting_eng);
	snprintf(uistr_info_wifi_connected, SHORT_STRING, "%s", uistr_info_wifi_connected_eng);
	snprintf(uistr_info_wifi_unknown, SHORT_STRING, "%s", uistr_info_wifi_unknown_eng);
	snprintf(uistr_info_wifi_mode, SHORT_STRING, "%s", uistr_info_wifi_mode_eng);
	snprintf(uistr_info_wifi_infra, SHORT_STRING, "%s", uistr_info_wifi_infra_eng);
	snprintf(uistr_info_wifi_adhoc, SHORT_STRING, "%s", uistr_info_wifi_adhoc_eng);
	snprintf(uistr_info_wifi_channel, SHORT_STRING, "%s", uistr_info_wifi_channel_eng);
	snprintf(uistr_info_wifi_rssi, SHORT_STRING, "%s", uistr_info_wifi_rssi_eng);
	snprintf(uistr_info_wifi_rate, SHORT_STRING, "%s", uistr_info_wifi_rate_eng);
	snprintf(uistr_info_wifi_iface_err, SHORT_STRING, "%s", uistr_info_wifi_iface_err_eng);
	snprintf(uistr_info_wifi_fail_scan, SHORT_STRING, "%s", uistr_info_wifi_fail_scan_eng);
	snprintf(uistr_info_wifi_no_scan_res, SHORT_STRING, "%s", uistr_info_wifi_no_scan_res_eng);
	snprintf(uistr_info_wifi_connect_err, SHORT_STRING, "%s", uistr_info_wifi_connect_err_eng);
	snprintf(uistr_info_wifi_no_ap, SHORT_STRING, "%s", uistr_info_wifi_no_ap_eng);





	/* camera */
	snprintf(uistr_main_sensor, SHORT_STRING, "%s", uistr_main_sensor_eng);
	snprintf(uistr_main2_sensor, SHORT_STRING, "%s", uistr_main2_sensor_eng);
	snprintf(uistr_main3_sensor, SHORT_STRING, "%s", uistr_main3_sensor_eng);
	snprintf(uistr_main4_sensor, SHORT_STRING, "%s", uistr_main4_sensor_eng);
	snprintf(uistr_sub_sensor, SHORT_STRING, "%s", uistr_sub_sensor_eng);
	snprintf(uistr_sub2_sensor, SHORT_STRING, "%s", uistr_sub2_sensor_eng);
	snprintf(uistr_main_lens, SHORT_STRING, "%s", uistr_main_lens_eng);
	snprintf(uistr_main2_lens, SHORT_STRING, "%s", uistr_main2_lens_eng);
	snprintf(uistr_main3_lens, SHORT_STRING, "%s", uistr_main3_lens_eng);
	snprintf(uistr_sub_lens, SHORT_STRING, "%s", uistr_sub_lens_eng);
	snprintf(uistr_camera_prv_cap_strobe, SHORT_STRING, "%s", uistr_camera_prv_cap_strobe_eng);
	snprintf(uistr_camera_prv_strobe, SHORT_STRING, "%s", uistr_camera_prv_strobe_eng);
	snprintf(uistr_camera_prv_cap, SHORT_STRING, "%s", uistr_camera_prv_cap_eng);
	snprintf(uistr_camera_back, SHORT_STRING, "%s", uistr_camera_back_eng);
	snprintf(uistr_camera_capture, SHORT_STRING, "%s", uistr_camera_capture_eng);
	snprintf(uistr_camera_preview, SHORT_STRING, "%s", uistr_camera_preview_eng);
	snprintf(uistr_strobe, SHORT_STRING, "%s", uistr_strobe_eng);


	/* USB */
	snprintf(uistr_info_usb_connect, SHORT_STRING, "%s", uistr_info_usb_connect_eng);
	snprintf(uistr_info_usb_disconnect, SHORT_STRING, "%s", uistr_info_usb_disconnect_eng);

	/* battery&charging */
	snprintf(uistr_info_title_battery_charger, SHORT_STRING, "%s", uistr_info_title_battery_charger_eng);
	snprintf(uistr_info_title_ac_charger, SHORT_STRING, "%s", uistr_info_title_ac_charger_eng);
	snprintf(uistr_info_title_usb_charger, SHORT_STRING, "%s", uistr_info_title_usb_charger_eng);
	snprintf(uistr_info_title_battery_yes, SHORT_STRING, "%s", uistr_info_title_battery_yes_eng);
	snprintf(uistr_info_title_battery_no, SHORT_STRING, "%s", uistr_info_title_battery_no_eng);
	snprintf(uistr_info_title_battery_connect, SHORT_STRING, "%s", uistr_info_title_battery_connect_eng);
	snprintf(uistr_info_title_battery_no_connect, SHORT_STRING, "%s", uistr_info_title_battery_no_connect_eng);
	snprintf(uistr_info_title_battery_cal, SHORT_STRING, "%s", uistr_info_title_battery_cal_eng);
	snprintf(uistr_info_title_battery_val, SHORT_STRING, "%s", uistr_info_title_battery_val_eng);
	snprintf(uistr_info_title_battery_temp, SHORT_STRING, "%s", uistr_info_title_battery_temp_eng);
	snprintf(uistr_info_title_battery_chr, SHORT_STRING, "%s", uistr_info_title_battery_chr_eng);
	snprintf(uistr_info_title_battery_chr_val, SHORT_STRING, "%s", uistr_info_title_battery_chr_val_eng);
	snprintf(uistr_info_title_battery_chr_current, SHORT_STRING, "%s", uistr_info_title_battery_chr_current_eng);
	snprintf(uistr_info_title_battery_ad32, SHORT_STRING, "%s", uistr_info_title_battery_ad32_eng);
	snprintf(uistr_info_title_battery_ad42, SHORT_STRING, "%s", uistr_info_title_battery_ad42_eng);
	snprintf(uistr_info_title_battery_curad, SHORT_STRING, "%s", uistr_info_title_battery_curad_eng);
	snprintf(uistr_info_title_battery_fg_cur, SHORT_STRING, "%s", uistr_info_title_battery_fg_cur_eng);
	snprintf(uistr_info_title_battery_pmic_chip, SHORT_STRING, "%s", uistr_info_title_battery_pmic_chip_eng);
	snprintf(uistr_info_title_battery_mv, SHORT_STRING, "%s", uistr_info_title_battery_mv_eng);
	snprintf(uistr_info_title_battery_ma, SHORT_STRING, "%s", uistr_info_title_battery_ma_eng);
	snprintf(uistr_info_title_battery_c, SHORT_STRING, "%s", uistr_info_title_battery_c_eng);

	/* slave charger */
	snprintf(uistr_info_title_slave_charger, SHORT_STRING, "%s", uistr_info_title_slave_charger_eng);
	snprintf(uistr_info_title_slave_charger_connect, SHORT_STRING, "%s", uistr_info_title_slave_charger_connect_eng);
	snprintf(uistr_info_title_slave_charger_no_connect, SHORT_STRING, "%s", uistr_info_title_slave_charger_no_connect_eng);

	/* ext buck */
	snprintf(uistr_info_title_ext_buck_item, SHORT_STRING, "%s", uistr_info_title_ext_buck_item_eng);
	snprintf(uistr_info_title_ext_buck_chip, SHORT_STRING, "%s", uistr_info_title_ext_buck_chip_eng);
	snprintf(uistr_info_title_ext_buck2_chip, SHORT_STRING, "%s", uistr_info_title_ext_buck2_chip_eng);
	snprintf(uistr_info_title_ext_buck3_chip, SHORT_STRING, "%s", uistr_info_title_ext_buck3_chip_eng);
	snprintf(uistr_info_title_ext_buck_connect, SHORT_STRING, "%s", uistr_info_title_ext_buck_connect_eng);
	snprintf(uistr_info_title_ext_buck_no_connect, SHORT_STRING, "%s", uistr_info_title_ext_buck_no_connect_eng);

	/* ext vbat boost */
	snprintf(uistr_info_title_ext_vbat_boost_item, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_item_eng);
	snprintf(uistr_info_title_ext_vbat_boost_chip, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_chip_eng);
	snprintf(uistr_info_title_ext_vbat_boost_connect, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_connect_eng);
	snprintf(uistr_info_title_ext_vbat_boost_no_connect, SHORT_STRING, "%s", uistr_info_title_ext_vbat_boost_no_connect_eng);

	/* OTG */
	snprintf(uistr_info_otg_status, SHORT_STRING, "%s", uistr_info_otg_status_eng);
	snprintf(uistr_info_otg_status_device, SHORT_STRING, "%s", uistr_info_otg_status_device_eng);
	snprintf(uistr_info_otg_status_host, SHORT_STRING, "%s", uistr_info_otg_status_host_eng);

	/* LED */
	snprintf(uistr_info_nled_test, SHORT_STRING, "%s", uistr_info_nled_test_eng);
	snprintf(uistr_info_keypad_led_test, SHORT_STRING, "%s", uistr_info_keypad_led_test_eng);

	/* Backlight */
	snprintf(uistr_info_show_test_images, SHORT_STRING, "%s", uistr_info_show_test_images_eng);
	snprintf(uistr_info_change_contrast, SHORT_STRING, "%s", uistr_info_change_contrast_eng);

	/* System Stability */
	snprintf(uistr_info_stress_test_result, SHORT_STRING, "%s", uistr_info_stress_test_result_eng);

	/* GPS */
	snprintf(uistr_info_gps_hot_restart, SHORT_STRING, "%s", uistr_info_gps_hot_restart_eng);
	snprintf(uistr_info_gps_cold_restart, SHORT_STRING, "%s", uistr_info_gps_cold_restart_eng);
	snprintf(uistr_info_gps_error, SHORT_STRING, "%s", uistr_info_gps_error_eng);
	snprintf(uistr_info_gps_fixed, SHORT_STRING, "%s", uistr_info_gps_fixed_eng);
	snprintf(uistr_info_gps_ttff, SHORT_STRING, "%s", uistr_info_gps_ttff_eng);
	snprintf(uistr_info_gps_svid, SHORT_STRING, "%s", uistr_info_gps_svid_eng);
	snprintf(uistr_info_gps_init, SHORT_STRING, "%s", uistr_info_gps_init_eng);

	/*NAND*/
	snprintf(uistr_info_nand_clear_flash, SHORT_STRING, "%s", uistr_info_nand_clear_flash_eng);
	snprintf(uistr_info_nand_Manufacturer_ID, SHORT_STRING, "%s", uistr_info_nand_Manufacturer_ID_eng);
	snprintf(uistr_info_nand_Device_ID, SHORT_STRING, "%s", uistr_info_nand_Device_ID_eng);
	snprintf(uistr_info_nand_R_W_tests_result, SHORT_STRING, "%s", uistr_info_nand_R_W_tests_result_eng);
	snprintf(uistr_info_nand_erase_info, LONG_STRING, "%s", uistr_info_nand_erase_info_eng);

	/*mATV*/
	snprintf(uistr_info_atv_autoscan, SHORT_STRING, "%s", uistr_info_atv_autoscan_eng);
	snprintf(uistr_info_atv_channellist, SHORT_STRING, "%s", uistr_info_atv_channellist_eng);
	snprintf(uistr_info_atv_init_ok, SHORT_STRING, "%s", uistr_info_atv_init_ok_eng);
	snprintf(uistr_info_atv_initizling, SHORT_STRING, "%s", uistr_info_atv_initizling_eng);
	snprintf(uistr_info_atv_init_fail, SHORT_STRING, "%s", uistr_info_atv_init_fail_eng);
	snprintf(uistr_info_atv_previewCH, SHORT_STRING, "%s", uistr_info_atv_previewCH_eng);
	snprintf(uistr_info_atv_switchCH , SHORT_STRING, "%s", uistr_info_atv_switchCH_eng);
	snprintf(uistr_info_atv_refreshCH, SHORT_STRING, "%s", uistr_info_atv_refreshCH_eng);
	snprintf(uistr_info_atv_CH, SHORT_STRING, "%s", uistr_info_atv_CH_eng);
	snprintf(Country_AFGHANISTAN, SHORT_STRING, "%s", Country_AFGHANISTAN_eng);
	snprintf(Country_ARGENTINA, SHORT_STRING, "%s", Country_ARGENTINA_eng);
	snprintf(Country_AUSTRALIA, SHORT_STRING, "%s", Country_AUSTRALIA_eng);
	snprintf(Country_BRAZIL, SHORT_STRING, "%s", Country_BRAZIL_eng);
	snprintf(Country_BURMA, SHORT_STRING, "%s", Country_BURMA_eng);
	snprintf(Country_CAMBODIA, SHORT_STRING, "%s", Country_CAMBODIA_eng);
	snprintf(Country_CANADA, SHORT_STRING, "%s", Country_CANADA_eng);
	snprintf(Country_CHILE, SHORT_STRING, "%s", Country_CHILE_eng);
	snprintf(Country_CHINA, SHORT_STRING, "%s", Country_CHINA_eng);
	snprintf(Country_CHINA_HONGKONG, SHORT_STRING, "%s", Country_CHINA_HONGKONG_eng);
	snprintf(Country_CHINA_SHENZHEN, SHORT_STRING, "%s", Country_CHINA_SHENZHEN_eng);
	snprintf(Country_EUROPE_EASTERN, SHORT_STRING, "%s", Country_EUROPE_EASTERN_eng);
	snprintf(Country_EUROPE_WESTERN, SHORT_STRING, "%s", Country_EUROPE_WESTERN_eng);
	snprintf(Country_FRANCE, SHORT_STRING, "%s", Country_FRANCE_eng);
	snprintf(Country_FRENCH_COLONIE, SHORT_STRING, "%s", Country_FRENCH_COLONIE_eng);
	snprintf(Country_INDIA, SHORT_STRING, "%s", Country_INDIA_eng);
	snprintf(Country_INDONESIA, SHORT_STRING, "%s", Country_INDONESIA_eng);
	snprintf(Country_IRAN, SHORT_STRING, "%s", Country_IRAN_eng);
	snprintf(Country_ITALY, SHORT_STRING, "%s", Country_ITALY_eng);
	snprintf(Country_JAPAN, SHORT_STRING, "%s", Country_JAPAN_eng);
	snprintf(Country_KOREA, SHORT_STRING, "%s", Country_KOREA_eng);
	snprintf(Country_LAOS, SHORT_STRING, "%s", Country_LAOS_eng);
	snprintf(Country_MALAYSIA, SHORT_STRING, "%s", Country_MALAYSIA_eng);
	snprintf(Country_MEXICO, SHORT_STRING, "%s", Country_MEXICO_eng);
	snprintf(Country_NEWZEALAND, SHORT_STRING, "%s", Country_NEWZEALAND_eng);
	snprintf(Country_PAKISTAN, SHORT_STRING, "%s", Country_PAKISTAN_eng);
	snprintf(Country_PARAGUAY, SHORT_STRING, "%s", Country_PARAGUAY_eng);
	snprintf(Country_PHILIPPINES, SHORT_STRING, "%s", Country_PHILIPPINES_eng);
	snprintf(Country_PORTUGAL, SHORT_STRING, "%s", Country_PORTUGAL_eng);
	snprintf(Country_RUSSIA, SHORT_STRING, "%s", Country_RUSSIA_eng);
	snprintf(Country_SINGAPORE, SHORT_STRING, "%s", Country_SINGAPORE_eng);
	snprintf(Country_SOUTHAFRICA, SHORT_STRING, "%s", Country_SOUTHAFRICA_eng);
	snprintf(Country_SPAIN, SHORT_STRING, "%s", Country_SPAIN_eng);
	snprintf(Country_TAIWAN, SHORT_STRING, "%s", Country_TAIWAN_eng);
	snprintf(Country_THAILAND, SHORT_STRING, "%s", Country_THAILAND_eng);
	snprintf(Country_TURKEY, SHORT_STRING, "%s", Country_TURKEY_eng);
	snprintf(Country_UNITED_ARAB_EMIRATES, SHORT_STRING, "%s", Country_UNITED_ARAB_EMIRATES_eng);
	snprintf(Country_UNITED_KINGDOM, SHORT_STRING, "%s", Country_UNITED_KINGDOM_eng);
	snprintf(Country_USA, SHORT_STRING, "%s", Country_USA_eng);
	snprintf(Country_URUGUAY, SHORT_STRING, "%s", Country_URUGUAY_eng);
	snprintf(Country_VENEZUELA, SHORT_STRING, "%s", Country_VENEZUELA_eng);
	snprintf(Country_VIETNAM, SHORT_STRING, "%s", Country_VIETNAM_eng);
	snprintf(Country_IRELAND, SHORT_STRING, "%s", Country_IRELAND_eng);
	snprintf(Country_MOROCCO, SHORT_STRING, "%s", Country_MOROCCO_eng);
	snprintf(Country_BANGLADESH, SHORT_STRING, "%s", Country_BANGLADESH_eng);
	snprintf(Country_EXIT, SHORT_STRING, "%s", Country_EXIT_eng);


	/* TV out */

	snprintf(uistr_info_tvout_plugin, SHORT_STRING, "%s", uistr_info_tvout_plugin_eng);
	snprintf(uistr_info_tvout_checkifplugin, SHORT_STRING, "%s", uistr_info_tvout_checkifplugin_eng);
	snprintf(uistr_info_tvout_notplugin, SHORT_STRING, "%s", uistr_info_tvout_notplugin_eng);
	snprintf(uistr_info_tvout_item, SHORT_STRING, "%s", uistr_info_tvout_item_eng);


	/* SIM detect */
	snprintf(uistr_info_sim_detect_item_sim_1, SHORT_STRING, "%s", uistr_info_sim_detect_item_sim_1_eng);
	snprintf(uistr_info_sim_detect_item_sim_2, SHORT_STRING, "%s", uistr_info_sim_detect_item_sim_2_eng);
	snprintf(uistr_info_sim_detect_item_pass, SHORT_STRING, "%s", uistr_info_sim_detect_item_pass_eng);
	snprintf(uistr_info_sim_detect_item_fail, SHORT_STRING, "%s", uistr_info_sim_detect_item_fail_eng);
	snprintf(uistr_info_sim_detect_test_result, SHORT_STRING, "%s", uistr_info_sim_detect_test_result_eng);
	snprintf(uistr_info_sim_detect_result_pass, SHORT_STRING, "%s", uistr_info_sim_detect_result_pass_eng);
	snprintf(uistr_info_sim_detect_result_fail, SHORT_STRING, "%s", uistr_info_sim_detect_result_fail_eng);

	/* FM Radio */
	snprintf(uistr_info_fmr_title, SHORT_STRING, "%s", uistr_info_fmr_title_eng);
	snprintf(uistr_info_fmr_no_headset_warning, SHORT_STRING, "%s", uistr_info_fmr_no_headset_warning_eng);
	snprintf(uistr_info_fmr_open_fail, SHORT_STRING, "%s", uistr_info_fmr_open_fail_eng);
	snprintf(uistr_info_fmr_poweron_fail, SHORT_STRING, "%s", uistr_info_fmr_poweron_fail_eng);
	snprintf(uistr_info_fmr_mute_fail, SHORT_STRING, "%s", uistr_info_fmr_mute_fail_eng);
	snprintf(uistr_info_fmr_poweron_ok, SHORT_STRING, "%s", uistr_info_fmr_poweron_ok_eng);
	snprintf(uistr_info_fmr_setfreq, SHORT_STRING, "%s", uistr_info_fmr_setfreq_eng);
	snprintf(uistr_info_fmr_mhz, SHORT_STRING, "%s", uistr_info_fmr_mhz_eng);
	snprintf(uistr_info_fmr_fail, SHORT_STRING, "%s", uistr_info_fmr_fail_eng);
	snprintf(uistr_info_fmr_success, SHORT_STRING, "%s", uistr_info_fmr_success_eng);
	snprintf(uistr_info_fmr_rssi, SHORT_STRING, "%s", uistr_info_fmr_rssi_eng);
	snprintf(uistr_info_fmr_freq0, SHORT_STRING, "%s", uistr_info_fmr_freq0_eng);
	snprintf(uistr_info_fmr_freq1, SHORT_STRING, "%s", uistr_info_fmr_freq1_eng);
	snprintf(uistr_info_fmr_freq2, SHORT_STRING, "%s", uistr_info_fmr_freq2_eng);
	snprintf(uistr_info_fmr_freq3, SHORT_STRING, "%s", uistr_info_fmr_freq3_eng);
	snprintf(uistr_info_fmr_pass, SHORT_STRING, "%s", uistr_info_fmr_pass_eng);
	snprintf(uistr_info_fmr_failed, SHORT_STRING, "%s", uistr_info_fmr_failed_eng);

	/* FM Transmitter */
	snprintf(uistr_info_fmt_title, SHORT_STRING, "%s", uistr_info_fmt_title_eng);
	snprintf(uistr_info_fmt_open_fail, SHORT_STRING, "%s", uistr_info_fmt_open_fail_eng);
	snprintf(uistr_info_fmt_poweron_fail, SHORT_STRING, "%s", uistr_info_fmt_poweron_fail_eng);
	snprintf(uistr_info_fmt_poweron_ok, SHORT_STRING, "%s", uistr_info_fmt_poweron_ok_eng);
	snprintf(uistr_info_fmt_audio_out, SHORT_STRING, "%s", uistr_info_fmt_audio_out_eng);
	snprintf(uistr_info_fmt_setfreq, SHORT_STRING, "%s", uistr_info_fmt_setfreq_eng);
	snprintf(uistr_info_fmt_mhz, SHORT_STRING, "%s", uistr_info_fmt_mhz_eng);
	snprintf(uistr_info_fmt_fail, SHORT_STRING, "%s", uistr_info_fmt_fail_eng);
	snprintf(uistr_info_fmt_success, SHORT_STRING, "%s", uistr_info_fmt_success_eng);
	snprintf(uistr_info_fmt_check_rds_fail, SHORT_STRING, "%s", uistr_info_fmt_check_rds_fail_eng);
	snprintf(uistr_info_fmt_enable_rds_fail, SHORT_STRING, "%s", uistr_info_fmt_enable_rds_fail_eng);
	snprintf(uistr_info_fmt_set_rds_fail, SHORT_STRING, "%s", uistr_info_fmt_set_rds_fail_eng);
	snprintf(uistr_info_fmt_rds, SHORT_STRING, "%s", uistr_info_fmt_rds_eng);
	snprintf(uistr_info_fmt_freq0, SHORT_STRING, "%s", uistr_info_fmt_freq0_eng);
	snprintf(uistr_info_fmt_freq1, SHORT_STRING, "%s", uistr_info_fmt_freq1_eng);
	snprintf(uistr_info_fmt_freq2, SHORT_STRING, "%s", uistr_info_fmt_freq2_eng);
	snprintf(uistr_info_fmt_freq3, SHORT_STRING, "%s", uistr_info_fmt_freq3_eng);
	snprintf(uistr_info_fmt_freq4, SHORT_STRING, "%s", uistr_info_fmt_freq4_eng);
	snprintf(uistr_info_fmt_freq5, SHORT_STRING, "%s", uistr_info_fmt_freq5_eng);
	snprintf(uistr_info_fmt_freq6, SHORT_STRING, "%s", uistr_info_fmt_freq6_eng);
	snprintf(uistr_info_fmt_freq7, SHORT_STRING, "%s", uistr_info_fmt_freq7_eng);
	snprintf(uistr_info_fmt_freq8, SHORT_STRING, "%s", uistr_info_fmt_freq8_eng);
	snprintf(uistr_info_fmt_freq9, SHORT_STRING, "%s", uistr_info_fmt_freq9_eng);
	snprintf(uistr_info_fmt_pass, SHORT_STRING, "%s", uistr_info_fmt_pass_eng);
	snprintf(uistr_info_fmt_failed, SHORT_STRING, "%s", uistr_info_fmt_failed_eng);
	/*Wi-Fi*/
	snprintf(uistr_info_wifi_test_fail, SHORT_STRING, "%s", uistr_info_wifi_test_fail_eng);
	snprintf(uistr_info_wifi_renew, SHORT_STRING, "%s", uistr_info_wifi_renew_eng);
	snprintf(uistr_info_wifi_error, SHORT_STRING, "%s", uistr_info_wifi_error_eng);
	snprintf(uistr_info_wifi_warn, SHORT_STRING, "%s", uistr_info_wifi_warn_eng);
	snprintf(uistr_info_wifi_status, SHORT_STRING, "%s", uistr_info_wifi_status_eng);
	snprintf(uistr_info_wifi_start, SHORT_STRING, "%s", uistr_info_wifi_start_eng);
	snprintf(uistr_info_wifi_init_fail, SHORT_STRING, "%s", uistr_info_wifi_init_fail_eng);
	snprintf(uistr_info_wifi_scanning, SHORT_STRING, "%s", uistr_info_wifi_scanning_eng);
	snprintf(uistr_info_wifi_timeout, SHORT_STRING, "%s", uistr_info_wifi_timeout_eng);
	snprintf(uistr_info_wifi_disconnect, SHORT_STRING, "%s", uistr_info_wifi_disconnect_eng);
	snprintf(uistr_info_wifi_connecting, SHORT_STRING, "%s", uistr_info_wifi_connecting_eng);
	snprintf(uistr_info_wifi_connected, SHORT_STRING, "%s", uistr_info_wifi_connected_eng);
	snprintf(uistr_info_wifi_unknown, SHORT_STRING, "%s", uistr_info_wifi_unknown_eng);
	snprintf(uistr_info_wifi_mode, SHORT_STRING, "%s", uistr_info_wifi_mode_eng);
	snprintf(uistr_info_wifi_infra, SHORT_STRING, "%s", uistr_info_wifi_infra_eng);
	snprintf(uistr_info_wifi_adhoc, SHORT_STRING, "%s", uistr_info_wifi_adhoc_eng);
	snprintf(uistr_info_wifi_channel, SHORT_STRING, "%s", uistr_info_wifi_channel_eng);
	snprintf(uistr_info_wifi_rssi, SHORT_STRING, "%s", uistr_info_wifi_rssi_eng);
	snprintf(uistr_info_wifi_rate, SHORT_STRING, "%s", uistr_info_wifi_rate_eng);

	/* Touchpanel */
	snprintf(uistr_info_touch_ctp_main, SHORT_STRING, "%s", uistr_info_touch_ctp_main_eng);
	snprintf(uistr_info_touch_calibration, SHORT_STRING, "%s", uistr_info_touch_calibration_eng);
	snprintf(uistr_info_touch_rtp_linearity, SHORT_STRING, "%s", uistr_info_touch_rtp_linearity_eng);
	snprintf(uistr_info_touch_ctp_linearity, SHORT_STRING, "%s", uistr_info_touch_ctp_linearity_eng);
	snprintf(uistr_info_touch_rtp_accuracy, SHORT_STRING, "%s", uistr_info_touch_rtp_accuracy_eng);
	snprintf(uistr_info_touch_ctp_accuracy, SHORT_STRING, "%s", uistr_info_touch_ctp_accuracy_eng);
	snprintf(uistr_info_touch_sensitivity, SHORT_STRING, "%s", uistr_info_touch_sensitivity_eng);
	snprintf(uistr_info_touch_deadzone, SHORT_STRING, "%s", uistr_info_touch_deadzone_eng);
	snprintf(uistr_info_touch_zoom, SHORT_STRING, "%s", uistr_info_touch_zoom_eng);
	snprintf(uistr_info_touch_freemode, SHORT_STRING, "%s", uistr_info_touch_freemode_eng);
	snprintf(uistr_info_touch_start, SHORT_STRING, "%s", uistr_info_touch_start_eng);
	snprintf(uistr_info_touch_red_cross, SHORT_STRING, "%s", uistr_info_touch_red_cross_eng);
	snprintf(uistr_info_touch_pass_continue, SHORT_STRING, "%s", uistr_info_touch_pass_continue_eng);

	/* OFN */
	snprintf(uistr_info_ofn_fail, SHORT_STRING, "%s", uistr_info_ofn_fail_eng);
	snprintf(uistr_info_ofn_pass, SHORT_STRING, "%s", uistr_info_ofn_pass_eng);
	snprintf(uistr_info_ofn_back, SHORT_STRING, "%s", uistr_info_ofn_back_eng);
	snprintf(uistr_info_ofn_free_mode_item, SHORT_STRING, "%s", uistr_info_ofn_free_mode_item_eng);
	snprintf(uistr_info_ofn_pass_item, SHORT_STRING, "%s", uistr_info_ofn_pass_item_eng);
	snprintf(uistr_info_ofn_fail_item, SHORT_STRING, "%s", uistr_info_ofn_fail_item_eng);
	snprintf(uistr_info_ofn_return_item, SHORT_STRING, "%s", uistr_info_ofn_return_item_eng);

	/* G/M-Sensor & ALS/PS & Gyroscope */
	snprintf(uistr_info_sensor_back, SHORT_STRING, "%s", uistr_info_sensor_back_eng);
	snprintf(uistr_info_sensor_pass, SHORT_STRING, "%s", uistr_info_sensor_pass_eng);
	snprintf(uistr_info_sensor_init_fail, SHORT_STRING, "%s", uistr_info_sensor_init_fail_eng);
	snprintf(uistr_info_sensor_initializing, SHORT_STRING, "%s", uistr_info_sensor_initializing_eng);
	snprintf(uistr_info_sensor_fail, SHORT_STRING, "%s", uistr_info_sensor_fail_eng);

	snprintf(uistr_info_g_sensor_testing, SHORT_STRING, "%s", uistr_info_g_sensor_testing_eng);
	snprintf(uistr_info_g_sensor_unknow, SHORT_STRING, "%s", uistr_info_g_sensor_unknow_eng);
	snprintf(uistr_info_g_sensor_doing, SHORT_STRING, "%s", uistr_info_g_sensor_doing_eng);
	snprintf(uistr_info_g_sensor_done, SHORT_STRING, "%s", uistr_info_g_sensor_done_eng);
	snprintf(uistr_info_g_sensor_max, SHORT_STRING, "%s", uistr_info_g_sensor_max_eng);
	snprintf(uistr_info_g_sensor_min, SHORT_STRING, "%s", uistr_info_g_sensor_min_eng);
	snprintf(uistr_info_g_sensor_range, SHORT_STRING, "%s", uistr_info_g_sensor_range_eng);
	snprintf(uistr_info_g_sensor_selftest, SHORT_STRING, "%s", uistr_info_g_sensor_selftest_eng);
	snprintf(uistr_info_g_sensor_statistic, SHORT_STRING, "%s", uistr_info_g_sensor_statistic_eng);
	snprintf(uistr_info_g_sensor_notsupport, SHORT_STRING, "%s", uistr_info_g_sensor_notsupport_eng);
	snprintf(uistr_info_g_sensor_avg, SHORT_STRING, "%s", uistr_info_g_sensor_avg_eng);
	snprintf(uistr_info_g_sensor_std, SHORT_STRING, "%s", uistr_info_g_sensor_std_eng);

	snprintf(uistr_info_m_sensor_self, SHORT_STRING, "%s", uistr_info_m_sensor_self_eng);
	snprintf(uistr_info_m_sensor_notsupport, SHORT_STRING, "%s", uistr_info_m_sensor_notsupport_eng);
	snprintf(uistr_info_m_sensor_ok, SHORT_STRING, "%s", uistr_info_m_sensor_ok_eng);
	snprintf(uistr_info_m_sensor_testing, SHORT_STRING, "%s", uistr_info_m_sensor_testing_eng);
	snprintf(uistr_info_m_sensor_fail, SHORT_STRING, "%s", uistr_info_m_sensor_fail_eng);
	snprintf(uistr_info_m_sensor_status, SHORT_STRING, "%s", uistr_info_m_sensor_status_eng);
	snprintf(uistr_info_m_sensor_data, SHORT_STRING, "%s", uistr_info_m_sensor_data_eng);

	snprintf(uistr_info_sensor_cali_clear, SHORT_STRING, "%s", uistr_info_sensor_cali_clear_eng);
	snprintf(uistr_info_sensor_cali_do, SHORT_STRING, "%s", uistr_info_sensor_cali_do_eng);
	snprintf(uistr_info_sensor_cali_do_20, SHORT_STRING, "%s", uistr_info_sensor_cali_do_20_eng);
	snprintf(uistr_info_sensor_cali_do_40, SHORT_STRING, "%s", uistr_info_sensor_cali_do_40_eng);
	snprintf(uistr_info_sensor_cali_ok, SHORT_STRING, "%s", uistr_info_sensor_cali_ok_eng);
	snprintf(uistr_info_sensor_cali_fail, SHORT_STRING, "%s", uistr_info_sensor_cali_fail_eng);
	snprintf(uistr_info_sensor_cali_ongoing, SHORT_STRING, "%s", uistr_info_sensor_cali_ongoing_eng);

	snprintf(uistr_info_sensor_alsps_thres_high, SHORT_STRING, "%s", uistr_info_sensor_alsps_thres_high_eng);
	snprintf(uistr_info_sensor_alsps_thres_low, SHORT_STRING, "%s", uistr_info_sensor_alsps_thres_low_eng);
	snprintf(uistr_info_sensor_alsps_check_command, SHORT_STRING, "%s", uistr_info_sensor_alsps_check_command_eng);
	snprintf(uistr_info_sensor_alsps_result, SHORT_STRING, "%s", uistr_info_sensor_alsps_result_eng);

	snprintf(uistr_info_sensor_pressure_value, SHORT_STRING, "%s", uistr_info_sensor_pressure_value_eng);
	snprintf(uistr_info_sensor_temperature_value, SHORT_STRING, "%s", uistr_info_sensor_temperature_value_eng);

	snprintf(uistr_info_bio_sensor_max, SHORT_STRING, "%s", uistr_info_bio_sensor_max_eng);
	snprintf(uistr_info_bio_sensor_min, SHORT_STRING, "%s", uistr_info_bio_sensor_min_eng);
	snprintf(uistr_info_bio_sensor_thres, SHORT_STRING, "%s", uistr_info_bio_sensor_thres_eng);
	snprintf(uistr_info_bio_sensor_result, SHORT_STRING, "%s", uistr_info_bio_sensor_result_eng);
	/*test report*/
	snprintf(uistr_info_test_report_back, SHORT_STRING, "%s", uistr_info_test_report_back_eng);

	snprintf(uistr_info_detect_sim1, SHORT_STRING, "%s", uistr_info_detect_sim1_eng);
	snprintf(uistr_info_detect_sim2, SHORT_STRING, "%s", uistr_info_detect_sim2_eng);
	snprintf(uistr_info_test_pass, SHORT_STRING, "%s", uistr_info_test_pass_eng);
	snprintf(uistr_info_test_fail, SHORT_STRING, "%s", uistr_info_test_fail_eng);
	snprintf(uistr_info_detect_sim, SHORT_STRING, "%s", uistr_info_detect_sim_eng);
	snprintf(uistr_info_yes, SHORT_STRING, "%s", uistr_info_yes_eng);
	snprintf(uistr_info_no, SHORT_STRING, "%s", uistr_info_no_eng);
	snprintf(uistr_info_fail, SHORT_STRING, "%s", uistr_info_fail_eng);
	snprintf(uistr_info_pass, SHORT_STRING, "%s", uistr_info_pass_eng);

	/* BTS */
	snprintf(uistr_bts, SHORT_STRING, "%s", uistr_bts_eng);

	/* NFC */
	snprintf(uistr_info_nfc_swp_test, SHORT_STRING, "%s", uistr_info_nfc_swp_test_eng);
	snprintf(uistr_info_nfc_tag_dep, SHORT_STRING, "%s", uistr_info_nfc_tag_dep_eng);
	snprintf(uistr_info_nfc_card_mode, SHORT_STRING, "%s", uistr_info_nfc_card_mode_eng);
	snprintf(uistr_info_nfc_vcard_mode, SHORT_STRING, "%s", uistr_info_nfc_vcard_mode_eng);
	snprintf(uistr_info_nfc_colse2reader, SHORT_STRING, "%s", uistr_info_nfc_colse2reader_eng);
	snprintf(uistr_info_nfc_testing, SHORT_STRING, "%s", uistr_info_nfc_testing_eng);
	snprintf(uistr_info_nfc_init, SHORT_STRING, "%s", uistr_info_nfc_init_eng);
	snprintf(uistr_info_nfc_vcard_removedSIM, SHORT_STRING, "%s", uistr_info_nfc_vcard_removedSIM_eng);
	snprintf(uistr_info_nfc_put_dut2reader_cm, SHORT_STRING, "%s", uistr_info_nfc_put_dut2reader_cm_eng);
	snprintf(uistr_info_nfc_put_dut2reader_vcm, SHORT_STRING, "%s", uistr_info_nfc_put_dut2reader_vcm_eng);
	/*Hotknot*/
	snprintf(uistr_hotknot, SHORT_STRING, "%s", uistr_hotknot_eng);
	snprintf(uistr_info_hotknot_master, SHORT_STRING, "%s", uistr_info_hotknot_master_eng);
	snprintf(uistr_info_hotknot_slave, SHORT_STRING, "%s", uistr_info_hotknot_slave_eng);
	snprintf(uistr_info_hotknot_fail, SHORT_STRING, "%s", uistr_info_hotknot_fail_eng);
	snprintf(uistr_info_hotknot_pass, SHORT_STRING, "%s", uistr_info_hotknot_pass_eng);
	snprintf(uistr_info_hotknot_mode_select, SHORT_STRING, "%s", uistr_info_hotknot_mode_select_eng);
	snprintf(uistr_info_hotknot_info, SHORT_STRING, "%s", uistr_info_hotknot_info_eng);

	/* IrTx */
	snprintf(uistr_info_irtx_led_test, SHORT_STRING, "%s", uistr_info_irtx_led_test_eng);
	snprintf(uistr_info_irtx_open_device, SHORT_STRING, "%s", uistr_info_irtx_open_device_eng);
	snprintf(uistr_info_irtx_call_ioctl, SHORT_STRING, "%s", uistr_info_irtx_call_ioctl_eng);
	snprintf(uistr_info_irtx_led_enable, SHORT_STRING, "%s", uistr_info_irtx_led_enable_eng);
	snprintf(uistr_info_irtx_led_disable, SHORT_STRING, "%s", uistr_info_irtx_led_disable_eng);

	/* Fingerprint */
	/* goodix begin */
	snprintf(uistr_goodix_fingerprint, SHORT_STRING, "%s", uistr_goodix_fingerprint_eng);
	/* goodix end */

	/* efuse */
	snprintf(uistr_info_efuse_test, SHORT_STRING, "%s", uistr_info_efuse_test_eng);
	snprintf(uistr_info_efuse_result, SHORT_STRING, "%s", uistr_info_efuse_result_eng);
	snprintf(uistr_info_efuse_success, SHORT_STRING, "%s", uistr_info_efuse_success_eng);
	snprintf(uistr_info_efuse_reblow, SHORT_STRING, "%s", uistr_info_efuse_reblow_eng);
	snprintf(uistr_info_efuse_broken, SHORT_STRING, "%s", uistr_info_efuse_broken_eng);
	snprintf(uistr_info_efuse_unknown, SHORT_STRING, "%s", uistr_info_efuse_unknown_eng);
	snprintf(uistr_ntc, SHORT_STRING, "%s", uistr_ntc_eng);
	snprintf(uistr_keybox, SHORT_STRING, "%s", uistr_keybox_eng);
	snprintf(uistr_dcdc, SHORT_STRING, "%s", uistr_dcdc_eng);
	snprintf(uistr_gms, SHORT_STRING, "%s", uistr_gms_eng);
	snprintf(uistr_hall, SHORT_STRING, "%s", uistr_hall_eng);
}

void init_factory_string()
{
	if(is_gb2312())
	{
		LOGD(TAG "factory init Chinese string");
		init_chn_string();
	}
	else
	{
		LOGD(TAG "factory init eng string");
		init_eng_string();
	}
}

bool is_gb2312()
{
	int  ret  = 0;
#ifdef MTK_GENERIC_HAL
	char szVal[128] = {0};

	property_get(FONT_GB2312_PROP, szVal, NULL);


	if(strcmp(szVal,"yes")==0)
		ret = 1;
#else
	#ifdef SUPPORT_GB2312
	ret = 1;
	#endif
#endif

	return ret;
}



