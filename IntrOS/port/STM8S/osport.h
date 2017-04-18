/******************************************************************************

    @file    IntrOS: osport.h
    @author  Rajmund Szymanski
    @date    18.04.2017
    @brief   IntrOS port definitions for STM8 uC.

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

#ifndef __INTROSPORT_H
#define __INTROSPORT_H

#include <osconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15);

/* -------------------------------------------------------------------------- */

#ifndef  OS_TIMER
#define  OS_TIMER             0
#endif

#if      OS_TIMER > 0
#error   osconfig.h: Incorrect OS_TIMER value! This port does not support tick-less mode.
#endif

/* -------------------------------------------------------------------------- */

#ifndef CPU_FREQUENCY
#error   osconfig.h: Undefined CPU_FREQUENCY value!
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_FREQUENCY
#define  OS_FREQUENCY      1000 /* Hz */
#else

#if     (OS_FREQUENCY > 1000)
#error   osconfig.h: Incorrect OS_FREQUENCY value!
#endif

#endif //OS_FREQUENCY

/* -------------------------------------------------------------------------- */

#ifndef  OS_STACK_SIZE
#define  OS_STACK_SIZE      128 /* default task stack size in bytes           */
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_ASSERT
#define  OS_ASSERT            0 /* don't include standard assertions          */
#endif

#if     (OS_ASSERT == 0)
#ifndef  NDEBUG
#define  NDEBUG
#endif
#endif

#include <assert.h>

/* -------------------------------------------------------------------------- */

typedef  uint8_t              stk_t;

/* -------------------------------------------------------------------------- */

#if      defined(__CSMC__)
extern   stk_t               _stack[];
#define  MAIN_TOP            _stack+1
#endif

/* -------------------------------------------------------------------------- */

#if      defined(__CSMC__)

#ifndef  __CONSTRUCTOR
#define  __CONSTRUCTOR
#endif
#ifndef  __NO_RETURN
#define  __NO_RETURN
#endif
#ifndef  __STATIC_INLINE
#define  __STATIC_INLINE      static inline
#endif
#ifndef  __WFI
#define  __WFI                wfi
#endif

__STATIC_INLINE
char _get_CC( void )
{
	return (char) _asm("push cc""\n""pop a");
}

__STATIC_INLINE
void _set_CC( char cc)
{
	(void) _asm("push a""\n""pop cc", cc);
}

#elif    defined(__SDCC)

#ifndef  __CONSTRUCTOR
#define  __CONSTRUCTOR
#endif
#ifndef  __NO_RETURN
#define  __NO_RETURN         _Noreturn
#endif
#ifndef  __STATIC_INLINE
#define  __STATIC_INLINE      static inline
#endif
#ifndef  __WFI
#define  __WFI                wfi
#endif

char _get_CC( void );
void _set_CC( char cc);

#endif

/* -------------------------------------------------------------------------- */

#define  port_get_lock()     _get_CC()
#define  port_put_lock(lck)  _set_CC(lck)

#define  port_set_lock()      disableInterrupts()
#define  port_clr_lock()      enableInterrupts()

#define  port_sys_lock()      do { char __LOCK = port_get_lock(); port_set_lock()
#define  port_sys_unlock()         port_put_lock(__LOCK); } while(0)

#define  port_isr_lock()      do { port_set_lock()
#define  port_isr_unlock()         port_clr_lock(); } while(0)

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__INTROSPORT_H
