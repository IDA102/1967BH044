/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_gtimer.h
 *	Описание: 	HAL для модуля таймеров с функцией захвата/ШИМ
 *
 *	История:
 *	 			20-Jun-2017 Dmitriy Sharaviev 	- 	создан
 *
 */

#ifndef __HAL_GTIMER_H__
#define __HAL_GTIMER_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CNT_EN 			:1;															// Разрешение работы таймера
			uint32_t ARRB_EN 			:1;															// Разрешение мгновенного обновления ARR
			uint32_t WR_CMPL 			:1;															// Окончание записи, при задании нового значения регистров CNT, PSG и ARR
			uint32_t DIR 				:1;															// Направление счета основного счетчика
			uint32_t FDTS 				:2;															// Частота семплирования данных FDTS
			uint32_t CNT_MODE 			:2;															// Режим счета основного счетчика
			uint32_t EVENT_SEL 			:4;															// Биты выбора источника событий
			uint32_t RESERVED 			:20;														//
		} b;
	};
} GTIMER_CNTRL_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CHFLTR 			:4;															// Фильтр
			uint32_t CHSEL 				:2;															// Выбор события по входному каналу
			uint32_t CHPSC 				:2;															// Предварительный делитель входного канала
			uint32_t OCCE 				:1;															// Разрешение работы  ETR
			uint32_t OCCM 				:3;															// Формат выработки сигнала REF в режиме ШИМ
			uint32_t BRKEN 				:1;															// Разрешение сброса по выводу BRK
			uint32_t ETREN 				:1;															// Разрешения сброса по выводу ETR
			uint32_t WR_CMPL 			:1;															// Флаг окончания записи, при задании нового значения регистра CCR
			uint32_t CAP_nPWM 			:1;															// Режим работы канала Захват или ШИМ
			uint32_t RESERVED 			:16;														//
		} b;
	};
} GTIMER_CHX_CNTRL_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t SELOE 				:2;															// Режим работы прямого канала на выход
			uint32_t SELO 				:2;															// Режим работы выхода прямого канала
			uint32_t INV 				:1;															// Режим выходной инверсии прямого канала
			uint32_t RESERVED 			:3;															//
			uint32_t NSELOE 			:2;															// Режим работы инверсного канала на выход
			uint32_t NSELO 				:2;															// Режим работы выхода инверсного канала
			uint32_t NINV 				:1;															// Режим выходной инверсии инверсного канала
			uint32_t RESERVED2 			:19;														//
		} b;
	};
} GTIMER_CHX_CNTRL1_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CHSEL1 			:2;															// Выбор события по входному каналу для CAP1
			uint32_t CCR1_EN 			:1;															// Разрешение работы регистра CCR1
			uint32_t CRRRLD 			:1;															// Разрешение обновления регистров CCR и CCR1
			uint32_t RESERVED 			:28;														//
		} b;
	};
} GTIMER_CHX_CNTRL2_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t DTG 				:4;															// Предварительный делитель частоты
			uint32_t EDTS 				:1;															// Частота работы DTG
			uint32_t RESERVED 			:3;															//
			uint32_t DTGX 				:8;															// Основной делитель частоты
			uint32_t RESERVED2 			:16;														//
		} b;
	};
} GTIMER_CHX_DTG_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t BRK_INV 			:1;															// Инверсия входа BRK
			uint32_t ETR_INV 			:1;															// Инверсия входа ETR
			uint32_t ETRPSC 			:2;															// Асинхронный предделитель внешней  частоты
			uint32_t ETR_FILTER 		:4;															// Цифровой фильтр на входе ETR
			uint32_t RESERVED2 			:24;														//
		} b;
	};
} GTIMER_BRKETR_CNTRL_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CNT_ZERO_EVENT 	:1;															// Событие совпадения CNT с нулем
			uint32_t CNT_ARR_EVENT 		:1;															// Событие совпадения CNT с ARR
			uint32_t ETR_RE_EVENT 		:1;															// Событие переднего фронта на входе ETR
			uint32_t ETR_FE_EVENT 		:1;															// Событие заднего фронта на входе ETR
			uint32_t BRK_EVENT 			:1;															// Триггерированное по PCLK состояние входа BRK
			uint32_t CCR_CAP_EVENT 		:4;															// Событие переднего фронта на входе CAP каналов таймера
			uint32_t CCR_REF_EVENT 		:4;															// Событие переднего фронта на выходе REF каналов таймера
			uint32_t CCR_CAP1_EVENT 	:4;															// Событие переднего фронта на входе CAP1 каналов таймера
			uint32_t RESERVED2 			:15;														//
		} b;
	};
} GTIMER_STATUS_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CNT_ZERO_EVENT_IE 	:1;															// Событие совпадения CNT с нулем
			uint32_t CNT_ARR_EVENT_IE 	:1;															// Событие совпадения CNT с ARR
			uint32_t ETR_RE_EVENT_IE 	:1;															// Событие переднего фронта на входе ETR
			uint32_t ETR_FE_EVENT_IE 	:1;															// Событие заднего фронта на входе ETR
			uint32_t BRK_EVENT_IE 		:1;															// Триггерированное по PCLK состояние входа BRK
			uint32_t CCR_CAP_EVENT_IE 	:4;															// Событие переднего фронта на входе CAP каналов таймера
			uint32_t CCR_REF_EVENT_IE 	:4;															// Событие переднего фронта на выходе REF каналов таймера
			uint32_t CCR_CAP1_EVENT_IE 	:4;															// Событие переднего фронта на входе CAP1 каналов таймера
			uint32_t RESERVED2 			:15;														//
		} b;
	};
} GTIMER_IE_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CNT_ZERO_EVENT_RE	:1;															// Событие совпадения CNT с нулем
			uint32_t CNT_ARR_EVENT_RE 	:1;															// Событие совпадения CNT с ARR
			uint32_t ETR_RE_EVENT_RE 	:1;															// Событие переднего фронта на входе ETR
			uint32_t ETR_FE_EVENT_RE 	:1;															// Событие заднего фронта на входе ETR
			uint32_t BRK_EVENT_RE 		:1;															// Триггерированное по PCLK состояние входа BRK
			uint32_t CCR_CAP_EVENT_RE 	:4;															// Событие переднего фронта на входе CAP каналов таймера
			uint32_t CCR_REF_EVENT_RE 	:4;															// Событие переднего фронта на выходе REF каналов таймера
			uint32_t CCR_CAP1_EVENT_RE 	:4;															// Событие переднего фронта на входе CAP1 каналов таймера
			uint32_t RESERVED2 			:15;														//
		} b;
	};
} GTIMER_DMA_RE_type;
/* ------------------------------------ Структура периферии ------------------------------------- */

typedef volatile struct
{
  uint32_t CNT;																						// Основной счетчик таймера CNT
  uint32_t PSG;																						// Делитель частоты при счете основного счетчика PSG
  uint32_t ARR;																						// Основание счета основного счетчика ARR
  GTIMER_CNTRL_type CNTRL;																			// Регистр управления основного счетчика CNTRL
  uint32_t CCRx[4];																					// Регистры сравнения/захвата для X канала таймера CCRX
  GTIMER_CHX_CNTRL_type CHx_CNTRL[4];																// Регистры управления для X канала таймера CHX_CNTRL
  GTIMER_CHX_CNTRL1_type CHx_CNTRL1[4];																// Регистры управления 1 для X канала таймера CHX_CNTRL1
  GTIMER_CHX_DTG_type CHx_DTG[4];																	// Регистры CHX_DTG управления DTG
  GTIMER_BRKETR_CNTRL_type BRKETR_CNTRL;																// Регистр BRKETR_CNTRL управления входом BRK и ETR
  GTIMER_STATUS_type STATUS;																			// Регистр статуса таймера STATUS
  GTIMER_IE_type IE;																					// Регистр разрешения прерывания таймера IE
  GTIMER_DMA_RE_type DMA_RE;																			// Регистр DMA_RE разрешения запросов DMA от прерываний таймера
  GTIMER_CHX_CNTRL2_type CHx_CNTRL2[4];																// Регистрs управления 2 для X канала таймера CHX_CNTRL2
  uint32_t CCRx1[4];																				// Регистрs сравнения/захвата для X канала таймера CCRX1
  uint32_t DMA_REChx[4];
} GTIMER_type;

#define LX_GTIMER0 		( ( volatile GTIMER_type * ) base_GTIMER0 )
#define LX_GTIMER1 		( ( volatile GTIMER_type * ) base_GTIMER1 )

/* -------------------------------------- Специфические типы ------------------------------------ */

typedef enum
{
	GTIMER_CH1 = 0,
	GTIMER_CH2,
	GTIMER_CH3,
	GTIMER_CH4
} GTIMER_CH_type;

typedef enum
{
	GTIMER_DMA_REQ1 = 0,
	GTIMER_DMA_REQ2,
	GTIMER_DMA_REQ3,
	GTIMER_DMA_REQ4,
	GTIMER_DMA_REQ
} GTIMER_DMA_REQ_type;

typedef enum
{
	GTIMER_Update_Immediately = 0,																	// Обновление возможно в любой момент времени
	GTIMER_Update_On_CNT_Overflow,																	// Обновление будет осуществлено только при CNT = 0
} GTIMER_Update_type;

typedef enum
{
	GTIMER_CntDir_Up = 0,																			// Счет вверх
	GTIMER_CntDir_Down,																				// Счет вниз
} GTIMER_CNTRL_DIR_type;

typedef enum
{
	GTIMER_FdtsDiv_1 = 0,																			// Частота семплирования каждый TIM_CLK
	GTIMER_FdtsDiv_2,																				// Частота семплирования каждый второй TIM_CLK
	GTIMER_FdtsDiv_3,																				// Частота семплирования каждый третий TIM_CLK
	GTIMER_FdtsDiv_4,																				// Частота семплирования каждый четвертый TIM_CLK
} GTIMER_CNTRL_FDTS_type;

typedef enum
{
	GTIMER_CntMode_ClkFixedDir = 0,																	// Счет без изменения направления от внутреннего тактового сигнала
	GTIMER_CntMode_ClkChangeDir,																		// Счет с изменением направления от внутреннего тактового сигнала
	GTIMER_CntMode_EvtFixedDir,																		// Счет без изменения направления от событий
	GTIMER_CntMode_EvtChangeDir,																		// Счет с изменением направления от событий
} GTIMER_CNTRL_CNT_MODE_type;

typedef enum
{
	GTIMER_EvSrc_None = 0,																			// Нет событий
	GTIMER_EvSrc_TM1,																				// CNT == ARR в таймере 1
	GTIMER_EvSrc_TM2,																				// CNT == ARR в таймере 2
	GTIMER_EvSrc_CH1 = 4,																			// Событие на первом канале
	GTIMER_EvSrc_CH2,																				// Событие на втором канале
	GTIMER_EvSrc_CH3,																				// Событие на третьем канале
	GTIMER_EvSrc_CH4,																				// Событие на четвертом канале
	GTIMER_EvSrc_ETR_RE,																				// Событие переднего фронта ETR
	GTIMER_EvSrc_ETR_FE,																				// Событие заднего фронта ETR
} GTIMER_CNTRL_EVENT_SEL_type;

typedef enum
{
	GTIMER_Filter_1FF_at_GTIMER_CLK = 0,    															// Сигнал зафиксирован в 1 триггере на частоте TIM_CLK
	GTIMER_Filter_2FF_at_GTIMER_CLK,																	// Сигнал зафиксирован в 2 триггерах на частоте TIM_CLK
	GTIMER_Filter_4FF_at_GTIMER_CLK,  																// Сигнал зафиксирован в 4 триггерах на частоте TIM_CLK
	GTIMER_Filter_8FF_at_GTIMER_CLK,  																// Сигнал зафиксирован в 8 триггерах на частоте TIM_CLK
	GTIMER_Filter_6FF_at_FDTS_div_2,  																// Сигнал зафиксирован в 6 триггерах на частоте FDTS/2
	GTIMER_Filter_8FF_at_FDTS_div_2,  																// Сигнал зафиксирован в 8 триггерах на частоте FDTS/2
	GTIMER_Filter_6FF_at_FDTS_div_4,        					  										// Сигнал зафиксирован в 6 триггерах на частоте FDTS/4
	GTIMER_Filter_8FF_at_FDTS_div_4,        															// Сигнал зафиксирован в 8 триггерах на частоте FDTS/4
	GTIMER_Filter_6FF_at_FDTS_div_8,        					  										// Сигнал зафиксирован в 6 триггерах на частоте FDTS/8
	GTIMER_Filter_8FF_at_FDTS_div_8,        															// Сигнал зафиксирован в 8 триггерах на частоте FDTS/8
	GTIMER_Filter_5FF_at_FDTS_div_16,       															// Сигнал зафиксирован в 5 триггерах на частоте FDTS/16
	GTIMER_Filter_6FF_at_FDTS_div_16,       				  											// Сигнал зафиксирован в 6 триггерах на частоте FDTS/16
	GTIMER_Filter_8FF_at_FDTS_div_16,      					 										// Сигнал зафиксирован в 8 триггерах на частоте FDTS/16
	GTIMER_Filter_5FF_at_FDTS_div_32,       					 										// Сигнал зафиксирован в 5 триггерах на частоте FDTS/32
	GTIMER_Filter_6FF_at_FDTS_div_32,      					  										// Сигнал зафиксирован в 6 триггерах на частоте FDTS/32
	GTIMER_Filter_8FF_at_FDTS_div_32       					  										// Сигнал зафиксирован в 8 триггерах на частоте FDTS/32
} GTIMER_Filter_type;

typedef enum
{
	GTIMER_Prescaler_None = 0,																		// нет деления
	GTIMER_Prescaler_2,																				// /2
	GTIMER_Prescaler_4,																				// /4
	GTIMER_Prescaler_8																				// /8
} GTIMER_Prescaler_type;

typedef enum
{
	GTIMER_Polarity_NonInverted = 0,																	// Нет инверсии входа
	GTIMER_Polarity_Inverted																			// Инверсия входа
} GTIMER_Polarity_type;

typedef enum
{
	GTIMER_CH_MODE_PWM = 0,																			// Канал работает в режиме ШИМ
	GTIMER_CH_MODE_CAPTURE  																			// Канал работает в режиме Захват
} GTIMER_ChMode_type;

typedef enum
{
	GTIMER_CH_RESET_DISABLE = 0,																		// Сброс по внешнему выводу запрещен
	GTIMER_CH_RESET_ENABLE  																			// Сброс по внешнему выводу разрешен
} GTIMER_ResetCh_type;

typedef enum
{
	GTIMER_EventIn_PE = 0,																			// Событие входного канала положительный фронт
	GTIMER_EventIn_NE,																				// Событие входного канала отрицательный фронт
	GTIMER_EventIn_EX1,																				// Событие входного канала положительный фронт от других каналов ( 1<-2, 2<-3, 3<-4, 4<-1 )
	GTIMER_EventIn_EX2																				// Событие входного канала положительный фронт от других каналов ( 1<-3, 2<-4, 3<-1, 4<-2 )
} GTIMER_EventIn_type;

typedef enum
{
	GTIMER_CH_REF_Format0 = 0,																		// REF = 0
	GTIMER_CH_REF_Format1,																			// REF = 1 если: (CNT == CCR) или ((CNT == CCR1) и CCR1_EN = 1), иначе REF = 0
	GTIMER_CH_REF_Format2,																			// REF = 0 если: (CNT == CCR) или ((CNT == CCR1) и CCR1_EN = 1), иначе REF = 1
	GTIMER_CH_REF_Format3,																			// Переключение REF если: (CNT == CCR) или ((CNT == CCR1) и CCR1_EN = 1)
	GTIMER_CH_REF_Format4,																			// REF = 0
	GTIMER_CH_REF_Format5,																			// REF = 1
	GTIMER_CH_REF_Format6,																			// REF = ~DIR если: (CCR1_EN = 0 и (CNT < CCR)) или (CCR1_EN = 1 и (CCR1 < CNT < CCR)), иначе REF = DIR
	GTIMER_CH_REF_Format7																			// REF = DIR  если: (CCR1_EN = 0 и (CNT < CCR)) или (CCR1_EN = 1 и (CCR1 < CNT < CCR)), иначе REF = ~DIR
} GTIMER_PWM_REF_type;

typedef enum
{
	GTIMER_CH_OutMode_Input = 0,																		// Канал работает на вход
	GTIMER_CH_OutMode_Output,																		// Канал всегда работает на выход
	GTIMER_CH_OutMode_REF_as_OE,																		// Режим канала зависит от сигнала REF
	GTIMER_CH_OutMode_DTG_as_OE																		// Режим канала зависит от сигнала DTG
} GTIMER_CHO_MODE_type;

typedef enum
{
	GTIMER_CH_OutSrc_Only_0 = 0,																		// На выход подается всегда 0
	GTIMER_CH_OutSrc_Only_1,																			// На выход подается всегда 1
	GTIMER_CH_OutSrc_REF,																			// На выход выдается сигнал REF
	GTIMER_CH_OutSrc_DTG																				// На выход выдается сигнал с DTG
} GTIMER_CHO_SOURCE_type;

typedef enum
{
	GTIMER_CHOPolarity_NonInverted = 0,																// Выход не инвертируется
	GTIMER_CHOPolarity_Inverted																		// Выход не инвертируется
} GTIMER_CHO_POLARITY_type;

typedef enum
{
	GTIMER_CH_DTG_ClkSrc_GTIMER_CLK = 0,																// Частота TIM_CLK
	GTIMER_CH_DTG_ClkSrc_FDTS,																		// Частота FDTS
} GTIMER_CH_DTG_CLKSRC_type;

typedef struct
{
	uint32_t 					IniCounter;															// Начальное значение счетчика
	uint16_t 					Prescaler;															// Делитель тактовой частоты
	uint32_t 					Period;																// Основание счета
	GTIMER_CNTRL_CNT_MODE_type 	CounterMode;														// Режим счета
	GTIMER_CNTRL_DIR_type 		CounterDirection;													// Направление счета основного счетчика
	GTIMER_CNTRL_EVENT_SEL_type 	EventSource;														// Источник событий
	GTIMER_CNTRL_FDTS_type 		FilterSampling;														// Частота семплирования данных
	GTIMER_Update_type 			ARR_UpdateMode;														// Разрешение мгновенного обновления ARR
	GTIMER_Filter_type			ETR_FilterConf;														// Цифровой фильтр на входе ETR
	GTIMER_Prescaler_type 		ETR_Prescaler;														// Асинхронный пред делитель внешней  частоты
	GTIMER_Polarity_type 		ETR_Polarity;														// Инверсия ETR
	GTIMER_Polarity_type 		BRK_Polarity;														// Инверсия BRK
} GTIMER_CntInit_type;

typedef struct
{
	GTIMER_CH_type 				ChNumber;															// Номер канала таймера
	GTIMER_ChMode_type 			Mode;																// Режим работы канала
	FunctionalState_type		ETR_Ena;															// Разрешение работы ETR
	GTIMER_ResetCh_type 			ETR_Reset;															// Разрешения сброса по выводу ETR
	GTIMER_ResetCh_type 			BRK_Reset;															// Разрешение сброса по выводу BRK
	GTIMER_PWM_REF_type 			REF_Format;															// Формат выработки сигнала REF в режиме ШИМ
	GTIMER_Prescaler_type 		Prescaler;															// Предварительный делитель входного канала
	GTIMER_EventIn_type 			EventSource;														// Выбор события по входному каналу
	GTIMER_Filter_type 			FilterConf;															// Фильтрация канала
	GTIMER_Update_type 			CCR_UpdateMode;														// Настройка обновления регистров CCR и CCR1
	FunctionalState_type		CCR1_Ena;															// Разрешение работы регистра CCR1
	GTIMER_EventIn_type 			CCR1_EventSource;													// Выбор события по входному каналу для CAP1
} GTIMER_ChnInit_type;

typedef struct
{
	GTIMER_CH_type 				ChNumber;															// Номер канала таймера
	GTIMER_CHO_POLARITY_type 	DirOut_Polarity;													// Инверсия прямого канала
	GTIMER_CHO_SOURCE_type 		DirOut_Source;														// Источник сигнала прямого канала
	GTIMER_CHO_MODE_type 		DirOut_Mode;														// Режим работы прямого канала
	GTIMER_CHO_POLARITY_type		NegOut_Polarity;													// Инверсия инверсного канала
	GTIMER_CHO_SOURCE_type 		NegOut_Source;														// Источник сигнала инверсного канала
	GTIMER_CHO_MODE_type 		NegOut_Mode;														// Режим работы инверсного канала
	uint16_t 					DTG_MainPrescaler;													// Основной делитель частоты ( DTG_delay = MainPrescaler * ( AuxPrescaler + 1 ) )
	uint16_t 					DTG_AuxPrescaler;													// Предварительный делитель частоты
	GTIMER_CH_DTG_CLKSRC_type 	DTG_ClockSource;													// Источник тактирования
} GTIMER_ChnOutInit_type;

/* ---------------------------------------- Флаги событий --------------------------------------- */
#define GTIMER_EVENT_CNT_ZERO                 	( ( ( uint32_t )0x1 ) <<  0 ) 						// Событие совпадения CNT с нулем
#define GTIMER_EVENT_CNT_ARR                  	( ( ( uint32_t )0x1 ) <<  1 ) 						// Событие совпадения CNT с ARR
#define GTIMER_EVENT_ETR_RISING_EDGE          	( ( ( uint32_t )0x1 ) <<  2 ) 						// Событие переднего фронта на входе ETR
#define GTIMER_EVENT_ETR_FALLING_EDGE         	( ( ( uint32_t )0x1 ) <<  3 ) 						// Событие заднего фронта на входе ETR
#define GTIMER_EVENT_BRK                      	( ( ( uint32_t )0x1 ) <<  4 ) 						// Триггерированное по PCLK состояние входа BRK
#define GTIMER_EVENT_CCR_CAP_CH1              	( ( ( uint32_t )0x1 ) <<  5 ) 						// Событие настроенного фронта на входе CHxi каналов таймера
#define GTIMER_EVENT_CCR_CAP_CH2              	( ( ( uint32_t )0x1 ) <<  6 ) 						//
#define GTIMER_EVENT_CCR_CAP_CH3              	( ( ( uint32_t )0x1 ) <<  7 )						//
#define GTIMER_EVENT_CCR_CAP_CH4              	( ( ( uint32_t )0x1 ) <<  8 ) 						//
#define GTIMER_EVENT_CCR_REF_CH1              	( ( ( uint32_t )0x1 ) <<  9 ) 						// Событие переднего фронта на выходе REF каналов таймера
#define GTIMER_EVENT_CCR_REF_CH2              	( ( ( uint32_t )0x1 ) << 10 ) 						//
#define GTIMER_EVENT_CCR_REF_CH3              	( ( ( uint32_t )0x1 ) << 11 ) 						//
#define GTIMER_EVENT_CCR_REF_CH4              	( ( ( uint32_t )0x1 ) << 12 ) 						//
#define GTIMER_EVENT_CCR_CAP1_CH1             	( ( ( uint32_t )0x1 ) << 13 ) 						// Событие настроенного фронта на входе CHxi каналов таймера
#define GTIMER_EVENT_CCR_CAP1_CH2             	( ( ( uint32_t )0x1 ) << 14 ) 						//
#define GTIMER_EVENT_CCR_CAP1_CH3             	( ( ( uint32_t )0x1 ) << 15 ) 						//
#define GTIMER_EVENT_CCR_CAP1_CH4             	( ( ( uint32_t )0x1 ) << 16 ) 						//

#ifdef __cplusplus
extern "C"
{
#endif
	void 					HAL_GTIMER_DeInit					( GTIMER_type* GTIMERx );
	void 					HAL_GTIMER_CntInit					( GTIMER_type* GTIMERx, const GTIMER_CntInit_type* GTIMER_CntInitStruct );
	void 					HAL_GTIMER_CntStructInit			( GTIMER_CntInit_type* GTIMER_CntInitStruct );
	void 					HAL_GTIMER_Cmd						( GTIMER_type* TIMERx, FunctionalState_type NewState );
	void 					HAL_GTIMER_SetCounter				( GTIMER_type* GTIMERx, uint32_t Counter );
	void 					HAL_GTIMER_SetCntPrescaler			( GTIMER_type* GTIMERx, uint16_t Prescaler );
	void 					HAL_GTIMER_CntAutoreloadConfig		( GTIMER_type* GTIMERx, uint32_t Autoreload, GTIMER_Update_type UpdateMode );
	uint32_t 				HAL_GTIMER_GetCounter				( GTIMER_type* GTIMERx );
	GTIMER_CNTRL_DIR_type 	HAL_GTIMER_GetCounterDirection		( GTIMER_type* GTIMERx );
	State_type 				HAL_GTIMER_GetCntWriteComplete		( GTIMER_type* GTIMERx );

	void 					HAL_GTIMER_ETRInputConfig			( GTIMER_type* GTIMERx, GTIMER_Prescaler_type Prescaler, GTIMER_Polarity_type Polarity, GTIMER_Filter_type Filter );
	void 					HAL_GTIMER_BRKPolarityConfig		( GTIMER_type* GTIMERx, GTIMER_Polarity_type Polarity );

	void 					HAL_GTIMER_ChnInit					( GTIMER_type* GTIMERx, const GTIMER_ChnInit_type* GTIMER_ChnInitStruct );
	void					HAL_GTIMER_ChnStructInit			( GTIMER_ChnInit_type* GTIMER_ChnInitStruct );
	void 					HAL_GTIMER_ChnCompareSet			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare );
	void 					HAL_GTIMER_ChnCompareConfig			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare, GTIMER_Update_type UpdateMode );
	void 					HAL_GTIMER_ChnCompare1Set			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare );
	void 					HAL_GTIMER_ChnCompare1Config		( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare, GTIMER_Update_type UpdateMode );
	uint32_t 				HAL_GTIMER_GetChnCapture			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel );
	uint32_t 				HAL_GTIMER_GetChnCapture1			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel );

	void 					HAL_GTIMER_ChnETR_Cmd				( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState );
	void 					HAL_GTIMER_ChnETRResetConfig		( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState );
	void 					HAL_GTIMER_ChnBRKResetConfig		( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState );
	void 					HAL_GTIMER_ChnREFFormatConfig		( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_PWM_REF_type Format );
	void 					HAL_GTIMER_ChnCapturePrescalerConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_Prescaler_type Prescaler );
	void 					HAL_GTIMER_ChnEventSourceConfig		( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_EventIn_type EventSource );
	void 					HAL_GTIMER_ChnFilterConfig			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_Filter_type Filter );
	State_type				HAL_GTIMER_GetChnWriteComplete		( GTIMER_type* GTIMERx, GTIMER_CH_type Channel );
	void 					HAL_GTIMER_ChnCCR1_EventSourceConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_EventIn_type EventSource );
	void 					HAL_GTIMER_ChnCCR1_Cmd				( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState );

	void 					HAL_GTIMER_ChnOutInit				( GTIMER_type* GTIMERx, const GTIMER_ChnOutInit_type* GTIMER_ChnOutInitStruct );
	void 					HAL_GTIMER_ChnOutStructInit			( GTIMER_ChnOutInit_type* GTIMER_ChnOutInitStruct );
	void 					HAL_GTIMER_ChnOutConfig				( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_CHO_SOURCE_type OutSource, GTIMER_CHO_MODE_type Mode, GTIMER_CHO_POLARITY_type Polarity );
	void 					HAL_GTIMER_ChnNOutConfig			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_CHO_SOURCE_type OutSource, GTIMER_CHO_MODE_type Mode, GTIMER_CHO_POLARITY_type Polarity );
	void 					HAL_GTIMER_ChnOutDTGConfig			( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint16_t MainPrescaler, uint16_t AuxPrescaler, GTIMER_CH_DTG_CLKSRC_type ClockSource );

	uint32_t 				HAL_GTIMER_GetStatus				( GTIMER_type* GTIMERx );
	State_type 				HAL_GTIMER_GetFlagStatus			( GTIMER_type* GTIMERx, uint32_t Flag );
	void 					HAL_GTIMER_ClearFlag				( GTIMER_type* GTIMERx, uint32_t Flags );

	void 					HAL_GTIMER_DMACmd					( GTIMER_type* GTIMERx, uint32_t GTIMER_DMASource, GTIMER_DMA_REQ_type GTIMER_DMA_Request, FunctionalState_type NewState );
	void 					HAL_GTIMER_ITConfig					( GTIMER_type* GTIMERx, uint32_t GTIMER_IT, FunctionalState_type NewState );
	State_type 				HAL_GTIMER_GetITStatus				( GTIMER_type* GTIMERx, uint32_t GTIMER_IT );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_GTIMER_H__ */
