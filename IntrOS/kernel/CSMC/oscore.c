/******************************************************************************

    @file    IntrOS: oscore.c
    @author  Rajmund Szymanski
    @date    20.01.2017
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

#if defined(__CSMC__)

/* -------------------------------------------------------------------------- */

void core_ctx_switch( void )
{
	#asm

#if defined(__MODS__) || defined(__MODSL__)
__state:   set 0x08
__sp:      set 0x11
__top:     set 0x13
#else
__state:   set 0x08
__sp:      set 0x10
__top:     set 0x12
#endif

	push   cc
	ldw    y,  sp

_priv_tsk_save:

	ldw    x, _System
	ldw   (__sp, x), y
#if defined(__MODS__) || defined(__MODSL__)
	xref  f_core_tsk_handler
	callf f_core_tsk_handler
#else
	xref   _core_tsk_handler
	call   _core_tsk_handler
#endif
	ldw    y, x
	ldw    y, (__sp, y)
	tnzw   y
	jreq   _priv_tsk_start
	ldw    sp, y
	pop    cc
#if defined(__MODS__) || defined(__MODSL__)
	retf
#else
	ret
#endif    

_priv_tsk_start:

	ldw    y, x
	ldw    y, (__top, y)
	decw   y
	ldw    sp, y
#if defined(__MODS__) || defined(__MODSL__)
	xref.b c_x
	ld	   a, (__state, x)
	ldw	   x, (__state + 1, x)
	ld	   c_x, a
	ldw	   c_x+1, x
	callf [c_x.e]
#else
	ldw    x, (__state, x)
	call  (x)
#endif    

#if defined(__MODS__) || defined(__MODSL__)
	xdef  f_core_tsk_break
f_core_tsk_break:
#else
	xdef   _core_tsk_break
_core_tsk_break:
#endif    

	rim
	clrw   y
	jp     _priv_tsk_save

	#endasm
}

/* -------------------------------------------------------------------------- */

#endif // __CSMC__
