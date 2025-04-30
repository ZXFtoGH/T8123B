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
#include <linux/i2c.h>

#define HFP (18)
#define HSA (12)
#define HBP (48)
#define VFP (220)
#define VSA (8)
#define VBP (28)
#define VAC (2176)
#define HAC (1600)
#define PCLK_IN_KHZ \
    ((HAC+HFP+HSA+HBP)*(VAC+VFP+VSA+VBP)*(60)/1000)

static u32 fake_heigh = 2176;
static u32 fake_width = 1600;
static bool need_fake_resolution;

extern struct i2c_client *gm8773c_global_client;
extern void gm8773c_config_init(struct i2c_client *client);

struct lcm {
    struct device *dev;
    struct drm_panel panel;
    struct backlight_device *backlight;
    struct gpio_desc *vdd_en_gpio;
    struct gpio_desc *reset_gpio;
    struct gpio_desc *bias_pos, *bias_neg;
    struct gpio_desc *ivdd12_en_gpio;
    struct gpio_desc *ovdd18_en_gpio;

    bool prepared;
    bool enabled;

    unsigned int gate_ic;

    int error;
};

static inline struct lcm *panel_to_lcm(struct drm_panel *panel)
{
    return container_of(panel, struct lcm, panel);
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

static int lcm_disable(struct drm_panel *panel)
{
    struct lcm *ctx = panel_to_lcm(panel);

    pr_err(">>>%s\n", __func__);
    
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

    pr_err(">>>%s\n", __func__);
    
    if (!ctx->prepared)
        return 0;


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

    ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->vdd_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
            __func__, PTR_ERR(ctx->vdd_en_gpio));
        return PTR_ERR(ctx->vdd_en_gpio);
    }
    gpiod_set_value(ctx->vdd_en_gpio, 0);
    devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

    //ovdd18 off
    ctx->ovdd18_en_gpio = devm_gpiod_get(ctx->dev, "ovdd18_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->ovdd18_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get ovdd18_en-gpios %ld\n",
            __func__, PTR_ERR(ctx->ovdd18_en_gpio));
        return PTR_ERR(ctx->ovdd18_en_gpio);
    }
    gpiod_set_value(ctx->ovdd18_en_gpio, 0);
    devm_gpiod_put(ctx->dev, ctx->ovdd18_en_gpio);

    //ivdd12 off
    ctx->ivdd12_en_gpio = devm_gpiod_get(ctx->dev, "ivdd12_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->ivdd12_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get ivdd12_en-gpios %ld\n",
            __func__, PTR_ERR(ctx->ivdd12_en_gpio));
        return PTR_ERR(ctx->ivdd12_en_gpio);
    }
    gpiod_set_value(ctx->ivdd12_en_gpio, 0);
    devm_gpiod_put(ctx->dev, ctx->ivdd12_en_gpio);
    return 0;
}

static int lcm_prepare(struct drm_panel *panel)
{
    struct lcm *ctx = panel_to_lcm(panel);
    int ret;

    pr_info("%s\n", __func__);

    if (ctx->prepared)
        return 0;

    //ivdd12
    ctx->ivdd12_en_gpio = devm_gpiod_get(ctx->dev, "ivdd12_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->ivdd12_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get ivdd12_en-gpios %ld\n",
            __func__, PTR_ERR(ctx->ivdd12_en_gpio));
        return PTR_ERR(ctx->ivdd12_en_gpio);
    }
    gpiod_set_value(ctx->ivdd12_en_gpio, 1);
    devm_gpiod_put(ctx->dev, ctx->ivdd12_en_gpio);

    //ovdd18
    ctx->ovdd18_en_gpio = devm_gpiod_get(ctx->dev, "ovdd18_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->ovdd18_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get ovdd18_en-gpios %ld\n",
            __func__, PTR_ERR(ctx->ovdd18_en_gpio));
        return PTR_ERR(ctx->ovdd18_en_gpio);
    }
    gpiod_set_value(ctx->ovdd18_en_gpio, 1);
    devm_gpiod_put(ctx->dev, ctx->ovdd18_en_gpio);

    ctx->vdd_en_gpio = devm_gpiod_get(ctx->dev, "vdd_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->vdd_en_gpio)) {
        dev_err(ctx->dev, "%s: cannot get vdd_en_gpio %ld\n",
            __func__, PTR_ERR(ctx->vdd_en_gpio));
        return PTR_ERR(ctx->vdd_en_gpio);
    }
    gpiod_set_value(ctx->vdd_en_gpio, 1);
    devm_gpiod_put(ctx->dev, ctx->vdd_en_gpio);

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

        udelay(2000);

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

    ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->reset_gpio)) {
        dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
            __func__, PTR_ERR(ctx->reset_gpio));
        return PTR_ERR(ctx->reset_gpio);
    }
    gpiod_set_value(ctx->reset_gpio, 1);
    usleep_range(10 * 1000, 11 * 1000);
    gpiod_set_value(ctx->reset_gpio, 0);
    usleep_range(10 * 1000, 11 * 1000);
    gpiod_set_value(ctx->reset_gpio, 1);
    usleep_range(10 * 1000, 11 * 1000);
    devm_gpiod_put(ctx->dev, ctx->reset_gpio);
    msleep(80);

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

    pr_err(">>>%s\n", __func__);
    gm8773c_config_init(gm8773c_global_client);
    
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
    .clock = PCLK_IN_KHZ,// htotal * vtotal * vrefresh /1000
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
    return 1;
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

#define PHYSICAL_WIDTH      191040
#define PHYSICAL_HEIGHT     259814
static struct mtk_panel_params ext_params = {
    .pll_clk = 790,
    // .data_rate = 1224,
    //.vfp_low_power = 750,
    .cust_esd_check = 0,
    .esd_check_enable = 0,
    .ssc_enable = 0,
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

static int lcm_get_modes(struct drm_panel *panel, struct drm_connector *connector)//1111
{
    struct drm_display_mode *mode;

    if (need_fake_resolution)
        change_drm_disp_mode_params(&default_mode);
    mode = drm_mode_duplicate(connector->dev, &default_mode);//11111
    if (!mode) {
        dev_info(connector->dev->dev, "failed to add mode %ux%ux@%u\n",
            default_mode.hdisplay, default_mode.vdisplay,
            drm_mode_vrefresh(&default_mode));
        return -ENOMEM;
    }

    drm_mode_set_name(mode);
    mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
    drm_mode_probed_add(connector, mode);

    connector->display_info.width_mm = 135;
    connector->display_info.height_mm = 225;

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

    pr_err(">>>%s+\n", __func__);
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
    dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE;

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

    ctx->ivdd12_en_gpio = devm_gpiod_get(dev, "ivdd12_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->ivdd12_en_gpio)) {
        dev_err(dev, "%s: cannot get ivdd12_en-gpios %ld\n",
            __func__, PTR_ERR(ctx->ivdd12_en_gpio));
        return PTR_ERR(ctx->ivdd12_en_gpio);
    }
    devm_gpiod_put(dev, ctx->ivdd12_en_gpio);

    ctx->ovdd18_en_gpio = devm_gpiod_get(dev, "ovdd18_en", GPIOD_OUT_HIGH);
    if (IS_ERR(ctx->ovdd18_en_gpio)) {
        dev_err(dev, "%s: cannot get ovdd18_en-gpios %ld\n",
            __func__, PTR_ERR(ctx->ovdd18_en_gpio));
        return PTR_ERR(ctx->ovdd18_en_gpio);
    }
    devm_gpiod_put(dev, ctx->ovdd18_en_gpio);

    ctx->prepared = true;
    ctx->enabled = true;

    drm_panel_init(&ctx->panel, dev, &lcm_drm_funcs, DRM_MODE_CONNECTOR_DSI);

    drm_panel_add(&ctx->panel);

    ret = mipi_dsi_attach(dsi);
    if (ret < 0)
        drm_panel_remove(&ctx->panel);

#if defined(CONFIG_MTK_PANEL_EXT)
    mtk_panel_tch_handle_reg(&ctx->panel);
    ret = mtk_panel_ext_create(dev, &ext_params, &ext_funcs, &ctx->panel);//111
    if (ret < 0)
        return ret;
#endif
    check_is_need_fake_resolution(dev);
    pr_err(">>>%s-\n", __func__);

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

static const struct of_device_id lcm_of_match[] = {
    { .compatible = "zj,C00ZJP127V21021,vdo", },
    { }
};

MODULE_DEVICE_TABLE(of, lcm_of_match);

static struct mipi_dsi_driver lcm_driver = {
    .probe = lcm_probe,
    .remove = lcm_remove,
    .driver = {
        .name = "panel-C00ZJP127V21021",
        .owner = THIS_MODULE,
        .of_match_table = lcm_of_match,
    },
};

module_mipi_dsi_driver(lcm_driver);

MODULE_AUTHOR("Xiuhai Deng <xiuhai.deng@mediatek.com>");
MODULE_DESCRIPTION("C00ZJP127V21021 LCD Panel Driver");
MODULE_LICENSE("GPL v2");
