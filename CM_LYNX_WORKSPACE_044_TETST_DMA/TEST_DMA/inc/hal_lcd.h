/*
 *
 *	Файл: 		hal_lcd.h
 *	Описание: 	HAL для контроллера LCD панели
 *
 *	История:
 *				20-Jun-2017 Dmitriy Sharaviev 	- 	изменен под вторую ревизию
 *				20-Mar-2017 Dmitriy Sharaviev 	- 	создан
 *
 */

#ifndef	__HAL_LCD_H__
#define	__HAL_LCD_H__

#include <stdint.h>
#include "hal_1967VN044.h"

/* ----------------------------------------- Типы регистров ------------------------------------- */

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t VEN 		:1;																	// Разрешение работы контроллера
			uint32_t VIE 		:1;																	// Разрешение прерывания после окончания фрейма
			uint32_t HIE 		:1;																	// Разрешение прерывания после окончания одной линии
			uint32_t VBIE 		:1;																	// Разрешение прерывания после загрузки всего видеобанка данных в выходной буфер
			uint32_t SLPIE 		:1;																	// Разрешение прерывания после завершения сна
			uint32_t RESERVED1 	:1;																	//
			uint32_t XRQEN 		:1;																	// Разрешение прерывания после завершения чтения DMA из внешней памяти
			uint32_t VBL 		:2;																	// Количеств бит в пикселе
			uint32_t CD 		:2;																	// Количество бит для одного пикселя входной информации
			uint32_t RESERVED2 	:1;																	//
			uint32_t HLDM 		:1;																	// Останов повторной загрузки видеоданных после завершения выгрузки видеобуфера
			uint32_t HLDV 		:1;																	// Останов повторной генерации управляющих сигналов панели после завершения выгрузки видеобуфера
			uint32_t RESERVED3 	:1;																	//
			uint32_t BL 		:1;																	// Активный уровень линии готовности данных DRDY (0 – высокий, 1 – низкий)
			uint32_t RESERVED4 	:1;																	//
			uint32_t VBGR 		:1;																	// 1 – BGR режим, 0 – RGB режим
			uint32_t RESERVED5 	:2;																	//
			uint32_t SLP_MODE 	:1;																	// 1 – спящий режим после выгрузки видеобуфера
			uint32_t SLP_PCLK 	:1;																	// Стоп делитель пиксель клока в спящем режиме
			uint32_t SPL_PXEN 	:1;																	// Стоп разрешения работы пиксель клока в спящем режиме
			uint32_t SPL_HOLD 	:1;																	// Останов повторной генерации управляющих сигналов панели после завершения выгрузки видеобуфера (1 – разрешено)
			uint32_t SPL_CLRF 	:1;																	// 1 – автоматический сброс флагов после задержки спящего режима, 0 – ожидание программного обслуживания интерфейса
			uint32_t DMA_2QW 	:1;																	// Разрешение режима загрузки двух квадрослов за одну транзакцию DMA
			uint32_t DMA_EXT_EN :1;																	// Разрешение режима DMA с внешней памятью без участия системного контроллера DMA
			uint32_t DMA_IRQ_WT :1;																	// Разрешение ожидание обработки прерывания
			uint32_t W2W_EN 	:1;																	// Разрешение использования регистров дополнительной активной области
			uint32_t PXP_EN 	:1;																	// Разрешение останова пиксельклока в случае если в выходном FIFO нет данных
			uint32_t RESERVED7 	:2;																	//
		} b;
	};
} LCD_CTRL_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t RESERVED1 	:1;																	//
			uint32_t LUINT 		:1;																	// Флаг прерывания при ошибке чтения выходного буфера
			uint32_t STA_SLEEP 	:1;																	// Таймер сна начал отсчет. Устанавливается в момент перехода контроллера в режим сна
			uint32_t FIN_SLEEP 	:1;																	// Таймер сна закончил отсчет. Устанавливается в момент завершения режима сна
			uint32_t VINT 		:1;																	// Флаг завершения выдачи фрэйма
			uint32_t HINT 		:1;																	// Флаг завершения выдачи линии
			uint32_t VBSINT 	:1;																	// Флаг завершения чтения данных видеобуфера
			uint32_t DMA_END 	:1;																	// Флаг устанавливается в 1, если работал встроенный DMA канал внутри ЛСД контроллера и он отправил запрос на чтение последнего квадрослова данных
			uint32_t RESERVED2 	:12;																//
			uint32_t SLEEP_EXE 	:1;																	//	Режим сна (если 1). Только чтение
			uint32_t RESERVED3 	:11;																//
		} b;
	};
} LCD_STATUS_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t HPS 		:10;																// FPLINE начальная позиция (hps+1)
			uint32_t RESERVED1 	:5;																	//
			uint32_t HPL 		:1;																	// FPLINE активный уровень (0 – высокий, 1 – низкий)
			uint32_t HPW 		:10;																// FPLINE конечная позиция (hpw+1)
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_HTIM_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t VPS 		:10;																// FPFRAME начальная позиция
			uint32_t RESERVED1 	:5;																	//
			uint32_t VPL 		:1;																	// FPFRAME активный уровень (0 – высокий, 1 – низкий)
			uint32_t VPW 		:10;																// FPFRAME конечная позиция (vpw+1)
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_VTIM_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t HT 		:7;																	// Размер по горизонтали ((ht + 1) * 8)
			uint32_t RESERVED1 	:9;																	//
			uint32_t VT 		:10;																// Размер по вертикали (vt + 1)
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_HVLEN_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t VSIZE 		:20;																// Размер видеобуфера в 32-бит словах + 1
			uint32_t RESERVED1 	:12;																//
		} b;
	};
} LCD_VSIZE_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t P_DIV 		:8;																	// Значение делителя
			uint32_t EN_DIV 	:1;																	// Включение делителя
			uint32_t RESERVED1 	:23;																//
		} b;
	};
} LCD_PXDV_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t HDPS 		:10;																// Начальная позиция активной области (HDPS+1)
			uint32_t RESERVED1 	:6;																	//
			uint32_t HDPE 		:10;																// Конечная позиция активной области (HDPE+1)
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_HDTIM_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t VDPS 		:10;																// Номер начальной отображаемой линии
			uint32_t RESERVED1 	:6;																	//
			uint32_t VDPE 		:10;																// Номер конечной отображаемой линии
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_VDTIM_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t HXDPS 		:10;																// Начальная позиция активной области (HХDPS+1)
			uint32_t RESERVED1 	:6;																	//
			uint32_t HXDPE 		:10;																// Конечная позиция активной области (HХDPE+1)
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_HDxTIM_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t VXDPS 		:10;																// Номер начальной отображаемой линии
			uint32_t RESERVED1 	:6;																	//
			uint32_t VXDPE 		:10;																// Номер конечной отображаемой линии
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_VDxTIM_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t RESERVED1 	:6;																	//
			uint32_t FPSHI 		:1;																	// Инверсия пиксель клока
			uint32_t RESERVED2 	:2;																	//
			uint32_t DVI_MODE 	:1;																	// При записи всегда должен быть 0
			uint32_t DVI_CLK 	:1;																	// При записи всегда должен быть 0
			uint32_t RESERVED3 	:21;																//
		} b;
	};
} LCD_PANEL_CFG_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t CLKEN 		:1;																	// Разрешение работы
			uint32_t RESERVED1 	:2;																	// Должен быть 0
			uint32_t FRSH 		:1;																	// Когда равен 1, на выходе PWM постоянный высокий уровень вне зависимости от работы внутренних счетчиков
			uint32_t PWM_Dv 	:4;																	// Делитель
			uint32_t DUTY 		:8;																	// Длительность активного уровня.
			uint32_t RELOAD 	:8;																	// 8-бит верхнее значение. Когда счетчик достигнет это значение, он загрузит ноль и начнет отсчет заново.
			uint32_t RESERVED2 	:8;																	//
		} b;
	};
} LCD_PWM_CR_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t GPO_ST 	:10;																// Начальная позиция (+1)
			uint32_t RESERVED1	:5;																	// Активный уровень (0 – высокий, 1 – низкий)
			uint32_t HPL 		:1;																	//
			uint32_t GPO_SP 	:10;																// Конечная позиция (+1)
			uint32_t RESERVED2 	:6;																	//
		} b;
	};
} LCD_GPIO_type;

/* ------------------------------------ Структура периферии ------------------------------------- */

typedef volatile struct
{
	LCD_CTRL_type CTRL;					 															// Регистр управления
	LCD_STATUS_type STATUS;																			// Регистр состояния
	LCD_HTIM_type HTIM;																				// Fpline (старт, стоп)
	LCD_VTIM_type VTIM;																				// Fpframe (старт, стоп)
	LCD_HVLEN_type HVLEN;																			// Размер экрана
	LCD_HDxTIM_type HDxTIM;																			// Горизонтальная выдача данных (старт, стоп). Дополнительное окно
	LCD_VDxTIM_type VDxTIM;																			// Вертикальная выдача данных (старт, стоп). Дополнительное окно
	LCD_VSIZE_type VSIZE;																			// Размер видеобуфера
	uint32_t FON;																					// Данные для заполнения фоновой области
	LCD_PXDV_type PXDV;																				// Делитель пиксель клока
	LCD_HDTIM_type HDTIM;																			// Горизонтальная выдача данных (старт, стоп). Основное окно
	LCD_VDTIM_type VDTIM;																			// Вертикальная выдача данных (старт, стоп). Основное окно
	LCD_PANEL_CFG_type PANEL_CFG;																	// Регистр конфигурации панели
	LCD_PWM_CR_type PWM_CR;																			// Регистр управления ШИМ
	uint32_t SLP_PERIOD;																			// Регистр задания интервала сна
	uint32_t RESERVED;																				// Резерв
	LCD_GPIO_type GPIO_0;																			// Управление сигналом GPIO_0
	LCD_GPIO_type GPIO_1;																			// Управление сигналом GPIO_1
	LCD_GPIO_type GPIO_2;																			// Управление сигналом GPIO_2
	LCD_GPIO_type GPIO_3;																			// Управление сигналом GPIO_3
	uint32_t EXT_MEM_ADDR;																			// Начальный адрес внешней памяти для выделенного канала ПДП
} LCD_type;

#define LX_LCD 		( ( volatile LCD_type * ) base_LCD )

/* -------------------------------------- Специфические типы ------------------------------------ */
typedef enum
{
	LCD_Gpio_0 = 0,
	LCD_Gpio_1,
	LCD_Gpio_2,
	LCD_Gpio_3
} LCD_ExGpioNum_type;

typedef struct
{
	uint32_t StartPosition;																			// Начальная позиция
	uint32_t StopPosition;																			// Конечная позиция
	uint32_t ActiveLevel;																			// Активный уровень ( 1 - высокий, 0 - низкий )
} LCD_ExGpio_type;

typedef struct
{
	uint16_t usHSize;																				// Размер окна
	uint16_t usVSize;
	int16_t sHOffset;																				// Смещение относительно области заданной usHSize и usVSize в основной конфигурационной структуре
	int16_t sVOffset;
	uint32_t ulBackgndColor;																		// Цвет фона
} LCD_WindowConf_type;

typedef struct
{
	uint32_t ulRgbMode;																				// Формат цвета
	uint32_t ulPixelClock;																			// Частота синхроимпулься для LCD панели, кГц
	uint32_t ulPwmDiv;																				// Частота PWM = Fcore / (2 * HAL_LCD_MAX_PWM_DUTY * 2^pwm_div)
	uint8_t bVSyncActiveLevel;																	// Активный уровень VSYNC (0 - низкий, 1 - высокий)
	uint8_t bHSyncActiveLevel;																	// Активный уровень HSYNC (0 - низкий, 1 - высокий)
	uint8_t bDReadyActiveLevel;																	// Активный уровень линии готовности данных
	uint8_t bPClkInverse;																		// Инверсия пиксель клока
	uint8_t bStopPclkWhenNoData;																// Останов пиксельклока, когда нет данных
	uint8_t bSwapRgbToBgr;
	uint16_t usHSyncPos;																			// Единицы - период пиксельклока
	uint16_t usHSyncLen;																			//
	uint16_t usHSize;																				//
	uint16_t usHFrontBlank;																			//
	uint16_t usHBackBlank;																			//

	uint16_t usVSyncPos;																			// Единицы - линии
	uint16_t usVSyncLen;																			//
	uint16_t usVSize;																				//
	uint16_t usVFrontBlank;																			//
	uint16_t usVBackBlank;																			//

	LCD_WindowConf_type *pxWindow;																	// Установить в 0 если не используется оконный режим
} LCD_Conf_type;

#define LCD_WINDOW_BCKGRND_FROM_RGB(red, green, blue) 	( ((red & 0xFF)<<16) | ((green & 0xFF) << 8) | ((blue & 0xFF) << 0) )

/* Режимы цветов
                    R            G            B
 	RGB888:    data[23:16]   data[15:8]   data[7:0]
 	RGB565:    data[15:11]   data[10:5]   data[4:0]
 	RGB444_AR: data[13:10]   data[8:5]    data[3:0]
	RGB444_AL: data[14:11]   data[9:6]    data[4:1]
	RGB233:    data[7:6]     data[5:3]    data[2:0]
*/
//                   			[buffer size div]  [3:2] = CD   [1:0] = VBL
#define	HAL_LCD_RGB888 				((1 << 4) | (0x3 << 2) | (0x0 << 0))
#define	HAL_LCD_RGB565 				((2 << 4) | (0x1 << 2) | (0x0 << 0))
#define	HAL_LCD_RGB444_AR 			((2 << 4) | (0x1 << 2) | (0x2 << 0))
#define	HAL_LCD_RGB444_AL			((2 << 4) | (0x1 << 2) | (0x3 << 0))
#define	HAL_LCD_RGB233				((4 << 4) | (0x0 << 2) | (0x0 << 0))

#define HAL_LCD_MAX_PWM_DUTY		100																//

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
	void HAL_LCD_Enable			( void );
	void HAL_LCD_Disable		( void );

	void HAL_LCD_GpioInit		( void );
	void HAL_LCD_GpioDeInit		( void );
	void HAL_LCD_ExGpioInit		( LCD_ExGpioNum_type eGpio, LCD_ExGpio_type *pxExGpioCfg );
	void HAL_LCD_ExGpioDeInit	( LCD_ExGpioNum_type eGpio );
	void HAL_LCD_Setup			( LCD_Conf_type *pxCfg );
	void HAL_LCD_PwmSetDuty		( uint8_t ucValue );
	void HAL_LCD_StartDma		( uint32_t ulChannel, void *pvLcdBuf, uint32_t ulXSize, uint32_t ulYSize, uint32_t ulRgbMode, void *pvDmaIsrHandler );
	void HAL_LCD_StartDma2Q		( uint32_t ulChannel, void *pvLcdBuf, uint32_t ulXSize, uint32_t ulYSize, uint32_t ulRgbMode, void *pvDmaIsrHandler );
//	void HAL_LCD_StartIntDma	( void *pvLcdBuf );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif	//__HAL_LCD_H__

