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




