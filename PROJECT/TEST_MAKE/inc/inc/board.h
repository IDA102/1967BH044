#ifndef _BOARD_H_
#define _BOARD_H_

#include "config.h"
#include "hal_1967VN044.h"


//светодиоды
#define LED1 GPIO_PIN_10
#define LED2 GPIO_PIN_11
#define LED3 GPIO_PIN_12
#define LED4 GPIO_PIN_31

#define LEDS_PORTC LX_GPIO_PC
#define LEDS_PORTB LX_GPIO_PB
#define LED_ACTIVE_LEVEL	1

//кнопки
#define BTN0	(1 << 0)
#define BTN1	(1 << 1)
#define BTN2	(1 << 2)
#define BTN3	(1 << 3)
#define BTN4	(1 << 4)
#define BTN5	(1 << 5)
#define BTN6	(1 << 6)
#define BTN7	(1 << 7)

typedef enum {
	brdLed1,		// VD2
	brdLed2,		// VD3
	brdLed3,		// VD4
	brdLed4,		// VD5
} boardLed;

void BRD_init(void);
void Init_LEDS_Port(void);
void BRD_LED_toggleState(uint32_t led);
void BRD_LED_setState(uint32_t led) ;
void BRD_LED_resetState(uint32_t led);
void BRD_SDRAM_init(void);
void BRD_Reboot(void);
void wait_msec(uint32_t msec);
uint32_t BRD_GetButtons(void);

#endif //_BOARD_H_

