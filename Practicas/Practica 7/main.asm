;------------- definiciones e includes ------------------------------
.INCLUDE "m2560def.inc"
.equ INIT_VALUE = 0x80
;------------- init PORTS -------------------------------------------
		ldi R24,INIT_VALUE
		out DDRB,R24
;------------- main loop -------------------------------------------
next:	
;**** Llamar Retardo ****
		
		call Delay_103uS
		out PORTB,R24		
		
		call Delay_100mS
		out PORTB,R24		
		
		call Delay_1S
		out PORTB,R24		
		
		rjmp next
		
;delay de 103uS		
Delay_103uS:
					;[1] * (5) = 5
		push R20	;[1] * (2) = 2
		push R21	;[1] * (2) = 2
		ldi R20,163;[1] * (1) = 1
		
nxt_A:	nop			;[A] * (1) = A
		nop			;[A] * (1) = A
		
		ldi R21,1	;[A] * (1) = A
nxt_B:	nop			;[AB] * (1) = AB
		nop			;[AB] * (1) = AB
		dec R21		;[AB] * (1) = AB
		brne nxt_B	;[A][2B - 1] = 2AB - A
		
		dec R20		;[A] * (1) = A
		brne nxt_A	;[2A - 1] = 2A - 1
		
		pop R21		;[1] * (2) = 2		
		pop R20		;[1] * (2) = 2
		ret			;[1] * (5) = 5
		
;Delay de 100mS		
Delay_100mS:
						;[1] * (5) (call)
		push R20		;[1] * (2)
		push R21		;[1] * (2)
		push R22		;[1] * (2)
		
		ldi R22, 65		;[1] * (1)
nxt_A2:	nop				;[A] * (1)
		nop				;[A] * (1)
		
		ldi R21, 46		;[A] * (1)
nxt_B2:	nop				;[A] * [B] * (1)
		nop				;[A] * [B] * (1)
		
		ldi R20, 106	;[A] * [B] * (1)
nxt_C2:	nop				;[A] * [B] * [C] * (1)
		nop				;[A] * [B] * [C] * (1)
		dec R20			;[A] * [B] * [C] * (1)
		brne nxt_C2		;[A] * [B] * [2C - 1] 
		
		dec R21			;[A] * [B] * (1)
		brne nxt_B2		;[A] * [2B-1]
		
		dec R22			;[A] * (1)
		brne nxt_A2		;[2A - 1]
		
		nop				;[1] * (1)
		nop				;[1] * (1)
		ldi R20, 0x0	;[1] * (1)
		
		pop R22			;[1] * (2)
		pop R21			;[1] * (2)
		pop R20			;[1] * (2)
		
		ret				;[1] * (5)

;Delay de 1 segundo
Delay_1S:
						;[1] * (5) (call)
		push R20		;[1] * (2)
		push R21		;[1] * (2)
		push R22		;[1] * (2)
		
		ldi R22, 0xEB	;[1] * (1)
nxt_A3:	nop				;[A] * (1)
		nop				;[A] * (1)
		
		ldi R21, 0x4A	;[A] * (1)
nxt_B3:	nop				;[A] * [B] * (1)
		nop				;[A] * [B] * (1)
		
		ldi R20, 0xB7	;[A] * [B] * (1)
nxt_C3:	nop				;[A] * [B] * [C] * (1)
		nop				;[A] * [B] * [C] * (1)
		dec R20			;[A] * [B] * [C] * (1)
		brne nxt_C3		;[A] * [B] * [2C - 1] 
		
		dec R21			;[A] * [B] * (1)
		brne nxt_B3		;[A] * [2B-1]
		
		dec R22			;[A] * (1)
		brne nxt_A3		;[2A - 1]
		
		nop				;[1] * (1)
		nop				;[1] * (1)
		ldi R22, 0		;[1] * (1)
		
		pop R22			;[1] * (2)
		pop R21			;[1] * (2)
		pop R20			;[1] * (2)
		
		ret				;[1] * (5)

	
