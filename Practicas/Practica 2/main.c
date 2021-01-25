extern void putchar(char c);/*imprime un caracter*/
extern char getchar();/*retorna un caracter*/

void puts(char *str);/*Imprime una cadena*/
void gets(char *str);/*Retorna una cadena capturada en str*/
void itoa(unsigned int number, char* str, unsigned char base);/*Función que convierte un número de 16 bits a su representación alfanumérica en la base dada, y la
retorna en el apuntador str.*/
unsigned int atoi(char *str); /*Función que convierte una cadena numérica (de base
10) y retorna su valor numérico en 16 bits.*/

unsigned int getCenturyConstant(unsigned int year);/*EXTRA función que retorna la constante de siglo de algun siglo*/
unsigned int getIndex(unsigned int day, unsigned int month, unsigned int year);/*EXTRA retorna el dia que pertenece la fecha*/
unsigned int trimYear(unsigned int year);/*EXTRA Retorna las dos primeras cifras del del año*/
unsigned int getMonthIndex(unsigned int month, unsigned int year);/*EXTRA Retorna el indice del mes, ya se bisiesto o no*/
void printDate(char dayStr[], char monthStr[], char yearStr[]);/*EXTRA imprime la fecha*/
void captureData(char dayStr[], char monthStr[], char yearStr[]);/*EXTRA Capturar datos del usuario*/
unsigned int verifyDate(char dayStr[], char monthStr[], char yearStr[]); /*EXTRA función que verirfica si la fecha introducida es válida*/
unsigned int leapYear(unsigned int year);/*EXTRA retorna 1 si el año es bisiesto*/

char day[3];
char month[3];
char year[4];

char monthArray[12][20] = 
{
" De Enero del ",
" De Febrero del ",
" De Marzo del ",
" De Abril del ",
" De Mayo del ",
" De Junio del ",
" De Julio del ",
" De Agosto del ",
" De Septiembre del ",
" De Octubre del ",
" De Noviembre del ",
" De Diciembre del ",
};

char dayArray[7][15] = 
{
	"Domingo ",
	"Lunes ",
	"Martes ",
	"Miercoles ",
	"Jueves ",
	"Viernes ",
	"Sabado "
	
};

int main(void)
{
	
	captureData(day, month, year);
	
	if(verifyDate(day, month, year) == 1)
		printDate(day, month, year);
	else 
		puts("Fecha invalida ");
	
	
	getchar();
	return 0;
}
unsigned int atoi(char *str)
{
	int i;
	unsigned int sum = 0;
	
	for(i = 0; str[i] != '\x0'; i++)
		sum = sum * 10 + str[i] - '0';
	
	return sum;
}
void itoa(unsigned int number, char* str, unsigned char base)
{
	unsigned int residue;
	int i = 0, j = 0;
	
	while(number >= base)
	{
		residue = number % base;
		asm push residue
		number = number / base;
		j++;
	}
	asm push number 
	j++;
	
	while(j > 0)
	{
		asm pop number
		
		if(number >= 10)
			str[i] = number + '7';
		else 
			str[i] = number + '0';
		
		j--;
		i++;
	}
	str[i] = '\x0';
	
}
void puts(char *str)
{	
	while(*str)
		putchar(*str++);

}
void gets(char *str)
{
	char key = 0;
	int i = 0;
	
	
	while(1)
	{
		key = getchar();
		
		if(key == 13)
			break;
		
		if(key == 8 && i >= 0)
		{
			str[i] = '\0';
			i--;
			putchar(' ');
			putchar(8);
			
		}
		else 
		{
			str[i] = key;
			str[i + 1] = '\0';
			i++;
		}
		if(i < 0)
			i = 0;	
	}
}
unsigned int getCenturyConstant(unsigned int year)
{
	
	if(year >= 100)
		year = year / 100;
	
	switch(year % 4)
	{
		case 0:	return 6;
		case 1: return 4;
		case 2: return 2;
		case 3: return 0;
	}
}
unsigned int getIndex(unsigned int day, unsigned int month, unsigned int year)
{
	return ((day +  getMonthIndex(month, year) + (trimYear(year) + (trimYear(year) / 4) + getCenturyConstant(year)))) % 7;
	
}
unsigned int trimYear(unsigned int year)
{
	unsigned int residue, sum = 0;
	
	if(year < 100)
		return year;
	
	residue = year % 10; 
	asm push residue
	year = year / 10;
	residue = year % 10;

	sum = sum * 10 + residue;
	asm pop residue 
	sum = sum * 10 + residue;
	
	return sum;
}
unsigned int leapYear(unsigned int year)
{	
	if(year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
		return 1;
	else 
		return 0;
	
}
unsigned int getMonthIndex(unsigned int month, unsigned int year)
{
	switch(month)
	{
		case 1:	if(leapYear(year) == 1)
					return 6;
				else return 2;
				
		case 2:	if(leapYear(year) == 1)
					return 2;
				else return 3;
				
		case 3:	return 3;
		case 4:	return 6;
		case 5:	return 1;
		case 6:	return 4;
		case 7:	return 6;
		case 8:	return 2;
		case 9:	return 5;
		case 10:	return 0;
		case 11:	return 3;
		case 12:	return 5;
	}
	
	
	return -1;
}
void printDate(char dayStr[], char monthStr[], char yearStr[])
{
	unsigned int day = atoi(dayStr);
	unsigned int month = atoi(monthStr);
	unsigned int year = atoi(yearStr);

	puts(dayArray[getIndex(day, month, year)]);
	
	itoa(month, monthStr, 10);
	itoa(day, dayStr, 10);
	itoa(year, yearStr, 10);
	
	puts(dayStr);
	puts(monthArray[month - 1]);
	puts(yearStr);
	
}
void captureData(char dayStr[], char monthStr[], char yearStr[])
{
	puts("Introduce el Dia ----> \n\r");
	gets(dayStr);
	
	puts("Introduce el numero de Mes ----> \n\r");
	gets(monthStr);
	
	puts("Introduce el a");
	putchar(164);
	puts("o ----> \n\r");
	gets(yearStr);
}
unsigned int verifyDate(char dayStr[], char monthStr[], char yearStr[])
{
	unsigned int day = atoi(dayStr);
	unsigned int month = atoi(monthStr);
	unsigned int year = atoi(yearStr);
	
	if(month > 12 || day > 31 || year > 9999 || day == 0 || month == 0)
		return 0;
	
	if(leapYear(year) == 1 && month == 2 && day <= 29)
		return 1;
	
	if(month == 2 && day > 29 || month == 11 && day > 30 || month == 9 && day > 30 || month == 6 && day > 30 || month == 4 && day > 30 || (leapYear(year) == 0 && month == 2 && day > 28))
		return 0;
	
	return 1;
}