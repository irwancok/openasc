/*! \file init.c \brief Initialization routines for the motherboard
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-06
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

#define OCR0_1MS	14

/*!Initializes the USART for the communication bus*/
void init_usart(void)
{
	//Init USART to 115.2kBaud at 14.7456 MHz (communication bus)
	usart0_init(15);
	fdevopen((void*)usart0_transmit, (void*)usart0_receive_loopback);
}

/*
  * Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base.
  */
void init_timer_0(void)
{
   TCCR0A = 0;
   TIFR0  |= (1<<OCIE0A)|(1<<TOIE0);
   TIMSK0 |= (1<<TOIE0)|(1<<OCIE0A);         /* enable output compare interrupt */
   TCCR0A  = (1<<WGM01)|(1<<CS02)|(0<<CS01)|(1<<CS00); /* CTC, prescale = 1024 */
   TCNT0  = 0;
   OCR0A   = OCR0_1MS;                     /* match in aprox 1 ms,  */
}

/*!Set the direction of the ports
*/
void init_ports(void)
{
	DDRA = 0xFF;
	DDRB = 0x0F;
	DDRC = 0xFC;
	DDRD = 0x0E;
	DDRF = 0x30;
	
	//Trigger on any edge
	EICRB = (0<<ISC61) | (1<<ISC60);
	EIMSK |= (1<<INT6);
}
