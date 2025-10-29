/*
 * Copyright (C) 2013-2016, Shenzhen Huiding Technology Co., Ltd.
 * All Rights Reserved.
 */
#include "common.h"
#include "ftm.h"
#include "errno.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define TAG "[GOODIX_FINGERPRINT]"

/* define the dummy API for Goodix fingerprint not supported projects.
use the real API just when FP is supported and tested.
for fingerprint not supported projects, common source code also needed 
to be built to avoid "init API can't be find" error */

#if defined(FEATURE_FTM_GOODIX_FINGERPRINT) && defined(MTK_GOODIX_FINGERPRINT_ENABLED)
extern int gf_factory_test_entry(void) ;

#else
int gf_factory_test_entry()
{
	LOGE(TAG " [%s] enter.  this is dummy", __func__);
        return 1;
}
#endif

/* define old solution here  as backup */
//#ifdef MT6775
#define FINGERPRINT_FACTORY_OLD_SOLUTION
//#endif

#define mod_to_fingerprint(p)     (struct fingerprint*)((char*)(p) + sizeof(struct ftm_module))

struct fingerprint {
    struct ftm_module *mod;
    text_t title;
    struct itemview* iv;
};

static struct itemview *iv = NULL;

static item_t finger_item[] = {
    item(0, uistr_info_test_pass),
    item(1, uistr_info_test_fail),
    item(-1, NULL)
};

#ifdef FINGERPRINT_FACTORY_OLD_SOLUTION
static int fingerprint_test_old_solution(void)
{
	FILE *fd = NULL;
	int err = 0;
	int chipID_verify = 0;

	LOGD(TAG " [%s] enter", __func__);

	fd = fopen("/sys/module/gf_spi_tee/parameters/chipID_verify", "r");

	if (fd != NULL) {
		chipID_verify = fgetc(fd);
		if (chipID_verify < 0) {
			LOGE(TAG " [%s] get chipID_verify fail.", __func__);
			clearerr(fd);
		} else {
			LOGD(TAG " [%s] chipID_verify = %d  %c\n", __func__,
				chipID_verify, (char)chipID_verify);
		}
		err = fclose(fd);
		if (err < 0) {
			LOGE(TAG " [%s] enter,close fd fail", __func__);
			goto error;
		}
		LOGD(TAG " [%s] enter,close fd", __func__);
	} else {
		LOGE(TAG " [%s] open fingerprint fail.", __func__);
		goto error;
	}

	if ((char)chipID_verify == '1') {
		LOGD(TAG " [%s] %d fingerprint test success\n", __func__, __LINE__);
		return 0;
	}

	LOGE(TAG " [%s] %d error", __func__, __LINE__);

error:
	return -1;
}

#endif

static int goodix_fingerprint_entry(struct ftm_param *param, void *priv) {
    uint8_t is_pass = 0;

    LOGD(TAG " [%s] enter", __func__);
#ifdef FINGERPRINT_FACTORY_OLD_SOLUTION
	if (0 == fingerprint_test_old_solution()) {
		is_pass = 1;
	}
#else
    if (0 == gf_factory_test_entry()){
        is_pass = 1;
    }
#endif

    struct fingerprint* fp = (struct fingerprint*) priv;

    init_text(&fp->title, param->name, COLOR_YELLOW);

    if (NULL == fp->iv) {
        iv = ui_new_itemview();
        if (!iv) {
            LOGD(TAG " [%s] No memory for item view", __func__);
            return -1;
        }
        fp->iv = iv;
    }
    iv->set_title(iv, &fp->title);

    if (is_pass) {
        LOGI(TAG " [%s] test pass", __func__);
        fp->mod->test_result = FTM_TEST_PASS;
        iv->set_items(iv, finger_item, 0);
        iv->start_menu(iv, 0);
    } else {
        LOGI(TAG " [%s] test fail", __func__);
        fp->mod->test_result = FTM_TEST_FAIL;
        iv->set_items(iv, finger_item, 1);
        iv->start_menu(iv, 1);
    }

    iv->redraw(iv);
    usleep(500000);
    return 0;
}

int goodix_fingerprint_init(void) {
    int ret = 0;
    struct fingerprint *fp = NULL;
    struct ftm_module *mod = NULL;

    LOGE(TAG " [%s] enter", __func__);
    LOGD(TAG " [%s] enter", __func__);

    mod = ftm_alloc(ITEM_FINGERPRINT_TEST, sizeof(struct fingerprint));
    fp = mod_to_fingerprint(mod);
    memset(fp, 0x00, sizeof(*fp));
    if (!mod) {
        return -ENOMEM;
    }
    fp->mod = mod;
    ret = ftm_register(mod, goodix_fingerprint_entry, (void *) fp);

    LOGD(TAG " [%s] exit, ret = %d", __func__, ret);
    return ret;
}
