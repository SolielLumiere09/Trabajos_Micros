#include "Timer.h"
#define BYTE unsigned char
#define WORD unsigned int

void UpdateClock( void );
void DisplayClock( void );
void Delay( BYTE );

/*Manejo del tiempo*/
void printNumber(WORD number, WORD base);/*EXTRA Imprime un entero sin signo*/
extern void putchar(BYTE c);/*imprime un caracter*/
extern void pokeB(WORD segment, WORD offset, BYTE value);
extern BYTE getchar();/*retorna un caracter*/
void SetClock( BYTE hr, BYTE min, BYTE sec);
void SetDate( BYTE day, BYTE month,WORD year);


/*Para mostrar la fecha*/
void printDate(BYTE day, BYTE month, WORD year);
BYTE verifyDate(BYTE day, BYTE month, WORD year);
BYTE leapYear(WORD year);
void puts(char *str);


BYTE seg=0, mins = 0, hour = 0; /* para el manejo de los segundos */
WORD years = 2020; 
BYTE months = 3, days = 6;


void main()
{
	Timer_Ini(); /* inicialzaTimer de 1 segundo */
	pokeB(0, 0xFFFF, 0);
	SetDate(31, 12, 2020);
	SetClock(23, 59, 50);
	
	
	while(1)
	{
		/* Verificación para actualizar el reloj */
		if(TimerSecFlag()) /* Ha pasado un segundo? */
		{ 	
			UpdateClock(); /* actualiza y muestra el reloj */
			DisplayClock(); /* desplegar reloj en 0,0 */
			putchar('\n');
			putchar('\r');
			
			pokeB(0, 0xFFFF, seg);
		}
		/* otras cosas por hacer */
		Delay(100);
		
	}
	
}
void SetClock( BYTE hr, BYTE min, BYTE sec)
{
	hour = hr;
	mins = min;
	seg = sec;
}
void SetDate( BYTE day, BYTE month,WORD year)
{
	days = day;
	months = month;
	years = year;
	
}
void UpdateClock( void ){

	seg++;
	if(seg == 60)
	{
		seg = 0;
		mins++;
		if(mins == 60)
		{
			mins = 0;
			hour++;
			if(hour == 24)
			{
				hour = 0;
				days++;	
				
					while(verifyDate(days, months, years) == 0)
					{
						days++;
						
						if(days > 31)
						{
							days = 1;
							months++;
							
							if(months == 13)
							{
								days = 1;
								months = 1;
								years++;			
									
							}
							
						}
							
					}
				
				
			}
		}
		
	}
	

}
void DisplayClock( void )
{
	printDate(days, months, years);
	putchar(' ');
	putchar(' ');
	printNumber(hour, 10);
	putchar(':');
	printNumber(mins, 10);
	putchar(':');
	printNumber(seg, 10);

	
}
void Delay( BYTE i)
{
	while(--i);
}
void printNumber(WORD number, WORD base)
{
	WORD residue;
	int i = 0;
	
	while(number >= base)
	{
		residue = number % base;
		asm push residue
		number = number / base;
		i++;
	}
	asm push number 
	i++;
	
	while(i > 0)
	{
		asm pop number
		
		if(number >= 10)
			putchar(number + '7');
		else
			putchar(number + '0');
		
		i--;
	}
}
void printDate(BYTE day, BYTE month, WORD year)
{

	putchar(' ');
	printNumber(day, 10);
	putchar('/');
	printNumber(month, 10);
	putchar('/');
	printNumber(year, 10);

}
BYTE verifyDate(BYTE day, BYTE month, WORD year)
{
	
	if(month > 12 || day > 31 || year > 9999 || day == 0 || month == 0)
		return 0;
	
	if(leapYear(year) == 1 && month == 2 && day <= 29)
		return 1;
	
	if(month == 2 && day > 29 || month == 11 && day > 30 || month == 9 && day > 30 || month == 6 && day > 30 || month == 4 && day > 30 || (leapYear(year) == 0 && month == 2 && day > 28))
		return 0;
	
	return 1;
}
BYTE leapYear(WORD year)
{	
	if(year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
		return 1;
	else 
		return 0;
	
}

