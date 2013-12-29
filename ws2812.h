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

#ifndef WS2812_h
#define WS2812_h

#include <avr/io.h>
#include <stdlib.h>


#define WS2812_COUNT	30
#define WS2812_PORT		PORTC
#define WS2812_DDR		DDRC
#define WS2812_MASK		(1<<PC7)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define nop() __asm__ __volatile__ ("nop")
uint8_t WS2812Buffer[(WS2812_COUNT * 3)];

void WS2812Setup(void);
void WS2812Clear(void);
void WS2812SetRGB(uint16_t led, uint8_t red, uint8_t green, uint8_t blue);
void WS2812Send(void);

#include "ws2812.c"

#endif
