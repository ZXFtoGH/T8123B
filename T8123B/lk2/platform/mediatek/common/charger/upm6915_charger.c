/*
 * copyright (c) 2021 mediatek inc.
 *
 * use of this source code is governed by a mit-style
 * license that can be found in the license file or at
 * https://opensource.org/licenses/mit
 */

#include <bits.h>
#include <debug.h>
#include <err.h>
#include <errno.h>
#include <malloc.h>
#include <platform/timer.h>
#include <regmap.h>
#include <sys/types.h>
#include <trace.h>
#include <mt_i2c.h>
#include <platform.h>

#include "mtk_charger_intf.h"
#include "upm6915_charger.h"

#define I2C_OK             0x0000

#define LOCAL_TRACE 0

/* CON2 */
#define CON2_OREG_MASK      0x3F
#define CON2_OREG_SHIFT     2
#define CON2_OTG_PL_MASK    0x01
#define CON2_OTG_PL_SHIFT   1
#define CON2_OTG_EN_MASK    0x01
#define CON2_OTG_EN_SHIFT   0

unsigned char upm6915_reg_list[UPM6915_REG_NUM] = { 0 };

static int upm6915_write_byte(u8 cmd, u8 data)
{
    int ret = 0;
    u8 write_buf[2] = {cmd, data};

    ret = mtk_i2c_write(UPM6915_I2C_ID, UPM6915_SLAVE_ADDR,
                        write_buf, 2, NULL);
    if (ret != 0)
        LTRACEF("%s: I2CW[0x%02X] = 0x%02X failed, code = %d\n",
                __func__, cmd, data, ret);
    else
        LTRACEF("%s: I2CW[0x%02X] = 0x%02X\n",
                __func__, cmd, data);
    return ret;
}

static int upm6915_read_byte(u8 cmd, u8 *data)
{
    int ret = 0;
    u8 ret_data = cmd;

    ret = mtk_i2c_write_read(UPM6915_I2C_ID, UPM6915_SLAVE_ADDR,
                            &ret_data, 1, 1, NULL);
    if (ret != 0)
        LTRACEF("%s: I2CR[0x%02X] failed, code = %d\n",
                __func__, cmd, ret);
    else {
        LTRACEF("%s: I2CR[0x%02X] = 0x%02X\n",
                __func__, cmd, ret_data);
        *data = ret_data;
    }
    return ret;
}

static int upm6915_read_interface(u8 reg, u8 *data, u8 mask, u8 shift)
{
    int ret = 0;
    u8 val = 0;

    ret = upm6915_read_byte(reg, &val);
    if (ret) {
        return ret;
    }

    val &= (mask << shift);
    *data = (val >> shift);

    return ret;
}

static int upm6915_config_interface(u16 cmd, u8 data, u8 mask, u8 shift)
{
    int ret = 0;
    u8 val = 0;

    ret = upm6915_read_byte(cmd, &val);
    if (ret != I2C_OK)
        return ret;
    val &= ~(mask << shift);
    val |= (data << shift);
    ret = upm6915_write_byte(cmd, val);
    return ret;
}

/* write one register directly */
static unsigned int upm6915_reg_config_interface(unsigned char reg_num, unsigned char val)
{
    unsigned char upm6915_reg = val;

    return upm6915_write_byte(reg_num, upm6915_reg);
}

static void upm6915_set_tmr_rst(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON0),
                (unsigned char)(val),
                (unsigned char)(CON0_TMR_RST_MASK),
                (unsigned char)(CON0_TMR_RST_SHIFT)
                );
}

static unsigned int upm6915_get_otg_status(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON0),
                (unsigned char *)(&val),
                (unsigned char)(CON0_OTG_MASK),
                (unsigned char)(CON0_OTG_SHIFT)
                );
    return val;
}

static void upm6915_set_en_stat(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON0),
                (unsigned char)(val),
                (unsigned char)(CON0_EN_STAT_MASK),
                (unsigned char)(CON0_EN_STAT_SHIFT)
                );
}

static unsigned int upm6915_get_chip_status(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON0),
                (unsigned char *)(&val),
                (unsigned char)(CON0_STAT_MASK),
                (unsigned char)(CON0_STAT_SHIFT)
                );
    return val;
}

static unsigned int upm6915_get_boost_status(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON0),
                (unsigned char *)(&val),
                (unsigned char)(CON0_BOOST_MASK),
                (unsigned char)(CON0_BOOST_SHIFT)
                );
    return val;

}

static unsigned int upm6915_get_fault_status(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON0),
                (unsigned char *)(&val),
                (unsigned char)(CON0_FAULT_MASK),
                (unsigned char)(CON0_FAULT_SHIFT)
                );
    return val;
}

static void upm6915_set_input_charging_current(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON1),
                (unsigned char)(val),
                (unsigned char)(CON1_LIN_LIMIT_MASK),
                (unsigned char)(CON1_LIN_LIMIT_SHIFT)
                );
}

static unsigned int upm6915_get_input_charging_current(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON1),
                (unsigned char *)(&val),
                (unsigned char)(CON1_LIN_LIMIT_MASK),
                (unsigned char)(CON1_LIN_LIMIT_SHIFT)
                );

    return val;
}

static void upm6915_set_v_low(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON1),
                (unsigned char)(val),
                (unsigned char)(CON1_LOW_V_MASK),
                (unsigned char)(CON1_LOW_V_SHIFT)
                );
}

static void upm6915_set_te(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON1),
                (unsigned char)(val),
                (unsigned char)(CON1_TE_MASK),
                (unsigned char)(CON1_TE_SHIFT)
                );
}

static void upm6915_set_ce(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON1),
                (unsigned char)(val),
                (unsigned char)(CON1_CE_MASK),
                (unsigned char)(CON1_CE_SHIFT)
                );
}

static void upm6915_set_hz_mode(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON1),
                (unsigned char)(val),
                (unsigned char)(CON1_HZ_MODE_MASK),
                (unsigned char)(CON1_HZ_MODE_SHIFT)
                );
}

static void upm6915_set_opa_mode(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON1),
                (unsigned char)(val),
                (unsigned char)(CON1_OPA_MODE_MASK),
                (unsigned char)(CON1_OPA_MODE_SHIFT)
                );
}

static void upm6915_set_oreg(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON2),
                (unsigned char)(val),
                (unsigned char)(CON2_OREG_MASK),
                (unsigned char)(CON2_OREG_SHIFT)
                );
}
static void upm6915_set_otg_pl(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON2),
                (unsigned char)(val),
                (unsigned char)(CON2_OTG_PL_MASK),
                (unsigned char)(CON2_OTG_PL_SHIFT)
                );
}
static void upm6915_set_otg_en(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON2),
                (unsigned char)(val),
                (unsigned char)(CON2_OTG_EN_MASK),
                (unsigned char)(CON2_OTG_EN_SHIFT)
                );
}

static unsigned int upm6915_get_vender_code(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON3),
                (unsigned char *)(&val),
                (unsigned char)(CON3_VENDER_CODE_MASK),
                (unsigned char)(CON3_VENDER_CODE_SHIFT)
                );
    return val;
}
static unsigned int upm6915_get_pn(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON3),
                (unsigned char *)(&val),
                (unsigned char)(CON3_PIN_MASK),
                (unsigned char)(CON3_PIN_SHIFT)
                );
    return val;
}

static unsigned int upm6915_get_revision(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON3),
                (unsigned char *)(&val),
                (unsigned char)(CON3_REVISION_MASK),
                (unsigned char)(CON3_REVISION_SHIFT)
                );
    return val;
}

static void upm6915_set_reset(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON4),
                (unsigned char)(val),
                (unsigned char)(CON4_RESET_MASK),
                (unsigned char)(CON4_RESET_SHIFT)
                );
}

static void upm6915_set_iocharge(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON4),
                (unsigned char)(val),
                (unsigned char)(CON4_I_CHR_MASK),
                (unsigned char)(CON4_I_CHR_SHIFT)
                );
}

static void upm6915_set_iterm(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON4),
                (unsigned char)(val),
                (unsigned char)(CON4_I_TERM_MASK),
                (unsigned char)(CON4_I_TERM_SHIFT)
                );
}

static void upm6915_set_dis_vreg(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON5),
                (unsigned char)(val),
                (unsigned char)(CON5_DIS_VREG_MASK),
                (unsigned char)(CON5_DIS_VREG_SHIFT)
                );
}

static void upm6915_set_io_level(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON5),
                (unsigned char)(val),
                (unsigned char)(CON5_IO_LEVEL_MASK),
                (unsigned char)(CON5_IO_LEVEL_SHIFT)
                );
}

static unsigned int upm6915_get_sp_status(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON5),
                (unsigned char *)(&val),
                (unsigned char)(CON5_SP_STATUS_MASK),
                (unsigned char)(CON5_SP_STATUS_SHIFT)
                );
    return val;
}

static unsigned int upm6915_get_en_level(void)
{
    unsigned char val = 0;

    upm6915_read_interface((unsigned char)(UPM6915_CON5),
                (unsigned char *)(&val),
                (unsigned char)(CON5_EN_LEVEL_MASK),
                (unsigned char)(CON5_EN_LEVEL_SHIFT)
                );
    return val;
}

static void upm6915_set_vsp(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON5),
                (unsigned char)(val),
                (unsigned char)(CON5_VSP_MASK),
                (unsigned char)(CON5_VSP_SHIFT)
                );
}

static void upm6915_set_i_safe(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON6),
                (unsigned char)(val),
                (unsigned char)(CON6_ISAFE_MASK),
                (unsigned char)(CON6_ISAFE_SHIFT)
                );
}

static void upm6915_set_v_safe(unsigned int val)
{
    upm6915_config_interface((unsigned char)(UPM6915_CON6),
                (unsigned char)(val),
                (unsigned char)(CON6_VSAFE_MASK),
                (unsigned char)(CON6_VSAFE_SHIFT)
                );
}

static void upm6915_dump_register(void)
{
    int i;

    for (i = 0;i < UPM6915_REG_NUM;i++) {
        upm6915_read_byte(i, &upm6915_reg_list[i]);
        printf("upm6915_dump_register [0x%x]=0x%x \n", i, upm6915_reg_list[i]);
    }
}

static int upm6915_enable_charging(bool enable)
{
    unsigned int status = 0;
    if (enable) {
        upm6915_reg_config_interface(0x00, 0xd0);
        upm6915_reg_config_interface(0x01, 0xf8);
        upm6915_set_ce(1);//upm6915作副充，lk阶段不打开
        upm6915_set_hz_mode(0);
        upm6915_set_opa_mode(0);
        printf(">>>%s: enable= 1 !\n",__func__);
    } else {
        upm6915_set_ce(1);
        printf(">>>%s: enable= 0 !\n",__func__);
    }

    return status;
}

static int upm6915_reset_watch_dog_timer(void)
{
    upm6915_set_tmr_rst(1);
    return 0;
}

void upm6915_charging_enable(void)
{
    upm6915_reg_config_interface(0x00,0xC0);
    upm6915_reg_config_interface(0x01,0xf8);
    //upm6915_reg_config_interface(0x02,0x8e);
    upm6915_reg_config_interface(0x05,0x04);
    upm6915_reg_config_interface(0x04,0x79); //146mA
}

static void upm6915_hw_init(void)
{
#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
    dprintf(CRITICAL, "[upm6915_hw_init] (0x06,0x77)\n");
    upm6915_reg_config_interface(0x06,0x77); // set ISAFE and HW CV point (4.34)
    upm6915_reg_config_interface(0x02,0xaa); // 4.34
#else
    dprintf(CRITICAL, "[upm6915_hw_init] (0x06,0x70)\n");
    upm6915_reg_config_interface(0x06,0x70); // set ISAFE
    upm6915_reg_config_interface(0x02,0x8e); // 4.2
#endif
    upm6915_charging_enable();
}

int upm6915_probe(void)
{
    printf("%s lk2\n", __func__);
    /* Check primary charger */
    upm6915_set_reset(1);
    upm6915_hw_init();
    upm6915_enable_charging(1);/*预充不需要打开副充*/

    upm6915_dump_register();

    return 0;
}
