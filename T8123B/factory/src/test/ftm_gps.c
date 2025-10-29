/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cutils/properties.h>
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include <strings.h>
#include <sys/un.h>

#include "common.h"
#include "miniui.h"
#include "ftm.h"

// for read NVRAM
#include "libnvram.h"
#include "CFG_GPS_File.h"
// #include "CFG_GPS_Default.h"
#include "CFG_file_lid.h"
#ifndef  MTK_GENERIC_HAL
#include "Custom_NvRam_LID.h"
#else
#include <dlfcn.h>
#endif


#ifdef FEATURE_FTM_GPS

//#define FTM_GPS_NMEA_DEBUG
#ifdef GPS_PROPERTY
#undef GPS_PROPERTY
#endif

#define FACTORY2MNLD_INTERFACE_BUFF_SIZE 20
#define FACTORY2MNLD_INTERFACE_PROTOCOL_TYPE 303
#define FACTORY_TO_MNLD_SOCKET        "mtk_meta2mnld"
#define FACTORY_TO_MNLD_PMTK          "meta2mnl_req"
#define MNLD_TO_FACTORY_PMTK          "mnl2meta_ack"

typedef enum {
    FACTORY2MNLD_INTERFACE_REQ_GNSS_LOCATION = 0,
    FACTORY2MNLD_INTERFACE_CANCEL_GNSS_LOCATION = 1,
} Factory2MnldInterface_message_id;

#define GPS_PROPERTY "/data/misc/GPS_CHIP_FTM.cfg"
static  ap_nvram_gps_config_struct stGPSReadback;
static int use_aosp_gps = -1;
extern char result[3][16];

#define MNL_ATTR_PWRCTL  "/sys/class/gpsdrv/gps/pwrctl"
#define MNL_ATTR_SUSPEND "/sys/class/gpsdrv/gps/suspend"
#define MNL_ATTR_STATE   "/sys/class/gpsdrv/gps/state"
#define MNL_ATTR_PWRSAVE "/sys/class/gpsdrv/gps/pwrsave"
#define MNL_ATTR_STATUS  "/sys/class/gpsdrv/gps/status"
#define mod_to_gps(p)  (struct gps_desc*)((char*)(p) + sizeof(struct ftm_module))

#define C_INVALID_PID  (-1)   /*invalid process id*/
#define C_INVALID_TID  (-1)   /*invalid thread id*/
#define C_INVALID_FD   (-1)   /*invalid file handle*/
#define C_INVALID_SOCKET (-1) /*invalid socket id*/

#define MND_ERR(fmt, arg ...) LOGD(TAG"%s: " fmt, __FUNCTION__ ,##arg)
#define MND_MSG(fmt, arg ...) LOGD(TAG"%s: " fmt, __FUNCTION__ ,##arg)

#define GPS_L5_SUPPORT_PROP "vendor.debug.gps.support.l5"

static int mnld_terminated = 0;

int fd_mnld2factory = C_INVALID_SOCKET;
#define FACTORY_STRNCPY(dst,src,size) do{\
                                       strncpy((char *)(dst), (src), (size - 1));\
                                      (dst)[size - 1] = '\0';\
                                     }while(0)
enum {
    GPS_PWRCTL_UNSUPPORTED  = 0xFF,
    GPS_PWRCTL_OFF          = 0x00,
    GPS_PWRCTL_ON           = 0x01,
    GPS_PWRCTL_RST          = 0x02,
    GPS_PWRCTL_OFF_FORCE    = 0x03,
    GPS_PWRCTL_RST_FORCE    = 0x04,
    GPS_PWRCTL_MAX          = 0x05,
};

#define TAG             "[GPS]   "
#define INFO_SIZE       1024
#define NMEA_SIZE       10240

unsigned char nmea_buf[NMEA_SIZE];

#define NUM_CH  (20)
#define PSEUDO_CH (32)
#define Knot2Kmhr (1.8532)
static int timeout = 60;
static int init_flag = 0;
typedef struct SVInfo
{
    int SVid;            // PRN
    int SNR;
    int elv;             // elevation angle : 0~90
    int azimuth;         // azimuth : 0~360
    unsigned char Fix;   // 0:None , 1:FixSV
} SVInfo;

typedef struct ChInfo
{
    int SVid;            // PRN
    int SNR;             // SNR
    unsigned char Status;// Status(0:Idle, 1:Search, 2:Tracking)
} ChInfo;

typedef struct GPSInfo
{
    int year;
    int mon;
    int day;
    int hour;
    int min;
    float sec;

    float Lat; // Position, +:E,N -:W,S
    float Lon;
    float Alt;
    unsigned char FixService;  // NoFix:0, SPS:1, DGPS:2, Estimate:6
    unsigned char FixType;     // None:0, 2D:1, 3D:2
    float Speed;  // km/hr
    float Track;  // 0~360
    float PDOP;   //DOP
    float HDOP;
    float VDOP;

    int SV_cnt;
    int SV_cnt_l5;
    int fixSV[NUM_CH];
}GPSInfo;
extern char *ftm_get_prop(const char *name);
GPSInfo g_gpsInfo;
SVInfo  g_svInfo[NUM_CH];
SVInfo  g_svL5Info[NUM_CH];
ChInfo  g_chInfo[PSEUDO_CH];

int ttff = 0;
int fixed = 0;
int httff = 0;
int cttff = 0;
int open_time = 0;

int ttff_check_res = 0;
int ttff_l5_check_res = 0;
bool g_l5_support = 0;

enum {
    ITEM_PASS,
    ITEM_FAIL,
    ITEM_HTTFF,
    ITEM_CTTFF
};

static item_t gps_items[] = {
  #ifndef FEATURE_FTM_TOUCH_MODE
    item(ITEM_PASS,   uistr_pass),
    item(ITEM_FAIL,   uistr_fail),
  #endif
    item(ITEM_HTTFF,  uistr_info_gps_hot_restart),
    item(ITEM_CTTFF,  uistr_info_gps_cold_restart),
    item(-1, NULL),
};

static item_t gps_items_auto[] = {

    item(-1, NULL),
};

struct gps_desc {
    char         info[INFO_SIZE];
    char        *mntpnt;
    bool         exit_thd;

    text_t title;
    text_t text;

    pthread_t update_thd;
    pthread_t update_ui;
    struct ftm_module *mod;
    struct itemview *iv;
};



typedef struct {
    const char*  p;
    const char*  end;
} Token;

#define  MAX_NMEA_TOKENS  64
#define  NMEA_MAX_SV_INFO 4

typedef struct {
    int     count;
    Token   tokens[MAX_NMEA_TOKENS];
} NmeaTokenizer;

typedef enum {
    SIGNAL_ID_GP_ALL    = 0,
    SIGNAL_ID_GP_L1_CA  = 1,
    SIGNAL_ID_GP_L1_P   = 2,
    SIGNAL_ID_GP_L1_M   = 3,
    SIGNAL_ID_GP_L2_P   = 4,
    SIGNAL_ID_GP_L2C_M  = 5,
    SIGNAL_ID_GP_L2C_L  = 6,
    SIGNAL_ID_GP_L5_I   = 7,
    SIGNAL_ID_GP_L5_Q   = 8,
} SIGNAL_ID_GP;

pid_t mnl_pid = C_INVALID_PID;
pthread_t gps_factory_thread_handle = C_INVALID_TID;
static struct ftm_param GPS_param;

#define FTM_SPRINTF(buf,fmt,...)  do{\
                                       if(sprintf((char *)(buf), fmt,##__VA_ARGS__) < 0){\
                                           MND_ERR("sprintf error occurred");\
                                       }\
                                     }while(0)

static void gps_check_l5_support() {
    char result[PROPERTY_VALUE_MAX] = {0};
    if ((property_get(GPS_L5_SUPPORT_PROP, result, NULL)) && (result[0] == '1')) {
        g_l5_support = true;
        LOGD("support L5");
    } else {
        g_l5_support = false;
        LOGD("not support L5");
    }
}

static bool gps_l5_support() {
    return !!g_l5_support;
}

static int gps_ttff_check_res()
{
    if(!gps_l5_support()) {
        LOGD(TAG"L1 only:%d", ttff_check_res);
        return ttff_check_res;
    } else {
        LOGD(TAG"L1 SV check:%d, L5 SV check:%d", ttff_check_res, ttff_l5_check_res);
        return (ttff_check_res && ttff_l5_check_res);
    }
}

static int mnl_write_attr(const char *name, unsigned char attr) {
    int err, fd = open(name, O_RDWR);
    char buf[] = {attr + '0'};

    if (fd < 0) {
        LOGD(TAG"open %s err = %s\n", name, strerror(errno));
        return -errno;
    }
    do { err = write(fd, buf, sizeof(buf) ); }
    while (err < 0 && errno == EINTR);

    if (err != sizeof(buf)) {
        LOGD(TAG"write fails = %s\n", strerror(errno));
        err = -errno;
    } else {
        err = 0;    /*no error*/
    }
    if (close(fd) == -1) {
        LOGD(TAG"close fails = %s\n", strerror(errno));
        err = (err) ? (err) : (-errno);
    }
    LOGD(TAG"write '%d' to %s okay\n", attr, name);
    return err;
}
/*****************************************************************************/
void power_on_3332() {
    int err;
    err = mnl_write_attr(MNL_ATTR_PWRCTL, GPS_PWRCTL_RST_FORCE);
    if (err != 0) {
        MND_ERR("GPS_Open: GPS power-on error: %d\n", err);
        return;
    }
    usleep(1000*100);
    return;
}
/*****************************************************************************/
void power_off_3332() {
    int err;
    err = mnl_write_attr(MNL_ATTR_PWRCTL, GPS_PWRCTL_OFF);
    if (err != 0) {
        MND_ERR("GPS_Open: GPS power-on error: %d\n", err);
        return;
    }
    usleep(1000*100);
    return;
}

/*******************************************************************/
static int read_NVRAM()
{
    // int gps_nvram_fd = 0;
    F_ID gps_nvram_fd;
    int rec_size;
    int rec_num;


    memset(&stGPSReadback, 0, sizeof(stGPSReadback));

#ifndef  MTK_GENERIC_HAL
    int file_lid = AP_CFG_CUSTOM_FILE_GPS_LID;
#else
    void *hcustlib;
    int *pui;

    hcustlib = dlopen("libcustom_nvram.so", RTLD_LAZY);
    if (!hcustlib) {
        MND_ERR("dlopen() fail! errno=%s\n", dlerror());
        return -1;
    }

    pui = (int *) dlsym(hcustlib, "iAP_CFG_CUSTOM_FILE_GPS_LID");
    if (!pui) {
        MND_ERR("dlsym() fail! errno=%s\n", dlerror());
        dlclose(hcustlib);
        return -1;
    }
    int file_lid = *pui;
    dlclose(hcustlib);
#endif

    gps_nvram_fd = NVM_GetFileDesc(file_lid, &rec_size, &rec_num, ISREAD);
    if (gps_nvram_fd.iFileDesc > 0) {   /* >0 means ok */
        if (read(gps_nvram_fd.iFileDesc, &stGPSReadback , rec_size*rec_num) < 0)
            MND_ERR("read NVRAM error, %s\n", strerror(errno));;
        NVM_CloseFileDesc(gps_nvram_fd);

        stGPSReadback.dsp_dev[sizeof(stGPSReadback.dsp_dev)-1] = 0;
        if (strlen(stGPSReadback.dsp_dev) != 0) {
             MND_MSG("GPS NVRam (%d * %d) : \n", rec_size, rec_num);
             MND_MSG("dsp_dev : %s\n", stGPSReadback.dsp_dev);
        } else {
             MND_ERR("GPS NVRam mnl_config.dev_dsp == NULL \n");
             return -1;
        }
    } else {
         MND_ERR("GPS NVRam gps_nvram_fd.iFileDesc<=0 \n");
         return -1;
    }
    if (strcmp(stGPSReadback.dsp_dev, "/dev/stpgps") == 0) {
        MND_ERR("not 3332 UART port\n");
        return 1;
    }
    return 0;
}

/*****************************************************************************/
static int init_3332_interface(const int fd) {

    struct termios termOptions;
    // fcntl(fd, F_SETFL, 0);

    // Get the current options:
    tcgetattr(fd, &termOptions);

    // Set 8bit data, No parity, stop 1 bit (8N1):
    termOptions.c_cflag &= ~PARENB;
    termOptions.c_cflag &= ~CSTOPB;
    termOptions.c_cflag &= ~CSIZE;
    termOptions.c_cflag |= CS8 | CLOCAL | CREAD;

    MND_MSG("GPS_Open: c_lflag=%x, c_iflag=%x, c_oflag=%x\n", termOptions.c_lflag, termOptions.c_iflag,
                            termOptions.c_oflag);

    // Raw mode
    termOptions.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF | IXANY);
    termOptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /*raw input*/
    termOptions.c_oflag &= ~OPOST;  /*raw output*/

    tcflush(fd, TCIFLUSH);  // clear input buffer
    termOptions.c_cc[VTIME] = 10;  /* inter-character timer unused, wait 1s, if no data, return */
    termOptions.c_cc[VMIN] = 0;  /* blocking read until 0 character arrives */

    // Set baudrate to 38400 bps
    cfsetispeed(&termOptions, B115200);  /*set baudrate to 115200, which is 3332 default bd*/
    cfsetospeed(&termOptions, B115200);

    tcsetattr(fd, TCSANOW, &termOptions);

    return 0;
}
/*****************************************************************************/
static int hw_test_3332(const int fd) {
    ssize_t bytewrite, byteread;
    char buf[6] = {0};
    char cmd[] = {0xAA, 0xF0, 0x6E, 0x00, 0x08, 0xFE, 0x1A, 0x00, 0x00, 0x00, 0x00,
                0x00, 0xC3, 0x01, 0xA5, 0x02, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x45, 0x00,
                0x80, 0x04, 0x80, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00,
                0x96, 0x00, 0x6F, 0x3C, 0xDE, 0xDF, 0x8B, 0x6D, 0x04, 0x04, 0x00, 0xD2, 0x00,
                0xB7, 0x00, 0x28, 0x00, 0x5D, 0x4A, 0x1E, 0x00, 0xC6, 0x37, 0x28, 0x00, 0x5D,
                0x4A, 0x8E, 0x65, 0x00, 0x00, 0x01, 0x00, 0x28, 0x00, 0xFF, 0x00, 0x80, 0x00,
                0x47, 0x00, 0x64, 0x00, 0x50, 0x00, 0xD8, 0x00, 0x50, 0x00, 0xBB, 0x00, 0x03,
                0x00, 0x3C, 0x00, 0x6F, 0x00, 0x89, 0x00, 0x88, 0x00, 0x02, 0x00, 0xFB, 0x00,
                0x01, 0x00, 0x00, 0x00, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x7A,
                0x16, 0xAA, 0x0F};
    char ack[] = {0xaa, 0xf0, 0x0e, 0x00, 0x31, 0xfe};

    bytewrite = write(fd, cmd, sizeof(cmd));
    if (bytewrite == sizeof(cmd)) {
        usleep(500*1000);
        byteread = read(fd, buf, sizeof(buf));
        MND_MSG("ack:%02x %02x %02x %02x %02x %02x\n",
                 buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
        if ((byteread == sizeof(ack)) && (memcmp(buf, ack, sizeof(ack)) == 0)) {
            MND_MSG("it's 3332\n");
            return 0;   /*0 means 3332,   1 means other GPS chips*/
        }
        return 1;
    } else {
        MND_ERR("write error, write API return error message is %s\n", strerror(errno));
        return 1;
    }
}

/*****************************************************************************/
static int hand_shake() {
    int fd;
    int ret;
    int nv;
    nv = read_NVRAM();

    if (nv == 1)
        return 1;
    else if (nv == -1)
        return -1;
    else
        MND_MSG("read NVRAM ok\n");

    fd = open(stGPSReadback.dsp_dev, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        MND_ERR("GPS_Open: Unable to open - %s, %s\n", stGPSReadback.dsp_dev, strerror(errno));
    return -1;
    }
    init_3332_interface(fd);    /*set UART parameter*/

    ret = hw_test_3332(fd); /*is 3332?  0:yes   1:no*/
    close(fd);
    return ret;
}

/*****************************************************************************/
static int confirm_if_3332() {
    int ret;
    power_on_3332();
    ret = hand_shake();
    power_off_3332();
    return ret;
}

/*****************************************************************************/
void chip_detector() {
    int get_time = 5;
    int res;
    char chip_id[PROPERTY_VALUE_MAX];/*combo chip ID*/

    int fd = -1;
    fd = open(GPS_PROPERTY, O_RDWR|O_CREAT, 0606);
    if (fd < 0) {
        MND_ERR("open %s error, %s\n", GPS_PROPERTY, strerror(errno));
        return;
    }
    int read_len;
    char buf[100] = {0};
    read_len = read(fd, buf, sizeof(buf));
    if (read_len == -1) {
        MND_ERR("read %s error, %s\n", GPS_PROPERTY, strerror(errno));
        goto exit_chip_detector;
    } else if (read_len != 0) {  /*print chip id then return*/
        MND_MSG("gps is %s\n", buf);
        goto exit_chip_detector;
    }
    else
        MND_MSG("we need to known which GPS chip is in use\n");
    while (get_time-- != 0 && (property_get("persist.mtk.wcn.combo.chipid", chip_id, NULL) <= 0)) {
        usleep(100000);
    }

    MND_MSG("combo_chip_id is %s\n", chip_id);
    /*get chip from combo chip property, if 6620 or 6572 just set GPS chip as the same value*/
    if (strcmp(chip_id, "0x6620") ==0) {
        MND_MSG("we get MT6620\n");
        if (write(fd, "0x6620", sizeof("0x6620")) == -1)
            MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

        goto exit_chip_detector;
    }
    if (strcmp(chip_id, "0x6630") ==0) {
        MND_MSG("we get MT6630\n");
        if (write(fd, "0x6630", sizeof("0x6630")) == -1)
            MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

        goto exit_chip_detector;
    }
    /*detect if there is 3332, yes set GPS property to 3332, then else read from combo chip to see which GPS chip used*/
    res = confirm_if_3332();    /*0 means 3332, 1 means not 3332, other value means error*/

    if (res == 0) {
        MND_MSG("we get MT3332\n");

        if (write(fd, "0x3332", sizeof("0x3332")) == -1)
            MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

        goto exit_chip_detector;
    } else if (res == 1) {
        /*we can not distinguish 6628T and 6628Q yet*/
        if (strcmp(chip_id, "0x6628") == 0) {
            MND_MSG("we get MT6628\n");
            if (write(fd, "0x6628", sizeof("0x6628")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
        if (strcmp(chip_id, "0x6582") ==0) {
            MND_MSG("we get MT6582\n");
            if (write(fd, "0x6582", sizeof("0x6582")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
        if (strcmp(chip_id, "0x8127") ==0) {
            MND_MSG("we get MT8127\n");
            if (write(fd, "0x8127", sizeof("0x8127")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
        if (strcmp(chip_id, "0x6572") ==0) {
            MND_MSG("we get MT6572\n");
            if (write(fd, "0x6572", sizeof("0x6572")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
        if (strcmp(chip_id, "0x6571") ==0) {
            MND_MSG("we get MT6571\n");
            if (write(fd, "0x6571", sizeof("0x6571")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
        if (strcmp(chip_id, "0x6592") ==0) {
            MND_MSG("we get MT6592\n");
            if (write(fd, "0x6592", sizeof("0x6592")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
        if (strcmp(chip_id, "0x6752") ==0) {
            MND_MSG("we get MT6752\n");
            if (write(fd, "0x6752", sizeof("0x6752")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
        if (strcmp(chip_id, "0x6735") ==0) {
            MND_MSG("we get MT6735\n");
            if (write(fd, "0x6735", sizeof("0x6735")) == -1)
                MND_ERR("write %s error, %s\n", GPS_PROPERTY, strerror(errno));

            goto exit_chip_detector;
        }
    }
    else
        MND_ERR("this should never be showed\n");

exit_chip_detector:
    close(fd);
    return;
}

static void sighlr(int signo) {
    //int err = 0;
    //pthread_t self = pthread_self();

    LOGD("Wait MNLD terminated");
    if (signo == SIGCHLD) {
        LOGD("MNLD terminated");
        mnld_terminated = 1;
    }
}

/*****************************************************************************/
int setup_signal_handler(void) {
    struct sigaction actions;
    int err;

    /*the signal handler is MUST, otherwise, the thread will not be killed*/
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = sighlr;

    if ((err = sigaction(SIGALRM, &actions, NULL))) {
        LOGE("register signal handler for SIGALRM: %s\n", strerror(errno));
        return -1;
    }
    if ((err = sigaction(SIGCHLD, &actions, NULL))) {
        LOGE("register signal handler for SIGALRM: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void mtk_socket_put_char(char* buff, int* offset, char input) {
    *((char*)&buff[*offset]) = input;
    *offset += 1;
}

void mtk_socket_put_short(char* buff, int* offset, short input) {
    mtk_socket_put_char(buff, offset, input & 0xff);
    mtk_socket_put_char(buff, offset, (input >> 8) & 0xff);
}

void mtk_socket_put_int(char* buff, int* offset, int input) {
    mtk_socket_put_short(buff, offset, input & 0xffff);
    mtk_socket_put_short(buff, offset, (input >> 16) & 0xffff);
}
int factory_safe_sendto(const char* path, const char* buff, int len) {
    int ret = 0;
    struct sockaddr_un addr;
    int retry = 10;
    int fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (fd < 0) {
        MND_ERR("safe_sendto() socket() failed reason=[%s]%d",
            strerror(errno), errno);
        return -1;
    }

    int flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1){
        MND_ERR("fcntl failed reason=[%s]%d",
                    strerror(errno), errno);

        close(fd);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_path[0] = 0;
    strncpy(addr.sun_path +1, path,sizeof(addr.sun_path) -2);
    addr.sun_family = AF_UNIX;

    while ((ret = sendto(fd, buff, len, 0,
        (const struct sockaddr *)&addr, sizeof(addr))) == -1) {
        if (errno == EINTR) continue;
        if (errno == EAGAIN) {
            if (retry-- > 0) {
                usleep(100 * 1000);
                continue;
            }
        }
        MND_MSG("safe_sendto() sendto() failed path=[%s] ret=%d reason=[%s]%d",
            path, ret, strerror(errno), errno);
        break;
    }

    close(fd);
    return ret;
}

// -1 means failure
int socket_bind_udp(const char* path) {
    int fd;
    struct sockaddr_un addr;

    fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (fd < 0) {
        MND_ERR("socket_bind_udp() socket() failed reason=[%s]%d",
            strerror(errno), errno);
        return -1;
    }
    MND_MSG("fd=%d,path=%s\n", fd, path);

    memset(&addr, 0, sizeof(addr));
    addr.sun_path[0] = 0;
    FACTORY_STRNCPY(addr.sun_path + 1, path,sizeof(addr.sun_path) - 1);
    addr.sun_family = AF_UNIX;
    unlink(addr.sun_path);
    if( bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        MND_ERR("socket_bind_udp() bind() failed path=[%s] reason=[%s]%d",
            path, strerror(errno), errno);
        close(fd);
        return -1;
    }

    MND_MSG("path=%s\n", path);

    return fd;
}

// -1 means failure
int socket_bind_udp_force(const char* path) {
    int ret = 0;
    int retry_cnt = 0;

    do {
        ret = socket_bind_udp(path);
        if(ret < 0) {
            retry_cnt++;
            MND_ERR("socket_bind_udp_force() bind() failed path=[%s] reason=[%s]%d, retry_cnt:%d",
                path, strerror(errno), errno, retry_cnt);
            sleep(1);
            if(retry_cnt > 10) {
                MND_ERR("socket bind fail, mnld exit...");
                _exit(-1);
            }
        }
    } while(ret < 0);

    MND_MSG("path=%s\n", path);

    return ret;
}

int create_mnld2factory_fd() {
    int fd = socket_bind_udp_force(MNLD_TO_FACTORY_PMTK);
    return fd;
}
// -1 means failure
int safe_recvfrom(int fd, char* buff, int len) {
    int ret = 0;
    int retry = 10;

    int flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1){
        MND_ERR("fcntl failed reason=[%s]%d",
                    strerror(errno), errno);
    }

    while ((ret = recvfrom(fd, buff, len, 0,
         NULL, NULL)) == -1) {
        //MND_ERR("safe_recvfrom() ret=%d len=%d", ret, len);
        if (errno == EINTR) continue;
        if (errno == EAGAIN) {
            if (retry-- > 0) {
                usleep(100 * 1000);
                continue;
            }
        }
        MND_ERR("safe_recvfrom() recvfrom() failed reason=[%s]%d",
            strerror(errno), errno);
        break;
    }
    return ret;
}
int safe_recvfrom_block(int fd, char* buff, int len) {
    int ret = 0;

    int flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags&~O_NONBLOCK) == -1){
        MND_ERR("fcntl failed reason=[%s]%d",
                    strerror(errno), errno);
    }

    ret = recvfrom(fd, buff, len, 0, NULL, NULL);
    //META_LOG("safe_recvfrom() ret=%d len=%d", ret, len);
    if (ret < 0) {
        MND_ERR("safe_recvfrom()ret=%d recvfrom() failed reason=[%s]%d",
            ret, strerror(errno), errno);
    }
    return ret;
}

void factory_request_gps(bool turnOn) {
    int src = 0;
    int _offset = 0;
    char _buff[FACTORY2MNLD_INTERFACE_BUFF_SIZE] = {0};
    int msg = turnOn ? FACTORY2MNLD_INTERFACE_REQ_GNSS_LOCATION: FACTORY2MNLD_INTERFACE_CANCEL_GNSS_LOCATION;

    mtk_socket_put_int(_buff, &_offset, FACTORY2MNLD_INTERFACE_PROTOCOL_TYPE);
    mtk_socket_put_int(_buff, &_offset, msg);
    mtk_socket_put_int(_buff, &_offset, src);
    MND_MSG("FACTORY_request_gps FACTORY2MNLD_INTERFACE_PROTOCOL_TYPE: %d, msg:%d, src:%d, _offset:%d",
        *(int*)_buff,*(int *)(_buff + 4),*(int *)(_buff+8),_offset);
    if (factory_safe_sendto(FACTORY_TO_MNLD_SOCKET, _buff, _offset) == -1) {
        MND_MSG("FACTORY_safe_sendto failed!");
    }
}

/*****************************************************************************/
static int GPS_Open()
{
    #if defined(MTK_GPS_MT3332)
    short err;
    #endif
    char buf[10];
    int read_len = 0;
    pid_t pid;
    int portno;
    struct sockaddr_in serv_addr;
    char *argv[] = {"/vendor/bin/libmnla", "libmnlp"};
    chip_detector();

    char chip_id[100] = {0};
    int fd = -1;
    if ((fd = open(GPS_PROPERTY, O_RDONLY)) < 0)
        MND_ERR("open %s error, %s\n", GPS_PROPERTY, strerror(errno));
    if (fd >= 0) {
        if (read(fd, chip_id, sizeof(chip_id)-1) == -1)
            MND_ERR("open %s error, %s\n", GPS_PROPERTY, strerror(errno));
        close(fd);
    }

    if ((strcmp(chip_id, "0x6620") != 0) || (strcmp(chip_id, "0x6628")!= 0)) {
        use_aosp_gps = 1;
        if (setup_signal_handler()) {
            LOGD("setup_signal_handler: %d (%s)\n", errno, strerror(errno));
            exit(1);
        }
    } else {
        LOGD("Not AOSP architecture");
        use_aosp_gps = 0;
    }

    LOGD(TAG"GPS_Open() 1\n");
    // power on GPS chip
#if defined(MTK_GPS_MT3332)
    err = mnl_write_attr("/sys/class/gpsdrv/gps/pwrctl", 4);
    if (err != 0) {
        LOGD(TAG"GPS_Open: GPS power-on error: %d\n", err);
        return (-1);
    }
#endif


    // run gps driver (libmnlp)
    if ((pid = fork()) < 0) {
        LOGD(TAG"GPS_Open: fork fails: %d (%s)\n", errno, strerror(errno));
        return (-2);
    }
    else if (pid == 0) {   /*child process*/
        int err = -1;

        char chip_id[100] = {0};
        int fd = -1;
        if ((fd = open(GPS_PROPERTY, O_RDONLY)) < 0)
            MND_ERR("open %s error, %s\n", GPS_PROPERTY, strerror(errno));
        if (fd >= 0) {
            if (read(fd, chip_id, sizeof(chip_id)) == -1)
                MND_ERR("open %s error, %s\n", GPS_PROPERTY, strerror(errno));
            close(fd);
        }

        MND_MSG("chip_id is %s\n", chip_id);
        if (strcmp(chip_id, "0x6620") == 0) {
            MND_MSG("we get MT6620\n");
            char *mnl6620 = "/vendor/bin/libmnlp_mt6620";
            argv[0] = mnl6620;
            MND_MSG("execute: %s \n", argv[0]);
            err = execl(argv[0], "libmnlp", "1Hz=y", NULL);
        } else if (strcmp(chip_id, "0x6628") == 0) {
            MND_MSG("we get MT6628\n");
            char *mnl6628 = "/vendor/bin/libmnlp_mt6628";
            argv[0] = mnl6628;
            MND_MSG("execute: %s \n", argv[0]);
            err = execl(argv[0], "libmnlp", "1Hz=y", NULL);
        } else {
            MND_MSG("Factory test, hidl start gps\n");
            factory_request_gps(true);
            err = 0;
        }
        if (err == -1) {
            MND_MSG("execl error: %s\n", strerror(errno));
            return -1;
        }
        return 0;
    } else {  /*parent process*/
        mnl_pid = pid;
        MND_MSG(TAG"GPS_Open: mnl_pid = %d\n", pid);
    }

    // create socket connection to gps driver
    fd_mnld2factory = create_mnld2factory_fd();
    if (fd_mnld2factory < 0) {
        MND_MSG("create_mnld2meta_fd() failed");
        return (-4);
    }
    int try_time = 10;
    //wait gps start done, first nmea report
    while(read_len <= 0) {
        read_len = safe_recvfrom(fd_mnld2factory, buf, sizeof(buf) - 1);
        if (try_time <= 0) {
            return (-6);
        }
        MND_MSG("safe_recvfrom try_time = %d", try_time);
        try_time --;
    }
    MND_MSG("Receive nmea from mnld success!!! read_len=%d", read_len);

    gps_check_l5_support();  //Property will be setted in mnld
    init_flag = 1;
    MND_MSG(TAG"GPS_Open() 2, set init_flag = 1\n");

    return 0;
}

static void GPS_Close()
{
    int err;
    int cnt = 0, max = 10;

    LOGD(TAG"GPS_Close() 1\n");

    // kill gps driver (libmnlp)
    if (mnl_pid != C_INVALID_PID) {
        LOGD(TAG"GPS_Close() 5\n");
        MND_MSG("Factory test, hidl stop gps\n");
        factory_request_gps(false);
        if (use_aosp_gps == 0) {
            kill(mnl_pid, SIGKILL);
            usleep(500000);  // 500ms
        } else if (use_aosp_gps == 1) {
            kill(mnl_pid, SIGTERM);
            while (!mnld_terminated) {
                if (cnt++ < max) {
                    /*timeout: 1 sec;
                      notice that libmnlp needs some sleep time after MTK_PARAM_CMD_RESET_DSP*/
                    usleep(100000);
                    continue;
                } else {
                    kill(mnl_pid, SIGKILL);
                    usleep(100000);
                }
            }
            LOGD("waiting counts: %d\n", cnt);
            mnl_pid = wait(&err);

            if (mnl_pid == -1)
                LOGD("wait error: %s\n",strerror(errno));
            LOGD("mnld process : %d is killed\n", mnl_pid);
            mnld_terminated = 1;
        }
    }
    LOGD(TAG"GPS_Close() 4\n");
    // disconnect to gps driver
    if (fd_mnld2factory != C_INVALID_SOCKET) {
        LOGD(TAG"GPS_Close() 2\n");
        close(fd_mnld2factory);
        LOGD(TAG"GPS_Close() 3\n");
        fd_mnld2factory = C_INVALID_SOCKET;
    }

    use_aosp_gps = -1;

    LOGD(TAG"GPS_Close() 6\n");

    // power off GPS chip
#if defined(MTK_GPS_MT3332)
    err = mnl_write_attr("/sys/class/gpsdrv/gps/pwrctl", 0);
    if (err != 0) {
        LOGD(TAG"GPS power-off error: %d\n", err);
    }
    LOGD(TAG"GPS_Close() 6\n");
#endif
    unlink(GPS_PROPERTY);
    return;
}

unsigned char CheckSum(char *buf, int size) {
   int i;
   char chksum=0, chksum2=0;

   if (size < 5)
      return false;

   chksum = buf[1];
   for (i = 2; i < (size - 2); i++) {
      if (buf[i] != '*') {
        chksum ^= buf[i];
      } else {
        if (buf[i + 1] >= 'A') {
          chksum2 = (buf[i+1]-'A'+10)<<4;
        } else {
          chksum2 = (buf[i+1]-'0')<<4;
        }

        if (buf[i + 2] >= 'A') {
          chksum2 += buf[i+2]-'A'+10;
        } else {
          chksum2 += buf[i+2]-'0';
        }
        break;
      }
    }

   /* if not found character '*' */
   if (i == (size - 2)) {
      return (false);
   }

   if (chksum == chksum2) {
     return (true);
   } else {
     return (false);
   }
}

bool FetchField(char *start, char *result) {
   char *end;

   if (start == NULL)
      return false;

   end = strstr(start, ",");
   // the end of sentence
   if (end == NULL)
      end = strstr(start, "*");

   if (end-start > 0)
   {
     strncpy(result, start, end-start);
     result[end-start] = '\0';
   } else {   // no data
     result[0]='\0';
     return false;
   }

   return true;
}

static int nmea_tokenizer_init(NmeaTokenizer*  t, const char*  p, const char*  end) {
    int    count = 0;

    // the initial '$' is optional
    if (p < end && p[0] == '$')
        p += 1;

    // remove trailing newline
    if (end > p && end[-1] == '\n') {
        end -= 1;
        if (end > p && end[-1] == '\r')
            end -= 1;
    }

    // get rid of checksum at the end of the sentecne
    if (end >= p+3 && end[-3] == '*') {
        end -= 3;
    }

    while (p < end) {
        const char*  q = p;

        q = memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (q >= p) {
            if (count < MAX_NMEA_TOKENS) {
                t->tokens[count].p   = p;
                t->tokens[count].end = q;
                count += 1;
            }
        }
        if (q < end)
            q += 1;

        p = q;
    }

    t->count = count;
    return count;
}

static Token nmea_tokenizer_get(NmeaTokenizer* t, int  index) {
    Token  tok;
    static const char*  dummy = "";

    if (index < 0 || index >= t->count) {
        tok.p = tok.end = dummy;
    } else
        tok = t->tokens[index];

    return tok;
}

/*****************************************************************************/
static int str2int(const char*  p, const char*  end) {
    int   result = 0;
    int   len    = end - p;
    int   sign = 1;

    if (*p == '-') {
        sign = -1;
        p++;
        len = end - p;
    }

    for (; len > 0; len--, p++) {
        int  c;

        if (p >= end)
            return -1;

        c = *p - '0';
        if ((unsigned)c >= 10)
            return -1;

        result = result*10 + c;
    }
    return  sign*result;
}

static double str2float(const char*    p, const char*  end) {
    int   len    = end - p;
    char  temp[16];

    if (len >= (int)sizeof(temp))
        return 0.;

    memcpy(temp, p, len);
    temp[len] = 0;
    return strtod( temp, NULL );
}

void GLL_Parse( char *head)
{
   char *start, result[20], tmp[20], *point;
   unsigned short len=0;
   char FixService;

   // check checksum
   if (CheckSum(head, strlen(head))) {
      // Position(Lat)
      start = strstr( head, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         point = strstr( result, ".");
         if (point != NULL){
             len = (point-2)-result;
             strncpy(tmp, result, len);
             tmp[len]='\0';
             g_gpsInfo.Lat = (float)(atof(tmp));
             strncpy(tmp, result+len, strlen(result)-len);
             tmp[strlen(result)-len]='\0';
             g_gpsInfo.Lat += (float)(atof(tmp)/60.0);
         }
      }

      // N or S
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         if (*result == 'S')
            g_gpsInfo.Lat = -g_gpsInfo.Lat;
      }

      // Position(Lon)
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         point = strstr( result, ".");
         if (point != NULL){
             len = (point-2)-result;
             strncpy(tmp, result, len);
             tmp[len]='\0';
             g_gpsInfo.Lon = (float)(atof(tmp));
             strncpy(tmp, result+len, strlen(result)-len);
             tmp[strlen(result)-len]='\0';
             g_gpsInfo.Lon += (float)(atof(tmp)/60.0);
         }
      }

      // E or W
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         if (*result == 'W')
            g_gpsInfo.Lon = -g_gpsInfo.Lon;
      }

      // UTC Time
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         // Hour
         strncpy(tmp, result, 2);
         tmp[2] = '\0';
         g_gpsInfo.hour = atoi(tmp);
         // Min
         strncpy(tmp, result+2, 2);
         tmp[2] = '\0';
         g_gpsInfo.min = atoi(tmp);
         // Sec
         strncpy(tmp, result+4, strlen(result)-4);
         tmp[strlen(result)-4] = '\0';
         g_gpsInfo.sec = (float)(atof(tmp));
      }

      // The positioning system Mode Indicator and Status fields shall not be null fields.
      // Data valid
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      if (!FetchField(start, result))
         return;

      if (*result == 'A') {
         // Fix Type
         if (g_gpsInfo.FixType == 0)
            g_gpsInfo.FixType = 1;   // Assume 2D, if there's no other info.

         // Fix Service
         start = strstr(start, ",");
         if (start != NULL)
             start = start +1;
         else
            return;

         if (!FetchField(start, result))
            return;

         FixService = *result;

         switch (FixService) {
            case 'A':
            {
               g_gpsInfo.FixService = 1;
               break;
            }
            case 'D':
            {
               g_gpsInfo.FixService = 2;
               break;
            }
            case 'E':
            {
               g_gpsInfo.FixService = 6;
               break;
            }
         }
      } else {  // Data invalid
         g_gpsInfo.FixType = 0;    // NoFix
         g_gpsInfo.FixService = 0; // NoFix
      }
   }
}
//---------------------------------------------------------------------------
void RMC_Parse( char *head)
{
   char *start, result[20], tmp[20], *point;
   unsigned short len=0;
   LOGD("%s", head);
   // check checksum
   if (CheckSum(head, strlen(head))) {
      // UTC time : 161229.487
      start = strstr(head, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, tmp)) {
         // Hour
         strncpy(result, tmp, 2);
         result[2]='\0';
         g_gpsInfo.hour = atoi(result);
         // Min
         strncpy( result, tmp+2, 2);
         result[2]='\0';
         g_gpsInfo.min = atoi(result);
         // Sec
         strncpy(result, tmp+4, strlen(tmp)-4);
         result[strlen(tmp)-4]='\0';
         g_gpsInfo.sec = (float)(atof(result));
      }

      // valid
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField(start, result))
         return;

      if (*result == 'A') {
         if (g_gpsInfo.FixType == 0)
            g_gpsInfo.FixType = 1;      // Assume 2D

         if (g_gpsInfo.FixService == 0)
            g_gpsInfo.FixService = 1;   // Assume SPS
      } else {
         g_gpsInfo.FixType = 0;    // NoFix
         g_gpsInfo.FixService = 0;  // NoFix
      }

      // Position(Lat) : 3723.2475(N)
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         point = strstr( result, ".");
         if (point != NULL){
             len = (point-2)-result;
             strncpy(tmp, result, len);
             tmp[len]='\0';
             g_gpsInfo.Lat = (float)(atoi(tmp));
             strncpy(tmp, result+len, strlen(result)-len);
             tmp[strlen(result)-len]='\0';
             g_gpsInfo.Lat += (float)(atof(tmp)/60.0);
         }
      } else {  // Can not fetch Lat field
         g_gpsInfo.Lat = 0;
      }

      // N or S
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result) && g_gpsInfo.Lat != 0) {
         if (*result == 'S')
            g_gpsInfo.Lat = -g_gpsInfo.Lat;
      }

      // Position(Lon) : 12158.3416(W)
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         point = strstr(result, ".");
         if (point != NULL){
             len = (point-2)-result;
             strncpy(tmp, result, len);
             tmp[len]='\0';
             g_gpsInfo.Lon = (float)(atoi(tmp));
             strncpy(tmp, result+len, strlen(result)-len);
             tmp[strlen(result)-len]='\0';
             g_gpsInfo.Lon += (float)(atof(tmp)/60.0);
         }
      }
      else {  // Can not fetch Lat field
         g_gpsInfo.Lon = 0;
      }

      // E or W
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result) && g_gpsInfo.Lat != 0) {
         if (*result == 'W')
            g_gpsInfo.Lon = -g_gpsInfo.Lon;
      }

      // Speed : 0.13
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         g_gpsInfo.Speed = (float)(atof(result) * Knot2Kmhr);
      }

      // Track : 309.62
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result))
      {
         g_gpsInfo.Track = (float)(atof(result));
      }

      // Date : 120598
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         // Day
         strncpy(tmp, result, 2);
         tmp[2]='\0';
         g_gpsInfo.day=atoi(tmp);

         // Month
         strncpy(tmp, result+2, 2);
         tmp[2]='\0';
         g_gpsInfo.mon=atoi(tmp);

         // Year
         strncpy(tmp, result+4, 2);
         tmp[2]='\0';
         g_gpsInfo.year=atoi(tmp)+2000;
      }

      // skip Magnetic variation
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      // mode indicator
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      if (!FetchField(start, result))
         return;

      if (g_gpsInfo.FixType > 0) {
         switch (result[0]) {
            case 'A': {
               g_gpsInfo.FixService = 1;
               break;
            }
            case 'D':  {
               g_gpsInfo.FixService = 2;
               break;
            }
            case 'E': {
               g_gpsInfo.FixService = 6;
               break;
            }
         }
      }
   }
}
//---------------------------------------------------------------------------
void VTG_Parse( char *head)
{
   //$GPVTG,159.16,T,,M,0.013,N,0.023,K,A*34
   char *start, result[20];
   char FixService;

   // check checksum
   if (CheckSum(head, strlen(head)))
   {
      // Track
      start = strstr(head, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         g_gpsInfo.Track = (float)(atof(result));
      }

      // ignore
      start = strstr(start, ",");     // T
      if (start != NULL)
         start = start +1;
      else
         return;

      start = strstr(start, ",");     // NULL
      if (start != NULL)
         start = start +1;
      else
         return;

      start = strstr(start, ",");     // M
      if (start != NULL)
         start = start +1;
      else
         return;

      // Speed
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         g_gpsInfo.Speed = (float)(atof(result) * Knot2Kmhr);
      }

      // ignore
      start = strstr(start, ",");     // N
      if (start != NULL)
         start = start +1;
      else
         return;

      start = strstr(start, ",");     // 0.023
      if (start != NULL)
         start = start +1;
      else
         return;

      start = strstr(start, ",");     // K
      if (start != NULL)
         start = start +1;
      else
         return;

      // Fix Service
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      if (!FetchField(start, result))
         return;
      FixService = *result;

      if (FixService != 'N') {
         if (g_gpsInfo.FixType == 0)
            g_gpsInfo.FixType = 1;  // Assume 2D

         switch (FixService) {
            case 'A': {
               g_gpsInfo.FixService = 1;
               break;
            }
            case 'D': {
               g_gpsInfo.FixService = 2;
               break;
            }
            case 'E': {
               g_gpsInfo.FixService = 6;
               break;
            }
         }
      }
      else {  // NoFix
         g_gpsInfo.FixType = 0;    // NoFix
         g_gpsInfo.FixService = 0;  // NoFix
      }
   }
}
//---------------------------------------------------------------------------
void GSA_Parse( char *head)
{
   char *start, result[20];
   short sv_cnt=0, i;
   LOGD("%s", head);
   if (CheckSum(head, strlen(head)))
   {
      //Fix SV
      memset(&g_gpsInfo.fixSV, 0, sizeof(g_gpsInfo.fixSV));

      //Valid
      start = strstr( head, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField(start, result))
         return;

      if ((*result == 'A') || (*result == 'M')) {
         // Fix Type
         start = strstr(start, ",");
         if (start != NULL)
            start = start +1;
         else
            return;
         if (!FetchField(start, result))
            return;
         g_gpsInfo.FixType = atoi(result)-1;

         if (g_gpsInfo.FixType > 0) {         // Fix
            if (g_gpsInfo.FixService == 0)
               g_gpsInfo.FixService = 1;    //Assume SPS FixSerivce
         } else {
            g_gpsInfo.FixType = 0;    // NoFix
            g_gpsInfo.FixService = 0; // NoFix
         }
      } else {
         g_gpsInfo.FixType = 0;    // NoFix
         g_gpsInfo.FixService = 0; // NoFix
      }

      for (i = 0; i < 12; i++) {
         start = strstr(start, ",");
         if (start != NULL)
            start = start +1;
         else
            return;

         FetchField(start, result);

         if (strlen(result) > 0)
            g_gpsInfo.fixSV[sv_cnt++] = atoi(result);
      }

      //PDOP
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField(start, result))
         return;
      g_gpsInfo.PDOP = (float)(atof(result));

      //HDOP
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField(start, result))
         return;
      g_gpsInfo.HDOP = (float)(atof(result));

      //VDOP
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField(start, result))
         return;
      g_gpsInfo.VDOP = (float)(atof(result));
   }
}
#if 0
//---------------------------------------------------------------------------
void GSV_Parse( char *head)
{
   char *start, result[20];
   unsigned short sv_cnt=0, i;
   short base;

   // check checksum
   if (CheckSum(head, strlen(head)))
   {
      // ignore
      start = strstr( head, ",");
      if (start != NULL)
         start = start + 1;
      else
         return;

      //first Message
      if (*(start+2) == '1')
      {
         memset( &g_svInfo, 0, sizeof(g_svInfo));
         //g_fgSVUpdate = false;
      }
      //base  //sentence number.
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField(start, result))
         return;
      base = (atoi(result)-1)*4;
      if (base < 0){
         return;
      }
      //total
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField(start, result))
         return;
      g_gpsInfo.SV_cnt = atoi(result);
      if (g_gpsInfo.SV_cnt == 0)
         return;

      for (i = 0; i < 4; i++) {
         //SVid
         start = strstr(start, ",");
         if (start != NULL)
            start = start +1;
         else
            return;

         if (((base+sv_cnt)) < 0 || ((base+sv_cnt) >= 20)){
            return;
         }
         FetchField(start, result);
         if (strlen(result) > 0)
            g_svInfo[base+sv_cnt].SVid = atoi(result);
         else
            g_svInfo[base+sv_cnt].SVid = 0;

         //elev
         start = strstr(start, ",");
         if (start != NULL)
            start = start +1;
         else
            return;
         FetchField(start, result);
         if (strlen(result) > 0)
            g_svInfo[base+sv_cnt].elv = atoi(result);
         else
            g_svInfo[base+sv_cnt].elv = 0;

         //azimuth
         start = strstr(start, ",");
         if (start != NULL)
            start = start +1;
         else
            return;
         FetchField( start, result);
         if (strlen(result) > 0)
            g_svInfo[base+sv_cnt].azimuth = atoi(result);
         else
            g_svInfo[base+sv_cnt].azimuth = 0;

         //SNR
         start = strstr(start, ",");
         if (start != NULL)
            start = start + 1;
         else
            return;
         if (*start == '*')
            g_svInfo[base+sv_cnt].SNR = 0;
         else {
            FetchField( start, result);
            if (strlen(result) > 0)
               g_svInfo[base+sv_cnt].SNR = atoi(result);
            else
               g_svInfo[base+sv_cnt].SNR = 0;
         }

         sv_cnt++;

         if (base+sv_cnt == g_gpsInfo.SV_cnt)
            break;
      }
   }
}
#else
void GSV_Parse( char *head)
{
    LOGD("%s", head);
    // check checksum
    if (CheckSum(head, strlen(head))) {
        NmeaTokenizer tzer = {0};
        SVInfo  *p_sv_info = NULL;
        memset(&tzer, 0, sizeof(NmeaTokenizer));
        char *end = head+strlen(head);
        if(end == NULL) {
           LOGW("GSV no end!!!");
           return;
        }
        nmea_tokenizer_init(&tzer, head, end);
    #ifdef FTM_GPS_NMEA_DEBUG
        LOGD("find token's count:%d", tzer.count);
        for(int i = 0; i < tzer.count; i++) {
            Token tok = nmea_tokenizer_get(&tzer, i);
            LOGD("[%2d]: '%.*s'", i, tok.end-tok.p, tok.p);
        }
    #endif
        Token tok_num = nmea_tokenizer_get(&tzer, 1);  // number of messages
        Token tok_seq = nmea_tokenizer_get(&tzer, 2);  // sequence number
        Token tok_cnt = nmea_tokenizer_get(&tzer, 3);  // Satellites in view
        int num = str2int(tok_num.p, tok_num.end);
        int seq = str2int(tok_seq.p, tok_seq.end);
        int cnt = str2int(tok_cnt.p, tok_cnt.end);
        int sglid = SIGNAL_ID_GP_L1_CA;
        int sv_base = (seq - 1)*NMEA_MAX_SV_INFO;
        int sv_num = cnt - sv_base;
        int idx, base = 4, base_idx;
        int sv_idx = 0;
        if (sv_num > NMEA_MAX_SV_INFO)
            sv_num = NMEA_MAX_SV_INFO;

        //$GPGSV,2,2,5,30,25,135,58.0,1*71
        //$GPGSV,2,1,5,06,74,090,55.9,02,51,138,41.3,13,83,206,50.5,09,67,108,60.4,7*55
        if(tzer.count % 4 == 1) {  //Get signal ID, for NMEA4.10
            Token tok_sglid = nmea_tokenizer_get(&tzer, tzer.count - 1);  //Signal ID
            sglid = str2int(tok_sglid.p, tok_sglid.end);
        }

        if(gps_l5_support() && sglid != SIGNAL_ID_GP_L1_CA) {  //GPS & Beidou & GA L5 signal, 1
            p_sv_info = g_svL5Info;
        } else {  //GPS & Beidou & GA L1 signal, 1
            p_sv_info = g_svInfo;
        }

        for (idx = 0; idx < sv_num; idx++) {
            base_idx = base*(idx+1);
            Token tok_id  = nmea_tokenizer_get(&tzer, base_idx+0);
            p_sv_info[sv_base+sv_idx].SVid = str2int(tok_id.p, tok_id.end);

            Token tok_ele = nmea_tokenizer_get(&tzer, base_idx+1);
            p_sv_info[sv_base+sv_idx].elv = str2int(tok_ele.p, tok_ele.end);

            Token tok_azi = nmea_tokenizer_get(&tzer, base_idx+2);
            p_sv_info[sv_base+sv_idx].azimuth = str2int(tok_azi.p, tok_azi.end);

            Token tok_cn0 = nmea_tokenizer_get(&tzer, base_idx+3);
            p_sv_info[sv_base+sv_idx].SNR = str2float(tok_cn0.p, tok_cn0.end);

            sv_idx++;
        }
        if(gps_l5_support() && sglid != SIGNAL_ID_GP_L1_CA) {
            g_gpsInfo.SV_cnt_l5 = cnt;
            LOGD("Find %d L5 SVs", g_gpsInfo.SV_cnt_l5);
        } else {
            g_gpsInfo.SV_cnt = cnt;
            LOGD("Find %d L1 SVs", g_gpsInfo.SV_cnt);
        }
    }
}
#endif
//---------------------------------------------------------------------------
void GGA_Parse( char *head)
{
   //$GPGGA,144437.000,2446.367638,N,12101.356226,E,1,9,0.95,155.696,M,15.057,M,,*58
   char *start, result[20], tmp[20], *point;
   unsigned short len=0;
   LOGD("%s", head);
   // check checksum
   if (CheckSum(head, strlen(head))) {
      // UTC time : 144437.000
      start = strstr(head, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      if (FetchField(start, result)) {
         // Hour
         strncpy( tmp, result, 2);
         tmp[2]='\0';
         g_gpsInfo.hour = atoi(tmp);
         // Min
         strncpy( tmp, result+2, 2);
         tmp[2]='\0';
         g_gpsInfo.min = atoi(tmp);
         // Sec
         strncpy(tmp, result + 4, strlen(result) - 4);
         tmp[strlen(result) - 4]='\0';
         g_gpsInfo.sec = (float)(atof(tmp));
      }

      // Position(Lat)
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         point = strstr(result, ".");
         if (point != NULL){
             len = (point-2)-result;
             strncpy(tmp, result, len);
             tmp[len]='\0';
             g_gpsInfo.Lat = (float)(atoi(tmp));
             strncpy(tmp, result + len, strlen(result) - len);
             tmp[strlen(result) - len]='\0';
             g_gpsInfo.Lat += (float)(atof(tmp)/60.0);
         }
      }

      // N or S
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         if (*result == 'S')
            g_gpsInfo.Lat = -g_gpsInfo.Lat;
      }

      // Position(Lon)
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result))
      {
         point = strstr( result, ".");
         if (point != NULL){
             len = (point-2)-result;
             strncpy(tmp, result, len);
             tmp[len]='\0';
             g_gpsInfo.Lon = (float)(atoi(tmp));
             strncpy(tmp, result+len, strlen(result)-len);
             tmp[strlen(result)-len]='\0';
             g_gpsInfo.Lon += (float)(atof(tmp)/60.0);
         }
      }

      // E or W
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         if (*result == 'W')
            g_gpsInfo.Lon = -g_gpsInfo.Lon;
      }

      // GPS Fix Type and Service
      // 0: NoFix, 1:SPS, 2:DGPS, 6:Estimate
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (!FetchField( start, result))
         return;
      g_gpsInfo.FixService = atoi(result);

      // Fix
      if (g_gpsInfo.FixService > 0)
      {
         if (g_gpsInfo.FixType == 0)
            g_gpsInfo.FixType = 1; // Assume 2D
      }

      start = strstr(start, ",");   // Number of SV in use , ex :9
      if (start != NULL)
         start = start +1;
      else
         return;

      // HDOP
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         g_gpsInfo.HDOP = (float)(atof(result));
      }

      //Altitude (mean sea level)
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result)) {
         g_gpsInfo.Alt = (float)(atof(result));
      }

      // Altitude unit (bypass)
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      // Altitude (Geoidal separation)
      start = strstr(start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      if (FetchField(start, result)) {
         g_gpsInfo.Alt += (float)(atof(result));
      }
   }
}
// ---------------------------------------------------------------------------
void ZDA_Parse( char *head)
{
   // $GPZDA,000007.123,06,01,2000,,*50
   char *start, result[20], tmp[20];

   // check checksum
   if (CheckSum(head, strlen(head)))
   {
      // UTC time : 000007.123
      start = strstr( head, ",");
      if (start != NULL)
         start = start +1;
      else
         return;

      if (FetchField( start, result))
      {
         // Hour
         strncpy( tmp, result, 2);
         tmp[2]='\0';
         g_gpsInfo.hour = atoi(tmp);

         // Min
         strncpy( tmp, result+2, 2);
         tmp[2]='\0';
         g_gpsInfo.min = atoi(tmp);

         // Sec
         strncpy( tmp, result+4, strlen(result)-4);
         tmp[strlen(result)-4]='\0';
         g_gpsInfo.sec = (float)(atof(tmp));
      }

      // Day
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField( start, result))
         g_gpsInfo.day = atoi(result);

      // Month
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField( start, result))
         g_gpsInfo.mon = atoi(result);

      // Year
      start = strstr( start, ",");
      if (start != NULL)
         start = start +1;
      else
         return;
      if (FetchField(start, result))
         g_gpsInfo.year = atoi(result);
   }
}

void gps_info_clear() {
    memset(&g_gpsInfo, 0, sizeof(g_gpsInfo));
    memset(g_svInfo, 0, (sizeof(SVInfo)*NUM_CH));
    memset(g_svL5Info, 0, (sizeof(SVInfo)*NUM_CH));
    memset(g_chInfo, 0, (sizeof(ChInfo)*PSEUDO_CH));
}

void NMEA_Parse(char *TempBuf)
{
    char *GP;
    char type[4];
    // char TempBuf[500];
    // strcpy(TempBuf,g_NMEAbuf);
    GP=strtok(TempBuf, "\r\n");

    gps_info_clear();

    if (GP == NULL)
        return;

    do {
        // Channel Status
        if (strncmp(GP, "$PMTKCHN", 8) == 0) {
            // Channel_Parse(GP);
        } else if (strncmp(GP, "$PMTK", 5) == 0) {   // Ack Parse
            // Ack_Parse(GP);
        } else if (/*(strncmp(GP, "$GP", 3) == 0) &&*/ (strlen(GP) > 10)) {  // NMEA Parse
            // skip "$GP" char to fetch Message Type
            strncpy(type, GP+3, 3);
            type[3]='\0';

            if (strcmp(type, "GLL") == 0) {
                GLL_Parse(GP);
            } else if (strcmp(type, "RMC") == 0) {
                RMC_Parse(GP);
            } else if (strcmp(type, "VTG") == 0) {
                VTG_Parse(GP);
            } else if (strcmp(type, "GSA") == 0) {
                GSA_Parse(GP);
            } else if (strcmp(type, "GSV") == 0) {
                GSV_Parse(GP);
            } else if (strcmp(type, "GGA") == 0) {
                GGA_Parse(GP);
            } else if (strcmp(type, "ZDA") == 0) {
                ZDA_Parse(GP);
            }
        } else if((strncmp(GP, "$GN", 3) == 0) && (strlen(GP) > 10)) {
            // skip "$GN" char to fetch Message Type
            strncpy(type, GP+3, 3);
            type[3]='\0';

            if (strcmp(type, "GLL") == 0) {
                GLL_Parse(GP);
            } else if (strcmp(type, "RMC") == 0) {
                RMC_Parse(GP);
            } else if (strcmp(type, "VTG") == 0) {
                VTG_Parse(GP);
            } else if (strcmp(type, "GSA") == 0) {
                GSA_Parse(GP);
            } else if (strcmp(type, "GGA") == 0) {
                GGA_Parse(GP);
            } else if (strcmp(type, "ZDA") == 0) {
                ZDA_Parse(GP);
            }
        }
    } while ((GP = strtok(NULL, "\r\n")) != NULL);
}

static bool gps_update_info(struct gps_desc *gps, char *info) {
    char *ptr;
    short i = 0;
    short read_leng = 0;
    short total_leng = 0;
    int offset_ptr = 0;

    if (fd_mnld2factory != C_INVALID_SOCKET) {
        memset(nmea_buf, 0, NMEA_SIZE);

        LOGD(TAG"read from fd_mnld2factory 1\n");
        read_leng = safe_recvfrom_block(fd_mnld2factory, &nmea_buf[total_leng], (NMEA_SIZE - total_leng));
        total_leng += read_leng;
        LOGD(TAG"read_leng=%d, total_leng=%d\n", read_leng, total_leng);

        if (read_leng <= 0) {
            LOGD(TAG"ERROR reading from socket\n");
            FTM_SPRINTF(gps->info, "%s\n", uistr_info_gps_error);
            gps->exit_thd = true;
            return false;
        }
        else if (total_leng > 0) {
            NMEA_Parse((char*)&nmea_buf[0]);

            ptr  = info;
            open_time++;
            if (((g_gpsInfo.FixType != 0) && (ttff != 0)/*avoid prev second's NMEA*/)
             || (fixed == 1)) {  // 2D or 3D fixed
                offset_ptr = sprintf(ptr, "%s: %d\n", uistr_info_gps_ttff, open_time);
                if(offset_ptr  < 0) {
                    LOGD(TAG"sprintf failed");
                } else {
                    ptr += offset_ptr;
                }
                offset_ptr = sprintf(ptr, "%s: %d\n", uistr_info_gps_fixed, ttff);
                if(offset_ptr  < 0) {
                    LOGD(TAG"sprintf failed");
                } else {
                    ptr += offset_ptr;
                }
                fixed = 1;
                // for auto test
                LOGD(TAG"Fix success");
                // ttff_check_res = 1;
            } else if ((g_gpsInfo.FixType != 0) && (ttff == 0)) {  // skip prev second's NMEA, clear data
                offset_ptr = sprintf(ptr, "%s: %d\n", uistr_info_gps_ttff, ttff++);
                if(offset_ptr  < 0) {
                    LOGD(TAG"sprintf failed");
                } else {
                    ptr += offset_ptr;
                }
                LOGD(TAG"gps_info_clear");
                gps_info_clear();
            }
            else {   // no fix
                offset_ptr = sprintf(ptr, "%s: %d\n", uistr_info_gps_ttff, ttff++);
                if(offset_ptr  < 0) {
                    LOGD(TAG"sprintf failed");
                } else {
                    ptr += offset_ptr;
                }
                LOGD(TAG"no fix");
            }
            if(gps_l5_support()) {
                LOGD(TAG"Find %d L1 SVs. %d L5 SVs", g_gpsInfo.SV_cnt, g_gpsInfo.SV_cnt_l5);
                for (i = 0; i < g_gpsInfo.SV_cnt; i++) {
                    if (g_svInfo[i].SNR > 0) {
                        offset_ptr = sprintf(ptr, "L1 %s[%d] : %d\n", uistr_info_gps_svid, g_svInfo[i].SVid, g_svInfo[i].SNR);
                        if(offset_ptr < 0) {
                            LOGD(TAG"sprintf failed");
                        } else {
                            ptr += offset_ptr;
                        }
                        ttff_check_res = 1;
                    }
                }
                for (i = 0; i < g_gpsInfo.SV_cnt_l5; i++) {
                    if (g_svL5Info[i].SNR > 0) {
                        offset_ptr =  sprintf(ptr, "L5 %s[%d] : %d\n", uistr_info_gps_svid, g_svL5Info[i].SVid, g_svL5Info[i].SNR);
                        if(offset_ptr < 0) {
                            LOGD(TAG"sprintf failed");
                        } else {
                            ptr += offset_ptr;
                        }
                        ttff_l5_check_res = 1;
                    }
                }
            } else {
                LOGD(TAG"Find %d SVs", g_gpsInfo.SV_cnt);
                for (i = 0; i < g_gpsInfo.SV_cnt; i++) {
                    if (g_svInfo[i].SNR > 0) {
                        offset_ptr =  sprintf(ptr, "%s[%d] : %d\n", uistr_info_gps_svid, g_svInfo[i].SVid, g_svInfo[i].SNR);
                        if(offset_ptr < 0) {
                            LOGD(TAG"sprintf failed");
                        } else {
                            ptr += offset_ptr;
                        }
                        ttff_check_res = 1;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

static void* gps_update_ui(void *priv) {
    int wait_time = 10;
    int init_try_time = 0;
    struct gps_desc *gps = (struct gps_desc *)priv;
    struct itemview *iv;
    char GPS_result[32] = {0};
    iv = gps->iv;
    while (!init_flag && wait_time) {
        LOGD(TAG" init_flag = %d, try time = %d\n", init_flag, ++init_try_time);
        sleep(1);
        --wait_time;
    }
    init_flag = 0;
    wait_time = 10;
    init_try_time = 0;
    LOGD(TAG" init_flag = %d\n", init_flag);
    do {
        // Auto test: only CTTFF
        cttff = 1;
        do {
            usleep(1000000);  // check status per sec
            LOGD(TAG"after %d sec from test starting, we still can not see any SV\n", open_time);
        } while (open_time <= timeout && !gps_ttff_check_res());

        if (gps_ttff_check_res()) {
            LOGD(TAG"Test pass");
            gps->mod->test_result = FTM_TEST_PASS;
        } else {
            LOGD(TAG"Test fail");
            gps->mod->test_result = FTM_TEST_FAIL;
        }
        if (unlink("/data/misc/mtkgps.dat") != 0)
            LOGD(TAG "unlink mtkgps.dat error, error is %s\n", strerror(errno));
        ttff_check_res = 0;
        ttff_l5_check_res = 0;
        unlink(GPS_PROPERTY);

    } while (0);

    if (GPS_param.test_type == 3) {
        FTM_SPRINTF(GPS_result, "%d:%s", gps->mod->id, result[gps->mod->test_result]);
        write_data_to_pc(GPS_result, strlen(GPS_result));
    }
    gps->exit_thd = true;
    LOGD(TAG "%s, gps->exit_thd = true\n", __FUNCTION__);
    pthread_join(gps->update_thd, NULL);

    return NULL;
}
static void *gps_update_iv_thread(void *priv)
{
    struct gps_desc *gps = (struct gps_desc *)priv;
    struct itemview *iv = gps->iv;
    short chkcnt = 10;
    int init_status;
    LOGD(TAG "%s: Start\n", __FUNCTION__);
    memset(gps->info, 0, sizeof(gps->info));
    gps_update_info(gps, gps->info);
    // init GPS driver
    memset(gps->info, 0, sizeof(gps->info));
    FTM_SPRINTF(gps->info, "%s\n", uistr_info_gps_init);
    if (GPS_param.test_type != 3) {  /* 3 means test on background */
        iv->redraw(iv);
    }
    sleep(1);
    init_status = GPS_Open();
    if (init_status != 0) {   // GPS init fail
        memset(gps->info, 0, sizeof(gps->info));
        FTM_SPRINTF(gps->info, "%s (%d)\n", uistr_info_gps_error, init_status);
        if (GPS_param.test_type != 3) {
            iv->redraw(iv);
        }
        timeout = -1;
    } else {
        // init GPS driver done
        ttff = 0;
        open_time = 0;
        fixed = 0;
        while (1) {
            usleep(100000);  // wake up every 0.1sec
            chkcnt--;

            if (gps->exit_thd) {
                LOGD(TAG "%s, gps->exit_thd = true\n", __FUNCTION__);
                break;
            }

            if (httff == 1) {
                httff = 0;
                if (factory_safe_sendto(FACTORY_TO_MNLD_PMTK, "$PMTK101*32\r\n", sizeof("$PMTK101*32\r\n")) == -1) {
                    LOGD(TAG "factory_safe_sendto failed!", __FUNCTION__);
                }
                ttff = 0;
                open_time = 0;
                fixed = 0;
                memset(gps->info, 0, sizeof(gps->info));
                gps->info[INFO_SIZE-1] = 0x0;
                if (GPS_param.test_type != 3) {
                    iv->redraw(iv);
                }
            }

            if (cttff == 1) {
                cttff = 0;
                if (factory_safe_sendto(FACTORY_TO_MNLD_PMTK, "$PMTK103*30\r\n", sizeof("$PMTK103*30\r\n")) == -1) {
                    LOGD(TAG "factory_safe_sendto failed!", __FUNCTION__);
                }
                ttff = 0;
                open_time = 0;
                fixed = 0;
                memset(gps->info, 0, sizeof(gps->info));
                gps->info[INFO_SIZE-1] = 0x0;
                if (GPS_param.test_type != 3) {
                    iv->redraw(iv);
                }
            }

            if (chkcnt > 0)
                continue;

            chkcnt = 10;
            memset(gps->info, 0, sizeof(gps->info));

            if(gps_update_info(gps, gps->info)) {
                LOGD(TAG " gps->info %s:\n", gps->info);
                if (GPS_param.test_type != 3) {
                    iv->redraw(iv);
                }
            } else {
                LOGD(TAG " don't redraw gps information gps->info %s:\n", gps->info);
            }
        }
    }
    //close GPS driver
#ifdef FEATURE_FTM_GPS_T6633_NOT_CLOSE_GPS
    //GPS_Close();
    LOGD(TAG "%s: ---do not close gps for t6633 gps ata test crash---\n", __FUNCTION__);
#else
	GPS_Close();
#endif
    LOGD(TAG "%s: Exit\n", __FUNCTION__);
    pthread_exit(NULL);
    usleep(1000000);
    return NULL;
}

int gps_entry(struct ftm_param *param, void *priv)
{
    char *pTimeout = NULL;
    int chosen;
    bool exit = false;
    #ifdef FEATURE_FTM_TOUCH_MODE
    text_t* lbtn = NULL;
    text_t* cbtn = NULL;
    text_t* rbtn = NULL;
    #endif
    struct gps_desc *gps = (struct gps_desc *)priv;
    struct itemview *iv;
    GPS_param.test_type = param->test_type;
    gps_info_clear();
    pTimeout = ftm_get_prop("GPS.TIMEOUT");
    if (pTimeout != NULL) {
        timeout = atoi(pTimeout);
        LOGD(TAG "timeout value is %d\n", timeout);
    } else {
        timeout = 60;
        LOGD(TAG "Set timeout value as default: %d\n", timeout);
    }
    LOGD(TAG "%s\n", __FUNCTION__);

    init_text(&gps->title, param->name, COLOR_YELLOW);
    init_text(&gps->text, &gps->info[0], COLOR_YELLOW);

    // gps_update_info(gps, gps->info);

    gps->exit_thd = false;

    if (!gps->iv) {
        iv = ui_new_itemview();
        if (!iv) {
            LOGD(TAG "No memory");
            return -1;
        }
        gps->iv = iv;
    }

    iv = gps->iv;
    if (param->test_type == FTM_MANUAL_ITEM) {
#ifdef FEATURE_FTM_TOUCH_MODE
        lbtn = malloc(sizeof(struct itemview));
        cbtn = malloc(sizeof(struct itemview));
        rbtn = malloc(sizeof(struct itemview));
        memset(lbtn, 0, sizeof(text_t));
        memset(cbtn, 0, sizeof(text_t));
        memset(rbtn, 0, sizeof(text_t));

        init_text(lbtn, uistr_key_fail, COLOR_YELLOW);
        init_text(cbtn, uistr_key_back, COLOR_YELLOW);
        init_text(rbtn, uistr_key_pass, COLOR_YELLOW);
        iv->set_btn(iv, lbtn, cbtn, rbtn);
#endif
        iv->set_title(iv, &gps->title);
        iv->set_items(iv, gps_items, 0);
        iv->set_text(iv, &gps->text);
        iv->start_menu(iv, 0);
    } else {  // if (param->test_type == FTM_AUTO_ITEM)
        iv->set_title(iv, &gps->title);
        iv->set_items(iv, gps_items_auto, 0);
        iv->set_text(iv, &gps->text);

        iv->start_menu(iv, 0);
    }
    pthread_create(&gps->update_thd, NULL, gps_update_iv_thread, priv);
    if (GPS_param.test_type == 3) {
        pthread_create(&gps->update_ui, NULL, gps_update_ui, priv);
        iv->redraw(iv);
    }
    if (GPS_param.test_type != 3) {
        int wait_time = 10;
        int init_try_time = 0;
        while (!init_flag && wait_time) {
            LOGD(TAG" init_flag = %d, try time = %d\n", init_flag, ++init_try_time);
            sleep(1);
            --wait_time;
        }
        init_flag = 0;
        wait_time = 10;
        init_try_time = 0;
        LOGD(TAG" init_flag = %d\n", init_flag);
    }
    LOGD("GPS test type:%d", param->test_type);
    if (param->test_type == FTM_MANUAL_ITEM) {
        do {
            chosen = iv->run(iv, &exit);
            LOGD(TAG "%s, chosen = %d\n", __FUNCTION__, chosen);
            switch (chosen) {
            case ITEM_HTTFF:
                httff = 1;
                break;
            case ITEM_CTTFF:
                cttff = 1;
                break;
     #ifndef FEATURE_FTM_TOUCH_MODE
            case ITEM_PASS:
            case ITEM_FAIL:
                if (chosen == ITEM_PASS) {
                    gps->mod->test_result = FTM_TEST_PASS;
                } else if (chosen == ITEM_FAIL) {
                    gps->mod->test_result = FTM_TEST_FAIL;
                }
                exit = true;
                break;
     #endif

     #ifdef FEATURE_FTM_TOUCH_MODE
        case L_BTN_DOWN:
              gps->mod->test_result = FTM_TEST_FAIL;
                        exit = true;
            break;
        case C_BTN_DOWN:
            exit = true;
            break;
        case R_BTN_DOWN:
            gps->mod->test_result = FTM_TEST_PASS;
            exit = true;
            break;
     #endif
            }

            if (exit) {
                gps->exit_thd = true;
                LOGD(TAG "%s, gps->exit_thd = true\n", __FUNCTION__);
                break;
            }
        } while (1);
    } else if (param->test_type == FTM_AUTO_ITEM) {
        do {
            // Auto test: only CTTFF
            cttff = 1;
            do {
                usleep(1000000);  // check status per sec
                LOGD(TAG"after %d sec from test starting, we still can not see any SV\n", open_time);
            } while (open_time <= timeout && !gps_ttff_check_res());

            if (gps_ttff_check_res()) {
                LOGD(TAG"Test pass");
                gps->mod->test_result = FTM_TEST_PASS;
            } else {
                LOGD(TAG"Test fail");
                gps->mod->test_result = FTM_TEST_FAIL;
            } if (unlink("/data/misc/mtkgps.dat") != 0 )
                LOGD(TAG "unlink mtkgps.dat error, error is %s\n", strerror(errno));
            ttff_check_res = 0;
            ttff_l5_check_res = 0;
            unlink(GPS_PROPERTY);
        } while (0);

        gps->exit_thd = true;
        LOGD(TAG "%s, gps->exit_thd = true\n", __FUNCTION__);
    } else
        LOGD("unknown test type\n");

    if (GPS_param.test_type == 3)
        sleep(1);
    else
    pthread_join(gps->update_thd, NULL);
#ifdef FEATURE_FTM_TOUCH_MODE
    if (lbtn)
    free(lbtn);
    if (cbtn)
    free(cbtn);
    if (rbtn)
    free(rbtn);
#endif

    return 0;
}

int gps_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct gps_desc *gps;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_GPS, sizeof(struct gps_desc));
    gps  = mod_to_gps(mod);

    gps->mod      = mod;

    if (!mod)
        return -ENOMEM;

    ret = ftm_register(mod, gps_entry, (void*)gps);

    return ret;
}

#endif

