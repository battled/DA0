/*
 * DA2.c
 * task3 toggle C0, C4, C5, count on Portb
 * Created: 3/9/2016 9:00:36 PM
 *  Author: battled
 */ 

#define F_CPU 8000000UL	//define 8MHz clock
#include <avr/io.h>
#include <util/delay.h>
//stack auto initialized in C

int main(void)
{
		int i=0;		//used to control port 9 and 10
		DDRB = 0XFF; //MAKE PORT B OUTPUT FOR COUNTING
		DDRC = 0X31; //MAKE PC5, PC4, PC0 OUTPUTS
		while (1)
		{
			_delay_ms(250);
			PORTC ^= (1<<PC0);
			//must account for each state portc may be in due to flipping of bits portc4,5
			if ( (PORTC ==0X01 ) || (PORTC == 0X11 ) || (PORTC == 0X21) || (PORTC == 0X31)) 
			{
				PORTB++;
				i++;
				if (i==5)
					PORTC ^= (1<<PC4); //(0x10); flip portc4
				else if (i ==10)
				{
					PORTC ^= (1<<PC4)|(1<<PC5); //0x30 flip portc4,5
					i = 0;
				}
			}
		}
}