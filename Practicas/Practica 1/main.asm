.model tiny 

locals
	org 100h
	
.data

.code

	.startup
	
	main proc
		
		mov sp, 0fffh
		
		mov ax, 123
		call print_AX_inv
		
		mov ax, 120
		call print_AX_inv
		
		mov ax, -12
		call print_AX_inv
		
		mov ax, 12345
		call print_AX_inv
		
		mov ax, 32765
		call print_AX_inv
		
		mov ax, -1253
		call print_AX_inv
		
		mov ax, -10235
		call print_AX_inv
@@stay:
		
		jmp @@stay
		ret
	endp
	
	
	
	
	get_AX_inv proc; Recibe en AX el numero, lo invierte y almacena en DX el resultado, caso de 'Overflow', pone DX en 0
		
		push bp; Guardar BP
		push cx; guardar CX
		push ax; Recuperar AX 
		mov bp, sp
		
		;Variables locales en la pila 
		mov [bp - 2], word ptr 0; b = 0
		mov [bp - 4], word ptr 0; sum = 0
		mov [bp - 6], ax; number
		mov [bp - 8], word ptr 0; temp = 0
		
@@if:	cmp [bp - 6], word ptr 0; if(number < 0)
		jge @@end_if
		neg word ptr [bp - 6];number =  -number
		mov [bp - 2], word ptr 1; b = 1

@@end_if:
		
@@while:	cmp [bp - 6], word ptr 0; while(number > 0)
			jle @@end_while
			;sum = sum * 10 + number % 10
			
			;sum * 10
			mov ax, [bp - 4]; ax = sum 
			mov cx, 10
			imul cx; dx-ax = ax * cx ---> ax = sum * 10 

@@if_bx:	test ax, 8000h; checar bit de signo
			jz @@end_bx; si el bit mas significativo de ax es igual a 1 entonces hay "overflow"
			mov [bp - 4], word ptr 0
			jmp @@end_while
@@end_bx:						

			;number % 10 
			mov [bp - 8], ax; temp = sum * 10 
			mov ax, [bp - 6]; ax = number 
			mov dx, 0; dx = 0
			idiv cx; dx-ax/cx ---> AX = AX / 10, DX = AX % 10 --> DX = number % 10 
			add dx, [bp - 8]; --> dx = sum * 10 + number % 10
			
			mov [bp - 4], dx; sum = sum * 10 + number % 10	
			
			;number = number / 10;
			mov ax, [bp - 6]; ax = number
			mov dx, 0
			idiv cx; dx-ax/cx --> ax = ax/10, dx = ax % 10 --> ax = number / 10
			mov [bp - 6], ax; number = number / 10;
			jmp @@while

@@end_while:
			
@@if_2:		cmp [bp - 2], word ptr 1; if(b == 1)
			jne @@end_if_2
			;sum = -sum 
			neg word ptr [bp - 4]; sum = -sum
			
@@end_if_2:
		
		mov dx, [bp - 4]; DX = sum
		
		pop ax ; recuperar AX 
		pop cx; recuperar CX
		pop bp; Recuperar BP
		ret
	endp
	
	print_AX_inv proc ; imprime el valor de AX y AX invertido y almacena el resultado en DX
		
		push ax 
		
		push ax 
		mov al, 'A'
		call putchar
		mov al, 'X'
		call putchar
		mov al, '='
		call putchar
		mov al, ' '
		call putchar
		pop ax 
		
		call printSignedNumber; Imprime AX
		call get_AX_inv; Retorna en DX el numero invertido de AX 
		
		mov al, 10
		call putchar
		mov al, 13
		call putchar
		mov al, 'D'
		call putchar
		mov al, 'X'
		call putchar
		mov al, '='
		call putchar
		mov al, ' '
		call putchar
		
		mov ax, dx 
		call printSignedNumber; Imprimir DX que contiene AX invertido 
		
		mov al, 10
		call putchar
		mov al, 13
		call putchar
		
		mov al, 10
		call putchar
		mov al, 13
		call putchar
		
		pop ax 
		ret
	endp
	
	
	
	printSignedNumber proc; Recibe en AX el numero y se imprime con signo
		
		push ax
		
@@if:	cmp ax, 0
		jge @@else ;Es positivo
		
		push ax
		mov al, '-'; Es negativo
		call putchar
		pop ax
		
		neg ax
		call printNumber
		
		jmp @@end_if

@@else:	call printNumber
		
	
@@end_if:
		
		
		pop ax
	
	ret
	endp
	
	putchar proc ;imprime un caracter contenido en AL
		
		push bx
		push dx
		
		
		mov bx, ax
		
		mov dl, al
		mov ah, 2h
		int 21h
		
		mov ah, bh
		
		pop dx
		pop bx
		
	ret
	endp
	
	
	printNumber proc ;imprime un numero contenido en AX 
		
		push ax
		push bx
		push cx
		push dx
		
		mov bx, 10
		mov cx, 0
		mov dx, 0
		
@@do_while: div bx
			push dx
			mov dx, 0
			inc cx
			cmp ax, word ptr 0
			jne @@do_while
@@end_do_While:

@@nxt:	pop ax
		add al, 30h
		call putchar
		
		loop @@nxt
		
		pop dx
		pop cx
		pop bx
		pop ax
		
	ret
	endp
	
	
end
