/*
 * init_and_config.c
 *
 *  Created on: Sep 13, 2021
 *      Author: yasser ahmed
 */
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define SET_BIT(reg,pin) reg|=(1<<pin)
#define CLEAR_BIT(reg,pin) reg&=~(1<<pin)
#define TOGGLE_BIT(reg,pin) reg^=(1<<pin)
unsigned char sec_tick, min_tick , h_tick ;


void init_pin()
{
	// 4 PINS CONNECTED TO 7447 DECODER
	// CONFIGURE IT AS OUTOUT
	SET_BIT(DDRC,0);
	SET_BIT(DDRC,1);
	SET_BIT(DDRC,2);
	SET_BIT(DDRC,3);
	// 6-PINS IN PORTA WORD AS ENABLE/DISABLE PINS
	// CONFIGURE IT AS OUTOUT
	SET_BIT(DDRA,0);
	SET_BIT(DDRA,1);
	SET_BIT(DDRA,2);
	SET_BIT(DDRA,3);
	SET_BIT(DDRA,4);
	SET_BIT(DDRA,5);
	// PORTD PIN 2 LET IT AS INPUT
	// IT IS FOR INT0 TO WORK AS RESET
	// WE USE INTERNAL PULL UP RESISTER
	CLEAR_BIT(DDRD,2);
	SET_BIT(PORTD,2);
	// PORTD PIN 3 LET IT AS INPUT
	// IT IS FOR INT0 TO WORK AS PAUSED
	// WE USE PULL DOWN RESISTER
	CLEAR_BIT(DDRD,3);
	// PORTB PIN 2 LET IT AS INPUT
	// IT IS FOR INT0 TO WORK AS RESUMED
	// WE USE INTERNAL PULL UP RESISTER
	CLEAR_BIT(DDRB,2);
	SET_BIT(PORTB,2);
}
void interrupt_config()
{
		// Configure interrupt
		// AT FIRST DIABLE INRERRUPT BY CLEARING I-BIT
		CLEAR_BIT(SREG,7);
		// ENABLE EXTERNAL INTERRUPT
		SET_BIT(GICR,5);
		SET_BIT(GICR,6);
		SET_BIT(GICR,7);
		// DEFINE LEVEL TRIGGER
		// INT0 TRIGGER AT FALLING EDGE ISC01 -> 1
		// INT1 TRIGGER AT RISING EDGEC	ISC10 & ISC11 -> 1
		MCUCR = (1<<ISC01)|(1<<ISC10)|(1<<ISC11);
		// INT2 TRIGGER AT FALLING EDGE ISC2 -> 1
		MCUCSR = (1<<ISC2);
		// ENABLE I-BIT
		SET_BIT(SREG,7);
}
void timer_config()
{
	// Fcpu=1 MEGA AND WE USE PRESCALER WITH 1024
		// SO Ftimer = Fcpu/prescaler = 1*10^6 / 1024 = 977
		// THEN Ttimer= 1.02*10^-3   so for 1sec we need 977 -> OCR1A =977
		SET_BIT(TCCR1B,CS10);
		SET_BIT(TCCR1B,CS12);
		// START THE TIMER
		SET_BIT(TCNT1,1);
		OCR1A = 977;
		SET_BIT(TIMSK,OCIE1A);
		// timer1 Configuration WITH CTC MODE
		SET_BIT(TCCR1B,WGM12);
}


ISR(INT0_vect)
{
	TCNT1=0;
	sec_tick=min_tick=h_tick=0;
}
ISR(INT1_vect)
{
	// clear cs10,sc11,sc12 TO STOP CLOCK
	CLEAR_BIT(TCCR1B,CS10);
	CLEAR_BIT(TCCR1B,CS11);
	CLEAR_BIT(TCCR1B,CS12);
}
ISR(INT2_vect)
{
	 SET_BIT(TCCR1B,CS10);
	 SET_BIT(TCCR1B,CS12);
}
ISR(TIMER1_COMPA_vect)
{
	sec_tick++;
	if(sec_tick==60)
	{
		sec_tick=0;
		min_tick++;
		if(min_tick==60)
		{
			min_tick=0;
			h_tick++;
			if(h_tick==24)
			{
				h_tick=0;
			}
		}
	}
}

