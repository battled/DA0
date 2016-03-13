/*
 * DA2.c
 * task 2
 * Created: 3/9/2016 9:00:36 PM
 *  Author: battled
 */ 

#define F_CPU 8000000UL	//define 8MHz clock
#include <avr/io.h>		//include std io
#include <util/delay.h>	//include delay 
//stack auto initialized in C

int main(void)
{
		DDRB = 0XFF; //MAKE PORT B OUTPUT FOR COUNTING
		DDRC = 0X31; //MAKE PC0 OUTPUTS
		while (1)
		{
			_delay_ms(250);
			PORTC ^= (1<<PC0);
			if (PORTC ==0X01)		//only count when portc0 == 1
				PORTB++;
		}
}