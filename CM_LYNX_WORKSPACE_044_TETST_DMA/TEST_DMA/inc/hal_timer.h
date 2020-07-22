/*
 *
 *	Файл: 		hal_timer.h
 *	Описание: 	HAL для модуля таймеров с функцией захвата/ШИМ
 *
 *	История:
 *	 			20-Jun-2017 Dmitriy Sharaviev 	- 	создан
 *
 */

#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

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
} TIMER_CNTRL_type;

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
} TIMER_CHX_CNTRL_type;

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
} TIMER_CHX_CNTRL1_type;

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
} TIMER_CHX_CNTRL2_type;

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
} TIMER_CHX_DTG_type;

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
} TIMER_BRKETR_CNTRL_type;

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
} TIMER_STATUS_type;

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
} TIMER_IE_type;

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
} TIMER_DMA_RE_type;
/* ------------------------------------ Структура периферии ------------------------------------- */

typedef volatile struct
{
  uint32_t CNT;																						// Основной счетчик таймера CNT
  uint32_t PSG;																						// Делитель частоты при счете основного счетчика PSG
  uint32_t ARR;																						// Основание счета основного счетчика ARR
  TIMER_CNTRL_type CNTRL;																			// Регистр управления основного счетчика CNTRL
  uint32_t CCRx[4];																					// Регистры сравнения/захвата для X канала таймера CCRX
  TIMER_CHX_CNTRL_type CHx_CNTRL[4];																// Регистры управления для X канала таймера CHX_CNTRL
  TIMER_CHX_CNTRL1_type CHx_CNTRL1[4];																// Регистры управления 1 для X канала таймера CHX_CNTRL1
  TIMER_CHX_DTG_type CHx_DTG[4];																	// Регистры CHX_DTG управления DTG
  TIMER_BRKETR_CNTRL_type BRKETR_CNTRL;																// Регистр BRKETR_CNTRL управления входом BRK и ETR
  TIMER_STATUS_type STATUS;																			// Регистр статуса таймера STATUS
  TIMER_IE_type IE;																					// Регистр разрешения прерывания таймера IE
  TIMER_DMA_RE_type DMA_RE;																			// Регистр DMA_RE разрешения запросов DMA от прерываний таймера
  TIMER_CHX_CNTRL2_type CHx_CNTRL2[4];																// Регистрs управления 2 для X канала таймера CHX_CNTRL2
  uint32_t CCRx1[4];																				// Регистрs сравнения/захвата для X канала таймера CCRX1
  uint32_t DMA_REChx[4];
}TIMER_type;

#define LX_TIMER0 		( ( volatile TIMER_type * ) base_TIMER0 )
#define LX_TIMER1 		( ( volatile TIMER_type * ) base_TIMER1 )

/* -------------------------------------- Специфические типы ------------------------------------ */

typedef enum
{
	TIMER_CH1 = 0,
	TIMER_CH2,
	TIMER_CH3,
	TIMER_CH4
}TIMER_CH_type;

typedef enum
{
	TIMER_DMA_CH1 = 0,
	TIMER_DMA_CH2,
	TIMER_DMA_CH3,
	TIMER_DMA_CH4,
	TIMER_DMA_ALL_CH
}TIMER_DMA_CH_type;

typedef enum
{
	TIMER_Update_Immediately = 0,																	// Обновление возможно в любой момент времени
	TIMER_Update_On_CNT_Overflow,																	// Обновление будет осуществлено только при CNT = 0
}TIMER_Update_type;

typedef enum
{
	TIMER_CntDir_Up = 0,																			// Счет вверх
	TIMER_CntDir_Down,																				// Счет вниз
}TIMER_CNTRL_DIR_type;

typedef enum
{
	TIMER_FdtsDiv_1 = 0,																			// Частота семплирования каждый TIM_CLK
	TIMER_FdtsDiv_2,																				// Частота семплирования каждый второй TIM_CLK
	TIMER_FdtsDiv_3,																				// Частота семплирования каждый третий TIM_CLK
	TIMER_FdtsDiv_4,																				// Частота семплирования каждый четвертый TIM_CLK
}TIMER_CNTRL_FDTS_type;

typedef enum
{
	TIMER_CntMode_ClkFixedDir = 0,																	// Счет без изменения направления от внутреннего тактового сигнала
	TIMER_CntMode_ClkChangeDir,																		// Счет с изменением направления от внутреннего тактового сигнала
	TIMER_CntMode_EvtFixedDir,																		// Счет без изменения направления от событий
	TIMER_CntMode_EvtChangeDir,																		// Счет с изменением направления от событий
}TIMER_CNTRL_CNT_MODE_type;

typedef enum
{
	TIMER_EvSrc_None = 0,																			// Нет событий
	TIMER_EvSrc_TM1,																				// CNT == ARR в таймере 1
	TIMER_EvSrc_TM2,																				// CNT == ARR в таймере 2
	TIMER_EvSrc_CH1 = 4,																			// Событие на первом канале
	TIMER_EvSrc_CH2,																				// Событие на втором канале
	TIMER_EvSrc_CH3,																				// Событие на третьем канале
	TIMER_EvSrc_CH4,																				// Событие на четвертом канале
	TIMER_EvSrc_ETR_RE,																				// Событие переднего фронта ETR
	TIMER_EvSrc_ETR_FE,																				// Событие заднего фронта ETR
}TIMER_CNTRL_EVENT_SEL_type;

typedef enum
{
	TIMER_Filter_1FF_at_TIMER_CLK = 0,    															// Сигнал зафиксирован в 1 триггере на частоте TIM_CLK
	TIMER_Filter_2FF_at_TIMER_CLK,																	// Сигнал зафиксирован в 2 триггерах на частоте TIM_CLK
	TIMER_Filter_4FF_at_TIMER_CLK,  																// Сигнал зафиксирован в 4 триггерах на частоте TIM_CLK
	TIMER_Filter_8FF_at_TIMER_CLK,  																// Сигнал зафиксирован в 8 триггерах на частоте TIM_CLK
	TIMER_Filter_6FF_at_FDTS_div_2,  																// Сигнал зафиксирован в 6 триггерах на частоте FDTS/2
	TIMER_Filter_8FF_at_FDTS_div_2,  																// Сигнал зафиксирован в 8 триггерах на частоте FDTS/2
	TIMER_Filter_6FF_at_FDTS_div_4,        					  										// Сигнал зафиксирован в 6 триггерах на частоте FDTS/4
	TIMER_Filter_8FF_at_FDTS_div_4,        															// Сигнал зафиксирован в 8 триггерах на частоте FDTS/4
	TIMER_Filter_6FF_at_FDTS_div_8,        					  										// Сигнал зафиксирован в 6 триггерах на частоте FDTS/8
	TIMER_Filter_8FF_at_FDTS_div_8,        															// Сигнал зафиксирован в 8 триггерах на частоте FDTS/8
	TIMER_Filter_5FF_at_FDTS_div_16,       															// Сигнал зафиксирован в 5 триггерах на частоте FDTS/16
	TIMER_Filter_6FF_at_FDTS_div_16,       				  											// Сигнал зафиксирован в 6 триггерах на частоте FDTS/16
	TIMER_Filter_8FF_at_FDTS_div_16,      					 										// Сигнал зафиксирован в 8 триггерах на частоте FDTS/16
	TIMER_Filter_5FF_at_FDTS_div_32,       					 										// Сигнал зафиксирован в 5 триггерах на частоте FDTS/32
	TIMER_Filter_6FF_at_FDTS_div_32,      					  										// Сигнал зафиксирован в 6 триггерах на частоте FDTS/32
	TIMER_Filter_8FF_at_FDTS_div_32       					  										// Сигнал зафиксирован в 8 триггерах на частоте FDTS/32
}TIMER_Filter_type;

typedef enum
{
	TIMER_Prescaler_None = 0,																		// нет деления
	TIMER_Prescaler_2,																				// /2
	TIMER_Prescaler_4,																				// /4
	TIMER_Prescaler_8																				// /8
}TIMER_Prescaler_type;

typedef enum
{
	TIMER_Polarity_NonInverted = 0,																	// Нет инверсии входа
	TIMER_Polarity_Inverted																			// Инверсия входа
}TIMER_Polarity_type;

typedef enum
{
	TIMER_CH_MODE_PWM = 0,																			// Канал работает в режиме ШИМ
	TIMER_CH_MODE_CAPTURE  																			// Канал работает в режиме Захват
}TIMER_ChMode_type;

typedef enum
{
	TIMER_CH_RESET_DISABLE = 0,																		// Сброс по внешнему выводу запрещен
	TIMER_CH_RESET_ENABLE  																			// Сброс по внешнему выводу разрешен
}TIMER_ResetCh_type;

typedef enum
{
	TIMER_EventIn_PE = 0,																			// Событие входного канала положительный фронт
	TIMER_EventIn_NE,																				// Событие входного канала отрицательный фронт
	TIMER_EventIn_EX1,																				// Событие входного канала положительный фронт от других каналов ( 1<-2, 2<-3, 3<-4, 4<-1 )
	TIMER_EventIn_EX2																				// Событие входного канала положительный фронт от других каналов ( 1<-3, 2<-4, 3<-1, 4<-2 )
}TIMER_EventIn_type;

typedef enum
{
	TIMER_CH_REF_Format0 = 0,																		// REF = 0
	TIMER_CH_REF_Format1,																			// REF = 1 если: (CNT == CCR) или ((CNT == CCR1) и CCR1_EN = 1), иначе REF = 0
	TIMER_CH_REF_Format2,																			// REF = 0 если: (CNT == CCR) или ((CNT == CCR1) и CCR1_EN = 1), иначе REF = 1
	TIMER_CH_REF_Format3,																			// Переключение REF если: (CNT == CCR) или ((CNT == CCR1) и CCR1_EN = 1)
	TIMER_CH_REF_Format4,																			// REF = 0
	TIMER_CH_REF_Format5,																			// REF = 1
	TIMER_CH_REF_Format6,																			// REF = ~DIR если: (CCR1_EN = 0 и (CNT < CCR)) или (CCR1_EN = 1 и (CCR1 < CNT < CCR)), иначе REF = DIR
	TIMER_CH_REF_Format7																			// REF = DIR  если: (CCR1_EN = 0 и (CNT < CCR)) или (CCR1_EN = 1 и (CCR1 < CNT < CCR)), иначе REF = ~DIR
}TIMER_PWM_REF_type;

typedef enum
{
	TIMER_CH_OutMode_Input = 0,																		// Канал работает на вход
	TIMER_CH_OutMode_Output,																		// Канал всегда работает на выход
	TIMER_CH_OutMode_REF_as_OE,																		// Режим канала зависит от сигнала REF
	TIMER_CH_OutMode_DTG_as_OE																		// Режим канала зависит от сигнала DTG
}TIMER_CHO_MODE_type;

typedef enum
{
	TIMER_CH_OutSrc_Only_0 = 0,																		// На выход подается всегда 0
	TIMER_CH_OutSrc_Only_1,																			// На выход подается всегда 1
	TIMER_CH_OutSrc_REF,																			// На выход выдается сигнал REF
	TIMER_CH_OutSrc_DTG																				// На выход выдается сигнал с DTG
}TIMER_CHO_SOURCE_type;

typedef enum
{
	TIMER_CHOPolarity_NonInverted = 0,																// Выход не инвертируется
	TIMER_CHOPolarity_Inverted																		// Выход не инвертируется
}TIMER_CHO_POLARITY_type;

typedef enum
{
	TIMER_CH_DTG_ClkSrc_TIMER_CLK = 0,																// Частота TIM_CLK
	TIMER_CH_DTG_ClkSrc_FDTS,																		// Частота FDTS
}TIMER_CH_DTG_CLKSRC_type;

typedef struct
{
	uint32_t 					IniCounter;															// Начальное значение счетчика
	uint16_t 					Prescaler;															// Делитель тактовой частоты
	uint32_t 					Period;																// Основание счета
	TIMER_CNTRL_CNT_MODE_type 	CounterMode;														// Режим счета
	TIMER_CNTRL_DIR_type 		CounterDirection;													// Направление счета основного счетчика
	TIMER_CNTRL_EVENT_SEL_type 	EventSource;														// Источник событий
	TIMER_CNTRL_FDTS_type 		FilterSampling;														// Частота семплирования данных
	TIMER_Update_type 			ARR_UpdateMode;														// Разрешение мгновенного обновления ARR
	TIMER_Filter_type			ETR_FilterConf;														// Цифровой фильтр на входе ETR
	TIMER_Prescaler_type 		ETR_Prescaler;														// Асинхронный пред делитель внешней  частоты
	TIMER_Polarity_type 		ETR_Polarity;														// Инверсия ETR
	TIMER_Polarity_type 		BRK_Polarity;														// Инверсия BRK
}TIMER_CntInit_type;

typedef struct
{
	TIMER_CH_type 				ChNumber;															// Номер канала таймера
	TIMER_ChMode_type 			Mode;																// Режим работы канала
	FunctionalState_type		ETR_Ena;															// Разрешение работы ETR
	TIMER_ResetCh_type 			ETR_Reset;															// Разрешения сброса по выводу ETR
	TIMER_ResetCh_type 			BRK_Reset;															// Разрешение сброса по выводу BRK
	TIMER_PWM_REF_type 			REF_Format;															// Формат выработки сигнала REF в режиме ШИМ
	TIMER_Prescaler_type 		Prescaler;															// Предварительный делитель входного канала
	TIMER_EventIn_type 			EventSource;														// Выбор события по входному каналу
	TIMER_Filter_type 			FilterConf;															// Фильтрация канала
	TIMER_Update_type 			CCR_UpdateMode;														// Настройка обновления регистров CCR и CCR1
	FunctionalState_type		CCR1_Ena;															// Разрешение работы регистра CCR1
	TIMER_EventIn_type 			CCR1_EventSource;													// Выбор события по входному каналу для CAP1
} TIMER_ChnInit_type;

typedef struct
{
	TIMER_CH_type 				ChNumber;															// Номер канала таймера
	TIMER_CHO_POLARITY_type 	DirOut_Polarity;													// Инверсия прямого канала
	TIMER_CHO_SOURCE_type 		DirOut_Source;														// Источник сигнала прямого канала
	TIMER_CHO_MODE_type 		DirOut_Mode;														// Режим работы прямого канала
	TIMER_CHO_POLARITY_type		NegOut_Polarity;													// Инверсия инверсного канала
	TIMER_CHO_SOURCE_type 		NegOut_Source;														// Источник сигнала инверсного канала
	TIMER_CHO_MODE_type 		NegOut_Mode;														// Режим работы инверсного канала
	uint16_t 					DTG_MainPrescaler;													// Основной делитель частоты ( DTG_delay = MainPrescaler * ( AuxPrescaler + 1 ) )
	uint16_t 					DTG_AuxPrescaler;													// Предварительный делитель частоты
	TIMER_CH_DTG_CLKSRC_type 	DTG_ClockSource;													// Источник тактирования
} TIMER_ChnOutInit_type;

/* ---------------------------------------- Флаги событий --------------------------------------- */
#define TIMER_EVENT_CNT_ZERO                 	( ( ( uint32_t )0x1 ) <<  0 ) 						// Событие совпадения CNT с нулем
#define TIMER_EVENT_CNT_ARR                  	( ( ( uint32_t )0x1 ) <<  1 ) 						// Событие совпадения CNT с ARR
#define TIMER_EVENT_ETR_RISING_EDGE          	( ( ( uint32_t )0x1 ) <<  2 ) 						// Событие переднего фронта на входе ETR
#define TIMER_EVENT_ETR_FALLING_EDGE         	( ( ( uint32_t )0x1 ) <<  3 ) 						// Событие заднего фронта на входе ETR
#define TIMER_EVENT_BRK                      	( ( ( uint32_t )0x1 ) <<  4 ) 						// Триггерированное по PCLK состояние входа BRK
#define TIMER_EVENT_CCR_CAP_CH1              	( ( ( uint32_t )0x1 ) <<  5 ) 						// Событие переднего фронта на входе CAP каналов таймера
#define TIMER_EVENT_CCR_CAP_CH2              	( ( ( uint32_t )0x1 ) <<  6 ) 						//
#define TIMER_EVENT_CCR_CAP_CH3              	( ( ( uint32_t )0x1 ) <<  7 )						//
#define TIMER_EVENT_CCR_CAP_CH4              	( ( ( uint32_t )0x1 ) <<  8 ) 						//
#define TIMER_EVENT_CCR_REF_CH1              	( ( ( uint32_t )0x1 ) <<  9 ) 						// Событие переднего фронта на выходе REF каналов таймера
#define TIMER_EVENT_CCR_REF_CH2              	( ( ( uint32_t )0x1 ) << 10 ) 						//
#define TIMER_EVENT_CCR_REF_CH3              	( ( ( uint32_t )0x1 ) << 11 ) 						//
#define TIMER_EVENT_CCR_REF_CH4              	( ( ( uint32_t )0x1 ) << 12 ) 						//
#define TIMER_EVENT_CCR_CAP1_CH1             	( ( ( uint32_t )0x1 ) << 13 ) 						// Событие переднего фронта на входе CAP1 каналов таймера
#define TIMER_EVENT_CCR_CAP1_CH2             	( ( ( uint32_t )0x1 ) << 14 ) 						//
#define TIMER_EVENT_CCR_CAP1_CH3             	( ( ( uint32_t )0x1 ) << 15 ) 						//
#define TIMER_EVENT_CCR_CAP1_CH4             	( ( ( uint32_t )0x1 ) << 16 ) 						//

#ifdef __cplusplus
extern "C"
{
#endif
	void 					HAL_TIMER_DeInit					( TIMER_type* TIMERx );
	void 					HAL_TIMER_CntInit					( TIMER_type* TIMERx, const TIMER_CntInit_type* TIMER_CntInitStruct );
	void 					HAL_TIMER_CntStructInit				( TIMER_CntInit_type* TIMER_CntInitStruct );
	void 					HAL_TIMER_Cmd						( TIMER_type* TIMERx, FunctionalState_type NewState );
	void 					HAL_TIMER_SetCounter				( TIMER_type* TIMERx, uint32_t Counter );
	void 					HAL_TIMER_SetCntPrescaler			( TIMER_type* TIMERx, uint16_t Prescaler );
	void 					HAL_TIMER_CntAutoreloadConfig		( TIMER_type* TIMERx, uint32_t Autoreload, TIMER_Update_type UpdateMode );
	uint32_t 				HAL_TIMER_GetCounter				( TIMER_type* TIMERx );
	TIMER_CNTRL_DIR_type 	HAL_TIMER_GetCounterDirection		( TIMER_type* TIMERx );
	State_type 				HAL_TIMER_GetCntWriteComplete		( TIMER_type* TIMERx );

	void 					HAL_TIMER_ETRInputConfig			( TIMER_type* TIMERx, TIMER_Prescaler_type Prescaler, TIMER_Polarity_type Polarity, TIMER_Filter_type Filter );
	void 					HAL_TIMER_BRKPolarityConfig			( TIMER_type* TIMERx, TIMER_Polarity_type Polarity );

	void 					HAL_TIMER_ChnInit					( TIMER_type* TIMERx, const TIMER_ChnInit_type* TIMER_ChnInitStruct );
	void					HAL_TIMER_ChnStructInit				( TIMER_ChnInit_type* TIMER_ChnInitStruct );
	void 					HAL_TIMER_ChnCompareSet				( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare );
	void 					HAL_TIMER_ChnCompareConfig			( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare, TIMER_Update_type UpdateMode );
	void 					HAL_TIMER_ChnCompare1Set			( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare );
	void 					HAL_TIMER_ChnCompare1Config			( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare, TIMER_Update_type UpdateMode );
	uint32_t 				HAL_TIMER_GetChnCapture				( TIMER_type* TIMERx, TIMER_CH_type Channel );
	uint32_t 				HAL_TIMER_GetChnCapture1			( TIMER_type* TIMERx, TIMER_CH_type Channel );

	void 					HAL_TIMER_ChnETR_Cmd				( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState );
	void 					HAL_TIMER_ChnETRResetConfig			( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState );
	void 					HAL_TIMER_ChnBRKResetConfig			( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState );
	void 					HAL_TIMER_ChnREFFormatConfig		( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_PWM_REF_type Format );
	void 					HAL_TIMER_ChnCapturePrescalerConfig	( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_Prescaler_type Prescaler );
	void 					HAL_TIMER_ChnEventSourceConfig		( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_EventIn_type EventSource );
	void 					HAL_TIMER_ChnFilterConfig			( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_Filter_type Filter );
	State_type				HAL_TIMER_GetChnWriteComplete		( TIMER_type* TIMERx, TIMER_CH_type Channel );
	void 					HAL_TIMER_ChnCCR1_EventSourceConfig	( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_EventIn_type EventSource );
	void 					HAL_TIMER_ChnCCR1_Cmd				( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState );

	void 					HAL_TIMER_ChnOutInit				( TIMER_type* TIMERx, const TIMER_ChnOutInit_type* TIMER_ChnOutInitStruct );
	void 					HAL_TIMER_ChnOutStructInit			( TIMER_ChnOutInit_type* TIMER_ChnOutInitStruct );
	void 					HAL_TIMER_ChnOutConfig				( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_CHO_SOURCE_type OutSource, TIMER_CHO_MODE_type Mode, TIMER_CHO_POLARITY_type Polarity );
	void 					HAL_TIMER_ChnNOutConfig				( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_CHO_SOURCE_type OutSource, TIMER_CHO_MODE_type Mode, TIMER_CHO_POLARITY_type Polarity );
	void 					HAL_TIMER_ChnOutDTGConfig			( TIMER_type* TIMERx, TIMER_CH_type Channel, uint16_t MainPrescaler, uint16_t AuxPrescaler, TIMER_CH_DTG_CLKSRC_type ClockSource );

	uint32_t 				HAL_TIMER_GetStatus					( TIMER_type* TIMERx );
	State_type 				HAL_TIMER_GetFlagStatus				( TIMER_type* TIMERx, uint32_t Flag );
	void 					HAL_TIMER_ClearFlag					( TIMER_type* TIMERx, uint32_t Flags );

	void 					HAL_TIMER_DMACmd					( TIMER_type* TIMERx, uint32_t TIMER_DMASource, TIMER_DMA_CH_type TIMER_DMA_Channel, FunctionalState_type NewState );
	void 					HAL_TIMER_ITConfig					( TIMER_type* TIMERx, uint32_t TIMER_IT, FunctionalState_type NewState );
	State_type 				HAL_TIMER_GetITStatus				( TIMER_type* TIMERx, uint32_t TIMER_IT );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_TIMER_H__ */
