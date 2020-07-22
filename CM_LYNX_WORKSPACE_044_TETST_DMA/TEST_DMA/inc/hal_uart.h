/*
 *
 *	Файл: 		hal_uart.h
 *	Описание: 	HAL для модуля последовательного асинхронного интерфейса UART
 *
 *	История:
 *				27-Jun-2017 Dmitriy Sharaviev 	- 	добавлена передача через прерывания
 *	 			Zatonsky Pavel 					- 	создан
 *
 */

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t FRAME 		:1;
		uint32_t PARITY 	:1;
		uint32_t OVERRUN 	:1;
		uint32_t ERROR 		:1;
		uint32_t res 		:28;
	} b;
} UART_RXSTAT;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t UARTEN 	:1;
		uint32_t UTXDIS 	:1;
		uint32_t URXDIS 	:1;
		uint32_t DMAONERR 	:1;
		uint32_t UTXFDIS 	:1;
		uint32_t URXFDIS 	:1;
		uint32_t UHBRE 		:2;
		uint32_t BREAK 		:1;
		uint32_t PRTEN 		:1;
		uint32_t EVENPRT 	:1;
		uint32_t XSTOP 		:1;
		uint32_t UFIFOEN 	:1;
		uint32_t WRDLEN 	:2;
		uint32_t INV 		:1;
		uint32_t TXINT 		:1;
		uint32_t RXINT 		:1;
		uint32_t RXERRINT 	:1;
		uint32_t MSINT 		:1;
		uint32_t UDINT 		:1;
		uint32_t UTXEINT 	:1;
		uint32_t URXTINT 	:1;
		uint32_t RSV 		:8;
		uint32_t LBM 		:1;
	} b;
} UART_CR;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t res_low 	:3;
		uint32_t UBUSY 		:1;																		// 1 - идет передача данных
		uint32_t URXFE 		:1;																		// 1 – буфер приемника пуст
		uint32_t UTXFF 		:1;																		// 1 – буфер передатчика заполнен полностью
		uint32_t res_high 	:26;
	} b;
} UART_FLAG;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t TXINT 		:1;
		uint32_t RXINT 		:1;
		uint32_t RXERRINT 	:1;
		uint32_t MSINT 		:1;
		uint32_t UDINT 		:1;
		uint32_t UTXEINT 	:1;
		uint32_t URXTINT 	:1;
		uint32_t RSV 		:25;
	} b;
} UART_INT;

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef volatile struct
{
	uint32_t UDR;																					// Регистр данных
	UART_RXSTAT RXSTAT;																				// Регистр состояния приемника
	UART_CR UCR_LOAD;																				// Регистр управления загрузка
	UART_CR UCR_SET;																				// Регистр управления установка
	UART_CR UCR_CLEAR;																				// Регистр управления сброс
	uint32_t UBitRate;																				// Регистр управления скоростью обмена
	UART_FLAG UFLAG;																				// Регистр флагов
	UART_INT UINTM;																					// Регистр запросов прерываний (маскированный)
	UART_INT UINT;																					// Регистр запросов прерываний  (немаскированный)
} UART_type;

#define LX_UART0           ( ( volatile UART_type * ) base_UART0 )
#define LX_UART1           ( ( volatile UART_type * ) base_UART1 )

/* -------------------------------------- Специфические типы ------------------------------------ */
#define UART_IT_TX			0x00000001																// Запрос прерывания от передатчика
#define UART_IT_RX			0x00000002																// Запрос прерывания от приемника
#define UART_IT_RXERR		0x00000004																// Ошибка приема во время работы с контроллером  DMA
#define UART_IT_MS			0x00000008																// Запрос прерывания от модема
#define UART_IT_UD			0x00000010																// Запрос прерывания от приемника в состоянии «выключен»
#define UART_IT_UTXE		0x00000020																// Запрос прерывания от буфера передатчика при отсутствии в нем данных
#define UART_IT_URXT		0x00000040																// Запрос прерывания от приемника при обнаружении ситуации “time-out”

typedef enum
{
	UART0 = 0,
	UART1,
	UART_NUMS
}UART_UartNum_type;

typedef enum
{
	UART_WordLen_8bit = 0,
	UART_WordLen_7bit = ( 1 << 13 ),
	UART_WordLen_6bit = ( 2 << 13 ),
	UART_WordLen_5bit = ( 3 << 13 )
} UART_WordLen_type;

typedef enum
{
	UART_TXD_Direct = 0,																			// На TX передаваемые данные
	UART_TXD_Low = ( ( 1 << 8 ) | ( 1 << 15 ) ),													// TX всегда 0
	UART_TXD_High = ( ( 1 << 8 ) | ( 0 << 15 ) ),													// TX всегда 1
	UART_TXD_Compliment = ( ( 0 << 8 ) | ( 1 << 15 ) )												// На TX инверсия передаваемых данных
} UART_TXD_type;

typedef enum
{
	UART_Parity_Off = 0,
	UART_Parity_Even = ( 1 << 9 ),
	UART_Parity_Odd = ( ( 1 << 9 ) | ( 1 << 10 ) )
} UART_Parity_type;

typedef enum
{
	UART_Mode_TxRx = 0,
	UART_Mode_Tx = ( ( 1 << 2 ) | ( 1 << 5 ) ),
	UART_Mode_Rx = ( ( 1 << 1 ) | ( 1 << 4 ) )
} UART_Mode_type;

typedef enum
{
	UART_Stop_1bit = 0,
	UART_Stop_2bit = ( 1 << 11 )
} UART_StopBits_type;

typedef enum
{
	UART_FIFOSize_Byte = 0,																			// Размер буфера 1 байт
	UART_FIFOSize_8Byte = ( 1 << 12 )																// Размер буфера 8 байт
} UART_FIFOSize_type;

typedef enum
{
	UART_DMACtrl_Dis = 0,																			// Обнаружение ошибки не влияет формирование запроса к контроллеру  DMA на обслуживание
	UART_DMACtrl_En = 1																				// Формирование запроса к контроллеру  DMA на обслуживание запрещено при обнаружении ошибки
} UART_DMACtrl_type;

typedef enum
{
	UART_IT_Dis = 0,
	UART_IT_En = 1
} UART_IT_type;

typedef enum
{
	UART_OverSampling_High = 0,
	UART_OverSampling_Normal = ( 1 << 6 )
} UART_OverSampling_type;

typedef struct
{
	uint32_t BitRate; 																				// Скорость обмена
	UART_Mode_type WorkMode; 																		// Режим работы
	UART_OverSampling_type OverSampling; 															// Длительность бита
	UART_WordLen_type WordLength; 																	// Длина передаваемых данных
	UART_StopBits_type StopBits; 																	// Количество стоп битов
	UART_Parity_type ParityMode; 																	// Контроль четности
	UART_FIFOSize_type FIFOSize;																	// Размер FIFO
	UART_TXD_type TXDMode; 																			// Настройка линии передатчика
	UART_DMACtrl_type DMACtrlErr; 																	// Настройка DMA при обнаружении ошибки
} UART_Init_type;

typedef struct
{
	uint8_t TxReady[ UART_NUMS ];																	// Все данные переданы
	uint8_t RxReady[ UART_NUMS ];																	// Приняты новые данные
	uint32_t RxLenght[ UART_NUMS ];																	// Количество принятых данных
} UART_TxRxStatus_type;

extern volatile UART_TxRxStatus_type UartTxRxStatus;

#ifdef __cplusplus
extern "C"
{
#endif
	uint32_t 	HAL_UART_Init				( UART_type* UARTx, UART_Init_type* InitStruct );
	void 		HAL_UART_DeInit				( UART_type* UARTx );
	void 		HAL_UART_DefaultInitStruct	( UART_Init_type* InitStruct );
	void 		HAL_UART_ITConfig			( UART_type* UARTx, uint32_t IT, UART_IT_type IT_State );
	void 		HAL_UART_ITEnable			( UART_type* UARTx );
	void 		HAL_UART_ITDisable			( UART_type* UARTx );

	uint32_t 	HAL_UART_Send				( UART_type* UARTx, uint8_t* BuffAddr, uint32_t amount );

	void 		HAL_UART_DmaSend			( UART_type* UARTx, uint32_t Channel, void* BuffAddr, uint32_t Lenght, void *DmaIsrHandler );
	void 		HAL_UART_DmaReceive			( UART_type* UARTx, uint32_t Channel, void* BuffAddr, uint32_t Lenght, void *DmaIsrHandler );

	void        HAL_UART_ITSend				( UART_type* UARTx, uint8_t* BuffAddr, uint32_t Lenght );
	void	 	HAL_UART_ITReceive			( UART_type* UARTx, uint8_t* BuffAddr, uint32_t MaxLenght );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_UART_H__ */
