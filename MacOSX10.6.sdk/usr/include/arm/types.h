/*
 * Copyright (c) 2000-2007 Apple Inc. All rights reserved.
 */
/*
 * Copyright 1995 NeXT Computer, Inc. All rights reserved.
 */
/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)types.h	8.3 (Berkeley) 1/5/94
 */

#ifndef	_MACHTYPES_H_
#define	_MACHTYPES_H_

#ifndef __ASSEMBLER__
#include <arm/_types.h>
#include <sys/cdefs.h>
/*
 * Basic integral types.  Omit the typedef if
 * not possible for a machine/compiler combination.
 */
#ifndef _INT8_T
#define _INT8_T
typedef	__signed char		int8_t;
#endif
typedef	unsigned char		u_int8_t;
#ifndef _INT16_T
#define _INT16_T
typedef	short			int16_t;
#endif
typedef	unsigned short		u_int16_t;
#ifndef _INT32_T
#define _INT32_T
typedef	int			int32_t;
#endif
typedef	unsigned int		u_int32_t;
#ifndef _INT64_T
#define _INT64_T
typedef	long long		int64_t;
#endif
typedef	unsigned long long	u_int64_t;

typedef int32_t			register_t;

#ifndef _INTPTR_T
#define _INTPTR_T
typedef __darwin_intptr_t	intptr_t;
#endif
#ifndef _UINTPTR_T
#define _UINTPTR_T
typedef unsigned long		uintptr_t;
#endif

#if !defined(_ANSI_SOURCE) && (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
/* These types are used for reserving the largest possible size. */
typedef u_int32_t		user_addr_t;	
typedef u_int32_t		user_size_t;	
typedef int32_t			user_ssize_t;
typedef int32_t			user_long_t;
typedef u_int32_t		user_ulong_t;
typedef int32_t			user_time_t;
typedef int64_t			user_off_t;
#define USER_ADDR_NULL	((user_addr_t) 0)
#define CAST_USER_ADDR_T(a_ptr)   ((user_addr_t)((uintptr_t)(a_ptr)))


#endif /* !_ANSI_SOURCE && (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */

/* This defines the size of syscall arguments after copying into the kernel: */
typedef u_int32_t		syscall_arg_t;

#ifndef __offsetof
#define __offsetof(type, field) ((size_t)(&((type *)0)->field))
#endif

#endif /* __ASSEMBLER__ */
#endif	/* _MACHTYPES_H_ */
