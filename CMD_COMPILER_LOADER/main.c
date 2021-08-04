/*Тестовая прогрма предназначенная для создания make*/

#include <stdio.h>

#define NOP asm("nop;;")

#define	INT	125

//Запись в файл
static void WRITE_DATA_BIN(void)
{
	fputs("OUTPUT DSP\n",stdout);
	for(int i = 0; i<2000000;i++)	{NOP;}
}

int main(void)
{
    WRITE_DATA_BIN();
	return 0;
}
