;
; DA2new.asm
;
; Created: 3/9/2016 10:21:51 AM
; task 2 in asm (Flip C0 and count on B)
; Author : desi.battle
;

.MACRO INITSTACK
LDI R16, HIGH(RAMEND)
OUT SPH,R16
LDI R16,LOW(RAMEND)
OUT SPL,R16
.ENDMACRO


INITSTACK ;use Macro here

;Make entire port B output.
LDI R16, 0XFF
OUT DDRB, R16
LDI R16, 0X01
OUT DDRC, R16; MAKE PC0 OUTPUTS
;set 0th bit in r16 to 1 for EOR with PC0 
LDI R16,0b00000001			;eor reg for PC0
LDI R17,0x80				;portc temp holder
LDI R18,0	;R18 IS COUNTER
OUT PORTC,R17 ;PC = 0



LDI R20,0x00  
STS TCCR1A,R20 ;WGM11:10 = 00 Normal mode

	
BEGIN:
RCALL DELAY
EOR R17,R16 ;toggle Bit0 of R17;
OUT PORTC,R17 ;toggle PC0
SBIS PORTC, 0 ;only increment Portb if portc.0 is 1
RJMP NOCOUNT
INC R18
OUT PORTB, R18
NOCOUNT:
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