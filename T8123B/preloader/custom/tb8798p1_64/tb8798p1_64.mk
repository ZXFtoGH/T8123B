# Common
TARGET=tb8798p1_64
MTK_PLATFORM=MT6983
TARGET_BOARD_PLATFORM=MT6983
MTK_SEC_CHIP_SUPPORT=yes
MTK_SEC_USBDL=ATTR_SUSBDL_ONLY_ENABLE_ON_SCHIP
MTK_SEC_BOOT=ATTR_SBOOT_ENABLE
MTK_SEC_MODEM_AUTH=no
MTK_SEC_SECRO_AC_SUPPORT=yes
# Platform
MTK_MT6336_SUPPORT=yes
MTK_FAN5405_SUPPORT=no
MTK_EMMC_SUPPORT=yes
MTK_MT8193_SUPPORT=no
MTK_SECURITY_SW_SUPPORT=yes
MTK_SECURITY_ANTI_ROLLBACK=no
MTK_FACTORY_LOCK_SUPPORT=no
CUSTOM_SEC_AUTH_SUPPORT=no
MTK_KERNEL_POWER_OFF_CHARGING=yes
## MTK_EMMC_SUPPORT_OTP=no
MTK_COMBO_NAND_SUPPORT=no
MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION=no
MTK_BQ24160_SUPPORT=no
MTK_SEC_VIDEO_PATH_SUPPORT=no
## MTK_TINYSYS_SSPM_SUPPORT=no
MTK_UFS_OTP_SUPPORT=yes
MTK_UFS_POWP_SUPPORT=yes
CUSTOM_CONFIG_MAX_DRAM_SIZE=0x800000000
MT6360_PMU=no
MTK_UNLOCK_BAT_SUPPORT=yes
MTK_EXT_CHGDET_SUPPORT=yes
MTK_AB_OTA_UPDATER=yes
MT6360_PMIC=no
MT6360_LDO=no
## MTK_TINYSYS_MCUPM_SUPPORT=no
MTK_MT22_MODE :=H1
A60931_SUPPORT=yes
I2C_MULTI_CH_MASK = yes
MT6375_PMU=yes
MT6375_BATTERY=yes
MT6375_AUXADC=yes
# MKP
MKP_SERVICE_SUPPORT=yes
MKP_SERVICE_MAX_SIZE=0
# GZ
MTK_ENABLE_GENIEZONE=yes
MTK_ENABLE_GENIEZONE_BOOT=yes
MTK_NEBULA_VM_SUPPORT=no
MTK_CAM_SECURITY_SUPPORT = yes
MTK_CAM_GENIEZONE_SUPPORT = yes
ARM_MTE_SUPPORT=no
export MTK_PLATFORM TARGET_BOARD_PLATFORM CFG_AARCH64_BUILD MTK_MT6336_SUPPORT MTK_FAN5405_SUPPORT MTK_EMMC_SUPPORT MTK_8193_SUPPORT MTK_SECURITY_SW_SUPPORT MTK_SEC_CHIP_SUPPORT MTK_SEC_USBDL MTK_SEC_BOOT MTK_SEC_MODEM_AUTH MTK_SEC_SECRO_AC_SUPPORT CUSTOM_SEC_AUTH_SUPPORT MTK_KERNEL_POWER_OFF_CHARGING MTK_EMMC_SUPPORT_OTP MTK_COMBO_NAND_SUPPORT MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION MTK_BQ24160_SUPPORT MTK_SEC_VIDEO_PATH_SUPPORT MTK_FACTORY_LOCK_SUPPORT MTK_TINYSYS_SSPM_SUPPORT MTK_SECURITY_ANTI_ROLLBACK CUSTOM_CONFIG_MAX_DRAM_SIZE MT6370_PMU MTK_UNLOCK_BAT_SUPPORT MTK_EXT_CHGDET_SUPPORT MT6360_PMU MTK_AB_OTA_UPDATER MT6360_PMIC MT6360_LDO MTK_TINYSYS_MCUPM_SUPPORT A60931_SUPPORT I2C_MULTI_CH_MASK MT6375_PMU MT6375_BATTERY MT6375_AUXADC MKP_SERVICE_SUPPORT MKP_SERVICE_MAX_SIZE MTK_TEE_SUPPORT TRUSTONIC_TEE_SUPPORT MICROTRUST_TEE_SUPPORT MICROTRUST_TEE_LITE_SUPPORT WATCHDATA_TEE_SUPPORT MTK_GOOGLE_TRUSTY_SUPPORT TRUSTKERNEL_TEE_SUPPORT MTK_ENABLE_GENIEZONE MTK_ENABLE_GENIEZONE_BOOT MTK_NEBULA_VM_SUPPORT MTK_CAM_SECURITY_SUPPORT MTK_CAM_GENIEZONE_SUPPORT ARM_MTE_SUPPORT
