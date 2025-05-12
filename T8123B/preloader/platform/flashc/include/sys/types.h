/*
* Copyright (c) 2008-2009 Travis Geiselbrecht
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files
* (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __SYS_TYPES_H
#define __SYS_TYPES_H

#ifndef __cplusplus
#define false 0
#define true 1
#define FALSE 0
#define TRUE 1
//typedef int BOOL;
#endif

#include <stdint.h>
#include <stddef.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
#ifndef NULL
#define NULL 0
#endif
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef long long     off_t;

typedef int status_t;

typedef uintptr_t addr_t;
typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;

typedef int kobj_id;

typedef uint32_t time_t;
typedef unsigned long long bigtime_t;
#define INFINITE_TIME UINT32_MAX

#define TIME_GTE(a, b) ((int32_t)((a) - (b)) >= 0)
#define TIME_LTE(a, b) ((int32_t)((a) - (b)) <= 0)
#define TIME_GT(a, b) ((int32_t)((a) - (b)) > 0)
#define TIME_LT(a, b) ((int32_t)((a) - (b)) < 0)

enum handler_return {
    INT_NO_RESCHEDULE = 0,
    INT_RESCHEDULE,
};

typedef signed long int ssize_t;
typedef __SIZE_TYPE__ size_t;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint32_t U32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;

/**	KAL for usb*/#define	KAL_FALSE	FALSE
#define	KAL_TRUE	TRUE
typedef int			kal_bool;
typedef unsigned char	kal_uint8;
typedef unsigned short	kal_uint16;
typedef unsigned int	kal_uint32;
typedef unsigned long long	kal_uint64;
typedef char			kal_int8;
typedef short			kal_int16;
typedef int			kal_int32;
typedef char			kal_char;

/*
* For mmc
*/

#ifndef s8
typedef signed char         s8;
#endif
#ifndef INT8
typedef signed char       INT8;
#endif

#ifndef uint8_t
typedef unsigned char       uint8_t;
#endif

#ifndef s16
typedef signed short        s16;
#endif
#ifndef u16
typedef unsigned short      u16;
#endif

#ifndef s32
typedef signed int          s32;
#endif
#ifndef INT32
typedef signed int          INT32;
#endif
#ifndef uint32_t
typedef unsigned int        uint32_t;
#endif
#ifndef UINT32
typedef unsigned int        UINT32;
#endif

#ifndef s64
typedef signed long long    s64;
#endif

#ifndef S8
typedef signed char         S8;
#endif
#ifndef U8
typedef unsigned char       U8;
#endif
#ifndef UINT8
typedef unsigned char      UINT8;
#endif


#ifndef S16
typedef signed short        S16;
#endif
#ifndef U16
typedef unsigned short      U16;
#endif
#ifndef UINT16
typedef unsigned short      UINT16;
#endif


#ifndef S32
typedef signed int          S32;
#endif
#ifndef U32
typedef unsigned int        U32;
#endif

#ifndef S64
typedef signed long long   S64;
#endif
#ifndef U64
typedef unsigned long long U64;
#endif



#ifndef __cplusplus
#define false 0
#define true 1
#define FALSE 0
#define TRUE 1
//typedef int BOOL;
#endif



#endif
