#include<avr/io.h>
#include<avr/interrupt.h>
#include "TIMER.h"
#include "UART.h"


static volatile uint8_t secFlag = 0;//bandera para indicar que ha pasado 1 milisegundo
static volatile uint64_t milisegs = 0;//milisegundos para la funcion 

static Clock clock = {0}; //estructrua para desplegar el reloj en pantalla

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
void Clock_Ini(uint64_t unixtime)
{	
	unixtime /= 1000;//obtener segundos
	unixtime -= 28800;//Restar 8 horas para mi zona horaria 
	clock.second = unixtime % 60;//obtener segundos
	unixtime /= 60;
	clock.minute = unixtime % 60;//obtener 	minutos
	unixtime /= 60;
	clock.hour = unixtime % 24;//obtener horas
	unixtime /= 24;
	unixtime += 719499;//Sumar la constante de ajuste 
	for (clock.year = 1969; unixtime > YEAR_TO_DAYS(clock.year + 1) + 30; clock.year++);//compesar los años del tiempo
	unixtime -= YEAR_TO_DAYS(clock.year);//restar años para obtener meses
	for (clock.month = 1; clock.month < 12 && unixtime > 367*(clock.month+1)/12; clock.month++);//ajustar para meses

	unixtime -= 367*clock.month/12;//obtener dias

	clock.month += 2;
	if (clock.month > 12) {
		clock.month -= 12;
		clock.year++;
	}
	clock.day = unixtime;
	
}
void Clock_Date_Display( )
{
	//hh:mm:ss dd/mm/aaaa
	char buffer[4];

	if(clock.hour >= 10)//imprimir horas
	{
		itoa(clock.hour, buffer, 10);
		UART_puts(0, buffer);
		UART_putchar(0, ':');
	}
	else
	{
		UART_putchar(0, '0');
		UART_putchar(0, clock.hour + '0');
		UART_putchar(0, ':');	
		
	}
	
	if(clock.minute >= 10)//imprimir minutos
	{
		itoa(clock.minute, buffer, 10);
		UART_puts(0, buffer);
		UART_putchar(0, ':');
	}
	else
	{
		UART_putchar(0, '0');
		UART_putchar(0, clock.minute + '0');
		UART_putchar(0, ':');	
		
	}
	
	if(clock.second >= 10)//imprimir segundos
	{
		itoa(clock.second, buffer, 10);
		UART_puts(0, buffer);
		UART_putchar(0, ' ');
	}
	else
	{
		UART_putchar(0, '0');
		UART_putchar(0, clock.second + '0');
		UART_putchar(0, ' ');	
		
	}
	
	if(clock.day >= 10)//imprimir dias
	{
		itoa(clock.day, buffer, 10);
		UART_puts(0, buffer);
		UART_putchar(0, ':');
	}
	else
	{
		UART_putchar(0, '0');
		UART_putchar(0, clock.day + '0');
		UART_putchar(0, ':');	
		
	}
	if(clock.month >= 10)//imprimir meses
	{
		itoa(clock.month, buffer, 10);
		UART_puts(0, buffer);
		UART_putchar(0, ':');
	}
	else
	{
		UART_putchar(0, '0');
		UART_putchar(0, clock.month + '0');
		UART_putchar(0, ':');	
		
	}
	

	itoa(clock.year, buffer, 10);//imprimir años
	UART_puts(0, buffer);
	UART_putchar(0, ' ');
	
}
void Clock_update()//actualizar reloj
{
	uint8_t day = 0;//bandera para saber si ha pasado un dia
	
	clock.second++;
	if(clock.second >= 60)
	{
		clock.second = 0;
		clock.minute++;
		if(clock.minute >= 60)
		{
			clock.minute = 0;
			clock.hour++;
			
			if(clock.hour >= 24)
			{
				clock.hour = 0;
				day = 1;
			}
			
		}
		
	}
	
	if(day == 1)
	{
		clock.day++;
		while(valid_date(clock.day, clock.month, clock.year) == 0)
		{
			if(clock.day > 31)
			{
				clock.day = 1;
				clock.month++;
			}
			clock.day++;
		}
		clock.day--;
	}
	
	
	
}
ISR(TIMER0_COMPA_vect)//Interrupcion para modo CTC del Timer0
{	
	TCNT0 = 0x00;
	milisegs++;
	
	if(milisegs % 1000 == 0)//si ya pasó un segundo
		secFlag = 1;
	
}
uint8_t valid_date(uint8_t day, uint8_t mon, uint16_t year)//funcion para validar fecha    
{
    uint8_t is_valid = 1, is_leap = 0;//para saber si hay año bisiesto o no
    
    if (year >= 1800 && year <= 9999)//dentro del rango de 4 cifras 
    {
 
        //  saber si le año es bisiesto
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
        {
            is_leap = 1;
        }
        
        // si los meses estan entre 1 y 12
        if(mon >= 1 && mon <= 12)
        {
            // dias en febrero
            if (mon == 2)
            {
                if (is_leap && day == 29) 
                {
                    is_valid = 1;
                }
                else if(day > 28) 
                {
                    is_valid = 0;
                }
            }
            
            // dias con 30 dias
            else if (mon == 4 || mon == 6 || mon == 9 || mon == 11) 
            {
                if (day > 30)
                {
                    is_valid = 0;
					
                }
            }
 
            
            // dias con 31 dias
            else if(day > 31)
            {            
                is_valid = 0;
            }        
        }
        
        else
        {
            is_valid = 0;
        }
 
    }
    else
    {
        is_valid = 0;
    }
    
    return is_valid;
        
}