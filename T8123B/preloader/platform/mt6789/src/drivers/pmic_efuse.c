#include <typedefs.h>
#include <platform.h>
#include <pmic.h>

#define pmic_set_register_value(flagname, val) \
	pmic_config_interface(flagname##_ADDR, (val), flagname##_MASK, flagname##_SHIFT)

#define pmic_get_register_value(flagname) \
({	\
	unsigned int val = 0;	\
	pmic_read_interface(flagname##_ADDR, &val, flagname##_MASK, flagname##_SHIFT);	\
	val;	\
})

unsigned int pmic_read_efuse_nolock(int i)
{
	unsigned int efuse_data = 0;

	/* 1. enable efuse ctrl engine clock */
	pmic_set_register_value(PMIC_TOP_CKHWEN_CON0_CLR, 1 << PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	pmic_set_register_value(PMIC_TOP_CKPDN_CON0_CLR, 1 << PMIC_RG_EFUSE_CK_PDN_SHIFT);
	/* 2. */
	pmic_set_register_value(PMIC_RG_OTP_RD_SW, 1);
	/* 3. Set row to read */
	pmic_set_register_value(PMIC_RG_OTP_PA, i * 2);
	/* 4. Toggle RG_OTP_RD_TRIG */
	if (pmic_get_register_value(PMIC_RG_OTP_RD_TRIG) == 0)
		pmic_set_register_value(PMIC_RG_OTP_RD_TRIG, 1);
	else
		pmic_set_register_value(PMIC_RG_OTP_RD_TRIG, 0);
	/* 5. Polling RG_OTP_RD_BUSY = 0 */
	udelay(300);
	while (pmic_get_register_value(PMIC_RG_OTP_RD_BUSY) == 1)
		;
	/* 6. Read RG_OTP_DOUT_SW */
	udelay(100);
	efuse_data = pmic_get_register_value(PMIC_RG_OTP_DOUT_SW);
	/* 7. disable efuse ctrl engine clock */
	pmic_set_register_value(PMIC_TOP_CKHWEN_CON0_SET, 1 << PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	pmic_set_register_value(PMIC_TOP_CKPDN_CON0_SET, 1 << PMIC_RG_EFUSE_CK_PDN_SHIFT);

	return efuse_data;
}

/* for positive number division operation */
#define DIV_ROUND_CLOSE(n, d) (((n) + (d) / 2) / (d))
#define VAUX18_CONVERT_RATIO	 997898
#define VAUX18_DIVISOR		1000000

static void temp_get_vaux18(void)
{
	unsigned int VAUX18 = pmic_read_efuse_nolock(44);
	int val = 0;

	VAUX18 = (VAUX18 & 0xFE00) >> 9;
	if ((VAUX18 & 0x40) == 0)
		val = VAUX18;
	else
		val = (~(VAUX18 - 0x01)) * -1;

	pal_log_info("pmic_efuse_sw_load VAUX18 efuse=%d.\n", val);
}

/* PMIC EFUSE SW load need to check EFUSE_TABLE */
void pmic_efuse_sw_load(void)
{
	/* efuse ch0 gain/offset */
	int64_t GAIN, OFFSET;

	GAIN = pmic_read_efuse_nolock(28) & 0xFFF;
	OFFSET = pmic_read_efuse_nolock(29) & 0x7FF;

	pal_log_info("%s get GAIN=0x%llx, OFFSET=0x%llx.\n", __func__, GAIN, OFFSET);

	GAIN = (GAIN & 0x800) ? (32768 - ((~GAIN + 1) & 0xFFF)) : (32768 + GAIN);
	GAIN = (DIV_ROUND_CLOSE(GAIN * VAUX18_CONVERT_RATIO, VAUX18_DIVISOR) - 32768) & 0xFFF;

	OFFSET = (OFFSET & 0x400) ? ((~OFFSET + 1) & 0x7FF) : (OFFSET);
	OFFSET = DIV_ROUND_CLOSE(OFFSET * VAUX18_CONVERT_RATIO, VAUX18_DIVISOR) & 0x7FF;

	upmu_set_reg_value(PMIC_EFUSE_GAIN_CH0_TRIM_ADDR, (u32)GAIN);
	upmu_set_reg_value(PMIC_EFUSE_OFFSET_CH0_TRIM_ADDR, (u32)OFFSET);
	pal_log_info("%s set GAIN=0x%x(0x%llx), OFFSET=0x%x(0x%llx)\n", __func__,
		     upmu_get_reg_value(PMIC_EFUSE_GAIN_CH0_TRIM_ADDR), GAIN,
		     upmu_get_reg_value(PMIC_EFUSE_OFFSET_CH0_TRIM_ADDR), OFFSET);
	temp_get_vaux18();
}
