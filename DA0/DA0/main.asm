;CpE 301 Design Assignment 0
; DA0.asm
; Created: 2/15/2016 6:25:14 PM
; Author : DJ

start:
    ldi r16, 0	;initialize rsum to 0
	ldi r17, 60 ;store random number in r17 
	ldi r18, 60 ;store random number in r18
	ldi r19, 60 ;store random number in r19
	ldi r20, 60 ;store random number in r20
	ldi r21, 16 ;store random number in r21

	;Add "random" numbers to r16, only check for carry flag
	;after 5th addition due to problem constraints that random
	;values are between 30 and 60. Therefore carry may only be
	;generated following the 5th addition
	add r16, r17
	add r16, r18
	add r16, r19
	add r16, r20
	add r16, r21
	brcs carrySet	;if the carry flag set break to carrySet label
	rjmp done		;if carry flag not send jump to done
	carrySet:
	ldi r22, 4		;load 4 or 0b00000100 into r22
	out PORTB, r22  ;set port b to 0b00000100 if carry flag was set. (set portb.2 = 1) 
	done:			;infinite loop denoting end of program
	rjmp done
