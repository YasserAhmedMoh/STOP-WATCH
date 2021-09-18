/*
 * main.c
 *
 *  Created on: Sep 13, 2021
 *      Author: yasser ahmed
 */

#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"init_and_config.h"
extern unsigned char sec_tick=0 , min_tick=0 , h_tick=0 ;



int main(void)
{
	init_pin();
	interrupt_config();
	timer_config();
	while(1)
	{
		int i=0;
		for(i=0;i<6;i++)
		{
			// TO ENABLE 7-SEGMENT
			PORTA =((1<<i) & 0x3f)|(PORTA & 0X00);
			if(i==0)
			{
				// THIS TO MAKE CHANGE IN FIRST 4-PINS IN PORTC AND MAKE NO CHANGE IN LAST 4-PINS
				PORTC = (PORTC&0XF0)|((sec_tick%10)&0X0F);
			}
			else if(i==1)
			{
				PORTC = (PORTC&0XF0)|((sec_tick/10)&0X0F);
			}
			else if(i==2)
			{
				PORTC = (PORTC&0XF0)|((min_tick%10)&0X0F);
			}
			else if(i==3)
			{
				PORTC = (PORTC&0XF0)|((min_tick/10)&0X0F);
			}
			else if(i==4)
			{
				PORTC = (PORTC&0XF0)|((h_tick%10)&0X0F);
			}
			else if(i==5)
			{
				PORTC = (PORTC&0XF0)|((h_tick/10)&0X0F);
			}
			// WE MAKE DELAY 4MS TO RUN FAST IN 6 7-SEGMENT SO WE WILL SEE THE 6 WORKS
			_delay_ms(4);

		}

	}
}
