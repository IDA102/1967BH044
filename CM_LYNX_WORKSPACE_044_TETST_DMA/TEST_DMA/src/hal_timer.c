/*
 *
 *	File/Файл: 		hal_timer.c
 *	Description/Описание: 	HAL for Timers with capture function or PWM/ HAL для модуля таймеров с функцией захвата/ШИМ
 *
 *	History/История:
 *	 			20-Jun-2017 Dmitriy Sharaviev 	- 	created/создан
 *
 */

#include "hal_1967VN044.h"

// Timer deinitialization/Сброс настроек таймера
void HAL_TIMER_DeInit( TIMER_type* TIMERx )
{
	TIMERx->CNTRL.word = 0;
	TIMERx->CNT = 0;
	TIMERx->PSG = 0;
	TIMERx->ARR = 0;

	TIMERx->CHx_CNTRL[ TIMER_CH1 ].word = 0;
	TIMERx->CHx_CNTRL[ TIMER_CH2 ].word = 0;
	TIMERx->CHx_CNTRL[ TIMER_CH3 ].word = 0;
	TIMERx->CHx_CNTRL[ TIMER_CH4 ].word = 0;
	TIMERx->CHx_CNTRL1[ TIMER_CH1 ].word = 0;
	TIMERx->CHx_CNTRL1[ TIMER_CH2 ].word = 0;
	TIMERx->CHx_CNTRL1[ TIMER_CH3 ].word = 0;
	TIMERx->CHx_CNTRL1[ TIMER_CH4 ].word = 0;
	TIMERx->CHx_CNTRL2[ TIMER_CH1 ].word = 0;
	TIMERx->CHx_CNTRL2[ TIMER_CH2 ].word = 0;
	TIMERx->CHx_CNTRL2[ TIMER_CH3 ].word = 0;
	TIMERx->CHx_CNTRL2[ TIMER_CH4 ].word = 0;

	TIMERx->CCRx[ TIMER_CH1 ] = 0;
	TIMERx->CCRx[ TIMER_CH2 ] = 0;
	TIMERx->CCRx[ TIMER_CH3 ] = 0;
	TIMERx->CCRx[ TIMER_CH4 ] = 0;
	TIMERx->CCRx1[ TIMER_CH1 ] = 0;
	TIMERx->CCRx1[ TIMER_CH2 ] = 0;
	TIMERx->CCRx1[ TIMER_CH3 ] = 0;
	TIMERx->CCRx1[ TIMER_CH4 ] = 0;
	TIMERx->CHx_DTG[ TIMER_CH1 ].word = 0;
	TIMERx->CHx_DTG[ TIMER_CH2 ].word = 0;
	TIMERx->CHx_DTG[ TIMER_CH3 ].word = 0;
	TIMERx->CHx_DTG[ TIMER_CH4 ].word = 0;
	TIMERx->BRKETR_CNTRL.word = 0;
	TIMERx->STATUS.word = 0;
	TIMERx->IE.word = 0;
	TIMERx->DMA_RE.word = 0;

	TIMERx->DMA_REChx[ TIMER_CH1 ] = 0;
	TIMERx->DMA_REChx[ TIMER_CH2 ] = 0;
	TIMERx->DMA_REChx[ TIMER_CH3 ] = 0;
	TIMERx->DMA_REChx[ TIMER_CH4 ] = 0;
}

// Timer periphery initialization/Инициализация перефирии таймера
void HAL_TIMER_CntInit( TIMER_type* TIMERx, const TIMER_CntInit_type* TIMER_CntInitStruct )
{
  TIMERx->CNT = TIMER_CntInitStruct->IniCounter;
  TIMERx->PSG = TIMER_CntInitStruct->Prescaler;
  TIMERx->ARR = TIMER_CntInitStruct->Period;

  TIMERx->CNTRL.b.CNT_MODE = TIMER_CntInitStruct->CounterMode;
  TIMERx->CNTRL.b.DIR = TIMER_CntInitStruct->CounterDirection;
  TIMERx->CNTRL.b.EVENT_SEL = TIMER_CntInitStruct->EventSource;
  TIMERx->CNTRL.b.FDTS = TIMER_CntInitStruct->FilterSampling;
  TIMERx->CNTRL.b.ARRB_EN = TIMER_CntInitStruct->ARR_UpdateMode;

  TIMERx->BRKETR_CNTRL.b.ETR_FILTER = TIMER_CntInitStruct->ETR_FilterConf;
  TIMERx->BRKETR_CNTRL.b.ETRPSC = TIMER_CntInitStruct->ETR_Prescaler;
  TIMERx->BRKETR_CNTRL.b.ETR_INV = TIMER_CntInitStruct->ETR_Polarity;
  TIMERx->BRKETR_CNTRL.b.BRK_INV = TIMER_CntInitStruct->BRK_Polarity;
}

// Timer structure initialization by default values/Инициализация структуры таймера значениями по-умолчанию
void HAL_TIMER_CntStructInit( TIMER_CntInit_type* TIMER_CntInitStruct )
{
  TIMER_CntInitStruct->IniCounter 	  	= 0;
  TIMER_CntInitStruct->Prescaler  	  	= 0;
  TIMER_CntInitStruct->Period     	 	= 0;
  TIMER_CntInitStruct->CounterMode      = TIMER_CntMode_ClkFixedDir;
  TIMER_CntInitStruct->CounterDirection = TIMER_CntDir_Up;
  TIMER_CntInitStruct->EventSource      = TIMER_EvSrc_None;
  TIMER_CntInitStruct->FilterSampling   = TIMER_FdtsDiv_1;
  TIMER_CntInitStruct->ARR_UpdateMode   = TIMER_Update_Immediately;
  TIMER_CntInitStruct->ETR_FilterConf   = TIMER_Filter_1FF_at_TIMER_CLK;
  TIMER_CntInitStruct->ETR_Prescaler    = TIMER_Prescaler_None;
  TIMER_CntInitStruct->ETR_Polarity     = TIMER_Polarity_NonInverted;
  TIMER_CntInitStruct->BRK_Polarity     = TIMER_Polarity_NonInverted;
}

// Timer is enabled/disabled/Включение/выключение работы таймера
void HAL_TIMER_Cmd( TIMER_type* TIMERx, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		TIMERx->CNTRL.b.CNT_EN = 1;
	else
		TIMERx->CNTRL.b.CNT_EN = 0;
}

// Setting of the timer counter/Задание счетчика таймера
void HAL_TIMER_SetCounter( TIMER_type* TIMERx, uint32_t Counter )
{
	TIMERx->CNT = Counter;
}

// Setting of the timer prescaler/Задание делителя частоты таймера
void HAL_TIMER_SetCntPrescaler( TIMER_type* TIMERx, uint16_t Prescaler )
{
	TIMERx->PSG = Prescaler;
}

// Setting of timer counting base and the way of its renewal/Задание основания счета таймера и способа его обновления
void HAL_TIMER_CntAutoreloadConfig( TIMER_type* TIMERx, uint32_t Autoreload, TIMER_Update_type UpdateMode )
{
	TIMERx->CNTRL.b.ARRB_EN = UpdateMode;
	TIMERx->ARR = Autoreload;
}

// Get counter current value/Получение текущего значения счетчика
uint32_t HAL_TIMER_GetCounter( TIMER_type* TIMERx )
{
	return TIMERx->CNT;
}

// Get the direction of the timer counter/Получение направления счета таймера
TIMER_CNTRL_DIR_type HAL_TIMER_GetCounterDirection( TIMER_type* TIMERx )
{
	return ( TIMER_CNTRL_DIR_type )TIMERx->CNTRL.b.DIR;
}

// Get the status of registers CNT, PSG, ARR/Получение статуса состояния записи регистров CNT, PSG и ARR
State_type HAL_TIMER_GetCntWriteComplete( TIMER_type* TIMERx )
{
	if( TIMERx->CNTRL.b.WR_CMPL == 0 )
		return RESET;
	else
		return SET;
}

// Configuration of the ETR input/Настройка входа ETR
void HAL_TIMER_ETRInputConfig( TIMER_type* TIMERx, TIMER_Prescaler_type Prescaler, TIMER_Polarity_type Polarity, TIMER_Filter_type Filter )
{
	 TIMERx->BRKETR_CNTRL.b.ETR_FILTER = Filter;
	 TIMERx->BRKETR_CNTRL.b.ETRPSC = Prescaler;
	 TIMERx->BRKETR_CNTRL.b.ETR_INV = Polarity;
}

// Configuration of BRK input/Настройка входа BRK
void HAL_TIMER_BRKPolarityConfig( TIMER_type* TIMERx, TIMER_Polarity_type Polarity )
{
	TIMERx->BRKETR_CNTRL.b.BRK_INV = Polarity;
}

// Timer channel configuration/Настройка канала таймера
void HAL_TIMER_ChnInit( TIMER_type* TIMERx, const TIMER_ChnInit_type* TIMER_ChnInitStruct )
{

	TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.CAP_nPWM = TIMER_ChnInitStruct->Mode;
	TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.ETREN = TIMER_ChnInitStruct->ETR_Reset;
	TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.BRKEN = TIMER_ChnInitStruct->BRK_Reset;
	TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.OCCM = TIMER_ChnInitStruct->REF_Format;
	TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.CHPSC = TIMER_ChnInitStruct->Prescaler;
	TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.CHSEL = TIMER_ChnInitStruct->EventSource;
	TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.CHFLTR = TIMER_ChnInitStruct->FilterConf;

	if ( TIMER_ChnInitStruct->ETR_Ena != DISABLE )
		TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.OCCE = 1;
	else
		TIMERx->CHx_CNTRL[ TIMER_ChnInitStruct->ChNumber ].b.OCCE = 0;

	if ( TIMER_ChnInitStruct->CCR1_Ena != DISABLE )
		TIMERx->CHx_CNTRL2[ TIMER_ChnInitStruct->ChNumber ].b.CCR1_EN = 1;
	else
		TIMERx->CHx_CNTRL2[ TIMER_ChnInitStruct->ChNumber ].b.CCR1_EN = 0;

	TIMERx->CHx_CNTRL2[ TIMER_ChnInitStruct->ChNumber ].b.CRRRLD = TIMER_ChnInitStruct->CCR_UpdateMode;
	TIMERx->CHx_CNTRL2[ TIMER_ChnInitStruct->ChNumber ].b.CHSEL1 = TIMER_ChnInitStruct->CCR1_EventSource;
}

// Initialization of timer channel structure by default values/Инициализация структуры канала таймера значениями по-умолчанию
void HAL_TIMER_ChnStructInit( TIMER_ChnInit_type* TIMER_ChnInitStruct )
{
	TIMER_ChnInitStruct->ChNumber = TIMER_CH1;
	TIMER_ChnInitStruct->Mode = TIMER_CH_MODE_PWM;
	TIMER_ChnInitStruct->ETR_Ena = DISABLE;
	TIMER_ChnInitStruct->ETR_Reset = TIMER_CH_RESET_DISABLE;
	TIMER_ChnInitStruct->BRK_Reset = TIMER_CH_RESET_DISABLE;
	TIMER_ChnInitStruct->REF_Format = TIMER_CH_REF_Format0;
	TIMER_ChnInitStruct->Prescaler = TIMER_Prescaler_None;
	TIMER_ChnInitStruct->EventSource = TIMER_EventIn_PE;
	TIMER_ChnInitStruct->FilterConf = TIMER_Filter_1FF_at_TIMER_CLK;
	TIMER_ChnInitStruct->CCR_UpdateMode = TIMER_Update_Immediately;
	TIMER_ChnInitStruct->CCR1_Ena = DISABLE;
	TIMER_ChnInitStruct->CCR1_EventSource = TIMER_EventIn_PE;
}

// Setting of the comparison, capture register/Задание регистра сравнения, захвата
void HAL_TIMER_ChnCompareSet( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare )
{
	TIMERx->CCRx[ Channel ] = Compare;
}

// Setting of the comparison, capture register and the way of its renewal/Задание регистра сравнения, захвата и способа его обновления
void HAL_TIMER_ChnCompareConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare, TIMER_Update_type UpdateMode )
{
	TIMERx->CHx_CNTRL2[ Channel ].b.CRRRLD = UpdateMode;
	TIMERx->CCRx[ Channel ] = Compare;
}

// Setting of the comparison, capture register 1 /Задание регистра сравнения, захвата 1
void HAL_TIMER_ChnCompare1Set( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare )
{
	TIMERx->CCRx1[ Channel ] = Compare;
}

// Setting of the comparison, capture register 1 and the way of its renewal/Задание регистра сравнения, захвата 1 и способа его обновления
void HAL_TIMER_ChnCompare1Config( TIMER_type* TIMERx, TIMER_CH_type Channel, uint32_t Compare, TIMER_Update_type UpdateMode )
{
	TIMERx->CHx_CNTRL2[ Channel ].b.CRRRLD = UpdateMode;
	TIMERx->CCRx1[ Channel ] = Compare;
}

// Getting of the comparison, capture register/Получение регистра сравнения, захвата
uint32_t HAL_TIMER_GetChnCapture( TIMER_type* TIMERx, TIMER_CH_type Channel )
{
	return TIMERx->CCRx[ Channel ];
}

// Getting of the comparison, capture register 1/Получение регистра сравнения, захвата 1
uint32_t HAL_TIMER_GetChnCapture1( TIMER_type* TIMERx, TIMER_CH_type Channel )
{
	return TIMERx->CCRx1[ Channel ];
}

// Control of channel ETR input /Управление выводом ETR канала
void HAL_TIMER_ChnETR_Cmd( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		TIMERx->CHx_CNTRL[ Channel ].b.OCCE = 1;
	else
		TIMERx->CHx_CNTRL[ Channel ].b.OCCE = 0;
}

// Channel ETR reset configuration/ Настройка сброса канала по выводу ETR
void HAL_TIMER_ChnETRResetConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		TIMERx->CHx_CNTRL[ Channel ].b.ETREN = 1;
	else
		TIMERx->CHx_CNTRL[ Channel ].b.ETREN = 0;
}

// Channel BRK reset configuration/ Настройка сброса канала по выводу BRK
void HAL_TIMER_ChnBRKResetConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		TIMERx->CHx_CNTRL[ Channel ].b.BRKEN = 1;
	else
		TIMERx->CHx_CNTRL[ Channel ].b.BRKEN = 0;
}

// REF signal format configuration/ Настройка формата REF сигнала
void HAL_TIMER_ChnREFFormatConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_PWM_REF_type Format )
{
	TIMERx->CHx_CNTRL[ Channel ].b.OCCM = Format;
}

// Input channel prescaler configuration/ Настройка предварительного делителя входного канала
void HAL_TIMER_ChnCapturePrescalerConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_Prescaler_type Prescaler )
{
	TIMERx->CHx_CNTRL[ Channel ].b.CHPSC = Prescaler;
}

// Event configuration by input channel/ Настройка события по входному каналу
void HAL_TIMER_ChnEventSourceConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_EventIn_type EventSource )
{
	TIMERx->CHx_CNTRL[ Channel ].b.CHSEL = EventSource;
}

// Channel filter configuration/ Настройка фильтрации канала
void HAL_TIMER_ChnFilterConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_Filter_type Filter )
{
	TIMERx->CHx_CNTRL[ Channel ].b.CHFLTR = Filter;
}

// Getting of the write-status of a new value of CCR/Получение состояния записи нового значения регистра CCR
State_type HAL_TIMER_GetChnWriteComplete( TIMER_type* TIMERx, TIMER_CH_type Channel )
{
	if( TIMERx->CHx_CNTRL[ Channel ].b.WR_CMPL == 0 )
		return RESET;
	else
		return SET;
}

// Event configuration by input channel for CAP1/ Настройка события по входному каналу для CAP1
void HAL_TIMER_ChnCCR1_EventSourceConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_EventIn_type EventSource )
{
	TIMERx->CHx_CNTRL2[ Channel ].b.CHSEL1 = EventSource;
}

// Timer CCR1 control/ Управление работой CCR1
void HAL_TIMER_ChnCCR1_Cmd( TIMER_type* TIMERx, TIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		TIMERx->CHx_CNTRL2[ Channel ].b.CCR1_EN = 1;
	else
		TIMERx->CHx_CNTRL2[ Channel ].b.CCR1_EN = 0;
}

// Configuration of timer channel direct and inverse outputs/Настройка прямого и инверсного выхода канала таймера
void HAL_TIMER_ChnOutInit( TIMER_type* TIMERx, const TIMER_ChnOutInit_type* TIMER_ChnOutInitStruct )
{
	TIMERx->CHx_CNTRL1[ TIMER_ChnOutInitStruct->ChNumber ].b.INV = TIMER_ChnOutInitStruct->DirOut_Polarity;
	TIMERx->CHx_CNTRL1[ TIMER_ChnOutInitStruct->ChNumber ].b.SELO = TIMER_ChnOutInitStruct->DirOut_Source;
	TIMERx->CHx_CNTRL1[ TIMER_ChnOutInitStruct->ChNumber ].b.SELOE = TIMER_ChnOutInitStruct->DirOut_Mode;

	TIMERx->CHx_CNTRL1[ TIMER_ChnOutInitStruct->ChNumber ].b.NINV = TIMER_ChnOutInitStruct->DirOut_Polarity;
	TIMERx->CHx_CNTRL1[ TIMER_ChnOutInitStruct->ChNumber ].b.NSELO = TIMER_ChnOutInitStruct->DirOut_Source;
	TIMERx->CHx_CNTRL1[ TIMER_ChnOutInitStruct->ChNumber ].b.NSELOE = TIMER_ChnOutInitStruct->DirOut_Mode;

	TIMERx->CHx_DTG[ TIMER_ChnOutInitStruct->ChNumber ].b.DTG = TIMER_ChnOutInitStruct->DTG_AuxPrescaler;
	TIMERx->CHx_DTG[ TIMER_ChnOutInitStruct->ChNumber ].b.DTGX = TIMER_ChnOutInitStruct->DTG_MainPrescaler;
	TIMERx->CHx_DTG[ TIMER_ChnOutInitStruct->ChNumber ].b.EDTS = TIMER_ChnOutInitStruct->DTG_ClockSource;
}

// Initialization by default values of timer channel direct and inverse outputs/ Инициализация структуры прямого и инверсного выхода канала таймера значениями по-умолчанию
void HAL_TIMER_ChnOutStructInit( TIMER_ChnOutInit_type* TIMER_ChnOutInitStruct )
{
	TIMER_ChnOutInitStruct->DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
	TIMER_ChnOutInitStruct->DirOut_Source = TIMER_CH_OutSrc_Only_0;
	TIMER_ChnOutInitStruct->DirOut_Mode = TIMER_CH_OutMode_Input;
	TIMER_ChnOutInitStruct->NegOut_Polarity = TIMER_CHOPolarity_NonInverted;
	TIMER_ChnOutInitStruct->NegOut_Source = TIMER_CH_OutSrc_Only_0;
	TIMER_ChnOutInitStruct->NegOut_Mode = TIMER_CH_OutMode_Input;
	TIMER_ChnOutInitStruct->DTG_MainPrescaler = 0;
	TIMER_ChnOutInitStruct->DTG_AuxPrescaler = 0;
	TIMER_ChnOutInitStruct->DTG_ClockSource = TIMER_CH_DTG_ClkSrc_TIMER_CLK;
}

// Configuration of the timer direct channel/ Настройка прямого канала таймера
void HAL_TIMER_ChnOutConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_CHO_SOURCE_type OutSource, TIMER_CHO_MODE_type Mode, TIMER_CHO_POLARITY_type Polarity )
{
	TIMERx->CHx_CNTRL1[ Channel ].b.INV = Polarity;
	TIMERx->CHx_CNTRL1[ Channel ].b.SELO = OutSource;
	TIMERx->CHx_CNTRL1[ Channel ].b.SELOE = Mode;
}

// Configuration of the timer inverse channel/ Настройка инверсного канала таймера
void HAL_TIMER_ChnNOutConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, TIMER_CHO_SOURCE_type OutSource, TIMER_CHO_MODE_type Mode, TIMER_CHO_POLARITY_type Polarity )
{
	TIMERx->CHx_CNTRL1[ Channel ].b.NINV = Polarity;
	TIMERx->CHx_CNTRL1[ Channel ].b.NSELO = OutSource;
	TIMERx->CHx_CNTRL1[ Channel ].b.NSELOE = Mode;
}

// Configuration of the dead time generator/ Настройка генератора мертвого времени
void HAL_TIMER_ChnOutDTGConfig( TIMER_type* TIMERx, TIMER_CH_type Channel, uint16_t MainPrescaler, 	uint16_t AuxPrescaler, TIMER_CH_DTG_CLKSRC_type ClockSource )
{
	TIMERx->CHx_DTG[ Channel ].b.DTG = AuxPrescaler;
	TIMERx->CHx_DTG[ Channel ].b.DTGX = MainPrescaler;
	TIMERx->CHx_DTG[ Channel ].b.EDTS = ClockSource;
}

// Getting of the timer status/ Получение статуса таймера
uint32_t HAL_TIMER_GetStatus( TIMER_type* TIMERx )
{
	return TIMERx->STATUS.word;
}

// Getting of the state of the status flag/ Получение состояния флага статусаа
State_type HAL_TIMER_GetFlagStatus( TIMER_type* TIMERx, uint32_t Flag )
{
	if( ( TIMERx->STATUS.word & Flag ) == 0 )
		return RESET;
	else
		return SET;
}

// Clearing of the status flag/Сброс флага статуса
void HAL_TIMER_ClearFlag( TIMER_type* TIMERx, uint32_t Flags )
{
	TIMERx->STATUS.word &= ~Flags;
}

// Configuration of DMA request source/ Настройка источника запросов для DMA
void HAL_TIMER_DMACmd( TIMER_type* TIMERx, uint32_t TIMER_DMASource, TIMER_DMA_CH_type TIMER_DMA_Channel, FunctionalState_type NewState )
{
	if( TIMER_DMA_Channel == TIMER_DMA_ALL_CH )
	{
		if( NewState != DISABLE )
			TIMERx->DMA_RE.word |= TIMER_DMASource;
		else
			TIMERx->DMA_RE.word &= ~TIMER_DMASource;
	}
	else
	{
		if( NewState != DISABLE )
			TIMERx->DMA_REChx[ TIMER_DMA_Channel ] |= TIMER_DMASource;
		else
			TIMERx->DMA_REChx[ TIMER_DMA_Channel ] &= ~TIMER_DMASource;
	}
}

// Configuration of timer interrupt/ Настройка прерываний от таймера
void HAL_TIMER_ITConfig( TIMER_type* TIMERx, uint32_t TIMER_IT, FunctionalState_type NewState )
{
	if( NewState != DISABLE )
		TIMERx->IE.word |= TIMER_IT;
	else
		TIMERx->IE.word &= ~TIMER_IT;
}

// Get interrupt status/Получение статуса прерывания
State_type HAL_TIMER_GetITStatus( TIMER_type* TIMERx, uint32_t TIMER_IT )
{
	if( TIMERx->STATUS.word & TIMERx->IE.word & TIMER_IT )
		return SET;
	else
		return RESET;
}
