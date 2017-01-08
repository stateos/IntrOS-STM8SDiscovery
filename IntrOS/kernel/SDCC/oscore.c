/******************************************************************************

    @file    IntrOS: oscore.c
    @author  Rajmund Szymanski
    @date    24.11.2016
    @brief   IntrOS port file for STM8.

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

#include <stddef.h>
#include <os.h>

#if defined(__SDCC)

/* -------------------------------------------------------------------------- */

void core_ctx_switch( void )
{
	__asm

#define __state 0x08
#define __sp    0x10
#define __top   0x12

	push   cc
	ldw    y,  sp

_priv_tsk_save:

	ldw    x, _System
	ldw   (__sp, x), y
	.globl _core_tsk_handler
	call  _core_tsk_handler
	ldw    y, x
	ldw    y, (__sp, y)
	tnzw   y
	jreq  _priv_tsk_start
	ldw    sp, y
	pop    cc
	ret

_priv_tsk_start:

	ldw    y, x
	ldw    y, (__top, y)
	decw   y
	ldw    sp, y
	ldw    x, (__state, x)
	call  (x)

	.globl _core_tsk_break
_core_tsk_break:

	rim
	clrw   y
	jp    _priv_tsk_save

	.globl _port_get_lock
_port_get_lock:

	push   cc
	pop    a
	ret

	.globl _port_put_lock
_port_put_lock:

	push   a
	pop    cc
	ret

	__endasm;
}

/* -------------------------------------------------------------------------- */

#endif // __SDCC
