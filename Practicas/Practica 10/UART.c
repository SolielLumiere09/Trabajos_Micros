#include<avr/io.h>
#include<avr/interrupt.h>
#include "UART.h"

ring_buffer buffer_out;//Buffer de salida
ring_buffer buffer_in;//fuffer de entrada	
uint8_t *com_offset = 0;//offset para inicio de los UART

void bufferAppend(ring_buffer *buffer, char ascii)
{
	buffer->buffer[mod(buffer->in_idx++)]=ascii;//colocar dato en la cola de salida
	
}
char bufferRemove(ring_buffer *buffer)
{
	
	return buffer->buffer[mod(buffer->out_idx++)];//tomar un dato de la cola de entrada
	
}
void UART_puts(uint8_t com, char *str)
{
	while(*str != '\0')
	{
		UART_putchar(com,*str);//imprimir cadena 
		str++;
	}
	
	
}
void UART_putchar(char com, char data)
{
	com_offset = ((uint8_t*)(com == 3 ? 0x130 : 0xC0 + ((8 << com) - 8)));//calculo del puerto seleccionado
	com_offset++;//apuntar al registro correspondiente para habilitar o deshabilitar interrupciones
	
	if(!isBufferFull(buffer_out))//mientras no este lleno 
	{
		bufferAppend(&buffer_out, data);//coloca el dato en la cola circular de entrada	
		if((*com_offset & (1 << UDRIE0)) == 0)//si no esta habilitada la interrupcion
			*com_offset |= (1 << UDRIE0);//Habilitar interrupcion UDRX vacio
	}
}
void UART_Ini(uint8_t com, uint16_t baudrate, uint8_t size, uint8_t parity, uint8_t stop)
{
	uint8_t *UARTX;//apuntador a la memoria del puerto
	const uint8_t OFFSET_UARTX = 0xC0;//inicio de los UARTX
	
	
	UARTX = (uint8_t*)(com == 3 ? 0x130 : OFFSET_UARTX + ((8 << com) - 8));//direccion correspondiente a un UARTX
		
	UARTX++;//apunto al registro UCSRXB
	*UARTX |= 0x18 | 128;//Activar TX,  RX e interrupcion TX complete
	
	UARTX++;//Apunto al registro UCSRXC para configurar parity, trama y bit de paro
	*UARTX = (uint8_t) 0x00;
	*UARTX |= ((parity == 1 ? 3 : parity) << 4) | ((stop - 1) << 3) | (((size - 1) & 3) << 1);
	
	UARTX+=2;//apunto al registro para configurar la velocidad 	
	
	*UARTX = (uint8_t)(baudrate);//Parte baja de UBRR0
	UARTX++;
	*UARTX = (uint8_t)(baudrate >> 8);//Parte alta de UBRR0
	
	
	sei();//habilitar interrupciones globales
}
ISR( SIG_USART0_DATA )//interrupcion para UART0
{
	if(!isBufferEmpty(buffer_out))//Mientras no este vacio
		UDR0 = bufferRemove(&buffer_out);//imprimir caracter
	else
	{
		UCSR0B &= ~(1 <<  UDRIE0);//deshabilitar interrupcion UDR0 vacio 
		
	}
		
	
}
//Interrupcion que no se usan 
ISR( SIG_USART1_DATA )
{
	if(!isBufferEmpty(buffer_out))//interrupcion para UART1
		UDR1 = bufferRemove(&buffer_out);//imprimir caracter
	else
	{
		UCSR1B &= ~(1 <<  UDRIE1);//deshabilitar interrupcion UDR1 vacio 
	}
}
ISR( SIG_USART2_DATA )
{
	if(!isBufferEmpty(buffer_out))//Interrupcion para UART2
		UDR2 = bufferRemove(&buffer_out);//imprimir caracter
	else
	{
		UCSR2B &= ~(1 <<  UDRIE2);//deshabilitar interrupcion UDR1 vacio 
	}
}
ISR( SIG_USART3_DATA )
{
	if(!isBufferEmpty(buffer_out))//interrupcion para UART3 
		UDR3 = bufferRemove(&buffer_out);//imprimir caracter
	else
	{
		UCSR3B &= ~(1 <<  UDRIE3);//deshabilitar interrupcion UDR1 vacio 
		
	}
}
////////////////////
ISR(USART0_RX_vect)//si UDR0 esta vacio
{
	if(!isBufferFull(buffer_in))//si no esta lleno
		bufferAppend(&buffer_in, UDR0);//colocar dato en la cola de entrada 
		
}
//interrupciones que no se usan 
ISR(USART1_RX_vect)
{
	if(!isBufferFull(buffer_in))
		bufferAppend(&buffer_in, UDR1);
		
}
ISR(USART2_RX_vect)
{
	if(!isBufferFull(buffer_in))
		bufferAppend(&buffer_in, UDR2);
		
}
ISR(USART3_RX_vect)
{
	if(!isBufferFull(buffer_in))
		bufferAppend(&buffer_in, UDR3);
		
}
///////////////////////////////////
uint8_t UART_available()
{
	return (!isBufferEmpty(buffer_in) ? 1 : 0);
	
}
char UART_getchar()
{
	while(!UART_available());//esperar a que haya un dato en la cola circular de entrada
	return buffer_in.buffer[mod(buffer_in.out_idx++)];//retornar el dato de la cola circular de entrada 
	
	
}
void UART_gets(uint8_t com, char *str)
{
	char keyPressed = 0;//tecla precionada 
	int index = 0;//indice para el inicio o final de la cadena 
	
	do{
		keyPressed = UART_getchar();//obtener tecla del UART0
			
			if(keyPressed == BACK_SPACE)//si la telca fue backspace 
			{
				str[index] = '\0';//quitar la tecla actual 
				index--;//disminuir indice
				
				if(index >= 0)
				{
					UART_putchar(com, BACK_SPACE);
					UART_putchar(com, SPACE);//eliminar caracter de pantala
					 UART_putchar(com, BACK_SPACE);
					
				}
				
				
			}
			else if(index < STR_LEN-1 && keyPressed != ENTER)//mientras no me pase del buffer
			{
				str[index] = keyPressed;
				UART_putchar(com, keyPressed);
				index++;
				str[index] = '\0';
			}

			else
			{
				index++;
				UART_putchar(com, keyPressed);
			}
			
			if(index < 0)
					index = 0;//siempre se mantega por encima de 0
			
	}while(keyPressed != ENTER);
	str[index-1] = '\0';
	
}
void itoa(uint16_t number, char* str, uint8_t base)
{
	uint16_t residue;
	uint16_t i = 0, j = 0;
	uint8_t temp_stack[16];
	
	while(number >= base && j < 16)
	{
		residue = number % base;
		temp_stack[j] = residue;
		number = number / base;
		j++;
	}
	temp_stack[j] = number;
	
	while(j > 0)
	{
		number = temp_stack[j];
		if(number >= 10)
			str[i] = number + '7';
		else 
			str[i] = number + '0';
		
		j--;
		i++;
	}

	number = temp_stack[j];
	if(number >= 10)
		str[i] = number + '7';
	else 
		str[i] = number + '0';
	
	i++;
	str[i] = '\x0';

}
uint16_t atoi(char *str)
{
	uint8_t i;
	uint16_t sum = 0;
	
	for(i = 0; str[i] != '\0' && str[i] >= 48 && str[i] <= 57; i++)
		sum = sum * 10 + (str[i] - '0');
	
	
	return sum;
}
void UART_setColor(uint8_t com, uint8_t color)
{
	static char str[] = "\x1b[30m";
	str[3] = color + '0'; 

    UART_puts(com, str);
	
}
void UART_gotoxy(uint8_t com, uint8_t x, uint8_t y)
{
	static char str[] = "\x1b[00;00H";
	  
    str[3] = '0' + y % 10;
    y/=10;
    str[2] = '0' + y %10; 

    str[6] = '0' + x % 10;
    x/=10;
    str[5] = '0' + x %10; 

  UART_puts(com, str);
}
void UART_clrscr( uint8_t com ){
	UART_puts(com, "\x1b[2J");
    UART_puts(com, "\x1b[H");
}
void UART0_AutoBaudRate( void )
{
  DDRE &= ~(1<<DDE0); //Puero PE0 como entrada
  PORTE |= (1<<PE0); //Habilitar pull up 

  TCNT0 = 0; //Poner contador en 0
  TCCR0A = 0; //Modo normal 
  TCCR0B = 0; //Detener timer0 "ZA WARUDO TOKI WO TOMARE!"!!!!

  while ((PINE & (1<<PE0)));//mientras lea el pull up 
  TCCR0B = (1<<CS01);  // "SOSHITE TOKI GA UGOKI DESU" Iniciar Timer0 con 8 de prescalador ya que se detecto start bit  
  while (!(PINE & (1<<PE0)));//mientras lea el start bit 
  TCCR0B = 0; //detener timer 
  
  UBRR0 = (TCNT0>>1)-1;//Nuevo periodo para UBRR0;
}