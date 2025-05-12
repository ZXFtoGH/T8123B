#include "platform.h"
#include "sec_devinfo.h"
#include "dramc_pi_api.h"
#include "emi.h"

#include "mtk_vcore_opp.h"
#include <dconfig_env.h>


static int vcore_uv_table[VCORE_OPP_NUM];

void set_vcore_uv_table(int vcore_opp, int vcore_uv)
{
	vcore_uv_table[vcore_opp] = vcore_uv;
}

int get_vcore_uv_table(int vcore_opp)
{
	return vcore_uv_table[vcore_opp];
}

unsigned int get_vcore_opp_volt(unsigned int ddr_type, unsigned int opp)
{
	return get_vcore_uv_table(opp);
}

void dvfsrc_opp_level_mapping(void)
{
	int vcore_opp_0_uv, vcore_opp_1_uv;
	int vcore_opp_2_uv, vcore_opp_3_uv;
	int ptpod0 = 0;
	int skip = 1;
	int ct_enable = 0;
#if MTK_DOE_CONFIG_ENV_SUPPORT
	char *doe_vcore;
	int dvfs_v_mode = 0;

	/* todo: remove when LP4 default enable */
	doe_vcore = dconfig_getenv("dvfs_v_mode");
	if (doe_vcore) {
		dvfs_v_mode = atoi(doe_vcore);
		print("Vcore dvfs_v_mode : %d\n", dvfs_v_mode);
	}
#endif
	print("%s: PTPOD0: 0x%x (ct_enable:%d, skip:%d)\n",
		__func__, ptpod0, ct_enable, skip);

	if (ct_enable) {
		/* no ct */
	} else if (skip == 1) {
		/* bypass efuse, allow dvfs */
		vcore_opp_0_uv = 725000;
		vcore_opp_1_uv = 650000;
		vcore_opp_2_uv = 600000;
		vcore_opp_3_uv = 575000;
		print("%s: skip use default vcore_opp_uv: %d, %d, %d, %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
	} else {
		/* can not dvfs */
		vcore_opp_0_uv = 725000;
		vcore_opp_1_uv = 650000;
		vcore_opp_2_uv = 600000;
		vcore_opp_3_uv = 575000;

		print("%s: disabled vcore_opp_uv: %d, %d, %d, %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
	}
#if MTK_DOE_CONFIG_ENV_SUPPORT
	if (dvfs_v_mode == 3) { /*LV */

	}else if (dvfs_v_mode == 1) { /*HV */

	}
#endif
	print("%s: final vcore_opp_uv: %d, %d, %d, %d\n", __func__,
			vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);

	set_vcore_uv_table(VCORE_OPP_0, vcore_opp_0_uv);
	set_vcore_uv_table(VCORE_OPP_1, vcore_opp_1_uv);
	set_vcore_uv_table(VCORE_OPP_2, vcore_opp_2_uv);
	set_vcore_uv_table(VCORE_OPP_3, vcore_opp_3_uv);
}

