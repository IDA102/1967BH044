#include "H.h"

#define N 65536//КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВЕ

/*ОПРЕДЕЛЕНИЯ ТИПА ПАМЯТИ И ТИПА ПЕРЕДАЧИ*/
static int32_t HAL_DMA_INIT_MEM_TYPE( const void* addr );

static uint32_t __attribute__((aligned(4))) TABLE_DATA_FLASH[N];

static void NAND_GET_SIZE_PAGE(uint32_t* RAZMER);

int main(void)
{
  SDRAM_type MODEL   = 1;
  uint32_t PAGE_SIZE = 0;
  
  /*ИНИЦИАЛИЗАЦИ И ЯВКЛЮЧЕНИЕ FLASH ПАМЯТИ*/
  NAND_CR = NAND_DISABLE;//	LX_NAND->CR.word = 0x0;
  NAND_WCT_CFG = ( NAND_ENWT_1 |
                   NAND_CCSE_1 | 
                   NAND_TOE_0 );/*LX_NAND->WCT_CFG.word = 0xFF;*/
  
  NAND_IO_CFG = ( NAND_CSCA_7 | NAND_CA_32  |
                  NAND_BWD_8  | NAND_BRT_15 |
                  NAND_BHT_8  | NAND_MDT_0  |
                  NAND_VGA_1  | NAND_CSKPL_1);/*LX_NAND->IO_CFG.word = 0x00A3FFFE;*/
  switch(MODEL)
  {
    case K9F8G08UXM:
      /*размер страницы 4КB*/
      NAND_NAND_CFG = ( NAND_VOLCOL_4096       | NAND_ROWBT_3           |
                        NAND_COMCLW_2          | NAND_COMCLR_2          |
                        NAND_ADRCL_LINE_COLUMN | NAND_ADRSC_HAND_OVER   |
                        NAND_DATSC_HAND_OVER   | NAND_COMSC_HAND_OVER   |
                        (0x050000)             | (0xE00000) );// 0x00E5035C                
      break;
    case K9F2G08X0B:
      /*размер страницы 2КВ*/
      NAND_NAND_CFG = ( NAND_VOLCOL_2048       | NAND_ROWBT_3           |
                        NAND_COMCLW_2          | NAND_COMCLR_2          |
                        NAND_ADRCL_LINE_COLUMN | NAND_ADRSC_HAND_OVER   |
                        NAND_DATSC_HAND_OVER   | NAND_COMSC_HAND_OVER   |
                        (0x050000)             | (0xE00000) ); // 0x00E5035B
      break;
	}
  
  /*Включение альтернативной функции GPIO для доступа к NAND*/
	*(unsigned *)PX_ALT_LOC = 0xFC; //
	NAND_CR = NAND_ENABLE; // LX_NAND->CR.word = 0x1;
  
  /*Размер страницы в Кбайтах, на работу с которым сейчас настроен NAND контролер.*/
  GET_SIZE_NAND_PAGE(PAGE_SIZE);
  
  /*Очистка блока памяти*/
  Driver_NANDSamsung_EraseBlocks(0, 1); 
  
  /*Запись данных в NAND память*/
  int MASS_IN[100];
  for(uint32_t i = 0 ; i < 100 ; i++)
  {
    MASS_IN[i] = i+1;
  }
  Driver_NANDSamsung_WriteBust(&MASS_IN, 0, 100);
  
  
  /*Чтение данных из NAND памяти*/
  int MASS_OUT[100];
  Driver_NANDSamsung_ReadBurst((unsigned int *) 0, &MASS_OUT, 100);
  
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
 
int Driver_NANDSamsung_GetPageSize(void){
	int pageSizeByte;
	pageSizeByte = (256<<(LX_NAND->NAND_CFG.word & 7));
	if (pageSizeByte == 256) pageSizeByte = 128;
	return pageSizeByte;
}

void Driver_NANDSamsung_EraseMem(int memSize){
	int amountBlocks, pageSize;
	pageSize = 256<<(LX_NAND->NAND_CFG.word & 7);
	if (pageSize == 256) pageSize = 128;
	amountBlocks = ((1024*1024)*memSize)/(64*pageSize);
	Driver_NANDSamsung_EraseBlocks(0, amountBlocks);
}

void Driver_NANDSamsung_EraseBlocks(int numBlock, int amount){
	int tmp, shAr, i;
	LX_NAND->RD_CFG.word = ERASE_COMM;
	tmp = LX_NAND->NAND_CFG.word & 0xFFFF000F;
	tmp |= 0x2240;
	LX_NAND->NAND_CFG.word = tmp;
	shAr = LX_NAND->NAND_CFG.word & 7;		//get min bit address for block
	if (shAr) shAr+=12;
	else shAr=11;
	LX_NAND->CR.word = 0x301;
	for(i=0;i<amount;i++){
		LX_NAND->SR.word = (1<<13);				//reset bit 13 from Status Register
		LX_NAND->AR = numBlock<<shAr;
		LX_NAND->CNTR = 1;
		while(!(LX_NAND->SR.word & 1<<13));		//wait Erase
		while(!(LX_NAND->SR.word & 8));
		tmp = LX_NAND->DR;					//read trash byte
		numBlock++;
	}
}

*/
