typedef unsigned int UWORD;
typedef unsigned char UBYTE;
typedef char BYTE;

#define DATABUSLINES 8
#define ADDRESSBUSLINES 11
#define PATTERN 0xFF
#define TOTALERROR 0xFF

extern UBYTE peek(UWORD segment, UWORD offset);/**/
extern void poke(UWORD segment, UWORD offset, BYTE data);/**/
extern void putchar(BYTE c);/*imprime un caracter*/
extern BYTE getchar();/*retorna un caracter*/


UBYTE testDataBus(const UWORD SEGMENT, const UWORD OFFSET);/*Funcion que retorna 0 si el bus de datos es correcto
caso contrario retorna las lineas de bus afectadas*/
UWORD testAddresBus(const UWORD SEGMENT, const UWORD OFFSET);
void dataBusStatus(UBYTE dataBusLinesAffected);
void addresBusStatus(UWORD AddrresLinesAffected);
void testMemory(UWORD SEGMENT, UWORD OFFSET);


void puts(BYTE *str);/*Imprime una cadena*/
void printNumber(UWORD number, UWORD base);/*EXTRA Imprime un entero sin signo*/

int main()
{
	UWORD OFFSET = 0xC000;/*Direccion inicial */
	UWORD SEGMENT = 0;

	puts("FOR OFFSET = ");
	printNumber(OFFSET, 16);
	puts("h\n\r");
	testMemory(SEGMENT, OFFSET);
	
	OFFSET = 0X4000; /*Direccion inicial espejo */
	puts("FOR OFFSET = ");
	printNumber(OFFSET, 16);
	puts("h\n\r");
	testMemory(SEGMENT, OFFSET);
	
	
	OFFSET = 0x3000;/*Direccion no valida*/
	puts("FOR OFFSET = ");
	printNumber(OFFSET, 16);
	puts("h\n\r");
	testMemory(SEGMENT, OFFSET);
	
	asm mov ax, 4c00h
	asm int 21h
	
	getchar();
	return 0;
}
void testMemory(UWORD SEGMENT, UWORD OFFSET)
{
	UBYTE busStatus = testDataBus(SEGMENT, OFFSET);
	
	puts("------TESTING DATA BUS------\n\r");
	dataBusStatus(busStatus);
	
	if(busStatus != TOTALERROR)
	{
		puts("------TESTING ADDRESS BUS------\n\r");
		addresBusStatus(testAddresBus(SEGMENT, OFFSET));
	}
	else
		puts("\r\nFALLO CATASTROFICO");
}
void addresBusStatus(UWORD AddrresLinesAffected)
{
	UWORD i, mask = 1; 
	
	for(i = 0; i < ADDRESSBUSLINES; i++)
	{
		if((mask & AddrresLinesAffected))
		{
			puts("[ERROR] in line ");
			printNumber(i + 1, 10);
			puts("\n\r");
		}
		else
		{
			puts(" [GOOD] in line ");
			printNumber(i + 1, 10);
			puts("\n\r");
			
		}
		mask = mask << 1;
	}

	
}
void dataBusStatus(UBYTE dataBusLinesAffected)
{
	UBYTE i, mask = 1;
	
	for(i = 0; i < DATABUSLINES; i++)
	{
		
		if((mask & dataBusLinesAffected))
		{
			puts("[ERROR] in line ");
			printNumber(i + 1, 10);
			puts("\n\r");
		}
		else
		{
			puts(" [GOOD] in line ");
			printNumber(i + 1, 10);
			puts("\n\r");
		}
		mask = mask << 1;
		
	}

}
UWORD testAddresBus(const UWORD SEGMENT, const UWORD OFFSET)
{
	UWORD i = 0, mask = 1; 
	UWORD AddrresLinesAffected = 0;
	
		
	/*Escribir un dato en direcciones potencias 2*/
	for(i = 0; i < ADDRESSBUSLINES; i++)
	{
		poke(SEGMENT, OFFSET + mask, 0);
		mask = mask << 1;
	}
	
	/*Escribir un patron para ver si las direccions no se sobreponen*/
	poke(SEGMENT, OFFSET, PATTERN);
	mask = 1;
	
	/*Recuperar datos escritos*/
	for(i = 0; i < ADDRESSBUSLINES; i++)
	{

		if(peek(SEGMENT, OFFSET + mask) == PATTERN || peek(SEGMENT, OFFSET + mask) != 0)
			AddrresLinesAffected |= mask; /*Error en la linea actual*/
		mask = mask << 1;
	}
	
	return AddrresLinesAffected;
	
}
UBYTE testDataBus(const UWORD SEGMENT, const UWORD OFFSET)
{
	UBYTE dataBusLinesAffected = 0;
	UBYTE i = 0, mask = 1;
	
	
	for(i = 0; i < DATABUSLINES; i++)
	{
		poke(SEGMENT, OFFSET, mask);/*Escribe el primer dato 0000 0001*/
		if(peek(SEGMENT, OFFSET) != mask)/*Si al recuperar no es el dato que puse, hay problema en lineas de datos*/
			dataBusLinesAffected |= mask;/*Este OR es para indicar que en la linea de mask hay un problema*/
		
		mask = mask << 1;
	}
	return dataBusLinesAffected;
	
}
void puts(char *str)
{	
	while(*str)
		putchar(*str++);

}
void printNumber(UWORD number, UWORD base)
{
	unsigned int residue;
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