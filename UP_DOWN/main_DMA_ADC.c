/*
    main.c
    Created on: 22 апр. 2021 г.
    Author: IDA_102
 */
#include "H.h"

#define NOP asm("nop;;")
#define N 1024

uint32_t __attribute((aligned(4 * (32/__CHAR_BIT__)))) BUF_RX[N];
uint32_t __attribute((aligned(4 * (32/__CHAR_BIT__)))) TCB[4];//DMA

//Запись в файл
static int WRITE_DATA_BIN(void)
{
	static FILE *FP;
	FP = fopen("W:/ML/MSO/UP_DOWN_CONVERTER/UDD.bin","a+b");
	if(FP != 0)
		{
			fwrite(BUF_RX , 1 , N , FP);
			fclose(FP);
			return 0;
		}
	return -1;
}

//Инициализация АЦП 0 по spi
static void ADC_INIT(void)
{
	ADC5101HB015_config_t ADC_CFG; 			 //Cтруктура для записи конфигурации АЦП
	ADC5101HB015_hw_config_t ADC_SPI_CFG;	 //Cтруктура для настройки SPI АЦП
//Задание выводов SPI интерфейса АЦП 0 и 1
	ADC_SPI_CFG.port = LX_GPIO_PC;
	ADC_SPI_CFG.cs_bit = (1 << 2);			// FLAG[2]
	ADC_SPI_CFG.clk_bit = (1 << 0);			// FLAG[0]
	ADC_SPI_CFG.mosi_bit = (1 << 3);		// FLAG[3]
	ADC_SPI_CFG.miso_bit = (1 << 3);
	ADC_SPI_CFG.power_down_bit = (1 << 24);	// L0ACKO
	ADC_SPI_CFG.core_freq_khz = CORE_FREQ_KHZ;

//Задание значений регистров АЦП
	ADC_CFG.reference_level = ADC5101HB015_REF_1P0;
	ADC_CFG.output_format = ADC5101HB015_OUTPUT_LVDS;
	ADC_CFG.lvdsen_pin_state = ADC5101HB015_LVDSEN_PIN_HIGH;
	ADC_CFG.lvds_current_mode = ADC5101HB015_LVDS_CURRENT_NORMAL;
	ADC_CFG.oen_pin_override = ADC5101HB015_OEN_OVERRIDE;
	ADC_CFG.common_mode_sel = ADC5101HB015_COMMON_MODE_0P75;
//Инициализация АЦП 0 и 1 (сброс, калибровка, отправка конфигурации регистров по spi)
	ADC5101HB015_init(1, 1, &ADC_SPI_CFG);
	ADC5101HB015_config(&ADC_CFG);
}

//Инициализация LINK-интерфейса для работы с АЦП 0
static void LINK_DMA_INIT(void)
{
	LinkRx_Init_type LINK_ADC_INI_STR; //Структура для описания внешенего устройства с LINK интерфейсом (от кого принимаем)
    LinkRxEx_type  LINK_RX_INI_STR;    //Структура для инициализации приемника LINK порта

//Деинициализация приемника LINK порта  0
	HAL_LinkRx_Disable(0);

	LINK_RX_INI_STR.AdcDataSize = Link_AdcDataSize_16b;
	LINK_RX_INI_STR.Rcode 		= Link_Rcode_Dis;
	LINK_RX_INI_STR.BitOrder 	= Link_BitOrder_OddFrontEvenCut;
	LINK_RX_INI_STR.GpsClk 		= Link_GpsClk_Dis;
	LINK_RX_INI_STR.RxDest 		= Link_RxDest_Buff;

	LINK_ADC_INI_STR.DataSize 	= Link_DataSize_8bit;
	LINK_ADC_INI_STR.CheckBCMPI = Link_CheckBCMP_Dis;
	LINK_ADC_INI_STR.CheckSum 	= Link_CheckSum_Dis;
	LINK_ADC_INI_STR.OVRIntEn 	= Link_OvrIT_Dis;
	LINK_ADC_INI_STR.TOIntEn 	= Link_TOIT_Dis;

//Инициализация приемника LINK порта  0
	HAL_LinkRx_Enable(0, &LINK_ADC_INI_STR,  &LINK_RX_INI_STR);
}

int main(void)
{
    BRD_init();
    ADC_INIT();
	LINK_DMA_INIT();

    //Mode 			= UPDOWN_Mode_Down;----------------------Режим децимации
    //LinkX 		= UPDOWN_Link_Test;----------------------APB-шина 
    //FiltersStage 	= UPDOWN_FiltersStage_3;-----------------3 ступени фильтрации
    //Round 		= UPDOWN_Round_Off;----------------------Округление выключено
    //Shift 		= 0xFF;----------------------------------Сдвиг выходных данных на 255
    //FifoMode 		= UPDOWN_FifoMode_LowWord_IQ;------------Режим хранения данных в FIFO.Первое слово хранится в младших битах регистра данных DR, мнимая часть хранится в битах [31:16], действительная часть в битах [15:0] слова.
    //Coefficient 	= 4;-------------------------------------
    //CntMode 		= UPDOWN_CntMode_Off;--------------------Режим работы счетчика отсчетов выключен
    //Step 			= 0;-------------------------------------Шаг приращения счетчика для выбора констант
    //Cnt 			= 0.-------------------------------------Значение счетчика отсчетов

    //LX_UPDOWN0->DR[0] = 0xFF;

    UPDOWN_Init_type UD;
    uint32_t __attribute((aligned(4 * (32/__CHAR_BIT__)))) TCB[4];
    volatile uint32_t tmp;

// Настройка UPDOWN:
    HAL_UPDOWN_DefaultInitStruct (&UD);
    UD.Mode 	= UPDOWN_Mode_Down;
    UD.LinkX 	= UPDOWN_Link_0;
    UD.Cnt 		= 1234;
    UD.Step 	= 10;
    UD.Coefficient = 1;
    UD.Shift 	= 0x10;
    HAL_UPDOWN_Init (LX_UPDOWN0, &UD);

// Настройка DMA:
    TCB[0] = (uint32_t) BUF_RX;
    TCB[1] = (N << 16) | 4;
    TCB[2] = 0;
    TCB[3] = TCB_INTMEM | TCB_QUAD;
    HAL_UPDOWN_ConnectDMA (LX_UPDOWN0, 0, TCB);

// Ожидание окончания заполнения буфера:
    HAL_DMA_WaitForChannel (4 + 0);
    HAL_UPDOWN_Enable(LX_UPDOWN0);

    WRITE_DATA_BIN();
    return 0; 
}
 




