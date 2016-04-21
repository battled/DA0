/*
 * DA5.c
 * Created: 4/19/2016 12:21:33 PM
 * Author : desi.battle
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
		int ctr =0; //counter to do software timer
		DDRB = 0XFF;
		DDRD = 0XFF;
		ADCSRA = 0X87; //10000111 Enable ADC, divide clock by 128
		ADMUX  = 0X40; //internal 1.1v voltage reference
	
		while(1)
		{
			ADCSRA |= (1<< ADSC)|(1<<ADIF); 	//read adc
			while ((ADCSRA & (1<<ADIF)) ==0);   //wait for adc to finish
			ctr = 1024 - ADC;	//set first part of duty cycle
			while (ctr != 0)
			{
				ctr --;
				_delay_us(10);
			}

			PORTB ^= 0x04;		//flip output bit
			
			ctr = ADC;			//set second half of duty cycle
			while (ctr != 0)
			{
				ctr--;
				_delay_us(10);	
			}
			PORTB ^= 0X04;		//flip output bit
		}
}
