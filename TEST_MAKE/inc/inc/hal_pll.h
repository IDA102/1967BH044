/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_pll.h
 *	Описание: 	HAL для модуля PLL
 *
 *	История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию, добавлен макрос определяющий частоту внешнего источника тактирования
 *				08-May-2015 Andrei Abramchuk 	- 	создан
 *
 */
#ifndef __HAL_PLL_H__
#define __HAL_PLL_H__

#include <stdint.h>

#include "1967VN044_HAL_config.h"

#define XTI_KHZ					48000//25000

/* -------------------------------------- специфические типы ------------------------------------ */
typedef struct
{
	uint32_t ref_freq_khz;
	uint32_t pll_freq_khz;
	union
	{
		uint32_t value;
		struct
		{
			uint32_t divr 	:4;
			uint32_t divf 	:7;
			uint32_t divq 	:3;
			uint32_t range 	:3;
			uint32_t ivco 	:3;
			uint32_t bypass :1;
			uint32_t res	:11;
		};
	};
} PLL_Config_type;

typedef struct
{
	int32_t CoreClk;																				// частота процессорного ядра, кГц
	int32_t SocClk;																					// частота шины периферийных устройств, кГц
	int32_t BusClk;																					// частота интерфейса внешней памяти, кГц
	int32_t LinkClk;																				// частота портов связи, кГц
} PLL_Freq_type;

extern volatile PLL_Freq_type PLL_Freq;

#ifdef __cplusplus
extern "C"
{
#endif
	int32_t	HAL_PLL_ConfigCalc				( PLL_Config_type* pll_cfg, uint32_t ref_freq_khz, uint32_t pll_freq_khz );
	void 	HAL_PLL_SleepOn					( void );

	void 	HAL_PLL_CoreSwitch				( PLL_Config_type* pll_cfg );
	void 	HAL_PLL_LinkSwitch				( PLL_Config_type* pll_cfg );
	void 	HAL_PLL_BusSwitch				( PLL_Config_type* pll_cfg );

	void 	HAL_PLL_CoreSetup				( uint32_t pll_freq );
	void 	HAL_PLL_LinkSetup				( uint32_t pll_freq );
	void 	HAL_PLL_BusSetup				( uint32_t pll_freq );

	void 	HAL_XTI_CoreSwitch				( void );
	void 	HAL_XTI_BusSwitch				( void );
	void 	HAL_XTI_LinkSwitch				( void );

	void 	HAL_PLL_BusSwitchHalfCore		( void );
	void 	HAL_PLL_BusSwitchFourthCore		( void );
	void 	HAL_PLL_BusSwitchEighthCore		( void );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_PLL_H__ */
