/*****************************************************************************

	Платформо-независимая библиотека функций для
	конфигурации АЦП ADC5101HB015 через интерфейс SPI.
	
	При использовании в составе проекта необходимо реализовать
	функции, указанные в данном файле, как extern.
	Эти функции специфичны для платформы и проекта.


*****************************************************************************/



#include "5101HB015.h"



void ADC5101HB015_write_reg(uint8_t reg, uint8_t value)
{
	uint16_t word_to_send = 0;
	word_to_send |= reg << 8;
	word_to_send |= value;
	ADC5101HB015_SPI_write(word_to_send);
}


uint8_t ADC5101HB015_read_reg(uint8_t reg)
{
	uint16_t word_read;
	uint16_t word_to_send = 1 << 15;
	word_to_send |= reg << 8;
	word_read = ADC5101HB015_SPI_write(word_to_send);
	return word_read & 0xFF;
}


void ADC5101HB015_init(uint8_t do_reset, uint8_t do_calibrate, void *spi_init_data)
{
	// Initialize SPI by calling outer function
	ADC5101HB015_SPI_init(spi_init_data);
	ADC5101HB015_delay(ADC5101HB015_DELAY_INIT);
	if (do_reset)
		ADC5101HB015_reset();
	if (do_calibrate)
		ADC5101HB015_calibrate();
}


void ADC5101HB015_reset(void)
{
	ADC5101HB015_write_reg(ADC5101HB015_REG_SOFT_RESET, 1);
	ADC5101HB015_delay(ADC5101HB015_DELAY_RESET);
}


void ADC5101HB015_calibrate(void)
{
	ADC5101HB015_SPI_calibrate();
	ADC5101HB015_delay(ADC5101HB015_DELAY_CALIBRATE);
}


void ADC5101HB015_config(ADC5101HB015_config_t *config)
{
	uint8_t value = 0;
	value |= (0x1 & config->reference_level) << 1;
	value |= ((0x1 & config->output_format) ^ (0x1 & config->lvdsen_pin_state)) << 2;
	value |= (0x1 & config->lvds_current_mode) << 3;
	value |= (0x1 & config->oen_pin_override) << 4;
	value |= (0x1 & config->common_mode_sel) << 5;
	ADC5101HB015_write_reg(ADC5101HB015_REG_OUT_MODE, value);
}


void ADC5101HB015_fill_config_with_defaults(ADC5101HB015_config_t *config)
{
	config->reference_level 	= ADC5101HB015_REF_1P0;
	config->output_format 		= ADC5101HB015_OUTPUT_CMOS;
	config->lvdsen_pin_state 	= ADC5101HB015_LVDSEN_PIN_LOW;
	config->lvds_current_mode 	= ADC5101HB015_LVDS_CURRENT_NORMAL;
	config->oen_pin_override	= ADC5101HB015_OEN_NO_OVERRIDE;
	config->common_mode_sel		= ADC5101HB015_COMMON_MODE_0P75;
}





