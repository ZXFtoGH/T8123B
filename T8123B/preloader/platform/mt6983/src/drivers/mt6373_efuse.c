#include <mt6373_pmic.h>

static u8 trig_sta;

/* i = 0 ~ 127. Read efuse bit 16i ~ 16i+15 */
U16 second_pmic_read_efuse_nolock(int i)
{
	U16 efuse_data = 0;

	/* 0. Unlock */
	second_upmu_set_reg_value16(MT6373_PMIC_TMA_KEY_L_ADDR, 0x9C8C);
	/* 1. enable efuse ctrl engine clock */
	second_upmu_set_reg_value(MT6373_PMIC_TOP_CKHWEN_CON0_CLR_ADDR,
				  1 << MT6373_PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	second_upmu_set_reg_value(MT6373_PMIC_TOP_CKPDN_CON1_CLR_ADDR,
				  1 << MT6373_PMIC_RG_EFUSE_CK_PDN_SHIFT);
	second_upmu_set_reg_value(MT6373_PMIC_RG_OTP_OSC_CK_EN_SW_ADDR, 0x3);
	/* 2. */
	second_upmu_set_reg_value(MT6373_PMIC_RG_OTP_RD_SW_ADDR, 1);
	/* 3. Set row to read */
	second_upmu_set_reg_value(MT6373_PMIC_RG_OTP_PA_ADDR, i * 2);
	/* 4. Toggle RG_OTP_RD_TRIG */
	trig_sta = trig_sta ? 0 : 1;
	second_upmu_set_reg_value(MT6373_PMIC_RG_OTP_RD_TRIG_ADDR, trig_sta);
	/* 5. Polling RG_OTP_RD_BUSY = 0 */
	udelay(300);
	while (second_upmu_get_reg_value(MT6373_PMIC_RG_OTP_RD_BUSY_ADDR) == 1)
		;
	/* 6. Read RG_OTP_DOUT_SW */
	udelay(100);
	efuse_data = second_upmu_get_reg_value16(MT6373_PMIC_RG_OTP_DOUT_SW_L_ADDR);

	/* 7. disable efuse ctrl engine clock */
	second_upmu_set_reg_value(MT6373_PMIC_TOP_CKHWEN_CON0_SET_ADDR,
				  1 << MT6373_PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	second_upmu_set_reg_value(MT6373_PMIC_TOP_CKPDN_CON1_SET_ADDR,
				  1 << MT6373_PMIC_RG_EFUSE_CK_PDN_SHIFT);
	second_upmu_set_reg_value(MT6373_PMIC_RG_OTP_OSC_CK_EN_SW_ADDR, 0);
	/* 8. Lock */
	second_upmu_set_reg_value16(MT6373_PMIC_TMA_KEY_L_ADDR, 0);

	return efuse_data;
}

/* PMIC EFUSE SW load need to check EFUSE_TABLE */
void second_pmic_efuse_sw_load(void)
{
#if 0

#endif
}
