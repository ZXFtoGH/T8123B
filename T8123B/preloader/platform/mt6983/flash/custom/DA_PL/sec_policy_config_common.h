#ifndef _SEC_POLICY_CONFIG_COMMON_H_
#define _SEC_POLICY_CONFIG_COMMON_H_

/*Policy Mask*/
#define DL_FORBIDDEN_BIT  0
#define VFY_BIT           1
#define HASH_BIND_BIT     2

#define CREATE_POLICY_ENTRY(bind, verify, dl) ((bind << HASH_BIND_BIT) | (verify << VFY_BIT) | (dl << DL_FORBIDDEN_BIT))
#define DL_FORBIDDEN_BIT_SET(policy) ((policy >> DL_FORBIDDEN_BIT) & 0x1)
#define VFY_BIT_SET(policy) ((policy >> VFY_BIT) & 0x1)
#define HASH_BIND_BIT_SET(policy) ((policy >> HASH_BIND_BIT) & 0x1)

#define PL_SW_ID          0
#define LK_SW_ID          0
#define LOGO_SW_ID        0
#define BOOT_SW_ID        0
#define RECOVERY_SW_ID    0
#define SYSTEM_SW_ID      0
#define VENDOR_SW_ID      0
#define ODM_SW_ID         0
#define VBMETA_SW_ID      0
#define TEE1_SW_ID        0
#define TEE2_SW_ID        0
#define CACHE_SW_ID        0
#define SECRO_SW_ID        0
#define OEMKEYSTORE_SW_ID 0
#define KEYSTORE_SW_ID    0
#define USERDATA_SW_ID    0
#define MD1IMG_SW_ID      0
#define MD1DSP_SW_ID      0
#define MD1ARM7_SW_ID     0
#define MD3IMG_SW_ID      0
#define SGPT_SW_ID        0
#define PGPT_SW_ID        0
#define CAM_VPU1_SW_ID    0
#define CAM_VPU2_SW_ID    0
#define CAM_VPU3_SW_ID    0
#define EFUSE_SW_ID       0
#define SPMFW_SW_ID       0
#define MCUPMFW_SW_ID     0
#define NVRAM_SW_ID       0
#define PROTECT1_SW_ID    0
#define PROTECT2_SW_ID    0
#define PROINFO_SW_ID     0
#define NVCFG_SW_ID       0
#define PERSIST_SW_ID     0
#define ODMDTBO_SW_ID     0
#define DTBO_SW_ID        0
#define PERSISTENT_SW_ID  0
#define VBMETA_SYSTEM_SW_ID  0
#define VBMETA_VENDOR_SW_ID  0
#define SUPER_SW_ID       0

/*Image Binding Hash*/
#define PL_BINDING_HASH  "fdd62730afd983f367b267037d1668c164ab51568485ba305621cc28d6268d96"
#define LK_BINDING_HASH  0
#define LOGO_BINDING_HASH  0
#define BOOT_BINDING_HASH  0
#define RECOVERY_BINDING_HASH  0
#define SYSTEM_BINDING_HASH  0
#define VENDOR_BINDING_HASH      0
#define ODM_BINDING_HASH         0
#define VBMETA_BINDING_HASH      0
#define TEE1_BINDING_HASH 0
#define TEE2_BINDING_HASH 0
#define CACHE_BINDING_HASH 0
#define SECRO_BINDING_HASH 0
#define OEMKEYSTORE_BINDING_HASH 0
#define KEYSTORE_BINDING_HASH 0
#define USERDATA_BINDING_HASH 0
#define MD1IMG_BINDING_HASH 0
#define MD1DSP_BINDING_HASH 0
#define MD1ARM7_BINDING_HASH 0
#define MD3IMG_BINDING_HASH 0
#define SGPT_BINDING_HASH 0
#define PGPT_BINDING_HASH 0
#define CAM_VPU1_BINDING_HASH 0
#define CAM_VPU2_BINDING_HASH 0
#define CAM_VPU3_BINDING_HASH 0
#define EFUSE_BINDING_HASH 0
#define SPMFW_BINDING_HASH 0
#define MCUPMFW_BINDING_HASH 0
#define NVRAM_BINDING_HASH       0
#define PROTECT1_BINDING_HASH    0
#define PROTECT2_BINDING_HASH    0
#define PROINFO_BINDING_HASH     0
#define NVCFG_BINDING_HASH       0
#define PERSIST_BINDING_HASH     0
#define ODMDTBO_BINDING_HASH     0
#define DTBO_BINDING_HASH        0
#define PERSISTENT_BINDING_HASH  0
#define VBMETA_SYSTEM_BINDING_HASH  0
#define VBMETA_VENDOR_BINDING_HASH  0
#define SUPER_BINDING_HASH       0

/*Image Binding Hash*/
#define  CUST1_BINDING_HASH  		0
#define  CUST2_BINDING_HASH 		0

/***********************************
CUSTOM IMG Config
************************************/
#define  CUST1_IMG_NAME        ""
#define  CUST2_IMG_NAME        ""

#endif
