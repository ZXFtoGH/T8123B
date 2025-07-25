MTK_SEC_BOOT=ATTR_SBOOT_DISABLE

HW_INIT_ONLY :=
CFG_BATTERY_DETECT :=0
CFG_EMERGENCY_DL_SUPPORT :=0
CFG_RAM_CONSOLE :=0
CFG_UART_TOOL_HANDSHAKE :=0
CFG_USB_TOOL_HANDSHAKE :=0
CFG_USB_DOWNLOAD :=1
CFG_PMT_SUPPORT :=0

CFG_LOAD_SLT_MD_RAMDISK :=1
CFG_LOAD_SLT_MD_DSP :=1
CFG_LOAD_SLT_MD :=1
CFG_LOAD_SLT_SCP :=0
CFG_LOAD_SLT_MD32 :=0
CFG_LOAD_SLT_SSPM :=1
CFG_LOAD_SLT_PICACHU :=0
CFG_LOAD_SLT_MCUPM :=0
CFG_LOAD_AP_ROM :=1
CFG_LOAD_UBOOT :=0
CFG_LOAD_MD_ROM :=1

CFG_LOAD_CONN_SYS :=0
CFG_APWDT_DISABLE :=1
CFG_ATF_SUPPORT :=0
CFG_BYPASS_LOAD_IMG_FORCE_ATF :=0
CFG_LOAD_BL2_EXT :=0

# boot to aarch64 CTP
#CFG_UBOOT_MEMADDR := 0x40005000
CFG_AP_ROM_MEMADDR := 0x40005000
CFG_AP_ROM_ARM64 := 1

MTK_VB_FLAVOR_LOAD=yes
MTK_SECURITY_SW_SUPPORT=no
MTK_TINYSYS_SSPM_SUPPORT=yes
MTK_TINYSYS_MCUPM_SUPPORT=no
MTK_SLT_SHOW_SEGMENT=yes
MTK_ENABLE_GENIEZONE=no
MTK_ENABLE_GENIEZONE_BOOT=no
MTK_NEBULA_VM_SUPPORT=no

