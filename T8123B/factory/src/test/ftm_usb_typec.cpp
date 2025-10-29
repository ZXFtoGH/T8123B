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

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/ioctl.h>

#include "common.h"
#include "miniui.h"
#include "ftm.h"

#ifdef FEATURE_FTM_USB_TYPEC

#ifdef __cplusplus
extern "C" {
#endif

#define TAG                 "[USB TYPEC] "
#define TYPEC_ERROR_COUNT_PATH "/proc/mtk_usb/smt_err_count"
//#define TYPEC_U2_CC "/proc/mtk_typec/smt_u2_cc_mode"
#define TYPEC_U2_CC "/sys/class/tcpc/type_c_port0/remote_state"
#define FUSB_SEL_PATH "/proc/mtk_typec/smt"
/* for new kernel version path */
#define TYPEC_LOOPBACK_PATH "/proc/mtk_usb/usb-phy0/u3_phy/loopback_test"
#define TYPEC_SWITCH_PATH "/proc/mtk_typec/mux_switch/switch"

enum {
	ITEM_PASS,
	ITEM_FAIL,
	ITEM_WAIT,
};

static item_t manual_menu_items[] = {
	item(ITEM_PASS, uistr_pass),
	item(ITEM_FAIL, uistr_fail),
	item(-1, NULL),
};

static item_t auto_menu_items[] = {
	item(-1, NULL),
};

struct usb_typec {
	char  info[1024];
	bool  avail;
	bool  exit_thd;
	int  test_type;

	pthread_t typec_update_thd;
	struct ftm_module *mod;
	struct itemview *iv;

	text_t    title;
	text_t    text;
	text_t    left_btn;
	text_t    center_btn;
	text_t    right_btn;
};

int g_result_usb_typec = ITEM_FAIL;
int typec_cc0_state;
int typec_cc1_state;

#define mod_to_typec(p)     (struct usb_typec*)((char*)(p) + sizeof(struct ftm_module))

static int check_sel_path(void)
{
	int fd_mux = -1;

	fd_mux = open(FUSB_SEL_PATH, O_WRONLY, 0);
	if (fd_mux < 0) {
		LOGD(TAG "Can't open %s\n", FUSB_SEL_PATH);

		fd_mux = open(TYPEC_SWITCH_PATH, O_WRONLY, 0);
		if (fd_mux < 0) {
			LOGD(TAG "Can't open %s\n", TYPEC_SWITCH_PATH);
			return 0;
		}
	}
	close(fd_mux);
	return 1;
}

static bool usb_cc_pin_test(void)
{
	int fd = -1;
	int cc_status = -1;
	int ret = 0;

	fd = open(TYPEC_U2_CC, O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s\n", TYPEC_U2_CC);
		goto end;
	}

	ret = read(fd, &cc_status, sizeof(cc_status));
	if (ret <= 0) {
		goto close_cc_fd;
	}

	LOGD(TAG "Result_CC = %d\n", cc_status);

	ret = cc_status;

close_cc_fd:
	close(fd);
end:
	return ret;
}

static bool usb_u3_loopback(void)
{
	int fd = -1;
	int fd_mux = -1;

	char ret = '9';
	int side_1 = 0;
	int side_2 = 0;

	fd_mux = open(FUSB_SEL_PATH, O_WRONLY, 0);
	if (fd_mux < 0) {
		LOGD(TAG "Can't open %s\n", FUSB_SEL_PATH);
		goto end;
	}

	/* Side 1 */
	write(fd_mux, "1", 1);

	fd = open(TYPEC_ERROR_COUNT_PATH, O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s\n", TYPEC_ERROR_COUNT_PATH);
	} else {
		if (read(fd, &ret, sizeof(ret)) <= 0) {
			LOGD(TAG "Read fail (ret1)\n");
		}
		close(fd);
	}

	/*success return char '1', fail return char '0'.*/
	if (ret == '1')
		side_1 = 1;
	else
		side_1 = 0;


	/* Side 2 */
	write(fd_mux, "2", 1);

	fd = open(TYPEC_ERROR_COUNT_PATH, O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s\n", TYPEC_ERROR_COUNT_PATH);
	} else {
		if (read(fd, &ret, sizeof(ret)) <= 0){
			LOGD(TAG "Read fail (ret2)\n");
		}
		close(fd);
	}

	if (ret == '1')
		side_2 = 1;
	else
		side_2 = 0;

	LOGD(TAG "side_1=%d, side_2=%d\n", side_1, side_2);
end:
	if (fd_mux >= 0)
		close(fd_mux);

	return side_1 && side_2;
}

static bool usb_u3_loopback2(void)
{
	int fd = -1;
	int fd_mux = -1;

	char ret = '9';
	int side_1 = 0;
	int side_2 = 0;

	fd_mux = open(TYPEC_SWITCH_PATH , O_WRONLY, 0);
	if (fd_mux < 0) {
		LOGD(TAG "Can't open %s\n", TYPEC_SWITCH_PATH );
		goto end;
	}

	/* Side 1 */
	write(fd_mux, "1", 1);
	/* Add Dealy to fix possible fail */
	usleep(5 * 200 * 1000);

	fd = open(TYPEC_LOOPBACK_PATH , O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s\n", TYPEC_LOOPBACK_PATH );
	} else {
		if (read(fd, &ret, sizeof(ret)) <= 0) {
			LOGD(TAG "Read fail (ret1)\n");
		}
		close(fd);
	}

	/*success return char '1', fail return char '0'.*/
	if (ret == '1')
		side_1 = 1;
	else
		side_1 = 0;


	/* Side 2 */
	write(fd_mux, "2", 1);
	/* Add Dealy to fix possible fail */
	usleep(5 * 200 * 1000);

	fd = open(TYPEC_LOOPBACK_PATH , O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s\n", TYPEC_LOOPBACK_PATH );
	} else {
		if (read(fd, &ret, sizeof(ret)) <= 0){
			LOGD(TAG "Read fail (ret2)\n");
		}
		close(fd);
	}

	if (ret == '1')
		side_2 = 1;
	else
		side_2 = 0;

	LOGD(TAG "side_1=%d, side_2=%d\n", side_1, side_2);
end:
	if (fd_mux >= 0)
		close(fd_mux);

	return side_1 && side_2;
}

static int update_cc_pin_info(struct usb_typec *hds, char *info)
{
	char *ptr;
	int fd = -1;

	char ret = '9';
	int ret1 = 0;

	ptr  = info;
	hds->avail = false;
    
	ptr += sprintf(ptr, "Type-C testing\n");
	fd = open(TYPEC_U2_CC, O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s\n", TYPEC_U2_CC);
		ptr += sprintf(ptr, "can't test CC pin!!\n");
	} else {
		if (read(fd, &ret, sizeof(ret)) <= 0) {
			LOGD(TAG "Read fail (ret1)\n");
		}
		close(fd);
	}

	LOGD(TAG "ret=%d\n", ret);
	LOGD(TAG "typec_cc0_state=%d     typec_cc1_state=%d\n", typec_cc0_state,typec_cc1_state);
	/*success return char '1', fail return char '0'.*/
	if (ret == 0x31) {
		typec_cc0_state=1;
		
	} else if(ret == 0x32){
		typec_cc1_state=1;
		
	}else {
		//ptr += sprintf(ptr, "CC pin test fail\n");
	}

	if(typec_cc0_state==1){
		ptr += sprintf(ptr, "CC0 pin test pass\n");
	}

	if(typec_cc1_state==1){
		ptr += sprintf(ptr, "CC1 pin test pass\n");
	}
	if ((typec_cc0_state ==1)&&(typec_cc1_state ==1)) {
		ptr += sprintf(ptr, "Type-c test pass\n");
		return ITEM_PASS;
	} else {
		//ptr += sprintf(ptr, "CC pin test fail, please check cable!!\n");
		return ITEM_WAIT;
	}
}

static int update_info(struct usb_typec *hds, char *info)
{
	char *ptr = info;
	hds->avail = false;

	if (usb_u3_loopback() || usb_u3_loopback2()) {
		ptr += sprintf(ptr, "loop back pass!!\n");
		return ITEM_PASS;
	} else {
		ptr += sprintf(ptr, "loop back fail, please check cable!!\n");
		return ITEM_FAIL;
	}
}

static void *typec_update_iv_thread(void *priv)
{
	struct usb_typec *hds = (struct usb_typec *)priv;
	struct itemview *iv = hds->iv;

	int retry = 600;

	LOGD(TAG "%s: Start\n", __FUNCTION__);

	while (1) {
		LOGD(TAG "%s: retry %d\n", __FUNCTION__, retry);
		usleep(200000);

		if ((hds->test_type == FTM_AUTO_ITEM) && (retry > 0)) {
			retry--;
		}

		if (hds->exit_thd)
			break;

		g_result_usb_typec = update_cc_pin_info(hds, hds->info);
		
		iv->set_text(iv, &hds->text);
		iv->redraw(iv);

		if ((hds->test_type == FTM_AUTO_ITEM) &&
			((retry == 0) || (g_result_usb_typec == ITEM_PASS))) {
			iv->start_menu(iv,0);
			iv->redraw(iv);
			//LOGD(TAG "%s: auto exit %s\n", __FUNCTION__ );
			break;
		}
	}

	LOGD(TAG "%s: Exit\n", __FUNCTION__);
	if (hds->test_type == FTM_AUTO_ITEM) {
		usleep(1000000);
	} else {
		pthread_exit(NULL);
	}

	return NULL;
}


int usb_typec_entry(struct ftm_param *param, void *priv)
{
	int chosen;
	bool exit = false;
	struct usb_typec *hds = (struct usb_typec *)priv;
	struct itemview *iv;
	LOGD(TAG "%s\n", __FUNCTION__);
	typec_cc0_state=0;
	typec_cc1_state=0;

	init_text(&hds->title, param->name, COLOR_YELLOW);
	init_text(&hds->text, &hds->info[0], COLOR_YELLOW);

	hds->exit_thd = false;
	hds->test_type = param->test_type;

	if (!hds->iv) {
		iv = ui_new_itemview();
		if (!iv) {
			LOGD(TAG "No memory");
			return -1;
		}
		hds->iv = iv;
	}

	iv = hds->iv;
	iv->set_title(iv, &hds->title);
	if (FTM_AUTO_ITEM == param->test_type) {
		iv->set_items(iv, auto_menu_items, 0);
	} else {
		iv->set_items(iv, manual_menu_items, 0);
	}
	iv->set_text(iv, &hds->text);

	if (FTM_AUTO_ITEM == param->test_type) {
		typec_update_iv_thread(priv);
	} else if (FTM_MANUAL_ITEM == param->test_type) {
		pthread_create(&hds->typec_update_thd, NULL, typec_update_iv_thread, priv);
		do {
			if(!get_is_ata()) {
				chosen = iv->run(iv, &exit);
			} else {
				if (check_sel_path()) {
					if(usb_u3_loopback() || usb_u3_loopback2())
						chosen = ITEM_PASS;
					else
						chosen = ITEM_FAIL;
				} else {
					if(usb_cc_pin_test())
						chosen = ITEM_PASS;
					else
						chosen = ITEM_FAIL;
				}
			}
			switch (chosen) {
			case ITEM_PASS:
			case ITEM_FAIL:
				if (chosen == ITEM_PASS) {
					hds->mod->test_result = FTM_TEST_PASS;
				} else if (chosen == ITEM_FAIL) {
					hds->mod->test_result = FTM_TEST_FAIL;
				}
				exit = true;
			break;
			}

			if (exit) {
				hds->exit_thd = true;
				break;
			}
		} while (1);
		pthread_join(hds->typec_update_thd, NULL);
	}

	LOGD(TAG "g_result_usb_typec=%x\n", g_result_usb_typec);

	if (FTM_AUTO_ITEM == param->test_type) {
		if (g_result_usb_typec == ITEM_PASS) {
			hds->mod->test_result = FTM_TEST_PASS;
		} else {
			hds->mod->test_result = FTM_TEST_FAIL;
		}
	}

	return 0;
}

int usb_typec_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct usb_typec *hds;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_USB_TYPEC, sizeof(struct usb_typec));
    if (!mod)
        return -ENOMEM;

    hds = mod_to_typec(mod);
    hds->mod = mod;
		//hds->avail	= false;
    ret = ftm_register(mod, usb_typec_entry, (void*)hds);
    return ret;
}


#ifdef __cplusplus
}
#endif

#endif
