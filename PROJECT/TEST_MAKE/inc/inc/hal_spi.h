/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_spi.h
 *	Описание: 	HAL для модуля SPI
 *
 *	История:
 *	 			23-May-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *				15-Mar-2017 Ivan Osadchy 		- 	создан
 *
 */
#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t DSS 	: 5;																		// Выбор размера данных DSS+1
		uint32_t RES1 	: 1;																		//
		uint32_t SPO 	: 1;																		// Полярность SPI_CLK: 0 - высокий импульс, 1 - низкий импульс
		uint32_t SPH 	: 1;																		// Активная фаза SPI_CLK: 0 - задний фронт, 1 - передний фронт
		uint32_t TWI_ON : 1; 																		// 1 – 3-х проводной интерфейс с двунаправленной линией
		uint32_t TWI_RW : 1; 																		// Чтение(0) или запись(1) в режиме TWI
		uint32_t LMSF 	: 1; 																		// Порядок выдачи бит: 0 - старший первый, 1 - младший первый
		uint32_t RES2 	: 1; 																		//
		uint32_t CSN 	: 3; 																		// Выбор номера SPI устройства
		uint32_t RES3 	: 1; 																		//
		uint32_t SCR 	: 12; 																		// Коэффициент деления частоты
		uint32_t RES4 	: 4; 																		//
	} b;
} SPI_SPCR0_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RIM 	: 1; 																		// Управление прерыванием от приемника интерфейса
		uint32_t TIM 	: 1; 																		// Управление прерыванием от передатчика интерфейс
		uint32_t LBM 	: 1; 																		// Разрешение тестового режима работы
		uint32_t SPE 	: 1; 																		// Разрешение работы интерфейса
		uint32_t MS 	: 1; 																		// Режим работы
		uint32_t TUM 	: 1; 																		// Разрешение прерывания по исчерпанию буфера передатчика при работе в режиме подчиненного
		uint32_t ROM 	: 1; 																		// Разрешение прерывания при переполнении буфера приемника
		uint32_t RES1 	: 1; 																		//
		uint32_t R_RQM 	: 1; 																		// Запрос на обслуживание со стороны FIFO приемника
		uint32_t T_RQM 	: 1; 																		// Запрос на обслуживание со стороны FIFO передатчика
		uint32_t TXO 	: 1; 																		// Только передача
		uint32_t RXO 	: 1; 																		// Только прием
		uint32_t CIM 	: 1; 																		// Разрешение прерывания от счетчика приемника
		uint32_t RES2 	: 1; 																		//
		uint32_t HOLDCS	: 1; 																		// Удержание линии CS активной после завершения обмена
		uint32_t ROTL 	: 1; 																		// Выходной уровень на выходе данных в случае работы в режиме «только прием»
		uint32_t CSAL 	: 6; 																		// Выбор активного уровня на выходе CSi
		uint32_t RES3 	: 2; 																		//
		uint32_t DLY_T 	: 4; 																		// Длительность неактивного уровня CS (при HOLDCS=0)
		uint32_t RES4 	: 4; 																		//
	} b;
} SPI_SPCR1_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t TFE 	: 1; 																		// Флаг заполнения FIFO передатчика («пустой»)
		uint32_t TNF 	: 1; 																		// Флаг заполнения FIFO передатчика («не полный»)
		uint32_t RNE 	: 1; 																		// Флаг пустого FIFO приемника («не пустой»)
		uint32_t BSY 	: 1; 																		// Занятость SPI интерфейса
		uint32_t TFS 	: 1; 																		// Флаг состояния FIFO передатчика
		uint32_t RFS 	: 1; 																		// Флаг состояния FIFO приемника
		uint32_t ROR 	: 1; 																		// Флаг переполнения FIFO приемника
		uint32_t TUR 	: 1; 																		// Флаг состояния FIFO передатчика (только в режиме «подчиненный») («чтение пустого буфера»)
		uint32_t RFF 	: 1; 																		// Флаг состояния FIFO приемника («полный»)
		uint32_t TI_REQ : 1; 																		// Запрос передатчика к процессору или ПДП
		uint32_t RI_REQ : 1; 																		// Запрос приемника к процессору или ПДП
		uint32_t RES 	: 21; 																		// 31:9
	} b;
} SPI_SPSR_type;

/* ------------------------------------ Структура периферии ------------------------------------- */

typedef volatile struct
{
	SPI_SPCR0_type 	SPCR0;																			// Регистр управления  0
	SPI_SPCR1_type 	SPCR1;																			// Регистр управления  1
	uint32_t 		SPDR;																			// Регистр данных
	SPI_SPSR_type 	SPSR;																			// Регистр состояния
	uint32_t 		RX_CNT;																			// Счетчик приемника
} SPI_type;

#define LX_SPI0                 ( ( volatile SPI_type * ) base_SPI0 )
#define LX_SPI1                 ( ( volatile SPI_type * ) base_SPI1 )
#define LX_SPI2                 ( ( volatile SPI_type * ) base_SPI2 )

/* -------------------------------------- Специфические типы ------------------------------------ */

// Структура для инициализации:
typedef enum
{
	SPI_LoopBack_Off 		= (0 <<  2),
	SPI_LoopBack_On			= (1 <<  2)
} SPI_LOOPBACK;

typedef enum
{
	SPI_Mode_Master 		= (0 <<  4),
	SPI_Mode_Slave 			= (1 <<  4)
} SPI_MODE;

typedef enum
{
	SPI_CLK_Polarity_High 	= (0 <<  6),
	SPI_CLK_Polarity_Low 	= (1 <<  6)
} SPI_CLK_POLARITY;

typedef enum
{
	SPI_CLK_Phase_Negedge 	= (0 <<  7),
	SPI_CLK_Phase_Posedge 	= (1 <<  7)
} SPI_CLK_PHASE;

typedef enum
{
	SPI_FirstBit_MSB 		= (0 <<  10),
	SPI_FirstBit_LSB 		= (1 <<  10)
} SPI_FIRSTBIT;

typedef enum
{
	SPI_CS_Unhold 			= (0 << 14),
	SPI_CS_Hold 			= (1 << 14)
} SPI_CSHOLD;

typedef enum
{
	SPI_CS_Active_Low 	 	= (0 << 16),
	SPI_CS_Active_High 		= (1 << 16)
} SPI_CSACTIVE;

typedef struct
{
	uint8_t 			WordSize;																	// Размер данных
	SPI_CLK_POLARITY	CLK_Polarity;																// Полярность CLK
	SPI_CLK_PHASE		CLK_Phase;																	// Фаза CLK
	SPI_FIRSTBIT 		FirstBit;																	// Порядок выдачи бит
	uint8_t				CSNum;																		// Номер вывода CS
	uint16_t			CLK_Prescaler;																// Коэффициент деления частоты: Скорость обмена = SOC_CLK / ( CLK_Prescaler + 1 )
	SPI_MODE 			Mode;																		// Режим работы
	SPI_CSHOLD 			CS_Hold;																	// Удержание линии CS после обмена
	uint8_t 			CS_Hold_Delay;																// Длительность неактивного уровня при CS_Unhold в тактах SCK
	SPI_CSACTIVE 		CS_Active;																	// Активный уровень CS
	SPI_LOOPBACK 		LoopBack;																	// Режим тестирования
} SPI_Init_type;


// Общий тип для выключения/включения:
typedef enum
{
	SPI_State_Disable = 0,
	SPI_State_Enable = 1
} SPI_State_type;

// Типы режимов обмена:
typedef enum
{
	SPI_ExchangeMode_All 	= 0,
	SPI_ExchangeMode_TxOnly = 1,
	SPI_ExchangeMode_RxOnly = 2
} SPI_ExchangeMode_type;


// Типы флагов статусного регистра и их значения:
typedef enum
{
	SPI_Flag_TxFifoEmpty 		= (1 << 0),
	SPI_Flag_TxFifoNotFull 		= (1 << 1),
	SPI_Flag_RxFifoNotEmpty 	= (1 << 2),
	SPI_Flag_Busy 				= (1 << 3),
	SPI_Flag_TxFifoHalfEmpty 	= (1 << 4),
	SPI_Flag_RxFifoHalfFull 	= (1 << 5),
	SPI_Flag_RxFifoOverflow 	= (1 << 6),
	SPI_Flag_TxFifoReadEmpty 	= (1 << 7),
	SPI_Flag_RxFifoFull 		= (1 << 8),
	SPI_Flag_TxReq 				= (1 << 9),
	SPI_Flag_RxReq 				= (1 << 10),
} SPI_Flag_type;

// Типы прерываний:
typedef enum
{
	SPI_IT_TxFifoNotFull,
	SPI_IT_TxFifoHalfEmpty,
	SPI_IT_TxReadEmptyFifo,
	SPI_IT_RxFifoNotEmpty,
	SPI_IT_RxFifoHalfFull,
	SPI_IT_RxFifoOverflow,
	SPI_IT_RxCntEnd
} SPI_IT_type;


typedef struct
{
	uint8_t Spi0_TxDmaReady;
	uint8_t Spi1_TxDmaReady;
	uint8_t Spi2_TxDmaReady;
	uint8_t Spi0_RxDmaReady;
	uint8_t Spi1_RxDmaReady;
	uint8_t Spi2_RxDmaReady;
}SPI_TransferStatus_type;

extern volatile SPI_TransferStatus_type SPI_TransferStatus;

#ifdef __cplusplus
  extern "C"
	{
	#endif
  	  void 					HAL_SPI_Init 				( SPI_type *SPI, SPI_Init_type *InitStr );
  	  void 					HAL_SPI_DeInit 				( SPI_type *SPI );
  	  void 					HAL_SPI_DefaultInitStruct 	( SPI_Init_type *InitStr );
  	  void 					HAL_SPI_Control				( SPI_type *SPI, SPI_State_type State );

  	  void 					HAL_SPI_SetExchangeMode 	( SPI_type *SPI, SPI_ExchangeMode_type Mode );
  	  SPI_ExchangeMode_type	HAL_SPI_GetExchangeMode		( SPI_type *SPI );

  	  uint32_t 				HAL_SPI_WriteToTxFIFO 		( SPI_type *SPI, uint32_t *src, uint32_t Size );
  	  uint32_t 				HAL_SPI_ReadFromRxFIFO		( SPI_type *SPI, uint32_t *dst, uint32_t Size );
  	  void 					HAL_SPI_SendOnly 			( SPI_type *SPI, uint32_t *src,  uint32_t Size );
  	  void 					HAL_SPI_ReceiveOnly 		( SPI_type *SPI, uint32_t *dst, uint32_t Size, uint8_t DO_State );
  	  void 					HAL_SPI_SendAndReceive 		( SPI_type *SPI, uint32_t *src, uint32_t *dst, uint16_t Size );

  	  SPI_State_type 		HAL_SPI_GetFlag 			( SPI_type *SPI, const SPI_Flag_type Flag );
  	  void 					HAL_SPI_ClearFlag 			( SPI_type *SPI, const SPI_Flag_type Flag );

  	  void 					HAL_SPI_ITConfig 			( SPI_type *SPI, const SPI_IT_type IT, const SPI_State_type State );
  	  SPI_State_type		HAL_SPI_ITCheck 			( SPI_type *SPI, const SPI_IT_type IT );

  	  void 					HAL_SPI_DMA_SendOnly		( SPI_type *SPI, uint32_t ulChannel, uint32_t *pusBuff, uint16_t usSize );
  	  void 					HAL_SPI_DMA_ReceiveOnly		( SPI_type *SPI, uint32_t ulChannel, uint32_t *pusBuff, uint16_t usSize );
  	  void 					HAL_SPI_DMA_SendAndReceive	( SPI_type *SPI, uint32_t ulChannelRx, uint32_t *pusBuffRx, uint32_t ulChannelTx, uint32_t *pusBuffTx, uint16_t usSize );
	#ifdef __cplusplus
	}
#endif /* __cplusplus */

#endif /* __HAL_SPI_H__ */
