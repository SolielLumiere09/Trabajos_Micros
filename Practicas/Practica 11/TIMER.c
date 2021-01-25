#include<avr/io.h>
#include<avr/interrupt.h>
#include "TIMER.h"
#include "UART.h"


static volatile uint8_t secFlag = 0;//bandera para indicar que ha pasado 1 milisegundo
static volatile uint64_t milisegs = 0;//milisegundos para la funcion 

uint8_t Timer0_SecFlag()//funcion para controlar la bandera secFlag
{
	if(secFlag == 1)
	{
		secFlag = 0;
		return 1;
		
	}
	else
		return 0;
	
}
void resetMillis()
{
	milisegs = 0;
	secFlag = 0;
}
void Timer0_Ini()//Modo CTC
{
	TIMSK0 &= ~0x01 << OCIE0A;//Deshabilitar interrupciones
	TCCR0B = 0x00; //Deshabilitar timer
	TCCR0A = 0x02;//Modo CTC
	TCCR0B |= 0x03; //prescalador de 6 bits
	OCR0A = 249;//Nuevo tope
	TCNT0 = 0x00;	//Valor inicial del timer 
	TIMSK0 |= 0x01 << OCIE0A;//Habilitar interrupciones
	//Timer0 para 1 milisegundo 
	
	sei();
}
uint64_t millis()
{
	return milisegs;//regresar milisegundos transcurridos
	
}
ISR(TIMER0_COMPA_vect)//Interrupcion para modo CTC del Timer0
{	
	TCNT0 = 0x00;
	milisegs++;
	
	if(milisegs % 1000 == 0)//si ya pasó un segundo
		secFlag = 1;
	
}