typedef unsigned int UWORD;
typedef unsigned char UBYTE;
typedef char BYTE;

#define DATABUSLINES 8
#define ADDRESSBUSLINES 12
#define PATTERN 0xFF
#define TRUE 1

extern UBYTE peek(UWORD segment, UWORD offset);/**/
extern void poke(UWORD segment, UWORD offset, BYTE data);/**/
extern void putchar(BYTE c);/*imprime un caracter*/
extern BYTE getchar();/*retorna un caracter*/


UBYTE testDataBus(const UWORD SEGMENT, const UWORD OFFSET);/*Funcion que retorna 0 si el bus de datos es correcto
caso contrario retorna las lineas de bus afectadas*/
UWORD testAddresBus(const UWORD SEGMENT, const UWORD OFFSET);
void dataBusStatus(UBYTE dataBusLinesAffected);
void addresBusStatus(UWORD AddrresLinesAffected);

void puts(BYTE *str);/*Imprime una cadena*/
void printNumber(UWORD number);/*EXTRA Imprime un number sin signo*/



int main()
{
	
	puts("------TESTING DATA BUS------\n\r");
	dataBusStatus(testDataBus(0, 0x2800));
	
	
	
	
	if(testDataBus(0, 0x2800) == 0xFF)
	{
		puts("\n\r FALLO CATASTROFICO");
		
		
	}
	else
	{
		
		puts("------TESTING ADDRESS BUS------\n\r");
	addresBusStatus(testAddresBus(0, 0x2800));
		
	}
	
	getchar();
	
	asm mov ax, 4c00h
	asm int 21h
	
	return 0;
}
void addresBusStatus(UWORD AddrresLinesAffected)
{
	UWORD i, mask = 1; 
	
	for(i = 0; i < ADDRESSBUSLINES; i++)
	{
		if((mask & AddrresLinesAffected))
		{
			puts("[ERROR] in line ");
			printNumber(i + 1);
			puts("\n\r");
		}
		else
		{
			puts(" [GOOD] in line ");
			printNumber(i + 1);
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
			printNumber(i + 1);
			puts("\n\r");
		}
		else
		{
			puts(" [GOOD] in line ");
			printNumber(i + 1);
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
void printNumber(UWORD number)
{
	unsigned int residue;
	int i = 0;
	
	while(number >= 10)
	{
		residue = number % 10;
		asm push residue
		number = number / 10;
		i++;
	}
	asm push number 
	i++;
	
	while(i > 0)
	{
		asm pop number
		putchar(number + '0');
		i--;
	}
}