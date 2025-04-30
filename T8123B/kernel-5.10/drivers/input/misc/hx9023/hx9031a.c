/**************************************************************
*                                                             *
*  Driver for NanJingTianYiHeXin HX9031A & HX9023E Cap Sensor *
*                                                             *
***************************************************************/
/*
                       客制化配置说明

1. 驱动兼容HX9031A & HX9023E, 通过头文件中的HX9031A_CHIP_SELECT进行选择
2. dts配置：仅供参考！！！
    hx9031a: hx9031a@28 {
        compatible = "tyhx,hx9031a";
        reg = <0x28>;
        tyhx,irq-gpio = <&pio 16 0x0>;
        tyhx,irq-flags = <2>; //1:rising 2:falling
        tyhx,channel-flag = <0x7>;
        //tyhx,power-supply-type =<1>;//可选，如何供电，如果有的话，客户自行移植
        status = "okay";
    };

3. 填充上下电函数：static void hx9031a_power_on(uint8_t on) 如果是常供电，可不用配置此项

4. 根据实际硬件电路形式，FAE协助确认通道配置函数中的cs和channel映射关系：static void hx9031a_ch_cfg()

5. 根据实际需要，在dtsi文件的对应节点中和dts解析函数static int hx9031a_parse_dt(struct device *dev)中添加你需要的其他属性信息，
   如其他gpio和regulator等，参考dts配置中的中断gpio号"tyhx,irq-gpio"和通道数配置"tyhx,channel-flag"是必要的。
   更多的gpio配置，请放在 static int hx9031a_gpio_init(void) 和 static void hx9031a_gpio_deinit(void) 中
*/

#define HX9031A_DRIVER_VER "cf678f" //Change-Id

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/notifier.h>
#include <linux/power_supply.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/irqdomain.h>
#include <linux/irq.h>
#ifdef CONFIG_PM_WAKELOCKS
#include <linux/pm_wakeup.h>
#else
#include <linux/wakelock.h>
#endif
#include "hx9031a.h"

#define HX9031_ENABLE_IRQ

static struct i2c_client *hx9031a_i2c_client = NULL;
static struct hx9031a_platform_data hx9031a_pdata;
static uint8_t ch_enable_status = 0x00;
static uint8_t hx9031a_polling_enable = 0;
static int hx9031a_polling_period_ms = 0;
static volatile uint8_t hx9031a_irq_from_suspend_flag = 0;
static volatile uint8_t hx9031a_irq_en_flag = 1;

static int32_t data_raw[HX9031A_CH_NUM] = {0};
static int32_t data_diff[HX9031A_CH_NUM] = {0};
static int32_t data_lp[HX9031A_CH_NUM] = {0};
static int32_t data_bl[HX9031A_CH_NUM] = {0};
static uint16_t data_offset_dac[HX9031A_CH_NUM] = {0};
static uint8_t hx9031a_data_accuracy = 16;

//hx9031a默认阈值设置值，请客户根据实测修改
static struct hx9031a_near_far_threshold hx9031a_ch_thres[HX9031A_CH_NUM] = {
    {.thr_near = 192, .thr_far = 160}, //ch0
    {.thr_near = 160, .thr_far = 128},
    {.thr_near = 32767, .thr_far = 32767},
    {.thr_near = 32767, .thr_far = 32767},
};

static DEFINE_MUTEX(hx9031a_i2c_rw_mutex);
static DEFINE_MUTEX(hx9031a_ch_en_mutex);
static DEFINE_MUTEX(hx9031a_cali_mutex);

#ifdef CONFIG_PM_WAKELOCKS
static struct wakeup_source *hx9031a_wake_lock = NULL;
#else
static struct wake_lock hx9031a_wake_lock;
#endif

//=============================================={ TYHX i2c wrap begin
//从指定起始寄存器开始，连续写入count个值
static int linux_common_i2c_write(struct i2c_client *client, uint8_t *reg_addr, uint8_t *txbuf, int count)
{
    int ret = -1;
    int ii = 0;
    uint8_t buf[HX9031A_MAX_XFER_SIZE + 1] = {0};
    struct i2c_msg msg[1];

    if(count > HX9031A_MAX_XFER_SIZE) {
        count = HX9031A_MAX_XFER_SIZE;
        PRINT_ERR("block write over size!!!\n");
    }
    buf[0] = *reg_addr;
    memcpy(buf + 1, txbuf, count);

    msg[0].addr = client->addr;
    msg[0].flags = 0;//write
    msg[0].len = count + 1;
    msg[0].buf = buf;

    ret = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));
    if (ARRAY_SIZE(msg) != ret) {
        PRINT_ERR("linux_common_i2c_write failed. ret=%d\n", ret);
        ret = -1;
        for(ii = 0; ii < msg[0].len; ii++) {
            PRINT_ERR("msg[0].addr=0x%04X, msg[0].flags=0x%04X, msg[0].len=0x%04X, msg[0].buf[%02d]=0x%02X\n",
                      msg[0].addr,
                      msg[0].flags,
                      msg[0].len,
                      ii,
                      msg[0].buf[ii]);
        }
    } else {
        ret = 0;
    }

    return ret;
}

//从指定起始寄存器开始，连续读取count个值
static int linux_common_i2c_read(struct i2c_client *client, uint8_t *reg_addr, uint8_t *rxbuf, int count)
{
    int ret = -1;
    int ii = 0;
    struct i2c_msg msg[2];

    if(count > HX9031A_MAX_XFER_SIZE) {
        count = HX9031A_MAX_XFER_SIZE;
        PRINT_ERR("block read over size!!!\n");
    }

    msg[0].addr = client->addr;
    msg[0].flags = 0;//write
    msg[0].len = 1;
    msg[0].buf = reg_addr;

    msg[1].addr = client->addr;
    msg[1].flags = I2C_M_RD;//read
    msg[1].len = count;
    msg[1].buf = rxbuf;

    ret = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));
    if (ARRAY_SIZE(msg) != ret) {
        PRINT_ERR("linux_common_i2c_read failed. ret=%d\n", ret);
        ret = -1;
        PRINT_ERR("msg[0].addr=0x%04X, msg[0].flags=0x%04X, msg[0].len=0x%04X, msg[0].buf[0]=0x%02X\n",
                  msg[0].addr,
                  msg[0].flags,
                  msg[0].len,
                  msg[0].buf[0]);
        if(msg[1].len >= 1) {
            for(ii = 0; ii < msg[1].len; ii++) {
                PRINT_ERR("msg[1].addr=0x%04X, msg[1].flags=0x%04X, msg[1].len=0x%04X, msg[1].buf[%02d]=0x%02X\n",
                          msg[1].addr,
                          msg[1].flags,
                          msg[1].len,
                          ii,
                          msg[1].buf[ii]);
            }
        }
    } else {
        ret = 0;
    }

    return ret;
}

//return 0 for success, return -1 for errors.
static int hx9031a_read(uint8_t addr, uint8_t *rxbuf, int count)
{
    int ret = -1;

    mutex_lock(&hx9031a_i2c_rw_mutex);
    ret = linux_common_i2c_read(hx9031a_i2c_client, &addr, rxbuf, count);
    if(0 != ret) {
        PRINT_ERR("linux_common_i2c_read failed\n");
        goto exit;
    }

exit:
    mutex_unlock(&hx9031a_i2c_rw_mutex);
    return ret;
}

//return 0 for success, return -1 for errors.
static int hx9031a_write(uint8_t addr, uint8_t *txbuf, int count)
{
    int ret = -1;

    mutex_lock(&hx9031a_i2c_rw_mutex);
    ret = linux_common_i2c_write(hx9031a_i2c_client, &addr, txbuf, count);
    if(0 != ret) {
        PRINT_ERR("linux_common_i2c_write failed\n");
        goto exit;
    }

exit:
    mutex_unlock(&hx9031a_i2c_rw_mutex);
    return ret;
}
//==============================================} TYHX i2c wrap end

//=============================================={ TYHX common code begin
static void hx9031a_data_lock(uint8_t lock_flag)
{
    int ret = -1;
    uint8_t rx_buf[1] = {0};

    if(HX9031A_DATA_LOCK == lock_flag) {
        ret = hx9031a_read(RA_DSP_CONFIG_CTRL1_0xc8, rx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_read failed\n");
        }

        rx_buf[0] = 0x10;
        ret = hx9031a_write(RA_DSP_CONFIG_CTRL1_0xc8, rx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
        }
    } else if(HX9031A_DATA_UNLOCK == lock_flag) {
        ret = hx9031a_read(RA_DSP_CONFIG_CTRL1_0xc8, rx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_read failed\n");
        }

        rx_buf[0] = 0x00;
        ret = hx9031a_write(RA_DSP_CONFIG_CTRL1_0xc8, rx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
        }
    } else {
        PRINT_ERR("ERROR!!! hx9031a_data_lock wrong para. now do data unlock!\n");
        ret = hx9031a_read(RA_DSP_CONFIG_CTRL1_0xc8, rx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_read failed\n");
        }

        rx_buf[0] = 0x00;
        ret = hx9031a_write(RA_DSP_CONFIG_CTRL1_0xc8, rx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
        }
    }
}

static int hx9031a_id_check(void)
{
    int ret = -1;
    int ii = 0;
    uint8_t rxbuf[1] = {0};

    for(ii = 0; ii < HX9031A_ID_CHECK_COUNT; ii++) {
        ret = hx9031a_read(RA_ID_REG_0x60, rxbuf, 1);
        if(ret < 0) {
            PRINT_ERR("hx9031a_read failed\n");
            continue;
        }
        hx9031a_pdata.device_id = rxbuf[0];
        if((HX9031A_CHIP_ID == hx9031a_pdata.device_id))
            break;
        else
            continue;
    }

    if(HX9031A_CHIP_ID == hx9031a_pdata.device_id) {
        ret = hx9031a_read(RO_5F_VERION_ID, rxbuf, 1);//hx9031a version id check
        if(ret < 0) {
            PRINT_ERR("hx9031a_read failed\n");
        }
        hx9031a_pdata.version_id = rxbuf[0];
        PRINT_INF("success! hx9031a_pdata.device_id=0x%02X(HX9031A) hx9031a_pdata.version_id=0x%02X\n",
                  hx9031a_pdata.device_id, hx9031a_pdata.version_id);
        return 0;
    } else {
        PRINT_ERR("failed! hx9031a_pdata.device_id=0x%02X(UNKNOW_CHIP_ID) hx9031a_pdata.version_id=0x%02X\n",
                  hx9031a_pdata.device_id, hx9031a_pdata.version_id);
        return -1;
    }
}

static void hx9031a_31as_compatible(void)
{
    int ret = -1;
    uint8_t txbuf[1] = {0};

    if((0x80 == hx9031a_pdata.version_id)) {
        PRINT_INF("HX9031AS\n");

        txbuf[0] = 0x71;
        ret = hx9031a_write(RA_AVG0_NOSR0_CFG_0x10, txbuf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
        }
        txbuf[0] = 0x44;
        ret = hx9031a_write(RA_NOSR12_CFG_0x11, txbuf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
        }
        txbuf[0] = 0x22;
        ret = hx9031a_write(RA_AVG12_CFG_0x13, txbuf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
        }

        PRINT_INF("reg init updated\n");
    } else {
        PRINT_INF("HX9031A\n");
    }
}

static void hx9031a_ch_cfg(void)
{
    int ret = -1;
    int ii = 0;
    uint16_t ch_cfg = 0;
    uint8_t cfg[HX9031A_CH_NUM * 2] = {0};

    uint8_t CS0 = 0;
    uint8_t CS1 = 0;
    uint8_t CS2 = 0;
    uint8_t CS3 = 0;
    uint8_t CS4 = 0;
    uint8_t NA = 16;
    uint8_t CH0_POS = NA;
    uint8_t CH0_NEG = NA;
    uint8_t CH1_POS = NA;
    uint8_t CH1_NEG = NA;
    uint8_t CH2_POS = NA;
    uint8_t CH2_NEG = NA;
    uint8_t CH3_POS = NA;
    uint8_t CH3_NEG = NA;

    ENTER;
    if(HX9023E_ON_BOARD == HX9031A_CHIP_SELECT) {
        CS0 = 0;
        CS1 = 2;
        CS2 = 4;
        CS3 = 6;
        CS4 = 8;
        NA = 16;
        PRINT_INF("HX9023E_ON_BOARD\n");
    } else if(HX9031A_ON_BOARD == HX9031A_CHIP_SELECT) {
        CS0 = 4;
        CS1 = 2;
        CS2 = 6;
        CS3 = 0;
        CS4 = 8;
        NA = 16;
        PRINT_INF("HX9031A_ON_BOARD\n");
    }

    //TODO:通道客制化配置开始 =================================================
    //每个通道的正负极都可以连接到任何一个CS，按需配置。未使用的通道应配置为NA，不可删除！
    CH0_POS = CS2;
    CH0_NEG = NA;
    CH1_POS = CS1;
    CH1_NEG = CS0;
    CH2_POS = CS0;
    CH2_NEG = NA;
    CH3_POS = CS3;
    CH3_NEG = NA;
    //通道客制化配置结束 ======================================================

    ch_cfg = (uint16_t)((0x03 << CH0_POS) + (0x02 << CH0_NEG));
    cfg[ii++] = (uint8_t)(ch_cfg);
    cfg[ii++] = (uint8_t)(ch_cfg >> 8);

    ch_cfg = (uint16_t)((0x03 << CH1_POS) + (0x02 << CH1_NEG));
    cfg[ii++] = (uint8_t)(ch_cfg);
    cfg[ii++] = (uint8_t)(ch_cfg >> 8);

    ch_cfg = (uint16_t)((0x03 << CH2_POS) + (0x02 << CH2_NEG));
    cfg[ii++] = (uint8_t)(ch_cfg);
    cfg[ii++] = (uint8_t)(ch_cfg >> 8);

    ch_cfg = (uint16_t)((0x03 << CH3_POS) + (0x02 << CH3_NEG));
    cfg[ii++] = (uint8_t)(ch_cfg);
    cfg[ii++] = (uint8_t)(ch_cfg >> 8);

    ret = hx9031a_write(RA_CH0_CFG_7_0_0x03, cfg, HX9031A_CH_NUM * 2);
    if(0 != ret) {
        PRINT_ERR("hx9031a_write failed\n");
    }
}

static void hx9031a_reg_init(void)
{
    int ii = 0;
    int ret = -1;

    while(ii < (int)ARRAY_SIZE(hx9031a_reg_init_list)) {
        ret = hx9031a_write(hx9031a_reg_init_list[ii].addr, &hx9031a_reg_init_list[ii].val, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
        }
        ii++;
    }
}

static void hx9031a_read_offset_dac(void)
{
    int ret = -1;
    int ii = 0;
    uint8_t bytes_per_channel = 0;
    uint8_t bytes_all_channels = 0;
    uint8_t rx_buf[HX9031A_CH_NUM * CH_DATA_BYTES_MAX] = {0};
    uint32_t data = 0;

    hx9031a_data_lock(HX9031A_DATA_LOCK);
    bytes_per_channel = CH_DATA_2BYTES;
    bytes_all_channels = HX9031A_CH_NUM * bytes_per_channel;
    ret = hx9031a_read(RA_OFFSET_DAC0_7_0_0x15, rx_buf, bytes_all_channels);
    if(0 == ret) {
        for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
            data = ((rx_buf[ii * bytes_per_channel + 1] << 8) | (rx_buf[ii * bytes_per_channel]));
            data = data & 0xFFF;//12位
            data_offset_dac[ii] = data;
        }
    }
    hx9031a_data_lock(HX9031A_DATA_UNLOCK);

    PRINT_DBG("OFFSET_DAC, %-8d, %-8d, %-8d, %-8d\n", data_offset_dac[0], data_offset_dac[1], data_offset_dac[2], data_offset_dac[3]);
}

static void hx9031a_manual_offset_calibration(uint8_t ch_id)//手动校准指定的某个channel一次
{
    int ret = -1;
    uint8_t buf[1] = {0};

    mutex_lock(&hx9031a_cali_mutex);
    ret = hx9031a_read(RA_OFFSET_CALI_CTRL_0xc2, buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }

    buf[0] |= (1 << (ch_id + 4));
    ret = hx9031a_write(RA_OFFSET_CALI_CTRL_0xc2, buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_write failed\n");
    }

    PRINT_INF("ch_%d will calibrate in next convert cycle (ODR=%dms)\n", ch_id, HX9031A_ODR_MS);//不能在一个ord周期内连续两次调用本函数！
    TYHX_DELAY_MS(HX9031A_ODR_MS);
    mutex_unlock(&hx9031a_cali_mutex);
}

static void hx9031a_manual_offset_calibration_all_chs(void)//手动校准所有channel
{
    int ret = -1;
    uint8_t buf[1] = {0};

    mutex_lock(&hx9031a_cali_mutex);
    ret = hx9031a_read(RA_OFFSET_CALI_CTRL_0xc2, buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }

    buf[0] |= 0xF0;
    ret = hx9031a_write(RA_OFFSET_CALI_CTRL_0xc2, buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_write failed\n");
    }

    PRINT_INF("channels will calibrate in next convert cycle (ODR=%dms)\n", HX9031A_ODR_MS);//不能在一个ord周期内连续两次调用本函数！
    TYHX_DELAY_MS(HX9031A_ODR_MS);
    mutex_unlock(&hx9031a_cali_mutex);
}

static int16_t hx9031a_get_thres_near(uint8_t ch)
{
    int ret = 0;
    uint8_t buf[2] = {0};

    ret = hx9031a_read(RA_PROX_HIGH_DIFF_CFG_CH0_0_0x80 + (ch * CH_DATA_2BYTES), buf, 2);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }

    hx9031a_ch_thres[ch].thr_near = (buf[0] + ((buf[1] & 0x03) << 8)) * 32;
    PRINT_INF("hx9031a_ch_thres[%d].thr_near=%d\n", ch, hx9031a_ch_thres[ch].thr_near);
    return hx9031a_ch_thres[ch].thr_near;
}
static int16_t hx9031a_get_thres_far(uint8_t ch)
{
    int ret = 0;
    uint8_t buf[2] = {0};

    ret = hx9031a_read(RA_PROX_LOW_DIFF_CFG_CH0_0_0x88 + (ch * CH_DATA_2BYTES), buf, 2);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }

    hx9031a_ch_thres[ch].thr_far = (buf[0] + ((buf[1] & 0x03) << 8)) * 32;
    PRINT_INF("hx9031a_ch_thres[%d].thr_far=%d\n", ch, hx9031a_ch_thres[ch].thr_far);
    return hx9031a_ch_thres[ch].thr_far;
}

static int16_t hx9031a_set_thres_near(uint8_t ch, int16_t val)
{
    int ret = -1;
    uint8_t buf[2];

    val /= 32;
    buf[0] = val & 0xFF;
    buf[1] = (val >> 8) & 0x03;
    hx9031a_ch_thres[ch].thr_near = (val & 0x03FF) * 32;
    ret = hx9031a_write(RA_PROX_HIGH_DIFF_CFG_CH0_0_0x80 + (ch * CH_DATA_2BYTES), buf, 2);
    if(0 != ret) {
        PRINT_ERR("hx9031a_write failed\n");
    }

    PRINT_INF("hx9031a_ch_thres[%d].thr_near=%d\n", ch, hx9031a_ch_thres[ch].thr_near);
    return hx9031a_ch_thres[ch].thr_near;
}

static int16_t hx9031a_set_thres_far(uint8_t ch, int16_t val)
{
    int ret = -1;
    uint8_t buf[2];

    val /= 32;
    buf[0] = val & 0xFF;
    buf[1] = (val >> 8) & 0x03;
    hx9031a_ch_thres[ch].thr_far = (val & 0x03FF) * 32;
    ret = hx9031a_write(RA_PROX_LOW_DIFF_CFG_CH0_0_0x88 + (ch * CH_DATA_2BYTES), buf, 2);
    if(0 != ret) {
        PRINT_ERR("hx9031a_write failed\n");
    }

    PRINT_INF("hx9031a_ch_thres[%d].thr_far=%d\n", ch, hx9031a_ch_thres[ch].thr_far);
    return hx9031a_ch_thres[ch].thr_far;
}

static void hx9031a_get_prox_state(void)
{
    int ret = -1;
    uint8_t buf[1] = {0};

    hx9031a_pdata.prox_state_reg = 0;
    ret = hx9031a_read(RA_PROX_STATUS_0x6b, buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }
    hx9031a_pdata.prox_state_reg = buf[0];

    PRINT_INF("prox_state_reg=0x%02X\n", hx9031a_pdata.prox_state_reg);
}

#if 0
static int hx9031a_chs_en(uint8_t on_off)
{
    int ret = -1;
    uint8_t tx_buf[1] = {0};

    if((1 == on_off) && (0 == hx9031a_pdata.chs_en_flag)) {
        hx9031a_pdata.prox_state_reg = 0;
        tx_buf[0] = hx9031a_pdata.channel_used_flag;
        ret = hx9031a_write(RA_CH_NUM_CFG_0x24, tx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
            return -1;
        }
        hx9031a_pdata.chs_en_flag = 1;
        TYHX_DELAY_MS(10);
    } else if((0 == on_off) && (1 == hx9031a_pdata.chs_en_flag)) {
        tx_buf[0] = 0x00;
        ret = hx9031a_write(RA_CH_NUM_CFG_0x24, tx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
            return -1;
        }
        hx9031a_pdata.chs_en_flag = 0;
    }

    PRINT_INF("hx9031a_pdata.chs_en_flag=%d\n", hx9031a_pdata.chs_en_flag);
    return 0;
}
#endif

static void hx9031a_data_select(void)
{
    int ret = -1;
    int ii = 0;
    uint8_t buf[1] = {0};

    ret = hx9031a_read(RA_RAW_BL_RD_CFG_0x38, buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }

    for(ii = 0; ii < 4; ii++) {//ch0~sh3
        hx9031a_pdata.sel_diff[ii] = buf[0] & (0x01 << ii);
        hx9031a_pdata.sel_lp[ii] = !hx9031a_pdata.sel_diff[ii];
        hx9031a_pdata.sel_bl[ii] = buf[0] & (0x10 << ii);
        hx9031a_pdata.sel_raw[ii] = !hx9031a_pdata.sel_bl[ii];
    }

//    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
//        PRINT_INF("diff[%d]=%d\n", ii, hx9031a_pdata.sel_diff[ii]);
//        PRINT_INF("  lp[%d]=%d\n", ii, hx9031a_pdata.sel_lp[ii]);
//        PRINT_INF("  bl[%d]=%d\n", ii, hx9031a_pdata.sel_bl[ii]);
//        PRINT_INF(" raw[%d]=%d\n", ii, hx9031a_pdata.sel_raw[ii]);
//    }
}

static void hx9031a_sample(void)
{
    int ret = -1;
    int ii = 0;
    uint8_t bytes_per_channel = 0;
    uint8_t bytes_all_channels = 0;
    uint8_t rx_buf[HX9031A_CH_NUM * CH_DATA_BYTES_MAX] = {0};
    int32_t data = 0;

    hx9031a_data_lock(HX9031A_DATA_LOCK);
    hx9031a_data_select();
    //====================================================================================================
    bytes_per_channel = CH_DATA_3BYTES;
    bytes_all_channels = HX9031A_CH_NUM * bytes_per_channel;
    ret = hx9031a_read(RA_RAW_BL_CH0_0_0xe8, rx_buf, bytes_all_channels);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }
    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        if(16 == hx9031a_data_accuracy) {
            data = ((rx_buf[ii * bytes_per_channel + 2] << 8) | (rx_buf[ii * bytes_per_channel + 1]));
            data = (data > 0x7FFF) ? (data - (0xFFFF + 1)) : data;
        } else {
            data = ((rx_buf[ii * bytes_per_channel + 2] << 16) | (rx_buf[ii * bytes_per_channel + 1] << 8) | (rx_buf[ii * bytes_per_channel]));
            data = (data > 0x7FFFFF) ? (data - (0xFFFFFF + 1)) : data;
        }
        data_raw[ii] = 0;
        data_bl[ii] = 0;
        if(true == hx9031a_pdata.sel_raw[ii]) {
            data_raw[ii] = data;
        }
        if(true == hx9031a_pdata.sel_bl[ii]) {
            data_bl[ii] = data;
        }
    }
    //====================================================================================================
    bytes_per_channel = CH_DATA_3BYTES;
    bytes_all_channels = HX9031A_CH_NUM * bytes_per_channel;
    ret = hx9031a_read(RA_LP_DIFF_CH0_0_0xf4, rx_buf, bytes_all_channels);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }
    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        if(16 == hx9031a_data_accuracy) {
            data = ((rx_buf[ii * bytes_per_channel + 2] << 8) | (rx_buf[ii * bytes_per_channel + 1]));
            data = (data > 0x7FFF) ? (data - (0xFFFF + 1)) : data;
        } else {
            data = ((rx_buf[ii * bytes_per_channel + 2] << 16) | (rx_buf[ii * bytes_per_channel + 1] << 8) | (rx_buf[ii * bytes_per_channel]));
            data = (data > 0x7FFFFF) ? (data - (0xFFFFFF + 1)) : data;
        }
        data_lp[ii] = 0;
        data_diff[ii] = 0;
        if(true == hx9031a_pdata.sel_lp[ii]) {
            data_lp[ii] = data;
        }
        if(true == hx9031a_pdata.sel_diff[ii]) {
            data_diff[ii] = data;
        }
    }
    //====================================================================================================
    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        if(true == hx9031a_pdata.sel_lp[ii] && true == hx9031a_pdata.sel_bl[ii]) {
            data_diff[ii] = data_lp[ii] - data_bl[ii];
        }
    }
    //====================================================================================================
    bytes_per_channel = CH_DATA_2BYTES;
    bytes_all_channels = HX9031A_CH_NUM * bytes_per_channel;
    ret = hx9031a_read(RA_OFFSET_DAC0_7_0_0x15, rx_buf, bytes_all_channels);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }
    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        data = ((rx_buf[ii * bytes_per_channel + 1] << 8) | (rx_buf[ii * bytes_per_channel]));
        data = data & 0xFFF;//12位
        data_offset_dac[ii] = data;
    }
    //====================================================================================================
    hx9031a_data_lock(HX9031A_DATA_UNLOCK);

    PRINT_DBG("accuracy=%d\n", hx9031a_data_accuracy);
    PRINT_DBG("DIFF  , %-8d, %-8d, %-8d, %-8d\n", data_diff[0], data_diff[1], data_diff[2], data_diff[3]);
    PRINT_DBG("RAW   , %-8d, %-8d, %-8d, %-8d\n", data_raw[0], data_raw[1], data_raw[2], data_raw[3]);
    PRINT_DBG("OFFSET, %-8d, %-8d, %-8d, %-8d\n", data_offset_dac[0], data_offset_dac[1], data_offset_dac[2], data_offset_dac[3]);
    PRINT_DBG("BL    , %-8d, %-8d, %-8d, %-8d\n", data_bl[0], data_bl[1], data_bl[2], data_bl[3]);
    PRINT_DBG("LP    , %-8d, %-8d, %-8d, %-8d\n", data_lp[0], data_lp[1], data_lp[2], data_lp[3]);
}

#if HX9031A_TEST_CHS_EN //通道测试时用全开全关策略
static int hx9031a_ch_en(uint8_t ch_id, uint8_t en)
{
    int ret = -1;
    uint8_t tx_buf[1] = {0};

    en = !!en;
    if(ch_id >= HX9031A_CH_NUM) {
        PRINT_ERR("channel index over range!!! ch_enable_status=0x%02X (ch_id=%d, en=%d)\n",
                  ch_enable_status, ch_id, en);
        return -1;
    }

    if(1 == en) {
        if(0 == ch_enable_status) {
            hx9031a_pdata.prox_state_reg = 0;
            tx_buf[0] = hx9031a_pdata.channel_used_flag;
            ret = hx9031a_write(RA_CH_NUM_CFG_0x24, tx_buf, 1);
            if(0 != ret) {
                PRINT_ERR("hx9031a_write failed\n");
                return -1;
            }
        }
        ch_enable_status |= (1 << ch_id);
        PRINT_INF("ch_enable_status=0x%02X (ch_%d enabled)\n", ch_enable_status, ch_id);
    } else {
        ch_enable_status &= ~(1 << ch_id);
        if(0 == ch_enable_status) {
            tx_buf[0] = 0x00;
            ret = hx9031a_write(RA_CH_NUM_CFG_0x24, tx_buf, 1);
            if(0 != ret) {
                PRINT_ERR("hx9031a_write failed\n");
                return -1;
            }
        }
        PRINT_INF("ch_enable_status=0x%02X (ch_%d disabled)\n", ch_enable_status, ch_id);
    }

    return 0;
}

#else

static int hx9031a_ch_en(uint8_t ch_id, uint8_t en)
{
    int ret = -1;
    uint8_t rx_buf[1] = {0};
    uint8_t tx_buf[1] = {0};

    en = !!en;
    if(ch_id >= HX9031A_CH_NUM) {
        PRINT_ERR("channel index over range!!! ch_enable_status=0x%02X (ch_id=%d, en=%d)\n",
                  ch_enable_status, ch_id, en);
        return -1;
    }

    ret = hx9031a_read(RA_CH_NUM_CFG_0x24, rx_buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
        return -1;
    }
    ch_enable_status = rx_buf[0];

    if(1 == en) {
        if(0 == ch_enable_status) { //开启第一个ch
            hx9031a_pdata.prox_state_reg = 0;
        }
        ch_enable_status |= (1 << ch_id);
        tx_buf[0] = ch_enable_status;
        ret = hx9031a_write(RA_CH_NUM_CFG_0x24, tx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
            return -1;
        }
        PRINT_INF("ch_enable_status=0x%02X (ch_%d enabled)\n", ch_enable_status, ch_id);
        TYHX_DELAY_MS(10);
    } else {
        ch_enable_status &= ~(1 << ch_id);
        tx_buf[0] = ch_enable_status;
        ret = hx9031a_write(RA_CH_NUM_CFG_0x24, tx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
            return -1;
        }
        PRINT_INF("ch_enable_status=0x%02X (ch_%d disabled)\n", ch_enable_status, ch_id);
    }

    return 0;
}
#endif
//==============================================} TYHX common code end

static void hx9031a_disable_irq(unsigned int irq)
{
    if(0 == irq) {
        PRINT_ERR("wrong irq number!\n");
        return;
    }

    if(1 == hx9031a_irq_en_flag) {
        disable_irq_nosync(hx9031a_pdata.irq);
        hx9031a_irq_en_flag = 0;
        PRINT_DBG("irq_%d is disabled!\n", irq);
    } else {
        PRINT_ERR("irq_%d is disabled already!\n", irq);
    }
}

static void hx9031a_enable_irq(unsigned int irq)
{
    if(0 == irq) {
        PRINT_ERR("wrong irq number!\n");
        return;
    }

    if(0 == hx9031a_irq_en_flag) {
        enable_irq(hx9031a_pdata.irq);
        hx9031a_irq_en_flag = 1;
        PRINT_DBG("irq_%d is enabled!\n", irq);
    } else {
        PRINT_ERR("irq_%d is enabled already!\n", irq);
    }
}

static void hx9031a_input_report_abs(void)
{
    int ii = 0;
    uint8_t touch_state = 0;

    for (ii = 0; ii < HX9031A_CH_NUM; ii++) {
        if (false == hx9031a_pdata.chs_info[ii].enabled) {
            PRINT_DBG("ch_%d(name:%s) is disabled, nothing report\n", ii, hx9031a_pdata.chs_info[ii].name);
            continue;
        }
        if (false == hx9031a_pdata.chs_info[ii].used) {
            PRINT_DBG("ch_%d(name:%s) is unused, nothing report\n", ii, hx9031a_pdata.chs_info[ii].name);
            continue;
        }

        touch_state = (hx9031a_pdata.prox_state_reg >> ii) & 0x1;

        if (BODYACTIVE == touch_state) {
            if (hx9031a_pdata.chs_info[ii].state == BODYACTIVE)
                PRINT_DBG("%s already BODYACTIVE, nothing report\n", hx9031a_pdata.chs_info[ii].name);
            else {
#ifdef CONFIG_PM_WAKELOCKS
                __pm_wakeup_event(hx9031a_wake_lock, 1000);
#else
                wake_lock_timeout(&hx9031a_wake_lock, HZ * 1);
#endif
#ifdef SAR_REPORT_VENUS_KEY
                if ( ii == 1 ){         //wifi
                    //near
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR, 1);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR, 0);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                }else if ( ii == 0 ){   //4G
                    //near
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR2, 1);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR2, 0);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                }

                /*
                //for tcl mmi test
                input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, BTN_STYLUS2, 1);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, BTN_STYLUS2, 0);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                */
#else
                input_report_abs(hx9031a_pdata.chs_info[ii].input_dev_abs, ABS_DISTANCE, BODYACTIVE);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
#endif
                hx9031a_pdata.chs_info[ii].state = BODYACTIVE;
                PRINT_DBG("%s report BODYACTIVE(5mm)\n", hx9031a_pdata.chs_info[ii].name);
            }
        } else if (PROXACTIVE == touch_state) {
            if (hx9031a_pdata.chs_info[ii].state == PROXACTIVE)
                PRINT_DBG("%s already PROXACTIVE, nothing report\n", hx9031a_pdata.chs_info[ii].name);
            else {
#ifdef CONFIG_PM_WAKELOCKS
                __pm_wakeup_event(hx9031a_wake_lock, 1000);
#else
                wake_lock_timeout(&hx9031a_wake_lock, HZ * 1);
#endif
#ifdef SAR_REPORT_VENUS_KEY
                if ( ii == 1 ){         //wifi
                    //near
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR, 1);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR, 0);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                }else if ( ii == 0 ){   //4G
                    //near
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR2, 1);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_NEAR2, 0);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                }
                /*
                //for tcl mmi test
                input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, BTN_STYLUS2, 1);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, BTN_STYLUS2, 0);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                */
#else
                input_report_abs(hx9031a_pdata.chs_info[ii].input_dev_abs, ABS_DISTANCE, PROXACTIVE);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
#endif
                hx9031a_pdata.chs_info[ii].state = PROXACTIVE;
                PRINT_DBG("%s report PROXACTIVE(15mm)\n", hx9031a_pdata.chs_info[ii].name);
            }
        } else if (IDLE == touch_state) {
            if (hx9031a_pdata.chs_info[ii].state == IDLE)
                PRINT_DBG("%s already released, nothing report\n", hx9031a_pdata.chs_info[ii].name);
            else {
#ifdef CONFIG_PM_WAKELOCKS
                __pm_wakeup_event(hx9031a_wake_lock, 1000);
#else
                wake_lock_timeout(&hx9031a_wake_lock, HZ * 1);
#endif
#ifdef SAR_REPORT_VENUS_KEY
                if ( ii == 1 ){         //wifi
                    //far
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_FAR, 1);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_FAR, 0);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                 }else if ( ii == 0 ){   //4G
                    //far
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_FAR2, 1);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                    input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, KEY_SAR_FAR2, 0);
                    input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs); 
                 }
                /*
                //for tcl mmi test
                input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, BTN_STYLUS, 1);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                input_report_key(hx9031a_pdata.chs_info[ii].input_dev_abs, BTN_STYLUS, 0);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
                */
#else
                input_report_abs(hx9031a_pdata.chs_info[ii].input_dev_abs, ABS_DISTANCE, IDLE);
                input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
#endif
                hx9031a_pdata.chs_info[ii].state = IDLE;
                PRINT_DBG("%s report released\n", hx9031a_pdata.chs_info[ii].name);
            }
        } else {
            PRINT_ERR("unknow touch state! touch_state=%d\n", touch_state);
        }
    }
}

// 必要DTS属性：
// "tyhx,irq-gpio"     必要！中断对应的gpio number
// "tyhx,channel-flag"  必要！每个channel对应一个input设备，每个bit对应一个channel（channel）。如0xF代表开启0，1，2，3通道
static int hx9031a_parse_dt(struct device *dev)
{
    int ret = -1;
    struct device_node *dt_node = dev->of_node;

    if (NULL == dt_node) {
        PRINT_ERR("No DTS node\n");
        return -ENODEV;
    }

#if HX9031A_TEST_ON_MTK_DEMO_XY6761
    ret = of_property_read_u32(dt_node, "xy6761-fake-irq-gpio", &hx9031a_pdata.irq_gpio);
    if(ret < 0) {
        PRINT_ERR("failed to get irq_gpio from DT\n");
        return -1;
    }
#else
    hx9031a_pdata.irq_gpio = of_get_named_gpio_flags(dt_node, "tyhx,irq-gpio", 0, NULL);
    if(hx9031a_pdata.irq_gpio < 0) {
        PRINT_ERR("failed to get irq_gpio from DT\n");
        return -1;
    }
#endif

    PRINT_INF("hx9031a_pdata.irq_gpio=%d\n", hx9031a_pdata.irq_gpio);

    hx9031a_pdata.channel_used_flag = HX9031A_CH_USED;
    ret = of_property_read_u32(dt_node, "tyhx,channel-flag", &hx9031a_pdata.channel_used_flag);//客户配置：有效通道标志位：9031A最大传入0x0F，9023E最大传入0x07
    if(ret < 0) {
        PRINT_ERR("\"tyhx,channel-flag\" is not set in DT\n");
        return -1;
    }
    if(hx9031a_pdata.channel_used_flag > ((1 << HX9031A_CH_NUM) - 1)) {
        PRINT_ERR("the max value of channel_used_flag is 0x%X\n", ((1 << HX9031A_CH_NUM) - 1));
        return -1;
    }
    PRINT_INF("hx9031a_pdata.channel_used_flag=0x%X\n", hx9031a_pdata.channel_used_flag);

    return 0;
}

static int hx9031a_gpio_init(void)
{
    int ret = -1;
    if (gpio_is_valid(hx9031a_pdata.irq_gpio)) {
        ret = gpio_request(hx9031a_pdata.irq_gpio, "hx9031a_irq_gpio");
        if (ret < 0) {
            PRINT_ERR("gpio_request failed. ret=%d\n", ret);
            return ret;
        }
        ret = gpio_direction_input(hx9031a_pdata.irq_gpio);
        if (ret < 0) {
            PRINT_ERR("gpio_direction_input failed. ret=%d\n", ret);
            gpio_free(hx9031a_pdata.irq_gpio);
            return ret;
        }
    } else {
        PRINT_ERR("Invalid gpio num\n");
        return -1;
    }

    hx9031a_pdata.irq = gpio_to_irq(hx9031a_pdata.irq_gpio);
    PRINT_INF("hx9031a_pdata.irq_gpio=%d hx9031a_pdata.irq=%d\n", hx9031a_pdata.irq_gpio, hx9031a_pdata.irq);
    return 0;
}

static void hx9031a_gpio_deinit(void)
{
    ENTER;
    gpio_free(hx9031a_pdata.irq_gpio);
}

static void hx9031a_power_on(uint8_t on)
{
    if(on) {
        //TODO: 用户自行填充
        PRINT_INF("power on\n");
    } else {
        //TODO: 用户自行填充
        PRINT_INF("power off\n");
    }
}

static int hx9031a_ch_en_hal(uint8_t ch_id, uint8_t enable)//yasin: for upper layer
{
    int ret = -1;

    mutex_lock(&hx9031a_ch_en_mutex);
    if (1 == enable) {
        PRINT_INF("enable ch_%d(name:%s)\n", ch_id, hx9031a_pdata.chs_info[ch_id].name);
        ret = hx9031a_ch_en(ch_id, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_ch_en failed\n");
            mutex_unlock(&hx9031a_ch_en_mutex);
            return -1;
        }
        hx9031a_pdata.chs_info[ch_id].state = IDLE;
        hx9031a_pdata.chs_info[ch_id].enabled = true;

#ifdef CONFIG_PM_WAKELOCKS
        __pm_wakeup_event(hx9031a_wake_lock, 1000);
#else
        wake_lock_timeout(&hx9031a_wake_lock, HZ * 1);
#endif
#ifdef SAR_REPORT_VENUS_KEY
        if ( ch_id == 1 ){              //wifi
            //far
            input_report_key(hx9031a_pdata.chs_info[ch_id].input_dev_abs, KEY_SAR_FAR, 1);
            input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
            input_report_key(hx9031a_pdata.chs_info[ch_id].input_dev_abs, KEY_SAR_FAR, 0);
            input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
        }else if ( ch_id == 0 ){        //4G
            input_report_key(hx9031a_pdata.chs_info[ch_id].input_dev_abs, KEY_SAR_FAR2, 1);
            input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
            input_report_key(hx9031a_pdata.chs_info[ch_id].input_dev_abs, KEY_SAR_FAR2, 0);
            input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
        }
        /*
        //for tcl mmi test
        input_report_key(hx9031a_pdata.chs_info[ch_id].input_dev_abs, BTN_STYLUS, 1);
        input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
        input_report_key(hx9031a_pdata.chs_info[ch_id].input_dev_abs, BTN_STYLUS, 0);
        input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
        */
#else
        input_report_abs(hx9031a_pdata.chs_info[ch_id].input_dev_abs, ABS_DISTANCE, IDLE);
        input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
#endif
    } else if (0 == enable) {
        PRINT_INF("disable ch_%d(name:%s)\n", ch_id, hx9031a_pdata.chs_info[ch_id].name);
        ret = hx9031a_ch_en(ch_id, 0);
        if(0 != ret) {
            PRINT_ERR("hx9031a_ch_en failed\n");
            mutex_unlock(&hx9031a_ch_en_mutex);
            return -1;
        }
        hx9031a_pdata.chs_info[ch_id].state = IDLE;
        hx9031a_pdata.chs_info[ch_id].enabled = false;

#ifdef CONFIG_PM_WAKELOCKS
        __pm_wakeup_event(hx9031a_wake_lock, 1000);
#else
        wake_lock_timeout(&hx9031a_wake_lock, HZ * 1);
#endif
#ifndef SAR_REPORT_VENUS_KEY
        input_report_abs(hx9031a_pdata.chs_info[ch_id].input_dev_abs, ABS_DISTANCE, -1);
        input_sync(hx9031a_pdata.chs_info[ch_id].input_dev_abs);
#endif
    } else {
        PRINT_ERR("unknown enable symbol\n");
    }
    mutex_unlock(&hx9031a_ch_en_mutex);

    return 0;
}

static int hx9031a_reg_reinitialize(void)
{
    int ret = -1;
    int ii = 0;
    uint8_t tx_buf[1] = {0};

    ENTER;
    hx9031a_reg_init();
    hx9031a_ch_cfg();

    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        hx9031a_set_thres_near(ii, hx9031a_ch_thres[ii].thr_near);
        hx9031a_set_thres_far(ii, hx9031a_ch_thres[ii].thr_far);
    }

    if(ch_enable_status > 0) {
#if HX9031A_TEST_CHS_EN
        tx_buf[0] = hx9031a_pdata.channel_used_flag;
#else
        tx_buf[0] = ch_enable_status;
#endif
        ret = hx9031a_write(RA_CH_NUM_CFG_0x24, tx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_write failed\n");
            return -1;
        }
    }
    return 0;
}

static void hx9031a_polling_work_func(struct work_struct *work)
{
    ENTER;
    mutex_lock(&hx9031a_ch_en_mutex);
    hx9031a_sample();
    hx9031a_get_prox_state();
    hx9031a_input_report_abs();

    if(1 == hx9031a_polling_enable)
        schedule_delayed_work(&hx9031a_pdata.polling_work, msecs_to_jiffies(hx9031a_polling_period_ms));
    mutex_unlock(&hx9031a_ch_en_mutex);
    return;
}

#ifdef HX9031_ENABLE_IRQ
static irqreturn_t hx9031a_irq_handler(int irq, void *pvoid)
{
    ENTER;
    mutex_lock(&hx9031a_ch_en_mutex);
    if(1 == hx9031a_irq_from_suspend_flag) {
        hx9031a_irq_from_suspend_flag = 0;
        PRINT_INF("delay 50ms for waiting the i2c controller enter working mode\n");
        msleep(50);//如果从suspend被中断唤醒，该延时确保i2c控制器也从休眠唤醒并进入工作状态
    }
    hx9031a_sample();
    hx9031a_get_prox_state();
    hx9031a_input_report_abs();
    mutex_unlock(&hx9031a_ch_en_mutex);
    return IRQ_HANDLED;
}
#endif

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^sysfs for test begin
static ssize_t hx9031a_raw_data_show(struct class *class, struct class_attribute *attr, char *buf)
{
    char *p = buf;
    int ii = 0;

    ENTER;
    hx9031a_sample();
    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        p += snprintf(p, PAGE_SIZE, "ch[%d]: DIFF=%-8d, RAW=%-8d, OFFSET=%-8d, BL=%-8d, LP=%-8d\n",
                      ii, data_diff[ii], data_raw[ii], data_offset_dac[ii], data_bl[ii], data_lp[ii]);
    }
    return (p - buf);//返回实际放到buf中的实际字符个数
}

static ssize_t hx9031a_reg_write_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int ret = -1;
    unsigned int reg_address = 0;
    unsigned int val = 0;
    uint8_t addr = 0;
    uint8_t tx_buf[1] = {0};

    ENTER;
    if (sscanf(buf, "%x,%x", &reg_address, &val) != 2) {
        PRINT_ERR("please input two HEX numbers: aa,bb (aa: reg_address, bb: value_to_be_set)\n");
        return -EINVAL;
    }

    addr = (uint8_t)reg_address;
    tx_buf[0] = (uint8_t)val;

    ret = hx9031a_write(addr, tx_buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_write failed\n");
    }

    PRINT_INF("WRITE:Reg0x%02X=0x%02X\n", addr, tx_buf[0]);
    return count;
}

static ssize_t hx9031a_reg_read_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int ret = -1;
    unsigned int reg_address = 0;
    uint8_t addr = 0;
    uint8_t rx_buf[1] = {0};

    ENTER;
    if (sscanf(buf, "%x", &reg_address) != 1) {
        PRINT_ERR("please input a HEX number\n");
        return -EINVAL;
    }
    addr = (uint8_t)reg_address;

    ret = hx9031a_read(addr, rx_buf, 1);
    if(0 != ret) {
        PRINT_ERR("hx9031a_read failed\n");
    }

    PRINT_INF("READ:Reg0x%02X=0x%02X\n", addr, rx_buf[0]);
    return count;
}

static ssize_t hx9031a_channel_en_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int ch_id = 0;
    int en = 0;

    ENTER;
    if (sscanf(buf, "%d,%d", &ch_id, &en) != 2) {
        PRINT_ERR("please input two DEC numbers: ch_id,en (ch_id: channel number, en: 1=enable, 0=disable)\n");
        return -EINVAL;
    }

    if((ch_id >= HX9031A_CH_NUM) || (ch_id < 0)) {
        PRINT_ERR("channel number out of range, the effective number is 0~%d\n", HX9031A_CH_NUM - 1);
        return -EINVAL;
    }

    if ((hx9031a_pdata.channel_used_flag >> ch_id) & 0x01) {
        hx9031a_ch_en_hal(ch_id, (en > 0) ? 1 : 0);
    } else {
        PRINT_ERR("ch_%d is unused, you can not enable or disable an unused channel\n", ch_id);
    }

    return count;
}

static ssize_t hx9031a_channel_en_show(struct class *class, struct class_attribute *attr, char *buf)
{
    int ii = 0;
    char *p = buf;

    ENTER;
    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        if ((hx9031a_pdata.channel_used_flag >> ii) & 0x1) {
            PRINT_INF("hx9031a_pdata.chs_info[%d].enabled=%d\n", ii, hx9031a_pdata.chs_info[ii].enabled);
            p += snprintf(p, PAGE_SIZE, "hx9031a_pdata.chs_info[%d].enabled=%d\n", ii, hx9031a_pdata.chs_info[ii].enabled);
        }
    }

    return (p - buf);
}

static ssize_t hx9031a_manual_offset_calibration_show(struct class *class, struct class_attribute *attr, char *buf)
{
    hx9031a_read_offset_dac();
    return sprintf(buf, "OFFSET_DAC, %-8d, %-8d, %-8d, %-8d\n", data_offset_dac[0], data_offset_dac[1], data_offset_dac[2], data_offset_dac[3]);
}

static ssize_t hx9031a_manual_offset_calibration_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    unsigned long val;
    uint8_t ch_id = 0;

    ENTER;
    if (kstrtoul(buf, 10, &val)) {
        PRINT_ERR("Invalid Argument\n");
        return -EINVAL;
    }
    ch_id = (uint8_t)val;

    if (99 == ch_id) {
        PRINT_INF("you are enter the calibration test mode, all channels will be calibrated\n");
        hx9031a_manual_offset_calibration_all_chs();
        return count;
    }

    if (ch_id < HX9031A_CH_NUM)
        hx9031a_manual_offset_calibration(ch_id);
    else
        PRINT_ERR(" \"echo ch_id > calibrate\" to do a manual calibrate(ch_id is a channel num (0~%d)\n", HX9031A_CH_NUM);
    return count;
}

static ssize_t hx9031a_prox_state_show(struct class *class, struct class_attribute *attr, char *buf)
{
    hx9031a_get_prox_state();
    return sprintf(buf, "prox_state_reg=0x%02X\n", hx9031a_pdata.prox_state_reg);
}

static ssize_t hx9031a_polling_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int value = 0;
    int ret = -1;

    ENTER;
    ret = kstrtoint(buf, 10, &value);
    if (0 != ret) {
        PRINT_ERR("kstrtoint failed\n");
        goto exit;
    }

    if (value >= 10) {
        hx9031a_polling_period_ms = value;
        if(1 == hx9031a_polling_enable) {
            PRINT_INF("polling is already enabled!, no need to do enable again!, just update the polling period\n");
            goto exit;
        }

        hx9031a_polling_enable = 1;
        hx9031a_disable_irq(hx9031a_pdata.irq);//关闭中断，并停止中断底半部对应的工作队列

        PRINT_INF("polling started! period=%dms\n", hx9031a_polling_period_ms);
        schedule_delayed_work(&hx9031a_pdata.polling_work, msecs_to_jiffies(hx9031a_polling_period_ms));
    } else {
        if(0 == hx9031a_polling_enable) {
            PRINT_INF("polling is already disabled!, no need to do again!\n");
            goto exit;
        }
        hx9031a_polling_period_ms = 0;
        hx9031a_polling_enable = 0;
        PRINT_INF("polling stoped!\n");

        cancel_delayed_work(&hx9031a_pdata.polling_work);//停止polling对应的工作队列，并重新开启中断模式
        hx9031a_enable_irq(hx9031a_pdata.irq);
    }

exit:
    return count;
}

static ssize_t hx9031a_polling_show(struct class *class, struct class_attribute *attr, char *buf)
{
    PRINT_INF("hx9031a_polling_enable=%d hx9031a_polling_period_ms=%d\n", hx9031a_polling_enable, hx9031a_polling_period_ms);
    return sprintf(buf, "hx9031a_polling_enable=%d hx9031a_polling_period_ms=%d\n", hx9031a_polling_enable, hx9031a_polling_period_ms);
}

static ssize_t hx9031a_loglevel_show(struct class *class, struct class_attribute *attr, char *buf)
{
    PRINT_INF("tyhx_log_level=%d\n", tyhx_log_level);
    return sprintf(buf, "tyhx_log_level=%d\n", tyhx_log_level);
}

static ssize_t hx9031a_loglevel_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int ret = -1;
    int value = 0;

    ret = kstrtoint(buf, 10, &value);
    if (0 != ret) {
        PRINT_ERR("kstrtoint failed\n");
        return count;
    }

    tyhx_log_level = value;
    PRINT_INF("set tyhx_log_level=%d\n", tyhx_log_level);
    return count;
}

static ssize_t hx9031a_accuracy_show(struct class *class, struct class_attribute *attr, char *buf)
{
    PRINT_INF("hx9031a_data_accuracy=%d\n", hx9031a_data_accuracy);
    return sprintf(buf, "hx9031a_data_accuracy=%d\n", hx9031a_data_accuracy);
}

static ssize_t hx9031a_accuracy_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int ret = -1;
    int value = 0;

    ret = kstrtoint(buf, 10, &value);
    if (0 != ret) {
        PRINT_ERR("kstrtoint failed\n");
        return count;
    }

    hx9031a_data_accuracy = (24 == value) ? 24 : 16;
    PRINT_INF("set hx9031a_data_accuracy=%d\n", hx9031a_data_accuracy);
    return count;
}

static ssize_t hx9031a_threshold_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    unsigned int ch = 0;
    unsigned int thr_near = 0;
    unsigned int thr_far = 0;

    ENTER;
    if (sscanf(buf, "%d,%d,%d", &ch, &thr_near, &thr_far) != 3) {
        PRINT_ERR("please input 3 numbers in DEC: ch,thr_near,thr_far (eg: 0,500,300)\n");
        return -EINVAL;
    }

    if(ch >= HX9031A_CH_NUM || thr_near > (0x03FF * 32) || thr_far > thr_near) {
        PRINT_ERR("input value over range! (valid value: ch=%d, thr_near=%d, thr_far=%d)\n", ch, thr_near, thr_far);
        return -EINVAL;
    }

    thr_near = (thr_near / 32) * 32;
    thr_far = (thr_far / 32) * 32;

    PRINT_INF("set threshold: ch=%d, thr_near=%d, thr_far=%d\n", ch, thr_near, thr_far);
    hx9031a_set_thres_near(ch, thr_near);
    hx9031a_set_thres_far(ch, thr_far);

    return count;
}

static ssize_t hx9031a_threshold_show(struct class *class, struct class_attribute *attr, char *buf)
{
    int ii = 0;
    char *p = buf;

    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        hx9031a_get_thres_near(ii);
        hx9031a_get_thres_far(ii);
        PRINT_INF("ch_%d threshold: near=%-8d, far=%-8d\n", ii, hx9031a_ch_thres[ii].thr_near, hx9031a_ch_thres[ii].thr_far);
        p += snprintf(p, PAGE_SIZE, "ch_%d threshold: near=%-8d, far=%-8d\n", ii, hx9031a_ch_thres[ii].thr_near, hx9031a_ch_thres[ii].thr_far);
    }

    return (p - buf);
}

static ssize_t hx9031a_dump_show(struct class *class, struct class_attribute *attr, char *buf)
{
    int ret = -1;
    int ii = 0;
    char *p = buf;
    uint8_t rx_buf[1] = {0};

    for(ii = 0; ii < ARRAY_SIZE(hx9031a_reg_init_list); ii++) {
        ret = hx9031a_read(hx9031a_reg_init_list[ii].addr, rx_buf, 1);
        if(0 != ret) {
            PRINT_ERR("hx9031a_read failed\n");
        }
        PRINT_INF("0x%02X=0x%02X\n", hx9031a_reg_init_list[ii].addr, rx_buf[0]);
        p += snprintf(p, PAGE_SIZE, "0x%02X=0x%02X\n", hx9031a_reg_init_list[ii].addr, rx_buf[0]);
    }

    p += snprintf(p, PAGE_SIZE, "driver version:%s\n", HX9031A_DRIVER_VER);
    return (p - buf);
}

static ssize_t hx9031a_offset_dac_show(struct class *class, struct class_attribute *attr, char *buf)
{
    int ii = 0;
    char *p = buf;

    hx9031a_read_offset_dac();

    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        PRINT_INF("data_offset_dac[%d]=%dpF\n", ii, data_offset_dac[ii] * 58 / 1000);
        p += snprintf(p, PAGE_SIZE, "ch[%d]=%dpF ", ii, data_offset_dac[ii] * 58 / 1000);
    }
    p += snprintf(p, PAGE_SIZE, "\n");

    return (p - buf);
}

static ssize_t hx9031a_reinitialize_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int ret = -1;
    int value = 0;

    ret = kstrtoint(buf, 10, &value);
    if (0 != ret) {
        PRINT_ERR("kstrtoint failed\n");
        return count;
    }

    if(1 == value) {
        PRINT_INF("start reg reinitialize\n");
        hx9031a_reg_reinitialize();
    }
    return count;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
static struct class_attribute class_attr_raw_data = __ATTR(raw_data, 0664, hx9031a_raw_data_show, NULL);
static struct class_attribute class_attr_reg_write = __ATTR(reg_write,  0664, NULL, hx9031a_reg_write_store);
static struct class_attribute class_attr_reg_read = __ATTR(reg_read, 0664, NULL, hx9031a_reg_read_store);
static struct class_attribute class_attr_channel_en = __ATTR(channel_en, 0664, hx9031a_channel_en_show, hx9031a_channel_en_store);
static struct class_attribute class_attr_calibrate = __ATTR(calibrate, 0664, hx9031a_manual_offset_calibration_show, hx9031a_manual_offset_calibration_store);
static struct class_attribute class_attr_prox_state = __ATTR(prox_state, 0664, hx9031a_prox_state_show, NULL);
static struct class_attribute class_attr_polling_period = __ATTR(polling_period, 0664, hx9031a_polling_show, hx9031a_polling_store);
static struct class_attribute class_attr_threshold = __ATTR(threshold, 0664, hx9031a_threshold_show, hx9031a_threshold_store);
static struct class_attribute class_attr_loglevel = __ATTR(loglevel, 0664, hx9031a_loglevel_show, hx9031a_loglevel_store);
static struct class_attribute class_attr_accuracy = __ATTR(accuracy, 0664, hx9031a_accuracy_show, hx9031a_accuracy_store);
static struct class_attribute class_attr_dump = __ATTR(dump, 0664, hx9031a_dump_show, NULL);
static struct class_attribute class_attr_offset_dac = __ATTR(offset_dac, 0664, hx9031a_offset_dac_show, NULL);
static struct class_attribute class_attr_reinitialize = __ATTR(reinitialize,  0664, NULL, hx9031a_reinitialize_store);

static struct attribute *hx9031a_class_attrs[] = {
    &class_attr_raw_data.attr,
    &class_attr_reg_write.attr,
    &class_attr_reg_read.attr,
    &class_attr_channel_en.attr,
    &class_attr_calibrate.attr,
    &class_attr_prox_state.attr,
    &class_attr_polling_period.attr,
    &class_attr_threshold.attr,
    &class_attr_loglevel.attr,
    &class_attr_accuracy.attr,
    &class_attr_dump.attr,
    &class_attr_offset_dac.attr,
    &class_attr_reinitialize.attr,
    NULL,
};
ATTRIBUTE_GROUPS(hx9031a_class);
#else
static struct class_attribute hx9031a_class_attributes[] = {
    __ATTR(raw_data, 0664, hx9031a_raw_data_show, NULL),
    __ATTR(reg_write,  0664, NULL, hx9031a_reg_write_store),
    __ATTR(reg_read, 0664, NULL, hx9031a_reg_read_store),
    __ATTR(channel_en, 0664, hx9031a_channel_en_show, hx9031a_channel_en_store),
    __ATTR(calibrate, 0664, hx9031a_manual_offset_calibration_show, hx9031a_manual_offset_calibration_store),
    __ATTR(prox_state, 0664, hx9031a_prox_state_show, NULL),
    __ATTR(polling_period, 0664, hx9031a_polling_show, hx9031a_polling_store),
    __ATTR(threshold, 0664, hx9031a_threshold_show, hx9031a_threshold_store),
    __ATTR(loglevel, 0664, hx9031a_loglevel_show, hx9031a_loglevel_store),
    __ATTR(accuracy, 0664, hx9031a_accuracy_show, hx9031a_accuracy_store),
    __ATTR(dump, 0664, hx9031a_dump_show, NULL),
    __ATTR(offset_dac, 0664, hx9031a_offset_dac_show, NULL),
    __ATTR(reinitialize,  0664, NULL, hx9031a_reinitialize_store),
    __ATTR_NULL,
};
#endif

struct class hx9031a_class = {
        .name = "hx9031a",
        .owner = THIS_MODULE,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
        .class_groups = hx9031a_class_groups,
#else
        .class_attrs = hx9031a_class_attributes,
#endif
    };
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^sysfs for test end

static int hx9031a_input_init_abs(struct i2c_client *client)
{
    int ii = 0;
    int jj = 0;
    int ret = -1;

    hx9031a_pdata.chs_info = devm_kzalloc(&client->dev,
                                          sizeof(struct hx9031a_channel_info) * HX9031A_CH_NUM,
                                          GFP_KERNEL);
    if (NULL == hx9031a_pdata.chs_info) {
        PRINT_ERR("devm_kzalloc failed\n");
        ret = -ENOMEM;
        goto failed_devm_kzalloc;
    }

    for (ii = 0; ii < HX9031A_CH_NUM; ii++) {
        snprintf(hx9031a_pdata.chs_info[ii].name,
                 sizeof(hx9031a_pdata.chs_info[ii].name),
                 "hx9031a_abs_ch%d",
                 ii);
        PRINT_DBG("name of ch_%d:\"%s\"\n", ii, hx9031a_pdata.chs_info[ii].name);
        hx9031a_pdata.chs_info[ii].used = false;
        hx9031a_pdata.chs_info[ii].enabled = false;

        hx9031a_pdata.chs_info[ii].input_dev_abs = input_allocate_device();
        if (NULL == hx9031a_pdata.chs_info[ii].input_dev_abs) {
            PRINT_ERR("input_allocate_device failed, ii=%d\n", ii);
            ret = -ENOMEM;
            goto failed_input_allocate_device;
        }

        hx9031a_pdata.chs_info[ii].input_dev_abs->name = hx9031a_pdata.chs_info[ii].name;

#ifdef SAR_REPORT_VENUS_KEY
        __set_bit(EV_KEY, hx9031a_pdata.chs_info[ii].input_dev_abs->evbit);
        __set_bit(KEY_SAR_FAR, hx9031a_pdata.chs_info[ii].input_dev_abs->keybit);
        __set_bit(KEY_SAR_NEAR, hx9031a_pdata.chs_info[ii].input_dev_abs->keybit);
        __set_bit(KEY_SAR_FAR2, hx9031a_pdata.chs_info[ii].input_dev_abs->keybit);
        __set_bit(KEY_SAR_NEAR2, hx9031a_pdata.chs_info[ii].input_dev_abs->keybit);
        /*
        //for tcl mmi test
        __set_bit(BTN_STYLUS, hx9031a_pdata.chs_info[ii].input_dev_abs->keybit);
        __set_bit(BTN_STYLUS2, hx9031a_pdata.chs_info[ii].input_dev_abs->keybit);
        */

        ret = input_register_device(hx9031a_pdata.chs_info[ii].input_dev_abs);
#else
        __set_bit(EV_ABS, hx9031a_pdata.chs_info[ii].input_dev_abs->evbit);
        input_set_abs_params(hx9031a_pdata.chs_info[ii].input_dev_abs, ABS_DISTANCE, -1, 100, 0, 0);
        ret = input_register_device(hx9031a_pdata.chs_info[ii].input_dev_abs);
#endif
        if (ret) {
            PRINT_ERR("input_register_device failed, ii=%d\n", ii);
            goto failed_input_register_device;
        }

#ifndef SAR_REPORT_VENUS_KEY
        input_report_abs(hx9031a_pdata.chs_info[ii].input_dev_abs, ABS_DISTANCE, -1);
        input_sync(hx9031a_pdata.chs_info[ii].input_dev_abs);
#endif

        if ((hx9031a_pdata.channel_used_flag >> ii) & 0x1) {
            hx9031a_pdata.chs_info[ii].used = true;
            hx9031a_pdata.chs_info[ii].state = IDLE;
        }
    }

    PRINT_INF("input init success\n");
    return ret;

failed_input_register_device:
    for(jj = ii - 1; jj >= 0; jj--) {
        input_unregister_device(hx9031a_pdata.chs_info[jj].input_dev_abs);
    }
    ii++;
failed_input_allocate_device:
    for(jj = ii - 1; jj >= 0; jj--) {
        input_free_device(hx9031a_pdata.chs_info[jj].input_dev_abs);
    }
    devm_kfree(&client->dev, hx9031a_pdata.chs_info);
failed_devm_kzalloc:
    PRINT_ERR("hx9031a_input_init_abs failed\n");
    return ret;
}

static void hx9031a_input_deinit_abs(struct i2c_client *client)
{
    int ii = 0;

    ENTER;
    for (ii = 0; ii < HX9031A_CH_NUM; ii++) {
        input_unregister_device(hx9031a_pdata.chs_info[ii].input_dev_abs);
        input_free_device(hx9031a_pdata.chs_info[ii].input_dev_abs);
    }
    devm_kfree(&client->dev, hx9031a_pdata.chs_info);
}

static int hx9031a_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ii = 0;
    int ret = 0;

    PRINT_INF("i2c address:0x%02X\n", client->addr);

    if (!i2c_check_functionality(to_i2c_adapter(client->dev.parent), I2C_FUNC_SMBUS_READ_WORD_DATA)) {
        PRINT_ERR("i2c_check_functionality failed\n");
        ret = -EIO;
        goto failed_i2c_check_functionality;
    }

    i2c_set_clientdata(client, &hx9031a_pdata);
    hx9031a_i2c_client = client;
    hx9031a_pdata.pdev = &client->dev;
    client->dev.platform_data = &hx9031a_pdata;

//{begin =============================================需要客户自行配置dts属性和实现上电相关内容
    ret = hx9031a_parse_dt(&client->dev);//yasin: power, irq, regs
    if (ret) {
        PRINT_ERR("hx9031a_parse_dt failed\n");
        ret = -ENODEV;
        goto failed_parse_dt;
    }

    ret = hx9031a_gpio_init();
    if (ret) {
        PRINT_ERR("hx9031a_gpio_init failed\n");
        ret = -1;
        goto failed_gpio_init;
    }

    client->irq = hx9031a_pdata.irq;
    hx9031a_power_on(1);
//}end =============================================================

    ret = hx9031a_id_check();
    if(0 != ret) {
        PRINT_INF("hx9031a_id_check failed, retry\n");
        if(0x28 == client->addr)
            client->addr = 0x2A;
        else
            client->addr = 0x28;
        PRINT_INF("i2c address:0x%02X\n", client->addr);
        ret = hx9031a_id_check();
        if(0 != ret) {
            PRINT_ERR("hx9031a_id_check failed\n");
            goto failed_id_check;
        }
    }

    hx9031a_reg_init();
    hx9031a_ch_cfg();

    hx9031a_31as_compatible();

    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        hx9031a_set_thres_near(ii, hx9031a_ch_thres[ii].thr_near);
        hx9031a_set_thres_far(ii, hx9031a_ch_thres[ii].thr_far);
    }

    INIT_DELAYED_WORK(&hx9031a_pdata.polling_work, hx9031a_polling_work_func);
#ifdef CONFIG_PM_WAKELOCKS
    hx9031a_wake_lock = wakeup_source_register(&client->dev, "hx9031a_wakelock");
#else
    wake_lock_init(&hx9031a_wake_lock, WAKE_LOCK_SUSPEND, "hx9031a_wakelock");
#endif

    ret = hx9031a_input_init_abs(client);
    if(0 != ret) {
        PRINT_ERR("hx9031a_input_init_abs failed\n");
        goto failed_input_init;
    }

    ret = class_register(&hx9031a_class);//debug fs path:/sys/class/hx9031a/*
    if (ret < 0) {
        PRINT_ERR("class_register failed\n");
        goto failed_class_register;
    }
    
#ifdef HX9031_ENABLE_IRQ
    ret = request_threaded_irq(hx9031a_pdata.irq, NULL, hx9031a_irq_handler,
                               IRQF_TRIGGER_FALLING | IRQF_ONESHOT | IRQF_NO_SUSPEND,
                               hx9031a_pdata.pdev->driver->name, (&hx9031a_pdata));
    if(ret < 0) {
        PRINT_ERR("request_irq failed irq=%d ret=%d\n", hx9031a_pdata.irq, ret);
        goto failed_request_irq;
    }
    //enable_irq(hx9031a_pdata.irq);
    hx9031a_irq_en_flag = 1;//irq is enabled after request by default
#endif

#if HX9031A_TEST_CHS_EN //enable channels for test
    PRINT_INF("enable all chs for test\n");
    for(ii = 0; ii < HX9031A_CH_NUM; ii++) {
        if ((hx9031a_pdata.channel_used_flag >> ii) & 0x1) {
            hx9031a_ch_en_hal(ii, 1);
        }
    }
#endif

    PRINT_INF("probe success\n");
    return 0;

#ifdef HX9031_ENABLE_IRQ
failed_request_irq:
    class_unregister(&hx9031a_class);
#endif

failed_class_register:
    hx9031a_input_deinit_abs(client);
failed_input_init:
#ifdef CONFIG_PM_WAKELOCKS
    wakeup_source_unregister(hx9031a_wake_lock);
#else
    wake_lock_destroy(&hx9031a_wake_lock);
#endif
    cancel_delayed_work_sync(&(hx9031a_pdata.polling_work));
failed_id_check:
    hx9031a_power_on(0);
    hx9031a_gpio_deinit();
failed_gpio_init:
failed_parse_dt:
failed_i2c_check_functionality:
    PRINT_ERR("probe failed\n");
    return ret;
}

static int hx9031a_remove(struct i2c_client *client)
{
    ENTER;
    free_irq(hx9031a_pdata.irq, &hx9031a_pdata);
    class_unregister(&hx9031a_class);
    hx9031a_input_deinit_abs(client);
#ifdef CONFIG_PM_WAKELOCKS
    wakeup_source_unregister(hx9031a_wake_lock);
#else
    wake_lock_destroy(&hx9031a_wake_lock);
#endif
    cancel_delayed_work_sync(&(hx9031a_pdata.polling_work));
    hx9031a_power_on(0);
    hx9031a_gpio_deinit();
    return 0;
}

static int hx9031a_suspend(struct device *dev)
{
    ENTER;
    hx9031a_irq_from_suspend_flag = 1;
    return 0;
}

static int hx9031a_resume(struct device *dev)
{
    ENTER;
    hx9031a_irq_from_suspend_flag = 0;
    return 0;
}

static struct i2c_device_id hx9031a_i2c_id_table[] = {
    { HX9031A_DRIVER_NAME, 0 },
    { }
};

MODULE_DEVICE_TABLE(i2c, hx9031a_i2c_id_table);
#ifdef CONFIG_OF
static struct of_device_id hx9031a_of_match_table[] = {
#if HX9031A_TEST_ON_MTK_DEMO_XY6761
    {.compatible = "mediatek,sar_hx9031a"},
#else
    {.compatible = "tyhx,hx9031a"},
#endif
    { },
};
#else
#define hx9031a_of_match_table NULL
#endif

static const struct dev_pm_ops hx9031a_pm_ops = {
    .suspend = hx9031a_suspend,
    .resume = hx9031a_resume,
};

static struct i2c_driver hx9031a_i2c_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name = HX9031A_DRIVER_NAME,
        .of_match_table = hx9031a_of_match_table,
        .pm = &hx9031a_pm_ops,
    },
    .id_table = hx9031a_i2c_id_table,
    .probe = hx9031a_probe,
    .remove = hx9031a_remove,
};

static int __init hx9031a_module_init(void)
{
    ENTER;
    PRINT_INF("driver version:%s\n", HX9031A_DRIVER_VER);
    return i2c_add_driver(&hx9031a_i2c_driver);
}

static void __exit hx9031a_module_exit(void)
{
    ENTER;
    i2c_del_driver(&hx9031a_i2c_driver);
}

module_init(hx9031a_module_init);
module_exit(hx9031a_module_exit);

MODULE_AUTHOR("Yasin Lee <yasin.lee.x@gmail.com><yasin.lee@tianyihexin.com>");
MODULE_DESCRIPTION("Driver for NanJingTianYiHeXin HX9031A & HX9023E Cap Sensor");
MODULE_ALIAS("sar driver");
MODULE_LICENSE("GPL");
