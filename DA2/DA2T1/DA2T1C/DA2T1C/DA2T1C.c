/*
 * DA2.c
 * task 1
 * Created: 3/9/2016 9:00:36 PM
 *  Author: battled
 */ 

#define F_CPU 8000000UL	//define 8MHz clock
#include <avr/io.h>		//include stdio
#include <util/delay.h> //include delay subroutine
//stack auto initialized in C

int main(void)
{
		DDRC = 0X01; //MAKE PC0 OUTPUT
		while (1)
		{
			//toggle portc every 250 ms for .5 second period
			_delay_ms(250);
			PORTC ^= (1<<PC0);
		}
}