#define LED_BLINK 20//sensibilidad para el flash
#define FLASH_SENSIBILITY 40//sesnsibilidad para los tiempos
#define NEXT_SENSIBILITY 200//sensibilidad para cambio de caracter
#define SPACE_SENSIBILITY 800//sensibilidad para los espacios entre caracter

#define DOT '1'//pulso pequeño
#define DASH '2'//pulso largo

#include<avr/io.h>
#include "UART.h"
#include "TIMER.h"


uint8_t test_ADC();//Funcion para calcular el OFFSET del ADC
void init_ADC();//Inicializar ADC para el sensado del codigo morse
uint16_t ADC_read();//Lectura del ADC
uint8_t readASCII(char cad[]);//Funcion para leer los DOT y DASH y colocarlos en una cadena
char getSymbol();//funcion que retorna DOT o DASh dependiendo del timepo transcurrido
char SymbolsToASCII(char cad[]);//COnvierte una cadena de DOT y DASH en su LETRA correspondiente
int8_t strcmp(const char *X, const char *Y);//Funcion para comparar cadenas

uint64_t start = 0;//variables para medir los tiempo de los Flashes
uint64_t finish = 0;//

uint64_t start_wait = 0;//Variables para medir los tiempos de espera
uint64_t finish_wait = 0;//

uint8_t ADC_OFFSET;//OFFSET del ADC 

char msg[80];//Variable para guardar el mensaje

int main()
{
	char cad[10];//cadena para guardar DOT y DASH
	uint8_t i = 0;//indice para msg
	
	uint8_t flag = 0, msgFlag = 0, value;
	UART_clrscr(0);
	
	UART_Ini(0, BAUD_RATE, 8, 0, 1); //inicializar UART
	ADC_OFFSET = test_ADC();
	init_ADC();//inicializar ADC
	
	
	UART_puts(0, "Preciona un tecla para iniciar..\n\r");
	while(!UART_available());
	UART_puts(0, "Puedes empezar a mandar el mensaje..\n\r");
	UART_puts(0, "Preciona cualquier tecla para finalizar\n\r");
	while(UART_available())
	{
		UART_getchar();
	}
	while(1)
	{
		value = readASCII(cad);
		if(value == 1 && flag == 1)
		{
			msg[i] = SPACE;
			flag = 0;
			i++;
			msg[i + 1] = '\0';
		}
			
		else if(value == 0)
		{
			msg[i] = SymbolsToASCII(cad);
			flag = 1;
			i++;
			msg[i + 1] = '\0';
		}			
		while(UART_available() && msgFlag == 0)
		{
			msg[i-1] = '\0';
			UART_puts(0, "Mensaje enviado es:\n\r");
			UART_puts(0, msg);
			msgFlag = 1;
			
			
		}
		
	}
	
	
	return 0;
}
uint8_t test_ADC()
{
	char cad[10];
	ADMUX |= (1 << REFS0); //AVCC reference 
	ADMUX |= (0X1F << MUX0); //GND AS VIN FOR CALIBRTAE
	ADCSRA |= (1 << ADEN);//enables ADC 
	ADCSRA |= (7 << ADPS0);//prescaler 128 
	
	ADCSRA |= (1 << ADSC);//START CONVERTION
	
	while(!(ADCSRA & (1 << ADIF)));//waits for convertion
	
	//I should see a 0 value at the output
	itoa(ADC, cad, 10);
	UART_puts(0, "Valor OFFSET es: ");
	UART_puts(0, cad);
	UART_puts(0, "\n\r");
	
	ADMUX = 0;
	ADCSRA = 0;//regresar configuracion ADC
	return (uint8_t)ADC;
}
void init_ADC()
{
	ADMUX |= (1 << REFS0);//AVCC reference
	ADMUX |= (5 << MUX0);//USING ADC5 
	ADCSRA |= (1 << ADEN);//enables ADC
	ADCSRA |= (7 << ADPS0);//prescaler 128 
}
uint16_t ADC_read()
{
	ADCSRA |= (1 << ADSC);//START CONVERTION
	while(!(ADCSRA & (1 << ADIF)));//waits for convertion
	return ADC - ADC_OFFSET;
}
uint8_t readASCII(char cad[])
{
	uint8_t index = 0;
	uint8_t control_flag = 0;
	Timer0_Ini();//Inicalizar timer
	
	
	do{
		start_wait = millis();//empezar a contar el tiempo de espera 
		while(ADC_read() < LED_BLINK)//Esperar por la linterna
		{
			finish_wait = millis();//terminar tiempo de espera 
			
			if(finish_wait - start_wait > 2000 - SPACE_SENSIBILITY && control_flag == 0)//si ya paso 2 segundos sin flash
			{
				//espacio
				start = 0;
				finish = 0;
				return 1;	
			}
			if(finish_wait - start_wait > 1000 - NEXT_SENSIBILITY  && control_flag == 1)//si ya paso 1 segundo sin flash
			{
				//acaba caracter	
				start = 0;
				finish = 0;
				return 0;
				
			}
		}
		
		start = millis();//esperar tiempo de la linterna 
		
		while(ADC_read() > LED_BLINK);
			//Esperar a que se apage
			
		finish = millis();
		cad[index] = getSymbol();//obtener simbolo
		cad[index + 1] = '\0';//almacenarlo en cadena
		index++;
		
		control_flag = 1;
	}while(1);
	


}
char getSymbol()
{
	if(finish - start < 300 + FLASH_SENSIBILITY)//300 milisegundos para DOT
		return DOT;
	else if(finish - start < 1000 + FLASH_SENSIBILITY)//1000 milisegundos para DASH
		return DASH;
	
	return 0;
}
char SymbolsToASCII(char cad[])
{
	char ascii = ' ';
	//depiendo lo que contenga la cadena toma los siguentes valores
	if(strcmp(cad, "12") == 0)ascii = 'A';
	else if(strcmp(cad, "2111") == 0)ascii = 'B';
	else if(strcmp(cad, "2121") == 0)ascii = 'C';
	else if(strcmp(cad, "211") == 0)ascii = 'D';
	else if(strcmp(cad, "1") == 0)ascii = 'E';
	else if(strcmp(cad, "1121") == 0)ascii = 'F';
	else if(strcmp(cad, "221") == 0)ascii = 'G';
	else if(strcmp(cad, "1111") == 0)ascii = 'H';
	else if(strcmp(cad, "11") == 0)ascii = 'I';
	else if(strcmp(cad, "1222") == 0)ascii = 'J';
	else if(strcmp(cad, "212") == 0)ascii = 'K';
	else if(strcmp(cad, "1211") == 0)ascii = 'L';
	else if(strcmp(cad, "22") == 0)ascii = 'M';
	else if(strcmp(cad, "21") == 0)ascii = 'N';
	else if(strcmp(cad, "222") == 0)ascii = 'O';
	else if(strcmp(cad, "1221") == 0)ascii = 'P';
	else if(strcmp(cad, "2212") == 0)ascii = 'Q';
	else if(strcmp(cad, "121") == 0)ascii = 'R';
	else if(strcmp(cad, "111") == 0)ascii = 'S';
	else if(strcmp(cad, "2") == 0)ascii = 'T';
	else if(strcmp(cad, "112") == 0)ascii = 'U';
	else if(strcmp(cad, "1112") == 0)ascii = 'V';
	else if(strcmp(cad, "122") == 0)ascii = 'W';
	else if(strcmp(cad, "2112") == 0)ascii = 'X';
	else if(strcmp(cad, "2122") == 0)ascii = 'Y';
	else if(strcmp(cad, "2211") == 0)ascii = 'Z';
	
	
	
	
	return ascii;
	
}
int8_t strcmp(const char *X, const char *Y)
{
	while(*X)
	{
		if (*X != *Y)//si los caracteres son diferentes
			break;
		X++;
		Y++;
	}

	//regresar la diferencia de ASCII
	return *(const unsigned char*)X - *(const unsigned char*)Y;
}
