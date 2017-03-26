/******************************************************************************

    @file    IntrOS: oscore.h
    @author  Rajmund Szymanski
    @date    26.03.2017
    @brief   IntrOS port file for STM8 uC.

 ******************************************************************************

    IntrOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of IntrOS distribution.

    IntrOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    IntrOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef __INTROSCORE_H
#define __INTROSCORE_H

#include <osbase.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

// task context

typedef struct __ctx ctx_t;

#if   defined(__CSMC__)

struct __ctx
{
	void   * sp;
	fun_t  * pc;

};

#elif defined(__SDCC)

struct __ctx
{
	fun_t  * pc;
	void   * sp;

};

#endif

#define _CTX_INIT() { 0, 0 }

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_init( ctx_t *ctx, stk_t *sp, fun_t *pc )
{
	ctx->sp = sp - 1;
	ctx->pc = pc;
}

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif//__INTROSCORE_H
