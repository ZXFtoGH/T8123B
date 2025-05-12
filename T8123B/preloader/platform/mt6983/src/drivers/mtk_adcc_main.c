
/************************************************
 * BIT Operation and REG r/w
 ************************************************/
#undef  BIT
#define BIT(_bit_)                    (unsigned int)(1 << (_bit_))
#define BITS(_bits_, _val_)           ((((unsigned int) -1 >> (31 - ((1) ? _bits_))) \
& ~((1U << ((0) ? _bits_)) - 1)) & ((_val_)<<((0) ? _bits_)))
#define BITMASK(_bits_)               (((unsigned int) -1 >> (31 - ((1) ? _bits_))) \
& ~((1U << ((0) ? _bits_)) - 1))
#define GET_BITS_VAL(_bits_, _val_)   (((_val_) & (BITMASK(_bits_))) >> ((0) ? _bits_))

#define adcc_shift(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1)) << shift)

#define adcc_shift_lsb(val, bits, shift) \
	((val & (((unsigned int)0x1 << bits) - 1) << shift) >> shift)

#define adcc_read(addr)			(*(volatile unsigned int *)(unsigned long)(addr))
#define adcc_write(addr, val)	(*(volatile unsigned int *)(unsigned long)(addr) = (val))

#define adcc_write_bits(addr, bits, shift, val) \
	adcc_write(addr, ((adcc_read(addr) & ~(adcc_shift((unsigned int)0xffffffff, \
	bits, shift))) | adcc_shift(val, bits, shift)))

#define adcc_read_bits(addr, bits, shift) \
	((adcc_read(addr) >> shift) & (((unsigned int)0x1 << bits) - 1))

/************************************************
 * Register addr, offset, bits range
 ************************************************/
/* BRISKET3 base_addr */
#define ADCC_ADDR_MPSYS		0x0C110000
#define ADCC_ADDR_COMPLEX0	0x0C18C800
#define ADCC_ADDR_COMPLEX1	0x0C1AC800
#define ADCC_ADDR_CPU4		0x0C1CC800
#define ADCC_ADDR_CPU5		0x0C1DC800
#define ADCC_ADDR_CPU6		0x0C1EC800
#define ADCC_ADDR_CPU7		0x0C1FC800

/* BRISKET3 control register offset */
#define ADCC_OFFSET_P001 0x000
#define ADCC_OFFSET_P010 0x024
#define ADCC_OFFSET_P011 0x028
#define ADCC_OFFSET_P013 0x030
#define ADCC_OFFSET_P015 0x038
#define ADCC_OFFSET_P026 0x064

#define ADCC_ADDR_MPSYS_DCD			0x0C1000C4
#define ADCC_ADDR_COMPLEX0_DCD		0x0C18C100
#define ADCC_ADDR_COMPLEX0_DCD_SEL	0x0C18C104 //0:cpu0, 1:cpu1, 2:vcomplex
#define ADCC_ADDR_COMPLEX1_DCD		0x0C1AC100
#define ADCC_ADDR_COMPLEX1_DCD_SEL	0x0C1AC104 //0:cpu0, 1:cpu1, 2:vcomplex
#define ADCC_ADDR_CPU4_DCD			0x0C1C001C
#define ADCC_ADDR_CPU5_DCD			0x0C1D001C
#define ADCC_ADDR_CPU6_DCD			0x0C1E001C
#define ADCC_ADDR_CPU7_DCD			0x0C1F001C

#define ADCC_ADDR_CPU4_STALLDCM		0x0C1C0030
#define ADCC_ADDR_CPU5_STALLDCM		0x0C1D0030
#define ADCC_ADDR_CPU6_STALLDCM		0x0C1E0030
#define ADCC_ADDR_CPU7_STALLDCM		0x0C1F0030

#define ADCC_EFUSE_FLAG		0x11C10B30
#define ADCC_EFUSE_DATA		0x11C10B3C
#define ADCC_SYSRAM_BASE	0X00115000
//#define ADCC_SYSRAM_BASE	0X40115000

enum ADCC_PWR {
	DUMMY,
	ADCC_PWR_COMPLEX0,
	ADCC_PWR_COMPLEX1,
	ADCC_PWR_MPSYS,
	ADCC_PWR_CPU4,
	ADCC_PWR_CPU5,
	ADCC_PWR_CPU6,
	ADCC_PWR_CPU7,

	NR_ADCC_PWR
};

const unsigned int ADCC_addr[NR_ADCC_PWR] = {
	0,
	ADCC_ADDR_COMPLEX0,
	ADCC_ADDR_COMPLEX1,
	ADCC_ADDR_MPSYS,
	ADCC_ADDR_CPU4,
	ADCC_ADDR_CPU5,
	ADCC_ADDR_CPU6,
	ADCC_ADDR_CPU7
};

const unsigned int ADCC_DCD_addr[NR_ADCC_PWR] = {
	0,
	ADCC_ADDR_COMPLEX0_DCD,
	ADCC_ADDR_COMPLEX1_DCD,
	ADCC_ADDR_MPSYS_DCD,
	ADCC_ADDR_CPU4_DCD,
	ADCC_ADDR_CPU5_DCD,
	ADCC_ADDR_CPU6_DCD,
	ADCC_ADDR_CPU7_DCD
};

const unsigned int ADCC_stallDCM_addr[NR_ADCC_PWR] = {
	0,
	0,
	0,
	0,
	ADCC_ADDR_CPU4_STALLDCM,
	ADCC_ADDR_CPU5_STALLDCM,
	ADCC_ADDR_CPU6_STALLDCM,
	ADCC_ADDR_CPU7_STALLDCM
};

#define GET_SET_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16))
#define GET_PLL_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0x4)
#define GET_FLL_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0x8)
#define GET_DBG_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 16) + 0xc)
#define GET_DB1_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 256))
#define GET_DB2_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 256) + 0x4)
#define GET_DB3_ADDR(ii)	(ADCC_SYSRAM_BASE + ((ii) * 256) + 0x8)

#define GET_001_ADDR(ii)	(ADCC_addr[ii] + ADCC_OFFSET_P001)
#define GET_010_ADDR(ii)	(ADCC_addr[ii] + ADCC_OFFSET_P010)
#define GET_011_ADDR(ii)	(ADCC_addr[ii] + ADCC_OFFSET_P011)
#define GET_013_ADDR(ii)	(ADCC_addr[ii] + ADCC_OFFSET_P013)
#define GET_015_ADDR(ii)	(ADCC_addr[ii] + ADCC_OFFSET_P015)
#define GET_026_ADDR(ii)	(ADCC_addr[ii] + ADCC_OFFSET_P026)
#define GET_DCD_ADDR(ii)	(ADCC_DCD_addr[ii])
#define GET_DCM_ADDR(ii)	(ADCC_stallDCM_addr[ii])

/* PLL/FLL STORE */
#define ADCC_cfg_shift 17				/* 20:17 */
#define ADCC_cfg_bits 4
#define ADCC_Ack_shift 16				/* 16:16 */
#define ADCC_Ack_bits 16
#define ADCC_Duty_shift 6				/* 15:6 */
#define ADCC_Duty_bits 10
#define ADCC_CalDone_shift 5			/* 5:5 */
#define ADCC_CalDone_bits 1
#define ADCC_CalOut_shift 0				/* 4:0 */
#define ADCC_CalOut_bits 5

/* DRFC_P001_REG */
#define FllEn_shift 16					/* 16:16 */
#define FllEn_bits 1
#define SafeFreqReqOverride_shift 6		/* 6:6 */
#define SafeFreqReqOverride_bits 1

/* DRFC_P010_REG */
#define DccControlCalDone_shift 25		/* 25:25 */
#define DccControlCalDone_bits 1
#define DccControlCalout_shift 19		/* 23:19 */
#define DccControlCalout_bits 5
#define DccControlDcTarget_shift 9		/* 16:9 */
#define DccControlDcTarget_bits 8
#define DccControlForce_shift 7			/* 7:7 */
#define DccControlForce_bits 1
#define DccControlDetEnable_shift 6		/* 6:6 */
#define DccControlDetEnable_bits 1
#define DccClkShaperCalin_shift 0		/* 4:0 */
#define DccClkShaperCalin_bits 5

/* DRFC_P011_REG */
#define DccStatusIntegrator_shift 6			/* 15:6 */
#define DccStatusIntegrator_bits 10
#define DccStatusCalClkEnableAck_shift 5	/* 5:5 */
#define DccStatusCalClkEnableAck_bits 1
#define DccStatusLoopCounter_shift 0		/* 4:0 */
#define DccStatusLoopCounter_bits 5

/* DRFC_P013_REG */
#define FllInFreqOverrideVal_shift 0		/* 7:0 */
#define FllInFreqOverrideVal_bits 8

/* DRFC_P015_REG */
#define FllDccEn_shift 10					/* 10:10 */
#define FllDccEn_bits 1
#define FllDccClkShaperCalin_shift 5		/* 9:5 */
#define FllDccClkShaperCalin_bits 5

/* DRFC_P026_REG */
#define FllFreqLockDet_shift 1				/* 1:1 */
#define FllFreqLockDet_bits 1

/* DCD_CON_REG */
#define calend_shift 27						/* 27:27 */
#define calend_bits 1
#define integrator_shift 16					/* 25:16 */
#define integrator_bits 10
#define dctarget_shift 8					/* 15:8 */
#define dctarget_bits 8
#define detenable_shift 0					/* 0:0 */
#define detenable_bits 1

/* DCD_CON_REG */
#define stall_dcm_en_shift 0				/* 0:0 */
#define stall_dcm_en_bits 1

union efuse_v3 {
	struct {
		unsigned int GHB:4;        /* [3:0] */
		unsigned int BTB:4;        /* [7:4] */
		unsigned int MDATA:4;      /* [11:8] */
		unsigned int L2TQ:4;       /* [15:12] */
		unsigned int root:4;       /* [19:16] */
		unsigned int FLL_en:1;     /* [20:20] */
		unsigned int PLL_en:1;     /* [21:21] */
		unsigned int empty:6;      /* [27:22]*/
		unsigned int version:4;    /* [31:28] */
	};
	unsigned long long reg;
};

union p010_u {
	struct {
		unsigned int DccClkShaperCalin:5;       /* 4:0 */
		unsigned int DccControlCalibrate:1;     /* 5:5 */
		unsigned int DccControlDetEnable:1;     /* 6:6 */
		unsigned int DccControlForce:1;         /* 7:7 */
		unsigned int DccControlCalRate:1;       /* 8:8 */
		unsigned int DccControlDcTarget:8;      /* 16:9 */
		unsigned int DccControlRefclkSelect:1;  /* 17:17 */
		unsigned int DccControlDcdSelect:1;     /* 18:18 */
		unsigned int DccControlCalout:5;        /* 23:19 */
		unsigned int DccControlStatus:1;        /* 24:24 */
		unsigned int DccControlCalDone:1;       /* 25:25 */
		unsigned int empty:6;                   /* 31:26 */
	};
	unsigned long long reg;
};

union p015_u {
	struct {
		unsigned int empty1:5;                 /* 4:0 */
		unsigned int FllDccClkShaperCalin:5;   /* 9:5 */
		unsigned int FllDccEn:1;               /* 10:10 */
		unsigned int empty2:21;                /* 31:11 */
	};
	unsigned long long reg;
};


union set_u {
	struct {
		unsigned int DcTarget:8;        /* [7:0] */
		unsigned int DcdSelect:8;       /* [15:8] */
		unsigned int SW_nFlag:1;        /* [16:16] */
		unsigned int adcc_cfg:4;        /* [20:17] */
		unsigned int checksum:11;   /* [31:21] */
	};
	unsigned long long reg;
};


union dcc_u {
	struct {
		unsigned int CalOut:5;      /* [4:0] */
		unsigned int CalDone:1;     /* [5:5] */
		unsigned int integrator:10; /* [15:6] */
		unsigned int Ack:1;         /* [16:16] */
		unsigned int adcc_cfg:4;    /* [20:17] */
		unsigned int checksum:11;   /* [31:21] */
	};
	unsigned long long reg;
};


struct adcc_class {
	/* PLL, FLL, SET STORE */
	union set_u set;
	union dcc_u PLL;
	union dcc_u FLL;
};


union dcd_u {
	struct {
		unsigned int detenable:1;           /* 0:0 */
		unsigned int calibrate:1;           /* 1:1 */
		unsigned int empty1:2;              /* 3:2 */
		unsigned int dcd_select:4;          /* 7:4 */
		unsigned int dctarget:8;            /* 15:8  */
		unsigned int integrator:10;         /* 25:16 */
		unsigned int dcd2drfc_calsign:1;    /* 26:26 */
		unsigned int dcd2drfc_calend:1;     /* 27:27 */
		unsigned int force_clk_enable:1;    /* 28:28 */
		unsigned int sw_force_clk_enable:1; /* 29:29 */
		unsigned int refclk_sel:1;          /* 30:30 */
		unsigned int empty2:1;              /* 31:31 */
	};
	unsigned long long reg;
};

/************************************************
 * config & struct
 ************************************************/
/* #define CHECK_TIME */
#define ADCC_PLL	1
#define ADCC_FLL	2
#define FLL_DCC0	0xFFE00020
#define PLL_DCC0	0xFFE0003E
#define CHECKSUM	0x7FF
#define P010_DEF	0x10182
#define DCD_DEF		0x8000
#define DCC_DEF		0x0

#define isb()		__asm__ __volatile__ ("isb" : : : "memory")
#define dsb(type)	__asm__ __volatile__ ("dsb " #type : : : "memory");

static void disable_l2clkcg(unsigned int cpu)
{
	/* for BCPU only */
	if (cpu > 3) {
		__asm__ __volatile__ (
			"mrs x0, S3_0_C15_C1_1 \n\t"
			"orr x0, x0, #(0x1 << 36) \n\t"
			"msr S3_0_C15_C1_1 , x0\n\t"
			:::"x0","cc"
			);
	}
}

static unsigned int adcc_cali(unsigned int cpu, unsigned int shaper)
{
	unsigned int cnt;
#ifdef CHECK_TIME
	unsigned int start_cycle=0, delta=0;
#endif
	union p010_u p010;
	union dcd_u dcd;
	union set_u set;
	union dcc_u dcc;

	p010.reg = P010_DEF;
	dcd.reg = DCD_DEF;
	dcc.reg = DCC_DEF;

	adcc_write(GET_010_ADDR(cpu), p010.reg);
	adcc_write(GET_DCD_ADDR(cpu), dcd.reg);
	set.reg = adcc_read(GET_SET_ADDR(cpu));

	if (shaper == ADCC_FLL)
		adcc_write_bits(GET_DBG_ADDR(cpu), 16, 16, 0);
	else
		adcc_write_bits(GET_DBG_ADDR(cpu), 16, 0, 0);

	/* SafeFreqReqOverride = 1 */
	adcc_write_bits(GET_001_ADDR(cpu), SafeFreqReqOverride_bits, SafeFreqReqOverride_shift, 1);

	if (shaper == ADCC_FLL) {
		/* FllDccEn =1 */
		adcc_write_bits(GET_015_ADDR(cpu), FllDccEn_bits, FllDccEn_shift, 1);

		/* FllInFreqOverrideVal = d70 */
		adcc_write_bits(GET_013_ADDR(cpu), FllInFreqOverrideVal_bits, FllInFreqOverrideVal_shift, 70);

		/* FllEn = 1 */
		adcc_write_bits(GET_001_ADDR(cpu), FllEn_bits, FllEn_shift, 1);

		cnt = 0;
#ifdef CHECK_TIME
		start_cycle = adcc_read(0x10017008);
#endif
		while (!(adcc_read_bits(GET_026_ADDR(cpu), FllFreqLockDet_bits, FllFreqLockDet_shift))) {
			/* wait FllFreqLockDet = 1 , , wait 100 * 100 ns = 10us  */
			if (cnt++ > 200) {
				dcc.reg = FLL_DCC0;
				return 0x024;
			}
		}
#ifdef CHECK_TIME
		delta = adcc_read(0x10017008) - start_cycle;
		adcc_write(GET_DB1_ADDR(cpu), delta);
#endif
	} else {
		/* FllDccEn =0 */
		adcc_write_bits(GET_015_ADDR(cpu), FllDccEn_bits, FllDccEn_shift, 0);

		/* FllEn = 0 */
		adcc_write_bits(GET_001_ADDR(cpu), FllEn_bits, FllEn_shift, 0);
	}

	/* DccControlDcTarget default 0x80 */
	p010.DccControlDcTarget = set.DcTarget;

	/* DccControlForce = 0 */
	p010.DccControlForce = 0;

	//disable_l2clkcg(cpu);
	if (cpu > 3)
		adcc_write_bits(GET_DCM_ADDR(cpu), stall_dcm_en_bits, stall_dcm_en_shift, 0);

	if (set.DcdSelect == 0x0) {
		/* DccControlDcdSelect = 0 (internal)*/
		p010.DccControlDcdSelect = 0;
		if (shaper == ADCC_FLL)
			p010.DccControlRefclkSelect = 1;
		else
			p010.DccControlRefclkSelect = 0;
		p010.DccControlCalRate = 1;
	} else {
		/* DccControlDcdSelect = 1 (externel) */
		if (cpu == 1)
			adcc_write(ADCC_ADDR_COMPLEX0_DCD_SEL, 2);
		else if (cpu == 2)
			adcc_write(ADCC_ADDR_COMPLEX1_DCD_SEL, 2);

		p010.DccControlDcdSelect = 1;
		dcd.dcd_select = set.DcdSelect & 0x0F;
		dcd.dctarget = set.DcTarget;
		dcd.refclk_sel = 0;
		dcd.sw_force_clk_enable = 1;
		dcd.calibrate = 1;
		adcc_write(GET_DCD_ADDR(cpu), dcd.reg);
	}

	adcc_write(GET_010_ADDR(cpu), p010.reg);

if (set.adcc_cfg > 2) {
	/* DccControlCalibrate = 1 */
	p010.DccControlCalibrate = 1;
	adcc_write(GET_010_ADDR(cpu), p010.reg);

	cnt = 0;
#ifdef CHECK_TIME
	start_cycle = adcc_read(0x10017008);
#endif
	while (!(adcc_read_bits(GET_010_ADDR(cpu), DccControlCalDone_bits, DccControlCalDone_shift))) {
		/* wait DccControlCalDone = 1 , wait 10000 * 100 ns = 1 ms */
		if (cnt++ > 20000) {
			if (shaper == ADCC_FLL)
				dcc.reg = FLL_DCC0;
			else
				dcc.reg = PLL_DCC0;
			return 0x010;
		}
	}
#ifdef CHECK_TIME
	delta = adcc_read(0x10017008) - start_cycle;
	adcc_write(GET_DB2_ADDR(cpu), delta);
#endif
	/* store calout */
	dcc.CalOut = adcc_read_bits(GET_010_ADDR(cpu), DccControlCalout_bits, DccControlCalout_shift);
	dcc.CalDone = 1;

} else {
	if (shaper == ADCC_FLL) {
		dcc.CalOut = adcc_read_bits(GET_FLL_ADDR(cpu), ADCC_CalOut_bits, ADCC_CalOut_shift);
		adcc_write_bits(GET_015_ADDR(cpu), FllDccClkShaperCalin_bits, FllDccClkShaperCalin_shift, dcc.CalOut);
		/* DRFC_P015.FllDccEn  = 1 */
		adcc_write_bits(GET_015_ADDR(cpu), FllDccEn_bits, FllDccEn_shift, 1);
		/* DRFC_p010.DccControlForce = 1 */
		adcc_write_bits(GET_010_ADDR(cpu), DccControlForce_bits, DccControlForce_shift, 1);
	} else {
		dcc.CalOut = adcc_read_bits(GET_PLL_ADDR(cpu), ADCC_CalOut_bits, ADCC_CalOut_shift);
		adcc_write_bits(GET_010_ADDR(cpu), DccClkShaperCalin_bits, DccClkShaperCalin_shift, dcc.CalOut);
		/* DRFC_p010.DccControlForce = 1 */
		adcc_write_bits(GET_010_ADDR(cpu), DccControlForce_bits, DccControlForce_shift, 1);
	}
	dcc.CalDone = 1;
}
	/* get duty*/
	cnt = 0;
	if (set.DcdSelect == 0x0) {
		adcc_write_bits(GET_010_ADDR(cpu), DccControlDcTarget_bits, DccControlDcTarget_shift, 0x80);
		adcc_write_bits(GET_010_ADDR(cpu), DccControlDetEnable_bits, DccControlDetEnable_shift, 1);

		while (!(adcc_read_bits(GET_011_ADDR(cpu), DccStatusCalClkEnableAck_bits, DccStatusCalClkEnableAck_shift))) {
			/* wait DccStatusCalClkEnableAck = 1 , , wait 2560 * 100 ns = 256 us */
			if (cnt++ > 5120) {
				if (shaper == ADCC_FLL)
					dcc.reg = FLL_DCC0;
				else
					dcc.reg = PLL_DCC0;
				return 0x011;
			}
		}
		/* store Integrator */
		dcc.integrator = adcc_read_bits(GET_011_ADDR(cpu), DccStatusIntegrator_bits, DccStatusIntegrator_shift);

	} else {
		adcc_write_bits(GET_DCD_ADDR(cpu), dctarget_bits, dctarget_shift, 0x80);
		adcc_write_bits(GET_DCD_ADDR(cpu), detenable_bits, detenable_shift, 1);
#ifdef CHECK_TIME
		start_cycle = adcc_read(0x10017008);
#endif
		while (!(adcc_read_bits(GET_DCD_ADDR(cpu), calend_bits, calend_shift))) {
			/* wait calend = 1 , wait 2560 * 100 ns = 256 us */
			if (cnt++ > 5120) {
				if (shaper == ADCC_FLL)
					dcc.reg = FLL_DCC0;
				else
					dcc.reg = PLL_DCC0;
				return 0x011;
			}
		}
#ifdef CHECK_TIME
		delta = adcc_read(0x10017008) - start_cycle;
		adcc_write(GET_DB3_ADDR(cpu), delta);
#endif
		/* store Integrator */
		dcc.integrator = adcc_read_bits(GET_DCD_ADDR(cpu), integrator_bits, integrator_shift);
	}

	dcc.Ack = 1;
	dcc.checksum = CHECKSUM;
	dcc.adcc_cfg = set.adcc_cfg;

	if ((dcc.integrator < 507) || (dcc.integrator > 517))
		return 0x911;

	if (shaper == ADCC_FLL)
		adcc_write(GET_FLL_ADDR(cpu), dcc.reg);
	else
		adcc_write(GET_PLL_ADDR(cpu), dcc.reg);

	return 0;
}


unsigned int adcc_main(unsigned int cpu)
{
	unsigned int cnt, ret=0;
//	union efuse_v3 efuse;
	union set_u set;

//	efuse.reg = adcc_read(ADCC_EFUSE_FLAG);
	set.reg = adcc_read(GET_SET_ADDR(cpu));
/*
	if (efuse.version > 2)
		if(efuse.FLL_en == 1)
			if(efuse.MDATA != 0) {
				adcc_write(GET_FLL_ADDR(cpu), FLL_DCC0);
				adcc_write(GET_PLL_ADDR(cpu), PLL_DCC0);
				set.adcc_cfg = 0;
				goto OUT;
			}
*/
	cnt = 0;
	while(cnt < 3) {
		ret = adcc_cali(cpu, ADCC_FLL);
		if (!ret)
			break;
		cnt++;
		if (cnt == 3) {
			adcc_write_bits(GET_DBG_ADDR(cpu), 16, 16, ret);
			adcc_write(GET_FLL_ADDR(cpu), FLL_DCC0);
			set.adcc_cfg = 0;
		}
	}

	cnt = 2;
	while(cnt < 3) {
		ret = adcc_cali(cpu, ADCC_PLL);
		if (!ret)
			break;
		cnt++;
	}

OUT:
	/* Calibration finish */
	//if (cpu > 3)
	//	adcc_write_bits(GET_DCM_ADDR(cpu), stall_dcm_en_bits, stall_dcm_en_shift, 1);

	adcc_write(GET_DCD_ADDR(cpu), DCD_DEF);
	set.SW_nFlag = 1;
	adcc_write(GET_SET_ADDR(cpu), set.reg);
	return 0;
}
