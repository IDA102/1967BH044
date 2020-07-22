/*
 *
 *	Файл: 		hal_gptimer.h
 *	Описание: 	HAL для таймеров общего назначения
 *
 *	История:
 *	 			07-Mar-2017 Ivan Osadchy 	- 	создан
 */

#ifndef __HAL_GPTIMER_H__
#define __HAL_GPTIMER_H__

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
} GPTIMER_type;

/* ------------------------------------ Структура периферии ------------------------------------- */
#define 	LX_GPTIMER  			( ( volatile GPTIMER_type * ) base_GPTIMER )

/* -------------------------------------- Специфические типы ------------------------------------ */
// Типы таймеров:
typedef enum
{
	GPTimer_0 = 0, GPTimer_1 = 1,
} GPTimer_X_type;

// Состояние таймеров:
typedef enum
{
	GPTimer_State_Disable = 0, GPTimer_State_Enable = 1,
} GPTimer_State_type;

#ifdef __cplusplus
  extern "C"
	{
	#endif

  	 	 void 				HAL_GPTimer_Init 			( GPTimer_X_type Timer, uint64_t Count, GPTimer_State_type State );
  	 	 void 				HAL_GPTimer_DeInit 			( GPTimer_X_type Timer );

  	 	 uint64_t 			HAL_GPTimer_GetCurrent 		( GPTimer_X_type Timer );

  	 	 void 				HAL_GPTimer_SetCount		( GPTimer_X_type Timer, uint64_t Count );
  	 	 void 				HAL_GPTimer_SetCountNow 	( GPTimer_X_type Timer, uint64_t Count );
  	 	 uint64_t 			HAL_GPTimer_GetCount		( GPTimer_X_type Timer);
  	 	 void 				HAL_GPTimer_SetState		( GPTimer_X_type Timer, GPTimer_State_type State );
  	 	GPTimer_State_type 	HAL_GPTimer_GetState 		( GPTimer_X_type Timer );

  	 	 void 				HAL_GPTimers_SetState 		( GPTimer_State_type Timer0, GPTimer_State_type Timer1 );

	#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_GPTIMER_H__ */
