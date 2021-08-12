#include "H.h"

/*Режим 22/10 - увелечение DXM до 22 бит, и  уменьшение DXC до 10 бит.
Увеличивает смещение адреса >32768 */

#define DDUC_BASE0_STEP_OFFSET (*(uint32_t*)(DDUC_BASE0+DDUC_STEP_OFFSET))
#define DDUC_BASE0_CR_OFFSET   (*(uint32_t*)(DDUC_BASE0+DDUC_CR_OFFSET))

#define N 32768//КОЛИЧЕСТВО ЭЛЕМЕНТОВ В МАССИВЕ ПРИЁМНИКА
#define DMA_CH_RX 8//НОМЕР DMA ПРИЁМНИКА ( УСТРОЙСТВО -> ПАМЯТЬ )

/*DMA_RX*/
uint32_t __attribute((aligned(4))) TCB_RX[4] = {0,};//DMA_RX

static  uint32_t __attribute__((aligned(4))) BUF_RX[N]; 

/*Определение типа памяти для контроллера DMA*/
static int32_t HAL_DMA_INIT_MEM_TYPE(const void* addr );

int main(void)
{
  size_t DMA_CNT = 1;
  size_t IQ_CNT  = 0;
  size_t ErrFlag = 0;
  static FILE *FP_IN;
  static FILE *FP_OUT;
  uint32_t TCB_TYPE_MEMORY = 0;

  BRD_init();
  
    /*OPEN_READ_FILE_SETTING_UPDOWN*/
  //динамический массив,что бы не тащить массив dducregs[3] через весь алгоритм,т.к. нужен 1 раз
  uint32_t* dducregs = (uint32_t*) calloc(3,sizeof(uint32_t));
  FP_IN = fopen("SETTING_UPDOWN.bin","r+b");
  if(FP_IN == 0){ puts("NOT OPEN FILE SETTING UPDOWN.bin") ; return -1; }
  fread(dducregs,3,sizeof(uint32_t),FP_IN);
  fclose(FP_IN);
  
  DDUC_BASE0_STEP_OFFSET = dducregs[0];
  DDUC_BASE0_CR_OFFSET   = dducregs[1];
  FLAG_SETTING_UPDOWN    = dducregs[2];
  free(dducregs);
  
  /*OPEN_WRITE_FILE_DATA*/
  FP_OUT = fopen("W:/ML/MSO/DMA_UP_DOWN_CONVERTER/ODDUC.bin","w+b");
  if(FP_OUT == 0){ puts("NOT OPEN FILE ODDUC.bin") ; return -1 ;}

  /*SETTINGS ADC 0 по SPI*/
	ADC5101HB015_config_t ADC_CFG; 			 //Cтруктура для записи конфигурации АЦП
	ADC5101HB015_hw_config_t ADC_SPI_CFG;//Cтруктура для настройки SPI АЦП
  //Задание выводов SPI интерфейса АЦП 0 и 1
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
	ADC5101HB015_init( 1, 1, &ADC_SPI_CFG );
	ADC5101HB015_config( &ADC_CFG );

  /*SETTINGS LINK-интерфейса для работы с ADC 0*/
  //RX
	LinkRx_Init_type LINK_ADC_INI_STR; //Структура для описания внешенего устройства с LINK интерфейсом (от кого принимаем)
  LinkRxEx_type    LINK_RX_INI_STR;  //Структура для инициализации приемника LINK порта

	LINK_RX_INI_STR.AdcDataSize = Link_AdcDataSize_14b;
	LINK_RX_INI_STR.Rcode 		  = Link_Rcode_Dis;
	LINK_RX_INI_STR.BitOrder 	  = Link_BitOrder_OddFrontEvenCut;
	LINK_RX_INI_STR.GpsClk 		  = Link_GpsClk_Dis;
	LINK_RX_INI_STR.RxDest 		  = Link_RxDest_Buff;

	LINK_ADC_INI_STR.DataSize 	= Link_DataSize_8bit;
	LINK_ADC_INI_STR.CheckBCMPI = Link_CheckBCMP_Dis;
	LINK_ADC_INI_STR.CheckSum 	= Link_CheckSum_Dis;
	LINK_ADC_INI_STR.OVRIntEn 	= Link_OvrIT_Dis;
	LINK_ADC_INI_STR.TOIntEn    = Link_TOIT_Dis;

  //Деинициализация приемника LINK порта  0
	HAL_LinkRx_Disable(0);

  //Инициализация приемника LINK порта  0
	HAL_LinkRx_Enable( 0, &LINK_ADC_INI_STR,  &LINK_RX_INI_STR );

  //TX
  //Инициализации передатчика LINK порта
	LinkTx_Init_type  LINK_TX_INI_STR;//Структура для инициализации передатчика LINK порта
  LinkTxEx_type     LINK_DOWN_INI_STR;
  
  LINK_DOWN_INI_STR.TxSource  = Link_TxDest_UpDown;
  LINK_DOWN_INI_STR.Tcode     = Link_Tcode_Dis;
  LINK_DOWN_INI_STR.CheckACKI = Link_CheckACKI_Dis;
  
	LINK_TX_INI_STR.DataSize   = Link_DataSize_16bit;
	LINK_TX_INI_STR.CheckBCMPO = Link_CheckBCMP_Dis;
	LINK_TX_INI_STR.CheckSum   = Link_CheckSum_Dis;
	LINK_TX_INI_STR.TOIntEn    = Link_TOIT_Dis;
	LINK_TX_INI_STR.ClkSource  = Link_TxClk_Master;
	 
  //Деинициализация передатчика LINK порта  0
  HAL_LinkTx_Disable(0);     
  
  //Инициализация передатчика LINK порта  0
	HAL_LinkTx_Enable( 0, &LINK_DOWN_INI_STR, &LINK_TX_INI_STR  );

  /*SETTINGS UP/DOWN*/
  if ( FLAG_SETTING_UPDOWN == 0)
  {
    DDUC_BASE0_STEP_OFFSET = 23593;//REG_STEP
    DDUC_BASE0_CR_OFFSET   = SR_DDUC_ENABLE    |
                             SR_FIR_TAP*15     |
                             SR_SRC_LINK       |
                             SR_ROUND_OFF      |
                             SR_SATURATION_OFF |
                             SR_ROUNDM_OFF     |
                             SR_FIR_ORDER3     |
                             SR_INPUT_BE       |
                             SR_OUTPUT_LE      |
                             SR_MODE_DDC       |
                             SR_SHIFT*15;// default - 0x101
  }
  /*SETTING DMA*/
  //ПРИЁМНИК
  TCB_TYPE_MEMORY = HAL_DMA_INIT_MEM_TYPE(&BUF_RX);//ОПРЕДЕЛЕНИЕ ТИПА ИСПОЛЬЗУЕМОЙ ПАМЯТИ
  if(TCB_TYPE_MEMORY == -1){ puts("ERROR TYPE MEMORY - RX"); }
  HAL_DMA_RqstClr(DMA_CH_RX);                 //ОЧИСТКА ИСТОЧНИКА ЗАПРОСА DMA
  HAL_DMA_RqstSet(DMA_CH_RX, dmaUPDOWN0);     //УСТАНОВКА НОВОГО ИСТОЧНИКА ОПРОСА ДЛЯ КАНАЛА DMA_CH_RX
  TCB_RX[0] = ((void *)BUF_RX);               //DI
  TCB_RX[1] = ((N) << 16) | 4;                //DX
  TCB_RX[2] = 0;                              //DY
  TCB_RX[3] |= ( TCB_TYPE_MEMORY | TCB_QUAD); //DP

  while(DMA_CNT != 8)
  {
    /*WRITE_SETTING_IN_DMA*/
    HAL_DMA_Stop( 8 );//ВЫКЛЮЧЕНИЕ DMA ПРИЁМНИКА----- НЕ КОРРЕКТНО РАБОТАЕТ ЕСЛИ НЕ ВЫКЛЮЧАТЬ!!!!!!
    HAL_DMA_WriteDC(DMA_CH_RX, &TCB_RX);//ЗАПИСЬ СТРУКТУРЫ В РЕГИСТР TCB И ЗАПУСК РАБОТЫ КАНАЛА DMA_CH_TX

    /*MAIN_1*/
    ErrFlag = HAL_DMA_WaitForChannel(DMA_CH_RX);
    if(ErrFlag == 0)	 { printf("OK TX DMA - %i\n",DMA_CNT++); }
    else if(ErrFlag == 1){ puts("ERROR TX DMA") ; return -1;   }
    else if(ErrFlag == 2){ puts("TX DMA OFF")   ; return -1;   }

    /*WRITE_FILE_DATA*/
    fwrite((void *)BUF_RX,sizeof(uint32_t),N,FP_OUT) ;
  }
  /*CLOSE_FILE*/
  fclose(FP_OUT);
  puts("END") ;
  return 0;
}
