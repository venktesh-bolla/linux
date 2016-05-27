/*
 * Compat system call wrappers
 *
 * Copyright (C) 2012 ARM Ltd.
 * Authors: Will Deacon <will.deacon@arm.com>
 *	    Catalin Marinas <catalin.marinas@arm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/linkage.h>
#include <linux/const.h>

#include <asm/assembler.h>
#include <asm/asm-offsets.h>
#include <asm/errno.h>
#include <asm/page.h>

/*
 * System call wrappers for the AArch32 compatibility layer.
 */

ENTRY(compat_sys_sigreturn_wrapper)
	mov	x0, sp
	b	compat_sys_sigreturn
ENDPROC(compat_sys_sigreturn_wrapper)

ENTRY(compat_sys_rt_sigreturn_wrapper)
	mov	x0, sp
	b	compat_sys_rt_sigreturn
ENDPROC(compat_sys_rt_sigreturn_wrapper)

ENTRY(compat_sys_statfs64_wrapper)
	mov	w3, #84
	cmp	w1, #88
	csel	w1, w3, w1, eq
	b	compat_sys_statfs64
ENDPROC(compat_sys_statfs64_wrapper)

ENTRY(compat_sys_fstatfs64_wrapper)
	mov	w3, #84
	cmp	w1, #88
	csel	w1, w3, w1, eq
	b	compat_sys_fstatfs64
ENDPROC(compat_sys_fstatfs64_wrapper)

ENTRY(compat_sys_sync_file_range2_wrapper)
	regs_to_64	x2, x2, x3
	regs_to_64	x3, x4, x5
	b	sys_sync_file_range2
ENDPROC(compat_sys_sync_file_range2_wrapper)
