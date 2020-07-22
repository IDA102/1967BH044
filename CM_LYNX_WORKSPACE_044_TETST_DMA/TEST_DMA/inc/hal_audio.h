/*
 *
 *	Файл: 		hal_audio.h
 *	Описание: 	HAL для интерфейса к аудио-кодеку AC97/I2S
 *
 *	История:
 *				21-Mar-2017 Dmitry Sharavev - создан
 *
 */


#ifndef __HAL_AUDIO_H__
#define __HAL_AUDIO_H__

#include "def1967VN044.h"
#include <stdint.h>

/* ----------------------------------------- Типы регистров ------------------------------------- */
// Регистр SICR0:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t ENB		:1;				// Включение режима AC97. Включен (1) или выключен (0)
		uint32_t AC97DIS	:1;				// Для включения режима АС97 должен быть равен 0. Если равен 1 – запрещает включение режима АС97.
		uint32_t MODEN		:1;				// Модемный кодек присутствует (1)
		uint32_t AMCCH		:1;				// АМС кодек присутствует (1)
		uint32_t RST		:1;				// Запись 1 вызывает сброс внешнего кодека
		uint32_t LPBK		:1;				// Тестовый режим АС97 (когда равен 1)
		uint32_t BCKD		:1;				// Источник синхронизации. 0 – используется внешний источник синхронизации,	1 – используется внутренний источник (биты делителя REQLP)
		uint32_t MONO_DA	:1;				// 0 – стерео выход, 1 – моно выход
		uint32_t BIT8_DA	:1;				// 0 – 16 бит выход, 1 – 8 бит выход
		uint32_t MONO_AD	:1;				// 0 – стерео вход, 1 – моно вход
		uint32_t BIT8_AD	:1;				// 0 – 16 бит вход, 1 – 8 бит вход
		uint32_t TO_EN		:1;				// Разрешение детектирования отсутствия клока аудиокодека. Если установлен в 1, осуществляется проверка отсутствия синхронизации на входе BITCLK (либо частота очень низкая).
		uint32_t DIV_EN		:1;				// Разрешение делителя клока АС97. Когда равен 1 разрешает включение внутреннего делителя клока. Значение делителя задается полем REQLP.
	} b;
} Audio_SICR0_type;

// Регистр SICR2:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t EREC		:1;				// Разрешение записи (1)
		uint32_t ERPL		:1;				// Разрешение воспроизведения (1)
		uint32_t EINC		:1;				// Разрешение приема модемных данных (1)
		uint32_t EOUT		:1;				// Разрешение передачи модемных данных (1)
		uint32_t EGPIO		:1;				// Разрешение приема GPIO данных (1)
		uint32_t WKUP		:1;				// Запись 1 вызывает инициализацию процедуры пробуждения кодека.
		uint32_t DRSTO		:1;				// Если 1 – выключает AC-link status read time-out function
		uint32_t REQLP		:16;			// Коэффициент деления для формирования синхросигнала в режиме I2S и АС97
	} b;
} Audio_SICR2_type;

// Регистр SICR3:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t COMSEL		:1;				// Разрешение вторичного кодека (1)
		uint32_t C2ID		:2;				// ID вторичного кодека
	} b;
} Audio_SICR3_type;

// Регистр SISR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t DTD		:1;				// Завершение передачи данных
		uint32_t RDD		:1;				// Завершение чтения данных
		uint32_t GTD		:1;				// Завершение передачи GPIO данных
		uint32_t RES		:1;				//
		uint32_t BSY		:1;				// Занят
		uint32_t ATNE		:1;				// Буфер передатчика аудио данных не пуст
		uint32_t ATNF		:1;				// Буфер передатчика аудио данных заполнен не полностью
		uint32_t ARNE		:1;				// Буфер приемника аудио данных не пуст.
		uint32_t ATFS		:1;				// Запрос буфера передатчика аудио данных.
		uint32_t ARFS		:1;				// Запрос буфера приемника аудио данных
		uint32_t ATUR		:1;				// Отсутствие аудио данных для передачи
		uint32_t AROR		:1;				// Переполнение буфера приемника аудио данных
		uint32_t MTNF		:1;				// Буфер передатчика модема не заполнен
		uint32_t MRNE		:1;				// Буфер приемника модема не пуст
		uint32_t MTFS		:1;				// Запрос от буфера передатчика модема
		uint32_t MRFS		:1;				// Запрос от буфера приемника модема
		uint32_t MTUR		:1;				// Отсутствие данных в буфере передатчика модема
		uint32_t MROR		:1;				// Переполнение буфера приемника модема
		uint32_t RSTO		:1;				// Флаг состояния «тайм-аут»
		uint32_t CLPM		:1;				// Флаг отсутствия частоты синхронизации
		uint32_t CRDYPR		:1;				// Первичный кодек готов.
		uint32_t CRDYSC		:1;				// Вторичный кодек готов.
		uint32_t RESU		:1;				// Возобновление.
		uint32_t GINT		:1;				// Флаг изменения состояния GPIO.
		uint32_t RS3V		:1;				// Слот 3 принятых данных достоверен
		uint32_t RS4V		:1;				// Слот 4 принятых данных достоверен
		uint32_t RS5V		:1;				// Слот 5 принятых данных достоверен
		uint32_t RS12V		:1;				// Слот 12 принятых данных достоверен
	} b;
} Audio_SISR_type;

// Регистр SIIER/SIIDR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t DTD		:1;				// Завершение передачи данных АС97
		uint32_t RDD		:1;				// Завершение приема данных АС97
		uint32_t GTD		:1;				// Завершение передачи данных GPIO
		uint32_t RES0		:5;				//
		uint32_t ATFS		:1;				// Запрос буфера передатчика аудио данных
		uint32_t ARFS		:1;				// Запрос буфера приемника аудио данных
		uint32_t ATUR		:1;				// Ошибка в буфере передатчика аудио данных (отсутствие данных в момент передачи)
		uint32_t AROR		:1;				// Переполнение буфера приемника аудио данных
		uint32_t RES1		:2;				//
		uint32_t MTFS		:1;				// Запрос буфера передатчика модемных данных
		uint32_t MRFS		:1;				// Запрос буфера приемника модемных данных
		uint32_t MTUR		:1;				// Ошибка в буфере передатчика модемных данных (отсутствие данных в момент передачи)
		uint32_t MROR		:1;				// Переполнение буфера приемника модемных данных
		uint32_t RSTO		:1;				// Флаг таймаута внешнего кодека
		uint32_t RES2		:3;				//
		uint32_t RESU		:1;				// Установка флага возобновления работы
		uint32_t GINT		:1;				// Изменение состояния GPIO
	} b;
} Audio_SIIERD_type;

// Регистр SIICR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t DTD		:1;				// Завершение передачи данных
		uint32_t RDD		:1;				// Завершение чтения данных
		uint32_t GTD		:1;				// Завершение передачи GPIO данных
		uint32_t RES0		:7;				//
		uint32_t ATUR		:1;				// Отсутствие аудио данных для передачи
		uint32_t AROR		:1;				// Переполнение буфера приемника аудио данных
		uint32_t RES1		:4;				//
		uint32_t MTUR		:1;				// Отсутствие данных в буфере передатчика модема
		uint32_t MROR		:1;				// Переполнение буфера приемника модема
		uint32_t RSTO		:1;				// Флаг состояния «тайм-аут»
		uint32_t RES2		:3;				//
		uint32_t RESU		:1;				// Возобновление.
		uint32_t GINT		:1;				// Флаг изменения состояния GPIO.
	} b;
} Audio_SIICR_type;

// Регистр ACCAR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RES		:12;			//
		uint32_t IX			:7;				// Индекс управляющего регистра кодека. Бит 12 всегда должен быть равен 1
		uint32_t RW			:1;				// Бит чтения (1) или записи (0)
	} b;
} Audio_ACCAR_type;

// Регистр ACCDR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RES		:4;				//
		uint32_t CDR		:16;			// 16 бит записываемые данные
	} b;
} Audio_ACCDR_type;

// Регистр ACSAR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RES		:12;			//
		uint32_t SAR		:7;				// Эхо индекса управляющего регистра кодека, данные которого были прочитаны
	} b;
} Audio_ACSAR_type;

// Регистр ACSDR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RES		:4;				//
		uint32_t SDR		:16;			// 16 бит данных, прочитанных из кодека
	} b;
} Audio_ACSDR_type;

// Регистр ACGDR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RES		:4;				//
		uint32_t GDAT		:16;			// 16 бит данных, которые будут записаны в GPIO кодека
	} b;
} Audio_ACGDR_type;

// Регистр ACGSR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t RES		:4;				//
		uint32_t SDR		:16;			// 16 бит GPIO данных, прочитанных из кодека
	} b;
} Audio_ACGSR_type;

// Регистр I2S_T_CR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t TEN		:1;				// Разрешение работы (1)
		uint32_t MODE		:1;				// 0 – I2S режим, 1 – DSP режим
		uint32_t SONY		:1;				// Выбор стандарта I2S : SONY(1) или Philips (0). Имеет смысл только в режиме I2S
		uint32_t MS			:1;				// Master (1) or slave (0)
		uint32_t DSS		:6;				// Длина передаваемых данных – от 1 до 64 бит. Значение в битах DSS всегда на 1 меньше количества бит передаваемых данных
		uint32_t PNIS		:1;				// Определяет момент приема выбора канала: 0 – прием по положительному фронту (из 0 в 1), 1 – прием по отрицательному фронту
		uint32_t PNOS		:1;				// Определяет момент выдачи данных и выбора канала: 0 – выдача по положительному фронту (из 0 в 1), 1 – выдача по отрицательному фронту
		uint32_t SWHW		:1;				// 1 – указывает на необходимость перестановки 16-разрядных полуслов в одном слове
		uint32_t PACKH		:1;				// Имеет смысл при длине передаваемых данных 16 бит и меньше. 1 – указывает на то, что в одном 32-разрядном слове упакованы два значения. Иначе в одном слове хранится одно значение.
		uint32_t LRSP		:1;				// 1 и установлен DSP- режим – указывает на то, что общее отсылаемое слово состоит из двух симметричных частей (левой и правой) которые размещаются в разных 32-бит словах
	} b;
} Audio_I2S_T_CR_type;

// Регистр I2S_R_CR:
typedef union
{
	uint32_t word;
	struct
	{
		uint32_t REN		:1;				// Разрешение работы (1)
		uint32_t MODE		:1;				// 0 – I2S режим, 1 – DSP режим
		uint32_t SONY		:1;				// Выбор стандарта I2S : SONY(1) или Philips (0). Имеет смысл только в режиме I2S
		uint32_t MS			:1;				// Master (1) or slave (0)
		uint32_t DSS		:6;				// Длина передаваемых данных – от 1 до 64 бит. Значение в битах DSS всегда на 1 меньше количества бит передаваемых данных
		uint32_t PNIS		:1;				// Определяет момент приема выбора канала: 0 – прием по положительному фронту (из 0 в 1), 1 – прием по отрицательному фронту
		uint32_t PNOS		:1;				// Определяет момент выдачи данных и выбора канала: 0 – выдача по положительному фронту (из 0 в 1), 1 – выдача по отрицательному фронту
		uint32_t SWHW		:1;				// 1 – указывает на необходимость перестановки 16-разрядных полуслов в одном слове
		uint32_t R_INIT		:1;				// В случае приема сообщения длина которого меньше 16 бит, либо больше 16 бит но меньше 32, возможна предварительная инициализация сдвигового регистра
		uint32_t LRSP		:1;				// 1 и установлен DSP- режим – указывает на то, что общее отсылаемое слово состоит из двух симметричных частей (левой и правой) которые размещаются в разных 32-бит словах
		uint32_t SEXT		:1;				// Если бит равен нулю, то регистр инициализируется нулями. Если бит равен 1, биты регистра инициализируются значением первого принимаемого бита (расширение знака)
	} b;
} Audio_I2S_R_CR_type;

/* ------------------------------------ Структура периферии ------------------------------------- */
typedef volatile struct
{
	Audio_SICR0_type   	SICR0;				// Регистр управления 0
	uint32_t           	SINT;				// Регистр запросов прерываний
	Audio_SICR2_type   	SICR2;				// Регистр управления 2
	Audio_SISR_type    	SISR;				// Регистр состояния после маскирования
	Audio_SISR_type    	SIRSR;				// Регистр состояния до маскирования
	Audio_SIIERD_type 	SIIER;				// Регистр разрешения прерывания (установка)
	Audio_SIIERD_type  	SIIDR;				// Регистр запрещения прерывания (сброс)
	Audio_SIICR_type   	SIICR;				// Регистр сброса флагов запросов прерывания
	uint32_t   		   	SIADR;				// Регистр-буфер аудио данных (FIFO восемь 32-битных слов)
	uint32_t   		   	SIMDR;				// Регистр-буфер модемных данных (FIFO восемь 16-битных слов)
	Audio_ACCAR_type   	ACCAR;				// AC канал, регистр адреса команды
	Audio_ACCDR_type   	ACCDR;				// AC канал, регистр данных команды
	Audio_ACSAR_type   	ACSAR;				// AC канал, регистр адреса состояния
	Audio_ACSDR_type   	ACSDR;				// AC канал, регистр данных состояния
	Audio_ACGDR_type   	ACGDR;				// AC канал, регистр данных GPIO
	Audio_ACGSR_type   	ACGSR;				// AC канал, регистр состояния GPIO
	Audio_I2S_T_CR_type I2S_T_CR;			// Регистр управления передатчиком I2S
	Audio_I2S_R_CR_type I2S_R_CR;			// Регистр управления приемником I2S
	uint32_t 			RES[ 10 ];			//
	Audio_SICR3_type   	SICR3;				// Регистр управления 3
} AUDIO_type;

#define LX_AUDIO0 			( ( volatile AUDIO_type * ) base_I2S0 )
#define LX_AUDIO1 			( ( volatile AUDIO_type * ) base_I2S1 )

/* -------------------------------------- Специфические типы ------------------------------------ */
#define ROLE_SLAVE			0
#define ROLE_MASTER			1
#define MODE_I2S			0
#define MODE_DSP			1
#define STANDART_PHILLIPS	0
#define STANDART_SONY		1

typedef struct
{
	uint8_t Role;							// 1 - ведущий, 0 - ведомый
	uint8_t Mode;							// 0 – I2S режим, 1 – DSP режим
	uint8_t Standart;						// 0 - Phillips, 1 - Sony
	uint8_t TfsPos;							// 0 - установка TFS по положительному фронту TCKL, 1 - по отрицательному фронту
	uint8_t SwapLR;							// Поменять местами каналы
	uint16_t DataLen;						// Длина данных
	uint32_t AudioFreq;						// Частота аудио потока, Гц
} AUDIO_I2S_type;

#define HAL_AUDIO_StartDMA HAL_AUDIO_StartDMA_TX // for backward compatibility

#ifdef __cplusplus
extern "C"
	{
#endif
	void HAL_AUDIO_I2SInit	( AUDIO_type* const AudioX, AUDIO_I2S_type* I2sConf );
	void HAL_AUDIO_StartDMA_TX	( uint32_t channel, AUDIO_type* const AudioX, void *pvDataBuf, uint16_t usSize, void* pvDmaIsrHandler );
	void HAL_AUDIO_StartDMA_RX	( uint32_t channel, AUDIO_type* const AudioX, void *pvDataBuf, uint16_t usSize, void* pvDmaIsrHandler );
	void HAL_AUDIO_StopDMA	( AUDIO_type* const AudioX );
	void HAL_AUDIO_StartDMA_TX_2BUF( uint32_t ulChannel, AUDIO_type* const AudioX, void *pvDataBufA, void *pvDataBufB, uint32_t usSize, void* pvDmaIsrHandler );
	void HAL_AUDIO_StartDMA_RX_2BUF( uint32_t ulChannel, AUDIO_type* const AudioX, void *pvDataBufA, void *pvDataBufB, uint32_t usSize, void* pvDmaIsrHandler );
#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_AUDIO_H__ */
