/*
 *
 *	file/Файл: 		hal_rtc.c
 *	Description/Описание: 	HAL for RTC/HAL для модуля часов реального времени
 *
 *	History/История:
 *				13-Dec-2016	Zatonsky Pavel 		- 	created/создан
 *
 */
#include "hal_1967VN044.h"
#include <math.h>

uint32_t HAL_RTC_RegRead( RTC_Reg_type RTCReg )
{
	volatile uint32_t tmp = 0;
	asm volatile("j8 = [%0 + 0x800001E0];;\n\t"\
			"j8 = [%0 + 0x800001E0];;\n\t"\
			"[%1+0] = j8;;"::"j"(RTCReg),"j"(&tmp));
	while( LX_RTC->RTC_BUSY == 1 );
	return tmp;
}

void HAL_RTC_Busy( void )
{
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_InitTicPeriod( uint32_t NumOfXTI )
{
	NumOfXTI--;
	while( LX_RTC->RTC_BUSY == 1 );
	LX_RTC->TIC_VAL = NumOfXTI;
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_InitTicPeriodUS( uint32_t XTI_khz, uint32_t Period_us )
{
	uint32_t divider;
	float period_ms = (float)Period_us/1000;
	divider = ( XTI_khz ) * ( (uint32_t)period_ms );
	while( LX_RTC->RTC_BUSY == 1 );
	LX_RTC->TIC_VAL = divider - 1;
	while( LX_RTC->RTC_BUSY == 1 );												// ns
}

void HAL_RTC_InitSecPeriod( uint32_t NumOfTic )
{
	NumOfTic--;
	while( LX_RTC->RTC_BUSY == 1 );
	LX_RTC->SEC_VAL = NumOfTic;
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_AlarmIntEn( uint32_t MR_sec )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 &= ~( 1 << 10 );
	LX_RTC->RTC_CR = tmp32;																			// Enable Alarm Interrupt
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CNT;
	tmp32 = LX_RTC->RTC_CNT;
	while( LX_RTC->RTC_BUSY == 1 );
	tmp32 += MR_sec;
	LX_RTC->RTC_MR = tmp32;																			// Init Alarm
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_AlarmIntEnExactTime( uint32_t MRExact_sec )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 &= ~( 1 << 10 );
	LX_RTC->RTC_CR = tmp32;																			// Enable Alarm Interrupt
	while( LX_RTC->RTC_BUSY == 1 );

	LX_RTC->RTC_MR = MRExact_sec;																	// Init Alarm
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_TicIntEn( void )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 &= ~( 1 << 8 );
	LX_RTC->RTC_CR = tmp32;																			//Enable TIC Interrupt
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_WDogIntEn( uint32_t WDT_tic )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 &= ~( 1 << 9 );

	switch ( WDT_tic )
	{
		case 1:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			break;
		case 2:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			tmp32 |= ( 1 << 4 );
			break;
		case 4:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			tmp32 |= ( 2 << 4 );
			break;
		case 8:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			tmp32 |= ( 3 << 4 );
			break;
		case 16:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			tmp32 |= ( 4 << 4 );
			break;
		case 32:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			tmp32 |= ( 5 << 4 );
			break;
		case 64:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			tmp32 |= ( 6 << 4 );
			break;
		case 128:
			tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
			tmp32 |= ( 7 << 4 );
			break;
		default:
			break;
	}

	LX_RTC->WDT_CNT = 0x04072013;																	// A little magic
	while( LX_RTC->RTC_BUSY == 1 );

	LX_RTC->RTC_CR = tmp32;																			// Enable TIC Interrupt
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_WDogHWResetEn( uint32_t WDT_tic )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	if ( WDT_tic == 1 )
	{
		tmp32 &= ~( 7 << 4 );
	}
	else if ( WDT_tic == 2 )
	{
		tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
		tmp32 |= ( 1 << 4 );
	}
	else if ( WDT_tic == 4 )
	{
		tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
		tmp32 |= ( 2 << 4 );
	}
	else if ( WDT_tic == 8 )
	{
		tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
		tmp32 |= ( 3 << 4 );
	}
	else if ( WDT_tic == 16 )
	{
		tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
		tmp32 |= ( 4 << 4 );
	}
	else if ( WDT_tic == 32 )
	{
		tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
		tmp32 |= ( 5 << 4 );
	}
	else if ( WDT_tic == 64 )
	{
		tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
		tmp32 |= ( 6 << 4 );
	}
	else if ( WDT_tic == 128 )
	{
		tmp32 &= ~( 7 << 4 ); 																	// reset WDT_SEL
		tmp32 |= ( 7 << 4 );
	}

	LX_RTC->RTC_CR = tmp32;																		// Init Wdog
	while( LX_RTC->RTC_BUSY == 1 );
	tmp32 |= ( 1 << 2 );

	LX_RTC->WDT_CNT = 0x04072013;																// A little magic
	while( LX_RTC->RTC_BUSY == 1 );

	LX_RTC->RTC_CR = tmp32;																		// Enable WDog
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_AlarmIntDis( void )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 |= ( 1 << 10 );
	LX_RTC->RTC_CR = tmp32;																			// Disable Alarm Interrupt
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_TicIntDis( void )
{
	uint32_t volatile tmp32;

	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 |= ( 1 << 8 );
	LX_RTC->RTC_CR = tmp32;																			// Disable Tic Interrupt
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_WDogIntDis( void )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 |= ( 1 << 9 );
	LX_RTC->RTC_CR = tmp32;																			// Disable WDog Interrupt
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_WDogHWResetDis( void )
{
	uint32_t volatile tmp32;

	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 &= ~(1 << 2);
	LX_RTC->RTC_CR = tmp32;																			// Disable WDog core reset
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_WDogLock( void )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 |= ( 1 << 3 );
	LX_RTC->RTC_CR = tmp32;																			// Lock WDOG
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_Lock( void )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 |= ( 1 << 7 );
	LX_RTC->RTC_CR = tmp32;																			// Lock WDOG
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_WDogUnLock( void )
{
	uint32_t volatile tmp32;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 &= ~( 1 << 3 );
	LX_RTC->RTC_CR = tmp32;																			// Lock WDOG
	while( LX_RTC->RTC_BUSY == 1 );
}

void HAL_RTC_UnLock( void )
{
	uint32_t volatile tmp32;

	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 = LX_RTC->RTC_CR;
	tmp32 = LX_RTC->RTC_CR;
	while( LX_RTC->RTC_BUSY == 1 );

	tmp32 &= ~( 1 << 7 );
	LX_RTC->RTC_CR = tmp32;																			// Lock WDOG
	while( LX_RTC->RTC_BUSY == 1 );
}

int HAL_RTC_GetCurrectClock(int Num)
{
	int XTI = 25000; // kHz
	int Start_clk , Stop_clk;
	int Count_rtc = 0 , Count_clk = 0;
	int SEC , TIC , CNT , i , Result;

	HAL_RTC_Lock();
	SEC = HAL_RTC_RegRead(rtcSECVAL);
	TIC = HAL_RTC_RegRead(rtcTICVAL);

	for (i = 0; i < Num+1; i++)
	{
		CNT = HAL_RTC_RegRead(rtcCNT);
		while(LX_RTC->RTC_CNT != CNT+1);
		Start_clk = __read_ccnt();
		while(LX_RTC->RTC_CNT != CNT+2);
		Stop_clk = __read_ccnt();
		if (i > 0) Count_clk += abs(Stop_clk - Start_clk);
	}

	Count_clk = (float)(Count_clk) / Num;
	Count_rtc = (SEC + 1) * (TIC  + 1);

	Result = ((int)(round(((float)(Count_clk) / Count_rtc ) * XTI)));
	HAL_RTC_UnLock();
	return Result;
}
