/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_timer.h
 *	Описание: 	HAL для таймеров общего назначения
 *
 *	История:
 *	 			07-Mar-2017 Ivan Osadchy 	- 	создан
 */

#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef volatile struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t res0 	:4;
			uint32_t ena	:2;
			uint32_t res1 	:26;
		} All;
		struct
		{
			uint32_t res0 	:4;
			uint32_t ena 	:1;
			uint32_t res1 	:27;
		} T0;
		struct
		{
			uint32_t res0 	:5;
			uint32_t ena 	:1;
			uint32_t res1 	:26;
		} T1;
	} CTRL;
	uint64_t Timer_curr[ 2 ];
	uint64_t Timer_count[ 2 ];
} TIMER_type;

/* ------------------------------------ Структура периферии ------------------------------------- */
#define 	LX_TIMER  			( ( volatile TIMER_type * ) base_TIMER )

/* -------------------------------------- Специфические типы ------------------------------------ */
// Типы таймеров:
typedef enum
{
	Timer_0 = 0, Timer_1 = 1,
} Timer_X_type;

// Состояние таймеров:
typedef enum
{
	Timer_State_Disable = 0, Timer_State_Enable = 1,
} Timer_State_type;

#ifdef __cplusplus
  extern "C"
	{
	#endif

  	 	 void 				HAL_Timer_Init 			( Timer_X_type Timer, uint64_t Count, Timer_State_type State );
  	 	 void 				HAL_Timer_DeInit 		( Timer_X_type Timer );

  	 	 uint64_t 			HAL_Timer_GetCurrent 	( Timer_X_type Timer );

  	 	 void 				HAL_Timer_SetCount		( Timer_X_type Timer, uint64_t Count );
  	 	 void 				HAL_Timer_SetCountNow 	( Timer_X_type Timer, uint64_t Count );
  	 	 uint64_t 			HAL_Timer_GetCount		( Timer_X_type Timer);
  	 	 void 				HAL_Timer_SetState		( Timer_X_type Timer, Timer_State_type State );
  	 	 Timer_State_type 	HAL_Timer_GetState 		( Timer_X_type Timer );

  	 	 void 				HAL_Timers_SetState 	( Timer_State_type Timer0, Timer_State_type Timer1 );

	#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_TIMER_H__ */
