/*
 * DA3.c
 *
 * Created: 3/31/2016 8:44:07 PM
 *  Author: battled
 */ 

#define F_CPU 8000000UL  //8 MHz xtal crystal
#include <avr/io.h>		  //
#include <avr/interrupt.h>
#include <string.h>	//for string concatenation
#include <stdio.h>	//for sprintf
//
void usart_init (void)
{
	UCSR0B = (1<<TXEN0); //transmit enable
	UCSR0C = ((1<<UCSZ01)|(1<<UCSZ00)); //asynch mode
	UBRR0L = 0x33; //baud rate 9600 at 8 MHz
}
void usart_tx_string(char *data) //takes a string and sends it serially
{
	while(*data != '\0'){ //send chars until NULL is found
		while(!(UCSR0A & (1<<UDRE0))); //wait for UDRE0 to be 1
		UDR0 = *data;	//push current char into udr0 reg
		data++; //point to next char
	}
}
int main(void)
{
	DDRC &= ~(1<<PORTC0);	//PC0 is analog input
	
	usart_init(); //initialize USART
	ADCSRA = ((1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));// ADC prescaler 128 ADEN
	ADMUX = ((1<<REFS1)|(1 << REFS0)); // select internal 1.1 V Ref w/ ext cap at AREF pin and ADC0 (default)
	sei(); //enable interrupts
	
	//configure timer 1 to interrupt every second
	TCNT1 = 65536 - ((double)F_CPU/256); // set timer to overflow in 1 sec
	TCCR1A = 0; //normal mode
	TCCR1B = 4; //prescaler = 256
	TIMSK1	|= (1<<TOIE1); //enable interrupt on overflow timer 1
	
	while(1); //wait for interrupts
	
	return 0;
}

ISR(TIMER1_OVF_vect) //timer1 overflow ISR
{
	TCCR1B = 0; //stop timer 1
	TIFR1 = 1; //clear overflow flag

	int adc_temp;	//stores ADC temporarily
	float adc_tempf; //float for calculations
	int adc_tempi; //integer part
	int adc_tempd; //decimal part
	
	//read ADC
	ADCSRA |= (1<<ADSC); //start conversion
	while((ADCSRA &(1<<ADIF)) == 0); //wait for conversion to finish
	adc_temp = ADC;	//save ADC value
	
	adc_tempf = (float)adc_temp * (1.1 / 1024) / 0.01; //(ADC*res/.01) (lm34 sf = 10mv/degF)
	adc_tempi = (int)adc_tempf; //integer part
	adc_tempf = adc_tempf - adc_tempi; //remove integer part of float temp
	adc_tempd = (int)(adc_tempf * 100); //store 2 decimal bits as float
	//current temp: xxx.xx°F\r\n == 24 chars
	char TmpTemp[24] = "Current Temp: ";

	//sprintf here is used to "print" to TempInt string and then dot is added at end
	sprintf(TmpTemp+14, "%d.%d°F\r\n", adc_tempi,adc_tempd);
	//output degrees Fahrenheit
	
	usart_tx_string(TmpTemp);
	
	//reset time 1 for interrupt
	TCNT1 = 65536 - ((double)F_CPU/256); //overflow in 1 sec
	TCCR1A = 0; //normal mode
	TCCR1B = 4; //prescaler = 256
	
	return;
}