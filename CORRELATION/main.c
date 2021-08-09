#include "H.h"

#define N 65536//КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВЕ ПРИЁМНИКА

/*ОПРЕДЕЛЕНИЯ ТИПА ПАМЯТИ И ТИПА ПЕРЕДАЧИ*/
static int32_t HAL_DMA_INIT_MEM_TYPE( const void* addr );

static uint32_t __attribute__((aligned(4))) TABLE_FLASH[N];


int main(void)
{
  /*ВКЛЮЧЕНИЕ И ИНИЦИАЛИЗАЦИЯ FLASH ПАМЯТИ*/
  Driver_NANDSamsung_Enable(K9F2G08X0B);
  LX_NAND->CR.word = 0x0;
  LX_NAND->WCT_CFG.word = 0xFF;
	LX_NAND->IO_CFG.word = 0x00A3FFFE;
  return 0;
}



/*

void Driver_NANDSamsung_Enable(NAND_Samsung_type numModel){
	//NAND
	LX_NAND->CR.word = 0x0;
	LX_NAND->WCT_CFG.word = 0xFF;
	LX_NAND->IO_CFG.word = 0x00A3FFFE;
	switch(numModel){
	case K9F8G08UXM:
		LX_NAND->NAND_CFG.word = 0x00E5035C;	//page size = 4kB
		break;
	case K9F2G08X0B:
		LX_NAND->NAND_CFG.word = 0x00E5035B;	//page size = 2kB
		break;
	}
	//GPIO
	*(unsigned *)PX_ALT_LOC = 0xFC;
	LX_NAND->CR.word = 0x1;
}





*/