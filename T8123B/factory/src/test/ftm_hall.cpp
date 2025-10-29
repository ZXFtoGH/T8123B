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

#ifdef FEATURE_FTM_HALL

#ifdef __cplusplus
extern "C" {
#endif

#define TAG                 "[HALL] "
#define HALL_PATH "/proc/driver/cust_hall_state"

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

struct hall {
	char  info[1024];
	bool  avail;
	bool  exit_thd;
	int  test_type;

	pthread_t hall_update_thd;
	struct ftm_module *mod;
	struct itemview *iv;

	text_t    title;
	text_t    text;
	text_t    left_btn;
	text_t    center_btn;
	text_t    right_btn;
};

int g_result_hall = ITEM_FAIL;
int hall_state;


#define mod_to_typec(p)     (struct hall*)((char*)(p) + sizeof(struct ftm_module))


static int update_hall_state(struct hall *hds, char *info)
{
	char *ptr;
	int fd = -1;

	char ret = '9';
	int ret1 = 0;

	ptr  = info;
	hds->avail = false;
    
	ptr += sprintf(ptr, "Hall testing\n");
	fd = open(HALL_PATH, O_RDONLY, 0);
	if (fd < 0) {
		LOGD(TAG "Can't open %s\n", HALL_PATH);
		ptr += sprintf(ptr, "can't test hall!!\n");
	} else {
		if (read(fd, &ret, sizeof(ret)) <= 0) {
			LOGD(TAG "Read fail (ret1)\n");
		}
		close(fd);
	}

	LOGD(TAG "ret=%d\n", ret);
	LOGD(TAG "hall_state=%d  \n", hall_state);
	/*success return char '1', fail return char '0'.*/
	if (ret == 0x30) {
		hall_state=1;
		
	} 

	if ((hall_state ==1)) {
		ptr += sprintf(ptr, "Hall test pass\n");
		return ITEM_PASS;
	} else {
		//ptr += sprintf(ptr, "CC pin test fail, please check cable!!\n");
		return ITEM_WAIT;
	}
}


static void *hall_update_iv_thread(void *priv)
{
	struct hall *hds = (struct hall *)priv;
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

		g_result_hall = update_hall_state(hds, hds->info);
		
		iv->set_text(iv, &hds->text);
		iv->redraw(iv);

		if ((hds->test_type == FTM_AUTO_ITEM) &&
			((retry == 0) || (g_result_hall == ITEM_PASS))) {
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


int hall_entry(struct ftm_param *param, void *priv)
{
	int chosen;
	bool exit = false;
	struct hall *hds = (struct hall *)priv;
	struct itemview *iv;
	LOGD(TAG "%s\n", __FUNCTION__);
	hall_state=0;

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
		hall_update_iv_thread(priv);
	} else if (FTM_MANUAL_ITEM == param->test_type) {
		pthread_create(&hds->hall_update_thd, NULL, hall_update_iv_thread, priv);
		do {
			if(!get_is_ata()) {
				chosen = iv->run(iv, &exit);
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
		pthread_join(hds->hall_update_thd, NULL);
	}

	LOGD(TAG "g_result_hall=%x\n", g_result_hall);

	if (FTM_AUTO_ITEM == param->test_type) {
		if (g_result_hall == ITEM_PASS) {
			hds->mod->test_result = FTM_TEST_PASS;
		} else {
			hds->mod->test_result = FTM_TEST_FAIL;
		}
	}

	return 0;
}

int hall_init(void)
{
    int ret = 0;
    struct ftm_module *mod;
    struct hall *hds;

    LOGD(TAG "%s\n", __FUNCTION__);

    mod = ftm_alloc(ITEM_HALL, sizeof(struct hall));
    if (!mod)
        return -ENOMEM;

    hds = mod_to_typec(mod);
    hds->mod = mod;
		//hds->avail	= false;
    ret = ftm_register(mod, hall_entry, (void*)hds);
    return ret;
}


#ifdef __cplusplus
}
#endif

#endif
