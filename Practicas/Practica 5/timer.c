#define INTR 8 /* Utilizar el vector adecuado */
volatile static unsigned char SecFlag; /*Bandera para indicar el Seg. */
/* Manejador de Interrupcion: ocurre 20 veces por segundo */

extern void pokeW(unsigned int segment, unsigned int offset, unsigned int value);

void interrupt ManejadorISR( void )
{
	static unsigned int Cont = 0;
	Cont++; /* Conteo de Ticks */
	if(Cont == 20)
	{
		SecFlag=1;
		Cont = 0;
	}
} 
unsigned char TimerSecFlag( void )
{
	if( SecFlag == 1 )
	{
		SecFlag = 0;
		return 1;
	}else
		return 0;
} 
void Timer_Ini( void )
{
	SecFlag=0;
	pokeW(0x0,INTR*4, 0x100 + (unsigned int) ManejadorISR); /* Determinar offset */
	pokeW(0x0,(INTR*4)+2,_CS);
	
}