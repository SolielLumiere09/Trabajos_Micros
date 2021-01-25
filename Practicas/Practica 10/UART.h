#define F_CPU 16000000UL//Frecuencia de oscilacion
#define BAUD 9600//BAUD para transmitir
#define BAUD_RATE  ((F_CPU/BAUD/16)-1)//fomrula para colocar el dato en UBBR0
#define BUFFER_SIZE 64//Tamaño del buffer circular
#define mod(x) (x&(BUFFER_SIZE-1))//modulo para el buffer circular
#define isBufferEmpty(buffer) (buffer.in_idx==buffer.out_idx)//indicar si el buffer esta vacio
#define isBufferFull(buffer) (buffer.in_idx==buffer.out_idx-1)//indicar si el buffer esta lleno
#define BACK_SPACE 8//tecla backspace
#define ENTER 13//tecla enter
#define SPACE 32//tecla para espacio 
#define STR_LEN 20//tamaño de la cadena de la funcion main

typedef struct //Estructura para manejar el buffer circular
{
	char buffer[BUFFER_SIZE];//datos para el bufffer
	volatile char in_idx;//indice de entrada de datos
	volatile char out_idx;//indice de salida de datos
}ring_buffer;

void bufferAppend(ring_buffer *buffer, char ascii);//funcion para colocar un dato en el buffer circular
char bufferRemove(ring_buffer *buffer);//funcion para quitar un dato del buffer circular

void UART_Ini(uint8_t com, uint16_t baudrate, uint8_t size, uint8_t parity, uint8_t stop);//inicializar un puerto serie

void UART_puts(uint8_t com, char *str);// Funcion que coloca un cadena en la cola circular
void UART_putchar(char com, char data);//Función que coloca un dato en la cola circular
uint8_t UART_available();// Función que retorna 1 si existe(n) dato(s) en la cola circular.
char UART_getchar();//Función toma el dato correspondiente a salir de la cola circular
void UART_gets(uint8_t com, char *str);//Funcion obtiene una cadena desde teclado usando UART
void UART_setColor(uint8_t com, uint8_t color);//Asigna color a los ascii de la consola
void UART_gotoxy(uint8_t com, uint8_t x, uint8_t y);//Posicionar el cursor
void UART_clrscr( uint8_t com );//limpiar pantalla
void UART0_AutoBaudRate();

void itoa(uint16_t number, char* str, uint8_t base);//convertir numero a cadena
uint16_t atoi(char *str);//convertir cadena a numero

enum color {BLACK = 0,RED,GREEN,YELLOW,BLUE,MAGENTA,CYAN,WHITE};//enumerador para colores
