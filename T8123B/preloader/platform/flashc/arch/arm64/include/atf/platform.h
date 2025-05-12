/*
 * Copyright (c) 2013-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_H
#define PLATFORM_H


/*******************************************************************************
 * Optional common functions (may be overridden)
 ******************************************************************************/
void plat_error_handler(int err) __dead2;
void plat_panic_handler(void) __dead2;



#endif /* PLATFORM_H */
