/******************************************************************************

    @file    IntrOS: osport.c
    @author  Rajmund Szymanski
    @date    01.12.2016
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

#include <oskernel.h>

/* -------------------------------------------------------------------------- */

void port_sys_init( void )
{
/******************************************************************************
 Put here configuration of system timer

 Example for TIM1 (STM32F4):

	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0xFF);
	NVIC_EnableIRQ  (TIM1_UP_TIM10_IRQn);
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	TIM1->PSC  = CPU_FREQUENCY / 10000 - 1;
	TIM1->ARR  = 10000 /  OS_FREQUENCY - 1;
	TIM1->EGR  = TIM_EGR_UG;
	TIM1->CR1  = TIM_CR1_CEN;
	TIM1->DIER = TIM_DIER_UIE;

*******************************************************************************/

	CLK->CKDIVR = 0;
	CLK->ECKR  |= CLK_ECKR_HSEEN; while ((CLK->ECKR & CLK_ECKR_HSERDY) == 0);
	CLK->SWCR  |= CLK_SWCR_SWEN;
	CLK->SWR    = 0xB4; /* HSE */ while ((CLK->SWCR & CLK_SWCR_SWBSY)  == 1);

#define LN1_(X)  ((X)&1)
#define LN2_(X)  ((X)>>1?1+LN1_((X)>>1):LN1_(X))
#define LN4_(X)  ((X)>>2?2+LN2_((X)>>2):LN2_(X))
#define LN8_(X)  ((X)>>4?4+LN4_((X)>>4):LN4_(X))
#define LEN_(X)  ((X)>>8?8+LN8_((X)>>8):LN8_(X))
#define PSC_ LEN_((CPU_FREQUENCY/OS_FREQUENCY-1)>>16)
#define ARR_      (CPU_FREQUENCY/OS_FREQUENCY/(1<<PSC_))

	TIM3->PSCR  = (PSC_);
	TIM3->ARRH  = (ARR_ - 1) >> 8;
	TIM3->ARRL  = (ARR_ - 1);
	TIM3->IER  |= TIM3_IER_UIE;
	TIM3->CR1  |= TIM3_CR1_CEN;

	enableInterrupts();
}

/* -------------------------------------------------------------------------- */

/******************************************************************************
 Put here the procedure of interrupt handler of system timer

 Example for TIM1 (STM32F4):

void TIM1_UP_TIM10_IRQHandler( void )
{
	TIM1->SR = ~TIM_SR_UIF; // clear timer's status register

	Counter++;
}

*******************************************************************************/

@interrupt
void TIM3_UPD_OVF_BRK_IRQHandler( void )
{
	TIM3->SR1= ~TIM3_SR1_UIF;

	Counter++;
}

/* -------------------------------------------------------------------------- */
