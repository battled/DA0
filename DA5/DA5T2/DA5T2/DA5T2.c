/*
 * DA5.c
 * Created: 4/19/2016 12:21:33 PM
 * Author : desi.battle
 */ 

#define F_CPU 8000000UL //XTAL = 8MHZ
#include <avr/io.h>
#include <util/delay.h>

void my_delay(int x)		//delay function
{
	int i;
	for(i= 0; i < x*2.5; i++)
	{
		_delay_us(1);
	}
}
int main()
{
	int del;	//dleay
	DDRB = (1<<PB0);//make PB0 an output
	ADCSRA	= 0x80;	//use AREF, internal Vref turned off, ck/2
	ADMUX=0;	//ADC single ended input

	while(1)
	{
		ADCSRA |= (1<<ADSC);			//start conversion
		while((ADCSRA&(1<<ADIF))==0);	//wait for conversion to finish
		del = 1024 - ADC;					//store adc
		PORTB = 0X06;
		my_delay(del);
		PORTB = 0x0A;
		my_delay(del);
		PORTB = 0x09;
		my_delay(del);
		PORTB = 0x05;
		my_delay(del);
	}
	return 0;
}
