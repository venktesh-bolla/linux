/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * asm-generic/int-ll64.h
 *
 * Integer declarations for architectures which use "long long"
 * for 64-bit types.
 */

#ifndef _UAPI_ASM_GENERIC_INT_LL64_H
#define _UAPI_ASM_GENERIC_INT_LL64_H

#include <asm/bitsperlong.h>

#ifndef __ASSEMBLY__
/*
 * __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
 * header files exported to user space
 */

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

#ifdef __GNUC__
__extension__ typedef __signed__ long long __s64;
__extension__ typedef unsigned long long __u64;
#ifdef CONFIG_HAVE_128BIT_ACCESS
__extension__ typedef __int128_t __s128;
__extension__ typedef __uint128_t __u128;
#endif
#else
typedef __signed__ long long __s64;
typedef unsigned long long __u64;
#ifdef CONFIG_HAVE_128BIT_ACCESS
typedef __int128_t __s128;
typedef __uint128_t __u128;
#endif
#endif

#endif /* __ASSEMBLY__ */


#endif /* _UAPI_ASM_GENERIC_INT_LL64_H */
