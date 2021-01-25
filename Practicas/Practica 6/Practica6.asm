.INCLUDE "m2560def.inc" ; Incluir definiciones de Registros para 2560

		
mainProc:	;FORMULA = Xn+1 = (0xF1 * 0x09 + 0x07) mod 0xFFFF
			ldi r17, 0x00; load Xn H		
			ldi r16, 0xF1;  load Xn L
			
			ldi r28, 0x07; constant

			
			;LOAD COUNTER FOR 0xF4240 -> 1,000,000
			ldi r31, 0x0F; HIGH BYTE
			ldi r30, 0x42; MED BYTE 
			ldi r29, 0x40; LOW BYTE 

next:		
			ldi r19, 0x00; load a H
			ldi r18, 0x09; load a L
			
			rcall MUL16x16; r19:r18 * r17:r16 (cast) ---> R21:R20, a * Xn -> R21:R20
					
			
			add r20, r28; Sum a constant to the value			

			movw r17:r16, r21:r20; load dividend for the procedure 
			
			ldi r19, 0xFF; load divisor m H
			ldi r18, 0xFF; load divisor m L
			
			rcall DIV16_16; residue --> r15:r14 
			
			movw r17:r16, r15:r14;new value for Xn 
			
			;Write number
			out PORTA, r16 	;write in a log file Xn (LOW)
			out PORTA, r17	;Write in a log file Xn (HIGH)
			
			subi r29, 1
			sbci r30, 0		;decrement counter pre-inicialized
			sbci r31, 0
			breq end_program
			
			jmp next
			
end_program:
			
			jmp mainProc
			
MUL16x16:
			;inputs
			;r17:r16 factor 
			;r19:r18 factor 
			
			;r23:r22:r21:r20 result just interest in r21:r20 
			
			clr r2             
			mul r17,r19             ;Multiply high bytes r17xr19
			movw r23:r22,r1:r0 		;Move two-byte result into answer

			mul r16,r18            	;Multiply low bytes r16xr18
			movw r21:r20,r1:r0 		;Move two-byte result into answer

			mul r17,r18           	;Multiply r17xr18
			add r21,r0          	;Add result to answer
			adc r22,r1          
			adc r23,r2        		;Add the Carry Bit

			mul r19,r16             ;Multiply r19xr16
			add r21,r0              ;Add result to answer
			adc r22,r1          
			adc r23,r2              ;Add the Carry Bit

			ret
	
DIV16_16:;inputs r17:r16 dividendo & r19:r18 divisor
		 ;outputs r15:r14 residue  & r17:r16 cociente
				
				push r21 

				clr	r14			;clear remainder Low byte
				sub	r15,r15		;clear remainder High byte and carry
				ldi	r21,17		;init loop counter
				
	DONE:		rol	r16			;shift left dividend
				rol	r17
				dec	r21			;decrement counter
				brne decrement_16			;if done
				pop r21
				ret				;    return
				
	decrement_16:	rol	r14			;shift dividend into remainder
					rol	r15
					sub	r14,r18		;remainder = remainder - divisor
					sbc	r15,r19		;
					brcc clear_flag		;if result negative
					add	r14,r18		;    restore remainder
					adc	r15,r19
					clc				;    clear carry to be shifted into result
					rjmp DONE		;else
				
	clear_flag:		sec				;    set carry to be shifted into result
					rjmp	DONE
