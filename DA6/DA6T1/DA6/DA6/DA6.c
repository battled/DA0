/*
 * DA6.c
 *
 * Created: 4/28/2016 6:15:01 PM
 *  Author: battled
 */ 

 #define F_CPU 8000000UL  //8 MHz xtal crystal
 #include <avr/io.h>		  //
 #include <util/delay.h> //delay header
 #include <avr/interrupt.h>
 #include <string.h>	//for string concatenation
 #include <stdio.h>	//for sprintf
#define F_CPU 8000000UL


#define LCD_DPRT PORTD //LCD DATA PORT
#define LCD_DDDR DDRD //LCD DATA DDR
#define LCD_DPIN PIND //LCD DATA PIN
#define LCD_CPRT PORTB //LCD COMMANDS PORT
#define LCD_CDDR DDRB //LCD COMMANDS DDR
#define LCD_CPIN PINB //LCD COMMANDS PIN
#define LCD_RS 0 //LCD RS
#define LCD_RW 1 //LCD RW
#define LCD_EN 2 //LCD EN


void lcdCommanda (unsigned char cmnd)
{
	LCD_DPRT = cmnd; //send cmnd to data port
	LCD_CPRT &= ~(1<<LCD_RS); //RS = 0 for command
	LCD_CPRT &= ~(1<<LCD_RW); //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
}
void lcdData(unsigned char data)
{
	LCD_DPRT = data; //send data to data port
	LCD_CPRT |= (1<<LCD_RS); //RS = 1 for data
	LCD_CPRT &= ~(1<<LCD_RW); //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
}


 void lcd_gotoxy(unsigned char x, unsigned char y)
 {
	 unsigned char firstCharAdr[] = {0x80, 0xC0, 0x94, 0xD4};
	 lcdCommanda(firstCharAdr[y-1] + x -1);
	 _delay_us(100);
 }
 
 void lcd_print(char * str)
 {
	 unsigned char i = 0;
	 while (str[i]!=0)
	 {
		 lcdData(str[i]);
		 i++;
	 }
 }
 
 void lcd_init()
 {
	 LCD_DDDR = 0xFF;
	 LCD_CDDR = 0xFF;
	 LCD_CPRT &=~(1<<LCD_EN); //LCD_EN = 0
	 _delay_us(2000); //wait for init
	 lcdCommanda(0x38); //inti. LCD 2 line, 5x7
	 lcdCommanda(0x0E); //display on, cursor on
	 lcdCommanda(0x01); //clear LCD
	 _delay_us(2000); //wait
	 lcdCommanda(0x06); //shift cursor right
 }


 int main(void)
 {
 DDRC &= ~(1<<PORTC0);	//PC0 is analog input
 lcd_init();
 
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
 char TmpTemp[6] = "";

 //sprintf here is used to "print" to TempInt string and then dot is added at end
 sprintf(TmpTemp, "%d.%d F", adc_tempi,adc_tempd);
 //output degrees Fahrenheit
 lcd_gotoxy(1,1);
 lcd_print("Temp is");
 lcd_gotoxy(1,2);
 lcd_print(TmpTemp);
 
 //reset time 1 for interrupt
 TCNT1 = 65536 - ((double)F_CPU/256); //overflow in 1 sec
 TCCR1A = 0; //normal mode
 TCCR1B = 4; //prescaler = 256
 
 return;
 }