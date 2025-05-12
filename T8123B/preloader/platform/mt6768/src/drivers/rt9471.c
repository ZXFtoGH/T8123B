#include <i2c.h>
#include <mt_drcc.h>
#include <mtk_subpmic.h>
#include <pal_log.h>
#include <timer.h>

#define RT9471_PL_DRV_VERSION	"1.0.3_MTK"

#define RT9471_SLAVE_ADDR	(0x53)
#define RT9470_DEVID		0x09
#define RT9470D_DEVID		0x0A
#define RT9471_DEVID		0x0D
#define RT9471D_DEVID		0x0E

#define RT9471_REG_TOP		(0x01)
#define RT9471_REG_IBUS		(0x03)
#define RT9471_REG_VBUS		(0x04)
#define RT9471_REG_VCHG		(0x07)
#define RT9471_REG_INFO		(0x0B)
#define RT9471_REG_DPDMDET	(0x0E)
#define RT9471_REG_STATUS	(0x0F)

/* ========== TOP 0x01 ============ */
#define RT9471_WDT_SHIFT	0
#define RT9471_WDT_MASK		0x03
/* ========== IBUS 0x03 ============ */
#define RT9471_AUTOAICR_MASK	BIT(6)
/* ========== VBUS 0x04 ============ */
#define RT9471_VAC_OVP_SHIFT	6
#define RT9471_VAC_OVP_MASK	0xC0
/* ========== VCHG 0x07 ============ */
#define RT9471_CV_SHIFT		0
#define RT9471_CV_MASK		0x7F
/* uV */
#define RT9471_CV_MIN		3900000
#define RT9471_CV_MAX		4700000
#define RT9471_CV_STEP		10000
/* ========== INFO 0x0B ============ */
#define RT9471_REGRST_MASK	BIT(7)
#define RT9471_DEVID_SHIFT	3
#define RT9471_DEVID_MASK	0x78
/* ========== DPDMDET 0x0E ============ */
#define RT9471_BC12_EN_MASK	BIT(7)
/* ========== STATUS 0x0F ============ */
#define RT9471_PORTSTAT_SHIFT	4
#define RT9471_PORTSTAT_MASK	0xF0

static const u32 rt9471_vac_ovp[] = {
	5800000, 6500000, 10900000, 14000000,
};

static const u32 rt9471_wdt[] = {
	0, 40, 80, 160,
};

enum rt9471_port_stat {
	RT9471_PORTSTAT_NOINFO = 0,
	RT9471_PORTSTAT_APPLE_10W = 8,
	RT9471_PORTSTAT_SAMSUNG_10W,
	RT9471_PORTSTAT_APPLE_5W,
	RT9471_PORTSTAT_APPLE_12W,
	RT9471_PORTSTAT_NSDP,
	RT9471_PORTSTAT_SDP,
	RT9471_PORTSTAT_CDP,
	RT9471_PORTSTAT_DCP,
	RT9471_PORTSTAT_MAX,
};

static struct mt_i2c_t i2c = {
	.id = I2C7,
	.addr = RT9471_SLAVE_ADDR,
	.mode = FS_MODE,
	.speed = 400,
};

static int rt9471_i2c_read_byte(u8 cmd, u8 *data)
{
	int ret = 0;
	u8 regval = cmd;

	ret = i2c_write_read(&i2c, &regval, 1, 1);
	if (ret != I2C_OK)
		pal_log_err("%s reg0x%X fail(%d)\n", __func__, cmd, ret);
	else {
		pal_log_debug("%s reg0x%X = 0x%X\n", __func__, cmd, regval);
		*data = regval;
	}

	return ret;
}

static int rt9471_i2c_write_byte(u8 cmd, u8 data)
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

static int rt9471_i2c_update_bits(u8 cmd, u8 data, u8 mask)
{
	int ret = 0;
	u8 regval = 0;

	ret = rt9471_i2c_read_byte(cmd, &regval);
	if (ret != I2C_OK)
		return ret;

	regval &= ~mask;
	regval |= (data & mask);

	return rt9471_i2c_write_byte(cmd, regval);
}

static inline int rt9471_set_bit(u8 cmd, u8 mask)
{
	return rt9471_i2c_update_bits(cmd, mask, mask);
}

static inline int rt9471_clr_bit(u8 cmd, u8 mask)
{
	return rt9471_i2c_update_bits(cmd, 0x00, mask);
}

static inline u8 rt9471_closest_reg_via_tbl(const u32 *tbl, u32 tbl_size,
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

static int rt9471_enable_autoaicr(bool en)
{
	pal_log_info("%s en = %d\n", __func__, en);
	return (en ? rt9471_set_bit : rt9471_clr_bit)
		(RT9471_REG_IBUS, RT9471_AUTOAICR_MASK);
}

static int rt9471_set_wdt(u32 sec)
{
	u8 regval = 0;

	/* 40s is the minimum, set to 40 except sec == 0 */
	if (sec <= 40 && sec > 0)
		sec = 40;
	regval = rt9471_closest_reg_via_tbl(rt9471_wdt, ARRAY_SIZE(rt9471_wdt),
					    sec);

	pal_log_info("%s time = %d(0x%X)\n", __func__, sec, regval);

	return rt9471_i2c_update_bits(RT9471_REG_TOP,
				      regval << RT9471_WDT_SHIFT,
				      RT9471_WDT_MASK);
}

static int rt9471_set_vac_ovp(u32 vac_ovp)
{
	u8 regval = 0;

	regval = rt9471_closest_reg_via_tbl(rt9471_vac_ovp,
					    ARRAY_SIZE(rt9471_vac_ovp),
					    vac_ovp);

	pal_log_info("%s vac_ovp = %d(0x%X)\n", __func__, vac_ovp, regval);

	return rt9471_i2c_update_bits(RT9471_REG_VBUS,
				      regval << RT9471_VAC_OVP_SHIFT,
				      RT9471_VAC_OVP_MASK);
}

#ifdef MTK_UNLOCK_BAT_SUPPORT
static inline u8 rt9471_closest_reg(u32 min, u32 max, u32 step, u32 target)
{
	if (target < min)
		return 0;

	if (target >= max)
		target = max;

	return (target - min) / step;
}

static int rt9471_set_cv(u32 cv)
{
	u8 regval = 0;

	regval = rt9471_closest_reg(RT9471_CV_MIN, RT9471_CV_MAX,
				    RT9471_CV_STEP, cv);

	pal_log_info("%s cv = %d(0x%X)\n", __func__, cv, regval);

	return rt9471_i2c_update_bits(RT9471_REG_VCHG,
				      regval << RT9471_CV_SHIFT,
				      RT9471_CV_MASK);
}

int mtk_try_unlock_bat(u32 uV)
{
	return rt9471_set_cv(uV);
}
#endif

#ifdef MTK_EXT_CHGDET_SUPPORT
static int rt9471_enable_bc12(bool en)
{
	pal_log_info("%s en = %d\n", __func__, en);
	return (en ? rt9471_set_bit : rt9471_clr_bit)
		(RT9471_REG_DPDMDET, RT9471_BC12_EN_MASK);
}

static int chgdet_pre_inited = 0;

static int mtk_ext_chgdet_pre_init(void)
{
	int ret = 0;
	u8 dev_id = 0;

	ret = rt9471_i2c_read_byte(RT9471_REG_INFO, &dev_id);
	if (ret != I2C_OK)
		return ret;
	dev_id = (dev_id & RT9471_DEVID_MASK) >> RT9471_DEVID_SHIFT;
	if (dev_id != RT9470D_DEVID && dev_id != RT9471D_DEVID) {
		pal_log_err("%s incorrect devid 0x%X\n", __func__, dev_id);
		return -EOPNOTSUPP_I2C;
	}

	Charger_Detect_Init();

	/* Toggle chgdet flow */
	ret = rt9471_enable_bc12(false);
	if (ret != I2C_OK)
		goto out;

	ret = rt9471_enable_bc12(true);
	if (ret != I2C_OK)
		goto out;

	chgdet_pre_inited = 1;
out:
	return ret;
}

int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int ret = 0, i = 0;
	const int max_retry_cnt = 120;
	u8 stat = 0;

	*chg_type = CHARGER_UNKNOWN;

	if (!chgdet_pre_inited) {
		ret = mtk_ext_chgdet_pre_init();
		if (ret != I2C_OK) {
			pal_log_err("%s chgdet pre init fail(%d)\n",
				    __func__, ret);
			goto out;
		}
		mdelay(100);
	}

	for (i = 0; i < max_retry_cnt; i++) {
		ret = rt9471_i2c_read_byte(RT9471_REG_STATUS, &stat);
		if (ret != I2C_OK)
			continue;
		pal_log_info("%s ic_stat = 0x%X\n", __func__, stat);
		stat = (stat & RT9471_PORTSTAT_MASK) >> RT9471_PORTSTAT_SHIFT;
		if (stat != RT9471_PORTSTAT_NOINFO)
			break;
		mdelay(20);
	}
	if (i == max_retry_cnt) {
		pal_log_err("%s chgdet fail\n", __func__);
		goto out;
	}

	switch (stat) {
	case RT9471_PORTSTAT_NOINFO:
		*chg_type = CHARGER_UNKNOWN;
		break;
	case RT9471_PORTSTAT_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case RT9471_PORTSTAT_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case RT9471_PORTSTAT_SAMSUNG_10W:
	case RT9471_PORTSTAT_APPLE_12W:
	case RT9471_PORTSTAT_DCP:
		*chg_type = STANDARD_CHARGER;
		break;
	case RT9471_PORTSTAT_APPLE_10W:
		*chg_type = APPLE_2_1A_CHARGER;
		break;
	case RT9471_PORTSTAT_APPLE_5W:
		*chg_type = APPLE_1_0A_CHARGER;
		break;
	case RT9471_PORTSTAT_NSDP:
	default:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	}

out:
	pal_log_info("%s chg type = %d\n", __func__, *chg_type);
	Charger_Detect_Release();

	return ret;
}
#endif

int mtk_subpmic_init(void)
{
	int ret = 0;
	u8 regval = 0;

	pal_log_info("%s (%s)\n", __func__, RT9471_PL_DRV_VERSION);

	ret = rt9471_set_bit(RT9471_REG_INFO, RT9471_REGRST_MASK);
	if (ret != I2C_OK)
		pal_log_err("%s reg reset fail(%d)\n", __func__, ret);
	ret = rt9471_enable_autoaicr(false);
	if (ret != I2C_OK)
		pal_log_err("%s dis autoaicr fail(%d)\n", __func__, ret);
	ret = rt9471_set_wdt(0);
	if (ret != I2C_OK)
		pal_log_err("%s dis wdt fail(%d)\n", __func__, ret);
	/* 14V */
	ret = rt9471_set_vac_ovp(14000000);
	if (ret != I2C_OK)
		pal_log_err("%s set vac_ovp fail(%d)\n", __func__, ret);
#ifdef MTK_EXT_CHGDET_SUPPORT
	ret = mtk_ext_chgdet_pre_init();
	if (ret != I2C_OK)
		pal_log_err("%s chgdet pre init fail(%d)\n", __func__, ret);
#endif
	return ret;
}

/*
 * Release Note
 * 1.0.3
 * (1) Disable Auto AICR and WDT after register reset
 *
 * 1.0.2
 * (1) Sync with LK Driver
 * (2) Add mtk_subpmic_init()
 *
 * 1.0.1
 * (1) Rearrange the #if blocks
 * (2) Add support for RT9470/RT9470D
 * (3) Sync with LK Driver
 * (4) Shorten the delay of mtk_ext_chgdet()
 *
 * 1.0.0
 * (1) Initial release
 */
