#include "H.h"

/*
 main.cpp
 Created on: 6 июля 2020 г.
 Author: IDA_102
*/

//Типы памяти для контроллера DMA (стр 2-64 HardReference)
#define TCB_INTMEM16  			0x40000000//Внутреняя память - Internal memory
#define TCB_INTMEM22			0x60000000//Внутреняя память(Большой шаг) - Internal memory (larger stride)
#define TCB_EXTMEM16			0x80000000//Внешняя память - External memory
#define TCB_EXTMEM22			0xE0000000//Внешняя память(Большой шаг) - External memory (larger stride)
#define N 4//Size Memory
unsigned int  __attribute((aligned(4))) DATA_TX_32[N] = {1,2,4,8};//Массив для передачи
unsigned int  __attribute((aligned(4))) DATA_RX_32[N] = {0,0,0,0};//Массив для приёма

/*UART*/
/*Обработчики прерываний*/
/*Обработчик прервывания UART при появлении новых входных данных*/
/*__attribute__((interrupt))
static void UART1_RX_Handler(void)
{
	uint32_t c = 0;
	if(LX_UART1->UFLAG.b.URXFE == 0) //Если fifo не пуcтой, читаем символ
	{	c = LX_UART1->UDR;	}
}*/
/*Инициализация интерфейса UART1*/
static void uart_init(void)
{
	UART_Init_type UARTInit; //Cтруктура для инициализации UART

	UARTInit.BitRate 		= 115200;
	UARTInit.OverSampling 	= UART_OverSampling_Normal;
	UARTInit.WorkMode 		= UART_Mode_TxRx;
	UARTInit.WordLength 	= UART_WordLen_8bit;
	UARTInit.StopBits 		= UART_Stop_1bit;
	UARTInit.ParityMode 	= UART_Parity_Off;
	UARTInit.FIFOSize 		= UART_FIFOSize_8Byte;
	UARTInit.TXDMode 		= UART_TXD_Direct;
	UARTInit.DMACtrlErr 	= UART_DMACtrl_Dis;

//Инициализации ножек процессора для работы альтернативной функции
	HAL_GPIO_Init (LX_GPIO_PA,  GPIO_PIN_2 | GPIO_PIN_3, GPIO_PinMode_Alt); // GPIO_PIN_x, x - зависит от выбранного UART
	HAL_UART_Init (LX_UART1, &UARTInit);
	HAL_UART_ITConfig(LX_UART1, UART_IT_RX, UART_IT_En ); //Разрешение генерации прерывания при появлении входных данных на в буфере UART
	//HAL_Interrupt_Enable(intUART1, UART1_RX_Handler); //Настройка прервываний от UART0, указатель на обработчик прерывния от UART0.	
}

/*Инициализия приема данных через UART с загрузкой принимаемых данных из буфера UART по DMA в выделенную область памяти*/

/*Определение типа памяти для контроллера DMA на основе адреса в указателе с помощью макроса. Эквивалентно функции "HAL_DMA_InitMemType"*/
/*#define HAL_DMA_InitMemType( addr, DPReg )\
	if ((addr >= (0x00000000) && addr <= (0x0000FFFF)) || (addr >= 0x00040000 && addr <= (0x04FFFF)) ||\
		(addr >= 0x080000 && addr <= (0x08FFFF)) || (addr >= 0x0C0000 && addr <= (0x0CFFFF)) ||\
		(addr >= 0x100000 && addr <= (0x10FFFF)) || (addr >= 0x140000 && addr <= (0x14FFFF)))\
		DPReg |= TCB_INTMEM16;\
	else if((addr >= 0x30000000 && addr <= (0x44000000) ) || (addr >= 0x50000000 && addr <= (0x54000000)) ||\
			(addr >= 0x60000000 && addr <= (0x64000000)) || (addr >= 0x70000000 && addr <= (0x74000000)) || (addr >= 0x80000000 && addr <= (0xFFFFFFFF)))\
		DPReg |= TCB_EXTMEM16;\
	else return dmaCopyAddrErr
*/
//f(x)
//Определение типа памяти для контроллера DMA на основе адреса в указателе
uint32_t HAL_DMA_InitMemType(uint32_t addr,uint32_t DPReg )
{
	if ((addr >= (0x00000000) && addr <= (0x0000FFFF)) || (addr >= 0x00040000 && addr <= (0x04FFFF)) ||	(addr >= 0x080000 && addr <= (0x08FFFF)) || (addr >= 0x0C0000 && addr <= (0x0CFFFF)) ||	(addr >= 0x100000 && addr <= (0x10FFFF)) || (addr >= 0x140000 && addr <= (0x14FFFF)))
		{
			DPReg |= TCB_INTMEM16;
			return DPReg;
		}
	else if((addr >= 0x30000000 && addr <= (0x44000000) ) || (addr >= 0x50000000 && addr <= (0x54000000)) || (addr >= 0x60000000 && addr <= (0x64000000)) || (addr >= 0x70000000 && addr <= (0x74000000)) || (addr >= 0x80000000 && addr <= (0xFFFFFFFF)))
		{
			DPReg |= TCB_EXTMEM16;
			return DPReg;
		}
	//else return dmaCopyAddrErr;
}

/*Задержка*/
void Delay(int waitTicks)
{for (int i = 0; i < waitTicks; i++);}

/*Запись из внутреней памяти в внутрению память по 3 каналу DMA*/
uint32_t DMA_INTERNAL_MEMORI_INTERNAL_MEMORY(void *TX, void *RX,  uint32_t size)
{
	if ( size > 0xFFFF ) { printf( "ERROR LENGTH!!!" ); return 0;}

	uint32_t __attribute((aligned(4))) tcb_dcs[4];
	uint32_t __attribute((aligned(4))) tcb_dcd[4];
/*Конфигурирование приёмника и передатчика канала 3 DMA, на пересылку данных "Внутреняя память -> Внутреняя память" */
/*TCB передатчика*/
	tcb_dcs[ 3 ] = 0;
	tcb_dcs[ 3 ] = HAL_DMA_InitMemType( ( uint32_t )TX, tcb_dcs[ 3 ] );
	tcb_dcs[ 0 ] = ( uint32_t )TX;
	tcb_dcs[ 1 ] = ( size << 16 ) | 1;
	tcb_dcs[ 2 ] = 0;
	tcb_dcs[ 3 ] |= TCB_NORMAL;//(0x02000000)//25 bit -> (01 – Normal (32-bit) word)

/*TCB приёмника*/
	tcb_dcd[ 3 ] = 0;
	tcb_dcd[ 3 ] = HAL_DMA_InitMemType( ( uint32_t )RX, tcb_dcd[ 3 ] );
	tcb_dcd[ 0 ] = ( uint32_t )  RX;
	tcb_dcd[ 1 ] = ( size << 16 ) | 1;
	tcb_dcd[ 2 ] = 0;
	tcb_dcd[ 3 ] |= TCB_NORMAL;
	
	HAL_DMA_WriteDCD ( (uint32_t)3, &tcb_dcd);
	HAL_DMA_WriteDCS ( (uint32_t)3, &tcb_dcs);//(0x02000000)//25 bit -> (01 – Normal (32-bit) word)
	uint32_t chStat = HAL_DMA_GetChannelStatus( 3 );
	return chStat;
}
/*Запись из UART в внутрению память*/
void DMA_UART_INTERNAL_MEMORY( void *RX,  uint32_t size )
{
	if ( size > 0xFFFF ) { printf( "ERROR LENGTH!!!" );}
	
	uint32_t __attribute((aligned(4))) tcb_dcd[4];

/*Задание номера устройства(UART_1) и канала(8) в регистре DMACFGL*/
	uint32_t dma_ch_number = 8;//Номер канала
/*Обнуление TCB приёмника*/
	{
		__builtin_quad zero_tcb = __builtin_compose_128( 0, 0 );//HAL_DMA_Stop( Channel );
		__builtin_sysreg_write4( __DC8, *( __builtin_quad * ) &zero_tcb );//HAL_DMA_WriteDC( ch_number, &zero_tcb );
	}

/*TCB приёмника*/
	tcb_dcd[ 3 ] = 0;
	tcb_dcd[ 3 ] = HAL_DMA_InitMemType( ( uint32_t )RX, tcb_dcd[ 3 ] );
	
	tcb_dcd[ 0 ] = ( uint32_t )  RX;
	tcb_dcd[ 1 ] = ( size << 16 ) | 1;
	tcb_dcd[ 2 ] = 0;
	
	unsigned int tmp = &tcb_dcd;
	tmp = tmp >> 2;
	unsigned int tmp2 = 0x400000;
	tcb_dcd[ 3 ] |= (TCB_NORMAL | TCB_DMAR | tmp2 | tmp);

	
	/*Определяем устройство запроса*/
	//HAL_DMA_RqstSet( Channel, dmaUART1 );
	uint32_t dma_device_Rqster = 2;//Устройство запроса

		*( uint32_t * ) base_DMACFGH &= ~( 0x10000 << dma_ch_number );
		*( uint32_t * ) base_DMACFGH |= ( ( ( dma_device_Rqster & 0x10 ) << 12 ) << dma_ch_number );
		dma_ch_number *= 4;
		dma_ch_number &= 0x1F;
		*( uint32_t * ) base_DMACFGH &= ~( 0xF << dma_ch_number );
		*( uint32_t * ) base_DMACFGH |= ( ( dma_device_Rqster & 0xF ) << dma_ch_number );

		/*Функция разрешает прерывание, и инициализирует адрес обработчика прерываний. (редактируется регистр IMASKL/H)*/
		//HAL_Interrupt_Enable( intDMA8, DmaIsrHandler );
/*		uint32_t tmp32 = 0;
		uint32_t intUART0 = 4;
		uint32_t intRQST = intUART0;
		void ptrHndlr  = 0;// Указатель на обработчик прерываний, Т.К. не используем передаём 0.
		*( uint32_t * ) ( base_IVT + intRQST ) = ( uint32_t ) ptrHndlr;
		tmp32 = __builtin_sysreg_read( __IMASKL );
		__builtin_sysreg_write( __IMASKL, ( tmp32 | ( 1 << intRQST ) ) );
*/
		HAL_DMA_WriteDC( 8, &tcb_dcd );
}

	
//функция отправляет данные буфера data по UART
static void uart_send_data(uint8_t *data, uint32_t len_data)
{
	HAL_UART_Send(LX_UART1, data, len_data);
}

int main(void)
{
	void *TX = &DATA_TX_32;
	void *RX = &DATA_RX_32;
	uint32_t size = N;
	
	uart_init();//инициализация UARTa

	//while(1)
	//{	HAL_UART_DmaReceive( LX_UART1 , 8 , RX , 4 , 0 ); }
	
	//while(1)
	//{		DMA_UART_INTERNAL_MEMORY( RX, size );	}
	DMA_UART_INTERNAL_MEMORY( RX, size );
	
	int i;
	while(1)
	{
		 ++i;
	}

	//uint32_t RETURN  = DMA_INTERNAL_MEMORI_INTERNAL_MEMORY(TX,RX,size);//if = 0 - > error, if = 2 -> OK
	
	return 0;	
}
