/*Программа сбора и записи данных АЦП в файл*/
#include <hal_1967VN044.h>
#include "hal_1967VN044.h"
#include <builtins.h>
#include <sysreg.h>
#include <stddef.h>
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include "../inc/memmap.h"
#include "../inc/_dduc.h"
#include "5101HB015.h"
#include "5101HB015_SPI_1967VC3.h"

#define DMA_CH	 8
#define	BUF_SIZE 32768

#define OUTPUTFNAME "adc.bin"

//static uint8_t MESSAGE = 0x32;
/*Буфер c данными АЦП (каждое слово данных (32 бита) содежит 2 отсчета АЦП)*/
__attribute__((aligned(4))) uint32_t adc[BUF_SIZE] ;
__attribute__((aligned(4))) uint32_t tcb[4] ; //буфер для записи конфигурации TCB-8

int main(void)
{
  FILE *fid ;
  ADC5101HB015_config_t    ADC_CFG; 			 //Cтруктура для записи конфигурации АЦП
  ADC5101HB015_hw_config_t ADC_SPI_CFG;	 //Cтруктура для настройки SPI АЦП
  LinkRx_Init_type         LINK_ADC_INI_STR; //Структура для описания внешенего устройства с LINK интерфейсом (от кого принимаем)
  LinkRxEx_type            LINK_RX_INI_STR;    //Структура для инициализации приемника LINK порта

  BRD_init();

  // Инициализация АЦП 0 по spi
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

  // Инициализация LINK-интерфейса для работы с АЦП 0
  LINK_RX_INI_STR.AdcDataSize = Link_AdcDataSize_14b;
  LINK_RX_INI_STR.Rcode       = Link_Rcode_En;
  LINK_RX_INI_STR.BitOrder    = Link_BitOrder_OddFrontEvenCut;
  LINK_RX_INI_STR.GpsClk      = Link_GpsClk_Dis;
  LINK_RX_INI_STR.RxDest      = Link_RxDest_Buff;
  
  LINK_ADC_INI_STR.DataSize   = Link_DataSize_8bit;
  LINK_ADC_INI_STR.CheckBCMPI = Link_CheckBCMP_Dis;
  LINK_ADC_INI_STR.CheckSum   = Link_CheckSum_Dis;
  LINK_ADC_INI_STR.OVRIntEn   = Link_OvrIT_Dis;
  LINK_ADC_INI_STR.TOIntEn    = Link_TOIT_Dis;

  HAL_LinkRx_Disable(0); //Деинициализация приемника LINK порта  0
  HAL_LinkRx_Enable(0, &LINK_ADC_INI_STR,  &LINK_RX_INI_STR);   //Инициализация приемника LINK порта  0

  //HAL_Interrupt_GlobalEnable();

  // Инициализация и запуск DMA контроллера
  HAL_DMA_RqstClr(DMA_CH);// Очистка источника опроса DMA_DAC_RX_CH - канала
  HAL_DMA_RqstSet(DMA_CH, dmaSTD);  // Установка нового источника опроса для канала DMA_CH_NUM,  dmaSTD
  // PING
  tcb[0] = (uint32_t)adc;
  tcb[1] = (BUF_SIZE << 16) | 4;
  tcb[2] = 0;
  tcb[3] = TCB_INTMEM | TCB_QUAD;
  
  HAL_DMA_WriteDC(DMA_CH,&tcb) ; // Запись структуры в регистр TCB и запуск работы канала DMA_CH_NUM DMA
  HAL_DMA_WaitForChannel(DMA_CH) ;      // Ожидание выполнения до конца передачи

  //Запись в файл
  fid = fopen(OUTPUTFNAME,"wb") ;
  if(fid == 0){ puts("Unable open output file.") ; return(-1) ; }
  fwrite(adc,1,sizeof(adc),fid) ;
  fclose(fid) ;
  return(0) ;
}
