/******************************************************************************

    @file    IntrOS: osport.h
    @author  Rajmund Szymanski
    @date    28.12.2016
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

INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15);

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#define  Counter System.cnt
#define  Current System.cur

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
#define  OS_STACK_SIZE      256 /* default task stack size in bytes           */
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_ASSERT
#define  OS_ASSERT            0 /* don't include standard assertions */
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

#ifndef  __CONSTRUCTOR
#define  __CONSTRUCTOR
#endif
#ifndef  __NORETURN
#define  __NORETURN
#endif

/* -------------------------------------------------------------------------- */

#if   defined(__CSMC__)
#define  port_get_lock()     (char)_asm("push cc""\n""pop a")
#define  port_put_lock(state)      _asm("push a""\n""pop cc", (char)(state))
#elif defined (__SDCC)
char     port_get_lock();
void     port_put_lock(char state);
#endif

#define  port_set_lock()            disableInterrupts()
#define  port_clr_lock()            enableInterrupts()

#define  port_sys_lock()       do { char __LOCK = port_get_lock(); port_set_lock()
#define  port_sys_unlock()          port_put_lock(__LOCK); } while(0)

#define  port_isr_lock()       do { port_set_lock()
#define  port_isr_unlock()          port_clr_lock(); } while(0)

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__INTROSPORT_H
