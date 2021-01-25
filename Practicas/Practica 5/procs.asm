dosseg 

.model tiny

.code

	public _pokeW; pokeW(WORD segment, WORD offset, WORD value)
	public _pokeB; pokeW(WORD segment, WORD offset, BYTE value)
	public _putchar; void putchar(char c)
	public _getchar; char getchar();
	
	_pokeW proc
		push bp
		mov bp, sp; protocolo de entrada
		
		mov [bp - 2], es; guardar segmento es 
		
		mov es, [bp + 4]; traerme el segmento 
		mov bx, [bp + 6]; traerme el offset
		mov ax, [bp + 8]; traerme el valor 
		
		mov es:[bx], ax; escribir el valor del valor 
		
		mov es, [bp - 2]; recuperar segmento es 
		
		pop bp; protocolo de salida 
		ret
	endp
	
	_pokeB proc
		push bp
		mov bp, sp; protocolo de entrada
		
		mov [bp - 2], es; guardar segmento es 
		
		mov es, [bp + 4]; traerme el segmento 
		mov bx, [bp + 6]; traerme el offset
		mov al, [bp + 8]; traerme el valor 
		
		mov es:[bx], al; escribir el valor del valor 
		
		mov es, [bp - 2]; recuperar segmento es 
		
		pop bp; protocolo de salida 
		ret
	endp
	
	
	_putchar proc; Función que manda un carácter a la pantalla
		push bp
		mov bp, sp; protocolo de entrada
		
		mov dl, [bp + 4]; primer parametro 
		mov ah, 2h
		int 21h
		
		pop bp; protocolo de salida
		ret
	endp
	
	_getchar proc;Función que retorna el carácter capturado del teclado en AL 
	
		push bp
		mov bp, sp; protocolo de entrada
		
		mov ah, 1h
		int 21h
		
	
		pop bp; protocolo de salida
		ret
	endp
	
end