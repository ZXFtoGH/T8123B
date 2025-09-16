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
#include "upm6920_reg.h"
#include "upm6920.h"


#ifdef EXT_CHARGER_IIC_NUM
#define I2C_NUM EXT_CHARGER_IIC_NUM
#else
#define I2C_NUM 5
#endif
#define DEV_ADDR (0x6A)

#define I2C_OK 0x0000


static int upm6920_read_byte(u8 reg, u8 *returnData);
static int upm6920_write_byte(u8 addr, u8 value);
static int upm6920_read_bits(u8 reg, u8 *data, u8 mask, u8 shift);
static int upm6920_update_bits(u16 cmd, u8 data, u8 mask, u8 shift);
static void upm6920_set_en_hiz(unsigned char val);
static void upm6920_set_aicr(unsigned char val);
static void upm6920_set_chg_config(unsigned char val);
static void upm6920_set_ichg(unsigned char val);
static void upm6920_set_iprechg(unsigned char val);
static void upm6920_set_vreg(unsigned char val);
static void upm6920_set_en_term(bool val);
static void upm6920_set_mivr(unsigned char val);
static unsigned char upm6920_get_pg_stat(void);
static int upm6920_detect_device(void);
static int upm6920_init_device(void);


static int upm6920_read_byte(u8 reg, u8 *returnData)
{
    u8 write_data[2] = { 0 };
    int ret_code = I2C_OK;

    write_data[0] = reg;
    ret_code = mtk_i2c_write_read((u8)(I2C_NUM & 0xFF), (u8)DEV_ADDR, 
                (u8 *)write_data, 1, 1, NULL);
    *returnData = write_data[0];
    if (ret_code < 0)
        printf("%s: i2c_read fail: ret_code: %d, addr 0x%x\n", 
              __func__, ret_code, reg);

    return ret_code;
}

static int upm6920_write_byte(u8 addr, u8 value)
{
    int ret_code = I2C_OK;
    u8 write_data[2];
    u32 len;

    write_data[0] = addr;
    write_data[1] = value;
    len = 2;

    ret_code = mtk_i2c_write((u8)(I2C_NUM & 0xFF), (u8)DEV_ADDR, 
                            write_data, len, NULL);
    if(ret_code)
        printf("%s: i2c_write: ret_code: %d, addr 0x%x, value 0x%x\n", 
              __func__, ret_code, addr, value);

    return ret_code;
}


static int upm6920_read_bits(u8 reg, u8 *data, u8 mask, u8 shift)
{
    int ret = 0;
    u8 val = 0;

    ret = upm6920_read_byte(reg, &val);
    if (ret) {
        return ret;
    }

    val &= (mask << shift);
    *data = (val >> shift);

    return ret;
}

static int upm6920_update_bits(u16 cmd, u8 data, u8 mask, u8 shift)
{
    int ret = 0;
    u8 val = 0;

    ret = upm6920_read_byte(cmd, &val);
    if (ret != I2C_OK)
        return ret;
    val &= ~(mask << shift);
    val |= (data << shift);
    ret = upm6920_write_byte(cmd, val);
    return ret;
}


static void upm6920_set_en_hiz(unsigned char val)
{
    upm6920_update_bits(UPM6920_REG_00, val, 
                       UPM6920_ENHIZ_MASK, UPM6920_ENHIZ_SHIFT);
}

static void upm6920_set_aicr(unsigned char val)
{
    upm6920_update_bits(UPM6920_REG_00, val,
                       UPM6920_IINLIM_MASK, UPM6920_IINLIM_SHIFT);
}

static void upm6920_set_chg_config(unsigned char val)
{
    upm6920_update_bits(UPM6920_REG_03, val,
                       UPM6920_CHG_CONFIG_MASK, UPM6920_CHG_CONFIG_SHIFT);
}

static void upm6920_set_ichg(unsigned char val)
{
    upm6920_update_bits(UPM6920_REG_04, val,
                       UPM6920_ICHG_MASK, UPM6920_ICHG_SHIFT);
}

static void upm6920_set_iprechg(unsigned char val)
{
    upm6920_update_bits(UPM6920_REG_05, val,
                       UPM6920_IPRECHG_MASK, UPM6920_IPRECHG_SHIFT);
}

static void upm6920_set_vreg(unsigned char val)
{
    upm6920_update_bits(UPM6920_REG_06, val,
                       UPM6920_VREG_MASK, UPM6920_VREG_SHIFT);
}

static void upm6920_set_en_term(bool val)
{
    upm6920_update_bits(UPM6920_REG_07, val,
                       UPM6920_EN_TERM_MASK, UPM6920_EN_TERM_SHIFT);
}

static void upm6920_set_mivr(unsigned char val)
{
    upm6920_update_bits(UPM6920_REG_0D, val,
                       UPM6920_VINDPM_MASK, UPM6920_VINDPM_SHIFT);
}

static unsigned char upm6920_get_pg_stat(void)
{
    unsigned char val = 0;
    upm6920_read_bits(UPM6920_REG_11, &val,
                     UPM6920_VBUS_GD_MASK, 0);
    return val;
}


static int upm6920_detect_device(void)
{
    int ret;
    u8 data;
    printf("upm6920_detect_device\n");
    ret = upm6920_read_byte(UPM6920_REG_14, &data);
    if (ret) {
        return ret;
    }

    if ((data & UPM6920_PN_MASK) == 0x03){
        printf("[upm6920] exist, Reg[0x14]=0x%x\n", data);
        ret = 0;
    }

    return ret;
}

static int upm6920_init_device(void)
{
    int ret;
    upm6920_set_en_hiz(0x0);
    upm6920_set_mivr(0x7);      /* VIN DPM check 4.6V */
    upm6920_set_iprechg(0x8);   /* Precharge current 540mA */
    upm6920_set_vreg(0xf);      /* VREG 4.35V */
    upm6920_set_en_term(0x1);   /* Enable termination */

#if (MULTI_EXT_MAIN_CHARGER)
    mtk_charger_sec_set_mivr(4500);
    mtk_charger_sec_set_ichg(500);
    mtk_charger_sec_set_aicr(500);
#else
    mtk_charger_set_mivr(4500);
    mtk_charger_set_ichg(500);
    mtk_charger_set_aicr(500);
#endif
    ret = mtk_charger_dump_register();
    printf("upm6920_init: hw_init done!\n");
    return 0;
}


#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_enable_charging(bool enable)
#else
int mtk_charger_enable_charging(bool enable)
#endif
{
    if (enable) {
        upm6920_set_en_hiz(0x0);
        upm6920_set_chg_config(0x01);
    } else {
        upm6920_set_chg_config(0);
    }
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_set_ichg(unsigned int ichg)
#else
int mtk_charger_set_ichg(unsigned int ichg)
#endif
{
    u8 reg_val = 0;
    
    if(ichg <= 0){
        reg_val = 0;
        goto set_ichg;
    }

    if(ichg >= 3000) {
        reg_val = 0x32;
    } else {
        reg_val = (ichg - UPM6920_ICHG_BASE) / UPM6920_ICHG_LSB;
    }
    
set_ichg:
    printf("%s ichg=%d, reg_val:0x%x\n", __func__, ichg, reg_val);
    upm6920_set_ichg(reg_val);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_get_aicr(unsigned int *aicr)
#else
int mtk_charger_get_aicr(unsigned int *aicr)
#endif
{
    u8 reg_val;
    upm6920_read_bits(UPM6920_REG_00, &reg_val,
                     UPM6920_IINLIM_MASK, UPM6920_IINLIM_SHIFT);

    *aicr = reg_val * UPM6920_IINLIM_LSB + UPM6920_IINLIM_BASE;
    printf("%s get_aicr mA = %d\n", __func__, *aicr);
    
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_set_aicr(unsigned int aicr)
#else
int mtk_charger_set_aicr(unsigned int aicr)
#endif
{
    u8 regval = (aicr - UPM6920_IINLIM_BASE) / UPM6920_IINLIM_LSB;
    printf("%s aicr=%d\n", __func__, aicr);
    upm6920_set_aicr(regval);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_get_vbus(unsigned int *vbus)
#else
int mtk_charger_get_vbus(unsigned int *vbus)
#endif
{
    u8 pg_stat = upm6920_get_pg_stat();
    *vbus = (pg_stat == 0) ? 0 : 5000000;
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_set_mivr(unsigned int mivr)
#else
int mtk_charger_set_mivr(unsigned int mivr)
#endif
{
    u8 val = (mivr < UPM6920_VINDPM_MIN) ? 
             ((UPM6920_VINDPM_MIN - UPM6920_VINDPM_BASE) / UPM6920_VINDPM_LSB) :
             ((mivr - UPM6920_VINDPM_BASE) / UPM6920_VINDPM_LSB);

    printf("%s vindpm_base = %d, val:%d, mV:%d\n", 
          __func__, UPM6920_VINDPM_BASE, val, mivr);
    upm6920_set_mivr(val);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_dump_register(void)
#else
int mtk_charger_dump_register(void)
#endif
{
    int addr;
    u8 val;
    int ret;

    for (addr = 0x0; addr <= 0x14; addr++) {
        ret = upm6920_read_byte(addr, &val);
        if (ret == 0)
            printf("upm6920 LK Reg[%.2x] = 0x%.2x\n", addr, val);
    }

    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_get_pg_state(void)
#else
int mtk_charger_get_pg_state(void)
#endif
{
    return upm6920_get_pg_stat();
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_init(void)
#else
int mtk_charger_init(void)
#endif
{
    printf("upm6920_charger_init star!\n");
    int ret = upm6920_detect_device();
    if (ret < 0)
        return ret;

    return upm6920_init_device();
}