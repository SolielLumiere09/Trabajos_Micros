#define YEAR_TO_DAYS(y) ((y)*365 + (y)/4 - (y)/100 + (y)/400)
//                      
#include <inttypes.h>

typedef struct //Definicion del tipo de dato para el reloj
{
	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}Clock;


void Timer0_Ini ( void );//funcion para incializar el timer0
uint8_t Timer0_SecFlag ( void );//funcion que retorna la bandera del timer0
uint64_t millis();//funcion que retorna los milisegundos transcurridos
void Clock_Ini(uint64_t unixtime);//funcion para inicializar el reloj
void Clock_Date_Display( );//funcion para mostrar el reloj en pantalla
void Clock_update();//funcion para actualizar el reloj
uint8_t valid_date(uint8_t day, uint8_t mon, uint16_t year);//funcion para validar la fecha 