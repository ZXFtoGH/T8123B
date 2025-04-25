#ifndef __UPM6915_HEADER__
#define __UPM6915_HEADER__

/* Register 00h */
#define UPM6915_REG_00                  0x00
#define REG00_TMR_RST_MASK              0x80
#define REG00_TMR_RST_SHIFT             7
#define REG00_TMR_RST_CMD               1

#define REG00_OTG_STAT_MASK             0x80
#define REG00_OTG_STAT_SHIFT            7
#define REG00_OTG_PIN_LOW               0
#define REG00_OTG_PIN_HIGH              1

#define REG00_EN_STAT_MASK              0x40
#define REG00_EN_STAT_SHIFT             6
#define REG00_EN_STAT_ENABLE            1
#define REG00_EN_STAT_DISABLE           0

#define REG00_CHG_STAT_MASK             0x30
#define REG00_CHG_STAT_SHIFT            4

#define REG00_BOOST_MASK                0x08
#define REG00_BOOST_SHIFT               3
#define REG00_BOOST_ENABLE              1
#define REG00_BOOST_DISABLE             0

#define REG00_FAULT_MASK                0x07
#define REG00_FAULT_SHIFT               0

/* Register 01h */
#define UPM6915_REG_01                  0x01
#define REG01_IIN_LIMIT_1_MASK          0xC0
#define REG01_IIN_LIMIT_1_SHIFT         6

#define UPM6915_IINDPM_MIN              100
#define UPM6915_IINDPM_MAX              3000

#define REG01_VLOWV_MASK                0x30
#define REG01_VLOWV_SHIFT               4
#define REG01_VLOWV_MIN                 3400
#define REG01_VLOWV_MAX                 3700
#define REG01_VLOWV_3P4V                0
#define REG01_VLOWV_3P5V                1
#define REG01_VLOWV_3P6V                2
#define REG01_VLOWV_3P7V                3

#define REG01_TE_MASK                   0x08
#define REG01_TE_SHIFT                  3
#define REG01_TE_ENABLE                 1
#define REG01_TE_DISABLE                0

#define REG01_NCE_MASK                  0x04
#define REG01_NCE_SHIFT                 2
#define REG01_NCE_ENABLE                0
#define REG01_NCE_DISABLE               1

#define REG01_HZ_MODE_MASK              0x02
#define REG01_HZ_MODE_SHIFT             1
#define REG01_HZ_MODE_ENABLE            1
#define REG01_HZ_MODE_DISABLE           0

#define REG01_OPA_MODE_MASK             0x01
#define REG01_OPA_MODE_SHIFT            0
#define REG01_OPA_MODE_CHARGE           0
#define REG01_OPA_MODE_BOOST            1

#define UPM6915_VOREG_MIN               4000
#define UPM6915_VOREG_MAX               4800

/* Register 02h */
#define UPM6915_REG_02                  0x02
#define REG02_VOREG_MASK                0xFC
#define REG02_VOREG_SHIFT               2

#define REG02_OPA_MODE_2_MASK           0x03
#define REG02_OPA_MODE_2_SHIFT          0
#define REG02_OPA_MODE_2_CHG            0
#define REG02_OPA_MODE_2_OTG            1

#define REG02_OTG_PL_MASK               0x02
#define REG02_OTG_PL_SHIFT              1
#define REG02_OTG_PL_LOW                0
#define REG02_OTG_PL_HIGH               1

#define REG02_OTG_PC_MASK               0x01
#define REG02_OTG_PC_SHIFT              0
#define REG02_OTG_PC_DISABLE            0
#define REG02_OTG_PC_ENABLE             1

/* Register 03h */
#define UPM6915_REG_03                  0x03
#define REG03_VENDER_MASK               0xE0
#define REG03_VENDER_SHIFT              5
#define REG03_VENDER_CODE               5

#define REG03_PN_MASK                   0x18
#define REG03_PN_SHIFT                  3
#define REG03_PN_CODE                   0

#define REG03_REVISION_MASK             0x07
#define REG03_REVISION_SHIFT            0

/* Register 04h */
#define UPM6915_REG_04                  0x04
#define REG04_RESET_MASK                0x80
#define REG04_RESET_SHIFT               7
#define REG04_RESET_CHARGE              1

#define REG04_ICHG_MASK                 0x70
#define REG04_ICHG_SHIFT                4

#define REG04_ICHRG_OFFSET_MASK         0x08
#define REG04_ICHRG_OFFSET_SHIFT        3
#define REG04_ICHRG_OFFSET_50MA         0
#define REG04_ICHRG_OFFSET_650MA        1

#define UPM6915_ICHG_MIN                50
#define UPM6915_ICHG_MAX                3780
#define UPM6915_ICHG_OFFSET_MA          650
#define UPM6915_ICHG_LSB                100

#define REG04_ITERM_MASK                0x07
#define REG04_ITERM_SHIFT               0
#define REG04_ITERM_LSB                 50
#define UPM6915_ITERM_MIN               25
#define UPM6915_ITERM_MAX               350


/* Register 05h */
#define UPM6915_REG_05                  0x05
#define REG05_ICHG_MASK                 0xC0
#define REG05_ICHG_SHIFT                6

#define REG05_LOW_CHG_MASK              0x20
#define REG05_LOW_CHG_SHIFT             5
#define REG05_LOW_CHG_BY_ICHG           0
#define REG05_LOW_CHG_FORCE_550MA       1

#define REG05_DPM_STATUS_MASK           0x10
#define REG05_DPM_STATUS_SHIFT          4
#define REG05_DPM_STATUS_NOT_ACTIVE     0
#define REG05_DPM_STATUS_ACTIVE         1

#define REG05_CD_STATUS_MASK            0x08
#define REG05_CD_STATUS_SHIFT           3
#define REG05_CD_STATUS_LOW             0
#define REG05_CD_STATUS_HIGH            1

#define REG05_VINDPM_MASK               0x07
#define REG05_VINDPM_SHIFT              0
#define UPM6915_VINDPM_MIN            	4200
#define UPM6915_VINDPM_MAX            	14360
#define UPM6915_VINDPM_LSB            	80
#define UPM6915_VINDPM_BASE            	4200

/* Register 06h */
#define UPM6915_REG_06                  0x06
#define REG06_IMCHRG_MASK               0xF0
#define REG06_IMCHRG_SHIFT              4

#define UPM6915_IMCHG_MIN               50
#define UPM6915_IMCHG_MAX               3780

#define REG06_VMREG_MASK                0x0F
#define REG06_VMREG_SHIFT               0
#define UPM6915_VMREG_MIN               4200
#define UPM6915_VMREG_MAX               4800
#define UPM6915_VMREG_BASE              4200
#define UPM6915_VMREG_LSB               40

/* Register 07h */
#define UPM6915_REG_07                  0x07
#define REG07_VINDPM_MASK               0xF0
#define REG07_VINDPM_SHIFT              4

#define REG07_EN_ILIM2_MASK             0x08
#define REG07_EN_ILIM2_SHIFT            3
#define REG07_EN_ILIM2_DISABLE          0
#define REG07_EN_ILIM2_ENABLE           1

#define REG07_IIN_LIMIT_2_MASK          0x07
#define REG07_IIN_LIMIT_2_SHIFT         0

/* Register 08h */
#define UPM6915_REG_08                  0x08
#define REG08_BST_ILIM_MASK             0xC0
#define REG08_BST_ILIM_SHIFT            6
#define REG08_BST_ILIM_500MA            0
#define REG08_BST_ILIM_1200MA           1
#define REG08_BST_ILIM_2000MA           2
#define REG08_BST_ILIM_2400MA           3

#define REG08_VRECHG_TH_MASK            0x20
#define REG08_VRECHG_TH_SHIFT           5
#define REG08_VRECHG_TH_120MV           0
#define REG08_VRECHG_TH_210MV           1

#define REG08_VBUS_OVP_MASK             0x18
#define REG08_VBUS_OVP_SHIFT            3
#define REG08_VBUS_OVP_6P5V             0
#define REG08_VBUS_OVP_9V               1
#define REG08_VBUS_OVP_11V              2
#define REG08_VBUS_OVP_14V              3

#define REG08_THERMAL_REG_MASK          0x04
#define REG08_THERMAL_REG_SHIFT         2
#define REG08_THERMAL_REG_90C           0
#define REG08_THERMAL_REG_120C          1

#define REG08_OTG_OC_MODE_MASK          0x02
#define REG08_OTG_OC_MODE_SHIFT         1
#define REG08_OTG_OC_MODE_REGULATION    0
#define REG08_OTG_OC_MODE_TRIG_FAULT    1

#define REG08_BST_UVLO_TH_MASK          0x01
#define REG08_BST_UVLO_TH_SHIFT         0
#define REG08_BST_UVLO_TH_2P5V          0
#define REG08_BST_UVLO_TH_3V            1

/* Register 09h */
#define UPM6915_REG_09                  0x09
#define REG09_DP_VSET_MASK              0xC0
#define REG09_DP_VSET_SHIFT             6

#define REG09_DM_VSET_MASK              0x30
#define REG09_DM_VSET_SHIFT             4

#define REG09_DPDM_VSET_HIZ             0
#define REG09_DPDM_VSET_0V              1
#define REG09_DPDM_VSET_0P6V            2
#define REG09_DPDM_VSET_3P3V            3

#define REG09_HV_DET_EN_MASK            0x08
#define REG09_HV_DET_EN_SHIFT           3
#define REG09_HV_DET_DISABLE            0
#define REG09_HV_DET_ENABLE             1

#define REG09_OREG_STATUS_MASK          0x04
#define REG09_OREG_STATUS_SHIFT         2
#define REG09_OREG_STATUS_NOT_CV        0
#define REG09_OREG_STATUS_IN_CV         1

#define REG09_THERMAL_STATUS_MASK       0x02
#define REG09_THERMAL_STATUS_SHIFT      1
#define REG09_THERMAL_STATUS_NO_THERM   0
#define REG09_THERMAL_STATUS_IN_THERM   1

#define REG09_EN_HTOL_MASK              0x01
#define REG09_EN_HTOL_SHIFT             0
#define REG09_EN_HTOL_NORMAL            0
#define REG09_EN_HTOL_32S_HC            1

enum chip_mark {
    CHIP_MARK_UPM6915,
    CHIP_MARK_UPM6915D,
};

enum chrg_stat {
    CHARGE_STATUS_READY = 0,
    CHARGE_STATUS_PROGRESS,
    CHARGE_STATUS_DONE,
    CHARGE_STATUS_FAULT,
};

#endif /* __UPM6915_HEADER__ */
