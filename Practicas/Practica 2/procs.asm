dosseg

.model small

	locals
	
.code
	
	public _getchar; char getchar();
	public _putchar; void putchar(char c)
	
	
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
end