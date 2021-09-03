/**********************************************************************************************************************
  Copyright (C) 2016-2020 JSC "ICC Milandr" 

**********************************************************************************************************************/

/*
 *
 *	File/Файл: 		hal_lcd.c
 *	Description/Описание: 	HAL for the controller of LCD-pannel/HAL для контроллера LCD панели
 *
 *	History/История:
 *				20-Jun-2017 Dmitriy Sharaviev 	- 	changed for the second revision/изменен под вторую ревизию
 *				20-Mar-2017 Dmitriy Sharaviev 	- 	created/создан
 *
 */
#include "hal_1967VN044.h"

#define wa_delay()	asm("nop;; nop;; nop;; nop;;")

static __builtin_quad tcb;

void HAL_LCD_Enable( void )
{
	LX_LCD->CTRL.b.VEN = 1;
}

void HAL_LCD_Disable( void )
{
	LX_LCD->CTRL.b.VEN = 0;
}

void HAL_LCD_GpioInit( void )
{
	LX_GPIO_PB->ALT.SET = 0x7C3FFFF0;
}

void HAL_LCD_GpioDeInit( void )
{
	LX_GPIO_PB->ALT.CLR = 0x7C3FFFF0;
}

void HAL_LCD_ExGpioInit( LCD_ExGpioNum_type eGpio, LCD_ExGpio_type *pxExGpioCfg )
{
	uint32_t ulTemp;

	switch ( eGpio )
	{
		case LCD_Gpio_0:
			LX_GPIO_PB->ALT.SET = 0x00400000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_0.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_0.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_0.b.HPL = 0;
			else
				LX_LCD->GPIO_0.b.HPL = 1;
			break;
		case LCD_Gpio_1:
			LX_GPIO_PB->ALT.SET = 0x00800000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_1.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_1.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_1.b.HPL = 0;
			else
				LX_LCD->GPIO_1.b.HPL = 1;
			break;
		case LCD_Gpio_2:
			LX_GPIO_PB->ALT.SET = 0x01000000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_2.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_2.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_2.b.HPL = 0;
			else
				LX_LCD->GPIO_2.b.HPL = 1;
			break;
		case LCD_Gpio_3:
			LX_GPIO_PB->ALT.SET = 0x02000000;
			ulTemp = pxExGpioCfg->StartPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_3.b.GPO_ST = ulTemp & 0x03FF;
			ulTemp = pxExGpioCfg->StopPosition;
			if ( ulTemp > 0 ) ulTemp--;
			LX_LCD->GPIO_3.b.GPO_SP = ulTemp & 0x03FF;
			if ( pxExGpioCfg->ActiveLevel == 1 )
				LX_LCD->GPIO_3.b.HPL = 0;
			else
				LX_LCD->GPIO_3.b.HPL = 1;
			break;
	}
}

void HAL_LCD_ExGpioDeInit( LCD_ExGpioNum_type eGpio )
{
	switch( eGpio )
	{
		case LCD_Gpio_0:
			LX_GPIO_PB->ALT.CLR = 0x00400000;
			break;
		case LCD_Gpio_1:
			LX_GPIO_PB->ALT.CLR = 0x00800000;
			break;
		case LCD_Gpio_2:
			LX_GPIO_PB->ALT.CLR = 0x01000000;
			break;
		case LCD_Gpio_3:
			LX_GPIO_PB->ALT.CLR = 0x02000000;
			break;
	}
}

void HAL_LCD_Setup( LCD_Conf_type *pxCfg )
{
	uint32_t ulTemp;

	LX_LCD->CTRL.word = 0;																			// Turn off the controlle/Выключаем контроллер
	LX_LCD->PWM_CR.word = 0;

	LX_LCD->VSIZE.b.VSIZE = pxCfg->usHSize * pxCfg->usVSize / 										// Set the size of the video-buffer in 32-bit words/Задаем размер видеобуффера в 32-битных словах
			( ( pxCfg->ulRgbMode >> 4 ) & 0x0F );

	ulTemp = PLL_Freq.CoreClk / ( 2 * pxCfg->ulPixelClock );										// PX_CLK = SOC_CLK / (P_div + 1)
	if ( ulTemp > 0 ) ulTemp -= 1;
	LX_LCD->PXDV.b.P_DIV = ulTemp;
	wa_delay();
	LX_LCD->PXDV.b.EN_DIV = 1;

	// The number of pixels in the line and the general number of lines/Количество пикселей в линии и общее количество линий
	// Not all lines and pixels can be seen on the screen/Не все линии и пиксели видны на экране
	LX_LCD->HVLEN.b.HT = ( ( pxCfg->usHFrontBlank + pxCfg->usHSize + pxCfg->usHBackBlank ) >> 3 ) - 1;
	wa_delay();
	LX_LCD->HVLEN.b.VT = ( pxCfg->usVFrontBlank + pxCfg->usVSize + pxCfg->usVBackBlank ) - 1;

	// HSYNC (fpline) timings/тайминги
	LX_LCD->HTIM.b.HPS = pxCfg->usHSyncPos - 1;
	wa_delay();
	LX_LCD->HTIM.b.HPL = ~pxCfg->bHSyncActiveLevel;													// Active level: 0 - high; 1 - low/Активный уровень: 0 - высокий, 1 - низкий
	wa_delay();
	LX_LCD->HTIM.b.HPW = pxCfg->usHSyncPos + pxCfg->usHSyncLen - 1;

	// VSYNC (fpframe) timings/тайминги
	LX_LCD->VTIM.b.VPS = pxCfg->usVSyncPos - 1;
	wa_delay();
	LX_LCD->VTIM.b.VPL = ~pxCfg->bVSyncActiveLevel;													// Active level: 0 - high; 1 - low/Активный уровень: 0 - высокий, 1 - низкий
	wa_delay();
	LX_LCD->VTIM.b.VPW = pxCfg->usVSyncPos + pxCfg->usVSyncLen - 1;

	// Control of active horizontal area of the pannel/Управление горизонтальной активной областью панели
	LX_LCD->HDTIM.b.HDPS = pxCfg->usHFrontBlank - 1;
	wa_delay();
	LX_LCD->HDTIM.b.HDPE = pxCfg->usHFrontBlank + pxCfg->usHSize - 1;

	// Control of active vertical area of the pannel/Управление вертикальной активной областью панели
	LX_LCD->VDTIM.b.VDPS = pxCfg->usVFrontBlank - 1;
	wa_delay();
	LX_LCD->VDTIM.b.VDPE = pxCfg->usVFrontBlank + pxCfg->usVSize - 1;

	if ( pxCfg->pxWindow )
	{
		// visible horizontal area/ Видимая область по горизонтали
		LX_LCD->HDxTIM.b.HXDPS = pxCfg->usHFrontBlank + pxCfg->pxWindow->sHOffset - 1;
		wa_delay();
		LX_LCD->HDxTIM.b.HXDPE = pxCfg->usHFrontBlank + pxCfg->pxWindow->sHOffset + pxCfg->pxWindow->usHSize - 1;

		// visible vertical area/ Видимая область по вертикали
		LX_LCD->VDxTIM.b.VXDPS = pxCfg->usVFrontBlank + pxCfg->pxWindow->sVOffset - 1;
		wa_delay();
		LX_LCD->VDxTIM.b.VXDPE = pxCfg->usVFrontBlank + pxCfg->pxWindow->sVOffset + pxCfg->pxWindow->usVSize - 1;

		// backgound (for the window-mode)/Фон (для оконного режима)
		LX_LCD->FON = pxCfg->pxWindow->ulBackgndColor;
	}

	LX_LCD->PANEL_CFG.b.FPSHI = pxCfg->bPClkInverse;												// Inversion of the clock pixel/Инверсия пиксель клока

	// PWM for lightning/Настройка ШИМ подсветки
	LX_LCD->PWM_CR.b.RELOAD = HAL_LCD_MAX_PWM_DUTY;
	wa_delay();
	LX_LCD->PWM_CR.b.DUTY = 0;
	wa_delay();
	LX_LCD->PWM_CR.b.PWM_Dv = pxCfg->ulPwmDiv; 														// Divisor value/Значение делителя;
	wa_delay();
	LX_LCD->PWM_CR.b.CLKEN = 1;

	LX_LCD->CTRL.b.VBGR = pxCfg->bSwapRgbToBgr;														// The order of colours - BGR or RGB/Порядок следования цветов BGR или RGB
	wa_delay();
	LX_LCD->CTRL.b.CD = ( pxCfg->ulRgbMode >> 2 ) & 0x3;											// 0 - 8bit, 1 - 16bit, 2 - 24bit, 3 - 32bit in the pixel
	wa_delay();
	LX_LCD->CTRL.b.VBL = ( pxCfg->ulRgbMode >> 0 ) & 0x3;											// The number of bits in the pixel/Количеств бит в пикселе
	wa_delay();
	LX_LCD->CTRL.b.BL = ~pxCfg->bDReadyActiveLevel;													// Active level for data redyness 0 - DRDY active high; 1 - DRDY active low/ Активный уровень линии готовности данных (0 - DRDY активный высокий, 1 - DRDY активный низкий)
	wa_delay();
	LX_LCD->CTRL.b.W2W_EN = ( pxCfg->pxWindow != 0 );												// If 1 then use HDxTIM and VDxTIM registers/ Если 1, то используются регистры HDxTIM и VDxTIM
	wa_delay();
	LX_LCD->CTRL.b.PXP_EN = pxCfg->bStopPclkWhenNoData;												// If 1, then pixelclock can be stopped if output FIFO buffer has no data/ Если 1, разрешен останов пиксельклока в случае если в выходном FIFO нет данных
}

void HAL_LCD_PwmSetDuty( uint8_t ucValue )
{
	if ( ucValue >= HAL_LCD_MAX_PWM_DUTY ) 															// Elimenation of flicker when duty == reload/Устранение небольших мерцаний при значениях duty == reload
		ucValue = HAL_LCD_MAX_PWM_DUTY + 1;
	LX_LCD->PWM_CR.b.DUTY = ucValue;
}

void HAL_LCD_StartDma( uint32_t ulChannel, void *pvLcdBuf,
					uint32_t ulXSize, uint32_t ulYSize, uint32_t ulRgbMode,
					void *pvDmaIsrHandler )
{
	uint32_t *ptr = ( uint32_t* ) &tcb;

	if( ulChannel < 4 || ulChannel > 7 )
			ulChannel = 4;

	HAL_DMA_Stop( ulChannel );

	switch ( ( ulRgbMode >> 4 ) & 0x0F )
	{
		case 2:
			ulXSize >>= 1;																			// 2 pixels in 32-bit word/2 пикселя в 32-bit слове
			break;
		case 4:
			ulXSize >>= 2;																			// 4 pixels in 32-bit word/4 пикселя в 32-bit слове
			break;
		default:
			break;
	}

	*( ptr + 0 ) = ( uint32_t ) pvLcdBuf;															// Information source/Источник информации
	*( ptr + 1 ) = ( ulXSize << 16 ) | 4;															// The number of words and address increment/Количество слов + инкремент адреса
	*( ptr + 2 ) = ( ulYSize << 16 ) | 4;															// The number of X-transmission attempts+X-address increment/Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_TWODIM |																	// 2D sending mode enabling/Включение режима 2-х мерной пересылки
					TCB_QUAD |																		// The length of transmitted data (operand) in one exchange-cycle/Длина передаваемых данных (операнда) в одном цикле обмена
					TCB_DMAR;
	*( ptr + 3 ) |= ( ( uint32_t ) pvLcdBuf < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// The source is in the internal/external memory/Источник в внешней/внутренней памяти

	HAL_DMA_CreateChannelDest( ulChannel, &tcb, &tcb );
	HAL_DMA_RqstSet( ulChannel, dmaVIDEO );

	if ( pvDmaIsrHandler )																			// Setting of an interrupt if needed/Настройка прерывания, если оно нужно
	{
		switch( ulChannel )
		{
			case 4:
				HAL_Interrupt_Enable( intDMA4, pvDmaIsrHandler );
				break;
			case 5:
				HAL_Interrupt_Enable( intDMA5, pvDmaIsrHandler );
				break;
			case 6:
				HAL_Interrupt_Enable( intDMA6, pvDmaIsrHandler );
				break;
			case 7:
				HAL_Interrupt_Enable( intDMA7, pvDmaIsrHandler );
				break;
			default:
				break;
		}
		*( ptr + 3 ) |= TCB_INT;
	}

	HAL_DMA_WriteDC( ulChannel, &tcb );																// DMA start/Запуск DMA
}

void HAL_LCD_StartDma2Q( uint32_t ulChannel, void *pvLcdBuf,
					uint32_t ulXSize, uint32_t ulYSize, uint32_t ulRgbMode,
					void *pvDmaIsrHandler )
{
	uint32_t *ptr = ( uint32_t* ) &tcb;
	uint64_t DCA;

	DCA = 0x2000000000000180;

	if( ulChannel < 4 || ulChannel > 7 )
			ulChannel = 4;

	HAL_DMA_Stop( ulChannel );

	switch ( ( ulRgbMode >> 4 ) & 0x0F )
	{
		case 2:
			ulXSize >>= 1;																			// 2 pixels in 32-bit word/2 пикселя в 32-bit слове
			break;
		case 4:
			ulXSize >>= 2;																			// 4 pixels in 32-bit word/4 пикселя в 32-bit слове
			break;
		default:
			break;
	}

	*( ptr + 0 ) = ( uint32_t ) pvLcdBuf;															// Information source/Источник информации
	*( ptr + 1 ) = ( ulXSize << 16 ) | 4;															// The number of words and address increment/Количество слов + инкремент адреса
	*( ptr + 2 ) = ( ulYSize << 16 ) | 4;															// The number of X-transmission attempts+X-address increment/Количество попыток передачи по X + инкремент адреса X
	*( ptr + 3 ) = 	TCB_TWODIM |																	// 2D sending mode enabling/Включение режима 2-х мерной пересылки
					TCB_QUAD |																		// The length of transmitted data (operand) in one exchange-cycle/Длина передаваемых данных (операнда) в одном цикле обмена
					TCB_DMAR;
	*( ptr + 3 ) |= ( ( uint32_t ) pvLcdBuf < 0x0C000000 ) ? TCB_INTMEM : TCB_EXTMEM;				// The source is in the internal/external memory/Источник в внешней/внутренней памяти

	HAL_DMA_CreateChannelDest( ulChannel, &tcb, &tcb );
	HAL_DMA_RqstSet( ulChannel, dmaVIDEO );

	if ( pvDmaIsrHandler )																			// Setting of an interrupt if needed/Настройка прерывания, если оно нужно
	{
		switch( ulChannel )
		{
			case 4:
				HAL_Interrupt_Enable( intDMA4, pvDmaIsrHandler );
				break;
			case 5:
				HAL_Interrupt_Enable( intDMA5, pvDmaIsrHandler );
				break;
			case 6:
				HAL_Interrupt_Enable( intDMA6, pvDmaIsrHandler );
				break;
			case 7:
				HAL_Interrupt_Enable( intDMA7, pvDmaIsrHandler );
				break;
			default:
				break;
		}
		*( ptr + 3 ) |= TCB_INT;
	}

	LX_LCD->CTRL.b.DMA_2QW = 1;
	HAL_DMA_PrimaryPriority();
	HAL_DMA_WriteDCA( ulChannel, &DCA );
	HAL_DMA_WriteDC( ulChannel, &tcb );																// DMA Start/Запуск DMA
}

// This mode doesn't work at a moment/Этот режим пока не работает
//void HAL_LCD_StartIntDma( void *pvLcdBuf )
//{
//	LX_LCD->EXT_MEM_ADDR = ( uint32_t ) pvLcdBuf;
//	LX_LCD->CTRL.b.DMA_EXT_EN = 1;
//}
