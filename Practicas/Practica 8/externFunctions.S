.global getRandom
.global delay_1S
.global delay_1mS

		;BYTE getRandom(BYTE seed);
		;seed+1 = seed * A mod (m)
		
getRandom:	
			;R24 = sed
			push R0; Guardar registro de acuerdo al documento de Atmel 
				
			ldi R18, 37; Amplitud del valor 
			mul R24, R18; A * seed
			;R1:R0 tiene el resultado
			
			add R0, R18;sumar un valor constante para mejor aleatoridad
			movw R20, R0; R21:R20 = R1:R0
			
			lsr R21;
			ror R20; dividir entre 2
			lsr R21;
			ror R20; dividir entre 4
			lsr R21;
			ror R20; dividir entre 8
			
			lsl R20;
			rol R21; multiplicar por 2
			lsl R20;
			rol R21; multiplicar por 2
			lsl R20;
			rol R21; multiplicar por 2
			
			
			;R1:R0 - R21:R20
			
			sub R0, R20 ; Obtener el residuo restar parte baja
			sbc R1, R21 ; restar parte alta con acarreo
					
			mov R24, R0; Colocar el resultado en el registro de retorno
			
			
			clr R1; limpiar R1 de acuerdo al documento de Atmel
			pop R0; Restaurar R0 
			ret

delay_1mS:
						;[1] * (5) (call)
		push R20		;[1] * (2)
		push R21 		;[1] * (2)
		
		ldi R20, 0xBC	;[1] * (1)
nxt_A:	nop				;[A] * (1)
		nop				;[A] * (1)
		ldi R21, 0x10	;[A] * (1)
nxt_B:	nop				;[A] * [B] * (1)
		nop				;[A] * [B] * (1)
		dec R21			;[A] * [B] * (1)
		brne nxt_B		;[A] * ([2B]-1)
		dec R20			;[A] * (1)
		brne nxt_A		;[2A]-1
		nop				;[1] * (1)
		nop				;[1] * (1)
		pop R21			;[1] * (2)
		pop R20			;[1] * (2)
		ret				;[1] * (5)

delay_1S:
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


