/*
 * DA2.c
 * task 4 task 3 w/ interrupts
 * Created: 3/9/2016 9:00:36 PM
 *  Author: battled
 */ 

#define F_CPU 8000000UL	//define 8MHz clock
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//stack auto initialized in C

ISR(PCINT1_vect)
{

	static int i=0; //declare i as static int to prevent re-initialization every interrupt
	//enter if statement if pc0 == 1
	if ( (PORTC ==0X01 ) || (PORTC == 0X11 ) || (PORTC == 0X21) || (PORTC == 0X31))
{
	PORTB++;
	i++;
	if (i==5)
	PORTC ^= (1<<PC4); //(0x10);	//flip portc4
	else if (i ==10)
	{
		PORTC ^= (1<<PC4)|(1<<PC5); //0x30; flip portc4,5 reinit i
		i = 0;
	}
}
}

int main(void)
{
    while(1)
    {
		PCICR = 1 << PCIE1;		//enable pcint1
		PCMSK1 =1 << PCINT8;	//enable pcint8 to allow pc0 to trigger interrupt (set flag)
		sei();
		DDRB = 0XFF; //MAKE PORT B OUTPUT FOR COUNTING
		DDRC = 0X31; //MAKE PC5, PC4, PC0 OUTPUTS
		while (1)
		{
			_delay_ms(250);
			PORTC ^= (1<<PC0);
		}
		
    }
}