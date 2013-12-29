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
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ws2812.h"

#define LED_R		(1<<PC6)
#define LED_R_PORT	PORTC
#define LED_R_DDR	DDRC
#define LED_R_PWM	OCR1A

#define LED_G		(1<<PC5)
#define LED_G_PORT	PORTC
#define LED_G_DDR	DDRC
#define LED_G_PWM	OCR1B

#define LED_B		(1<<PB7)
#define LED_B_PORT	PORTB
#define LED_B_DDR	DDRB
#define LED_B_PWM	OCR1C

#define BTN		(1<<PD7)
#define BTN_PORT	PORTD
#define BTN_PIN		PIND
#define BTN_DDR		DDRD

#define LED_MAX		255 //number of steps in each color, affects brightness and speed
#define LED_MAX_PWM	LED_MAX*LED_MAX

#define EFFECT_LOOP 5

uint8_t autoeffect = 1;
uint8_t effect = 0;
uint8_t effectcount = EFFECT_LOOP;
uint16_t state = 0;

int main(void)
{
	//--- Set clock speed ---//
	//Set full speed (disables CKDIV8 by registers)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;

	//--- Init button ---//
	//Has external pullup due to HWB
	EICRB |= (1<<ISC71); //trigger interrupt on falling edge
	EIMSK |= (1<<INT7); //Enable interrupt 7

	WS2812Setup();
	
	srand(20936);
	
	sei();
	
	while(1)
	{
		rand();
		switch(effect)
		{
		case 0:;
			for(char j = 0; j<WS2812_COUNT; j++)
			{
				for(char i = 0; i<WS2812_COUNT; i++)
				{
					if (i==j) WS2812SetHSV(i, i*(1535/WS2812_COUNT+1), 255, 255);
					else WS2812SetRGB(i, 0,0,0);
				}
				WS2812Send();
				_delay_ms(25);
			}
			break;
		case 1:;
			for(char l = 0; l < 2; l++)
			{
				for(char k = 0; k < 3; k++)
				{
					for(char j = 0; j<8; j++)
					{
						for(char i = 0; i<WS2812_COUNT; i++)
						{
							if (j==1 && i<((l+1)*WS2812_COUNT / 2) && i>=(l*WS2812_COUNT / 2) ) WS2812SetRGB(i, 0, 0, 255);
							else WS2812SetRGB(i, 0,0,0);
						}
						WS2812Send();
						_delay_ms(5);
					}
				}
				_delay_ms(100);
			}
			break;
		case 2:;
			uint8_t level = 0;
			for(char l = 0; l < 2; l++)
			{
				for(char j = 0; j<WS2812_COUNT; j++)
				{
					for(char i = 0; i<WS2812_COUNT; i++)
					{
						if (i==j) level = 255;
						else if ((i+1==j) || (i-1==j)) level = 127;
						else if ((i+2==j) || (i-2==j)) level = 63;
						else if ((i+3==j) || (i-3==j)) level = 31;
						else level = 0;
						
						if (l) WS2812SetRGB(i, level, 0, 0);
						else WS2812SetRGB(WS2812_COUNT-i, level, 0, 0);
					}
					WS2812Send();
					_delay_ms(20);
				}
			}
			break;
		case 3:;
			uint16_t df = (RAND_MAX / WS2812_COUNT +1);
			for(uint8_t j = 0; j < 10; j++)
			{
				WS2812Clear();
				for(uint8_t i = 0; i < WS2812_COUNT/5; i++)
				{
					WS2812SetRGB(rand() / df, 255, 255, 0);
				}
				WS2812Send();
				_delay_ms(25);
			}
			break;
		case 4:;
			uint16_t steps = (1535/WS2812_COUNT+1);
			for(char j = 0; j<WS2812_COUNT; j++)
			{
				for(char i = 0; i<WS2812_COUNT; i++)
				{
					WS2812SetHSV(i, (steps*i+steps*j)%1535, 255, 255);
				}
				WS2812Send();
				_delay_ms(10);
			}
			break;
		default:;
			effect = 0;
		}
		
		if(autoeffect)
		{
			effectcount--;
			if(!effectcount)
			{
				effectcount = EFFECT_LOOP;
				effect++;
			}
		}
		
	}
}

//--- HWB Button interrupt ---//
ISR(INT7_vect)
{
	uint8_t longpress = 0;
	while( (BTN_PIN & BTN) )
	{
		_delay_ms(10);
		if(longpress < 200)longpress++;
		else
		{
			autoeffect = !autoeffect;
		}
	}
	if (longpress < 200) effect++;
	
}
