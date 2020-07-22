/*
 *
 *	File/Файл: 		hal_uart.с
 *	Description/Описание: 	HAL for UART unit/HAL для модуля последовательного асинхронного интерфейса UART
 *
 *	History/История:
 *				27-Jun-2017 Dmitriy Sharaviev 	- 	added the function of data transmission by means of interrupts/добавлена передача через прерывания
 *	 			Zatonsky Pavel 					- 	created/создан
 *
 */
#include "hal_1967VN044.h"
#include <math.h>
volatile UART_TxRxStatus_type UartTxRxStatus;

static volatile struct
{
	uint32_t TxLenght[ UART_NUMS ];																	// The amount of data for transmission by means of interrupts/Количество данных для передачи по прерываниям
	uint8_t *Buff[ UART_NUMS ];																		// Pointer to the buffer/Указатель на буфер
} UART_TX_Data;

static volatile struct
{
	uint32_t RxLenght[ UART_NUMS ];																	// The amount of data for transmission be means of interrupts/Количество данных для передачи по прерываниям
	uint32_t RxMaxLenght[ UART_NUMS ];																// The maximum amount of data for receiving/Максимальное количество данных для приема
	uint8_t *Buff[ UART_NUMS ];																		// Pointer to the buffer/Указатель на буфер
} UART_RX_Data;

#pragma interrupt
static void vUART0_IrqHandler( void );

#pragma interrupt
static void vUART1_IrqHandler( void );

// Настройка UART
uint32_t HAL_UART_Init( UART_type* UARTx, UART_Init_type* InitStruct )
{
	uint32_t tmpreg = 0;
	uint32_t XTIBaud;

	XTIBaud = XTI_KHZ * 1000;

	if ( ( UARTx != ( UART_type* ) base_UART0 ) && ( UARTx != ( UART_type* ) base_UART1 ) )			// Checking of UART/Проверка UART
		return 1; 																					// Error: wrong UART/Ошибка: неправильный UART

	tmpreg |= InitStruct->WorkMode;
	tmpreg |= InitStruct->OverSampling;

	if ( InitStruct->BitRate == 0 ) return 2; 														// Error: the exchange rate = 0/Ошибка: скорость обмена = 0

	if ( InitStruct->OverSampling == UART_OverSampling_High )
	{
		if ( InitStruct->BitRate > XTIBaud / 16 ) return 4; 										// Error: the exchange rate if higher than allowed for the current bit duration/Ошибка: скорость обмена выше допустимой для текущей длительности бита
	}
	else
	{
		if ( InitStruct->BitRate > XTIBaud / 4 ) return 3; 											// Error: the exchange rate if higher than allowed for the current bit duration/Ошибка: скорость обмена выше допустимой для текущей длительности бита
	}

	if ( InitStruct->OverSampling == UART_OverSampling_High )
		UARTx->UBitRate = round( XTIBaud / ( InitStruct->BitRate * 16 ) ) - 1;
	else
		UARTx->UBitRate = round( XTIBaud / ( InitStruct->BitRate * 4 ) ) - 1;


	tmpreg |= InitStruct->WordLength;
	tmpreg |= InitStruct->StopBits;
	tmpreg |= InitStruct->ParityMode;
	tmpreg |= InitStruct->FIFOSize;
	tmpreg |= InitStruct->TXDMode;
	tmpreg |= ( InitStruct->DMACtrlErr << 3 );

	UARTx->UCR_LOAD.word = tmpreg;																	// UART initialization/Инициализация UART

	UARTx->UCR_SET.word = ( 1 << UCR_UARTEN_P );													// UART enabling/Включение UART
	return 0;
}

// UART deinitialization/Сброс настроек UART
void HAL_UART_DeInit( UART_type* UARTx )
{
	UARTx->UBitRate = 0;
	UARTx->UINT.word = 0;
	UARTx->UCR_LOAD.word = 0;
}

// Initialization of UART structure by defaults values/Инициализация структуры UART значениями по-умолчанию
void HAL_UART_DefaultInitStruct( UART_Init_type* InitStruct )
{
	InitStruct->BitRate = 9600;
	InitStruct->WorkMode = UART_Mode_TxRx;
	InitStruct->OverSampling = UART_OverSampling_Normal;
	InitStruct->WordLength = UART_WordLen_8bit;
	InitStruct->StopBits = UART_Stop_1bit;
	InitStruct->ParityMode = UART_Parity_Off;
	InitStruct->FIFOSize = UART_FIFOSize_8Byte;
	InitStruct->TXDMode = UART_TXD_Direct;
	InitStruct->DMACtrlErr = UART_DMACtrl_Dis;
}

// Interrupts configuration/Настройка прерываний
void HAL_UART_ITConfig( UART_type* UARTx, uint32_t IT, UART_IT_type IT_State )
{
	if( IT_State == UART_IT_En )
		UARTx->UCR_SET.word = IT << 16;
	else
		UARTx->UCR_CLEAR.word = IT << 16;
}

// Enabling of UART interrups/Разрешение прерываний от UART
void HAL_UART_ITEnable( UART_type* UARTx )
{
	if( UARTx == LX_UART0 )
		HAL_Interrupt_Enable( intUART0, vUART0_IrqHandler );
	else
		HAL_Interrupt_Enable( intUART1, vUART1_IrqHandler );
}

// Disabling of UART interrups/Запрещение прерываний от UART
void HAL_UART_ITDisable( UART_type* UARTx )
{
	if( UARTx == LX_UART0 )
		HAL_Interrupt_Disable( intUART0 );
	else
		HAL_Interrupt_Disable( intUART1 );
}

// Buffer transmission by UART/Отправка буфера через UART
uint32_t HAL_UART_Send( UART_type* UARTx, uint8_t* BuffAddr, uint32_t Lenght )
{
	unsigned char data;
	uint32_t i;

	if ( ( UARTx != ( UART_type* ) base_UART0 ) && ( UARTx != ( UART_type* ) base_UART1 ) )			// Checking of UART/Проверка UART
		return 1; 																					// Error: wrong UART/Ошибка: неправильный UART

	for ( i = 0; i < Lenght; i++ )
	{
		while( UARTx->UFLAG.b.UTXFF ) ; 															// Wait until the transmission buffer is free/Ждем освобождения буфера передачи
		data = *( BuffAddr + i );
		UARTx->UDR = data;
	}
	return 0;
}

// Buffer transmission my means of UART with the help of DMA. The buffer must be 32-bit/Отправка буфера через UART с использованием DMA. Буфер должен быть 32-разрядный
void HAL_UART_DmaSend( UART_type* UARTx, uint32_t Channel, void* BuffAddr, uint32_t Lenght, void *DmaIsrHandler )
{
	__builtin_quad tcb;
	uint32_t *ptr = ( uint32_t* ) &tcb;

	if( Channel < 4 || Channel > 7 )
			Channel = 4;

	HAL_DMA_Stop( Channel );

	*( ptr + 0 ) = ( uint32_t ) BuffAddr;															// Information source/Источник информации
	*( ptr + 1 ) = ( Lenght << 16 ) | 1;															// The amount of words + address increment/Количество слов + инкремент адреса
	*( ptr + 2 ) = 0;																				// The number of X-transmission attempts + X address increment/Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = TCB_NORMAL|																		// The length of the transmitted data (operand) in one exchange cycle /Длина передаваемых данных (операнда) в одном цикле обмена
					TCB_DMAR;
	*( ptr + 3 ) |= ( ( uint32_t ) BuffAddr < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// The source is in the internal/external memory/Источник в внешней/внутренней памяти

	if( UARTx == LX_UART0 )
		HAL_DMA_RqstSet( Channel, dmaUART0 );
	else
		HAL_DMA_RqstSet( Channel, dmaUART1 );

	if ( DmaIsrHandler )																			// Setting of the interrupt if needed/Настройка прерывания, если оно нужно
	{
		switch( Channel )
		{
			case 4:
				HAL_Interrupt_Enable( intDMA4, DmaIsrHandler );
				break;
			case 5:
				HAL_Interrupt_Enable( intDMA5, DmaIsrHandler );
				break;
			case 6:
				HAL_Interrupt_Enable( intDMA6, DmaIsrHandler );
				break;
			case 7:
				HAL_Interrupt_Enable( intDMA7, DmaIsrHandler );
				break;
			default:
				break;
		}
		*( ptr + 3 ) |= TCB_INT;
	}

	HAL_DMA_WriteDC( Channel, &tcb );
}

// Buffer receiving my means of UART with the help of DMA. The buffer must be 32-bit/Прием буфера через UART с использованием DMA. Буфер должен быть 32-разрядный
void HAL_UART_DmaReceive( UART_type* UARTx, uint32_t Channel, void* BuffAddr, uint32_t Lenght, void *DmaIsrHandler )
{
	__builtin_quad tcb;
	uint32_t *ptr = ( uint32_t* ) &tcb;

	if( Channel < 8 || Channel > 11 )
			Channel = 8;

	HAL_DMA_Stop( Channel );

	*( ptr + 0 ) = ( uint32_t ) BuffAddr;															// Information source/Источник информации
	*( ptr + 1 ) = ( Lenght << 16 ) | 1;															// The amount of words + address increment/Количество слов + инкремент адреса
	*( ptr + 2 ) = 0;																				// The number of X-transmission attempts + X address increment/Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = TCB_NORMAL|																		// The length of the transmitted data (operand) in one exchange cycle /Длина передаваемых данных (операнда) в одном цикле обмена
					TCB_DMAR;
	*( ptr + 3 ) |= ( ( uint32_t ) BuffAddr < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// The source is in the internal/external memory/Источник в внешней/внутренней памяти

	if( UARTx == LX_UART0 )
		HAL_DMA_RqstSet( Channel, dmaUART0 );
	else
		HAL_DMA_RqstSet( Channel, dmaUART1 );

	if ( DmaIsrHandler )																			// Setting of the interrupt if needed/Настройка прерывания, если оно нужно
	{
		switch( Channel )
		{
			case 8:
				HAL_Interrupt_Enable( intDMA8, DmaIsrHandler );
				break;
			case 9:
				HAL_Interrupt_Enable( intDMA9, DmaIsrHandler );
				break;
			case 10:
				HAL_Interrupt_Enable( intDMA10, DmaIsrHandler );
				break;
			case 11:
				HAL_Interrupt_Enable( intDMA11, DmaIsrHandler );
				break;
			default:
				break;
		}
		*( ptr + 3 ) |= TCB_INT;
	}

	HAL_DMA_WriteDC( Channel, &tcb );
}

// Buffer transmission with the use of interrupts/Передача буфера с использованием прерываний
void HAL_UART_ITSend( UART_type* UARTx, uint8_t* BuffAddr, uint32_t Lenght )
{
	uint32_t UartNum;

	if( UARTx == LX_UART0 )
		UartNum = 0;
	else
		UartNum = 1;

	UARTx->UCR_SET.b.UFIFOEN = 1;																	// Enable FIFO/Включаем FIFO

	UartTxRxStatus.TxReady[ UartNum ] = 0;
	UART_TX_Data.TxLenght[ UartNum ] = Lenght;
	UART_TX_Data.Buff[ UartNum ] = BuffAddr;

	while( !( UARTx->UFLAG.b.UTXFF ) && UART_TX_Data.TxLenght[ UartNum ] )
	{
		UARTx->UDR = *UART_TX_Data.Buff[ UartNum ]++;
		UART_TX_Data.TxLenght[ UartNum ]--;
	}

	HAL_UART_ITConfig( UARTx, UART_IT_TX, UART_IT_En );												// The intterupt from transmitter is enabled/Разрешаем прерывание от передатчика
}

// Buffer operates in receiving mode with interrupts/Прием буфера с использованием прерываний
void HAL_UART_ITReceive( UART_type* UARTx, uint8_t* BuffAddr, uint32_t MaxLenght )
{
	uint32_t UartNum;

	if( UARTx == LX_UART0 )
		UartNum = 0;
	else
		UartNum = 1;

	UARTx->UCR_SET.b.UFIFOEN = 1;																	// FIFO disabling/Включаем FIFO

	UartTxRxStatus.RxReady[ UartNum ] = 0;
	UartTxRxStatus.RxLenght[ UartNum ] = 0;

	UART_RX_Data.Buff[ UartNum ] = BuffAddr;
	UART_RX_Data.RxMaxLenght[ UartNum ] = MaxLenght;
	UART_RX_Data.RxLenght[ UartNum ] = 0;

	HAL_UART_ITConfig( UARTx, UART_IT_RX | UART_IT_RXERR | UART_IT_URXT, UART_IT_En );				// Enable the interrupts from the receiver/Разрешаем прерывания от приемника
}

/* ------------------------------------- Interrupt Handlers/Обработчики прерываний --------------------------------- */
void vUART0_IrqHandler( void )
{
	volatile uint32_t Tmp;
	uint32_t rxCnt;

	if( LX_UART0->UINTM.b.URXTINT )																	// Interrupt from receiver by timeout/Прерывание от приемника по таймауту
	{
		while( !LX_UART0->UFLAG.b.URXFE )
		{
			if( UART_RX_Data.RxLenght[ UART0 ] < UART_RX_Data.RxMaxLenght[ UART0 ] )
			{
				*UART_RX_Data.Buff[ UART0 ]++ = LX_UART0->UDR;
				UART_RX_Data.RxLenght[ UART0 ]++;
			}
			else
				Tmp = LX_UART0->UDR;
		}
		UartTxRxStatus.RxLenght[ UART0 ] = UART_RX_Data.RxLenght[ UART0 ];
		UartTxRxStatus.RxReady[ UART0 ] = 1;
	}

	if( LX_UART0->UINTM.b.RXINT )																	// Interrupt from receiver/ Прерывание от приемника
	{
		rxCnt = 0;
		while( rxCnt < 3 )
		{
			if( UART_RX_Data.RxLenght[ UART0 ] < UART_RX_Data.RxMaxLenght[ UART0 ] )
			{
				*UART_RX_Data.Buff[ UART0 ]++ = LX_UART0->UDR;
				UART_RX_Data.RxLenght[ UART0 ]++;
			}
			else
				Tmp = LX_UART0->UDR;

			rxCnt++;
		}
	}

	if( LX_UART0->UINTM.b.TXINT )																	// Interrupt from transmitter/Прерывание от передатчика
	{
		if( UART_TX_Data.TxLenght[ UART0 ] != 0 )
		{
			while( !LX_UART0->UFLAG.b.UTXFF && UART_TX_Data.TxLenght[ UART0 ] )
			{
				LX_UART0->UDR = *UART_TX_Data.Buff[ UART0 ]++;
				UART_TX_Data.TxLenght[ UART0 ]--;
			}
		}
		else
		{
			HAL_UART_ITConfig( LX_UART0, UART_IT_TX /*| UART_IT_URXT*/, UART_IT_Dis );
			HAL_UART_ITConfig( LX_UART0, UART_IT_UTXE, UART_IT_En );
		}
	}

	if( LX_UART0->UINTM.b.UTXEINT )																	// Transmitter buffer is empty/Буфер передатчика пуст
	{
		UartTxRxStatus.TxReady[ UART0 ] = 1;
		HAL_UART_ITConfig( LX_UART0, UART_IT_UTXE, UART_IT_Dis );
	}
}

void vUART1_IrqHandler( void )
{
	volatile uint32_t Tmp;
	uint32_t rxCnt;

	if( LX_UART1->UINTM.b.URXTINT )																	// Interrupt from receiver by timeout/Прерывание от приемника по таймауту
	{
		while( !LX_UART1->UFLAG.b.URXFE )
		{
			if( UART_RX_Data.RxLenght[ UART1 ] < UART_RX_Data.RxMaxLenght[ UART1 ] )
			{
				*UART_RX_Data.Buff[ UART1 ]++ = LX_UART1->UDR;
				UART_RX_Data.RxLenght[ UART1 ]++;
			}
			else
				Tmp = LX_UART1->UDR;
		}
		UartTxRxStatus.RxLenght[ UART1 ] = UART_RX_Data.RxLenght[ UART1 ];
		UartTxRxStatus.RxReady[ UART1 ] = 1;
	}

	if( LX_UART1->UINTM.b.RXINT )																	// Interrupt from receiver/Прерывание от приемника
	{
		rxCnt = 0;
		while( rxCnt < 3 )
		{
			if( UART_RX_Data.RxLenght[ UART1 ] < UART_RX_Data.RxMaxLenght[ UART1 ] )
			{
				*UART_RX_Data.Buff[ UART1 ]++ = LX_UART1->UDR;
				UART_RX_Data.RxLenght[ UART1 ]++;
			}
			else
				Tmp = LX_UART1->UDR;

			rxCnt++;
		}
	}

	if( LX_UART1->UINTM.b.TXINT )																	// Interrupt from transmitter/Прерывание от передатчика
	{
		if( UART_TX_Data.TxLenght[ UART1 ] != 0 )
		{
			while( !LX_UART1->UFLAG.b.UTXFF && UART_TX_Data.TxLenght[ UART1 ] )
			{
				LX_UART1->UDR = *UART_TX_Data.Buff[ UART1 ]++;
				UART_TX_Data.TxLenght[ UART1 ]--;
			}
		}
		else
		{
			HAL_UART_ITConfig( LX_UART1, UART_IT_TX, UART_IT_Dis );
			HAL_UART_ITConfig( LX_UART1, UART_IT_UTXE, UART_IT_En );
		}
	}

	if( LX_UART1->UINTM.b.UTXEINT )																	// Transmitter buffer is empty/Буфер передатчика пуст
	{
		UartTxRxStatus.TxReady[ UART1 ] = 1;
		HAL_UART_ITConfig( LX_UART1, UART_IT_UTXE, UART_IT_Dis );
	}
}
