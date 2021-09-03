/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	Файл: 		hal_cmu.h
 *	Описание: 	HAL для модуля управления синхронизацией и энергопотреблением
 *
 *	История:
 *	 			19-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *						    Zatonsky Pavel 		- 	создан
 *
 */
#ifndef __HAL_CMU_H__
#define __HAL_CMU_H__

#include "def1967VN044.h"

#define TMR0_DIS    GTMR0_DIS		//because renamed in spec from TMR to GTMR in CFG1 and CFG8
#define TMR1_DIS    GTMR1_DIS		//because renamed in spec from TMR to GTMR in CFG1 and CFG8
#define TMR0_EN     GTMR0_EN		//because renamed in spec from TMR to GTMR in CFG1 and CFG8
#define TMR1_EN     GTMR1_EN		//because renamed in spec from TMR to GTMR in CFG1 and CFG8

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RSV0 			:2;
		uint32_t LINK_BW 		:1;
		uint32_t SYS_WE 		:1;
		uint32_t ARINC_T0_EN 	:1;
		uint32_t ARINC_T1_EN 	:1;
		uint32_t ARINC_T2_EN 	:1;
		uint32_t ARINC_T3_EN 	:1;
		uint32_t MIL_DIS 		:1;
		uint32_t RSV1 			:3;
		uint32_t DMA_HP_EN 		:1;
		uint32_t H_OFF 			:1;
		uint32_t SPI1_EN 		:1;
		uint32_t SPI2_EN 		:1;
		uint32_t I2C_ALT 		:1;
		uint32_t NAND_ALT 		:2;
		uint32_t GTMR0_EN 		:1;
		uint32_t GTMR1_EN 		:1;
		uint32_t RSV2 			:11;
	} b;
} CMU_CFG1_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t DIVR 			:4;
		uint32_t DIVF 			:7;
		uint32_t DIVQ 			:3;
		uint32_t RANGE 			:3;
		uint32_t BYPASS 		:1;
		uint32_t S 				:2;
		uint32_t RSV 			:12;
	} b;
} CMU_CFG_PLL_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t CPLL_SEL 		:1;
		uint32_t BPLL_SEL 		:1;
		uint32_t DIS_CC 		:1;
		uint32_t DIS_BC 		:1;
		uint32_t BCLK_SEL 		:2;
		uint32_t RSV0 			:1;
		uint32_t LPLL_SEL 		:1;
		uint32_t RSV1 			:24;
	} b;
} CMU_CFG4_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t BOOT_0 		:1;
		uint32_t BOOT_1 		:1;
		uint32_t BOOT_2 		:1;
		uint32_t RSV0 			:1;
		uint32_t L0_BCMPO 		:1;
		uint32_t L1_BCMPO 		:1;
		uint32_t RST 			:1;
		uint32_t POR 			:1;
		uint32_t RSV1 			:24;
	} b;
} CMU_SYS_STS_type;

typedef union
{
	uint32_t word;
	struct
	{
		uint32_t I2S0_DIS 		:1;
		uint32_t I2S1_DIS 		:1;
		uint32_t VCAM_DIS 		:1;
		uint32_t SPI0_DIS 		:1;
		uint32_t NAND_DIS 		:1;
		uint32_t ARINC_DIS 		:1;
		uint32_t MIL0_DIS 		:1;
		uint32_t MIL1_DIS 		:1;
		uint32_t UDDC0_DIS 		:1;
		uint32_t UDDC1_DIS 		:1;
		uint32_t UDDC2_DIS 		:1;
		uint32_t UDDC3_DIS 		:1;
		uint32_t GPS0_DIS 		:1;
		uint32_t GPS1_DIS 		:1;
		uint32_t LCD_DIS 		:1;
		uint32_t UART0_DIS 		:1;
		uint32_t UART1_DIS 		:1;
		uint32_t SPI1_DIS 		:1;
		uint32_t SPI2_DIS 		:1;
		uint32_t GTMR0_DIS 		:1;
		uint32_t GTMR1_DIS 		:1;
		uint32_t I2C_DIS 		:1;
		uint32_t RSV 			:10;
	} b;
} CMU_CFG8_type;

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef volatile struct
{
	CMU_CFG1_type CFG1;
	CMU_CFG_PLL_type CFG2;
	CMU_CFG_PLL_type CFG3;
	CMU_CFG4_type CFG4;
	CMU_CFG_PLL_type CFG5;
	CMU_SYS_STS_type SYS_STS;
	uint32_t RSV1;
	uint32_t RSV2;
	CMU_CFG8_type CFG8;
} CMU_type;

#define LX_CMU			( ( volatile CMU_type	* ) base_CMU )
#define CFG_CPU_PLL		CFG2
#define CFG_BUS_PLL		CFG3
#define CFG_LINK_PLL	CFG5
#define CFG_CLKDIS		CFG8

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_CMU_H__ */

