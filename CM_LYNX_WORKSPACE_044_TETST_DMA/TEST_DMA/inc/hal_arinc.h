/*
 *
 *	Файл: 		hal_arinc.h
 *	Описание: 	HAL для модуля контроллера интерфейса ARINC
 *
 *	История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	частота шины периферии берется из PLL_Freq
 *						    Zatonsky Pavel 		- 	создан
 *
 */

#ifndef __HAL_ARINC_H__
#define __HAL_ARINC_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t CH_EN		:1;																		// Разрешение работы канала
		uint32_t CLK		:1;																		// Скорость приема данных
		uint32_t LB_EN		:1;																		// Разрешение обнаружения меток
		uint32_t SD_EN		:1;																		// Разрешение декодирования бит данных 9 и 10
		uint32_t DA			:1;																		// Бит прямого доступа
		uint32_t SDI1		:1;																		// Бит сравнения SDI1
		uint32_t SDI2		:1;																		// Бит сравнения SDI2
		uint32_t ENSYNC		:1;																		// Разрешение работы входов приемника в режиме данных и синхросигнала
		uint32_t ENPAR		:1;																		// Разрешение 32 бита паритета для канала
		uint32_t ODD		:1;																		// Выбор четности или нечетности бита паритета
		uint32_t RSV0		:2;																		//
		uint32_t INTEDR		:1;																		// Разрешение прерывания наличие данных в FIFO
		uint32_t INTEER		:1;																		// Разрешение прерывания ошибка приема
		uint32_t INTEFF		:1;																		// Разрешение прерывания FIFO полно
		uint32_t INTEHF		:1;																		// Разрешение прерывания FIFO наполовину полно
		uint32_t DIV		:8;																		// Делитель частоты SOC шины до 1 МГц
		uint32_t RSV1		:8;																		//
	} b;
} ARINC_RX_CONTROL_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t DR			:1;																		// Бит наличия данных в FIFO
		uint32_t ERR		:1;																		// Бит ошибки
		uint32_t FF        	:1;																		// Бит полноты FIFO
		uint32_t HF         :1;																		// Бит наполненности FIFO
		uint32_t IRQ_DR     :1;																		// Запрос прерывания при наличии данных в FIFO
		uint32_t IRQ_ERR    :1;																		// Запрос прерывания при наличии ошибки
		uint32_t IRQ_FF    	:1;																		// Запрос прерывания при полном заполнении FIFO
		uint32_t IRQ_HF    	:1;																		// Запрос прерывания при заполнении FIFO наполовину
		uint32_t IRQ_RX_CH0	:1;																		// Запрос прерывания от канала 0
		uint32_t IRQ_RX_CH1	:1;																		// Запрос прерывания от канала 1
		uint32_t IRQ_RX_CH2	:1;																		// Запрос прерывания от канала 2
		uint32_t IRQ_RX_CH3 :1;																		// Запрос прерывания от канала 3
		uint32_t IRQ_RX_CH4 :1;																		// Запрос прерывания от канала 4
		uint32_t IRQ_RX_CH5 :1;																		// Запрос прерывания от канала 5
		uint32_t IRQ_RX_CH6 :1;																		// Запрос прерывания от канала 6
		uint32_t IRQ_RX_CH7 :1;																		// Запрос прерывания от канала 7
		uint32_t CHAN_WP    :8;																		// Указатель на запись данных
		uint32_t DATA_RP    :8;																		// Указатель на чтение данных
	} b;
} ARINC_RX_STATUS_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t CH_EN   	:1;																		// Разрешение работы канала
		uint32_t CLK     	:1;																		// Скорость передачи данных
		uint32_t EN_PAR  	:1;																		// Разрешение 32 бита паритета
		uint32_t ODD      	:1;																		// Выбор четности или нечетности бита паритета
		uint32_t INTE_TXR 	:1;																		// Разрешение прерывания при опустошении буфера FIFO
		uint32_t INTE_FFT 	:1;																		// Разрешение прерывания при полном заполнении буфера FIFO
		uint32_t INTE_HFT	:1;																		// Разрешение прерывания при заполнении наполовину или менее буфера FIFO
		uint32_t ENSYNC  	:1;																		// Разрешение работы выходов передатчика в режиме данных и синхросигнала
		uint32_t DIV     	:8;																		// Делитель частоты SOC шины до 1 МГц
		uint32_t RSV     	:16;																	//
	} b;
} ARINC_TX_CONTROL_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t TX_R      	:1;																		// Флаг наличия данных в FIFO
		uint32_t BUSY      	:1;																		// Флаг занятости передатчика
		uint32_t FFT       	:1;																		// Флаг полноты FIFO
		uint32_t HFT        :1;																		// Флаг наполненности FIFO канала
		uint32_t IRQ_TXR    :1;																		// Запрос прерывания если FIFO пусто
		uint32_t RSV0		:1;																		//
		uint32_t IRQ_FFT   	:1;																		// Запрос прерывания при полном заполнении   FIFO
		uint32_t IRQ_HFT   	:1;																		// Запрос прерывания при заполнении  FIFO наполовину или менее
		uint32_t IRQ_TX_CH0	:1;																		// Запрос прерывания от канала 0
		uint32_t IRQ_TX_CH1	:1;																		// Запрос прерывания от канала 1
		uint32_t IRQ_TX_CH2	:1;																		// Запрос прерывания от канала 2
		uint32_t IRQ_TX_CH3 :1;																		// Запрос прерывания от канала 3
		uint32_t RSV1       :4;																		//
		uint32_t DATA_WP    :8;																		// Значение  указателя на запись данных
		uint32_t CHAN_RP    :8;																		// Значение  указателя на чтение данных
	}b;
} ARINC_TX_STATUS_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t LABEL	:	8;																		// Метка
		uint32_t SDI	:	2;																		// Источник/приемник данных
		uint32_t DATA	:	19;																		// Данные
		uint32_t SSM	:	2;																		// Знак или статус
		uint32_t P		:	1;																		// Паритет, дополняет слово до нечетного
	}b;
} ARINC_Word_type;

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef volatile struct
{
	ARINC_RX_CONTROL_type	RX_CONTROL;
	uint32_t 				RSV;
	uint32_t 				RX_DATA;
	ARINC_RX_STATUS_type	RX_STATUS;
} ARINC_RX_type;

typedef volatile struct
{
	ARINC_TX_CONTROL_type	TX_CONTROL;
	uint32_t 				RSV;
	uint32_t 				TX_DATA;
	ARINC_TX_STATUS_type	TX_STATUS;
} ARINC_TX_type;

#define LX_ARINC_RX_CH0      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH0)
#define LX_ARINC_RX_CH1      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH1)
#define LX_ARINC_RX_CH2      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH2)
#define LX_ARINC_RX_CH3      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH3)
#define LX_ARINC_RX_CH4      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH4)
#define LX_ARINC_RX_CH5      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH5)
#define LX_ARINC_RX_CH6      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH6)
#define LX_ARINC_RX_CH7      ((volatile ARINC_RX_type    *) base_ARINT_RX_CH7)

#define LX_ARINC_RX_MEM0	 ((volatile uint32_t 	*) ARINC_RX_MEM0_LOC)
#define LX_ARINC_RX_MEM1	 ((volatile uint32_t 	*) ARINC_RX_MEM1_LOC)
#define LX_ARINC_RX_MEM2	 ((volatile uint32_t 	*) ARINC_RX_MEM2_LOC)
#define LX_ARINC_RX_MEM3	 ((volatile uint32_t 	*) ARINC_RX_MEM3_LOC)
#define LX_ARINC_RX_MEM4	 ((volatile uint32_t 	*) ARINC_RX_MEM4_LOC)
#define LX_ARINC_RX_MEM5	 ((volatile uint32_t 	*) ARINC_RX_MEM5_LOC)
#define LX_ARINC_RX_MEM6	 ((volatile uint32_t 	*) ARINC_RX_MEM6_LOC)
#define LX_ARINC_RX_MEM7	 ((volatile uint32_t 	*) ARINC_RX_MEM7_LOC)
#define LX_ARINC_RX_LABEL0	 ((volatile uint32_t 	*) ARINC_RX_LABEL0_LOC)
#define LX_ARINC_RX_LABEL1	 ((volatile uint32_t 	*) ARINC_RX_LABEL1_LOC)
#define LX_ARINC_RX_LABEL2	 ((volatile uint32_t 	*) ARINC_RX_LABEL2_LOC)
#define LX_ARINC_RX_LABEL3	 ((volatile uint32_t 	*) ARINC_RX_LABEL3_LOC)
#define LX_ARINC_RX_LABEL4	 ((volatile uint32_t 	*) ARINC_RX_LABEL4_LOC)
#define LX_ARINC_RX_LABEL5	 ((volatile uint32_t 	*) ARINC_RX_LABEL5_LOC)
#define LX_ARINC_RX_LABEL6	 ((volatile uint32_t 	*) ARINC_RX_LABEL6_LOC)
#define LX_ARINC_RX_LABEL7	 ((volatile uint32_t 	*) ARINC_RX_LABEL7_LOC)

#define LX_ARINC_TX_CH0      ((volatile ARINC_TX_type    *) base_ARINT_TX_CH0)
#define LX_ARINC_TX_CH1      ((volatile ARINC_TX_type    *) base_ARINT_TX_CH1)
#define LX_ARINC_TX_CH2      ((volatile ARINC_TX_type    *) base_ARINT_TX_CH2)
#define LX_ARINC_TX_CH3      ((volatile ARINC_TX_type    *) base_ARINT_TX_CH3)

#define LX_ARINC_TX_MEM0	 ((volatile uint32_t 	*) ARINC_TX_MEM0_LOC)
#define LX_ARINC_TX_MEM1	 ((volatile uint32_t 	*) ARINC_TX_MEM1_LOC)
#define LX_ARINC_TX_MEM2	 ((volatile uint32_t 	*) ARINC_TX_MEM2_LOC)
#define LX_ARINC_TX_MEM3	 ((volatile uint32_t 	*) ARINC_TX_MEM3_LOC)

/* -------------------------------------- Специфические типы ------------------------------------ */
typedef enum {ARINC_Clk_100kHz, ARINC_Clk_12p5kHz} ARINC_Clk;
typedef enum {ARINC_LB_Dis, ARINC_LB_En} ARINC_LB;
typedef enum {ARINC_SD_Dis, ARINC_SD_En} ARINC_SD;
typedef enum {ARINC_DA_Dis, ARINC_DA_En} ARINC_DA;
typedef enum {ARINC_SDI_Null, ARINC_SDI_One} ARINC_SDI;
typedef enum {ARINC_Sync_Dis, ARINC_Sync_En} ARINC_Sync;
typedef enum {ARINC_Parity_Dis, ARINC_Parity_Odd, ARINC_Parity_Even} ARINC_Parity;
typedef enum {ARINC_IT_Dis, ARINC_IT_En} ARINC_IT;

typedef enum {ARINC_RX_IT_nEmptyFIFO, ARINC_RX_IT_Err, ARINC_RX_IT_HalfFIFO, ARINC_RX_IT_FullFIFO} ARINC_RX_IT;
typedef enum {ARINC_TX_IT_EmptyFIFO, ARINC_TX_IT_HalfFIFO, ARINC_TX_IT_FullFIFO} ARINC_TX_IT;

typedef struct
{
	ARINC_Clk 		Clk;
	ARINC_LB		LB;
	ARINC_SD		SD;
	ARINC_DA		DA;
	ARINC_SDI		SDI1;
	ARINC_SDI		SDI2;
	ARINC_Sync		Sync;
	ARINC_Parity 	Parity;
	ARINC_IT		ITnEmptyFIFO;
	ARINC_IT		ITErr;
	ARINC_IT		ITFullFIFO;
	ARINC_IT		ITHalfFIFO;
} ARINC_RX_Init_type;

typedef struct
{
	ARINC_Clk		Clk;
	ARINC_Parity 	Parity;
	ARINC_Sync		Sync;
	ARINC_IT		ITEmptyFIFO;
	ARINC_IT		ITFullFIFO;
	ARINC_IT		ITHalfFIFO;
} ARINC_TX_Init_type;

#ifdef __cplusplus
extern "C"
	{
#endif
	void 		HAL_ARINC_RX_Init			( ARINC_RX_type *LX_ARINC_RX_CHx, ARINC_RX_Init_type *InitStruct );
	void 		HAL_ARINC_RX_DeInit			( ARINC_RX_type *LX_ARINC_RX_CHx );
	void 		HAL_ARINC_RX_ITEn			( ARINC_RX_type *LX_ARINC_RX_CHx, ARINC_RX_IT ITName );
	void 		HAL_ARINC_RX_ITDis			( ARINC_RX_type *LX_ARINC_RX_CHx, ARINC_RX_IT ITName );
	void 		HAL_ARINC_RX_Enable			( ARINC_RX_type *LX_ARINC_RX_CHx );
	void 		HAL_ARINC_RX_Disable		( ARINC_RX_type *LX_ARINC_RX_CHx );
	void 		HAL_ARINC_RX_LabelSet		( uint32_t *LX_ARINC_RX_LABELx, uint32_t *CorrectLabel, uint32_t Size );
	void 		HAL_ARINC_RX_LabelReset		( uint32_t *LX_ARINC_RX_LABELx, uint32_t *IncorrectLabel, uint32_t Size );
	void 		HAL_ARINC_RX_LabelAllReset	( uint32_t *LX_ARINC_RX_LABELx );
	void 		HAL_ARINC_RX_DataRead		( ARINC_RX_type *LX_ARINC_RX_CHx, void *PtrRxArr, unsigned int Size );
	void 		HAL_ARINC_RX_MDataRead		( uint32_t *LX_ARINC_RX_MEMx, void *PtrRxArr, uint32_t Size );
	uint32_t 	HAL_ARINC_RX_AllDataRead	( ARINC_RX_type *LX_ARINC_RX_CHx, void *PtrRxArr );

	void 		HAL_ARINC_TX_Init			( ARINC_TX_type *LX_ARINC_TX_CHx, ARINC_TX_Init_type *InitStruct );
	void 		HAL_ARINC_TX_DeInit			( ARINC_TX_type *LX_ARINC_TX_CHx );
	void 		HAL_ARINC_TX_ITEn			( ARINC_TX_type *LX_ARINC_TX_CHx, ARINC_TX_IT ITName );
	void	 	HAL_ARINC_TX_ITDis			( ARINC_TX_type *LX_ARINC_TX_CHx, ARINC_TX_IT ITName );
	void 		HAL_ARINC_TX_DataWrite		( ARINC_TX_type *LX_ARINC_TX_CHx, void *PtrTxArr, uint32_t Size );
	void 		HAL_ARINC_TX_Enable			( ARINC_TX_type *LX_ARINC_TX_CHx );
	void 		HAL_ARINC_TX_Disable		( ARINC_TX_type *LX_ARINC_TX_CHx );
#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_ARINC_H__ */
