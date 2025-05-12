/*
 * Copyright (c) 2023, MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * MediaTek Inc. and/or its licensors.
 * Except as otherwise provided in the applicable licensing terms with
 * MediaTek Inc. and/or its licensors, any reproduction, modification, use or
 * disclosure of MediaTek Software, and information contained herein, in whole
 * or in part, shall be strictly prohibited.
 */
#include "typedefs.h"
#include "platform.h"

#define WDT_SWSYSRST       (RGU_BASE + 0x018)
#define RGU_KEY_CODE       (0x88 << 24)
#define CONNSYS_CPU_SW_RST (0x1 << 12)

int connsys_init(void)
{
    unsigned int temp;

    /* force assert to avoid Connsys MCU start running unexpectedly */
    /* CONNSYS MCU reset */
    temp = DRV_Reg32(WDT_SWSYSRST);
    DRV_WriteReg32(WDT_SWSYSRST, (temp | CONNSYS_CPU_SW_RST | RGU_KEY_CODE));

    return 0;
}
