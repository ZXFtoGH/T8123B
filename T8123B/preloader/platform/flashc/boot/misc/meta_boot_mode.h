#ifndef __META_BOOT_MODE_H__
#define __META_BOOT_MODE_H__

#include <stdint.h>
/*********From spmeta.h file*************/
struct factory_mode_flag
{
    unsigned char reserved[20];
} ;	// 20Bytes

struct ate_gsm_flag
{
    unsigned char   total_pass:1;
    unsigned char   tadc:1;
    unsigned char   afc:1;
    unsigned char   capid:1;
    unsigned char   agc:1;
    unsigned char   w_coe:1;
    unsigned char   gmsk_apc:1;
    unsigned char   gmsk_apc_subband:1;
    unsigned char   epsk_apc:1;
    unsigned char   epsk_apc_subband:1;
    unsigned char   LPM:1;
    unsigned char   trx_offset:1;
    unsigned char   b_reserved:4;
    unsigned char   reserved[2];
} ;         // 4Bytes

struct ate_tdscdma_flag
{
    unsigned char   total_pass:1;
    unsigned char   tadc:1;
    unsigned char   afc:1;
    unsigned char   capid:1;
    unsigned char   agc:1;
    unsigned char   apc:1;
    unsigned char   subband:1;
    unsigned char   b_reserved:1;
    unsigned char   reserved[1];
} ;    // 2Bytes

struct ate_wcdma_flag
{
    unsigned char  total_pass:1;
    unsigned char  tadc:1;
    unsigned char  afc:1;
    unsigned char  capid:1;
    unsigned char  agc:1;
    unsigned char  rxd:1;
    unsigned char  apc:1;
    unsigned char  subband:1;
    unsigned char  reserved[1];
} ;      // 2Bytes

struct ate_lte_flag
{
    unsigned char   total_pass:1;
    unsigned char   tadc:1;
    unsigned char   afc:1;
    unsigned char   capid:1;
    unsigned char   agc:1;
    unsigned char   rxd:1;
    unsigned char   apc:1;
    unsigned char   subband:1;
    unsigned char   reserved[1];
} ;    // 2Bytes

 struct ate_cdma_flag
{
    unsigned char   total_pass:1;  
    unsigned char   tadc:1;    
    unsigned char   afc:1;    
    unsigned char   capid:1;   
    unsigned char   agc:1;    
    unsigned char   rxd:1;  
    unsigned char   apc:1;   
    unsigned char   subband:1;   
    unsigned char   reserved[1];
}  ;      // 2Bytes

struct ate_nsft_flag
{
    unsigned char        gsm:1;
    unsigned char        tdscdma:1;
    unsigned char        wcdma:1;
    unsigned char        lte:1;
    unsigned char        cdma:1;
    unsigned char        b_reserved:3;
    unsigned char        reserved[1];
} ;      // 2Bytes

struct ate_wireless_flag
{
    unsigned char        gsm:1;
    unsigned char        tdscdma:1;
    unsigned char        wcdma:1;
    unsigned char        lte:1;
    unsigned char        cdma:1;
    unsigned char        b_reserved:3;
    unsigned char        reserved[1];
} ;  // 2Bytes

struct ate_other_flag
{
    unsigned char        gpscoclock:1;
    unsigned char        b_reserved:7;
} ;     // 1Bytes

struct ate_flag
{
    unsigned char                    sw_index;
    unsigned char                    md_index;
    
    struct ate_gsm_flag        	gsm_flag;
    struct ate_tdscdma_flag  	tdscdma_flag;
    struct ate_wcdma_flag     	wcdma_flag;
    struct ate_lte_flag    	    lte_flag;
    struct ate_cdma_flag         cdma_flag;
    struct ate_other_flag      	other_flag;
    struct ate_nsft_flag      	nsft_flag;
    struct ate_wireless_flag 	wireless_flag;
    
    unsigned char                    reserved[3];
} ;       // 22Bytes


/* 
* ****************************************
*                 WCN_ATE_Tool
* ****************************************
*/
struct wcn_ate_flag
{
    unsigned char        wifi_2g_cal:1;
    unsigned char        wifi_5g_cal:1;
    unsigned char	    cal_reserved:6;
    
    unsigned char        wifi:1;
    unsigned char        bt:1;
    unsigned char        gps:1;
    unsigned char        fm:1;
    unsigned char	    nsft_reserved:4;
    unsigned char	    reserved[2];
} ;   // 4Bytes

/* 
* ****************************************
*                 HW_Test_Tool
* ****************************************
*/
struct hw_test_flag
{
    //total pass flag
    unsigned char        total_pass:1;
    
    //test item check flag
    //Auto Test
    unsigned char        bluebooth_check:1;
    unsigned char        wifi_check:1;
    unsigned char        gps_check:1;
    unsigned char        fm_check:1;
    unsigned char        sdcard_check:1;
    unsigned char        sim1_check:1;
    unsigned char        sim2_check:1;
    unsigned char        msensor_check:1;
    unsigned char        gsensor_check:1;
    unsigned char        alspssensor_check:1;
    unsigned char        ctp_check:1;
    unsigned char        nfc_check:1;
    unsigned char        sdio_autoK:1;
    unsigned char        b_reserved_auto_check:2; //2 Bytes
    unsigned char        reserved_auto_check[2];  //2 Bytes
    
    //Manual Test
    unsigned char        keypadled_check:1;
    unsigned char        vibrator_check:1;
    unsigned char        indexled_check:1;
    unsigned char        lcmbacklight_check:1;
    unsigned char        lcm_check:1;
    unsigned char        speakerl_check:1;
    unsigned char        speakerr_check:1;
    unsigned char        receiver_check:1;
    unsigned char        mic1loopback_check:1;
    unsigned char        mic2loopback_check:1;
    unsigned char        photoflash_check:1;
    unsigned char        camera_check:1;
    unsigned char        subcamera_check:1;
    unsigned char        b_reserved_manual_check:3; //2 Bytes
    unsigned char        reserved_manual_check[2];  //2 Bytes
    //test item check flag end(26bit)
    
    //test item flag
    //Auto Test
    unsigned char        bluebooth:1;
    unsigned char        wifi:1;
    unsigned char        gps:1;
    unsigned char        fm:1;
    unsigned char        sdcard:1;
    unsigned char        sim1:1;
    unsigned char        sim2:1;
    unsigned char        msensor:1;
    unsigned char        gsensor:1;
    unsigned char        alspssensor:1;
    unsigned char        ctp:1;
    unsigned char        nfc:1;
    unsigned char        sdio:1;
    unsigned char        b_reserved_auto:3; //2 Bytes
    unsigned char        reserved_auto[2];  //2 Bytes
    
    //Manual Test
    unsigned char        keypadled:1;
    unsigned char        vibrator:1;
    unsigned char        indexled:1;
    unsigned char        lcmbacklight:1;
    unsigned char        lcm:1;
    unsigned char        speakerl:1;
    unsigned char        speakerr:1;
    unsigned char        receiver:1;
    unsigned char        mic1loopback:1;
    unsigned char        mic2loopback:1;
    unsigned char        photoflash:1;
    unsigned char        camera:1;
    unsigned char        subcamera:1;
    unsigned char        b_reserved_manual:3; //2 Bytes
    unsigned char        reserved_manual[2];  //2 Bytes
    //test item flag end(51bit)
    
    //default no use
    //Auto Test
    unsigned char        dvb_check:1;
    unsigned char        battery_check:1;
    unsigned char        pmic_check:1;
    unsigned char        eint_check:1;
    unsigned char        gpioin_check:1;
    unsigned char        sim3_check:1;
    unsigned char        sim4_check:1;
    //Manual Test
    unsigned char        earphone_check:1;
    unsigned char        pwm_check:1;
    unsigned char        sleepmode_check:1;
    unsigned char        charging_check:1;
    unsigned char        gpiooutput_check:1;
    
    //Auto Test
    unsigned char        dvb:1;
    unsigned char        battery:1;
    unsigned char        pmic:1;
    unsigned char        eint:1;
    unsigned char        gpioin:1;
    unsigned char        sim3:1;
    unsigned char        sim4:1;
    //Manual Test
    unsigned char        earphone:1;
    unsigned char        pwm:1;
    unsigned char        sleepmode:1;
    unsigned char        charging:1;
    unsigned char        gpiooutput:1;
    
    unsigned char        reserved[1];    //1 Byte
};               // 20Bytes

struct mtk_test_flag
{
    struct ate_flag  ate_flag[5];	// 110Bytes
    struct wcn_ate_flag  wcn_ate_flag;	// 4Bytes
    struct hw_test_flag   hw_test_flag;	// 20Bytes
    struct factory_mode_flag  factory_mode_flag; // 20Bytes
    unsigned char reserved[16];
} ;	// 170Bytes

struct nvram_ef_imei_imeisv
{
    unsigned char   imei[8];
    unsigned char   svn;
    unsigned char   pad;
} ;

struct target_info_record
{
    unsigned char   BTAddr[6];
    unsigned char   WifiAddr[6];
    unsigned char   ADBSeriaNo[20];
    unsigned char   revered[32];
};

struct mtk_boot_mode_flag {
  unsigned char boot_mode; // 0:normal, 1: meta,  бн
  unsigned char com_type;  // 0:unknow com,1:uart, 2:usb
  unsigned char com_id;    // 0:single interface device, 1: composite device
};

struct product_info
{
    unsigned char               barcode[64];    // 64B
    struct nvram_ef_imei_imeisv IMEI[4];        // 40B
    struct target_info_record        target_info;    // 64B
    struct mtk_test_flag             mtk_test_flag;  // 170B
    struct mtk_boot_mode_flag        mtk_boot_mode_flag ;//3B
    unsigned char reserved[1024-170-64-40-64-3];
};

/*********From spmeta.h file*************/

status_t set_meta_boot_mode(int connect_type, BOOL mobile_log, BOOL adb);
status_t set_atm_mode(void);

#endif
