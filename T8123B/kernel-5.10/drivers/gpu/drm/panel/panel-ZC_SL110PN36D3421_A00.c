// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#include <linux/backlight.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>
#include <drm/drm_modes.h>
#include <linux/delay.h>
#include <drm/drm_connector.h>
#include <drm/drm_device.h>

#include <linux/gpio/consumer.h>
#include <linux/regulator/consumer.h>

#include <video/mipi_display.h>
#include <video/of_videomode.h>
#include <video/videomode.h>

#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>

#define CONFIG_MTK_PANEL_EXT
#if defined(CONFIG_MTK_PANEL_EXT)
#include "../mediatek/mediatek_v2/mtk_panel_ext.h"
#include "../mediatek/mediatek_v2/mtk_drm_graphics_base.h"
#endif

#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
#include "../mediatek/mediatek_v2/mtk_corner_pattern/mtk_data_hw_roundedpattern.h"
#endif

#if IS_ENABLED(CONFIG_EMDOOR_DEVICEINFO)
#include "../../../misc/mediatek/deviceinfo.h"
extern struct dev_info_hw *hw_dev;
#endif

#define HFP (16)
#define HSA (10)
#define HBP (16)
#define VFP (116)
#define VSA (8)
#define VBP (12)
#define VAC (2000)
#define HAC (1200)
#define PCLK_IN_KHZ \
    ((HAC+HFP+HSA+HBP)*(VAC+VFP+VSA+VBP)*(60)/1000)

static u32 fake_heigh = 2000;
static u32 fake_width = 1200;
static bool need_fake_resolution;

struct lcm {
    struct device *dev;
    struct drm_panel panel;
    struct backlight_device *backlight;
    struct gpio_desc *vdd_en_gpio;
    struct gpio_desc *reset_gpio;
    struct gpio_desc *bias_pos, *bias_neg;

    bool prepared;
    bool enabled;

    unsigned int gate_ic;

    int error;
};

#define lcm_dcs_write_seq(ctx, seq...) \
({\
    const u8 d[] = { seq };\
    BUILD_BUG_ON_MSG(ARRAY_SIZE(d) > 64, "DCS sequence too big for stack");\
    lcm_dcs_write(ctx, d, ARRAY_SIZE(d));\
})

#define lcm_dcs_write_seq_static(ctx, seq...) \
({\
    static const u8 d[] = { seq };\
    lcm_dcs_write(ctx, d, ARRAY_SIZE(d));\
})

static inline struct lcm *panel_to_lcm(struct drm_panel *panel)
{
    return container_of(panel, struct lcm, panel);
}

static void lcm_dcs_write(struct lcm *ctx, const void *data, size_t len)
{
    struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
    ssize_t ret;
    char *addr;

    if (ctx->error < 0)
        return;

    addr = (char *)data;
    if ((int)*addr < 0xB0)
        ret = mipi_dsi_dcs_write_buffer(dsi, data, len);
    else
        ret = mipi_dsi_generic_write(dsi, data, len);
    if (ret < 0) {
        dev_err(ctx->dev, "error %zd writing seq: %ph\n", ret, data);
        ctx->error = ret;
    }
}

#ifdef PANEL_SUPPORT_READBACK
static int lcm_dcs_read(struct lcm *ctx, u8 cmd, void *data, size_t len)
{
    struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
    ssize_t ret;

    if (ctx->error < 0)
        return 0;

    ret = mipi_dsi_dcs_read(dsi, cmd, data, len);
    if (ret < 0) {
        dev_err(ctx->dev, "error %d reading dcs seq:(%#x)\n", ret, cmd);
        ctx->error = ret;
    }

    return ret;
}

static void lcm_panel_get_data(struct lcm *ctx)
{
    u8 buffer[3] = {0};
    static int ret;

    if (ret == 0) {
        ret = lcm_dcs_read(ctx,  0x0A, buffer, 1);
        dev_info(ctx->dev, "return %d data(0x%08x) to dsi engine\n",
             ret, buffer[0] | (buffer[1] << 8));
    }
}
#endif

static void lcm_panel_init(struct lcm *ctx)
{
    lcm_dcs_write_seq_static(ctx, 0xB9,0x83,0x10,0x21,0x55,0x00);
    lcm_dcs_write_seq_static(ctx, 0xB1,0x2C,0xB1,0xB1,0x2D,0xED,0x32,0xD7,0x43,0x36,0x36,0x36,0x36);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xD9);
    lcm_dcs_write_seq_static(ctx, 0xB1,0x78,0x33);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xB2,0x00,0x47,0xB0,0xD0,0x00,0x12,0x72,0x3C,0x9B,0x22,0x00,0x00,0x00,0x88,0xF4);
    lcm_dcs_write_seq_static(ctx, 0xB4,0x6A,0x6A,0x6A,0x6A,0x6A,0x6A,0x6A,0x4D,0x6A,0x4D,0x01,0x95);
    lcm_dcs_write_seq_static(ctx, 0xB6,0x34,0x34,0x03);
    lcm_dcs_write_seq_static(ctx, 0xB8,0x40);
    lcm_dcs_write_seq_static(ctx, 0xBC,0x1B,0x04);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xCD);
    lcm_dcs_write_seq_static(ctx, 0xBA,0x84);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xBE,0x20);
    lcm_dcs_write_seq_static(ctx, 0xBF,0xFC,0xC4);
    lcm_dcs_write_seq_static(ctx, 0xC0,0x32,0x32,0x22,0x11,0x33,0xA0,0x61,0x08,0xF5,0x03);
    lcm_dcs_write_seq_static(ctx, 0xC1,0x01);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xCC);
    lcm_dcs_write_seq_static(ctx, 0xC7,0x80);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xC6);
    lcm_dcs_write_seq_static(ctx, 0xC8,0x97);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xC9,0x00,0x1E,0x30,0xD4,0x01);
    lcm_dcs_write_seq_static(ctx, 0xCB,0x08,0x13,0x07,0x00,0x0E,0xA1);
    lcm_dcs_write_seq_static(ctx, 0xCC,0x02,0x03,0x44);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xC4);
    lcm_dcs_write_seq_static(ctx, 0xD0,0x03);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xD1,0x37,0x06,0x00,0x02,0x04,0x0C,0xFF);
    lcm_dcs_write_seq_static(ctx, 0xD2,0x1F,0x11,0x1F);
    lcm_dcs_write_seq_static(ctx, 0xD3,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6B,0x03,0xAA,0xAB,0x0E,0x0E,0x03,0x03,0x98,0x10,0x08,0x00,0x08,0x32,0x17,0xDE,0x07,0xDE,0x32,0x17,0xE2,0x07,0xE2,0x00,0x00);
    lcm_dcs_write_seq_static(ctx, 0xD5,0x18,0x18,0x18,0x18,0x18,0x18,0x48,0x48,0x18,0x18,0x18,0x18,0x22,0x23,0x1F,0x1F,0x1E,0x1E,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x20,0x21,0x18,0x18,0x18,0x18);
    lcm_dcs_write_seq_static(ctx, 0xD8,0x02,0xAA,0xEA,0xAA,0xAA,0x00,0x02,0xAA,0xEA,0xAA,0xAA,0x00);
    lcm_dcs_write_seq_static(ctx, 0xE0,0x00,0x02,0x08,0x0D,0x12,0x18,0x2C,0x32,0x38,0x34,0x4F,0x56,0x5E,0x70,0x72,0x7E,0x89,0x9E,0x9F,0x50,0x59,0x62,0x68,0x00,0x02,0x08,0x0D,0x12,0x18,0x2C,0x32,0x38,0x34,0x4F,0x56,0x5E,0x70,0x72,0x7E,0x89,0x9E,0x9F,0x50,0x59,0x65,0x74);
    lcm_dcs_write_seq_static(ctx, 0xE7,0x0B,0x10,0x10,0x1E,0x25,0x95,0x01,0x59,0x92,0x14,0x14,0x00,0x00,0x00,0x00,0x12,0x05,0x02,0x02,0x10);
    lcm_dcs_write_seq_static(ctx, 0xBD,0x01);
    lcm_dcs_write_seq_static(ctx, 0xB1,0x01,0xBF,0x11);
    lcm_dcs_write_seq_static(ctx, 0xC1,0x00,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C,0x20,0x24,0x28,0x2D,0x30,0x35,0x39,0x3D,0x41,0x46,0x4A,0x4E,0x56,0x5D,0x65,0x6E,0x76,0x7E,0x86,0x8D,0x96,0x9E,0xA5,0xAD,0xB6,0xBE,0xC6,0xCF,0xD6,0xDE,0xE6,0xED,0xF5,0xF9,0xFA,0xFC,0xFE,0xFF,0x19,0xA6,0xBC,0xDF,0xC0,0x7D,0x97,0x2E,0x24,0xB6,0x4D,0x00);
    lcm_dcs_write_seq_static(ctx, 0xCB,0x86);
    lcm_dcs_write_seq_static(ctx, 0xD2,0x3C);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xC5);
    lcm_dcs_write_seq_static(ctx, 0xD3,0x00,0x00,0x00,0x80,0x80,0x0C,0xA1);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xD8,0x03,0xFF,0xFF,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0x00);
    lcm_dcs_write_seq_static(ctx, 0xE7,0x02,0x00,0x3C,0x01,0x83,0x0F,0x80,0x10,0xA0,0x00,0x00);
    lcm_dcs_write_seq_static(ctx, 0xBD,0x02);
    lcm_dcs_write_seq_static(ctx, 0xC1,0x00,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C,0x20,0x24,0x28,0x2C,0x30,0x34,0x39,0x3D,0x41,0x45,0x49,0x4D,0x55,0x5C,0x64,0x6C,0x75,0x7C,0x84,0x8C,0x94,0x9C,0xA4,0xAB,0xB4,0xBC,0xC4,0xCD,0xD4,0xDD,0xE5,0xEC,0xF4,0xF8,0xFA,0xFC,0xFE,0xFF,0x19,0x95,0x6B,0x70,0x15,0x7B,0x3D,0xA6,0x28,0xC2,0x94,0x00);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xCC);
    lcm_dcs_write_seq_static(ctx, 0xD3,0x02,0x1D,0xE0,0x11,0x70);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xD8,0xAB,0xFF,0xFF,0xFF,0xFF,0xA0,0xAB,0xFF,0xFF,0xFF,0xFF,0xA0);
    lcm_dcs_write_seq_static(ctx, 0xE7,0xFB,0x01,0xFB,0x01,0xFB,0x01,0x00,0x00,0x00,0x26,0x00,0x26,0x81,0x02,0x40,0x00,0x20,0x94,0x05,0x04,0x03,0x02,0x01,0x00,0x00,0x00,0x01,0x00);
    lcm_dcs_write_seq_static(ctx, 0xBD,0x03);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xC6);
    lcm_dcs_write_seq_static(ctx, 0xB4,0x03,0xFF,0xF0);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xC1,0x00,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C,0x1F,0x24,0x28,0x2B,0x2F,0x33,0x38,0x3C,0x3F,0x43,0x47,0x4B,0x53,0x5B,0x62,0x6A,0x73,0x7B,0x82,0x8A,0x92,0x9A,0xA2,0xA9,0xB2,0xBA,0xC3,0xCB,0xD3,0xDB,0xE4,0xEB,0xF3,0xF8,0xFA,0xFC,0xFE,0xFF,0x11,0x90,0xC3,0xA0,0xFF,0xDB,0x0D,0x7B,0x72,0xA2,0xC0,0x00);
    lcm_dcs_write_seq_static(ctx, 0xD8,0xAA,0xAB,0xEA,0xAA,0xAA,0xA0,0xAA,0xAB,0xEA,0xAA,0xAA,0xA0,0xAA,0xBF,0xFF,0xFF,0xFE,0xA0,0xAA,0xBF,0xFF,0xFF,0xFE,0xA0,0xAA,0xAA,0xAA,0xAA,0xAA,0xA0,0xAA,0xAA,0xAA,0xAA,0xAA,0xA0);
    lcm_dcs_write_seq_static(ctx, 0xBD,0x00);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xC4);
    lcm_dcs_write_seq_static(ctx, 0xBA,0x96);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xBD,0x01);
    lcm_dcs_write_seq_static(ctx, 0xE9,0xC5);
    lcm_dcs_write_seq_static(ctx, 0xBA,0x4F);
    lcm_dcs_write_seq_static(ctx, 0xE9,0x3F);
    lcm_dcs_write_seq_static(ctx, 0xBD,0x00);
    lcm_dcs_write_seq_static(ctx, 0xB9,0x00,0x00,0x00);
    lcm_dcs_write_seq_static(ctx, 0x11);
    msleep(120);
    lcm_dcs_write_seq_static(ctx, 0x29);
    msleep(20);
}

static int lcm_disable(struct drm_panel *panel)
{
    struct lcm *ctx = panel_to_lcm(panel);

    if (!ctx->enabled)
        return 0;

    if (ctx->backlight) {
        ctx->backlight->props.power = FB_BLANK_POWERDOWN;
        backlight_update_status(ctx->backlight);
    }

    ctx->enabled = false;

    return 0;
}

static int lcm_unprepare(struct drm_panel *panel)
{
    struct lcm *ctx = panel_to_lcm(panel);

    if (!ctx->prepared)
        return 0;

    lcm_dcs_write_seq_static(ctx, 0x28);
    msleep(10);
    lcm_dcs_write_seq_static(ctx, 0x10);
    msleep(120);

    ctx->error = 0;
    ctx->prepared = false;

    ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->reset_gpio)) {
        dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
            __func__, PTR_ERR(ctx->reset_gpio));
        return PTR_ERR(ctx->reset_gpio);
    }
    gpiod_set_value(ctx->reset_gpio, 0);
    devm_gpiod_put(ctx->dev, ctx->reset_gpio);

    if (ctx->gate_ic == 0) {
        ctx->bias_neg = devm_gpiod_get_index(ctx->dev, "bias", 1, GPIOD_OUT_HIGH);
        if (IS_ERR(ctx->bias_neg)) {
            dev_err(ctx->dev, "%s: cannot get bias_neg %ld\n",
                __func__, PTR_ERR(ctx->bias_neg));
            return PTR_ERR(ctx->bias_neg);
        }
        gpiod_set_value(ctx->bias_neg, 0);
        devm_gpiod_put(ctx->dev, ctx->bias_neg);

        udelay(1000);

        ctx->bias_pos = devm_gpiod_get_index(ctx->dev, "bias", 0, GPIOD_OUT_HIGH);
        if (IS_ERR(ctx->bias_pos)) {
            dev_err(ctx->dev, "%s: cannot get bias_pos %ld\n",
                __func__, PTR_ERR(ctx->bias_pos));
            return PTR_ERR(ctx->bias_pos);
        }
        gpiod_set_value(ctx->bias_pos, 0);
        devm_gpiod_put(ctx->dev, ctx->bias_pos);
    }

    udelay(2000);
    ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->vdd_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
            __func__, PTR_ERR(ctx->vdd_en_gpio));
        return PTR_ERR(ctx->vdd_en_gpio);
    }
    gpiod_set_value(ctx->vdd_en_gpio, 0);
    devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

    return 0;
}

static int lcm_prepare(struct drm_panel *panel)
{
    struct lcm *ctx = panel_to_lcm(panel);
    int ret;

    pr_info("%s\n", __func__);
    if (ctx->prepared)
        return 0;

    ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->vdd_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
            __func__, PTR_ERR(ctx->vdd_en_gpio));
        return PTR_ERR(ctx->vdd_en_gpio);
    }
    gpiod_set_value(ctx->vdd_en_gpio, 1);
    devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

    usleep_range(3 * 1000, 5 * 1000);

    if (ctx->gate_ic == 0) {
        ctx->bias_pos = devm_gpiod_get_index(ctx->dev,
            "bias", 0, GPIOD_OUT_HIGH);
        if (IS_ERR(ctx->bias_pos)) {
            dev_err(ctx->dev, "%s: cannot get bias_pos %ld\n",
                __func__, PTR_ERR(ctx->bias_pos));
            return PTR_ERR(ctx->bias_pos);
        }
        gpiod_set_value(ctx->bias_pos, 1);
        devm_gpiod_put(ctx->dev, ctx->bias_pos);

        usleep_range(3 * 1000, 4 * 1000);

        ctx->bias_neg = devm_gpiod_get_index(ctx->dev,
            "bias", 1, GPIOD_OUT_HIGH);
        if (IS_ERR(ctx->bias_neg)) {
            dev_err(ctx->dev, "%s: cannot get bias_neg %ld\n",
                __func__, PTR_ERR(ctx->bias_neg));
            return PTR_ERR(ctx->bias_neg);
        }
        gpiod_set_value(ctx->bias_neg, 1);
        devm_gpiod_put(ctx->dev, ctx->bias_neg);
    }

    usleep_range(10 * 1000, 11 * 1000);

    ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->reset_gpio)) {
        dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
            __func__, PTR_ERR(ctx->reset_gpio));
        return PTR_ERR(ctx->reset_gpio);
    }
    gpiod_set_value(ctx->reset_gpio, 1);
    usleep_range(10 * 1000, 11 * 1000);
    gpiod_set_value(ctx->reset_gpio, 0);
    usleep_range(5 * 1000,6 * 1000);
    gpiod_set_value(ctx->reset_gpio, 1);
    usleep_range(40 * 1000, 41 * 1000);
    devm_gpiod_put(ctx->dev, ctx->reset_gpio);

    lcm_panel_init(ctx);

    ret = ctx->error;
    if (ret < 0)
        lcm_unprepare(panel);

    ctx->prepared = true;

#if defined(CONFIG_MTK_PANEL_EXT)
    mtk_panel_tch_rst(panel);
#endif
#ifdef PANEL_SUPPORT_READBACK
    lcm_panel_get_data(ctx);
#endif

    return ret;
}

static int lcm_enable(struct drm_panel *panel)
{
    struct lcm *ctx = panel_to_lcm(panel);

    if (ctx->enabled)
        return 0;

    if (ctx->backlight) {
        ctx->backlight->props.power = FB_BLANK_UNBLANK;
        backlight_update_status(ctx->backlight);
    }

    ctx->enabled = true;

    return 0;
}

static struct drm_display_mode default_mode = {
    .clock = 159210,
    .hdisplay = HAC,
    .hsync_start = HAC + HFP,
    .hsync_end = HAC + HFP + HSA,
    .htotal = HAC + HFP + HSA + HBP,
    .vdisplay = VAC,
    .vsync_start = VAC + VFP,
    .vsync_end = VAC + VFP + VSA,
    .vtotal = VAC + VFP + VSA + VBP,
    //.vrefresh = 60,
};

static void change_drm_disp_mode_params(struct drm_display_mode *mode)
{
    if (fake_heigh > 0 && fake_heigh < VAC) {
        mode->vdisplay = fake_heigh;
        mode->vsync_start = fake_heigh + VFP;
        mode->vsync_end = fake_heigh + VFP + VSA;
        mode->vtotal = fake_heigh + VFP + VSA + VBP;
    }
    if (fake_width > 0 && fake_width < HAC) {
        mode->hdisplay = fake_width;
        mode->hsync_start = fake_width + HFP;
        mode->hsync_end = fake_width + HFP + HSA;
        mode->htotal = fake_width + HFP + HSA + HBP;
    }
    //calculate clock, ref drm_mode_vrefresh function
    if (fake_heigh == 1440 && fake_width == 720)
        mode->clock = 69820;
}

#if defined(CONFIG_MTK_PANEL_EXT)
static int panel_ext_reset(struct drm_panel *panel, int on)
{
    struct lcm *ctx = panel_to_lcm(panel);

    ctx->reset_gpio =
        devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->reset_gpio)) {
        dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
            __func__, PTR_ERR(ctx->reset_gpio));
        return PTR_ERR(ctx->reset_gpio);
    }
    gpiod_set_value(ctx->reset_gpio, on);
    devm_gpiod_put(ctx->dev, ctx->reset_gpio);

    return 0;
}

static int panel_ata_check(struct drm_panel *panel)
{
    struct lcm *ctx = panel_to_lcm(panel);
    struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
    unsigned char data[3] = {0x00, 0x00, 0x00};
    unsigned char id[3] = {0x83, 0x10, 0x21};
    ssize_t ret;

    ret = mipi_dsi_dcs_read(dsi, 0xDA, &data[0], 1);
    if (ret < 0) {
        pr_err("DA %s error\n", __func__);
        return 0;
    }
    ret = mipi_dsi_dcs_read(dsi, 0xDB, &data[1], 1);
    if (ret < 0) {
        pr_err("DB %s error\n", __func__);
        return 0;
    }

    ret = mipi_dsi_dcs_read(dsi, 0xDC, &data[2], 1);
    if (ret < 0) {
        pr_err("DC %s error\n", __func__);
        return 0;
    }

    pr_info("ATA read data %x %x %x\n", data[0], data[1], data[2]);

    if (data[0] == id[0] &&
        data[1] == id[1] &&
        data[2] == id[2])
        return 1;

    pr_info("ATA expect read data is 0x%X 0x%X\n",
            id[0], id[1]);

    return 0;
}

static int lcm_setbacklight_cmdq(void *dsi, dcs_write_gce cb,
    void *handle, unsigned int level)
{
    char bl_tb0[] = {0x51, 0xFF};

    bl_tb0[1] = level;

    if (!cb)
        return -1;

    cb(dsi, handle, bl_tb0, ARRAY_SIZE(bl_tb0));

    return 0;
}

static int lcm_get_virtual_heigh(void)
{
    return VAC;
}

static int lcm_get_virtual_width(void)
{
    return HAC;
}

#define PHYSICAL_WIDTH      143100
#define PHYSICAL_HEIGHT     238500
static struct mtk_panel_params ext_params = {
    .pll_clk = 510,
    .data_rate = 1020,
    //.vfp_low_power = 750,
    .cust_esd_check = 1,
    .esd_check_enable = 1,
    .lcm_esd_check_table[0] = {
        .cmd = 0x0a,
        .count = 1,
        .para_list[0] = 0x9d,
    },
    .physical_width_um = PHYSICAL_WIDTH,
    .physical_height_um = PHYSICAL_HEIGHT,
    .rotate = MTK_PANEL_ROTATE_180,
};

static struct mtk_panel_funcs ext_funcs = {
    .reset = panel_ext_reset,
    .set_backlight_cmdq = lcm_setbacklight_cmdq,
    .ata_check = panel_ata_check,
    .get_virtual_heigh = lcm_get_virtual_heigh,
    .get_virtual_width = lcm_get_virtual_width,
};
#endif

struct panel_desc {
    const struct drm_display_mode *modes;
    unsigned int num_modes;

    unsigned int bpc;

    struct {
        unsigned int width;
        unsigned int height;
    } size;

    struct {
        unsigned int prepare;
        unsigned int enable;
        unsigned int disable;
        unsigned int unprepare;
    } delay;
};

static int lcm_get_modes(struct drm_panel *panel, struct drm_connector *connector)
{
    struct drm_display_mode *mode;

    if (need_fake_resolution)
        change_drm_disp_mode_params(&default_mode);
    mode = drm_mode_duplicate(connector->dev, &default_mode);
    if (!mode) {
        dev_info(connector->dev->dev, "failed to add mode %ux%ux@%u\n",
            default_mode.hdisplay, default_mode.vdisplay,
            drm_mode_vrefresh(&default_mode));
        return -ENOMEM;
    }

    drm_mode_set_name(mode);
    mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
    drm_mode_probed_add(connector, mode);

    connector->display_info.width_mm = 143;
    connector->display_info.height_mm = 238;

    return 1;
}

static const struct drm_panel_funcs lcm_drm_funcs = {
    .disable = lcm_disable,
    .unprepare = lcm_unprepare,
    .prepare = lcm_prepare,
    .enable = lcm_enable,
    .get_modes = lcm_get_modes,
};

static void check_is_need_fake_resolution(struct device *dev)
{
    unsigned int ret = 0;

    ret = of_property_read_u32(dev->of_node, "fake_heigh", &fake_heigh);
    if (ret) {
        need_fake_resolution = false;
        return;
    }
    ret = of_property_read_u32(dev->of_node, "fake_width", &fake_width);
    if (ret) {
        need_fake_resolution = false;
        return;
    }
    if (fake_heigh > 0 && fake_heigh < VAC && fake_width > 0 && fake_width < HAC)
        need_fake_resolution = true;
    else
        need_fake_resolution = false;
}

static int lcm_probe(struct mipi_dsi_device *dsi)
{
    struct device *dev = &dsi->dev;
    struct device_node *dsi_node, *remote_node = NULL, *endpoint = NULL;
    struct lcm *ctx;
    struct device_node *backlight;
    unsigned int value;
    int ret;

    pr_info("%s+\n", __func__);
    dsi_node = of_get_parent(dev->of_node);
    if (dsi_node) {
        endpoint = of_graph_get_next_endpoint(dsi_node, NULL);
        if (endpoint) {
            remote_node = of_graph_get_remote_port_parent(endpoint);
            if (!remote_node) {
                pr_info("No panel connected,skip probe lcm\n");
                return -ENODEV;
            }
            pr_info("device node name:%s\n", remote_node->name);
        }
    }
    if (remote_node != dev->of_node) {
        pr_info("%s+ skip probe due to not current lcm\n", __func__);
        return -ENODEV;
    }

    ctx = devm_kzalloc(dev, sizeof(struct lcm), GFP_KERNEL);
    if (!ctx)
        return -ENOMEM;

    mipi_dsi_set_drvdata(dsi, ctx);

    ctx->dev = dev;
    dsi->lanes = 4;
    dsi->format = MIPI_DSI_FMT_RGB888;
    dsi->mode_flags = MIPI_DSI_MODE_VIDEO
            | MIPI_DSI_MODE_LPM | MIPI_DSI_MODE_EOT_PACKET
             | MIPI_DSI_CLOCK_NON_CONTINUOUS;

    ret = of_property_read_u32(dev->of_node, "gate-ic", &value);
    if (ret < 0)
        value = 0;
    else
        ctx->gate_ic = value;

    backlight = of_parse_phandle(dev->of_node, "backlight", 0);
    if (backlight) {
        ctx->backlight = of_find_backlight_by_node(backlight);
        of_node_put(backlight);

        if (!ctx->backlight)
            return -EPROBE_DEFER;
    }

    ctx->vdd_en_gpio = devm_gpiod_get(dev, "vdd_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->vdd_en_gpio)) {
        dev_err(dev, "%s: cannot get vdd_en-gpios %ld\n",
            __func__, PTR_ERR(ctx->vdd_en_gpio));
        return PTR_ERR(ctx->vdd_en_gpio);
    }
    devm_gpiod_put(dev, ctx->vdd_en_gpio);

    ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->reset_gpio)) {
        dev_err(dev, "%s: cannot get reset-gpios %ld\n",
            __func__, PTR_ERR(ctx->reset_gpio));
        return PTR_ERR(ctx->reset_gpio);
    }
    devm_gpiod_put(dev, ctx->reset_gpio);

    if (ctx->gate_ic == 0) {
        ctx->bias_pos = devm_gpiod_get_index(dev, "bias", 0, GPIOD_OUT_HIGH);
        if (IS_ERR(ctx->bias_pos)) {
            dev_err(dev, "%s: cannot get bias-pos 0 %ld\n",
                __func__, PTR_ERR(ctx->bias_pos));
            return PTR_ERR(ctx->bias_pos);
        }
        devm_gpiod_put(dev, ctx->bias_pos);

        ctx->bias_neg = devm_gpiod_get_index(dev, "bias", 1, GPIOD_OUT_HIGH);
        if (IS_ERR(ctx->bias_neg)) {
            dev_err(dev, "%s: cannot get bias-neg 1 %ld\n",
                __func__, PTR_ERR(ctx->bias_neg));
            return PTR_ERR(ctx->bias_neg);
        }
        devm_gpiod_put(dev, ctx->bias_neg);
    }

    ctx->prepared = true;
    ctx->enabled = true;

    drm_panel_init(&ctx->panel, dev, &lcm_drm_funcs, DRM_MODE_CONNECTOR_DSI);

    drm_panel_add(&ctx->panel);

    ret = mipi_dsi_attach(dsi);
    if (ret < 0)
        drm_panel_remove(&ctx->panel);

#if defined(CONFIG_MTK_PANEL_EXT)
    mtk_panel_tch_handle_reg(&ctx->panel);
    ret = mtk_panel_ext_create(dev, &ext_params, &ext_funcs, &ctx->panel);
    if (ret < 0)
        return ret;
#endif
    check_is_need_fake_resolution(dev);
    pr_info("%s-\n", __func__);

#if IS_ENABLED(CONFIG_EMDOOR_DEVICEINFO)
    if(hw_dev) {
        strcpy(hw_dev->dev_LCM.model, "ZC_SL110PN36D3421_A00");
        strcpy(hw_dev->dev_LCM.supplier, "AOLY");
        strcpy(hw_dev->dev_LCM.param, "1200x2000");
        strcpy(hw_dev->dev_LCM.tclid, "NA");
    }
#endif

    return ret;
}


static int lcm_remove(struct mipi_dsi_device *dsi)
{
    struct lcm *ctx = mipi_dsi_get_drvdata(dsi);
#if defined(CONFIG_MTK_PANEL_EXT)
    struct mtk_panel_ctx *ext_ctx = find_panel_ctx(&ctx->panel);
#endif

    mipi_dsi_detach(dsi);
    drm_panel_remove(&ctx->panel);

#if defined(CONFIG_MTK_PANEL_EXT)
    mtk_panel_detach(ext_ctx);
    mtk_panel_remove(ext_ctx);
#endif

    return 0;
}

static void lcm_shutdown(struct mipi_dsi_device *dsi)
{
	struct lcm *ctx = mipi_dsi_get_drvdata(dsi);

	lcm_disable(&ctx->panel);
}

static const struct of_device_id lcm_of_match[] = {
    { .compatible = "aoly,zc_sl110pn36d3421_a00,vdo", },
    { }
};

MODULE_DEVICE_TABLE(of, lcm_of_match);

static struct mipi_dsi_driver lcm_driver = {
    .probe = lcm_probe,
    .remove = lcm_remove,
    .driver = {
        .name = "panel-ZC_SL110PN36D3421_A00",
        .owner = THIS_MODULE,
        .of_match_table = lcm_of_match,
    },
    .shutdown = lcm_shutdown,
};

module_mipi_dsi_driver(lcm_driver);

MODULE_AUTHOR("Xiuhai Deng <xiuhai.deng@mediatek.com>");
MODULE_DESCRIPTION("ZC_SL110PN36D3421_A00 LCD Panel Driver");
MODULE_LICENSE("GPL v2");
