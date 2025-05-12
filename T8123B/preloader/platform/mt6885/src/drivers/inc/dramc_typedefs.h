#ifndef _DRAMC_TYPEDEFS_H_
#define _DRAMC_TYPEDEFS_H_

#define IMPORT  EXTERN
#ifndef __cplusplus
  #define EXTERN  extern
#else
  #define EXTERN  extern "C"
#endif
#define LOCAL     static
#define GLOBAL
#define EXPORT    GLOBAL


#define EQ        ==
#define NEQ       !=
#define AND       &&
#define OR        ||
#define XOR(A,B)  ((!(A) AND (B)) OR ((A) AND !(B)))

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef NULL
#define NULL	0
#endif

#if __ETT__

#ifndef false
#define false	0
#endif

#ifndef true
#define true	1
#endif

#endif

#ifndef BOOL
typedef unsigned char  BOOL;
#endif
#ifndef bool
typedef unsigned char       bool;
#endif

typedef signed char				S8;
typedef signed short			S16;
typedef signed int				S32;
typedef signed long long		S64;

typedef unsigned char			U8;
typedef unsigned short			U16;
typedef unsigned int			U32;
typedef unsigned long long		U64;

typedef unsigned char			US8;
typedef unsigned short			US16;
typedef unsigned int			US32;
typedef unsigned long long		US64;

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned int			u32;
typedef unsigned long long		u64;

typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned int			UINT32;
typedef unsigned short			USHORT;
typedef signed char				INT8;
typedef signed short			INT16;
typedef signed int				INT32;

typedef volatile signed char	*P_S8;
typedef volatile signed short	*P_S16;
typedef volatile signed int		*P_S32;

typedef long					LONG;
typedef unsigned char			UBYTE;
typedef short					SHORT;

typedef unsigned int			*UINT32P;
typedef volatile unsigned short	*UINT16P;
typedef volatile unsigned char	*UINT8P;
typedef unsigned char			*U8P;

typedef volatile unsigned char	*P_U8;
typedef volatile unsigned short	*P_U16;
typedef volatile unsigned int	*P_U32;
typedef unsigned long long		*P_U64;
typedef signed long long		*P_S64;

typedef unsigned int			uint;

typedef void					VOID;
typedef unsigned char			BYTE;
typedef float					FLOAT;


#if FOR_DV_SIMULATION_USED
#include <stdio.h>
#include <string.h>
#endif

#endif
