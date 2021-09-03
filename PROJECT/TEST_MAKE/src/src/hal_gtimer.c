/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	File/Файл: 		hal_GTIMER.c
 *	Description/Описание: 	HAL for GTIMERs with capture function or PWM/ HAL для модуля таймеров с функцией захвата/ШИМ
 *
 *	History/История:
 *	 			20-Jun-2017 Dmitriy Sharaviev 	- 	created/создан
 *
 */

#include "hal_1967VN044.h"

// GTIMER deinitialization/Сброс настроек таймера
void HAL_GTIMER_DeInit( GTIMER_type* GTIMERx )
{
	GTIMERx->CNTRL.word = 0;
	GTIMERx->CNT = 0;
	GTIMERx->PSG = 0;
	GTIMERx->ARR = 0;

	GTIMERx->CHx_CNTRL[ GTIMER_CH1 ].word = 0;
	GTIMERx->CHx_CNTRL[ GTIMER_CH2 ].word = 0;
	GTIMERx->CHx_CNTRL[ GTIMER_CH3 ].word = 0;
	GTIMERx->CHx_CNTRL[ GTIMER_CH4 ].word = 0;
	GTIMERx->CHx_CNTRL1[ GTIMER_CH1 ].word = 0;
	GTIMERx->CHx_CNTRL1[ GTIMER_CH2 ].word = 0;
	GTIMERx->CHx_CNTRL1[ GTIMER_CH3 ].word = 0;
	GTIMERx->CHx_CNTRL1[ GTIMER_CH4 ].word = 0;
	GTIMERx->CHx_CNTRL2[ GTIMER_CH1 ].word = 0;
	GTIMERx->CHx_CNTRL2[ GTIMER_CH2 ].word = 0;
	GTIMERx->CHx_CNTRL2[ GTIMER_CH3 ].word = 0;
	GTIMERx->CHx_CNTRL2[ GTIMER_CH4 ].word = 0;

	GTIMERx->CCRx[ GTIMER_CH1 ] = 0;
	GTIMERx->CCRx[ GTIMER_CH2 ] = 0;
	GTIMERx->CCRx[ GTIMER_CH3 ] = 0;
	GTIMERx->CCRx[ GTIMER_CH4 ] = 0;
	GTIMERx->CCRx1[ GTIMER_CH1 ] = 0;
	GTIMERx->CCRx1[ GTIMER_CH2 ] = 0;
	GTIMERx->CCRx1[ GTIMER_CH3 ] = 0;
	GTIMERx->CCRx1[ GTIMER_CH4 ] = 0;
	GTIMERx->CHx_DTG[ GTIMER_CH1 ].word = 0;
	GTIMERx->CHx_DTG[ GTIMER_CH2 ].word = 0;
	GTIMERx->CHx_DTG[ GTIMER_CH3 ].word = 0;
	GTIMERx->CHx_DTG[ GTIMER_CH4 ].word = 0;
	GTIMERx->BRKETR_CNTRL.word = 0;
	GTIMERx->STATUS.word = 0;
	GTIMERx->IE.word = 0;
	GTIMERx->DMA_RE.word = 0;

	GTIMERx->DMA_REChx[ GTIMER_CH1 ] = 0;
	GTIMERx->DMA_REChx[ GTIMER_CH2 ] = 0;
	GTIMERx->DMA_REChx[ GTIMER_CH3 ] = 0;
	GTIMERx->DMA_REChx[ GTIMER_CH4 ] = 0;
}

// GTIMER periphery initialization/Инициализация перефирии таймера
void HAL_GTIMER_CntInit( GTIMER_type* GTIMERx, const GTIMER_CntInit_type* GTIMER_CntInitStruct )
{
  GTIMERx->CNT = GTIMER_CntInitStruct->IniCounter;
  GTIMERx->PSG = GTIMER_CntInitStruct->Prescaler;
  GTIMERx->ARR = GTIMER_CntInitStruct->Period;

  GTIMERx->CNTRL.b.CNT_MODE = GTIMER_CntInitStruct->CounterMode;
  GTIMERx->CNTRL.b.DIR = GTIMER_CntInitStruct->CounterDirection;
  GTIMERx->CNTRL.b.EVENT_SEL = GTIMER_CntInitStruct->EventSource;
  GTIMERx->CNTRL.b.FDTS = GTIMER_CntInitStruct->FilterSampling;
  GTIMERx->CNTRL.b.ARRB_EN = GTIMER_CntInitStruct->ARR_UpdateMode;

  GTIMERx->BRKETR_CNTRL.b.ETR_FILTER = GTIMER_CntInitStruct->ETR_FilterConf;
  GTIMERx->BRKETR_CNTRL.b.ETRPSC = GTIMER_CntInitStruct->ETR_Prescaler;
  GTIMERx->BRKETR_CNTRL.b.ETR_INV = GTIMER_CntInitStruct->ETR_Polarity;
  GTIMERx->BRKETR_CNTRL.b.BRK_INV = GTIMER_CntInitStruct->BRK_Polarity;
}

// GTIMER structure initialization by default values/Инициализация структуры таймера значениями по-умолчанию
void HAL_GTIMER_CntStructInit( GTIMER_CntInit_type* GTIMER_CntInitStruct )
{
  GTIMER_CntInitStruct->IniCounter 	  	= 0;
  GTIMER_CntInitStruct->Prescaler  	  	= 0;
  GTIMER_CntInitStruct->Period     	 	= 0;
  GTIMER_CntInitStruct->CounterMode      = GTIMER_CntMode_ClkFixedDir;
  GTIMER_CntInitStruct->CounterDirection = GTIMER_CntDir_Up;
  GTIMER_CntInitStruct->EventSource      = GTIMER_EvSrc_None;
  GTIMER_CntInitStruct->FilterSampling   = GTIMER_FdtsDiv_1;
  GTIMER_CntInitStruct->ARR_UpdateMode   = GTIMER_Update_Immediately;
  GTIMER_CntInitStruct->ETR_FilterConf   = GTIMER_Filter_1FF_at_GTIMER_CLK;
  GTIMER_CntInitStruct->ETR_Prescaler    = GTIMER_Prescaler_None;
  GTIMER_CntInitStruct->ETR_Polarity     = GTIMER_Polarity_NonInverted;
  GTIMER_CntInitStruct->BRK_Polarity     = GTIMER_Polarity_NonInverted;
}

// GTIMER is enabled/disabled/Включение/выключение работы таймера
void HAL_GTIMER_Cmd( GTIMER_type* GTIMERx, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		GTIMERx->CNTRL.b.CNT_EN = 1;
	else
		GTIMERx->CNTRL.b.CNT_EN = 0;
}

// Setting of the GTIMER counter/Задание счетчика таймера
void HAL_GTIMER_SetCounter( GTIMER_type* GTIMERx, uint32_t Counter )
{
	GTIMERx->CNT = Counter;
}

// Setting of the GTIMER prescaler/Задание делителя частоты таймера
void HAL_GTIMER_SetCntPrescaler( GTIMER_type* GTIMERx, uint16_t Prescaler )
{
	GTIMERx->PSG = Prescaler;
}

// Setting of GTIMER counting base and the way of its renewal/Задание основания счета таймера и способа его обновления
void HAL_GTIMER_CntAutoreloadConfig( GTIMER_type* GTIMERx, uint32_t Autoreload, GTIMER_Update_type UpdateMode )
{
	GTIMERx->CNTRL.b.ARRB_EN = UpdateMode;
	GTIMERx->ARR = Autoreload;
}

// Get counter current value/Получение текущего значения счетчика
uint32_t HAL_GTIMER_GetCounter( GTIMER_type* GTIMERx )
{
	return GTIMERx->CNT;
}

// Get the direction of the GTIMER counter/Получение направления счета таймера
GTIMER_CNTRL_DIR_type HAL_GTIMER_GetCounterDirection( GTIMER_type* GTIMERx )
{
	return ( GTIMER_CNTRL_DIR_type )GTIMERx->CNTRL.b.DIR;
}

// Get the status of registers CNT, PSG, ARR/Получение статуса состояния записи регистров CNT, PSG и ARR
State_type HAL_GTIMER_GetCntWriteComplete( GTIMER_type* GTIMERx )
{
	if( GTIMERx->CNTRL.b.WR_CMPL == 0 )
		return RESET;
	else
		return SET;
}

// Configuration of the ETR input/Настройка входа ETR
void HAL_GTIMER_ETRInputConfig( GTIMER_type* GTIMERx, GTIMER_Prescaler_type Prescaler, GTIMER_Polarity_type Polarity, GTIMER_Filter_type Filter )
{
	 GTIMERx->BRKETR_CNTRL.b.ETR_FILTER = Filter;
	 GTIMERx->BRKETR_CNTRL.b.ETRPSC = Prescaler;
	 GTIMERx->BRKETR_CNTRL.b.ETR_INV = Polarity;
}

// Configuration of BRK input/Настройка входа BRK
void HAL_GTIMER_BRKPolarityConfig( GTIMER_type* GTIMERx, GTIMER_Polarity_type Polarity )
{
	GTIMERx->BRKETR_CNTRL.b.BRK_INV = Polarity;
}

// GTIMER channel configuration/Настройка канала таймера
void HAL_GTIMER_ChnInit( GTIMER_type* GTIMERx, const GTIMER_ChnInit_type* GTIMER_ChnInitStruct )
{

	GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.CAP_nPWM = GTIMER_ChnInitStruct->Mode;
	GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.ETREN = GTIMER_ChnInitStruct->ETR_Reset;
	GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.BRKEN = GTIMER_ChnInitStruct->BRK_Reset;
	GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.OCCM = GTIMER_ChnInitStruct->REF_Format;
	GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.CHPSC = GTIMER_ChnInitStruct->Prescaler;
	GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.CHSEL = GTIMER_ChnInitStruct->EventSource;
	GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.CHFLTR = GTIMER_ChnInitStruct->FilterConf;

	if ( GTIMER_ChnInitStruct->ETR_Ena != DISABLE )
		GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.OCCE = 1;
	else
		GTIMERx->CHx_CNTRL[ GTIMER_ChnInitStruct->ChNumber ].b.OCCE = 0;

	if ( GTIMER_ChnInitStruct->CCR1_Ena != DISABLE )
		GTIMERx->CHx_CNTRL2[ GTIMER_ChnInitStruct->ChNumber ].b.CCR1_EN = 1;
	else
		GTIMERx->CHx_CNTRL2[ GTIMER_ChnInitStruct->ChNumber ].b.CCR1_EN = 0;

	GTIMERx->CHx_CNTRL2[ GTIMER_ChnInitStruct->ChNumber ].b.CRRRLD = GTIMER_ChnInitStruct->CCR_UpdateMode;
	GTIMERx->CHx_CNTRL2[ GTIMER_ChnInitStruct->ChNumber ].b.CHSEL1 = GTIMER_ChnInitStruct->CCR1_EventSource;
}

// Initialization of GTIMER channel structure by default values/Инициализация структуры канала таймера значениями по-умолчанию
void HAL_GTIMER_ChnStructInit( GTIMER_ChnInit_type* GTIMER_ChnInitStruct )
{
	GTIMER_ChnInitStruct->ChNumber = GTIMER_CH1;
	GTIMER_ChnInitStruct->Mode = GTIMER_CH_MODE_PWM;
	GTIMER_ChnInitStruct->ETR_Ena = DISABLE;
	GTIMER_ChnInitStruct->ETR_Reset = GTIMER_CH_RESET_DISABLE;
	GTIMER_ChnInitStruct->BRK_Reset = GTIMER_CH_RESET_DISABLE;
	GTIMER_ChnInitStruct->REF_Format = GTIMER_CH_REF_Format0;
	GTIMER_ChnInitStruct->Prescaler = GTIMER_Prescaler_None;
	GTIMER_ChnInitStruct->EventSource = GTIMER_EventIn_PE;
	GTIMER_ChnInitStruct->FilterConf = GTIMER_Filter_1FF_at_GTIMER_CLK;
	GTIMER_ChnInitStruct->CCR_UpdateMode = GTIMER_Update_Immediately;
	GTIMER_ChnInitStruct->CCR1_Ena = DISABLE;
	GTIMER_ChnInitStruct->CCR1_EventSource = GTIMER_EventIn_PE;
}

// Setting of the comparison, capture register/Задание регистра сравнения, захвата
void HAL_GTIMER_ChnCompareSet( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare )
{
	GTIMERx->CCRx[ Channel ] = Compare;
}

// Setting of the comparison, capture register and the way of its renewal/Задание регистра сравнения, захвата и способа его обновления
void HAL_GTIMER_ChnCompareConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare, GTIMER_Update_type UpdateMode )
{
	GTIMERx->CHx_CNTRL2[ Channel ].b.CRRRLD = UpdateMode;
	GTIMERx->CCRx[ Channel ] = Compare;
}

// Setting of the comparison, capture register 1 /Задание регистра сравнения, захвата 1
void HAL_GTIMER_ChnCompare1Set( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare )
{
	GTIMERx->CCRx1[ Channel ] = Compare;
}

// Setting of the comparison, capture register 1 and the way of its renewal/Задание регистра сравнения, захвата 1 и способа его обновления
void HAL_GTIMER_ChnCompare1Config( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint32_t Compare, GTIMER_Update_type UpdateMode )
{
	GTIMERx->CHx_CNTRL2[ Channel ].b.CRRRLD = UpdateMode;
	GTIMERx->CCRx1[ Channel ] = Compare;
}

// Getting of the comparison, capture register/Получение регистра сравнения, захвата
uint32_t HAL_GTIMER_GetChnCapture( GTIMER_type* GTIMERx, GTIMER_CH_type Channel )
{
	return GTIMERx->CCRx[ Channel ];
}

// Getting of the comparison, capture register 1/Получение регистра сравнения, захвата 1
uint32_t HAL_GTIMER_GetChnCapture1( GTIMER_type* GTIMERx, GTIMER_CH_type Channel )
{
	return GTIMERx->CCRx1[ Channel ];
}

// Control of channel ETR input /Управление выводом ETR канала
void HAL_GTIMER_ChnETR_Cmd( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		GTIMERx->CHx_CNTRL[ Channel ].b.OCCE = 1;
	else
		GTIMERx->CHx_CNTRL[ Channel ].b.OCCE = 0;
}

// Channel ETR reset configuration/ Настройка сброса канала по выводу ETR
void HAL_GTIMER_ChnETRResetConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		GTIMERx->CHx_CNTRL[ Channel ].b.ETREN = 1;
	else
		GTIMERx->CHx_CNTRL[ Channel ].b.ETREN = 0;
}

// Channel BRK reset configuration/ Настройка сброса канала по выводу BRK
void HAL_GTIMER_ChnBRKResetConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		GTIMERx->CHx_CNTRL[ Channel ].b.BRKEN = 1;
	else
		GTIMERx->CHx_CNTRL[ Channel ].b.BRKEN = 0;
}

// REF signal format configuration/ Настройка формата REF сигнала
void HAL_GTIMER_ChnREFFormatConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_PWM_REF_type Format )
{
	GTIMERx->CHx_CNTRL[ Channel ].b.OCCM = Format;
}

// Input channel prescaler configuration/ Настройка предварительного делителя входного канала
void HAL_GTIMER_ChnCapturePrescalerConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_Prescaler_type Prescaler )
{
	GTIMERx->CHx_CNTRL[ Channel ].b.CHPSC = Prescaler;
}

// Event configuration by input channel/ Настройка события по входному каналу
void HAL_GTIMER_ChnEventSourceConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_EventIn_type EventSource )
{
	GTIMERx->CHx_CNTRL[ Channel ].b.CHSEL = EventSource;
}

// Channel filter configuration/ Настройка фильтрации канала
void HAL_GTIMER_ChnFilterConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_Filter_type Filter )
{
	GTIMERx->CHx_CNTRL[ Channel ].b.CHFLTR = Filter;
}

// Getting of the write-status of a new value of CCR/Получение состояния записи нового значения регистра CCR
State_type HAL_GTIMER_GetChnWriteComplete( GTIMER_type* GTIMERx, GTIMER_CH_type Channel )
{
	if( GTIMERx->CHx_CNTRL[ Channel ].b.WR_CMPL == 0 )
		return RESET;
	else
		return SET;
}

// Event configuration by input channel for CAP1/ Настройка события по входному каналу для CAP1
void HAL_GTIMER_ChnCCR1_EventSourceConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_EventIn_type EventSource )
{
	GTIMERx->CHx_CNTRL2[ Channel ].b.CHSEL1 = EventSource;
}

// GTIMER CCR1 control/ Управление работой CCR1
void HAL_GTIMER_ChnCCR1_Cmd( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, FunctionalState_type NewState )
{
	if ( NewState != DISABLE )
		GTIMERx->CHx_CNTRL2[ Channel ].b.CCR1_EN = 1;
	else
		GTIMERx->CHx_CNTRL2[ Channel ].b.CCR1_EN = 0;
}

// Configuration of GTIMER channel direct and inverse outputs/Настройка прямого и инверсного выхода канала таймера
void HAL_GTIMER_ChnOutInit( GTIMER_type* GTIMERx, const GTIMER_ChnOutInit_type* GTIMER_ChnOutInitStruct )
{
	GTIMERx->CHx_CNTRL1[ GTIMER_ChnOutInitStruct->ChNumber ].b.INV = GTIMER_ChnOutInitStruct->DirOut_Polarity;
	GTIMERx->CHx_CNTRL1[ GTIMER_ChnOutInitStruct->ChNumber ].b.SELO = GTIMER_ChnOutInitStruct->DirOut_Source;
	GTIMERx->CHx_CNTRL1[ GTIMER_ChnOutInitStruct->ChNumber ].b.SELOE = GTIMER_ChnOutInitStruct->DirOut_Mode;

	GTIMERx->CHx_CNTRL1[ GTIMER_ChnOutInitStruct->ChNumber ].b.NINV = GTIMER_ChnOutInitStruct->DirOut_Polarity;
	GTIMERx->CHx_CNTRL1[ GTIMER_ChnOutInitStruct->ChNumber ].b.NSELO = GTIMER_ChnOutInitStruct->DirOut_Source;
	GTIMERx->CHx_CNTRL1[ GTIMER_ChnOutInitStruct->ChNumber ].b.NSELOE = GTIMER_ChnOutInitStruct->DirOut_Mode;

	GTIMERx->CHx_DTG[ GTIMER_ChnOutInitStruct->ChNumber ].b.DTG = GTIMER_ChnOutInitStruct->DTG_AuxPrescaler;
	GTIMERx->CHx_DTG[ GTIMER_ChnOutInitStruct->ChNumber ].b.DTGX = GTIMER_ChnOutInitStruct->DTG_MainPrescaler;
	GTIMERx->CHx_DTG[ GTIMER_ChnOutInitStruct->ChNumber ].b.EDTS = GTIMER_ChnOutInitStruct->DTG_ClockSource;
}

// Initialization by default values of GTIMER channel direct and inverse outputs/ Инициализация структуры прямого и инверсного выхода канала таймера значениями по-умолчанию
void HAL_GTIMER_ChnOutStructInit( GTIMER_ChnOutInit_type* GTIMER_ChnOutInitStruct )
{
	GTIMER_ChnOutInitStruct->DirOut_Polarity = GTIMER_CHOPolarity_NonInverted;
	GTIMER_ChnOutInitStruct->DirOut_Source = GTIMER_CH_OutSrc_Only_0;
	GTIMER_ChnOutInitStruct->DirOut_Mode = GTIMER_CH_OutMode_Input;
	GTIMER_ChnOutInitStruct->NegOut_Polarity = GTIMER_CHOPolarity_NonInverted;
	GTIMER_ChnOutInitStruct->NegOut_Source = GTIMER_CH_OutSrc_Only_0;
	GTIMER_ChnOutInitStruct->NegOut_Mode = GTIMER_CH_OutMode_Input;
	GTIMER_ChnOutInitStruct->DTG_MainPrescaler = 0;
	GTIMER_ChnOutInitStruct->DTG_AuxPrescaler = 0;
	GTIMER_ChnOutInitStruct->DTG_ClockSource = GTIMER_CH_DTG_ClkSrc_GTIMER_CLK;
}

// Configuration of the GTIMER direct channel/ Настройка прямого канала таймера
void HAL_GTIMER_ChnOutConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_CHO_SOURCE_type OutSource, GTIMER_CHO_MODE_type Mode, GTIMER_CHO_POLARITY_type Polarity )
{
	GTIMERx->CHx_CNTRL1[ Channel ].b.INV = Polarity;
	GTIMERx->CHx_CNTRL1[ Channel ].b.SELO = OutSource;
	GTIMERx->CHx_CNTRL1[ Channel ].b.SELOE = Mode;
}

// Configuration of the GTIMER inverse channel/ Настройка инверсного канала таймера
void HAL_GTIMER_ChnNOutConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, GTIMER_CHO_SOURCE_type OutSource, GTIMER_CHO_MODE_type Mode, GTIMER_CHO_POLARITY_type Polarity )
{
	GTIMERx->CHx_CNTRL1[ Channel ].b.NINV = Polarity;
	GTIMERx->CHx_CNTRL1[ Channel ].b.NSELO = OutSource;
	GTIMERx->CHx_CNTRL1[ Channel ].b.NSELOE = Mode;
}

// Configuration of the dead time generator/ Настройка генератора мертвого времени
void HAL_GTIMER_ChnOutDTGConfig( GTIMER_type* GTIMERx, GTIMER_CH_type Channel, uint16_t MainPrescaler, 	uint16_t AuxPrescaler, GTIMER_CH_DTG_CLKSRC_type ClockSource )
{
	GTIMERx->CHx_DTG[ Channel ].b.DTG = AuxPrescaler;
	GTIMERx->CHx_DTG[ Channel ].b.DTGX = MainPrescaler;
	GTIMERx->CHx_DTG[ Channel ].b.EDTS = ClockSource;
}

// Getting of the GTIMER status/ Получение статуса таймера
uint32_t HAL_GTIMER_GetStatus( GTIMER_type* GTIMERx )
{
	return GTIMERx->STATUS.word;
}

// Getting of the state of the status flag/ Получение состояния флага статусаа
State_type HAL_GTIMER_GetFlagStatus( GTIMER_type* GTIMERx, uint32_t Flag )
{
	if( ( GTIMERx->STATUS.word & Flag ) == 0 )
		return RESET;
	else
		return SET;
}

// Clearing of the status flag/Сброс флага статуса
void HAL_GTIMER_ClearFlag( GTIMER_type* GTIMERx, uint32_t Flags )
{
	GTIMERx->STATUS.word &= ~Flags;
}

// Configuration of DMA request source/ Настройка источника запросов для DMA
void HAL_GTIMER_DMACmd( GTIMER_type* GTIMERx, uint32_t GTIMER_DMASource, GTIMER_DMA_REQ_type GTIMER_DMA_Request, FunctionalState_type NewState )
{
	if( GTIMER_DMA_Request == GTIMER_DMA_REQ )
	{
		if( NewState != DISABLE )
			GTIMERx->DMA_RE.word |= GTIMER_DMASource;
		else
			GTIMERx->DMA_RE.word &= ~GTIMER_DMASource;
	}
	else
	{
		if( NewState != DISABLE )
			GTIMERx->DMA_REChx[ GTIMER_DMA_Request ] |= GTIMER_DMASource;
		else
			GTIMERx->DMA_REChx[ GTIMER_DMA_Request ] &= ~GTIMER_DMASource;
	}
}

// Configuration of GTIMER interrupt/ Настройка прерываний от таймера
void HAL_GTIMER_ITConfig( GTIMER_type* GTIMERx, uint32_t GTIMER_IT, FunctionalState_type NewState )
{
	if( NewState != DISABLE )
		GTIMERx->IE.word |= GTIMER_IT;
	else
		GTIMERx->IE.word &= ~GTIMER_IT;
}

// Get interrupt status/Получение статуса прерывания
State_type HAL_GTIMER_GetITStatus( GTIMER_type* GTIMERx, uint32_t GTIMER_IT )
{
	if( GTIMERx->STATUS.word & GTIMERx->IE.word & GTIMER_IT )
		return SET;
	else
		return RESET;
}
