/*
*

//ko_order_table.csv
emdoor_led.ko, /drivers/misc/mediated/emdoor_led.ko, ramdisk, Y, Y, userdebug

//tb8786p1_64_k510_defconfig
CONFIG_LED_CONTROL=y

//Kconfig
config EMDOOR_LED
	tristate "EMDOOR_LED Support"
	depends on LEDS_CLASS
	help
	  This selects the MediaTek(R) LEDs connected to MediaTek chips.
	  If you want to use MediaTek(R) LEDs support, say Y or M here.
	  If unsure, say N.
	  Mediatek LED module includes backlight, indicate LED, keypad LED, button LED etc.

//Makefile
obj-$(CONFIG_EMDOOR_LED) += emdoor_led.o

//mt6768.dts
/ {
	model = "MT6768";
	compatible = "mediatek,MT6768";
	interrupt-parent = <&gic>;
	#address-cells = <2>;
	#size-cells = <2>;
	
	em_led:em_led{ 
		compatible = "emdoor_led";
		gpio-led_red = <&pio 160 0>;
		gpio-led_green = <&pio 151 0>;
		
	};

*
*/


//kernel-5.10/drivers/misc/mediatek/emdoor_led.c
#include <dt-bindings/gpio/gpio.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/delay.h>
#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/pwm_backlight.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

static int red_led_gpio;
static int green_led_gpio;

static ssize_t set_red_led(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    if (*(buf) == '0'){
        gpio_set_value(red_led_gpio, 0);
        printk("set_red_led 0\n");
    }

    if ( *(buf) == '1'){
        gpio_set_value(red_led_gpio, 1);
        printk("set_red_led 1\n");
    }

    return count;
}

static ssize_t get_red_led(struct device *dev, struct device_attribute *attr, char *buf)
{
    int cnt = 0;
    int gpio_val;

    gpio_val = gpio_get_value(red_led_gpio);
    cnt += snprintf(buf + cnt, 5, "%d\n", gpio_val);
    return cnt;
}

static ssize_t set_green_led(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    if (*(buf) == '0'){
        gpio_set_value(green_led_gpio, 0);
        printk("set_green_led 0\n");
    }

    if ( *(buf) == '1'){
        gpio_set_value(green_led_gpio, 1);
        printk("set_green_led 1\n");
    }

    return count;
}

static ssize_t get_green_led(struct device *dev, struct device_attribute *attr, char *buf)
{
    int cnt = 0;
    int gpio_val;

    gpio_val = gpio_get_value(green_led_gpio);
    cnt += snprintf(buf + cnt, 5, "%d\n", gpio_val);
    return cnt;
}

static DEVICE_ATTR(em_led_red, 0664, get_red_led, set_red_led);
static DEVICE_ATTR(em_led_green, 0664, get_green_led, set_green_led);

static int em_led_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    red_led_gpio = of_get_named_gpio(np, "gpio-led_red", 0);
    green_led_gpio = of_get_named_gpio(np, "gpio-led_green", 0);
    device_create_file(&pdev->dev, &dev_attr_em_led_red);
    device_create_file(&pdev->dev, &dev_attr_em_led_green);
    printk("%s probe success red_led_gpio = %d, green_led_gpio = %d\n", __func__, red_led_gpio, green_led_gpio);
    return 0;
}

static int em_led_remove(struct platform_device *pdev)
{
    return 0;
}

static int em_led_suspend(struct device *dev)
{
    return 0;
}

static int em_led_resume(struct device *dev)
{
    return 0;
}

static const struct dev_pm_ops em_led_pm_ops = {
    .suspend = em_led_suspend,
    .resume = em_led_resume,
    .poweroff = em_led_suspend,
    .restore = em_led_resume,
};

//设备匹配表（of_device_id）, 用于 设备树（Device Tree）匹配，让内核知道该驱动兼容哪些设备树节点
static const struct of_device_id em_led_of_match[] = {
    {.compatible = "emdoor_led"},   // 匹配设备树中的 compatible 属性
    {},
};
MODULE_DEVICE_TABLE(of, em_led_of_match);   // 导出匹配表，供内核使用

//平台设备ID表（platform_device_id）, 用于 非设备树（传统方式）匹配，适用于旧版内核或非设备树系统。
static const struct platform_device_id mt_id_table[] = {
    {"emdoor, led", 0}, // 匹配 platform_device 的 name 或 id
    {},
};

static struct platform_driver em_led_driver = {
    .driver = {
        .name = "em_led",   // 驱动名称（用于非设备树匹配）
        .owner = THIS_MODULE,
        .pm = &em_led_pm_ops,
        .of_match_table = of_match_ptr(em_led_of_match),    // 设备树匹配表
    },
    .probe = em_led_probe,
    .id_table = mt_id_table.
    .remove = em_led_remove,
};
module_platform_driver(em_led_driver);

MODULE_DESCRIPTION("em_led Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:em_led");