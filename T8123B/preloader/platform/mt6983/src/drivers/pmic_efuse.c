#include <typedefs.h>
#include <platform.h>
#include <pmic.h>
#include <timer.h>

#define pmic_set_register_value(flagname, val) \
	pmic_config_interface(flagname##_ADDR, (val), flagname##_MASK, flagname##_SHIFT)
#define pmic_get_register_value(flagname) \
({	\
	unsigned int val = 0;	\
	pmic_read_interface(flagname##_ADDR, &val, flagname##_MASK, flagname##_SHIFT);	\
	val;	\
})

static u8 trig_sta;

U16 pmic_read_efuse_nolock(int i)
{
	U16 efuse_data = 0;

	/* 1. enable efuse ctrl engine clock */
	upmu_set_reg_value(PMIC_TOP_CKHWEN_CON0_CLR_ADDR, 1 << PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	upmu_set_reg_value(PMIC_TOP_CKPDN_CON1_CLR_ADDR, 1 << PMIC_RG_EFUSE_CK_PDN_SHIFT);
	upmu_set_reg_value(PMIC_RG_OTP_OSC_CK_EN_SW_ADDR, 0x3);
	/* 2. */
	upmu_set_reg_value(PMIC_RG_OTP_RD_SW_ADDR, 1);
	/* 3. Set row to read */
	upmu_set_reg_value(PMIC_RG_OTP_PA_ADDR, i * 2);
	/* 4. Toggle RG_OTP_RD_TRIG */
	trig_sta = trig_sta ? 0 : 1;
	upmu_set_reg_value(PMIC_RG_OTP_RD_TRIG_ADDR, trig_sta);
	/* 5. Polling RG_OTP_RD_BUSY = 0 */
	udelay(300);
	while (upmu_get_reg_value(PMIC_RG_OTP_RD_BUSY_ADDR) == 1)
		;
	/* 6. Read RG_OTP_DOUT_SW */
	udelay(100);
	efuse_data = upmu_get_reg_value16(PMIC_RG_OTP_DOUT_SW_L_ADDR);

	/* 7. disable efuse ctrl engine clock */
	upmu_set_reg_value(PMIC_TOP_CKHWEN_CON0_SET_ADDR, 1 << PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	upmu_set_reg_value(PMIC_TOP_CKPDN_CON1_SET_ADDR, 1 << PMIC_RG_EFUSE_CK_PDN_SHIFT);
	upmu_set_reg_value(PMIC_RG_OTP_OSC_CK_EN_SW_ADDR, 0);


	return efuse_data;
}

/* PMIC EFUSE SW load need to check EFUSE_TABLE */
void pmic_efuse_sw_load(void)
{
	int VAUX18, VBG12;
	u16 efuse_data = 0;

	efuse_data = pmic_read_efuse_nolock(118);
	pmic_config_interface(PMIC_RG_AUX_RSV0_ADDR, (efuse_data >> 8) & 0xFF,
			      PMIC_RG_AUX_RSV0_MASK,
			      PMIC_RG_AUX_RSV0_SHIFT);

	trig_sta = upmu_get_reg_value(PMIC_RG_OTP_RD_TRIG_ADDR);
	/* dump VAUX18/VBG12 TRIM value */
	VAUX18 = pmic_get_register_value(PMIC_AUXADC_EFUSE_VAUX18);
	VBG12 = pmic_get_register_value(PMIC_AUXADC_EFUSE_VBG12);
	pal_log_info("VAUX18=%d.%dmV (0x%x), VBG12=%d.%dmV (0x%x)\n",
		(VAUX18 >> 6) ? (18400 + (VAUX18 - 0x80) * 5) / 10 : (18400 + VAUX18 * 5) / 10,
		(VAUX18 >> 6) ? (18400 + (VAUX18 - 0x80) * 5) % 10 : (18400 + VAUX18 * 5) % 10,
		VAUX18,
		(VBG12 >> 6) ? (120500 + (VBG12 - 0x80) * 25) / 100 : (120500 + VBG12 * 25) / 100,
		(VBG12 >> 6) ? (120500 + (VBG12 - 0x80) * 25) % 100 : (120500 + VBG12 * 25) % 100,
		VBG12);
}
