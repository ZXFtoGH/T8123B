#include "dramc_common.h"
#include "sv_c_data_traffic.h"

static cal_sv_rand_args_t *psv_rand_args = NULL;

void set_psra(cal_sv_rand_args_t *psra)
{
	
psv_rand_args = psra;
}

cal_sv_rand_args_t *get_psra(void)
{
	return psv_rand_args;
}

void print_sv_args(cal_sv_rand_args_t *psra)
{
	int i;

	mcSHOW_DBG_MSG("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	print_svarg(magic);
	print_svarg(calibration_rank);
	print_svarg(calibration_channel);
	print_svarg(dram_type);
	print_svarg(datarate);
	print_svarg(dmd);
	print_svarg(mr2_value);
	print_svarg(mr3_value);
	print_svarg(mr13_value);
	print_svarg(mr12_value);
	print_svarg(mr16_value);
	print_svarg(mr18_value);
	print_svarg(mr20_value);
	print_svarg(cbt);
	print_svarg(cbt_phase);
	print_svarg(cbt_training_mode);
	print_svarg(rk0_cbt_mode);
	print_svarg(rk1_cbt_mode);
	print_svarg(cbt_autok);
	print_svarg(cbt_atk_respi);
	for (i = 0; i < 8; i++)
		mcSHOW_DBG_MSG("pat_v[%d] =0x%x\n", i, psra->pat_v[i]);
	for (i = 0; i < 8; i++)
		mcSHOW_DBG_MSG("pat_a[%d] =0x%x\n", i, psra->pat_a[i]);
	print_svarg(pat_dmv);
	print_svarg(pat_dma);
	print_svarg(pat_cs);
	print_svarg(cagolden_sel);
	print_svarg(invert_num);
	print_svarg(wl);
	print_svarg(wl_autok);
	print_svarg(wl_atk_respi);
	print_svarg(gating);
	print_svarg(gating_autok);
	print_svarg(dqsien_autok_pi_offset);
	print_svarg(dqsien_autok_early_break_en);
	print_svarg(dqsien_autok_dbg_mode_en);
	print_svarg(rddqc);
	print_svarg(low_byte_invert_golden);
	print_svarg(upper_byte_invert_golden);
	print_svarg(mr_dq_a_golden);
	print_svarg(mr_dq_b_golden);
	print_svarg(lp5_mr20_6_golden);
	print_svarg(lp5_mr20_7_golden);
	print_svarg(tx_perbit);
	print_svarg(tx_auto_cal);
	print_svarg(tx_atk_pass_pi_thrd);
	print_svarg(tx_atk_early_break);
	print_svarg(rx_perbit);
	print_svarg(rx_auto_cal);
	print_svarg(rx_atk_cal_step);
	print_svarg(rx_atk_cal_out_dbg_en);
	print_svarg(rx_atk_cal_out_dbg_sel);
	mcSHOW_DBG_MSG("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
}

u8 valid_magic(cal_sv_rand_args_t *psra)
{
	/*
	* 0x4C503435 is ASCII of "LP45"
	*/
	return (psra->magic == 0x4C503435)? 1: 0;
}

void set_type_freq_by_svargs(DRAMC_CTX_T *p,
	    cal_sv_rand_args_t *psra)
{
	if (psra->dram_type == SV_LPDDR5) {
		p->dram_type = TYPE_LPDDR5;
		switch (psra->datarate) {
		case SV_DDR6400:
			p->freq_sel = LP5_DDR6400;
			p->frequency = 3200;
			break;
		case SV_DDR6000:
			p->freq_sel = LP5_DDR6000;
			p->frequency = 3000;
			break;
		case SV_DDR5500:
			p->freq_sel = LP5_DDR5500;
			p->frequency = 2750;
			break;
		case SV_DDR4800:
			p->freq_sel = LP5_DDR4800;
			p->frequency = 2400;
			break;
		case SV_DDR4266:
			p->freq_sel = LP5_DDR4266;
			p->frequency = 2133;
			break;
		case SV_DDR3733:
			p->freq_sel = LP5_DDR3733;
			p->frequency = 1866;
			break;
		case SV_DDR3200:
			p->freq_sel = LP5_DDR3200;
			p->frequency = 1600;
			break;
		case SV_DDR2400:
			p->freq_sel = LP5_DDR2400;
			p->frequency = 1200;
			break;
		case 1600:
			p->freq_sel = LP5_DDR1600;
			p->frequency = 800;
			break;
		case SV_DDR1200:
			p->freq_sel = LP5_DDR1200;
			p->frequency = 600;
			break;
		case SV_DDR800:
			p->freq_sel = LP5_DDR800;
			p->frequency = 400;
		default:
			mcSHOW_DBG_MSG("sv frequency is NOT valid value for lp5\n");
			psra->datarate = SV_DDR800;
			p->freq_sel = LP5_DDR800;
			p->frequency = 400;
			break;
		}
	} else {
		p->dram_type = TYPE_LPDDR4;
		switch (psra->datarate) {
		case SV_DDR4266:
			p->freq_sel = LP4_DDR4266;
			p->frequency = 2133;
			break;
		case SV_DDR3733:
			p->freq_sel = LP4_DDR3733;
			p->frequency = 1866;
			break;
		case SV_DDR3200:
			p->freq_sel = LP4_DDR3200;
			p->frequency = 1600;
			break;
		case SV_DDR2667:
			p->freq_sel = LP4_DDR2667;
			p->frequency = 1333;
			break;
		case SV_DDR2400:
			p->freq_sel = LP4_DDR2400;
			p->frequency = 1200;
			break;
		case SV_DDR1866:
			p->freq_sel = LP4_DDR1866;
			p->frequency = 933;
			break;
		case SV_DDR1600:
			p->freq_sel = LP4_DDR1600;
			p->frequency = 800;
			break;
		case SV_DDR1200:
			p->freq_sel = LP4_DDR1200;
			p->frequency = 600;
			break;
		case SV_DDR800:
			p->freq_sel = LP4_DDR800;
			p->frequency = 400;
			break;
		default:
			mcSHOW_DBG_MSG("sv frequency is NOT valid value for lp4\n");
			psra->datarate = SV_DDR800;
			p->freq_sel = LP4_DDR800;
			p->frequency = 400;
			break;
		}
	}
}

