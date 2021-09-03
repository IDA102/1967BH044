#include <hal_1967VN044.h>
#include "hal_1967VN044.h"
#include <builtins.h>
#include <sysreg.h>
#include <stddef.h>
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <driver1967VN044_NANDSamsung.h>
#include "../inc/memmap.h"
#include "../inc/_dduc.h"
#include "5101HB015.h"
#include "5101HB015_SPI_1967VC3.h"
#include <stdint.h>

#define NOP asm("nop;;")

#define S 128*4
#define PP_LENGTH 128
#define IQ_LENGTH 100*128 // 1Mb

#define DMA_CH_OUTPUT 8
#define OUTPUTFNAME   "w:/MILANDR/MSO/CURRENT_FOLDER/1967BH044/1967BH044/adci.bin"

__attribute__((aligned(4))) uint32_t ppbuf[IQ_LENGTH] ;  // pp bufer
__attribute__((aligned(4))) uint32_t pingtcb[4] ;
__attribute__((aligned(4))) uint32_t pongtcb[4] ;
//uint32_t ibuf[IQ_LENGTH] ;
//uint32_t qbuf[IQ_LENGTH] ;

static uint32_t flag,cnt,iqcnt;
uint32_t* ptr;
static uint32_t* ADDR = &(ppbuf[PP_LENGTH]);
__attribute((interrupt))
void dma_handler(void)
{
  if(cnt >= S)
  {
	HAL_DMA_Stop( DMA_CH_OUTPUT );
	return;
  }
  cnt++;
  ADDR = ADDR + PP_LENGTH;
  flag = flag ^ 0x00000001;
  if(flag == 1)  {
	  pingtcb[0] = (void*)(ADDR);
  }
  if(flag == 0)  {
	  pongtcb[0] = (void*)(ADDR);
  }
  /*
  if(flag == 1)  { ptr = ppbuf; }
  if(flag == 0)  { ptr = &ppbuf[PP_LENGTH]; }
  for(uint32_t i=0;i<PP_LENGTH/4;i++)
  {
    ibuf[iqcnt] = *ptr++;
    iqcnt++;
  }*/
}

int main(void)
{
  FILE*  fidi;

  ADC5101HB015_config_t    ADC_CFG;          //Cтруктура для записи конфигурации АЦП
  ADC5101HB015_hw_config_t ADC_SPI_CFG;	     //Cтруктура для настройки SPI АЦП
  LinkRx_Init_type         LINK_ADC_INI_STR; //Структура для описания внешенего устройства с LINK интерфейсом (от кого принимаем)
  LinkRxEx_type            LINK_RX_INI_STR;  //Структура для инициализации приемника LINK порта

/* Инициализация АЦП 0 по spi*/
// Задание выводов SPI интерфейса АЦП 0 и 1
  ADC_SPI_CFG.port           = LX_GPIO_PC;
  ADC_SPI_CFG.cs_bit         = (1 << 2); // FLAG[2]
  ADC_SPI_CFG.clk_bit        = (1 << 0); // FLAG[0]
  ADC_SPI_CFG.mosi_bit       = (1 << 3); // FLAG[3]
  ADC_SPI_CFG.miso_bit       = (1 << 3);
  ADC_SPI_CFG.power_down_bit = (1 << 24);// L0ACKO
  ADC_SPI_CFG.core_freq_khz  = CORE_FREQ_KHZ;

//Задание значений регистров АЦП
  ADC_CFG.reference_level   = ADC5101HB015_REF_1P0;
  ADC_CFG.output_format     = ADC5101HB015_OUTPUT_LVDS;
  ADC_CFG.lvdsen_pin_state  = ADC5101HB015_LVDSEN_PIN_HIGH;
  ADC_CFG.lvds_current_mode = ADC5101HB015_LVDS_CURRENT_NORMAL;
  ADC_CFG.oen_pin_override  = ADC5101HB015_OEN_OVERRIDE;
  ADC_CFG.common_mode_sel   = ADC5101HB015_COMMON_MODE_0P75;

//Инициализация АЦП 0 и 1 (сброс, калибровка, отправка конфигурации регистров по spi)
  ADC5101HB015_init(1, 1, &ADC_SPI_CFG);
  ADC5101HB015_config(&ADC_CFG);

/* Инициализация LINK-интерфейса для работы с АЦП 0*/
  LINK_RX_INI_STR.AdcDataSize = Link_AdcDataSize_14b;
  LINK_RX_INI_STR.Rcode       = Link_Rcode_En;//Link_Rcode_En
  LINK_RX_INI_STR.BitOrder    = Link_BitOrder_OddFrontEvenCut;
  LINK_RX_INI_STR.GpsClk      = Link_GpsClk_Dis;
  LINK_RX_INI_STR.RxDest      = Link_RxDest_Buff;

  LINK_ADC_INI_STR.DataSize   = Link_DataSize_8bit;
  LINK_ADC_INI_STR.CheckBCMPI = Link_CheckBCMP_Dis;
  LINK_ADC_INI_STR.CheckSum   = Link_CheckSum_Dis;
  LINK_ADC_INI_STR.OVRIntEn   = Link_OvrIT_Dis;
  LINK_ADC_INI_STR.TOIntEn    = Link_TOIT_Dis;

  HAL_LinkRx_Disable(0);                                        //Деинициализация приемника LINK порта  0
  HAL_LinkRx_Enable(0, &LINK_ADC_INI_STR,  &LINK_RX_INI_STR);   //Инициализация приемника LINK порта  0

//Разрешение прерывания от канла DMA 8, передача указателя на функцию обработчика прерывания
  HAL_Interrupt_GlobalEnable();
  HAL_Interrupt_Enable( intDMA8 , dma_handler );

/*OUTPUT DMA*/
  HAL_DMA_RqstClr(DMA_CH_OUTPUT);
  HAL_DMA_RqstSet(DMA_CH_OUTPUT, dmaSTD);

  pingtcb[0] = (void*)(ppbuf);                     //DI
  pongtcb[0] = (void*)(ppbuf+PP_LENGTH);           //DI
  pingtcb[1] = (PP_LENGTH<<16) | 4 ;               //DX
  pongtcb[1] = (PP_LENGTH<<16) | 4 ;               //DX
  pingtcb[2] = 0 ;                                 //DY
  pongtcb[2] = 0 ;                                 //DY
  pingtcb[3] = DP_DMA_TYPE_EXCHANGE_INT_MEM_16_16 |
               DP_DMA_LENGTH_DATA_128             |
               DP_DMA_INT_EN                      ;//DP
  pongtcb[3] = DP_DMA_TYPE_EXCHANGE_INT_MEM_16_16 |
               DP_DMA_LENGTH_DATA_128             |
               DP_DMA_INT_EN                      ;//DP

//Создание цепочки PING PONG
  HAL_DMA_CreateChannelDest(DMA_CH_OUTPUT , pingtcb , pongtcb);
  HAL_DMA_CreateChannelDest(DMA_CH_OUTPUT , pongtcb , pingtcb);

  HAL_DMA_Stop( DMA_CH_OUTPUT );
  HAL_DMA_GetChannelStatusClear( DMA_CH_OUTPUT );

/*Включение PING PONG*/

  HAL_DMA_WriteDC(DMA_CH_OUTPUT, pingtcb);
  HAL_SYS_WaitClk( 300000000 );//600000000
  printf("int - %i\n",cnt);

  fidi = fopen(OUTPUTFNAME,"w+b") ; if(fidi == 0){ puts("Unable open adci.bin."); }
  fwrite(ppbuf,sizeof(uint32_t),IQ_LENGTH,fidi) ;
  fclose(fidi);
  puts("END");
  return(0);
}
