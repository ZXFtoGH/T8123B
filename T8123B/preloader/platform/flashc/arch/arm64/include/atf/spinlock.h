/*
 * Copyright (c) 2013-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

//#ifndef ASSEMBLY 
#if 0
#include <stdint.h>

typedef struct spinlock {
	volatile uint32_t lock;
} spinlock_t;

void spin_lock(spinlock_t *lock);
void spin_unlock(spinlock_t *lock);
#if defined(MTK_PLAT_PORTING_LAYER)
int spin_trylock(spinlock_t *lock);
#endif

#else

/* Spin lock definitions for use in assembly */
#define SPINLOCK_ASM_ALIGN	2
#define SPINLOCK_ASM_SIZE	4

#endif

#endif /* __SPINLOCK_H__ */
