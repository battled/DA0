/*
 * DA4.c
 *
 * Created: 4/8/2016 7:01:29 PM
 *  Author: battled
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#define DEL 5

int main(void)
{
	//configure timer0; set up fast pwm TOP = OCRA;
	//inverting mode OC0B (set on match, clear at BOTTOM)
	//use OCRA for frequency changes, OCRB for duty cycle
	TCCR0A = (0<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(1<<COM0B0)|(1<<WGM01) | (1<<WGM00);
	TCCR0B = (0<<FOC0A )|(0<<FOC0B) |(1<<WGM02) |(0<<CS02)  |(0<<CS01)  | (1<<CS00);
	//configure timer1
	TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(1<<COM1B0)|(1<<WGM11)|(1<<WGM10);
	TCCR1B = (0<<ICNC1) |(0<<ICES1) |(1<<WGM13) |(1<<WGM12) |(0<<CS12) |(0<< CS11)|(1<<CS10);
	//configure timer2
	TCCR2A = (0<<COM2A1)|(0<<COM2A0)|(1<<COM2B1)|(1<<COM2B0)|(1<<WGM21) | (1<<WGM20);
	TCCR2B = (0<<FOC2A )|(0<<FOC2B) |(1<<WGM22) |(0<<CS22)  |(0<<CS21)  | (1<<CS20);	
	DDRB=0XFF;
	DDRD=0xFF;

    while(1)
    {
	//	_delay_ms(3000);
					
		for (int r_freq=225; r_freq > 25; r_freq -=25)
		{
			OCR2A = r_freq;
			for (int b_freq=225; b_freq > 25; b_freq -= 25)
			{
				OCR0A = b_freq;
				for(int g_freq=225; g_freq > 25; g_freq -= 25)
				{
					OCR1A = g_freq;
					for(int g_dc=0; g_dc <= g_freq; g_dc+=15)
					{
						_delay_ms(DEL);
						OCR1B = g_dc;
						for (int r_dc=0; r_dc <= r_freq;r_dc+=9)
							{
								_delay_ms(DEL);
								OCR2B = r_dc;
								for (int b_dc=0; b_dc <= b_freq; b_dc+=2)
								{
									_delay_ms(DEL);
									OCR0B = b_dc;
								}
							}
					}
				}
			}
		}
	}
}