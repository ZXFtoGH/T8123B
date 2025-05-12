#include <mt6315-regulator.h>
#include <mt6315-spmi.h>
#include <mt6315_upmu_hw.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_errno.h>
#include <spmi.h>
#include <spmi_sw.h>
#include <typedefs.h>

/* Enable dump all register */
#define MT6315_DEBUG		0
/* Enable this option when pmic initial setting is verified */
#define INIT_SETTING_VERIFIED	1
/* Enable efuse SW load */
#define MT6315_EFUSE_SWLOAD	1

#define DECL_CHIP(_pidx, _mid, _type, _saddr)\
{					\
	.power_idx = _pidx,		\
	.master_idx = _mid,		\
	.slave_addr = _saddr,		\
	.type = _type,			\
	.spmi_dev = NULL,		\
}

static struct mt6315_chip mt6315_chip[MT6315_MAX] = {
	DECL_CHIP(MT6315_S3, SPMI_MASTER_0, MT6315_TYPE_C, SPMI_SLAVE_3),
};

#if INIT_SETTING_VERIFIED
static struct pmic_setting init_setting_C[] = {
	{0x13, 0x2, 0x2, 0},
	{0x15, 0x1F, 0x1F, 0},
	{0x22, 0x12, 0x12, 0},
	{0x8A, 0x6, 0xF, 0},
	{0x10B, 0x3, 0x3, 0},
	{0x38B, 0x4, 0xFF, 0},
	{0xA07, 0x0, 0x1, 0},
	{0xA1A, 0x1F, 0x1F, 0},
	{0x1457, 0x0, 0xFF, 0},
	{0x997, 0x43, 0x7F, 0},
	{0x999, 0xF0, 0xF0, 0},
	{0x9A0, 0x0, 0x1F, 0},
	{0x9A1, 0x0, 0x1F, 0},
	{0x9A2, 0x10, 0x1F, 0},
	{0x9A3, 0x1, 0x1F, 0},
	{0x1440, 0x0, 0x6, 0},
	{0x1487, 0x54, 0xFF, 0},
	{0x148B, 0x1, 0x7F, 0},
	{0x148C, 0x1, 0x7F, 0},
	{0x1507, 0x54, 0xFF, 0},
	{0x150B, 0x1, 0x7F, 0},
	{0x150C, 0x1, 0x7F, 0},
	{0x1587, 0x58, 0xFF, 0},
	{0x158B, 0x1, 0x7F, 0},
	{0x158C, 0x3, 0x7F, 0},
	{0x1607, 0x5C, 0xFF, 0},
	{0x160B, 0x1, 0x7F, 0},
	{0x160C, 0x1, 0x7F, 0},
	{0x1687, 0x22, 0x66, 0},
	{0x1688, 0xE, 0x2F, 0},
	{0x1689, 0xA1, 0xE1, 0},
	{0x168A, 0x79, 0x7F, 0},
	{0x168B, 0x12, 0x3F, 0},
	{0x168D, 0x0, 0xC, 0},
	{0x168E, 0xD7, 0xFF, 0},
	{0x168F, 0x81, 0xFF, 0},
	{0x1690, 0x13, 0x3F, 0},
	{0x1691, 0x22, 0x66, 0},
	{0x1692, 0xE, 0x2F, 0},
	{0x1693, 0xA1, 0xE1, 0},
	{0x1694, 0x79, 0x7F, 0},
	{0x1695, 0x12, 0x3F, 0},
	{0x1697, 0x0, 0xC, 0},
	{0x1698, 0xD7, 0xFF, 0},
	{0x1699, 0x81, 0xFF, 0},
	{0x169A, 0x13, 0x3F, 0},
	{0x169B, 0x20, 0x60, 0},
	{0x169C, 0xE, 0x2F, 0},
	{0x169D, 0x80, 0xC1, 0},
	{0x169E, 0xF8, 0xF8, 0},
	{0x169F, 0x12, 0x3F, 0},
	{0x16A1, 0x0, 0xC, 0},
	{0x16A2, 0xDB, 0xFF, 0},
	{0x16A3, 0xA1, 0xFF, 0},
	{0x16A4, 0x1, 0xF, 0},
	{0x16A5, 0x20, 0x60, 0},
	{0x16A6, 0xD, 0x2F, 0},
	{0x16A7, 0x81, 0xC1, 0},
	{0x16A8, 0xF8, 0xF8, 0},
	{0x16A9, 0x12, 0x3F, 0},
	{0x16AB, 0x0, 0xC, 0},
	{0x16AC, 0xDB, 0xFF, 0},
	{0x16AD, 0xA1, 0xFF, 0},
	{0x16AE, 0x3, 0x3F, 0},
	{0x16CE, 0x8, 0x8, 0},
};

static void mt6315_init_setting(void)
{
	int pidx = 0, ret = 0, i = 0;
	struct mt6315_chip *chip;

	for (pidx = 0; pidx < MT6315_MAX; pidx++) {
		chip = &mt6315_chip[pidx];
		if (!chip->spmi_dev) {
			MT6315_ERR("%s fail.\n", __func__);
			return;
		}

		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_DIG_WPK_KEY_H_ADDR, 0x63,
				MT6315_PMIC_DIG_WPK_KEY_H_MASK,
				MT6315_PMIC_DIG_WPK_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_DIG_WPK_KEY_ADDR, 0x15,
				MT6315_PMIC_DIG_WPK_KEY_MASK,
				MT6315_PMIC_DIG_WPK_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_H_ADDR, 0x9C,
				MT6315_PMIC_TMA_KEY_H_MASK,
				MT6315_PMIC_TMA_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_ADDR, 0xEA,
				MT6315_PMIC_TMA_KEY_MASK,
				MT6315_PMIC_TMA_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_CPS_W_KEY_H_ADDR, 0x47,
				MT6315_PMIC_RG_CPS_W_KEY_H_MASK,
				MT6315_PMIC_RG_CPS_W_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_CPS_W_KEY_ADDR, 0x29,
				MT6315_PMIC_RG_CPS_W_KEY_MASK,
				MT6315_PMIC_RG_CPS_W_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0x55,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0x43,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_MASK,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_ANA_WRITE_KEY_H_ADDR, 0x2A,
				MT6315_PMIC_ANA_WRITE_KEY_H_MASK,
				MT6315_PMIC_ANA_WRITE_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_ANA_WRITE_KEY_ADDR, 0x7C,
				MT6315_PMIC_ANA_WRITE_KEY_MASK,
				MT6315_PMIC_ANA_WRITE_KEY_SHIFT);

		if (chip->type == MT6315_TYPE_C)
			for (i = 0; i < ARRAY_SIZE(init_setting_C); i++)
				mt6315_write_field_byte(chip->spmi_dev,
						init_setting_C[i].addr,
						init_setting_C[i].val,
						init_setting_C[i].mask,
						init_setting_C[i].shift);

		/* reset sequence off to zero */
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_TOP2_RSV2_ADDR,
				0x0, 0x1, 0);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_ANA_WRITE_KEY_ADDR, 0,
				MT6315_PMIC_ANA_WRITE_KEY_MASK,
				MT6315_PMIC_ANA_WRITE_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_ANA_WRITE_KEY_H_ADDR, 0,
				MT6315_PMIC_ANA_WRITE_KEY_H_MASK,
				MT6315_PMIC_ANA_WRITE_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_MASK,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
				MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_CPS_W_KEY_ADDR, 0,
				MT6315_PMIC_RG_CPS_W_KEY_MASK,
				MT6315_PMIC_RG_CPS_W_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_CPS_W_KEY_H_ADDR, 0,
				MT6315_PMIC_RG_CPS_W_KEY_H_MASK,
				MT6315_PMIC_RG_CPS_W_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_ADDR, 0,
				MT6315_PMIC_TMA_KEY_MASK,
				MT6315_PMIC_TMA_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_H_ADDR, 0,
				MT6315_PMIC_TMA_KEY_H_MASK,
				MT6315_PMIC_TMA_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_DIG_WPK_KEY_ADDR, 0,
				MT6315_PMIC_DIG_WPK_KEY_MASK,
				MT6315_PMIC_DIG_WPK_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_DIG_WPK_KEY_H_ADDR,	0,
				MT6315_PMIC_DIG_WPK_KEY_H_MASK,
				MT6315_PMIC_DIG_WPK_KEY_H_SHIFT);
	}
}
#endif /*INIT_SETTING_VERIFIED*/

#if MT6315_DEBUG
static void mt6315_dump_register(int pidx)
{
	int addr = 0;
	uint8_t val = 0;
	struct mt6315_chip *chip;

	if (pidx >= MT6315_MAX)
		return;

	chip = &mt6315_chip[pidx];
	if (!chip->spmi_dev)
		return;

	for (addr = 0; addr <= MT6315_BUCK_TOP_4PHASE_ELR_29; addr++) {
		spmi_ext_register_readl(chip->spmi_dev, addr, &val, 1);
		MT6315_INFO("[0x%x]=0x%x ", addr, val);
	}
}
#endif

struct mt6315_chip *mt6315_find_chip(int pidx)
{
	int i;

	for (i = 0; i < MT6315_MAX; i++) {
		if (mt6315_chip[i].power_idx == pidx)
			return &mt6315_chip[i];
	}
	return NULL;
}

struct mt6315_chip *mt6315_find_chip_by_slvid(int slvid)
{
	int i;

	for (i = 0; i < MT6315_MAX; i++) {
		if (mt6315_chip[i].slave_addr == slvid)
			return &mt6315_chip[i];
	}
	return NULL;
}

static int mt6315_dbg_status(struct mt6315_chip *chip, unsigned char option)
{
	int ret = 0;
	unsigned char poff_sts = 0, val = 0;

	if (option) {
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_PMIC_RG_SLV_ID_ADDR, &val, 1);
		MT6315_INFO("S%d RG_SLV_ID[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_PMIC_RG_SLV_ID_ADDR, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_TOP_RST_STATUS, &val, 1);
		MT6315_INFO("S%d TOP_RST_STATUS[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_TOP_RST_STATUS, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_POFFSTS, &val, 1);
		MT6315_INFO("S%d POFFSTS[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_POFFSTS, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_PMIC_RGS_POFFSTS_ADDR, &poff_sts, 1);
		MT6315_INFO("S%d RGS_POFFSTS[0x%x]=0x%x\n", chip->slave_addr
			    , MT6315_PMIC_RGS_POFFSTS_ADDR, poff_sts);
		/* PG/OC SDN status */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_PG_SDN_STS0, &val, 1);
		MT6315_INFO("S%d PG_SDN_STS0[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_PG_SDN_STS0, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_OC_SDN_STS0, &val, 1);
		MT6315_INFO("S%d OC_SDN_STS0[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_OC_SDN_STS0, val);
		/* BUCK OC */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_BUCK_TOP_OC_CON0, &val, 1);
		MT6315_INFO("S%d BUCK_OC_SDN_STS[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_BUCK_TOP_OC_CON0, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_BUCK_TOP_ELR8, &val, 1);
		MT6315_INFO("S%d BUCK_OC_SDN_EN[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_BUCK_TOP_ELR8, val);
		/* WDTRST */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_TOP_RST_MISC, &val, 1);
		MT6315_INFO("S%d TOP_RST_MISC[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6315_TOP_RST_MISC, val);
	} else {
		/* Clear TOP_RST_STATUS */
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TOP_RST_STATUS_SET_ADDR, 0x3,
				MT6315_PMIC_TOP_RST_STATUS_SET_MASK,
				MT6315_PMIC_TOP_RST_STATUS_SET_SHIFT);
		/* Clear POFFSTS */
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_POFFSTS_CLR_ADDR, 1,
				MT6315_PMIC_RG_POFFSTS_CLR_MASK,
				MT6315_PMIC_RG_POFFSTS_CLR_SHIFT);
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_POFFSTS_CLR_ADDR, 0,
				MT6315_PMIC_RG_POFFSTS_CLR_MASK,
				MT6315_PMIC_RG_POFFSTS_CLR_SHIFT);
		/* Clear BUCK_OC_SDN_STATUS */
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_BUCK_TOP_OC_CON0, 0xF, 0xF, 0);
	}

	if (ret < 0) {
		MT6315_ERR("%s error\n", __func__);
		return -EIO;
	}
	return 0;
}

static void mt6315_wdt_set(void)
{
	int pidx = 0, ret = 0;
	struct mt6315_chip *chip;
	uint8_t val = 0;

	for (pidx = 0; pidx < MT6315_MAX; pidx++) {
		chip = &mt6315_chip[pidx];
		if (!chip->spmi_dev)
			return;
		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_H_ADDR, 0x63,
			MT6315_PMIC_DIG_WPK_KEY_H_MASK,
			MT6315_PMIC_DIG_WPK_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_ADDR, 0x15,
			MT6315_PMIC_DIG_WPK_KEY_MASK,
			MT6315_PMIC_DIG_WPK_KEY_SHIFT);

		/* Reset digital only */
		/* [1]=1, RG_WDTRSTB_DEB */
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_TOP_RST_MISC_SET, 0x02, 0xFF, 0);
		/* [0]=1, RG_WDTRSTB_EN */
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_TOP_RST_MISC_SET, 0x01, 0xFF, 0);
		/* [3]=1, Clear WDT status */
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_TOP_RST_MISC_SET, 0x08, 0xFF, 0);
		udelay(50);
		ret |= mt6315_write_field_byte(chip->spmi_dev,
				MT6315_TOP_RST_MISC_CLR, 0x08, 0xFF, 0);

		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_ADDR, 0,
			MT6315_PMIC_DIG_WPK_KEY_MASK,
			MT6315_PMIC_DIG_WPK_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_H_ADDR, 0,
			MT6315_PMIC_DIG_WPK_KEY_H_MASK,
			MT6315_PMIC_DIG_WPK_KEY_H_SHIFT);
		/* re-read the current wdt setting */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6315_TOP_RST_MISC, &val, 1);
		MT6315_INFO("S%d set TOP_RST_MISC=0x%x\n", chip->slave_addr, val);
		if (ret < 0) {
			MT6315_ERR("%s error\n", __func__);
		}
	}
}

#if SPMI_NO_PMIC
static int mt6315_check_cid(void)
{
	return -ENODEV;
}
#else
static int mt6315_check_cid(void)
{
	int pidx = 0, ret = 0;
	struct mt6315_chip *chip;
	struct spmi_device *dev;
	uint8_t val = 0;

	for (pidx = 0; pidx < MT6315_MAX; pidx++) {
		chip = &mt6315_chip[pidx];
		dev = get_spmi_device(chip->master_idx,
					mt6315_chip[pidx].slave_addr);
		if (!dev) {
			MT6315_ERR("%s fail(%d).\n", __func__, pidx);
			return -ENODEV;
		}
		chip->spmi_dev = dev;
		ret = spmi_ext_register_readl_field(dev,
						    MT6315_PMIC_SWCID_H_ADDR,
						    &val, 1,
						    MT6315_PMIC_SWCID_H_MASK,
						    MT6315_PMIC_SWCID_H_SHIFT);
		if (ret != 0 ||
		    val != MT6315_SWCID_H_CODE) {
			MT6315_ERR("%s ret=%d val=%d\n", __func__, ret, val);
			return -EIO;
		}

		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_H_ADDR, 0x63,
			MT6315_PMIC_DIG_WPK_KEY_H_MASK,
			MT6315_PMIC_DIG_WPK_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_ADDR, 0x15,
			MT6315_PMIC_DIG_WPK_KEY_MASK,
			MT6315_PMIC_DIG_WPK_KEY_SHIFT);
		/* Print debug status */
		mt6315_dbg_status(chip, 1);
		/* Clear debug status */
		mt6315_dbg_status(chip, 0);
		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_ADDR, 0x0,
			MT6315_PMIC_DIG_WPK_KEY_MASK,
			MT6315_PMIC_DIG_WPK_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_DIG_WPK_KEY_H_ADDR, 0x0,
			MT6315_PMIC_DIG_WPK_KEY_H_MASK,
			MT6315_PMIC_DIG_WPK_KEY_H_SHIFT);
	}

	return 0;
}
#endif
#if MT6315_EFUSE_SWLOAD
static unsigned int mt6315_read_efuse_nolock(struct mt6315_chip *chip, int i)
{
	unsigned char val = 0;
	unsigned int efuse_data = 0;

	/* 1. enable efuse ctrl engine clock */
	mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TOP_CKHWEN_CON0_CLR_ADDR, 0x1,
				MT6315_PMIC_RG_EFUSE_CK_PDN_HWEN_MASK,
				MT6315_PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TOP_CKPDN_CON1_CLR_ADDR, 0x1,
				MT6315_PMIC_RG_EFUSE_CK_PDN_MASK,
				MT6315_PMIC_RG_EFUSE_CK_PDN_SHIFT);
	/* 2. */
	mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_OTP_RD_SW_ADDR, 0x1,
				MT6315_PMIC_RG_OTP_RD_SW_MASK,
				MT6315_PMIC_RG_OTP_RD_SW_SHIFT);
	/* 3. Set row to read */
	mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_OTP_PA_ADDR, i,
				MT6315_PMIC_RG_OTP_PA_MASK,
				MT6315_PMIC_RG_OTP_PA_SHIFT);
	/* 4. Toggle RG_OTP_RD_TRIG */
	spmi_ext_register_readl(chip->spmi_dev,
				MT6315_PMIC_RG_OTP_RD_TRIG_ADDR,
				&val, 1);
	if (val == 0)
		val = 1;
	else
		val = 0;
	spmi_ext_register_writel(chip->spmi_dev,
		MT6315_PMIC_RG_OTP_RD_TRIG_ADDR, &val, 1);
	/* 5. Polling RG_OTP_RD_BUSY = 0 */
	udelay(300);
	do {
		spmi_ext_register_readl(chip->spmi_dev,
			MT6315_PMIC_RG_OTP_RD_BUSY_ADDR, &val, 1);
	} while(val == 1);
	/* 6. Read RG_OTP_DOUT_SW */
	udelay(100);
	spmi_ext_register_readl(chip->spmi_dev,
				MT6315_PMIC_RG_OTP_DOUT_SW_H_ADDR,
				&val, 1);
	efuse_data |= ((val & MT6315_PMIC_RG_OTP_DOUT_SW_H_MASK) << 8);
	spmi_ext_register_readl(chip->spmi_dev,
				MT6315_PMIC_RG_OTP_DOUT_SW_ADDR,
				&val, 1);
	efuse_data |= (val & MT6315_PMIC_RG_OTP_DOUT_SW_MASK);
	/* 7. disable efuse ctrl engine clock */
	mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TOP_CKHWEN_CON0_SET_ADDR, 0x1,
				MT6315_PMIC_RG_EFUSE_CK_PDN_HWEN_MASK,
				MT6315_PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TOP_CKPDN_CON1_SET_ADDR, 0x1,
				MT6315_PMIC_RG_EFUSE_CK_PDN_MASK,
				MT6315_PMIC_RG_EFUSE_CK_PDN_SHIFT);

	return efuse_data;
}

static void mt6315_efuse_get_id(struct mt6315_chip *chip)
{
	unsigned int efuse_data = 0, val = 0, i = 0;

	/* bit 1918 ~ 2045 */
	for (i = 127; i >= 119; i--) {
		/* bit 1904 ~ 1919 (i:119) */
		/* bit 2032 ~ 2047 (i:127) */
		efuse_data = mt6315_read_efuse_nolock(chip, i);
		if (i == 127)
			val = (efuse_data & 0x3FFF) << 2;
		else {
			val |= (efuse_data & 0xC000) >> 14;
			pal_log_info("0x%x:", val);
			val = (efuse_data & 0x3FFF) << 2;
		}
	}
	pal_log_info("(S%d)\n", chip->slave_addr);
}

/* PMIC EFUSE SW load need to check EFUSE_TABLE */
static void mt6315_efuse_sw_load(void)
{
	struct mt6315_chip *chip;
	unsigned int efuse_data = 0, val = 0;

	/* for MT6315_S3 */
	chip = &mt6315_chip[MT6315_S3];
	if (!chip->spmi_dev) {
		MT6315_ERR("%s fail.\n", __func__);
		return;
	}
	mt6315_efuse_get_id(chip);

	/* bit 640 ~ 655 */
	efuse_data = mt6315_read_efuse_nolock(chip, 40);
	if ((efuse_data & 0x1) == 0) {
		pal_log_info("%s edata=0x%x\n", __func__, efuse_data);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_H_ADDR, 0x9C,
				MT6315_PMIC_TMA_KEY_H_MASK,
				MT6315_PMIC_TMA_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_ADDR, 0xEA,
				MT6315_PMIC_TMA_KEY_MASK,
				MT6315_PMIC_TMA_KEY_SHIFT);
		/* Change Vbuck3 fsw */
		/* bit 528 ~ 543 */
		val = mt6315_read_efuse_nolock(chip, 33);
		val = (val & 0x7F) + 6;
		if (val <= 0x7F)
			mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_VBUCK3_TON_TRIM_ADDR, val,
				MT6315_PMIC_RG_VBUCK3_TON_TRIM_MASK,
				MT6315_PMIC_RG_VBUCK3_TON_TRIM_SHIFT);

		/* Change Vbuck4 fsw */
		/* bit 576 ~ 591 */
		val = mt6315_read_efuse_nolock(chip, 36);
		val = ((val >> 8) & 0x7F) - 20;
		if (val <= 0x7F)
			mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_VBUCK4_TON_TRIM_ADDR, val,
				MT6315_PMIC_RG_VBUCK4_TON_TRIM_MASK,
				MT6315_PMIC_RG_VBUCK4_TON_TRIM_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_ADDR, 0,
				MT6315_PMIC_TMA_KEY_MASK,
				MT6315_PMIC_TMA_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_H_ADDR, 0,
				MT6315_PMIC_TMA_KEY_H_MASK,
				MT6315_PMIC_TMA_KEY_H_SHIFT);
	}
}
#endif /*MT6315_EFUSE_SWLOAD*/

void mt6315_all_seq_off(unsigned char en_seq_off)
{
	int pidx = 0;
	struct mt6315_chip *chip;

	for (pidx = 0; pidx < MT6315_MAX; pidx++) {
		chip = &mt6315_chip[pidx];
		if (!chip->spmi_dev)
			return;
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_H_ADDR, 0x9C,
				MT6315_PMIC_TMA_KEY_H_MASK,
				MT6315_PMIC_TMA_KEY_H_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_ADDR, 0xEA,
				MT6315_PMIC_TMA_KEY_MASK,
				MT6315_PMIC_TMA_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_RG_TOP2_RSV2_ADDR,
				en_seq_off, 0x1, 0);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_ADDR, 0,
				MT6315_PMIC_TMA_KEY_MASK,
				MT6315_PMIC_TMA_KEY_SHIFT);
		mt6315_write_field_byte(chip->spmi_dev,
				MT6315_PMIC_TMA_KEY_H_ADDR, 0,
				MT6315_PMIC_TMA_KEY_H_MASK,
				MT6315_PMIC_TMA_KEY_H_SHIFT);
		MT6315_INFO("S%d set seq off=%d\n"
			    , chip->slave_addr, en_seq_off);
	}
	return;
}

int mt6315_write_field_byte(struct spmi_device *dev, unsigned int addr,
		unsigned char val, unsigned short mask, unsigned short shift) {
#if SPMI_NO_PMIC
	return -ENODEV;
#else
	return spmi_ext_register_writel_field(dev, addr, &val, 1, mask, shift);
#endif
}

int mt6315_set_buck_slew_rate(int slvid, uint8_t buck_id, int rrate, int frate)
{
	struct mt6315_chip *chip = mt6315_find_chip_by_slvid(slvid);

	if (!chip || !chip->spmi_dev)
		return -EINVAL;

	mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0x55,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
	mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0x43,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_MASK,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	if (rrate) {
		mt6315_write_field_byte(chip->spmi_dev,
					MT6315_PMIC_RG_BUCK_VBUCK1_SFCHG_RRATE_ADDR + 0x80 * (buck_id - 1),
					25000 / rrate / 1000 - 1,
					MT6315_PMIC_RG_BUCK_VBUCK1_SFCHG_RRATE_MASK,
					MT6315_PMIC_RG_BUCK_VBUCK1_SFCHG_RRATE_SHIFT);
	}
	if (frate) {
		mt6315_write_field_byte(chip->spmi_dev,
					MT6315_PMIC_RG_BUCK_VBUCK1_SFCHG_FRATE_ADDR + 0x80 * (buck_id - 1),
					25000 / frate / 1000 - 1,
					MT6315_PMIC_RG_BUCK_VBUCK1_SFCHG_FRATE_MASK,
					MT6315_PMIC_RG_BUCK_VBUCK1_SFCHG_FRATE_SHIFT);
	}
	mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_MASK,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	mt6315_write_field_byte(chip->spmi_dev,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
			MT6315_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
	return 0;
}

void mt6315_dump_record_reg(struct spmi_device *dev)
{
	u8 rdata0 = 0, rdata1 = 0, rdata2 = 0, rdata3 = 0;
	unsigned int offset;

	spmi_ext_register_readl(dev, MT6315_SPMI_DEBUG_CMD0, &rdata0, 1);
	SPMI_CRI("slvid:%d DBG. Last cmd idx:%d\n",
		dev->slvid, (((rdata0 & 0xc) >> 2) + 3) % 4);
	/* log sequence, idx 0->1->2->3->0 */
	for (offset = MT6315_SPMI_DEBUG_ADDR0;
	     offset <= MT6315_SPMI_DEBUG_ADDR3; offset += 4) {
		spmi_ext_register_readl(dev, offset, &rdata0, 1);
		spmi_ext_register_readl(dev, offset + 1, &rdata1, 1);
		spmi_ext_register_readl(dev, offset + 2, &rdata2, 1);
		spmi_ext_register_readl(dev, offset + 3, &rdata3, 1);
		SPMI_CRI("Idx:%d slvid:%d Type:0x%x, [0x%x]=0x%x\r\n",
			(offset - MT6315_SPMI_DEBUG_ADDR0) / 4,
			dev->slvid, (rdata3 & 0x3),
			(rdata1 << 0x8) | rdata0, rdata2);
	}
}

int mt6315_spmi_probe(void)
{
	int ret = 0;

	ret = mt6315_check_cid();
	if (ret < 0)
		return ret;

	ret = mt6315_regulator_init();
	if (ret < 0)
		return ret;

	mt6315_wdt_set();
#if INIT_SETTING_VERIFIED
	mt6315_init_setting();
#endif /*INIT_SETTING_VERIFIED*/
#if MT6315_EFUSE_SWLOAD
	mt6315_efuse_sw_load();
#endif
	MT6315_INFO("%s done\n", __func__);

	return 0;
}
