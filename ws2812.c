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

void WS2812Clear()
{
	for(int i = 0; i < WS2812_COUNT*3; i++) WS2812Buffer[i] = 0;
}

void WS2812SetHSV(uint16_t led, uint16_t hue, uint16_t saturation, uint16_t value)
{
	if(hue < 1536 && saturation < 256 && value < 256)
	{
		uint8_t red, green, blue;
		uint8_t min, max, inc, dec, hquot, hrem;
		
		if(saturation == 0)
		{
			WS2812SetRGB(led, value, value, value);
			return;
		}
		
		hquot = hue / 256;
		hrem = hue % 256;
		
		max = value;
		min = (value * (255 - saturation)) / 255;
		inc = (value * ((saturation * hrem) / 255)) / 255;
		dec = (value * ((saturation * (255-hrem)) / 255)) / 255;
		
		
		switch (hquot)
		{
		case 0:
			red = max;
			green = inc;
			blue = min;
			break;
		case 1:
			red = dec;
			green = max;
			blue = min;
			break;
		case 2:
			red = min;
			green = max;
			blue = inc;
			break;
		case 3:
			red = min;
			green = dec;
			blue = max;
			break;
		case 4:
			red = inc;
			green = min;
			blue = max;
			break;
		case 5:
			red = max;
			green = min;
			blue = dec;
			break;
		}
		WS2812SetRGB(led, red, green, blue);
	}
}

void WS2812SetRGB(uint16_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	WS2812Buffer[led*3] = green / WS2812_SAVE;
	WS2812Buffer[1+led*3] = red / WS2812_SAVE;
	WS2812Buffer[2+led*3] = blue / WS2812_SAVE;
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
