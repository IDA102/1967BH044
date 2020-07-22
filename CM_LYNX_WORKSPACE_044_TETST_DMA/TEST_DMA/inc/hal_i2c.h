/*
 *
 *	Файл: 		hal_i2c.h
 *	Описание: 	HAL для интерфейса i2c
 *
 *	История:
 *	 			17-May-2017 Dmitry Sharavev		-	создан
 */

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

#include "hal_1967VN044.h"

/* ----------------------------------------- Типы регистров ------------------------------------- */
typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t ABRT 		:1;																	// Аварийное завершение работы интерфейса в режиме мастера
			uint32_t SLVE 		:1;																	// Завершение работы интерфейса в режиме подчиненного
			uint32_t AA 		:1;																	// Включение ACK и разрешение сравнения адреса при приеме
			uint32_t SI 		:1;																	// Запрос прерывания
			uint32_t STO 		:1;																	// Формирование STOP
			uint32_t STA 		:1;																	// Формирование START (сам не сбрасывается)
			uint32_t I2C_ON 	:1;																	// Разрешение работы i2c
			uint32_t M10 		:1;																	// Включение дополнительного адреса AXR
			uint32_t SLVE_EN 	:1;																	// Включение прерывания при обнаружении STOP в режиме подчиненного
			uint32_t ABN_EN 	:1;																	// Разрешение детектирования конфликтов в мультимастерной системе
			uint32_t TLOW_EN 	:1;																	// Разрешение контролировать параметр T_low при выполнении мастером процедуры “restart”
			uint32_t IRQ 		:1;																	// Запрос прерывания (сбрасывать при входе в прерывание)
			uint32_t FREE_EN 	:1;																	// Разрешена генерация прерывания если линии интерфейса готовы для обмена
			uint32_t RESERVED1 	:1;																	//
			uint32_t ABN_S_EN 	:1;																	// Разрешает анализ нарушений при генерации события «рестарт» и генерацию прерывания
			uint32_t ABN_P_EN 	:1;																	// Разрешает анализ нарушений при генерации события «стоп» и генерацию прерывания
			uint32_t RESERVED2 	:16;																//
		} b;
	};
} I2C_CR_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t STAT 		:5;																	// Код статуса
			uint32_t RESERVED1 	:1;																	//
			uint32_t ABRT 		:1;																	// Аварийное завершение работы интерфейса в режиме мастера
			uint32_t SLVE 		:1;																	// Завершения работы интерфейса в режиме подчиненного
			uint32_t RESERVED2 	:24;																//
		} b;
	};
} I2C_SR_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t AZE 		:1;																	// Разрешение доступа к «подчиненному» по нулевому адресу (когда бит  равен 1)
			uint32_t SA 		:7;																	// Адрес устройства в режиме «подчиненный»
			uint32_t RESERVED 	:24;																//
		} b;
	};
} I2C_AR_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t DIV 		:12;																// Коэффициент деления для формирования частоты SCL
			uint32_t RESERVED1 	:4;																	//
			uint32_t DSUP 		:8;																	// Количество тактов предустановки бита данных относительно SCL. Реальное время предустановки будет равно DSUP+2
			uint32_t RESERVED2 	:8;																	//
		} b;
	};
} I2C_VR_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t SDA_P 		:1;																	// Значение вывода SDA
			uint32_t SCL_P 		:1;																	// Значение вывода SCL
			uint32_t RESERVED 	:30;																//
		} b;
	};
} I2C_PR_type;

typedef struct
{
	union
	{
		uint32_t word;
		struct
		{
			uint32_t I2C_READY 	:1;																	// Линии SDA и SCL находятся в высоком уровне.
			uint32_t SCLM 		:1;																	// Логический уровень внутреннего сигнала синхронизации, который мастер выдает на линию SCL
			uint32_t SDA_MG 	:1;																	// Значение линии SDA. Берется мажоритарно по трем последним отсчетам
			uint32_t SCL_MG 	:1;																	// Значение линии SCL. Берется мажоритарно по трем последним отсчетам
			uint32_t MASTER 	:1;																	// Интерфейс работает в режиме мастера
			uint32_t SLAVE 		:1;																	// Интерфейс работает в режиме подчиненного
			uint32_t ST_BY 		:1;																	// Интерфейс выполняет передачу (если мастер) или прием (если подчиненный) первого байта сообщения (адрес)
			uint32_t R_W 		:1;																	// Бит указывает на направление передачи данных при обмене. 0 – в режиме мастера указывает на передачу данных; в режиме подчиненного – на прием
			uint32_t BIC 		:4;																	// Внутренний счетчик, указывающий на текущий номер бита передаваемого на линиях байта сообщения
			uint32_t BUSY 		:1;																	// 1 – интерфейс занят. Бит устанавливается событием “start” и сбрасывается событием “stop”
			uint32_t STX_BY 	:1;																	// 1 – интерфейс в режиме подчиненного выполняет прием второго байта сообщения (расширенный адрес).
			uint32_t SLV_EXE 	:1;																	// Отличие данного бита от флага Slave состоит в том, что данный флаг сбрасывается в ноль при выполнении чтения данных из подчиненного при получении бита подтверждения ACK=0.
			uint32_t SLV_XM 	:1;																	// 1 – интерфейс в режиме подчиненного принял полный 10-битовый адрес и он совпал с адресом интерфейса. Бит сбрасывается в ноль при получении ситуации “stop” или когда бит М10 регистра управления равен 0
			uint32_t M_EXE 		:1;																	// Отличие от флага master состоит в том, что данный флаг не сбрасывается перед выполнением события “restart”
			uint32_t RESERVED 	:15;																//
		} b;
	};
} I2C_INFO_type;
/* ------------------------------------ Структура периферии ------------------------------------- */

typedef volatile struct
{
	I2C_CR_type CR;																					// Управление
	I2C_SR_type SR;																					// Состояние
	uint32_t DR;																					// Сдвиговый регистр данных
	I2C_AR_type AR;																					// Адрес
	I2C_VR_type VR;																					// Делитель тактовой частоты
	uint32_t CR_SET;																				// Установка бит регистра управления
	uint32_t CR_CLEAR;																			    // Сброс бит регистра управления
	uint32_t AXR;																					// Младшие 8 бит адреса устройства для работы в режиме подчиненного и в режиме 10-битовой адресации
	I2C_INFO_type INFO;																				// Информация о внутреннем состоянии интерфейса
	I2C_PR_type PR;																					// Значение внешних линий SCL и SDA
}I2C_type;

typedef enum
{
	STAT_MSTR_STOP = 1,																				// Мастер сформировал условие “stop”. Интерфейс завершил работу в режиме мастера
	STAT_MSTR_START,																				// Мастер сформировал условие “start”. Интерфейс начал работу в режиме мастера
	STAT_MSTR_RESTART,																				// Мастер сформировал условие “restart”. Интерфейс продолжает работу в режиме мастера
	STAT_SLV_WR_NULL_ADR,																			// Подчиненный обнаружил со стороны мастера обращение для записи по адресу 0. Режим широковещательной записи во все подчиненные устройства
	STAT_SLV_STOP,																					// Подчиненный обнаружил условие «stop». Интерфейс закончил работу в режиме подчиненного
	STAT_SLV_WRF_ACK = 9,																			// Интерфейс, работая в состоянии «подчиненный», обнаружил на шине «свой» адрес, а также запрос на запись, после чего ответил битом подтверждения ACK = 1
	STAT_SLV_RDF_ACK = 11,																			// Интерфейс, работая в состоянии «подчиненный», обнаружил на шине «свой» адрес, а также запрос на чтение, после чего ответил битом подтверждения ACK = 1
	STAT_MSTR_WR_ADR_SLV_NAK,																		// Мастер отослал первый байт, получил в ответ бит ACK=0 и ждет записи данных для передачи
	STAT_MSTR_WR_ADR_SLV_ACK,																		// Мастер отослал первый байт, получил в ответ бит ACK=1 и ждет записи данных для передачи
	STAT_MSTR_RD_ADR_SLV_NAK,																		// Мастер отослал первый байт, получил в ответ бит ACK=0 и ждет сброса флага прерывания для начала чтения данных из внешнего устройства
	STAT_MSTR_RD_ADR_SLV_ACK,																		// Мастер отослал первый байт, получил в ответ бит ACK=1 и ждет сброса флага прерывания для начала чтения данных из внешнего устройства
	STAT_SLV_WR_NAK,																				// «Подчиненный» принял байт данных и ответил битом подтверждения ACK = 0
	STAT_SLV_WR_ACK,																				// «Подчиненный» принял байт данных и ответил битом подтверждения ACK = 1
	STAT_SLV_RD_NAK,																				// «Подчиненный» отослал байт данных и принял бит подтверждения ACK = 0
	STAT_SLV_RD_ACK,																				// «Подчиненный» отослал байт данных и принял бит подтверждения	ACK = 1
	STAT_MSTR_WR_SLV_NAK,																			// Мастер отослал байт данных и принял бит АСК=0
	STAT_MSTR_WR_SLV_ACK,																			// Мастер отослал байт данных и принял бит АСК=1
	STAT_MSTR_RD_SLV_NAK,																			// Мастер принял байт данных и отослал бит АСК=0
	STAT_MSTR_RD_SLV_ACK,																			// Мастер принял байт данных и отослал бит АСК=1
	STAT_BUS_FREE = 30,																				// Шина свободна
	STAT_RESET = 31																					// Значение после сброса
} I2C_STAT_type;


#define LX_I2C				( ( volatile I2C_type * ) base_I2C )

/* -------------------------------------- Специфические типы ------------------------------------ */
#define I2C_CR_ABRT			( 1 << 0 )
#define I2C_CR_SLVE			( 1 << 1 )
#define I2C_CR_AA			( 1 << 2 )
#define I2C_CR_SI			( 1 << 3 )
#define I2C_CR_STO			( 1 << 4 )
#define I2C_CR_STA			( 1 << 5 )
#define I2C_CR_I2C_ON		( 1 << 6 )
#define I2C_CR_M10			( 1 << 7 )
#define I2C_CR_SLVE_EN		( 1 << 8 )
#define I2C_CR_ABN_EN		( 1 << 9 )
#define I2C_CR_TLOW_EN		( 1 << 10 )
#define I2C_CR_IRQ			( 1 << 11 )
#define I2C_CR_FREE_EN		( 1 << 12 )
#define I2C_CR_ABN_S_EN		( 1 << 14 )
#define I2C_CR_ABN_P_EN		( 1 << 15 )

typedef enum
{
	I2C_PIN_PC2_PC3 = 0,
	I2C_PIN_PC11_PC12
}I2C_PIN_type;

/* ------------------------------- Конфигурирование i2c интерфейса ------------------------------ */
typedef enum
{
	I2C_MASTER = 0,
	I2C_SLAVE
}I2C_Mode_type;

typedef enum
{
	I2C_ADDR_7BIT = 0,
	I2C_ADDR_10BIT
}I2C_AddrMode_type;

typedef enum
{
	I2C_ADDR_0_DISABLE = 0,
	I2C_ADDR_0_ENABLE
}I2C_AddrZero_type;

typedef struct
{
	I2C_Mode_type eMode;																			// Режим работы ведущий или ведомый
	uint32_t ulClockSpeed;																			// Частота клока SLC
	I2C_AddrMode_type eAddrMode;																	// Режим адресации 7 или 10 бит
	uint16_t usAddr;																				// Адрес устройства в режиме подчиненного
	I2C_AddrZero_type eAddrZero;																	// Управление доступом по нулевому адресу
}I2C_Conf_type;

#define I2C_SLAVE_BUFF_SIZE		32
#define I2C_MASTER_BUFF_SIZE	32

typedef enum
{
	I2C_OK = 0,
	I2C_BUSY,
	I2C_NO_ANSWER,
	I2C_SLAVE_STOP,
	I2C_BUS_FAIL
}I2C_STATUS_type;

typedef enum
{
	I2C_MSTR_WR = 0,
	I2C_MSTR_RD,
	I2C_MSTR_RD_ADDR
}I2C_Control_type;

typedef struct
{
	I2C_STATUS_type eStatus;																		// Состояние
	I2C_Control_type eControl;																		// Конфигурация
	uint8_t ucRxCnt;																				// Количество принятых данных ведомым
	uint8_t ucSlaveInBuff[ I2C_SLAVE_BUFF_SIZE ];													// Входной буфер ведомого
	uint8_t ucSlaveOutBuff[ I2C_SLAVE_BUFF_SIZE ];													// Выходной буфер ведомого
	uint8_t ucMasterBuff[ I2C_MASTER_BUFF_SIZE ];													// Буфер мастера
	uint8_t ucMasterDataLen;																		// Количество данных для передачи/приема
	uint8_t ucSlaveAddr;																			// Адрес ведомого, в который будем писать в режиме мастера
	uint8_t ucDataAddr;																				// Адрес данных, которые собираемся читать
}I2C_Struct_type;

extern volatile I2C_Struct_type I2C_Struct;


#ifdef __cplusplus
extern "C"
	{
#endif
	void 	HAL_I2C_Init		( I2C_Conf_type *pxConf );
	void 	HAL_I2C_SetupGpio	( void );
	void 	HAL_I2C_Enable		( void );
	void 	HAL_I2C_Write		( uint8_t ucDevAddr, uint8_t *pucBuff, uint8_t ucLenght );
	void 	HAL_I2C_Read		( uint8_t ucDevAddr, uint8_t ucLenght );
	void 	HAL_I2C_ReadMemAddr	( uint8_t ucDevAddr, uint8_t ucDataAddr, uint8_t ucLenght );
#ifdef __cplusplus
	}
#endif // __cplusplus

#endif /* __HAL_I2C_H__ */
