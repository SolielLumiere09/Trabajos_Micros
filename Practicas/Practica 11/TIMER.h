#include <inttypes.h>

void Timer0_Ini ( void );//funcion para incializar el timer0
uint8_t Timer0_SecFlag ( void );//funcion que retorna la bandera del timer0
uint64_t millis();//funcion que retorna los milisegundos transcurridos
void resetMillis();