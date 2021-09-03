/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	File/Файл: 		hal_timer.c
 *	Description/Описание: 	HAL for Timers/HAL для таймеров общего назначения
 *
 *	History/История:
 *	 			07-Mar-2017 Ivan Osadchy 	- 	created/создан
 */
#include "hal_1967VN044.h"

// Timer initialization/Инициализация таймера:
void HAL_Timer_Init( Timer_X_type Timer, uint64_t Count, Timer_State_type State )
{
	// Arguments checking/Проверки аргументов:
	if ( Timer > Timer_1 ) return;
	if ( State > Timer_State_Enable ) return;

	HAL_Timer_SetCount( Timer, Count );
	HAL_Timer_SetState( Timer, State );
}

// Timer deinitialization/Деинициализация таймера:
void HAL_Timer_DeInit( Timer_X_type Timer )
{
	HAL_Timer_Init( Timer, 0, Timer_State_Disable );
}

// Get timer current value/Получить текущее значение таймера:
uint64_t HAL_Timer_GetCurrent( Timer_X_type Timer )
{
	volatile uint32_t *pReg = ( volatile uint32_t* )(& (LX_TIMER->Timer_curr[ Timer ] ) );
	uint64_t Curr;
	uint32_t *pCurr =  ( uint32_t* )(& ( Curr ) );

	*pCurr++ = *pReg++;
	*pCurr = *pReg;

	return Curr;
}

// Set the value for the timer counting in the following cycle/Установка значения для счета таймера в следущем цикле:
void HAL_Timer_SetCount( Timer_X_type Timer, uint64_t Count )
{
	uint32_t *pCount = ( uint32_t* )(& ( Count ) );
	volatile uint32_t *pTimer = ( volatile uint32_t* )(& ( LX_TIMER->Timer_count[ ( uint8_t ) Timer ] ) );

	*pTimer++ = *pCount++;
	*pTimer = *pCount;
}

// Immediate changing of the timer value/Немедленное изменение значения таймера:
void HAL_Timer_SetCountNow( Timer_X_type Timer, uint64_t Count )
{
	Timer_State_type State = HAL_Timer_GetState( Timer );
	HAL_Timer_SetState( Timer, Timer_State_Disable );
	HAL_Timer_SetCount( Timer, Count );
	HAL_Timer_SetState( Timer, State );
}

// Get the value of timer count/Получение значения счета таймера:
uint64_t HAL_Timer_GetCount( Timer_X_type Timer )
{
	volatile uint32_t *pReg = ( volatile uint32_t* )(& ( LX_TIMER->Timer_count[ Timer ] ) );
	uint64_t Count;
	uint32_t *pCount = ( uint32_t* )(& ( Count ) );

	*pCount++ = *pReg++;
	*pCount = *pReg;

	return Count;
}

// Timer enabling/disabling/Включение/выключенние таймера:
void HAL_Timer_SetState( Timer_X_type Timer, Timer_State_type State )
{
	if ( Timer == Timer_0 ) LX_TIMER->CTRL.T0.ena = State;
	if ( Timer == Timer_1 ) LX_TIMER->CTRL.T1.ena = State;
}

// Timer current state(on/off)/Текущее состояние таймера (включен/выключен):
Timer_State_type HAL_Timer_GetState( Timer_X_type Timer )
{
	return ( ( Timer_State_type ) (
			( Timer == Timer_0 ) ? LX_TIMER->CTRL.T0.ena : LX_TIMER->CTRL.T1.ena ) );
}

// Enabling/disabling of two timers at a time/Включение/выключение двух таймеров сразу:
void HAL_Timers_SetState( Timer_State_type Timer0, Timer_State_type Timer1 )
{
	uint32_t State = ( ( ( uint32_t ) Timer1 ) << 1 ) | ( ( uint32_t ) Timer0 );
	LX_TIMER->CTRL.All.ena = State;
}
