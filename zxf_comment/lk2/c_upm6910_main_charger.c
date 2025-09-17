/*
这是一个针对 UPM6910 充电芯片 的驱动模块，主要完成以下任务：
检测芯片是否存在
初始化芯片寄存器
设置充电电流（ICHG）、输入限流（AICR）、最小系统电压等参数
获取 VBUS 是否接入、当前充电电流等信息
控制充电启停、看门狗复位、MIVR（最小输入电压调节）
支持多充电器架构（MULTI_EXT_MAIN_CHARGER 宏定义）
⚠️ 运行环境：Bootloader（LK）阶段，不依赖完整操作系统。
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

#include "upm6910_main_reg.h"

#define UPM6910_MAIN_LK_DRV_VERSION "1.0.0_MTK"

enum upm6910_main_part_no {
    UPM6910_MAIN = 0x02,
};

#ifdef EXT_CHARGER_IIC_NUM
#define    I2C_NUM        EXT_CHARGER_IIC_NUM
#else
#define    I2C_NUM        5
#endif
#define    DEV_ADDR    (0x6B)

#define I2C_OK             0x0000

static int upm6910_main_read_byte(u8 reg, u8 *returnData)
{
    u8 write_data[2] = { 0 };
    int ret_code = I2C_OK;

    write_data[0] = reg;
    ret_code = mtk_i2c_write_read((u8)(I2C_NUM & 0xFF), (u8)DEV_ADDR, (u8 *)write_data, 1, 1, NULL);
    *returnData = write_data[0];
    if (ret_code < 0)
        printf("%s: i2c_read fail: ret_code: %d, addr 0x%x\n", __func__, ret_code, reg);

    return ret_code;
}

static int upm6910_main_write_byte(u8 addr, u8 value)
{
    int ret_code = I2C_OK;
    u8 write_data[2];
    u32 len;

    write_data[0] = addr;
    write_data[1] = value;
    len = 2;

    ret_code = mtk_i2c_write((u8)(I2C_NUM & 0xFF), (u8)DEV_ADDR, write_data, len, NULL);
    if(ret_code)
        printf("%s: i2c_write: ret_code: %d, addr 0x%x, value 0x%x\n", __func__, ret_code, addr, value);

    return ret_code;
}

static int upm6910_main_read_bits(u8 reg, u8 *data, u8 mask, u8 shift)
{
    int ret = 0;
    u8 val = 0;

    ret = upm6910_main_read_byte(reg, &val);
    if (ret) {
        return ret;
    }

    val &= (mask << shift);
    *data = (val >> shift);

    return ret;
}

static int upm6910_main_update_bits(u16 cmd, u8 data, u8 mask, u8 shift)
{
    int ret = 0;
    u8 val = 0;

    ret = upm6910_main_read_byte(cmd, &val);
    if (ret != I2C_OK)
        return ret;
    val &= ~(mask << shift);
    val |= (data << shift);
    ret = upm6910_main_write_byte(cmd, val);
    return ret;
}

static void upm6910_main_set_en_hiz(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_00),
                       (unsigned char) (val),
                       (unsigned char) (REG00_ENHIZ_MASK),
                       (unsigned char) (REG00_ENHIZ_SHIFT)
                      );
}

static void upm6910_main_set_aicr(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_00),
                       (unsigned char) (val),
                       (unsigned char) (REG00_IINLIM_MASK),
                       (unsigned char) (REG00_IINLIM_SHIFT)
                      );
}

static void upm6910_main_set_pfm(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_01),
                       (unsigned char) (val),
                       (unsigned char) (REG01_PFM_DIS_MASK),
                       (unsigned char) (REG01_PFM_DIS_SHIFT)
                      );
}

static void upm6910_main_set_wdt_rst(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_01),
                       (unsigned char) (val),
                       (unsigned char) (REG01_WDT_RESET_MASK),
                       (unsigned char) (REG01_WDT_RESET_SHIFT)
                      );
}

static void upm6910_main_set_chg_config(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_01),
                       (unsigned char) (val),
                       (unsigned char) (REG01_CHG_CONFIG_MASK),
                       (unsigned char) (REG01_CHG_CONFIG_SHIFT)
                      );
}

static void upm6910_main_set_sys_min(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_01),
                       (unsigned char) (val),
                       (unsigned char) (REG01_SYS_MINV_MASK),
                       (unsigned char) (REG01_SYS_MINV_SHIFT)
                      );
}

static void upm6910_main_set_batlowv(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_01),
                       (unsigned char) (val),
                       (unsigned char) (REG01_MIN_VBAT_SEL_MASK),
                       (unsigned char) (REG01_MIN_VBAT_SEL_SHIFT)
                      );
}

static void upm6910_main_set_rdson(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_02),
                       (unsigned char) (val),
                       (unsigned char) (REG02_Q1_FULLON_MASK),
                       (unsigned char) (REG02_Q1_FULLON_SHIFT)
                      );
}

static void upm6910_main_set_ichg(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_02),
                        (unsigned char) (val),
                        (unsigned char) (REG02_ICHG_MASK),
                        (unsigned char) (REG02_ICHG_SHIFT)
                        );
}

static void upm6910_main_set_iprechg(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_03),
                       (unsigned char) (val),
                       (unsigned char) (REG03_IPRECHG_MASK),
                       (unsigned char) (REG03_IPRECHG_SHIFT)
                      );
}

static void upm6910_main_set_iterm(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_03),
                       (unsigned char) (val),
                       (unsigned char) (REG03_ITERM_MASK),
                       (unsigned char) (REG03_ITERM_SHIFT)
                      );
}

static void upm6910_main_set_vreg(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_04),
                       (unsigned char) (val),
                       (unsigned char) (REG04_VREG_MASK),
                       (unsigned char) (REG04_VREG_SHIFT)
                      );
}

static void upm6910_main_set_vrechg(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_04),
                       (unsigned char) (val),
                       (unsigned char) (REG04_VRECHG_MASK),
                       (unsigned char) (REG04_VRECHG_SHIFT)
                      );
}

static void upm6910_main_set_en_term(bool val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_05),
                       (unsigned char) (val),
                       (unsigned char) (REG05_EN_TERM_MASK),
                       (unsigned char) (REG05_EN_TERM_SHIFT)
                      );
}

static void upm6910_main_set_watchdog(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_05),
                       (unsigned char) (val),
                       (unsigned char) (REG05_WDT_MASK),
                       (unsigned char) (REG05_WDT_SHIFT)
                      );
}

static void upm6910_main_set_en_timer(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_05),
                       (unsigned char) (val),
                       (unsigned char) (REG05_EN_TIMER_MASK),
                       (unsigned char) (REG05_EN_TIMER_SHIFT)
                      );
}

static int upm6910_main_get_ichg(u32 *ichg)
{
    int ret = 0;
    u8 val = 0;
    int curr;

        
    ret = upm6910_main_read_byte(UPM6910_MAIN_REG_02, &val);
    if (!ret) {
        curr = ((u32)(val & REG02_ICHG_MASK ) >> REG02_ICHG_SHIFT) * REG02_ICHG_LSB;
        curr +=  REG02_ICHG_BASE;
        
        *ichg = curr * 1000; /*to uA*/
    }
    
    return ret;
}

static void upm6910_main_set_mivr(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_06),
                       (unsigned char) (val),
                       (unsigned char) (REG06_VINDPM_MASK),
                       (unsigned char) (REG06_VINDPM_SHIFT)
                      );
}

static void upm6910_main_set_ovp(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_06),
                       (unsigned char) (val),
                       (unsigned char) (REG06_OVP_MASK),
                       (unsigned char) (REG06_OVP_SHIFT)
                      );
}

static void upm6910_main_set_jeita_vset(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_07),
                       (unsigned char) (val),
                       (unsigned char) (REG07_JEITA_VSET_MASK),
                       (unsigned char) (REG07_JEITA_VSET_SHIFT)
                      );
}

static unsigned char upm6910_main_get_pg_stat(void)
{
    unsigned char val = 0;

    upm6910_main_read_bits((unsigned char) (UPM6910_MAIN_REG_08),
                     (&val),
                     (unsigned char) (REG08_PG_STAT_MASK),
                     (unsigned char) (REG08_PG_STAT_SHIFT)
                    );
    return val;
}


static void upm6910_main_set_int_mask(unsigned char val)
{
    upm6910_main_update_bits((unsigned char) (UPM6910_MAIN_REG_0A),
                       (unsigned char) (val),
                       (unsigned char) (REG0A_INT_MASK_MASK),
                       (unsigned char) (REG0A_INT_MASK_SHIFT)
                      );
}

static int upm6910_main_detect_device(void)
{
    int ret;
    u8 data;

    ret = upm6910_main_read_byte(UPM6910_MAIN_REG_0B, &data);//0x0B
    if (ret) {
        return ret;
    }

    if ((data & 0xFF) == 0x10){
        printf("[upm6910_main] exist, Reg[0x0B]=0x%x\n", data);
        ret = 0;
    }

    return ret;
}
#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_enable_charging(bool enable)
#else
int mtk_charger_enable_charging(bool enable)
#endif
{
    if (enable) {
        upm6910_main_set_en_hiz(0x0);
        upm6910_main_set_chg_config(0x01);
        upm6910_main_set_ovp(1);//6.5V
    } else {
        upm6910_main_set_chg_config(0);
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
    }else{
        reg_val = ichg / REG02_ICHG_LSB;//step: 5, base:0
    }
set_ichg:
    printf("%s ichg=%d, reg_val:0x%x\n", __func__, ichg, reg_val);
    upm6910_main_set_ichg(reg_val);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_get_ichg(unsigned int *ichg)
#else
int mtk_charger_get_ichg(unsigned int *ichg)
#endif
{
    u8 reg_val;
    upm6910_main_read_bits((unsigned char) (UPM6910_MAIN_REG_02),
                       &reg_val,
                       (unsigned char) (REG02_ICHG_MASK),
                       (unsigned char) (REG02_ICHG_SHIFT)
                      );
    if(reg_val == 0){
        *ichg = 0;
        goto get_ichg;
    }

    *ichg = (u32)(reg_val) * REG02_ICHG_LSB;


get_ichg:
    printf("%s ichg=%d, reg_val:0x%x\n", __func__, *ichg, reg_val);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_get_aicr(unsigned int *aicr)
#else
int mtk_charger_get_aicr(unsigned int *aicr)
#endif
{
    u8 reg_val;
    upm6910_main_read_bits((unsigned char) (UPM6910_MAIN_REG_00),
                   &reg_val,
                   (unsigned char) (REG00_IINLIM_MASK),
                   (unsigned char) (REG00_IINLIM_SHIFT)
                  );

    *aicr = reg_val * REG00_IINLIM_BASE + 100;    //step=100mA

    printf("%s get_aicr mA = %d\n",__func__, *aicr);
    
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_set_aicr(unsigned int aicr)
#else
int mtk_charger_set_aicr(unsigned int aicr)
#endif
{
    u8 regval;

    printf("%s aicr=%d\n", __func__, aicr);

    regval = (aicr - 100) / REG00_IINLIM_BASE;
    upm6910_main_set_aicr(regval);

    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_get_vbus(unsigned int *vbus)
#else
int mtk_charger_get_vbus(unsigned int *vbus)
#endif
{
    u8 pg_stat = 0;
    pg_stat = upm6910_main_get_pg_stat();
    if (pg_stat == 0)
    {
        *vbus = 0;
        // printf("%s,vbus = %d\n", __func__, *vbus);
        return 0;
    }
    *vbus = 5000000;
    // printf("%s,vbus = %d\n", __func__, *vbus);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_set_mivr(unsigned int mivr)
#else
int mtk_charger_set_mivr(unsigned int mivr)
#endif
{
    u8 val;
    u32 vindpm_base = 3900;

    if (mivr < vindpm_base)
        mivr = vindpm_base;

    val = (u8)(mivr - vindpm_base) / 100;
    upm6910_main_set_mivr(val);

    printf("%s vindpm_base = %d, val:%d, mV:%d\n",__func__, vindpm_base, val, mivr);

    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_enable_power_path(bool enable)
#else
int mtk_charger_enable_power_path(bool enable)
#endif
{
    printf("%s en=%d\n", __func__, enable);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_enable_wdt(bool en)
#else
int mtk_charger_enable_wdt(bool en)
#endif
{
    printf("%s en=%d\n", __func__, en);
    if(en)
        upm6910_main_set_watchdog(0x3);//160s
    else
        upm6910_main_set_watchdog(0x0);
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_reset_wdt(void)
#else
int mtk_charger_reset_wdt(void)
#endif
{
    upm6910_main_set_wdt_rst(1);
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

    for (addr = 0x0; addr < UMP6910_MAIN_REG_NUM; addr++) {
        ret = upm6910_main_read_byte(addr, &val);
        if (ret == 0)
            printf("Reg[%.2x] = 0x%.2x\n", addr, val);
    }

    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_get_pg_state(void)
#else
int mtk_charger_get_pg_state(void)
#endif
{
    int ret = 0;
    printf("%s\n", __func__);
    ret = upm6910_main_get_pg_stat();
    return ret;
}

static int upm6910_main_init_device(void)
{
    //upm6910_main_set_en_jeita(0);   /* Disable jeita */
    upm6910_main_set_en_hiz(0x0);
    upm6910_main_set_mivr(0x7);      /* VIN DPM check 4.6V */
    upm6910_main_set_wdt_rst(0x1);   /* Kick watchdog */
    upm6910_main_set_sys_min(0x5);   /* Minimum system voltage 3.5V */
    upm6910_main_set_iprechg(0xC);   /* Precharge current 720mA */
    upm6910_main_set_iterm(0xC);     /* Termination current 160mA (set 200ma actual 180ma */
    upm6910_main_set_vreg(0xf);      /* VREG 4.35V */
    upm6910_main_set_pfm(0x1);       //disable pfm
    upm6910_main_set_rdson(0x0);     /*close rdson*/
    upm6910_main_set_batlowv(0x1);   /* BATLOWV 3.0V */
    upm6910_main_set_vrechg(0x0);    /* VRECHG 0.1V (4.108V) */
    upm6910_main_set_en_term(0x1);   /* Enable termination */
    upm6910_main_set_watchdog(0x0);  /* disable WDT 160s */
    upm6910_main_set_en_timer(0x0);  /* Enable charge timer */
    upm6910_main_set_int_mask(0x0);  /* Disable fault interrupt */
    upm6910_main_set_jeita_vset(1);  /*Set charge voltage to V REG*/
    upm6910_main_set_ovp(1);//6.5V

#if (MULTI_EXT_MAIN_CHARGER)
    mtk_charger_sec_set_mivr(4500);
    mtk_charger_sec_set_ichg(500);//800mA
    mtk_charger_sec_set_aicr(500);//800mA
#else
    mtk_charger_set_mivr(4500);
    mtk_charger_set_ichg(500);//800mA
    mtk_charger_set_aicr(500);//800mA
#endif

    printf("upm6910_main_init: hw_init done!\n");
    return 0;
}

#if (MULTI_EXT_MAIN_CHARGER)
int mtk_charger_sec_init(void)
#else
int mtk_charger_init(void)
#endif
{
    int ret = 0;

    ret = upm6910_main_detect_device();
    if (ret < 0)
        return ret;

    ret = upm6910_main_init_device();
    return 0;
}
