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

#include <arch/arm64.h>
#include "typedefs.h"
#include "platform.h"
#include "download.h"
#include "meta.h"
#include "sec.h"
#include <sboot.h>
#include "partition_api.h"
#include "dram_buffer.h"
#include "wdt.h"
#include "timer.h"
#include "log_store_pl.h"
#include "pmic.h"
#include <regulator/mtk_regulator.h>
#include "rpmb_key.h"
#include <ufs_aio_interface.h>
//#include "mt_ptp2.h"
#if CFG_ATF_SUPPORT
#include "tz_init.h"
#include "tz_mem.h"
#endif
#if (MTK_TINYSYS_SSPM_SUPPORT || CFG_LOAD_SLT_SSPM)
#include "sspm.h"
#include <emi_mpu_mt.h>
#endif
#include <pal_log.h>
#include <partition_api.h>
#include <partition_active.h>

#if CFG_GZ_SUPPORT
#include <gz_init.h>
#endif

#if defined(MTK_AB_OTA_UPDATER)
#include "bootctrl.h"
#include "ab_bootcheck.h"
#endif
#include <pal_log.h>
#include <pal_typedefs.h>

#if (MTK_TINYSYS_MCUPM_SUPPORT || CFG_LOAD_SLT_MCUPM)
#include "mcupm.h"
#endif

#if CFG_AEE_LK_MEMADDR
#include <string.h>
#include <ram_console.h>
#include <partition.h>

#define CFG_AEE_LK_IMG_NAME "aee"
#endif

#include "booker.h"

#include "emi.h"

/*============================================================================*/
/* CONSTAND DEFINITIONS                                                       */
/*============================================================================*/
#define MOD "[BLDR]"
#define ATF_DRAM_IMG_NAME "atf_dram"

/*============================================================================*/
/* MACROS DEFINITIONS                                                         */
/*============================================================================*/
#define CMD_MATCH(cmd1,cmd2)  \
    (!strncmp((const char*)(cmd1->data), (cmd2), min(strlen(cmd2), cmd1->len)))

#define ARM64_READ_SYSREG(reg) \
({ \
    uint64_t _val; \
    __asm__ volatile("mrs %0," #reg : "=r" (_val)); \
    _val; \
})

/*============================================================================*/
/* GLOBAL VARIABLES                                                           */
/*============================================================================*/
#define bootarg g_dram_buf->bootarg

/*============================================================================*/
/* STATIC VARIABLES                                                           */
/*============================================================================*/
bl_param_t *p_bldr_param = NULL;
static bl_param_t bldr_param;

/*============================================================================*/
/* EXTERN                                                                     */
/*============================================================================*/
#if CFG_FAST_META_SUPPORT
extern unsigned char g_is_fast_meta;
#endif

#if CFG_ARMPLL_BOOST
extern void armpll_ll_boost(void);
#endif
/*============================================================================*/
/* INTERNAL FUNCTIONS                                                         */
/*============================================================================*/
#if CFG_BOOT_TIME_PROFILE
U32 profiling_time = 0;	//declare in main.c
#endif

static struct mtk_regulator reg_vibr;

static void bldr_pre_process(void)
{
    int isLocked = 0;
    uint32_t ret = 0;
    #if CFG_FAST_META_SUPPORT
    u8 meta_boot = 0, meta_log = 0;
    #endif

    #if CFG_USB_AUTO_DETECT
    platform_usbdl_flag_check();
    #endif

    #if CFG_EMERGENCY_DL_SUPPORT
    platform_safe_mode(1, CFG_EMERGENCY_DL_TIMEOUT_MS);
    #endif

    /* essential hardware initialization. e.g. timer, pll, uart... */
    platform_pre_init();

#if CFG_ARMPLL_BOOST
    /* Boost LL after platform_pre_init*/
    armpll_ll_boost();
#endif

    pal_log_info("\n%s Build Time: %s\n", MOD, BUILD_TIME);

    /* note: mcu_dfd_debug_en() is only valid before sec_lib_init, be careful when moving the following functions */
#if (CFG_AEE_SUPPORT == 1)
    mcu_dfd_debug_en();
#endif

    /* init security library */
    sec_lib_init();
    BOOTING_TIME_PROFILING_LOG("Sec lib init");

#if MTK_DOE_CONFIG_ENV_SUPPORT
    dconfig_initenv();
    BOOTING_TIME_PROFILING_LOG("dconfig init");

    mtk_wdt_get_env();
    BOOTING_TIME_PROFILING_LOG("wdt dconfig init");
#endif

    g_boot_mode = NORMAL_BOOT;

#ifdef MTK_SECURITY_SW_SUPPORT
    /* note that if you use cmm file, these parameters are empty. */
	ret |= seclib_set_pl_load_addr(p_bldr_param->bl_loadinfo[0].bl_load_addr);
	ret |= seclib_set_cc_status(p_bldr_param->cc_lcs, p_bldr_param->cc_flags);
	ret |= seclib_set_sctrl_info(p_bldr_param->sctrl_cert_file_addr, p_bldr_param->sctrl_cert_file_len);
	ret |= seclib_set_tool_auth_info(p_bldr_param->tool_auth_file_addr, p_bldr_param->tool_auth_file_len);
	ret |= seclib_set_me_id(p_bldr_param->meid, ME_IDENTITY_LEN);
	ret |= seclib_set_soc_id(p_bldr_param->socid, SOC_ID_LEN);
	ret |= seclib_set_prov_key(p_bldr_param->prov_key, PROVISIONING_KEY_LEN);
	if (ret) {
		pal_log_err("Fail to init chip info.\n");
		ASSERT(0);
	}
#endif

    /* hardware initialization */
    platform_init();

    part_init();
    BOOTING_TIME_PROFILING_LOG("part_init");

#if CFG_GZ_SUPPORT
    gz_pre_init();
    BOOTING_TIME_PROFILING_LOG("gz_pre_init");
#endif

#if CFG_FAST_META_SUPPORT
    read_meta_proinfo(&meta_boot, &meta_log);
    if(check_meta_short_pin() || (meta_boot == 0x31)) {
        g_is_fast_meta = 1;
        g_boot_mode = META_BOOT;
        // let kernel always enumerate ELT port
        g_meta_com_id = CFG_FAST_META_FLAG | 0x00000004;
        g_meta_com_type = META_USB_COM;
        // default disable modem/uart log, and can be enable by proinfo flag or meta api
        if(meta_log == 0x31) {
            g_meta_log_disable = 0;

            #if CFG_UART_DYNAMIC_SWITCH
            set_log_switch(1);
            #endif
        }
        else
            g_meta_log_disable = 1;

        pal_log_err("%s fast_meta = %d, meta_log_disable = %d\n", MOD, g_is_fast_meta, g_meta_log_disable);
    }
    BOOTING_TIME_PROFILING_LOG("read_meta_proinfo");
#endif

#if !CFG_FPGA_PLATFORM
    ret = mtk_regulator_get("vibr", &reg_vibr);
    if (ret)
	pal_log_info("get vibr failed\n");
    if (g_boot_reason == BR_POWER_KEY || g_boot_reason == BR_LONG_POWKEY) {
        pal_log_info("Vibrator enable\n");
	/* 2.8V */
	mtk_regulator_set_voltage(&reg_vibr, 2800000, 2800000);
	mtk_regulator_enable(&reg_vibr, 1);
        BOOTING_TIME_PROFILING_LOG("Vibrator enable");
    }
#endif

    store_switch_to_dram();
    BOOTING_TIME_PROFILING_LOG("store_switch_to_dram");
    part_dump();
    BOOTING_TIME_PROFILING_LOG("part_dump");
    setdconfigresultString("emi_dcm");

#if !CFG_FPGA_PLATFORM
    if (g_boot_reason == BR_POWER_KEY || g_boot_reason == BR_LONG_POWKEY) {
        pal_log_info("Vibrator disable\n");
	mtk_regulator_enable(&reg_vibr, 0);
        BOOTING_TIME_PROFILING_LOG("Vibrator disable");
    }
#endif

    rpmb_program_key();
    BOOTING_TIME_PROFILING_LOG("rpmb");

    /* note: lock state is only valid after sec_lib_init, be careful when moving the following functions */
#ifdef MTK_FACTORY_LOCK_SUPPORT
    seclib_query_factory_lock(&isLocked);
#endif

#if CFG_UART_TOOL_HANDSHAKE
            /* init uart handshake for sending 'ready' to tool and receiving handshake
             * pattern from tool in the background and we'll see the pattern later.
             * this can reduce the handshake time.
             */
    boot_mode_t mode = NORMAL_BOOT;
#ifdef MTK_SECURITY_SW_SUPPORT
    mode = seclib_brom_meta_mode();
#endif
    if (!isLocked && mode == NORMAL_BOOT) {
        pal_log_info("%s Starting tool handshake.\n", MOD);
        uart_handshake_init();
        BOOTING_TIME_PROFILING_LOG("UART handshake init");
        log_buf_ctrl(1); /* switch log buffer to dram */
    }
#endif
}

static void bldr_post_process(void)
{
    platform_post_init();

#if CFG_GZ_SUPPORT
    gz_post_init();
    BOOTING_TIME_PROFILING_LOG("gz_post_init");
#endif
}

u32 seclib_get_devinfo_with_index(u32 index)
{
	return internal_seclib_get_devinfo_with_index(index);
}

static bool wait_for_discon(struct comport_ops *comm, u32 tmo_ms)
{
    bool ret;
    u8 discon[HSHK_DISCON_SZ];
    memset(discon, 0x0, HSHK_DISCON_SZ);

    pal_log_info("[BLDR] DISCON...");
    if (ret = comm->recv(discon, HSHK_DISCON_SZ, tmo_ms)) {
    pal_log_info("timeout\n");
    return ret;
    }

    if (0 == memcmp(discon, HSHK_DISCON, HSHK_DISCON_SZ))
    pal_log_info("OK\n");
    else
    pal_log_info("protocol mispatch\n");

    return ret;
}

int bldr_load_part(char *name, blkdev_t *bdev, u32 *addr, u32 *size)
{
    part_t *part = part_get(name);

    if (NULL == part) {
        pal_log_err("%s %s partition not found\n", MOD, name);
        return -1;
    }

    return part_load(bdev, part, addr, 0, size);
}

int bldr_load_part_lk(blkdev_t *bdev, u32 *addr, u32 *size)
{
    unsigned long lk_active = 0, lk2_active = 0;

    // check the active bit of lk partition
    part_t *part_lk = part_get("lk");
    if (NULL == part_lk) {
        pal_log_err("%s lk partition not found\n", MOD);
        return -1;
    } else {
        lk_active = mt_part_get_active_bit(part_lk);
    }

    // check the active bit of lk2 partition
    part_lk = part_get("lk2");
    if (NULL == part_lk) {
        pal_log_err("%s lk2 partition not found\n", MOD);
        //return -1; Comment it since we could load lk partition to boot
    } else {
        lk2_active = mt_part_get_active_bit(part_lk);
    }

    pal_log_info("%s lk active = %x, lk2 active = %x\n", MOD, lk_active, lk2_active);

    // load partition
    if (lk_active == 0 && lk2_active > 0) {
        pal_log_info("%s Loading LK2 Partition...\n", MOD);
        part_lk = part_get("lk2");
        if (NULL == part_lk) {
            pal_log_err("%s lk2 partition not found\n", MOD);
            return -1;
        }
        else {
            return part_load(bdev, part_lk, addr, 0, size);
        }
    } else {
        pal_log_info("%s Loading LK Partition...\n", MOD);
        part_lk = part_get("lk");
        if (NULL == part_lk) {
            pal_log_err("%s lk partition not found\n", MOD);
            return -1;
        }
        else {
            return part_load(bdev, part_lk, addr, 0, size);
        }
    }
}

#if CFG_AEE_LK_MEMADDR
int bldr_load_aee_lk(char *name, blkdev_t *bdev, u32 *addr, u32 *size)
{
    int ret = 0;
    part_t *part = part_get(name);
    u32 offset = 0;

    if (part == NULL) {
        pal_log_err("%s %s part. not found\n", MOD, name);
        ASSERT(0);
    }

    if (get_part_offset_by_img_name(bdev, part, &offset, CFG_AEE_LK_IMG_NAME) != 0) {
        pal_log_err("%s fail to get %s part_offset\n", MOD, CFG_AEE_LK_IMG_NAME);
        ASSERT(0);
    }

    ret = part_load(bdev, part, addr, offset, size);
    if (ret) {
        pal_log_err("%s fail to load %s\n", MOD, CFG_AEE_LK_IMG_NAME);
        ASSERT(0);
    }

    return 0;
}
#endif

#define BL2_EXT_IMAGE "bl2_ext"
int bldr_load_bl2_ext(char *name, blkdev_t *bdev, u32 *addr, u32 *size)
{
    int ret = 0;
    part_t *part = part_get(name);
    u32 offset = 0;

    if (part == NULL) {
        pal_log_err("%s %s part. not found\n", MOD, name);
        ASSERT(0);
    }

    if (get_part_offset_by_img_name(bdev, part, &offset, BL2_EXT_IMAGE) != 0) {
        pal_log_err("%s fail to get %s part_offset\n", MOD, BL2_EXT_IMAGE);
        ASSERT(0);
    }

    ret = part_load(bdev, part, addr, offset, size);
    if (ret) {
        pal_log_err("%s fail to load %s\n", MOD, name, BL2_EXT_IMAGE);
        ASSERT(0);
    }

    return 0;
}

int bldr_load_tee_part(char *name, blkdev_t *bdev, u32 *addr, u32 offset, u32 *size)
{
    int ret;
    part_t *part = part_get(name);
	u32 next_offset;
	u32 atf_sram_size;
	u32 atf_dram_addr = 0;

    if (NULL == part) {
        pal_log_err("%s %s part. not found\n", MOD, name);
        return -1;
    }

    ret = part_load(bdev, part, addr, offset, size);
    if (ret) {
        pal_log_err("%s %s part. ATF load fail\n", MOD, name);
        return ret;
    }
	pal_log_info("%s %s part. ATF load addr:0x%x, size:0x%x\n", MOD, name, *addr, *size);

	next_offset = sizeof(part_hdr_t) + *size;
	if (if_equal_img_name(bdev, part, next_offset, ATF_DRAM_IMG_NAME))
	{
		atf_sram_size = *size;
		ret = part_load(bdev, part, &atf_dram_addr, next_offset, size);
		if (ret) {
			pal_log_err("%s %s part. ATF load fail\n", MOD, name);
			return ret;
		}
		pal_log_info("%s %s part. ATF dram load addr:0x%x, size:0x%x\n", MOD, name, atf_dram_addr, *size);
		*size = *size + sizeof(part_hdr_t) + atf_sram_size;
	}

    return ret;
}

#if (MTK_TINYSYS_SSPM_SUPPORT || CFG_LOAD_SLT_SSPM)
static void *sspm_memcpy(void *dest, const void *src, int count)
{
    unsigned int *tmp = dest;
    const unsigned int *s = src;

    while (count > 0) {
        *tmp++ = *s++;
        count -= 4;
    }

    return dest;
}
#endif

#if MTK_TINYSYS_SSPM_SUPPORT
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;

static int bldr_load_sspm_part(blkdev_t *bdev, u32 *addr, u32 *size)
{
    int ret;
    u32 tmp_addr;
    part_t *part;
    ptimg_hdr_t *hdr;
    unsigned char *img, *pmimg, *dmimg, *cpmimg, *ximg;
    unsigned int pmsize, dmsize, cpmsize, xsize;
    char *parts[] = { "sspm_1", "sspm_2" };
    struct sspm_info_t *info = (struct sspm_info_t *) *addr;

#if SSPM_DBG_MODE
#define SSPM_CFGREG_PC              (SSPM_SW_RSTN + 0x64)
#define SSPM_CFGREG_AHB_STATUS      (SSPM_SW_RSTN + 0x6C)
#define SSPM_CFGREG_AHB_M0_ADDR     (SSPM_SW_RSTN + 0x70)
#define SSPM_CFGREG_AHB_M1_ADDR     (SSPM_SW_RSTN + 0x74)
    sspm_dbg("SSPM_CFGREG_PC: 0x%x\n", DRV_Reg32(SSPM_CFGREG_PC));
    sspm_dbg("SSPM_CFGREG_AHB_STATUS: 0x%x\n", DRV_Reg32(SSPM_CFGREG_AHB_STATUS));
    sspm_dbg("SSPM_CFGREG_AHB_M0_ADDR: 0x%x\n", DRV_Reg32(SSPM_CFGREG_AHB_M0_ADDR));
    sspm_dbg("SSPM_CFGREG_AHB_M1_ADDR: 0x%x\n", DRV_Reg32(SSPM_CFGREG_AHB_M1_ADDR));
#endif

    /*enable sspm sram*/
    pal_log_err("[SSPM] enable sspm sram\n");
    DRV_WriteReg32(0x1C001000, 0x0b160001);
    DRV_WriteReg32(0x1C001F20, 0x00000012);

#if defined(MTK_AB_OTA_UPDATER)
    const char *ab_suffix = get_suffix();
    if (ab_suffix == NULL)
        return -1;

    tmp_addr = *addr + ROUNDUP(sizeof(*info), 4);
    if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, 2)) {
        ret = bldr_load_part("sspm_a", bdev, &tmp_addr, size);
    } else {
        ret = bldr_load_part("sspm_b", bdev, &tmp_addr, size);
    }
#else
    for (ret = 0;ret < (sizeof(parts) / sizeof(*parts));ret++) {
        part = part_get(parts[ret]);

        if (part && mt_part_get_active_bit(part) > 0)
            break;
    }

    if (ret == (sizeof(parts) / sizeof(*parts)))
        ret = 0;

    /* decide partition by active bit */
    part = part_get(parts[ret]);

    if (NULL == part) {
        pal_log_err("%s SSPM part. not found\n", MOD);
        return -1;
    }

    tmp_addr = *addr + ROUNDUP(sizeof(*info), 4);

    /* load and verify image */
    ret = part_load(bdev, part, &tmp_addr, 0, size);
#endif //MTK_AB_OTA_UPDATER

    if (ret) {
        pal_log_err("%s SSPM part. load fail\n", MOD);
        return ret;
    }
    sspm_dbg("Load SSPM partition to dram 0x%x (%d bytes)\n", tmp_addr, *size);

    /* separate ptimg */
    pmimg = dmimg = cpmimg = ximg = NULL;
    pmsize = dmsize = cpmsize = xsize = 0;
    hdr = (ptimg_hdr_t *) tmp_addr;

    while (hdr->magic == PT_MAGIC) {
        img = ((char *) hdr) + hdr->hdr_size;

        switch (hdr->id) {
            case PT_ID_SSPM_PM:
                pmimg = img;
                pmsize = hdr->img_size;
                break;
            case PT_ID_SSPM_DM:
                dmimg = img;
                dmsize = hdr->img_size;
                break;
            case PT_ID_SSPM_CPM:
                cpmimg = img;
                cpmsize = hdr->img_size;
                break;
            case PT_ID_SSPM_XFILE:
                ximg = img;
                xsize = hdr->img_size;
                break;
            default:
                pal_log_warn("%s Warning: Ignore unknow SSPM image_%d\n", MOD, hdr->id);
                break;
        }

        img += ROUNDUP(hdr->img_size, hdr->align);
        hdr = (ptimg_hdr_t *) img;
    }

    sspm_dbg("pmimg: 0x%x(%d bytes), dmimg: 0x%x(%d bytes), ximg: 0x%x(%d bytes)\n",
        (u32)pmimg, pmsize, (u32)dmimg, dmsize, (u32)ximg, xsize);

    if (!pmimg || !dmimg || !ximg) {
        pal_log_err("%s SSPM partition missing - PM:0x%x, DM:0x%x (@0x%x)\n", MOD, (u32)pmimg, (u32)dmimg, tmp_addr);
        return -1;
    }


    if (cpmsize > SSPM_CPM_SZ) {
        pal_log_err("%s SSPM .cpm (%d bytes) is oversize\n", MOD, cpmsize);
        return -1;
    }

    /* load PM */
    sspm_memcpy((unsigned char *) CFG_SSPM_MEMADDR, pmimg, pmsize);

    /* Setup sspm_info  */
    memset(info, 0, ROUNDUP(sizeof(*info), 4));

    info->sspm_dm_ofs = (unsigned int)dmimg - (unsigned int)info;
    info->sspm_dm_sz = dmsize;

    info->rd_ofs = ROUNDUP(SSPM_IMG_SZ, SSPM_MEM_ALIGN);
    info->rd_sz  = SSPM_TCM_SZ - pmsize;

    info->xfile_addr = (unsigned int)ximg - sizeof(ptimg_hdr_t);
    info->xfile_sz = sizeof(ptimg_hdr_t) + xsize + sizeof(unsigned int); // with postfix 'END'

    sspm_dbg("sspm_info @0x%x: dm offset 0x%x (%d bytes), rd offset 0x%x(%d bytes), xfile addr 0x%x(%d bytes)\n",
        (u32)info, info->sspm_dm_ofs, info->sspm_dm_sz, info->rd_ofs, info->rd_sz, info->xfile_addr, info->xfile_sz);

    if ( (pmsize + dmsize + xsize + 3 * sizeof(ptimg_hdr_t)) > SSPM_IMG_SZ) {
        pal_log_warn("%s Warning: SSPM IMG is over than %dK, the coredump will overwrite it!\n", MOD, SSPM_IMG_SZ/1024);
    }

    /* re-allocate CPM */
    if (cpmimg> 0) {
        tmp_addr = ROUNDUP((*addr + SSPM_IMG_SZ + SSPM_TCM_SZ), SSPM_DDR_ALIGN) + (SSPM_TCM_SZ - SSPM_DDR_SIZE);
        memcpy((void *)tmp_addr, (void *)cpmimg, cpmsize);
        DRV_WriteReg32(SSPM_DDR_RMAP0, tmp_addr >> 18);

        sspm_dbg("Re-allocate CPM to 0x%x; DDR_RMAP is 0x%x\n", tmp_addr, DRV_Reg32(SSPM_DDR_RMAP0));
    }

    /* Setup SSPM WDT mode */
    mtk_wdt_request_mode_set(MTK_WDT_STATUS_SSPM_RST, WD_REQ_RST_MODE);
    mtk_wdt_request_en_set(MTK_WDT_STATUS_SSPM_RST, WD_REQ_EN);

#if SSPM_DBG_MODE
    //Setup GPIO pinmux SSPM JTAG
    DRV_SetReg32(0x10005360, 0x66066000);
    DRV_SetReg32(0x10005370, 0x00000006);
    //Setup GPIO pinmux for SSPM UART0
    DRV_SetReg32(0x100053B0, 0x44000000);
#endif

    DRV_WriteReg32(SSPM_CFGREG_GPR0, (unsigned int) *addr);
    DRV_WriteReg32(SSPM_CFGREG_GPR1, ram_console_is_abnormal_boot()); // If not normal boot, notify sspm to backup

    pal_log_info("%s SSPM Start %s ddr reserved mode (%s boot)\n", MOD,
         (g_ddr_reserve_enable==1 && g_ddr_reserve_success==1) ? "with" : "without",
         DRV_Reg32(SSPM_CFGREG_GPR1) ? "abnormal" : "normal");

    DRV_SetReg32(SSPM_SW_RSTN, 0x1);

    return 0;
}
#endif

#if CFG_LOAD_SLT_SSPM
static int bldr_load_sspm_part_slt(blkdev_t *bdev, u32 *addr, u32 *size)
{
	int ret;
	ptimg_hdr_t *hdr;
	unsigned char *img, *pmimg, *dmimg;
	unsigned int pmsize, dmsize;
	char *parts = "SSPM";

	/*enable sspm sram*/
	pal_log_err("[SSPM] enable sspm sram\n");
	DRV_WriteReg32(0x1C001000, 0x0b160001);
	DRV_WriteReg32(0x1C001F20, 0x00000012);

	/* load and verify image */
    ret = bldr_load_part(parts, bdev, addr, size);
	if (ret) {
		pal_log_err("%s SSPM part. load fail\n", MOD);
		return ret;
	}

	/* separate ptimg */
	pmimg = dmimg = NULL;
	hdr = (ptimg_hdr_t *) *addr;

	while (hdr->magic == PT_MAGIC) {
		img = ((char *) hdr) + hdr->hdr_size;

		switch (hdr->id) {
			case PT_ID_SSPM_PM:
				pmimg = img;
				pmsize = hdr->img_size;
				break;
			case PT_ID_SSPM_DM:
				dmimg = img;
				dmsize = hdr->img_size;
				break;
		}

		img += ROUNDUP(hdr->img_size, hdr->align);
		hdr = (ptimg_hdr_t *) img;
	}

	if (!pmimg || !dmimg) {
		pal_log_err("%s SSPM part missing: 0x%x, 0x%x\n", MOD, (u32) pmimg, (u32) dmimg);
		return -1;
	}

	// print("pmimg: 0x%x (0x%x), dmimg: 0x%x (0x%x)\n", (u32) pmimg, (u32) pmsize, (u32) dmimg, (u32) dmsize);

	sspm_memcpy((unsigned char *) CFG_SSPM_MEMADDR, pmimg, pmsize);
	sspm_memcpy((unsigned char *) CFG_SSPM_MEMADDR + pmsize, dmimg, dmsize);

	pal_log_info("%s SSPM finished\n");
	return 0;
}
#endif

#if (MTK_TINYSYS_MCUPM_SUPPORT || CFG_LOAD_SLT_MCUPM)
static void *mcupm_memcpy(void *dest, const void *src, int count)
{
    unsigned int *tmp = dest;
    const unsigned int *s = src;

    while (count > 0) {
        *tmp++ = *s++;
        count -= 4;
    }

    return dest;
}
#endif

#if MTK_TINYSYS_MCUPM_SUPPORT
static int bldr_load_mcupm_part(blkdev_t *bdev, u32 *addr, u32 *size)
{
    int ret;
    u32 tmp_addr;
    part_t *part;
    ptimg_hdr_t *hdr;
    unsigned char *img, *mcupm_pmimg, *mcupm_dmimg, *mcupm_ximg;
    unsigned int mcupm_pmsize, mcupm_dmsize, mcupm_xsize;
    char *parts[] = { "mcupm_1", "mcupm_2" };
    struct mcupm_info_t *info = (struct mcupm_info_t *) *addr;
    unsigned int mcupm_addr;

#if MCUPM_DBG_MODE
    mcupm_dbg("MCUPM_CFGREG_DBG_CON: 0x%x\n", DRV_Reg32(MCUPM_CFGREG_DBG_CON));
    mcupm_dbg("MCUPM_CFGREG_MON_PC: 0x%x\n", DRV_Reg32(MCUPM_CFGREG_MON_PC));
    mcupm_dbg("MCUPM_CFGREG_AHB_STATUS: 0x%x\n", DRV_Reg32(MCUPM_CFGREG_AHB_STATUS));
    mcupm_dbg("MCUPM_CFGREG_AHB_M0_ADDR: 0x%x\n", DRV_Reg32(MCUPM_CFGREG_AHB_M0_ADDR));
    mcupm_dbg("MCUPM_CFGREG_AHB_M1_ADDR: 0x%x\n", DRV_Reg32(MCUPM_CFGREG_AHB_M1_ADDR));
#endif

#if defined(MTK_AB_OTA_UPDATER)
    const char *ab_suffix = get_suffix();
    if (ab_suffix == NULL)
        return -1;

    tmp_addr = *addr + ROUNDUP(sizeof(*info), 4);
    if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, 2)) {
        ret = bldr_load_part("mcupm_a", bdev, &tmp_addr, size);
    } else {
        ret = bldr_load_part("mcupm_b", bdev, &tmp_addr, size);
    }
#else
    for (ret = 0;ret < (sizeof(parts) / sizeof(*parts));ret++) {
        part = part_get(parts[ret]);

        if (part && mt_part_get_active_bit(part) > 0)
            break;
    }

    if (ret == (sizeof(parts) / sizeof(*parts)))
        ret = 0;

    /* decide partition by active bit */
    part = part_get(parts[ret]);

    if (NULL == part) {
        pal_log_err("%s MCUPM part. not found\n", MOD);
        return -1;
    }

    tmp_addr = *addr + ROUNDUP(sizeof(*info), 4);

    /* load and verify image */
    ret = part_load(bdev, part, &tmp_addr, 0, size);
#endif //MTK_AB_OTA_UPDATER

    if (ret) {
        pal_log_err("%s MCUPM part. load fail\n", MOD);
        return ret;
    }
    mcupm_dbg("Load MCUPM partition to dram 0x%x (%d bytes)\n", tmp_addr, *size);

    /* separate ptimg */
    mcupm_pmimg = mcupm_dmimg = mcupm_ximg = NULL;
    mcupm_pmsize = mcupm_dmsize = mcupm_xsize = 0;
    hdr = (ptimg_hdr_t *) tmp_addr;

    while (hdr->magic == PT_MAGIC) {
        img = ((char *) hdr) + hdr->hdr_size;

        switch (hdr->id) {
            case PT_ID_MCUPM_PM:
                mcupm_pmimg = img;
                mcupm_pmsize = hdr->img_size;
                break;
            case PT_ID_MCUPM_DM:
                mcupm_dmimg = img;
                mcupm_dmsize = hdr->img_size;
                break;
            case PT_ID_MCUPM_XFILE:
                mcupm_ximg = img;
                mcupm_xsize = hdr->img_size;
                break;
            default:
                break;
        }

        img += ROUNDUP(hdr->img_size, hdr->align);
        hdr = (ptimg_hdr_t *) img;
    }

    if (!mcupm_pmimg || !mcupm_dmimg || !mcupm_ximg) {
        pal_log_err("%s MCUPM partition missing - PM:0x%x, DM:0x%x (@0x%x)\n", MOD, (u32)mcupm_pmimg, (u32)mcupm_dmimg, tmp_addr);
        return -1;
    }

    mcupm_dbg("mcupm_pmimg: 0x%x(%d bytes), mcupm_dmimg: 0x%x(%d bytes), mcupm_ximg: 0x%x(%d bytes), CFG_MCUPM_MEMADDR: 0x%x\n",
        (u32)mcupm_pmimg, mcupm_pmsize, (u32)mcupm_dmimg, mcupm_dmsize, (u32)mcupm_ximg, mcupm_xsize, CFG_MCUPM_MEMADDR);

    DRV_WriteReg32(MCUPM_SW_RSTN, 0x0);
    if (mcupm_pmsize <= SRAM_32KB_SIZE) {
	mcupm_memcpy((unsigned char *) CFG_MCUPM_MEMADDR, mcupm_pmimg, mcupm_pmsize);
    } else {
	mcupm_memcpy((unsigned char *) CFG_MCUPM_MEMADDR, mcupm_pmimg, SRAM_32KB_SIZE);
	mcupm_memcpy((unsigned char *) SRAM_32KB_AP_BASE, mcupm_pmimg + SRAM_32KB_SIZE, mcupm_pmsize - SRAM_32KB_SIZE);
    }

    memset(info, 0, ROUNDUP(sizeof(*info), 4));

    info->mcupm_dm_ofs = (unsigned int)mcupm_dmimg - (unsigned int)info;
    info->mcupm_dm_sz = mcupm_dmsize;

    info->rd_ofs = ROUNDUP(MCUPM_IMG_SZ, MCUPM_MEM_ALIGN);
    info->rd_sz  = MCUPM_TCM_SZ - mcupm_pmsize;

    info->xfile_addr = (unsigned int)mcupm_ximg - sizeof(ptimg_hdr_t);
    info->xfile_sz = sizeof(ptimg_hdr_t) + mcupm_xsize + sizeof(unsigned int); // with postfix 'END'

    mcupm_dbg("mcupm_info @0x%x: dm offset 0x%x (%d bytes), rd offset 0x%x(%d bytes), xfile addr 0x%x(%d bytes)\n",
        (u32)info, info->mcupm_dm_ofs, info->mcupm_dm_sz, info->rd_ofs, info->rd_sz, info->xfile_addr, info->xfile_sz);

    if ( (mcupm_pmsize + mcupm_dmsize + mcupm_xsize + 3 * sizeof(ptimg_hdr_t)) > MCUPM_IMG_SZ) {
        pal_log_warn("%s Warning: MCUPM IMG is over than %dK, the coredump will overwrite it!\n", MOD, MCUPM_IMG_SZ/1024);
    }

#if MCUPM_DBG_MODE
    /* MCUPM JTAG GPIO pinmux: */
    DRV_SetReg32(0x10005350, DRV_Reg32(0x10005350)|0x33300000);
    DRV_SetReg32(0x10005360, DRV_Reg32(0x10005360)|0x00000033);
    DRV_SetReg32(MCUPM_CFGREG_DBG_CON, DRV_Reg32(MCUPM_CFGREG_DBG_CON)|0x100);
#endif

    DRV_WriteReg32(MCUPM_SRAM_GPR0, (unsigned int) *addr);
    DRV_WriteReg32(MCUPM_SRAM_GPR1, ram_console_is_abnormal_boot()); // If not normal boot, notify mcupm to backup
    for (mcupm_addr = SRAM_CPUDVFS_INIT_PTBL; mcupm_addr <= SRAM_CPUDVFS_INIT_VOLT_E; mcupm_addr += 4)
        DRV_WriteReg32(mcupm_addr, 0x0);


#define MCUPM_CFGREG_SW_RSTN_SW_RSTN  (0x1 << 0)
#define MCUPM_CFGREG_SW_RSTN_DMA_BUSY_MASK  (0x1 << 1)
    DRV_WriteReg32(MCUPM_SW_RSTN, MCUPM_CFGREG_SW_RSTN_SW_RSTN | MCUPM_CFGREG_SW_RSTN_DMA_BUSY_MASK);

    mcupm_dbg("mcupm_pmimg: MCUPM_SRAM_GPR0: 0x%x, 0x%x\n", MCUPM_SRAM_GPR0, DRV_Reg32(MCUPM_SRAM_GPR0));
    mcupm_dbg("mcupm_pmimg: MCUPM_SRAM_GPR1: 0x%x, 0x%x\n", MCUPM_SRAM_GPR1, DRV_Reg32(MCUPM_SRAM_GPR1));

    pal_log_info("%s MCUPM part. load & reset finished\n", MOD);
    return 0;
}
#endif

#if CFG_LOAD_SLT_MCUPM
static int bldr_load_mcupm_part_slt(blkdev_t *bdev, u32 *addr, u32 *size)
{
	int ret;
	ptimg_hdr_t *hdr;
	unsigned char *img, *pmimg, *dmimg;
	unsigned int pmsize, dmsize;
	char *parts = "MCUPM";

	/* load and verify image */
	ret = bldr_load_part(parts, bdev, addr, size);
	if (ret) {
		pal_log_err("%s MCUPM part. slt load fail\n", MOD);
		return ret;
	}

	/* separate ptimg */
	pmimg = dmimg = NULL;
	hdr = (ptimg_hdr_t *) *addr;

	while (hdr->magic == PT_MAGIC) {
		img = ((char *) hdr) + hdr->hdr_size;

		switch (hdr->id) {
			case PT_ID_MCUPM_PM:
				pmimg = img;
				pmsize = hdr->img_size;
				break;
			case PT_ID_MCUPM_DM:
				dmimg = img;
				dmsize = hdr->img_size;
				break;
		}

		img += ROUNDUP(hdr->img_size, hdr->align);
		hdr = (ptimg_hdr_t *) img;
	}

	if (!pmimg || !dmimg) {
		pal_log_err("%s MCUPM part missing: 0x%x, 0x%x\n", MOD, (u32) pmimg, (u32) dmimg);
		return -1;
	}

	mcupm_memcpy((unsigned char *) CFG_MCUPM_MEMADDR, pmimg, pmsize);
	mcupm_memcpy((unsigned char *) CFG_MCUPM_MEMADDR + pmsize, dmimg, dmsize);

	pal_log_info("%s MCUPM part. slt load finished\n", MOD);
	return 0;
}
#endif

static bool bldr_cmd_handler(struct bldr_command_handler *handler,
    struct bldr_command *cmd, struct bldr_comport *comport)
{
    struct comport_ops *comm = comport->ops;
    u32 attr = handler->attr;

#if CFG_DT_MD_DOWNLOAD
    if (CMD_MATCH(cmd, SWITCH_MD_REQ)) {
        /* SWITCHMD */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        comm->send((u8*)SWITCH_MD_ACK, strlen(SWITCH_MD_ACK));
        platform_modem_download();
        return TRUE;
    }
#endif

    if (CMD_MATCH(cmd, ATCMD_PREFIX)) {
        /* "AT+XXX" */

        if (CMD_MATCH(cmd, ATCMD_NBOOT_REQ)) {
            /* return "AT+OK" to tool */
            comm->send((u8*)ATCMD_OK, strlen(ATCMD_OK));

            g_boot_mode = NORMAL_BOOT;
            g_boot_reason = BR_TOOL_BY_PASS_PWK;

        } else {
            /* return "AT+UNKONWN" to ack tool */
            comm->send((u8*)ATCMD_UNKNOWN, strlen(ATCMD_UNKNOWN));

            return FALSE;
        }
    } else if (CMD_MATCH(cmd, META_STR_REQ)) {
        para_t param;
        para_t_v2 param_v2;
        memset(&param, 0, sizeof(param));  /*init param*/
        memset(&param_v2, 0, sizeof(param_v2));
        bootarg.md_type[0] = 0;
        bootarg.md_type[1] = 0;

        /* "METAMETA" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

    /* for backward compatibility */
    comm->recv((u8*)&param.v0001, sizeof(param.v0001), 2000);

    /* meta usb type use for ELT Port */
    pal_log_info("meta_com_id = %d\n", param.v0001.usb_type);
    g_meta_com_id = param.v0001.usb_type;

#ifdef MTK_SECURITY_META_SLA_SUPPORT
    volatile int cfi = 0;
    volatile int pl_sla_status = sec_pl_meta_sla(comm, &cfi);
    if (pl_sla_status == PL_SLA_SUCCESS && cfi == PL_SLA_CFI)
        pal_log_info("[SEC] PL SLA SUCCESS\n");
    else if (pl_sla_status == PL_SLA_BYPASS && seclib_sbc_enabled() == 0)
        pal_log_info("[SEC] PL SLA BYPASS\n");
    else {
        pal_log_err("[SEC] PL SLA FAILED\n");
        goto forbidden;
    }
#endif

#if CFG_WORLD_PHONE_SUPPORT
        comm->send((u8*)META_ARG_VER_STR, strlen(META_ARG_VER_STR));

        if (0 == comm->recv((u8*)&param.v0001, sizeof(param.v0001), 5000)) {
            g_meta_com_id = param.v0001.usb_type;
        pal_log_info("meta_com_id = %d\n", param.v0001.usb_type);
        pal_log_info("md_type[0] = %d \n", param.v0001.md0_type);
        pal_log_info("md_type[1] = %d \n", param.v0001.md1_type);

        bootarg.md_type[0] = param.v0001.md0_type;
        bootarg.md_type[1] = param.v0001.md1_type;
        }
#endif

        /* V2 param extension */
        comm->send((u8*)META_ARG_VER_STR_V2, strlen(META_ARG_VER_STR_V2));
        if (0 == comm->recv((u8*)&param_v2.v0002, sizeof(param_v2.v0002), 5000)) {
            pal_log_info("meta_com_id = %d\n", param_v2.v0002.usb_type);
            pal_log_info("meta_log_disable = %d\n", param_v2.v0002.meta_log_disable);
            g_meta_com_id = param_v2.v0002.usb_type;
            g_meta_log_disable = param_v2.v0002.meta_log_disable; /* disable uart log to enhance boot time */
        }

        comm->send((u8*)META_STR_ACK, strlen(META_STR_ACK));

#if CFG_WORLD_PHONE_SUPPORT
        wait_for_discon(comm, 1000);
#endif
        g_boot_mode = META_BOOT;
    } else if (CMD_MATCH(cmd, FACTORY_STR_REQ)) {
        para_t param;

        /* "FACTFACT" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        if (0 == comm->recv((u8*)&param.v0001, sizeof(param.v0001), 5)) {
            g_meta_com_id = param.v0001.usb_type;
        }

        comm->send((u8*)FACTORY_STR_ACK, strlen(FACTORY_STR_ACK));

        g_boot_mode = FACTORY_BOOT;
    } else if (CMD_MATCH(cmd, META_ADV_REQ)) {
        /* "ADVEMETA" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        comm->send((u8*)META_ADV_ACK, strlen(META_ADV_ACK));

        wait_for_discon(comm, 1000);

        g_boot_mode = ADVMETA_BOOT;
    } else if (CMD_MATCH(cmd, ATE_STR_REQ)) {
        para_t param;

        /* "FACTORYM" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        if (0 == comm->recv((u8*)&param.v0001, sizeof(param.v0001), 5)) {
            g_meta_com_id = param.v0001.usb_type;
        }

        comm->send((u8*)ATE_STR_ACK, strlen(ATE_STR_ACK));

        g_boot_mode = ATE_FACTORY_BOOT;
    } else if (CMD_MATCH(cmd, FB_STR_REQ)) {

    /* "FASTBOOT" */
    comm->send((u8 *)FB_STR_ACK, strlen(FB_STR_ACK));

    g_boot_mode = FASTBOOT;
    } else {
        pal_log_warn("%s unknown received: \'%s\'\n", MOD, cmd->data);

        return FALSE;
    }

    pal_log_info("%s '%s' received!\n", MOD, cmd->data);

    return TRUE;

forbidden:
    comm->send((u8*)META_FORBIDDEN_ACK, strlen(META_FORBIDDEN_ACK));
    pal_log_warn("%s '%s' is forbidden!\n", MOD, cmd->data);
    return FALSE;
}

static int bldr_handshake(struct bldr_command_handler *handler)
{
    boot_mode_t mode = NORMAL_BOOT;
    bool isSLA = 0;
    int isLocked = 0;

#ifdef MTK_SECURITY_SW_SUPPORT
    /* get mode type */
    mode = seclib_brom_meta_mode();
    isSLA = seclib_sla_enabled();
	BOOTING_TIME_PROFILING_LOG("seclib_brom_meta_mode");
#endif

#ifdef MTK_FACTORY_LOCK_SUPPORT
    seclib_query_factory_lock(&isLocked);
#endif

    switch (mode) {
    case NORMAL_BOOT:
        /* ------------------------- */
        /* security check            */
        /* ------------------------- */
        if (TRUE == isSLA) {
            handler->attr |= CMD_HNDL_ATTR_COM_FORBIDDEN;
            pal_log_info("%s META DIS\n", MOD);
        }

        if (!isLocked) {
            pal_log_info("%s Tool connection is unlocked\n", MOD);
            #if CFG_USB_TOOL_HANDSHAKE
            #if CFG_FAST_META_SUPPORT
            if(g_is_fast_meta)
                pal_log_info("%s In fast meta, bypass usb handshake.\n", MOD);
            else
            #endif
            {
            if (TRUE == usb_handshake(handler))
                g_meta_com_type = META_USB_COM;
		    BOOTING_TIME_PROFILING_LOG("USB handshake");
            }
            #endif
            #if CFG_UART_TOOL_HANDSHAKE
            if (TRUE == uart_handshake(handler))
                g_meta_com_type = META_UART_COM;
		    BOOTING_TIME_PROFILING_LOG("UART handshake");
            #endif
        }
        else {
            pal_log_info("%s Tool connection is locked\n", MOD);
            bootarg.sec_limit.magic_num = SEC_LIMIT_MAGIC;
            bootarg.sec_limit.forbid_mode = F_FACTORY_MODE;
        }

        break;

    case META_BOOT:
        pal_log_info("%s BR META BOOT\n", MOD);

// init md_type for security chip + world phone project in BROM Meta case.
        bootarg.md_type[0] = 0;
        bootarg.md_type[1] = 0;

        g_boot_mode = META_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case FACTORY_BOOT:
        pal_log_info("%s BR FACTORY BOOT\n", MOD);
        g_boot_mode = FACTORY_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case ADVMETA_BOOT:
        pal_log_info("%s BR ADVMETA BOOT\n", MOD);
        g_boot_mode = ADVMETA_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case ATE_FACTORY_BOOT:
        pal_log_info("%s BR ATE FACTORY BOOT\n", MOD);
        g_boot_mode = ATE_FACTORY_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    default:
        pal_log_info("%s UNKNOWN MODE\n", MOD);
        break;
    }

    return 0;
}

static void bldr_wait_forever(void)
{
    /* prevent wdt timeout and clear usbdl flag */
    mtk_wdt_disable();
    platform_safe_mode(0, 0);
    pal_log_info("bldr_wait_forever\n");
    while(1);
}

static int bldr_load_slt_images(u32 *jump_addr)
{
    blkdev_t *bootdev;
    u32 addr = 0;
    u32 size = 0;
    int ret = 0;

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        pal_log_err("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

#if CFG_LOAD_SLT_MD_DSP
    addr = CFG_MD_DSP_MEMADDR;
    bldr_load_part("NLWCTG_MD_DSP",bootdev, &addr, &size);
#endif

#if CFG_LOAD_SLT_MD
    int *pass_size = NULL;
    addr = CFG_FDD_MD_ROM_MEMADDR;
    bldr_load_part("HVT_MD_ROM", bootdev, &addr, &size);
    addr = CFG_FDD_MD_ROM_MEMADDR;
    bldr_load_part("NLWCTG_MD_ROM", bootdev, &addr, &size);
#endif

#if MTK_TINYSYS_MCUPM_SUPPORT && CFG_LOAD_SLT_MCUPM
    addr = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)MCUPM_MEM_SIZE, (u64)MCUPM_MEM_ALIGN,
            MCUPM_MEM_LIMIT, 0, "MCUPM-reserved");
    if (!addr)
        return -1;
    ret = bldr_load_mcupm_part_slt(bootdev, &addr, &size);
    if (ret)
        return ret;
#endif

#if MTK_TINYSYS_SSPM_SUPPORT && CFG_LOAD_SLT_SSPM
    addr = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)SSPM_MEM_SIZE, (u64)SSPM_MEM_ALIGN,
            SSPM_MEM_LIMIT, 0, "SSPM-reserved");

    if (!addr)
        return -1;

    ret = bldr_load_sspm_part_slt(bootdev, &addr, &size);

    if (ret)
        pal_log_info("SLT doesnt want to check bldr_load_sspm_part_slt ret=%d.\n", ret);
#endif

#if CFG_LOAD_AP_ROM
    addr = CFG_AP_ROM_MEMADDR;
    ret = bldr_load_part("AP_ROM", bootdev, &addr, &size);
    if (ret) {
        pal_log_info("[ERROR] AP_ROM load fail\n");
        return ret;
    }
    *jump_addr = addr;
#endif

    return ret;
}

#if MTK_TINYSYS_SSPM_SUPPORT && !CFG_LOAD_SLT_SSPM
static int bldr_load_sspm_image(blkdev_t *bootdev)
{
    u32 addr = 0;
    u32 size = 0;
    int ret = 0;
    u32 type = 0;

    addr = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)SSPM_MEM_SIZE, (u64)SSPM_MEM_ALIGN,
            SSPM_MEM_LIMIT, 0, "SSPM-reserved");

    if (!addr)
        return -1;

#if ENABLE_MPU && (!SSPM_DBG_MODE)
    struct emi_region_info_t region_info;
    region_info.start = (u64)addr;
    region_info.end = (u64)addr + ROUNDUP(SSPM_MEM_SIZE, SSPM_MEM_ALIGN) - 1;
    region_info.region = SSPM_MPU_REGION_ID;
    SET_ACCESS_PERMISSION(region_info.apc, UNLOCK,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, SEC_RW,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, NO_PROTECTION);

    emi_mpu_set_protection(&region_info);
#endif

    ret = bldr_load_sspm_part(bootdev, &addr, &size);

    if (ret)
        return ret;

#if ENABLE_MPU && (!SSPM_DBG_MODE)
    if ((ret = ram_console_is_abnormal_boot()) != 0) {
        /* If ABNORMAL BOOT, SSPM_MPU_REGION_ID should be accessible for LKDump */
        type = SEC_RW_NSEC_R;
    } else {
        type = FORBIDDEN;
    }
    region_info.start = (u64)addr;
    region_info.end = (u64)addr + ROUNDUP(SSPM_MEM_SIZE, SSPM_MEM_ALIGN) - 1;
    region_info.region = SSPM_MPU_REGION_ID;
    SET_ACCESS_PERMISSION(region_info.apc, UNLOCK,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, SEC_RW,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, type);

    emi_mpu_set_protection(&region_info);
#endif
}
#endif

static int bldr_load_images(u32 *jump_addr)
{
    int ret = 0;
    blkdev_t *bootdev;
    u32 addr = 0;
    char *name;
    u32 size = 0;
    u32 spare0 = 0;
    u32 spare1 = 0;
    u32 type = 0;
    char active_part_name[PART_NAME_BUF_SZ] = {0};

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        pal_log_err("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        /* FIXME, should change to global error code */
        return -1;
    }

#if MTK_TINYSYS_SSPM_SUPPORT && !CFG_LOAD_SLT_SSPM
    ret = bldr_load_sspm_image(bootdev);
    BOOTING_TIME_PROFILING_LOG("bldr_load_sspm_image");
#endif

#if CFG_LOAD_AP_ROM
    addr = CFG_AP_ROM_MEMADDR;
    ret = bldr_load_part("AP_ROM", bootdev, &addr, &size);
    if (ret) {
	pal_log_info("[ERROR] AP_ROM load fail\n");
	return ret;
    }
#endif

#if CFG_LOAD_BL2_EXT

    memset(active_part_name, 0, PART_NAME_BUF_SZ);
    memcpy(active_part_name, "lk", strlen("lk"));
    ret = partition_get_active(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
    if (ret) {
        pal_log_err("partition_get_active fail: %d\n", ret);
        return ret;
    }

    if (ram_console_is_abnormal_boot()) {
        /* load AEE-LK image */
        addr = CFG_AEE_LK_MEMADDR;
        *jump_addr = addr;
        ret = bldr_load_aee_lk(active_part_name, bootdev, &addr, &size);
    } else {
        addr = mblock_alloc((u64)PL_BL2_EXT_MAX_SIZE, 0x1000,
                (u64)(PL_BL2_EXT_BASE + PL_BL2_EXT_MAX_SIZE), (u64)PL_BL2_EXT_BASE, 1, "system_bl2-ext");
        if (addr != PL_BL2_EXT_BASE) {
            pal_log_err("mblock_alloc: 0x%X, not match 0x%X\n", addr, PL_BL2_EXT_BASE);
            ASSERT(0);
        }
        /*
         * Jumping to BL2 extension does not need jump_addr. However,
         * jump_addr will be filled in bl31_info.bl33_start_addr in trustzone_jump
         * for TF-A jumping to next stage. So let it be LK's address.
         * Once GZ is involved, bl31_info.bl33_start_addr would be modified.
         */
        *jump_addr = CFG_UBOOT_MEMADDR;
        ret = bldr_load_bl2_ext(active_part_name, bootdev, &addr, &size);
    }

    if (ret)
        return ret;

    BOOTING_TIME_PROFILING_LOG("load BL2 extension");
#endif

#if CFG_GZ_SUPPORT
    if (!gz_de_init()) {
        if (strlen("gz") >= PART_NAME_BUF_SZ)
            return -1;

        memset(active_part_name, 0x0, PART_NAME_BUF_SZ);
        memcpy(active_part_name, "gz", strlen("gz"));

        ret = partition_get_active(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
        if (ret)
            return ret;

        ret = bldr_load_gz_part(bootdev, active_part_name);
        if (ret)
            return ret;
    }
    BOOTING_TIME_PROFILING_LOG("load gz");
#endif

#if MTK_TINYSYS_MCUPM_SUPPORT
    addr = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)MCUPM_MEM_SIZE, (u64)MCUPM_MEM_ALIGN,
            MCUPM_MEM_LIMIT, 0, "MCUPM-reserved");

    if (!addr)
        return -1;

#if ENABLE_MPU && (!MCUPM_DBG_MODE)
#define MCUPM_MPU_REGION_ID 20

    struct emi_region_info_t region_info_mcupm;
    region_info_mcupm.start = (u64)addr;
    region_info_mcupm.end = (u64)addr + ROUNDUP(MCUPM_MEM_SIZE, MCUPM_MEM_ALIGN) - 1;
    region_info_mcupm.region = MCUPM_MPU_REGION_ID;
    SET_ACCESS_PERMISSION(region_info_mcupm.apc, UNLOCK,
        FORBIDDEN, NO_PROTECTION, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, NO_PROTECTION);

    emi_mpu_set_protection(&region_info_mcupm);
#endif

    ret = bldr_load_mcupm_part(bootdev, &addr, &size);
    if (ret)
        return ret;
    BOOTING_TIME_PROFILING_LOG("load mcupm");
#endif
    return ret;
}
void mt_usb_phy_recover(void);
void mu3d_hal_rst_dev(void);
/*============================================================================*/
/* GLOBAL FUNCTIONS                                                           */
/*============================================================================*/
void bldr_jump(u32 addr, u32 arg1, u32 arg2)
{
    platform_wdt_kick();

    /* disable preloader safe mode */
    platform_safe_mode(0, 0);

    pal_log_err("\n%s jump to 0x%x\n", MOD, addr);
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr, *(u32*)addr);
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr + 4, *(u32*)(addr + 4));

    jump(addr, arg1, arg2);
}

void bldr_jump64(u32 addr, u32 arg1, u32 arg2)
{
    u64 addr64 = addr;

    platform_wdt_kick();

    /* disable preloader safe mode */
    platform_safe_mode(0, 0);

#if !CFG_BYPASS_EMI
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr, *(u32*)addr);
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr + 4, *(u32*)(addr + 4));
#endif

#if CFG_GZ_SUPPORT
    if (is_booting_el2()) {
        addr64 = gz_get_jump_addr();
        pal_log_info("%s next is GZ at 0x%llx\n", MOD, addr64);
    }
#endif

#if !CFG_BYPASS_EMI
    /* finalize preloader boot time */
    platform_set_boot_time();
#endif

#if CFG_ATF_SUPPORT
    trustzone_jump(addr64, arg1, arg2);
#else
    pal_log_err("%s trustzone is not supported!\n", MOD);
#endif
}

void main(u32 *arg)
{
    struct bldr_command_handler handler;
    u32 jump_addr, jump_arg;

    /* set the vector base */
    ARM64_WRITE_SYSREG(VBAR_EL3, (uint64_t)&arm64_exception_base);

    platform_init_boot_time();

    mtk_uart_init(UART_SRC_CLK_FRQ, CFG_LOG_BAUDRATE);

    /* get the bldr argument */
    COMPILE_ASSERT(BL_PARAM_SIZE == sizeof(bl_param_t));
    p_bldr_param = &bldr_param;
    memcpy((void *)p_bldr_param, (void *)*arg, sizeof(bl_param_t));
    pal_log_info("%s bl_param version=%d (expect=%d)\n", MOD,
        p_bldr_param->ver, BL_PARAM_VERSION);
    BOOTING_TIME_PROFILING_LOG("bldr_param");

    /*
     * Configure Booker and disable HN-D coherence request to avoid
     * receiving NDE before MMU enabled.
     * */
    booker_disable_nde();
    booker_init();
    BOOTING_TIME_PROFILING_LOG("booker_init");

    bldr_pre_process();

    /*
     * Reserve memory & setup MTU after we have the DRAM size information.
     */
    booker_config_mte();

#if !CFG_FPGA_PLATFORM
    wk_vcore_check();
#endif
    #ifdef HW_INIT_ONLY
    device_APC_dom_setup();
    devapc_set_md();
    devapc_set_debugsys();
    bldr_wait_forever();
    #endif

    handler.priv = NULL;
    handler.attr = 0;
    handler.cb   = bldr_cmd_handler;

    BOOTING_TIME_PROFILING_LOG("before bldr_handshake");
    bldr_handshake(&handler);
    BOOTING_TIME_PROFILING_LOG("bldr_handshake");

#if BOOTROM_INTEGRITY_CHECK
	/* if rom integrity check fails, device halts, so don't put it before bootloader
	   handshake, this could make device bricked */
	rom_integrity_check();
#endif

#if !CFG_FPGA_PLATFORM
    /* security check */
    device_APC_dom_setup();
    devapc_set_debugsys();
#endif
    BOOTING_TIME_PROFILING_LOG("sec_boot_check");

#if CFG_ATF_SUPPORT && !CFG_BYPASS_EMI
    trustzone_pre_init();
    BOOTING_TIME_PROFILING_LOG("trustzone pre init");
#endif

#if (defined(MTK_AB_OTA_UPDATER) && !defined(EARLY_PARTITION_ACCESS))
    ab_ota_boot_check();
    BOOTING_TIME_PROFILING_LOG("ab_ota_boot_check");
#endif

#ifdef SLT
    if (bldr_load_slt_images(&jump_addr) != 0) {
        pal_log_err("%s Load SLT images failed\n", MOD);
        goto error;
    }
#else //normal boot
#if !(CFG_BYPASS_LOAD_IMG_FORCE_ATF)
    /* Do not load ATF, lk, load by JTAG */
    if (0 != bldr_load_images(&jump_addr)) {
        pal_log_err("%s Second Bootloader Load Failed\n", MOD);
#if !CFG_BYPASS_EMI
        goto error;
#endif
    }
#else
    jump_addr = CFG_UBOOT_MEMADDR;
#endif
    BOOTING_TIME_PROFILING_LOG("load images");
#endif //SLT

    bldr_post_process();
#ifdef SLT
    mt_usb_phy_recover();
    //mu3d_hal_rst_dev();
#endif

    /* check if ISR EL1 A bit[8] was toggled */
    if (ARM64_READ_SYSREG(isr_el1) & (1U << 8)) {
        pal_log_err("System error detected! Plz check register addr.\n");
        ASSERT(0);
    }

    /* deinit platform before jump to next stage */
    platform_deinit();

    jump_arg = (u32)(boottag);

    /* 64S3,32S1,32S1 (MTK_ATF_BOOT_OPTION = 0)
	 * re-loader jump to LK directly and then LK jump to kernel directly */
#if CFG_ATF_SUPPORT
    bldr_jump64(jump_addr, jump_arg, sizeof(boot_arg_t));
#elif CFG_AP_ROM_ARM64
    jumparch64_slt();
#endif

error:
    platform_error_handler();
}
