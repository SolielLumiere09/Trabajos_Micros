#define setBitPort(port,bit) __asm__("sbi %0,%1" : :"I" ((uint8_t) (_SFR_IO_ADDR(port))) , "I" ((uint8_t) (bit)))
#define clrBitPort(port,bit) __asm__("cbi %0,%1" : :"I" ((uint8_t) (_SFR_IO_ADDR(port))) , "I" ((uint8_t) (bit)))
#define BYTE unsigned char
#define TURNOFFLEDS DDRF = 0x00;PORTF = 0x000;//macro para apagar todos los leds
#define ChooseBitToSet() if(i == 0)clrBitPort(PORTA, PA0);if(i == 1)clrBitPort(PORTA, PA1);if(i == 2)clrBitPort(PORTA, PA2);
#define ChooseBitToClr() if(i == 0)setBitPort(PORTA, PA0);if(i == 1)setBitPort(PORTA, PA1);if(i == 2)setBitPort(PORTA, PA2);
#define INIT_SEED 17

#include<avr/io.h>

extern BYTE getRandom(BYTE Xn);//Dada una semilla regresa la siguiente de la sucesión
extern void delay_1S();//retardo de 1 segundo para encender un LED
extern void delay_1mS();//Retardo de 1ms de soporte para las variables 'volatile'

BYTE getKeyPressed();//retorna 0 si no se ha precionado una tecla, caso contrario retorna el numero de la tecla
void turnOnLed(BYTE LED);//enciente el led indicado
void initGame();//secuencia de leds al inicio del juego e inicializador
void gameOverState();//Secuencia medio nibble encendido/apagado 
BYTE game(BYTE seed);//Lógica del juego
void fill_list_leds(BYTE V[], BYTE Xn);//Crear la sucesión requerida
void turnOnLedByByte(BYTE number);//Presenta el numero dado en los leds

int main()
{
	BYTE seed = INIT_SEED;//inicia con una semilla que sea numero primo
	DDRA = 0x07;//configuracion de los puertos para las teclas 
	PORTA = 0x38;//configurados adecuadamente 
	delay_1S();//Pequeño retardo antes de iniciar el juego
	
	initGame();
	while(1)
	{	

		if(game(seed) == 1)
			initGame();//caso de ganar
		else
			gameOverState();//caso de perder
		
		seed++;//se aumenta la semilla para la siguiente partida
	}

}
void gameOverState()
{
	volatile BYTE ticks = 33;// 130 ticks ~= 1 segundo, por lo tanto 33 ticks ~= 250ms 
	BYTE mask = 0x0F;//mascara para el nibble
	
	do
	{
		while(--ticks)
		{	
			turnOnLedByByte(mask);
		}
		mask = (mask >> 4) | (mask << 4);//swap nibbles		
		ticks = 33;
	}while(getKeyPressed() == 0);	
	
	
}
void fill_list_leds(BYTE V[], BYTE Xn)//Genera una lista de numeros aleatorios
{
	BYTE i;
	
	for(i = 0; i < 6; i++)
	{
		Xn = getRandom(Xn);
		V[i] = Xn;
	}
}
BYTE game(BYTE seed)
{
	BYTE i, j, k, keyPressed, flag;//variables requeridas
	BYTE led_list[6];//Lista de numeros para la secuencia 
	
	fill_list_leds(led_list, seed);//Llenar el arreglo con 6 numeros aleatorios del 0 al 8
	
	for(i = 0; i < 6; i++)//ciclio del juego principal
	{
		for(j = 0; j < i; j++)//ciclio para encender la secuencia de los leds
		{
			turnOnLed(led_list[j]);//encender la secuencia dada
			delay_1S();//retardo para mantener apagado los leds			
			TURNOFFLEDS//macro para apagar los leds
		}
		
		for(k = 0; k < i; k++)//ciclio para que el usuario responda con las teclas correctas
		{
			flag = 0;//bandera para controlar el antirrebote del pulsador
			while(flag == 0)//mientras no se haya precionado una tecla
			{
				keyPressed = getKeyPressed();//obtener del teclado la tecla precionada
				if(keyPressed != 0)//si ya se precionó una tecla
					flag = 1;
			}
			while(getKeyPressed() != 0){//mientras el pulsador siga pulsado
				
				asm("nop");//no operación
			}
			
			if(keyPressed - 1 != led_list[k])//si la tecla precionada no coincide, FIN DEL JUEGO
				return 0;			
			
		}
	}
	
	
	return 1;//Haz ganado
}
void initGame()//inicializador del juego
{
	volatile BYTE ticks = 13;//130  ticks ~= 1 segundo, 13 ticks = 100ms
	BYTE mask = 0x7F;//inicializador para el walking 0

	
	while(getKeyPressed() == 0)//mientras no se haya precionado una tecla
	{
		while(--ticks)//actualizando LEDS mientras pasa el tiempo, ya que no puedo crear un retardo
		{
			turnOnLedByByte(mask);
		}
		mask = (mask >> 1) | (mask << 7);//Rotacion para walking 0 
		
		ticks = 13;//regresar los ticks originales
		if(mask == 0xFF)//si hay unos, regresar al valor original 
			mask = 0x7F;
	}
}
void turnOnLedByByte(BYTE number)
{
	BYTE i = 0, mask = 0x01;//mascara y ciclio
	
	for(i = 0; i < 8; i++)
	{
		if((number & mask) > 0)
			turnOnLed(i);//presentar el numero en los leds 
		mask = mask << 1;//encender el siguiente led
		
		delay_1mS();//retardo para las varibles volatile
	}
	
	
	
}
BYTE getKeyPressed()
{
	BYTE i, j;//recorer la matriz de teclas
	BYTE temp;//temporal para obtener el valor
	
	PORTA = 0x3E;//configuracion adecuada
	
	for(i = 0; i < 3; i++)//ciclio para las filas
	{
	
		ChooseBitToSet()//esto debido a que setBitPort recibe una constante para el numero de bit y no una variable
		for(j = 3; j < 6; j++)//ciclio para las columnas 
		{
			temp = PINA >> j;//leer del puerto y recorrer para obtener un bit
			temp = temp & 0x01;//aplicar una mascara para desactivar todos los bits
			
			if(temp == 0)//como estoy leyendo GND para saber si una tecla se precionó
			{
				
				return (i * 3 + (j - 3) + 1);//retornar la tecla precionada
			}
		}
		ChooseBitToClr()//esto es debido a que clrBitPort recibe una constante para el numero de bit y no una variable

		
	}
	
	return 0;
}
void turnOnLed(BYTE LED)//encender un led dado 
{
	
	switch(LED)//dependiendo el led a encender se configuran los puertos
	{
		case 0:	DDRF = 0x03;
				PORTF = 0x03;
				setBitPort(PORTF, PF1);
				clrBitPort(PORTF, PF0);
				break;
				
		case 1:	DDRF = 0x03;
				PORTF = 0x03;
				setBitPort(PORTF, PF0);
				clrBitPort(PORTF, PF1);
				break;
				
		case 2:	DDRF = 0x06;
				PORTF = 0x06;
				setBitPort(PORTF, PF2);
				clrBitPort(PORTF, PF1);
				break;
				
		case 3:	DDRF = 0x06;
				PORTF = 0x06;
				setBitPort(PORTF, PF1);
				clrBitPort(PORTF, PF2);
				break;
				
		case 4:	DDRF = 0x05;
				PORTF = 0x05;
				setBitPort(PORTF, PF0);
				clrBitPort(PORTF, PF2);
				break;
				
		case 5:	DDRF = 0x05;
				PORTF = 0x05;
				setBitPort(PORTF, PF2);
				clrBitPort(PORTF, PF0);
				break;
		
		case 6:	DDRF = 0x0C;
				PORTF = 0x0C;
				setBitPort(PORTF, PF3);
				clrBitPort(PORTF, PF2);
				break;
				
		case 7:	DDRF = 0x0C;
				PORTF = 0x0C;
				setBitPort(PORTF, PF2);
				clrBitPort(PORTF, PF3);
				break;
	}
	
	
	
}
