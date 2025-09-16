/*
该驱动主要完成以下任务：
功能					 描述
✅ 设备探测				检查是否连接了正确的 UPM6910D 芯片
✅ 初始化设置			设置默认充电电流、看门狗、过压保护等
✅ BC1.2 充电检测		判断插入的是哪种类型的充电器（SDP/CDP/DCP）
✅ 参数配置				可设置输入限流、充电电压、看门狗时间等
✅ 安全保障				支持软复位、关断充电、OVP 保护
⚠️ 注意：这是运行在 Preloader 层的轻量级驱动，不是 Linux 内核中的完整 platform_driver。
*/

#include <i2c.h>               // 提供 i2c_write/read 函数
//#include <mt_drcc.h>          // 注释掉，可能用于 DRAM 控制
#include <mtk_subpmic.h>       // MTK 平台子 PMIC 相关接口
#include <pal_log.h>           // 打印日志（Preloader Application Layer）
#include <timer.h>             // 延时函数 mdelay()


#define UPM6910D_SLAVE_ADDR	(0x6B)
#define UPM6910D_PN			0x02			//通过读取 REG_0B[6:3] 获取产品编号（Part Number），确认是 UPM6910D

/* Register 00h */
#define UPM6910X_REG_00      		0x00
#define REG00_ENHIZ_MASK		    0x80
#define REG00_ENHIZ_SHIFT		    7
#define	REG00_HIZ_ENABLE			1
#define	REG00_HIZ_DISABLE			0

#define	REG00_STAT_CTRL_MASK		0x60
#define REG00_STAT_CTRL_SHIFT		5
#define	REG00_STAT_CTRL_STAT		0
#define	REG00_STAT_CTRL_ICHG		1
#define	REG00_STAT_CTRL_IINDPM		2
#define	REG00_STAT_CTRL_DISABLE		3

#define REG00_IINLIM_MASK		    0x1F
#define REG00_IINLIM_SHIFT			0
#define	REG00_IINLIM_LSB			100
#define	REG00_IINLIM_BASE			100

/* Register 01h */
#define UPM6910X_REG_01		    	0x01
#define REG01_PFM_DIS_MASK	      	0x80
#define	REG01_PFM_DIS_SHIFT			7
#define	REG01_PFM_ENABLE			0
#define	REG01_PFM_DISABLE			1

#define REG01_WDT_RESET_MASK		0x40
#define REG01_WDT_RESET_SHIFT		6
#define REG01_WDT_RESET				1

#define	REG01_OTG_CONFIG_MASK		0x20
#define	REG01_OTG_CONFIG_SHIFT		5
#define	REG01_OTG_ENABLE			1
#define	REG01_OTG_DISABLE			0

#define REG01_CHG_CONFIG_MASK     	0x10
#define REG01_CHG_CONFIG_SHIFT    	4
#define REG01_CHG_DISABLE        	0
#define REG01_CHG_ENABLE         	1

#define REG01_SYS_MINV_MASK       	0x0E
#define REG01_SYS_MINV_SHIFT      	1

#define	REG01_MIN_VBAT_SEL_MASK		0x01
#define	REG01_MIN_VBAT_SEL_SHIFT	0
#define	REG01_MIN_VBAT_2P8V			0
#define	REG01_MIN_VBAT_2P5V			1

/* Register 0x02*/
#define UPM6910X_REG_02              0x02
#define	REG02_BOOST_LIM_MASK		0x80
#define	REG02_BOOST_LIM_SHIFT		7
#define	REG02_BOOST_LIM_0P5A		0
#define	REG02_BOOST_LIM_1P2A		1

#define	REG02_Q1_FULLON_MASK		0x40
#define	REG02_Q1_FULLON_SHIFT		6
#define	REG02_Q1_FULLON_ENABLE		1
#define	REG02_Q1_FULLON_DISABLE		0

#define REG02_ICHG_MASK           	0x3F
#define REG02_ICHG_SHIFT          	0
#define REG02_ICHG_BASE           	0
#define REG02_ICHG_LSB            	60

/* Register 0x03*/
#define UPM6910X_REG_03              	0x03
#define REG03_IPRECHG_MASK        	0xF0
#define REG03_IPRECHG_SHIFT       	4
#define REG03_IPRECHG_BASE        	60
#define REG03_IPRECHG_LSB         	60

#define REG03_ITERM_MASK          	0x0F
#define REG03_ITERM_SHIFT         	0
#define REG03_ITERM_BASE          	60
#define REG03_ITERM_LSB           	60

/* Register 0x04*/
#define UPM6910X_REG_04              0x04
#define REG04_VREG_MASK           	0xF8
#define REG04_VREG_SHIFT          	3
#define REG04_VREG_BASE           	3856
#define REG04_VREG_LSB            	32

#define	REG04_TOPOFF_TIMER_MASK		0x06
#define	REG04_TOPOFF_TIMER_SHIFT	1
#define	REG04_TOPOFF_TIMER_DISABLE	0
#define	REG04_TOPOFF_TIMER_15M		1
#define	REG04_TOPOFF_TIMER_30M		2
#define	REG04_TOPOFF_TIMER_45M		3


#define REG04_VRECHG_MASK         	0x01
#define REG04_VRECHG_SHIFT        	0
#define REG04_VRECHG_100MV        	0
#define REG04_VRECHG_200MV        	1

/* Register 0x05*/
#define UPM6910X_REG_05             	0x05
#define REG05_EN_TERM_MASK        	0x80
#define REG05_EN_TERM_SHIFT       	7
#define REG05_TERM_ENABLE         	1
#define REG05_TERM_DISABLE        	0

#define REG05_WDT_MASK            	0x30
#define REG05_WDT_SHIFT           	4
#define REG05_WDT_DISABLE         	0
#define REG05_WDT_40S             	1
#define REG05_WDT_80S             	2
#define REG05_WDT_160S            	3
#define REG05_WDT_BASE            	0
#define REG05_WDT_LSB             	40

#define REG05_EN_TIMER_MASK       	0x08
#define REG05_EN_TIMER_SHIFT      	3
#define REG05_CHG_TIMER_ENABLE    	1
#define REG05_CHG_TIMER_DISABLE   	0

#define REG05_CHG_TIMER_MASK      	0x04
#define REG05_CHG_TIMER_SHIFT     	2
#define REG05_CHG_TIMER_5HOURS    	0
#define REG05_CHG_TIMER_10HOURS   	1

#define	REG05_TREG_MASK				0x02
#define	REG05_TREG_SHIFT			1
#define	REG05_TREG_90C				0
#define	REG05_TREG_110C				1

#define REG05_JEITA_ISET_MASK     	0x01
#define REG05_JEITA_ISET_SHIFT    	0
#define REG05_JEITA_ISET_50PCT    	0
#define REG05_JEITA_ISET_20PCT    	1

/* Register 0x06*/
#define UPM6910X_REG_06              0x06
#define	REG06_OVP_MASK				0xC0
#define	REG06_OVP_SHIFT				0x6
#define	REG06_OVP_5P5V				0
#define	REG06_OVP_6P5V				1
#define	REG06_OVP_10P5V				2
#define	REG06_OVP_14P0V				3

#define	REG06_BOOSTV_MASK			0x30
#define	REG06_BOOSTV_SHIFT			4
#define	REG06_BOOSTV_4P85V			0
#define	REG06_BOOSTV_5V				1
#define	REG06_BOOSTV_5P15V			2
#define	REG06_BOOSTV_5P3V			3

#define	REG06_VINDPM_MASK			0x0F
#define	REG06_VINDPM_SHIFT			0
#define	REG06_VINDPM_BASE			3900
#define	REG06_VINDPM_LSB			100

/* Register 0x07*/
#define UPM6910X_REG_07              0x07
#define REG07_FORCE_DPDM_MASK     	0x80
#define REG07_FORCE_DPDM_SHIFT    	7
#define REG07_FORCE_DPDM          	1

#define REG07_TMR2X_EN_MASK       	0x40
#define REG07_TMR2X_EN_SHIFT      	6
#define REG07_TMR2X_ENABLE        	1
#define REG07_TMR2X_DISABLE       	0

#define REG07_BATFET_DIS_MASK     	0x20
#define REG07_BATFET_DIS_SHIFT    	5
#define REG07_BATFET_OFF          	1
#define REG07_BATFET_ON          	0

#define REG07_JEITA_VSET_MASK     	0x10
#define REG07_JEITA_VSET_SHIFT    	4
#define REG07_JEITA_VSET_4100     	0
#define REG07_JEITA_VSET_VREG     	1

#define	REG07_BATFET_DLY_MASK		0x08
#define	REG07_BATFET_DLY_SHIFT		3
#define	REG07_BATFET_DLY_0S			0
#define	REG07_BATFET_DLY_10S		1

#define	REG07_BATFET_RST_EN_MASK	0x04
#define	REG07_BATFET_RST_EN_SHIFT	2
#define	REG07_BATFET_RST_DISABLE	0
#define	REG07_BATFET_RST_ENABLE		1

#define	REG07_VDPM_BAT_TRACK_MASK	0x03
#define	REG07_VDPM_BAT_TRACK_SHIFT 	0
#define	REG07_VDPM_BAT_TRACK_DISABLE	0
#define	REG07_VDPM_BAT_TRACK_200MV	1
#define	REG07_VDPM_BAT_TRACK_250MV	2
#define	REG07_VDPM_BAT_TRACK_300MV	3

/* Register 0x08*/
#define UPM6910X_REG_08              0x08
#define REG08_VBUS_STAT_MASK      	0xE0           
#define REG08_VBUS_STAT_SHIFT     	5
#define REG08_VBUS_TYPE_NONE	  	0
#define REG08_VBUS_TYPE_SDP       	1
#define REG08_VBUS_TYPE_CDP       	2
#define REG08_VBUS_TYPE_DCP			3
#define REG08_VBUS_TYPE_UNKNOWN		5
#define REG08_VBUS_TYPE_NON_STD		6
#define REG08_VBUS_TYPE_OTG       	7

#define REG08_CHRG_STAT_MASK      0x18
#define REG08_CHRG_STAT_SHIFT     3
#define REG08_CHRG_STAT_IDLE      0
#define REG08_CHRG_STAT_PRECHG    1
#define REG08_CHRG_STAT_FASTCHG   2
#define REG08_CHRG_STAT_CHGDONE   3

#define REG08_PG_STAT_MASK        0x04
#define REG08_PG_STAT_SHIFT       2
#define REG08_POWER_GOOD          1

#define REG08_THERM_STAT_MASK     0x02
#define REG08_THERM_STAT_SHIFT    1

#define REG08_VSYS_STAT_MASK      0x01
#define REG08_VSYS_STAT_SHIFT     0
#define REG08_IN_VSYS_STAT        1

/* Register 0x09*/
#define UPM6910X_REG_09              0x09
#define REG09_FAULT_WDT_MASK      0x80
#define REG09_FAULT_WDT_SHIFT     7
#define REG09_FAULT_WDT           1

#define REG09_FAULT_BOOST_MASK    0x40
#define REG09_FAULT_BOOST_SHIFT   6

#define REG09_FAULT_CHRG_MASK     0x30
#define REG09_FAULT_CHRG_SHIFT    4
#define REG09_FAULT_CHRG_NORMAL   0
#define REG09_FAULT_CHRG_INPUT    1
#define REG09_FAULT_CHRG_THERMAL  2
#define REG09_FAULT_CHRG_TIMER    3

#define REG09_FAULT_BAT_MASK      0x08
#define REG09_FAULT_BAT_SHIFT     3
#define	REG09_FAULT_BAT_OVP		1

#define REG09_FAULT_NTC_MASK      0x07
#define REG09_FAULT_NTC_SHIFT     0
#define	REG09_FAULT_NTC_NORMAL		0
#define REG09_FAULT_NTC_WARM      2
#define REG09_FAULT_NTC_COOL      3
#define REG09_FAULT_NTC_COLD      5
#define REG09_FAULT_NTC_HOT       6

/* Register 0x0A */
#define UPM6910X_REG_0A              0x0A
#define	REG0A_VBUS_GD_MASK			0x80
#define	REG0A_VBUS_GD_SHIFT			7
#define	REG0A_VBUS_GD				1

#define	REG0A_VINDPM_STAT_MASK		0x40
#define	REG0A_VINDPM_STAT_SHIFT		6
#define	REG0A_VINDPM_ACTIVE			1

#define	REG0A_IINDPM_STAT_MASK		0x20
#define	REG0A_IINDPM_STAT_SHIFT		5
#define	REG0A_IINDPM_ACTIVE			1

#define	REG0A_TOPOFF_ACTIVE_MASK	0x08
#define	REG0A_TOPOFF_ACTIVE_SHIFT	3
#define	REG0A_TOPOFF_ACTIVE			1

#define	REG0A_ACOV_STAT_MASK		0x04
#define	REG0A_ACOV_STAT_SHIFT		2
#define	REG0A_ACOV_ACTIVE			1

#define	REG0A_VINDPM_INT_MASK		0x02
#define	REG0A_VINDPM_INT_SHIFT		1
#define	REG0A_VINDPM_INT_ENABLE		0
#define	REG0A_VINDPM_INT_DISABLE	1

#define	REG0A_IINDPM_INT_MASK		0x01
#define	REG0A_IINDPM_INT_SHIFT		0
#define	REG0A_IINDPM_INT_ENABLE		0
#define	REG0A_IINDPM_INT_DISABLE	1

#define	REG0A_INT_MASK_MASK			0x03
#define	REG0A_INT_MASK_SHIFT		0

/* Register 0x0B */
#define	UPM6910X_REG_0B				0x0B
#define	REG0B_REG_RESET_MASK		0x80
#define	REG0B_REG_RESET_SHIFT		7
#define	REG0B_REG_RESET				1

#define REG0B_PN_MASK             	0x78
#define REG0B_PN_SHIFT            	3

#define REG0B_DEV_REV_MASK        	0x03
#define REG0B_DEV_REV_SHIFT       	0

static const u32 upm6910d_vac_ovp[] = {
	5500000, 6500000, 10500000, 14000000,
};

static const u32 upm6910d_wdt[] = {
	0, 40, 80, 160,
};

static struct mt_i2c_t i2c = {
	.id = I2C5,
	.addr = UPM6910D_SLAVE_ADDR,
	.mode = FS_MODE,
	.speed = 400,
};

/*
cmd：要读取的寄存器地址
data：输出参数，用于保存读回来的数据
返回值：成功为 I2C_OK，失败返回错误码
*/
static int upm6910d_i2c_read_byte(u8 cmd, u8 *data)
{
	int ret = 0;
	u8 regval = cmd;

	/*
	&i2c: I2C控制器实例（包含I2C总线号、设备地址等信息）
	&regval: 要写入的寄存器地址
	第一个1: 写入的字节数（1字节，即寄存器地址）
	第二个1: 要读取的字节数（1字节）
	这是一个复合I2C操作：先写入寄存器地址，然后立即读取该地址的数据。
*/
	ret = i2c_write_read(&i2c, &regval, 1, 1);
	if (ret != I2C_OK)
		pal_log_err("%s reg0x%X fail(%d)\n", __func__, cmd, ret);
	else {
		pal_log_debug("%s reg0x%X = 0x%X\n", __func__, cmd, regval);
		*data = regval;	//将读取到的数据通过指针data返回给调用者
	}

	return ret;
}

static int upm6910d_i2c_write_byte(u8 cmd, u8 data)
{
	int ret = 0;
	u8 write_buf[2] = {cmd, data};

	ret = i2c_write(&i2c, write_buf, 2);
	if (ret != I2C_OK)
		pal_log_err("%s reg0x%X = 0x%X fail(%d)\n",
			    __func__, cmd, data, ret);
	else
		pal_log_debug("%s reg0x%X = 0x%X\n", __func__, cmd, data);

	return ret;
}

static int upm6910d_i2c_update_bits(u8 cmd, u8 data, u8 mask)
{
	int ret = 0;
	u8 regval = 0;

	ret = upm6910d_i2c_read_byte(cmd, &regval);
	if (ret != I2C_OK)
		return ret;

	regval &= ~mask;			// 清除目标位
	regval |= (data & mask);	//写入新值

	return upm6910d_i2c_write_byte(cmd, regval);
}

static inline int upm6910d_set_bit(u8 cmd, u8 mask)
{
	return upm6910d_i2c_update_bits(cmd, mask, mask);
}

static inline int upm6910d_clr_bit(u8 cmd, u8 mask)
{
	return upm6910d_i2c_update_bits(cmd, 0x00, mask);
}

static void upm6910_main_set_aicr(unsigned char val)
{
    upm6910d_i2c_update_bits((unsigned char) (0),
                        (unsigned char) (val),
                        (unsigned char) (0x1F)
                        );
}

static void upm6910_main_set_ichg(unsigned char val)
{
    upm6910d_i2c_update_bits((unsigned char) (0x02),
                        (unsigned char) (val),
                        (unsigned char) (0x3F)
                        );
}

static void upm6910_main_set_chg_config(unsigned char val)
{
    upm6910d_i2c_update_bits((unsigned char) (0x1),
                       (unsigned char) (val),
                       (unsigned char) (0x10)
                      );
}

static void upm6910_main_set_default_cur(void)
{
    upm6910_main_set_ichg(0x8);//480ma
    upm6910_main_set_aicr(0x5);//300ma
}

static inline u8 upm6910d_closest_reg_via_tbl(const u32 *tbl, u32 tbl_size,
					    u32 target)
{
	u32 i = 0;

	if (target < tbl[0])
		return 0;

	for (i = 0; i < tbl_size - 1; i++) {
		if (target >= tbl[i] && target < tbl[i + 1])
			return i;
	}

	return tbl_size - 1;
}

static int upm6910d_detect_device(void)
{
	int ret = 0;
	u8 data = 0;
	u8 dev_id = 0;

	ret = upm6910d_i2c_read_byte(UPM6910X_REG_0B, &data);
	if (ret != I2C_OK) {
		pal_log_err("%s fail(%d)\n", __func__, ret);
	}

	pal_log_info("%s: data:%d\n", __func__, data);
	upm6910_main_set_default_cur();
	dev_id = (data & REG0B_PN_MASK) >> REG0B_PN_SHIFT;
	if (dev_id != UPM6910D_PN) {
		pal_log_err("%s incorrect devid 0x%X\n", __func__, dev_id);
		return -EOPNOTSUPP_I2C;
	}
	pal_log_err("%s DEVID = %d\n", __func__, dev_id);

	return ret;
}

static int upm6910d_reset_device(void)
{
	int ret = 0;
	
	ret = upm6910d_set_bit(UPM6910X_REG_0B, REG0B_REG_RESET_MASK);
	if (ret != I2C_OK)
		pal_log_err("%s fail(%d)\n", __func__, ret);

	return ret;
}

static int upm6910d_set_wdt(int sec)
{
	u8 regval = 0;

	/* 40s is the minimum, set to 40 except sec == 0 */
	if (sec <= 40 && sec > 0)
		sec = 40;
	
	regval = upm6910d_closest_reg_via_tbl(upm6910d_wdt, ARRAY_SIZE(upm6910d_wdt), sec);

	pal_log_info("%s time = %d(0x%X)\n", __func__, sec, regval);

	return upm6910d_i2c_update_bits(UPM6910X_REG_05,
				      regval << REG05_WDT_SHIFT,
				      REG05_WDT_MASK);
}

static int upm6910d_set_vac_ovp(u32 vac_ovp)
{
	u8 regval = 0;

	regval = upm6910d_closest_reg_via_tbl(upm6910d_vac_ovp,
					    ARRAY_SIZE(upm6910d_vac_ovp),
					    vac_ovp);

	pal_log_info("%s vac_ovp = %d(0x%X)\n", __func__, vac_ovp, regval);

	return upm6910d_i2c_update_bits(UPM6910X_REG_06,
				      regval << REG06_OVP_SHIFT,
				      REG06_OVP_MASK);
}

static int chgdet_pre_inited = 0;

#ifdef MTK_UNLOCK_BAT_SUPPORT
static int upm6910d_set_chargevolt(int volt)
{
        u8 val;

        if (volt < REG04_VREG_BASE)
                volt = REG04_VREG_BASE;

        val = (volt - REG04_VREG_BASE) / REG04_VREG_LSB;
        return upm6910d_i2c_update_bits(UPM6910X_REG_04,
                                   val << REG04_VREG_SHIFT,
                                   REG04_VREG_MASK);
}

int upm6910d_try_unlock_bat(u32 uV)
{
	int ret = 0;

	if (!chgdet_pre_inited) {

		ret = upm6910d_detect_device();
		if (ret) {
			pal_log_err("%s detect device fail, ret:%d\n",
					__func__, ret);
			return ret;
		}

	}

	return upm6910d_set_chargevolt(uV);
}
#endif

static int upm6910_enable_bc12(bool en)
{
	u8 regval = 0;

	pal_log_info("%s en = %d\n", __func__, en);

	if (en) {
		regval = REG07_FORCE_DPDM << REG07_FORCE_DPDM_SHIFT;
	} else {
		regval = 0;
	}

	return upm6910d_i2c_update_bits(UPM6910X_REG_07,
				      regval << REG07_FORCE_DPDM_SHIFT,
				      REG07_FORCE_DPDM_MASK);
}

static int mtk_ext_chgdet_pre_init(void)
{
	int ret = 0;

	Charger_Detect_Init();

	/* Toggle chgdet flow  通过 先关后开 的方式，可以确保触发一次全新的、完整的 BC1.2 检测流程。 */
	ret = upm6910_enable_bc12(false);
	if (ret != I2C_OK)
		goto out;

	ret = upm6910_enable_bc12(true);
	if (ret != I2C_OK)
		goto out;

out:
	return ret;
}

#ifdef MULTI_EXT_MAIN_CHARGER
int second_mainchg_chgdet(CHARGER_TYPE *chg_type) // mtk_ext_chgdet()
#else
int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
#endif
{
	int ret = 0, i = 0;
	u8 data = 0, power_good = 0, vbus_stat = 0;
	const int max_retry_cnt = 120;

	*chg_type = CHARGER_UNKNOWN;

	if (!chgdet_pre_inited) {

		ret = upm6910d_detect_device();
		if (ret) {
			pal_log_err("%s detect device fail, ret:%d\n",
					__func__, ret);
			return ret;
		}

		ret = mtk_ext_chgdet_pre_init();
		if (ret != I2C_OK) {
			pal_log_err("%s chgdet pre init fail(%d)\n",
				    __func__, ret);
			goto out;
		}
		mdelay(100);
	}

	do {
		ret = upm6910d_i2c_read_byte(UPM6910X_REG_08, &data);
		if (ret != I2C_OK)
			continue;
		pal_log_info("%s REG_08 = 0x%X\n", __func__, data);
		power_good = !!(data & REG08_PG_STAT_MASK);
		if (!power_good) {
			pal_log_info("%s REG_08:0x%X, no vbus, skip\n", __func__, data);
			goto out;
		} else {
			vbus_stat = (data & REG08_VBUS_STAT_MASK) >> REG08_VBUS_STAT_SHIFT;
			if (vbus_stat != REG08_VBUS_TYPE_NONE)
				break;
		}
		mdelay(20);
	} while (i++ < max_retry_cnt);

	if (i >= max_retry_cnt) {
		pal_log_err("%s chgdet fail\n", __func__);
		goto out;
	}

	switch (vbus_stat) {
		case REG08_VBUS_TYPE_NONE:
			*chg_type = CHARGER_UNKNOWN;
			break;
		case REG08_VBUS_TYPE_SDP:
			*chg_type = STANDARD_HOST;
			break;
		case REG08_VBUS_TYPE_CDP:
			*chg_type = CHARGING_HOST;
			break;
		case REG08_VBUS_TYPE_DCP:
			*chg_type = STANDARD_CHARGER;
			break;
		case REG08_VBUS_TYPE_UNKNOWN:
			*chg_type = NONSTANDARD_CHARGER;
			break;
		case REG08_VBUS_TYPE_NON_STD:
			*chg_type = STANDARD_CHARGER;
			break;
		default:
			*chg_type = NONSTANDARD_CHARGER;
			break;
	}

	Charger_Detect_Release();

out:
	pal_log_info("%s chg type = %d\n", __func__, *chg_type);

	return ret;
}

/*
完成以下关键初始化步骤：
检测 UPM6910D 芯片是否存在
软复位芯片（恢复默认状态）
关闭或设置看门狗（WDT）
设置输入过压保护（OVP）阈值
初始化 BC1.2 充电检测（可选）
⚠️ 这个函数通常在 操作系统启动前（Preloader 或 BootROM 阶段）调用，确保设备插上电源就能开始安全充电。
*/
int upm6910d_probe(void)
{
	int ret = 0;

	pal_log_info("preloader %s\n", __func__);

	ret = upm6910d_detect_device();
	if (ret) {
		pal_log_err("%s detect device fail, ret:%d\n",
				__func__, ret);
		return ret;
	}

	/*
	设置一个全局标志位，表示“充电检测预初始化已完成”。
	*/
	chgdet_pre_inited = 1;

	ret = upm6910d_reset_device();	//重置UPM6910D芯片到默认状态,软复位会让芯片所有寄存器恢复到默认值，确保处于已知状态
	if (ret != I2C_OK) {
		pal_log_err("%s detect reset fail(%d)\n", __func__, ret);
		goto out;
	}

	/*
	设置看门狗定时器为0，即禁用看门狗功能
	防止芯片在预引导阶段因超时而复位
	看门狗的作用是：如果系统长时间不“喂狗”，芯片会自动重启系统。
	在 Preloader 阶段通常不需要 WDT，所以先关掉。
	*/
	ret = upm6910d_set_wdt(0);
	if (ret != I2C_OK) {
		pal_log_err("%s set wdt fail(%d)\n", __func__, ret);
		goto out;
	}

	ret = upm6910d_set_vac_ovp(14000000);
	if (ret != I2C_OK) {
		pal_log_err("%s set vac ovp fail(%d)\n", __func__, ret);
		goto out;
	}

#ifdef MTK_EXT_CHGDET_SUPPORT
	ret = mtk_ext_chgdet_pre_init();
	if (ret != I2C_OK)
		pal_log_err("%s chgdet pre init fail(%d)\n", __func__, ret);
#endif

out:
	return ret;
}

