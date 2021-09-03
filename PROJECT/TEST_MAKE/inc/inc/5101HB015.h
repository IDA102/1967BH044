#ifndef __ADC5101HB015_H__
#define __ADC5101HB015_H__


#include "stdint.h"

// Внутренняя карта регистров АЦП
#define ADC5101HB015_REG_MEM_L			0x00
#define ADC5101HB015_REG_MEM_H			0x10
#define ADC5101HB015_REG_CAL_MODE		0x29
#define ADC5101HB015_REG_OUT_MODE		0x2A
#define ADC5101HB015_REG_REF_TRIM		0x2B
#define ADC5101HB015_REG_SOFT_RESET		0x2F
#define ADC5101HB015_REG_CFACA_L		0x30
#define ADC5101HB015_REG_CFACA_M		0x31
#define ADC5101HB015_REG_CFACA_H		0x32
#define ADC5101HB015_REG_CFACB_L		0x34
#define ADC5101HB015_REG_CFACB_M		0x35
#define ADC5101HB015_REG_CFACB_H		0x36
#define ADC5101HB015_REG_CDACC_L		0x38
#define ADC5101HB015_REG_CDACC_M		0x39
#define ADC5101HB015_REG_CDACC_H		0x3A
#define ADC5101HB015_REG_SHMD_L			0x3C
#define ADC5101HB015_REG_SHMD_M			0x3D
#define ADC5101HB015_REG_SHMD_H			0x3E
#define ADC5101HB015_REG_ACC_L			0x40
#define ADC5101HB015_REG_ACC_M			0x41
#define ADC5101HB015_REG_ACC_H			0x42
#define ADC5101HB015_REG_QR_L			0x43
#define ADC5101HB015_REG_QR_H			0x44


// Типы задержек
enum { ADC5101HB015_DELAY_INIT, ADC5101HB015_DELAY_RESET, ADC5101HB015_DELAY_CALIBRATE };

// Аргументы функции ADC5101HB015_init()
#define ADC5101HB015_NO_RESET			0
#define ADC5101HB015_DO_RESET			1
#define ADC5101HB015_NO_CALIBRATION		0
#define ADC5101HB015_DO_CALIBRATION		1

// Значения для заполнения конфигурационной структуры
#define ADC5101HB015_REF_0P5					0
#define ADC5101HB015_REF_1P0					1
#define ADC5101HB015_OUTPUT_CMOS				0
#define ADC5101HB015_OUTPUT_LVDS				1
#define ADC5101HB015_LVDSEN_PIN_LOW				0
#define ADC5101HB015_LVDSEN_PIN_HIGH			1
#define ADC5101HB015_LVDS_CURRENT_NORMAL		0
#define ADC5101HB015_LVDS_CURRENT_LOW			1
#define ADC5101HB015_OEN_NO_OVERRIDE			0
#define ADC5101HB015_OEN_OVERRIDE				1
#define ADC5101HB015_COMMON_MODE_0P75			0
#define ADC5101HB015_COMMON_MODE_VREFDIV2		0

// Конфигурационная структура для функции ADC5101HB015_config()
typedef struct {
	uint8_t	reference_level;
	uint8_t output_format;
	uint8_t lvdsen_pin_state;
	uint8_t	lvds_current_mode;
	uint8_t	oen_pin_override;
	uint8_t common_mode_sel;
} ADC5101HB015_config_t;


// Функции, реализованные в модуле
void ADC5101HB015_write_reg(uint8_t reg, uint8_t value);
uint8_t ADC5101HB015_read_reg(uint8_t reg);
void ADC5101HB015_init(uint8_t do_reset, uint8_t do_calibrate, void *spi_init_data);
void ADC5101HB015_reset(void);
void ADC5101HB015_calibrate(void);
void ADC5101HB015_config(ADC5101HB015_config_t *config);
void ADC5101HB015_fill_config_with_defaults(ADC5101HB015_config_t *config);


// Внешние функции
// Инициализация SPI
extern void ADC5101HB015_SPI_init(void *);
// Передача 16 бит данных
extern uint16_t ADC5101HB015_SPI_write(uint16_t data);
// Калибровка АЦП. Запускается специальным состоянием линий SPI
extern void ADC5101HB015_SPI_calibrate(void);
// Задержка для соблюдения таймингов АЦП
extern void ADC5101HB015_delay(uint8_t delay_type);



#endif //__ADC5101HB015_H__
