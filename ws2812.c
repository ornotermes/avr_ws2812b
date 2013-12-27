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

#ifndef WS2812_c
#define WS2812_c

void WS2812Setup()
{
WS2812_DDR |= WS2812_MASK;	
}

void WS2812Set(uint16_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	WS2812Buffer[led*3] = green;
	WS2812Buffer[1+led*3] = red;
	WS2812Buffer[2+led*3] = blue;
}

void WS2812Send()
{
	for(uint16_t c = 0; c < (WS2812_COUNT * 3); c++)
	{
		for(uint8_t b = 8; b; b--)
		{
			
			if(WS2812Buffer[c] & (1<<b))
			{
				WS2812_PORT |= WS2812_MASK;
				nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
				WS2812_PORT &= ~WS2812_MASK;
				
			}
			else
			{
				WS2812_PORT |= WS2812_MASK;
				nop();
				WS2812_PORT &= ~WS2812_MASK;
				nop();nop();nop();nop();nop();nop();nop();nop();
			}
		}
	}		
}

#endif
