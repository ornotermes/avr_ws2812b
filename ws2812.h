/*
	Copyright (c) 2013 Rikard Lindstrom <ornotermes@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <avr/delay.h>
#include "ws2812.h"

uint8_t state = 0;

int main(void)
{
	//--- Set clock speed ---//
	//Set full speed (disables CKDIV8 by registers)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;

	DDRB = 1;	
	
	while(1)
	{
		for(char i = 0; i<60; i++)
		{
			if (i==state) WS2812Set(i, i*4,0,255-(4*i));
			else WS2812Set(i, 0,0,0);
		}
		WS2812Send();
		_delay_ms(10);
		state++;
		if (state==60) state=0;		
	}
}
#ifndef WS2812_h
#define WS2812_h

#include <avr/io.h>


#define WS2812_COUNT	60
#define WS2812_PORT		PORTB
#define WS2812_DDR		DDRB
#define WS2812_MASK		(1<<PB0)
#define nop() __asm__ __volatile__ ("nop")
uint8_t WS2812Buffer[(WS2812_COUNT * 3)];

void WS2812Set(uint16_t led, uint8_t red, uint8_t green, uint8_t blue);
void WS2812Send(void);

#include "ws2812.c"

#endif
