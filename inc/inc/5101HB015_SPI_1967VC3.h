#ifndef __ADC5101HB015_SPI__H__
#define __ADC5101HB015_SPI__H__


#include "stdint.h"
#include "hal_typedef.h"
#include "hal_1967VN044.h"

// Typical delays
#define ADC5101HB015_SPI_PERIOD_NS		1000
#define ADC5101HB015_INIT_DELAY_US		1000
#define ADC5101HB015_RESET_DELAY_US		2000
#define ADC5101HB015_CALIB_DELAY_US		50000


typedef struct {
	GPIO_PortPi *port;
	uint32_t cs_bit;
	uint32_t clk_bit;
	uint32_t mosi_bit;
	uint32_t miso_bit;
	uint32_t power_down_bit;
	uint32_t core_freq_khz;
} ADC5101HB015_hw_config_t;



void ADC5101HB015_SPI_init(void *hw_config);
uint16_t ADC5101HB015_SPI_write(uint16_t data);
void ADC5101HB015_SPI_calibrate(void);
void ADC5101HB015_delay(uint8_t delay_type);



#endif //__ADC5101HB015_SPI_H__
