/*
 * DA5.c
 * Created: 4/19/2016 12:21:33 PM
 * Author : desi.battle
 */ 

#include <util/delay.h>
#include <avr/io.h>
#define F_CPU 8000000UL
int main()

{
	//PORTD pins as input
	DDRD = 0xFF;
	//Enable internal pull ups
	PORTD = 0xFF;
	//Set PORTB1 pin as output
	DDRB = 0xFF;
	ADCSRA	= 0x80;	//use AREF, internal Vref turned off, ck/2
	ADMUX=0;	//ADC single ended input

	//TOP = ICR1;
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM
	//ICR1 = 20000 defines 50Hz pwm
	float adcConv;
	ICR1 = 20000;
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10); //TCCR1A = 0x82
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
	//TCCR1B = 0x1A
	//start timer with prescaler 8
	OCR1A = 500;
	while(1)
	{
		ADCSRA |= (1<<ADSC);			//start conversion
		while((ADCSRA&(1<<ADIF))==0);	//wait for conversion to finish
		adcConv =1024-ADC;
		adcConv /=1024;					//convert ADC value (0->1024 as a ratio by dividing by 1024)
		adcConv *=1800;					//convert ADC value to scale of our servo by multiplying by 1800
		adcConv += 500;					//add constant 500 as 0 degrees ==500 

		
		OCR1A =(int)adcConv;					//store adc
		_delay_ms(250);
	}

}