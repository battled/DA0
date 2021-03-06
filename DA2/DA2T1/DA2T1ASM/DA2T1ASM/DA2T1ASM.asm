
;  DA2T1ASM.asm
; 
;  Created: 3/12/2016 8:23:42 PM
;    Author: battled

;
; Created: 3/9/2016 10:21:51 AM
;task 1 in asm (toggle pc0
; Author : desi.battle
;

.MACRO INITSTACK
LDI R16, HIGH(RAMEND)
OUT SPH,R16
LDI R16,LOW(RAMEND)
OUT SPL,R16
.ENDMACRO


INITSTACK ;use Macro here

LDI R16, 0X01
OUT DDRC, R16; MAKE PC0 OUTPUT
;set 0th bit in r16 to 1 for EOR with PC0 
LDI R16,0b00000001			;eor reg for PC0
LDI R17,0x80				;portc temp holder
OUT PORTC,R17 ;PC = 0		;reset ==1 (active low)



LDI R20,0x00  
STS TCCR1A,R20 ;WGM11:10 = 00 Normal mode

	
BEGIN:
RCALL DELAY
EOR R17,R16 ;toggle Bit0 of R17;
OUT PORTC,R17 ;toggle PC0
RJMP BEGIN
;TIMER1 DELAY


DELAY:

LDI R20,HIGH(34286)
STS TCNT1H,R20 ;TCNT1H = high portion of 34285
LDI R20,LOW(34286)
STS TCNT1L,R20 ;TCNT1L = low portion of 34285
LDI R20,0x03
STS TCCR1B,R20 ;WGM13:12 = 00, Normal mode, prescaler = 3 (1/64)

;poll for timer1 overflow interrupt
AGAIN:
IN R20,TIFR1 ;read TIFR
SBRS R20,TOV1 ;if TOV1 is set skip next instruction
RJMP AGAIN

LDI R20,0x00
STS TCCR1B,R20 ;stop Timer1
LDI R20,0x01
OUT TIFR1,R20 ;clear TOV1 flag
RET