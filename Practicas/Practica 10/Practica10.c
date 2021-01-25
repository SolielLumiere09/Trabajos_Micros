#include<avr/io.h>
#include<avr/interrupt.h>
#include"TIMER.h"
#include"UART.h"


int main()
{
	char str[20];
	
	UART_Ini(0, BAUD_RATE, 8, 0, 1);
	UART0_AutoBaudRate();
	UART_clrscr(0);
	UART_gotoxy(0,5,1);
	UART_puts(0,"Autobauding done. UBRR0=");
	itoa(UBRR0,str,10);
	UART_puts(0,str);
	UART_puts(0,"\n\r");
	UART_puts(0,"\n\r");
	UART_puts(0,"\n\r");
	Timer0_Ini(); /* Inicializar contador de millis.*/
	Clock_Ini(1583049590000);
	UART_puts(0, "\n\r");
	while(1)
	{ /* == main loop == */
		if( Timer0_SecFlag() )
		{ /* ¿ha pasado un Segundo? */
			UART_gotoxy(0, 5,2);
			Clock_update();
			Clock_Date_Display();
			UART_puts(0, "\n\r");
			UART_gotoxy(0, 5,3);
		}
	} /* fin del loop principal */
	return 0;
}
