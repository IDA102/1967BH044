/*Программа сбора и записи данных АЦП*/

#include "H.h"

#define NOP asm("nop;;")

#define DMA_CH	 			8
#define	ADC_DMA_BUF_SIZE	500

static uint8_t MESSAGE = 0x32;

//Буфер c данными АЦП (каждое слово данных (32 бита) содежит 2 отсчета АЦП)
static uint32_t ADC_DMA_BUF[ADC_DMA_BUF_SIZE]__attribute__((aligned(4)));

static uint32_t LINK_DMA_RX_TCB_PING[4]__attribute__((aligned(4))); //буфер для записи конфигурации TCB-8

//Запись в файл
static int WRITE_DATA_BIN(void)
{
	static FILE *FP;
	FP = fopen("W:/ML/MSO/FREQ_RESPONCE/ADC_SWEEP.bin","a+b");
	if(FP != 0)
		{
			fwrite(ADC_DMA_BUF , 1 , ADC_DMA_BUF_SIZE  , FP);
			fclose(FP);
			return 0;
		}
	return -1;
}

//Инициализация и запуск DMA контроллера
static void DMA_INIT(void)
{
	//Очистка источника опроса DMA_DAC_RX_CH - канала
	HAL_DMA_RqstClr(DMA_CH);

	//Конфигурация регистра TCB DMA
	LINK_DMA_RX_TCB_PING[ 3 ] = 0;

	//Установка нового источника опроса для канала DMA_CH_NUM,  dmaSTD
	HAL_DMA_RqstSet(DMA_CH, dmaSTD);

	//PING
	LINK_DMA_RX_TCB_PING[0] = (uint32_t)&ADC_DMA_BUF;
	LINK_DMA_RX_TCB_PING[1] = (ADC_DMA_BUF_SIZE << 16) | 4;
	LINK_DMA_RX_TCB_PING[2] = 0;
	LINK_DMA_RX_TCB_PING[3] |= TCB_INTMEM | TCB_QUAD;

	//Запись структуры в регистр TCB и запуск работы канала DMA_CH_NUM DMA
	HAL_DMA_WriteDC(DMA_CH,  &LINK_DMA_RX_TCB_PING);

	//Ожидание выполнения до конца передачи
	HAL_DMA_WaitForChannel (DMA_CH);
}

//Функция для отправки данных буфера data по UART
static void UART_SEND_DATA(uint8_t *data, uint32_t len_data)
{
	HAL_UART_Send(LX_UART1, data, len_data);
}

//Обработчик прервывания при появлении новых входных данных
__attribute__((interrupt))
static void UART1_RX_HANDLER(void)
{
	uint32_t c;
	if(LX_UART1->UFLAG.b.URXFE == 0) //если fifo не пуcтой, читаем символ
	{
		c = LX_UART1->UDR;
	}
	if(c == 49)
	{
		DMA_INIT();
		WRITE_DATA_BIN();
		UART_SEND_DATA(&MESSAGE, 1);
		NOP;
	}
}

static void UART_INIT(void)
{
	UART_Init_type UARTInit; //структура для инициализации UART

	UARTInit.BitRate = 9600;
	UARTInit.OverSampling = UART_OverSampling_Normal;
	UARTInit.WorkMode = UART_Mode_TxRx;
	UARTInit.WordLength = UART_WordLen_8bit;
	UARTInit.StopBits = UART_Stop_1bit;
	UARTInit.ParityMode = UART_Parity_Off;
	UARTInit.FIFOSize = UART_FIFOSize_Byte;
	UARTInit.TXDMode = UART_TXD_Direct;
	UARTInit.DMACtrlErr = UART_DMACtrl_Dis;

	//инициализации ножек процессора для работы альтернативной функции
	//HAL_GPIO_Init (LX_GPIO_PA,  GPIO_PIN_0 | GPIO_PIN_1, GPIO_PinMode_Alt); // GPIO_PIN_x, x - зависит от выбранного UART
	HAL_GPIO_Init (LX_GPIO_PA,  GPIO_PIN_2 | GPIO_PIN_3, GPIO_PinMode_Alt); // GPIO_PIN_x, x - зависит от выбранного UART
	HAL_UART_Init (LX_UART1, &UARTInit);
	HAL_UART_ITConfig(LX_UART1, UART_IT_RX, UART_IT_En ); //разрешение генерации прерывания при появлении входных данных на в буфере UART
	HAL_Interrupt_Enable(intUART1, UART1_RX_HANDLER); //настройка прервываний от UART0, указатель на обработчик прерывния от UART0.
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


	LINK_RX_INI_STR.AdcDataSize = Link_AdcDataSize_14b;
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
	HAL_Interrupt_GlobalEnable();
	UART_INIT();
	while(1)
	{
		NOP;
		//UART_SEND_DATA(0xF, 1);
		//wait_msec(500);
	}
	return 0;
}
