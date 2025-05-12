#include <mt6319-regulator.h>
#include <mt6319-spmi.h>
#include <mt6319_upmu_hw.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_errno.h>
#include <spmi.h>
#include <spmi_sw.h>
#include <timer.h>
#include <typedefs.h>

/* Enable dump all register */
#define MT6319_DEBUG		0
/* Enable this option when pmic initial setting is verified */
#define INIT_SETTING_VERIFIED	1

struct pmic_setting {
	unsigned short addr;
	unsigned short val;
	unsigned short mask;
	unsigned char shift;
};

#define DECL_CHIP(_pidx, _mid, _type, _saddr)\
{					\
	.power_idx = _pidx,		\
	.master_idx = _mid,		\
	.slave_addr = _saddr,		\
	.type = _type,			\
	.spmi_dev = NULL,		\
}

static struct mt6319_chip mt6319_chip[MT6319_MAX] = {
	DECL_CHIP(MT6319_S6, SPMI_MASTER_1, MT6319_TYPE_A, SPMI_SLAVE_6),
};

int mt6319_write_field_byte(struct spmi_device *dev, uint32_t addr,
			    uint8_t val, uint16_t mask, uint16_t shift)
{
#if !SPMI_NO_PMIC
	return spmi_ext_register_writel_field(dev, addr, &val, 1, mask, shift);
#else
	return -ENODEV;
#endif
}

#if INIT_SETTING_VERIFIED
static struct pmic_setting init_setting_A[] = {
	{0x13, 0x2, 0x2, 0},
	{0x15, 0x1F, 0x1F, 0},
	{0x22, 0x12, 0x12, 0},
	{0x8A, 0x6, 0xF, 0},
	{0x10B, 0x3, 0x3, 0},
	{0x38B, 0x4, 0xFF, 0},
	{0xA07, 0x0, 0x1, 0},
	{0xA1A, 0x1F, 0x1F, 0},
	{0x1457, 0x0, 0xFF, 0},
	{0x997, 0x1F, 0x7F, 0},
	{0x999, 0xF0, 0xF0, 0},
	{0x9A0, 0x7, 0x1F, 0},
	{0x9A1, 0x7, 0x1F, 0},
	{0x9A2, 0x0, 0x1F, 0},
	{0x9A3, 0x2, 0x1F, 0},
	{0xA0C, 0x0, 0x78, 0},
	{0xA11, 0x1, 0x1, 0},
	{0xA1A, 0x1F, 0x1F, 0},
	{0x1440, 0xD, 0xF, 0},
	{0x1487, 0x58, 0xFF, 0},
	{0x148B, 0x1, 0x7F, 0},
	{0x148C, 0x3, 0x7F, 0},
	{0x1507, 0x58, 0xFF, 0},
	{0x150B, 0x1, 0x7F, 0},
	{0x150C, 0x3, 0x7F, 0},
	{0x1587, 0x58, 0xFF, 0},
	{0x158B, 0x1, 0x7F, 0},
	{0x158C, 0x3, 0x7F, 0},
	{0x1607, 0x58, 0xFF, 0},
	{0x160B, 0x1, 0x7F, 0},
	{0x160C, 0x3, 0x7F, 0},
	{0x1687, 0x22, 0x76, 0},
	{0x1688, 0xE, 0x2F, 0},
	{0x1689, 0xA1, 0xE1, 0},
	{0x168A, 0x79, 0x7F, 0},
	{0x168B, 0x12, 0x3F, 0},
	{0x168E, 0xD7, 0xFF, 0},
	{0x168F, 0x81, 0xFF, 0},
	{0x1690, 0x3, 0x3F, 0},
	{0x1691, 0x22, 0x76, 0},
	{0x1692, 0xE, 0x2F, 0},
	{0x1693, 0xA1, 0xE1, 0},
	{0x1694, 0x79, 0x7F, 0},
	{0x1695, 0x12, 0x3F, 0},
	{0x1698, 0xD7, 0xFF, 0},
	{0x1699, 0x81, 0xFF, 0},
	{0x169A, 0x3, 0x3F, 0},
	{0x169B, 0x20, 0x70, 0},
	{0x169C, 0xE, 0x2F, 0},
	{0x169D, 0x81, 0xC1, 0},
	{0x169E, 0xF8, 0xF8, 0},
	{0x169F, 0x12, 0x3F, 0},
	{0x16A2, 0xDB, 0xFF, 0},
	{0x16A3, 0xA1, 0xFF, 0},
	{0x16A4, 0x3, 0xF, 0},
	{0x16A5, 0x20, 0x70, 0},
	{0x16A6, 0xE, 0x2F, 0},
	{0x16A7, 0x81, 0xC1, 0},
	{0x16A8, 0xF8, 0xF8, 0},
	{0x16A9, 0x12, 0x3F, 0},
	{0x16AC, 0xDB, 0xFF, 0},
	{0x16AD, 0xA1, 0xFF, 0},
	{0x16AE, 0x3, 0xF, 0},
	{0x16B8, 0x20, 0xE0, 0},
	{0x16B9, 0x0, 0xF0, 0},
};

static void mt6319_init_setting(void)
{
	int pidx = 0, ret = 0;
	unsigned int i = 0;
	struct mt6319_chip *chip;

	for (pidx = 0; pidx < MT6319_MAX; pidx++) {
		chip = &mt6319_chip[pidx];
		if (!chip->spmi_dev) {
			MT6319_ERR("%s fail.\n", __func__);
			return;
		}

		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_DIG_WPK_KEY_H_ADDR, 0x63,
				MT6319_PMIC_DIG_WPK_KEY_H_MASK,
				MT6319_PMIC_DIG_WPK_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_DIG_WPK_KEY_ADDR, 0x15,
				MT6319_PMIC_DIG_WPK_KEY_MASK,
				MT6319_PMIC_DIG_WPK_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_TMA_KEY_H_ADDR, 0x9C,
				MT6319_PMIC_TMA_KEY_H_MASK,
				MT6319_PMIC_TMA_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_TMA_KEY_ADDR, 0xEA,
				MT6319_PMIC_TMA_KEY_MASK,
				MT6319_PMIC_TMA_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_RG_CPS_W_KEY_H_ADDR, 0x47,
				MT6319_PMIC_RG_CPS_W_KEY_H_MASK,
				MT6319_PMIC_RG_CPS_W_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_RG_CPS_W_KEY_ADDR, 0x29,
				MT6319_PMIC_RG_CPS_W_KEY_MASK,
				MT6319_PMIC_RG_CPS_W_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0x55,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0x43,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_MASK,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_ANA_WRITE_KEY_H_ADDR, 0x2A,
				MT6319_PMIC_ANA_WRITE_KEY_H_MASK,
				MT6319_PMIC_ANA_WRITE_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_ANA_WRITE_KEY_ADDR, 0x7C,
				MT6319_PMIC_ANA_WRITE_KEY_MASK,
				MT6319_PMIC_ANA_WRITE_KEY_SHIFT);

		if (chip->type == MT6319_TYPE_A)
			for (i = 0; i < ARRAY_SIZE(init_setting_A); i++)
				mt6319_write_field_byte(chip->spmi_dev,
						init_setting_A[i].addr,
						init_setting_A[i].val,
						init_setting_A[i].mask,
						init_setting_A[i].shift);
		mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_RG_LDO_VDIG18_HW_OP_EN_ADDR, 1,
			MT6319_PMIC_RG_LDO_VDIG18_HW_OP_EN_MASK,
			MT6319_PMIC_RG_LDO_VDIG18_HW_OP_EN_SHIFT);

		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_ANA_WRITE_KEY_ADDR, 0,
				MT6319_PMIC_ANA_WRITE_KEY_MASK,
				MT6319_PMIC_ANA_WRITE_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_ANA_WRITE_KEY_H_ADDR, 0,
				MT6319_PMIC_ANA_WRITE_KEY_H_MASK,
				MT6319_PMIC_ANA_WRITE_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_MASK,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
				MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_RG_CPS_W_KEY_ADDR, 0,
				MT6319_PMIC_RG_CPS_W_KEY_MASK,
				MT6319_PMIC_RG_CPS_W_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_RG_CPS_W_KEY_H_ADDR, 0,
				MT6319_PMIC_RG_CPS_W_KEY_H_MASK,
				MT6319_PMIC_RG_CPS_W_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_TMA_KEY_ADDR, 0,
				MT6319_PMIC_TMA_KEY_MASK,
				MT6319_PMIC_TMA_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_TMA_KEY_H_ADDR, 0,
				MT6319_PMIC_TMA_KEY_H_MASK,
				MT6319_PMIC_TMA_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_DIG_WPK_KEY_ADDR, 0,
				MT6319_PMIC_DIG_WPK_KEY_MASK,
				MT6319_PMIC_DIG_WPK_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_DIG_WPK_KEY_H_ADDR,	0,
				MT6319_PMIC_DIG_WPK_KEY_H_MASK,
				MT6319_PMIC_DIG_WPK_KEY_H_SHIFT);
	}
}
#endif /*INIT_SETTING_VERIFIED*/

#if MT6319_DEBUG
static void mt6319_dump_register(int pidx)
{
	int addr = 0;
	uint8_t val = 0;
	struct mt6319_chip *chip;

	if (pidx >= MT6319_MAX)
		return;

	chip = &mt6319_chip[pidx];
	if (!chip->spmi_dev)
		return;

	for (addr = 0; addr <= MT6319_BUCK_TOP_4PHASE_ELR_29; addr++) {
		spmi_ext_register_readl(chip->spmi_dev, addr, &val, 1);
		MT6319_INFO("[0x%x]=0x%x ", addr, val);
	}
}
#endif

struct mt6319_chip *mt6319_find_chip(int pidx)
{
	int i;

	for (i = 0; i < MT6319_MAX; i++) {
		if (mt6319_chip[i].power_idx == pidx)
			return &mt6319_chip[i];
	}
	return NULL;
}

struct mt6319_chip *mt6319_find_chip_by_slvid(int slvid)
{
	int i;

	for (i = 0; i < MT6319_MAX; i++) {
		if (mt6319_chip[i].slave_addr == slvid)
			return &mt6319_chip[i];
	}
	return NULL;
}

static int mt6319_dbg_status(struct mt6319_chip *chip, unsigned char option)
{
	int ret = 0;
	unsigned char poff_sts = 0, val = 0;

	if (option) {
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_PMIC_RG_SLV_ID_ADDR, &val, 1);
		MT6319_INFO("S%d RG_SLV_ID[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6319_PMIC_RG_SLV_ID_ADDR, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_POFFSTS, &val, 1);
		MT6319_INFO("S%d POFFSTS[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6319_POFFSTS, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_PMIC_RGS_POFFSTS_ADDR, &poff_sts, 1);
		MT6319_INFO("S%d RGS_POFFSTS[0x%x]=0x%x\n", chip->slave_addr
			    , MT6319_PMIC_RGS_POFFSTS_ADDR, poff_sts);
		/* PG/OC SDN status */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_PG_SDN_STS0, &val, 1);
		MT6319_INFO("S%d PG_SDN_STS0[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6319_PG_SDN_STS0, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_OC_SDN_STS0, &val, 1);
		MT6319_INFO("S%d OC_SDN_STS0[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6319_OC_SDN_STS0, val);
		/* BUCK OC */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_BUCK_TOP_OC_CON0, &val, 1);
		MT6319_INFO("S%d BUCK_OC_SDN_STS[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6319_BUCK_TOP_OC_CON0, val);
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_BUCK_TOP_ELR8, &val, 1);
		MT6319_INFO("S%d BUCK_OC_SDN_EN[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6319_BUCK_TOP_ELR8, val);
		/* WDTRST */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_TOP_RST_MISC, &val, 1);
		MT6319_INFO("S%d TOP_RST_MISC[0x%x]=0x%x\n"
			    , chip->slave_addr, MT6319_TOP_RST_MISC, val);
	} else {
		/* Clear POFFSTS */
		ret |= mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_RG_POFFSTS_CLR_ADDR, 1,
				MT6319_PMIC_RG_POFFSTS_CLR_MASK,
				MT6319_PMIC_RG_POFFSTS_CLR_SHIFT);
		ret |= mt6319_write_field_byte(chip->spmi_dev,
				MT6319_PMIC_RG_POFFSTS_CLR_ADDR, 0,
				MT6319_PMIC_RG_POFFSTS_CLR_MASK,
				MT6319_PMIC_RG_POFFSTS_CLR_SHIFT);
		/* Clear BUCK_OC_SDN_STATUS */
		ret |= mt6319_write_field_byte(chip->spmi_dev,
				MT6319_BUCK_TOP_OC_CON0, 0xF, 0xF, 0);
	}

	if (ret < 0) {
		MT6319_ERR("%s error\n", __func__);
		return -EIO;
	}
	return 0;
}

static void mt6319_wdt_set(void)
{
	int pidx = 0, ret = 0;
	struct mt6319_chip *chip;
	uint8_t val = 0;

	for (pidx = 0; pidx < MT6319_MAX; pidx++) {
		chip = &mt6319_chip[pidx];
		if (!chip->spmi_dev)
			return;
		mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_DIG_WPK_KEY_H_ADDR, 0x63,
			MT6319_PMIC_DIG_WPK_KEY_H_MASK,
			MT6319_PMIC_DIG_WPK_KEY_H_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_DIG_WPK_KEY_ADDR, 0x15,
			MT6319_PMIC_DIG_WPK_KEY_MASK,
			MT6319_PMIC_DIG_WPK_KEY_SHIFT);

		/* Reset digital only */
		/* [1]=1, RG_WDTRSTB_DEB */
		ret |= mt6319_write_field_byte(chip->spmi_dev,
				MT6319_TOP_RST_MISC_SET, 0x02, 0xFF, 0);
		/* [0]=1, RG_WDTRSTB_EN */
		ret |= mt6319_write_field_byte(chip->spmi_dev,
				MT6319_TOP_RST_MISC_SET, 0x01, 0xFF, 0);
		/* [3]=1, Clear WDT status */
		ret |= mt6319_write_field_byte(chip->spmi_dev,
				MT6319_TOP_RST_MISC_SET, 0x08, 0xFF, 0);
		udelay(50);
		ret |= mt6319_write_field_byte(chip->spmi_dev,
				MT6319_TOP_RST_MISC_CLR, 0x08, 0xFF, 0);

		mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_DIG_WPK_KEY_ADDR, 0,
			MT6319_PMIC_DIG_WPK_KEY_MASK,
			MT6319_PMIC_DIG_WPK_KEY_SHIFT);
		mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_DIG_WPK_KEY_H_ADDR, 0,
			MT6319_PMIC_DIG_WPK_KEY_H_MASK,
			MT6319_PMIC_DIG_WPK_KEY_H_SHIFT);
		/* re-read the current wdt setting */
		ret |= spmi_ext_register_readl(chip->spmi_dev,
				MT6319_TOP_RST_MISC, &val, 1);
		MT6319_INFO("S%d set TOP_RST_MISC=0x%x\n", chip->slave_addr, val);
		if (ret < 0) {
			MT6319_ERR("%s error\n", __func__);
		}
	}
}

#if !SPMI_NO_PMIC
static int mt6319_check_cid(void)
{
	int pidx = 0, ret = 0;
	struct mt6319_chip *chip;
	struct spmi_device *dev;
	uint8_t val = 0;

	for (pidx = 0; pidx < MT6319_MAX; pidx++) {
		chip = &mt6319_chip[pidx];
		dev = get_spmi_device(chip->master_idx, mt6319_chip[pidx].slave_addr);
		if (!dev) {
			MT6319_ERR("%s fail(%d).\n", __func__, pidx);
			return -ENODEV;
		}
		chip->spmi_dev = dev;
		ret = spmi_ext_register_readl_field(dev,
						    MT6319_PMIC_SWCID_H_ADDR,
						    &val, 1,
						    MT6319_PMIC_SWCID_H_MASK,
						    MT6319_PMIC_SWCID_H_SHIFT);
		if (ret != 0 ||
		    val != MT6319_SWCID_H_CODE) {
			MT6319_ERR("%s ret=%d val=%d\n", __func__, ret, val);
			return -EIO;
		}

		/* Print debug status */
		mt6319_dbg_status(chip, 1);
		/* Clear debug status */
		mt6319_dbg_status(chip, 0);
	}

	return 0;
}
#else
static int mt6319_check_cid(void)
{
	return -ENODEV;
}
#endif

void mt6319_dump_record_reg(struct spmi_device *dev)
{
	u8 rdata0 = 0, rdata1 = 0, rdata2 = 0, rdata3 = 0, slv_cmd_idx;
	unsigned int i, offset;

	spmi_ext_register_readl(dev, MT6319_SPMI_DEBUG_CMD0, &rdata0, 1);
	slv_cmd_idx = (rdata0 & 0xc) >> 2;
	/* log sequence, first line is the oldest, last line is the latest */
	for (i = 0; i < 4; i++) {
		offset = MT6319_SPMI_DEBUG_ADDR0 + ((slv_cmd_idx + i) % 4) * 4;
		spmi_ext_register_readl(dev, offset, &rdata0, 1);
		spmi_ext_register_readl(dev, offset + 1, &rdata1, 1);
		spmi_ext_register_readl(dev, offset + 2, &rdata2, 1);
		spmi_ext_register_readl(dev, offset + 3, &rdata3, 1);
		SPMI_CRI("Idx:%d slvid:%d Type:0x%x, [0x%x]=0x%x\n",
			(offset - MT6319_SPMI_DEBUG_ADDR0) / 4,
			dev->slvid, (rdata3 & 0x3),
			(rdata1 << 0x8) | rdata0, rdata2);
	}
}

void mt6319_all_seq_off(unsigned char en_seq_off)
{
	unsigned int pidx = 0;
	struct mt6319_chip *chip;

	for (pidx = 0; pidx < ARRAY_SIZE(mt6319_chip); pidx++) {
		chip = &mt6319_chip[pidx];
		if (!chip->spmi_dev)
			return;
		spmi_ext_register_writel(chip->spmi_dev, MT6319_PMIC_RG_SEQ_OFF_ADDR,
					 &en_seq_off, 1);
		MT6319_INFO("S%d set seq off=%d\n", chip->slave_addr, en_seq_off);
	}
}

int mt6319_set_buck_slew_rate(int slvid, uint8_t buck_id, int rrate, int frate)
{
	struct mt6319_chip *chip = mt6319_find_chip_by_slvid(slvid);

	if (!chip || !chip->spmi_dev)
		return -EINVAL;

	mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0x55,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
	mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0x43,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_MASK,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	if (rrate) {
		mt6319_write_field_byte(chip->spmi_dev,
					MT6319_PMIC_RG_BUCK_VBUCK1_SFCHG_RRATE_ADDR + 0x80 * (buck_id - 1),
					25000 / rrate / 1000 - 1,
					MT6319_PMIC_RG_BUCK_VBUCK1_SFCHG_RRATE_MASK,
					MT6319_PMIC_RG_BUCK_VBUCK1_SFCHG_RRATE_SHIFT);
	}
	if (frate) {
		mt6319_write_field_byte(chip->spmi_dev,
					MT6319_PMIC_RG_BUCK_VBUCK1_SFCHG_FRATE_ADDR + 0x80 * (buck_id - 1),
					25000 / frate / 1000 - 1,
					MT6319_PMIC_RG_BUCK_VBUCK1_SFCHG_FRATE_MASK,
					MT6319_PMIC_RG_BUCK_VBUCK1_SFCHG_FRATE_SHIFT);
	}
	mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_MASK,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	mt6319_write_field_byte(chip->spmi_dev,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_ADDR, 0,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_MASK,
			MT6319_PMIC_BUCK_TOP_WRITE_KEY_H_SHIFT);
	return 0;
}

int mt6319_spmi_probe(void)
{
	int ret = 0;

	ret = mt6319_check_cid();
	if (ret < 0)
		return ret;

	ret = mt6319_regulator_init();
	if (ret < 0)
		return ret;

	mt6319_wdt_set();
#if INIT_SETTING_VERIFIED
	mt6319_init_setting();
#endif /*INIT_SETTING_VERIFIED*/
	MT6319_INFO("%s done\n", __func__);

	return 0;
}
