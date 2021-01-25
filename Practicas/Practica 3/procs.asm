dosseg

.model tiny

.code

	public _peek;unsigned char peek(unsigned int segment, unsigned int offset)
	public _poke;void poke(unsigned int segment, unsigned int offset, unsigned char data)
	public _getchar; char getchar();
	public _putchar; void putchar(char c)

	
	;Peek; La cual retorna el byte almacenado en el segmento 
	;y desplazamiento especificado en los parámetros.
	
	;poke; La cual almacena un byte en el segmento 
	;y desplazamiento especificado en los parámetros
		
		
		
	_getchar proc;Función que retorna el carácter capturado del teclado en AL 
	
		push bp
		mov bp, sp; protocolo de entrada
		
		mov ah, 1h
		int 21h
		
	
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
	
	
	_peek proc
		
		push bp
		mov bp, sp
		mov [bp - 2], es ; Guardar e registro de segmento a usar 
		
		mov es, [bp + 4]; Traerme el segmento 
		mov bx, [bp + 6]; traerme el offset 
		
		mov al, es:[bx]; Para retornar AL 
		
		
		mov es, [bp - 2]; recuperar registro ES
		pop bp
		ret
	endp
	
	
	_poke proc
		push bp
		mov bp, sp
		
		mov [bp - 2], es
		
		mov es, [bp + 4]; Traerme el segmento
		mov bx, [bp + 6]; traerme el offset 
		mov al, [bp + 8]; traerme el dato 
		
		mov es:[bx], al; Guardar el dato en dirección especificada
		
		
		mov es, [bp - 2]; recuperar es
		
		pop bp
		ret
	endp
	

end