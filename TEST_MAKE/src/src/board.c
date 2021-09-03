#include "board.h"
#include "hal_1967VN044.h"


static void BRD_PLL_init(uint32_t f_ref, uint32_t f_core, uint32_t f_bus);

static void BRD_SYS_init(void);



//инициализация отладочной платы
void BRD_init(void) {	

	asm("rds;;");
	asm("btben;;");
	asm("nop;;");

	//инициализация системной шины (для работы с SDRAM, LED, TOUCH)
	BRD_SYS_init();
	//инициализация pll, установка частоты ядра и внешней шины
	BRD_PLL_init(XTI_FREQ_KHZ, CORE_FREQ_KHZ, BUS_FREQ_KHZ);

	//разрешение тактирования всей переферии
	LX_CMU->CFG8.word = 0;
	LX_CMU->CFG1.word = 1;
	
	//разрешение глобальных прерываний
	HAL_Interrupt_IMASKClear();
	HAL_Interrupt_ILATClear();
	HAL_Interrupt_GlobalEnable();
	


}


static void BRD_PLL_init(uint32_t f_ref, uint32_t f_core, uint32_t f_bus) {
	HAL_PLL_CoreSetup(f_core);
	HAL_PLL_BusSetup(f_bus);
}


//инициализация светодиодов
void Init_LEDS_Port(void)
{
	uint32_t tmp;

		// GPIO
		tmp = 	GPIO_PIN_10 |	//SIG0
				GPIO_PIN_11 |	//SIG1
				GPIO_PIN_12;	//SIG2
		HAL_GPIO_Init (LX_GPIO_PC, tmp , GPIO_PinMode_Out);

		tmp = GPIO_PIN_31; 		//SIG3
		HAL_GPIO_Init (LX_GPIO_PB, tmp , GPIO_PinMode_Out);
}

//изменение состояния светодиода с номером led
void BRD_LED_toggleState(uint32_t led)
{
	GPIO_PIN bit;
	switch(led & 0x03)
	{
		case 0:	bit = LED1;	break;
		case 1:	bit = LED2;	break;
		case 2:	bit = LED3;	break;
		case 3:	bit = LED4;	break;
	}

	(led <= 2) ? HAL_GPIO_TogglePins (LEDS_PORTC, bit) : HAL_GPIO_TogglePins (LEDS_PORTB, bit);
}

//включение светодиода с номером led
void BRD_LED_setState(uint32_t led) {
	GPIO_PIN bit;
	switch(led & 0x03)
	{
		case 0:	bit = LED1;	break;
		case 1:	bit = LED2;	break;
		case 2:	bit = LED3;	break;
		case 3:	bit = LED4;	break;
	}

	(led <= 2) ? HAL_GPIO_WritePin (LEDS_PORTC, bit, GPIO_PinState_Set) : HAL_GPIO_WritePin (LEDS_PORTB, bit, GPIO_PinState_Set);
}

//выключение светодиода с номером led
void BRD_LED_resetState(uint32_t led) {
	GPIO_PIN bit;
	switch(led & 0x03)
	{
		case 0:	bit = LED1;	break;
		case 1:	bit = LED2;	break;
		case 2:	bit = LED3;	break;
		case 3:	bit = LED4;	break;
	}

	(led <= 2) ? HAL_GPIO_WritePin (LEDS_PORTC, bit, GPIO_PinState_Reset) : HAL_GPIO_WritePin (LEDS_PORTB, bit, GPIO_PinState_Reset);
}

//задержка выполнения программы на msec в миллисекундах
void wait_msec(uint32_t msec)
{
	HAL_SYS_WaitMs(msec);
}

//инициализация системной шины
static void BRD_SYS_init(void)
{
	uint32_t			temp32u;
	// Разрешение многократной записи регистров SYSCON/SDRCON
	*(unsigned int*)CMU_CFG1_LOC = (1<<3);  // SYS_WE

	// Configure syscon register
	temp32u = 	SYSCON_MS0_IDLE		|
				SYSCON_MS0_WT3		|
				SYSCON_MS0_PIPE1	|
				SYSCON_MS0_SLOW		|

				SYSCON_MS1_IDLE		|
				SYSCON_MS1_WT0		|
				SYSCON_MS1_PIPE2	|
				//SYSCON_MS1_SLOW	|

				//SYSCON_MSH_IDLE	|
				//SYSCON_MSH_WT0	|
				//SYSCON_MSH_PIPE2	|
				//SYSCON_MSH_SLOW	|

				//SYSCON_MEM_WID64	|
				//SYSCON_MP_WID64	|
				//SYSCON_HOST_WID64	|

				SYSCON_SDRAM_ALIAS	|
				0x00;;
	__builtin_sysreg_write(__SYSCON, temp32u);
	__builtin_sysreg_write(__FLAGREGST, 0xFF);
}
//инициализация SDRAM
void BRD_SDRAM_init(void)
{
	uint32_t temp32u;

	// Задание альтернативной функции для порта С
	// Сигналы внешей шины
	LX_GPIO_PC->ALT.LD = (1<<18) | (1<<19) | (1<<20) | (1<<21) | (1<<22) |
						(1<<8) | (1<<9) | (1<<10) | (1<<11) | (1<<12) | (1<<13) | (1<<14) | (1<<15) | (1<<16) | (1<<17);

	// Альтернативная функция для шин  DATA and ADDR
	temp32u =		 	PX_ALT_PDB0 | PX_ALT_PDB1 | PX_ALT_PDB2 | PX_ALT_PDB3 |
						PX_ALT_PAB0 | PX_ALT_PAB1 | PX_ALT_PAB2 |
						//PX_ALT_PDB23F0 |								// DATA[31:16] = NAND
						0;												// DATA[31:16] = bus
	* (uint32_t *)PX_ALT_LOC = temp32u;


	//инициализация SDRAM
	__builtin_sysreg_write(__SDRCON, 0);
	temp32u = 	SDRCON_ENBL 	|
			SDRCON_CLAT3 	| 		// only CAS latency 2 or 3 are supported by mt48lc4m32b2 model
			SDRCON_PG256	|
			//SDRCON_PG512	|
			//SDRCON_PG1K		|
			SDRCON_REF1100	|
			SDRCON_PC2RAS2	|		// tRAS, precharge to active time, 20 ns
			SDRCON_RAS2PC5	|		// tRP,  active to precharge time, 42 ns min
			SDRCON_INIT		|
			0x00;;
	__builtin_sysreg_write(__SDRCON, temp32u);

	#ifndef	__SIM__
	while((__builtin_sysreg_read(__SYSTAT) & (1<<13)) == 0);
	#endif	//__SIM__
}

//анализ нажатия кнопки
uint32_t BRD_GetButtons(void)
{
	uint32_t btn = 0;
	uint32_t pinAVal = LX_GPIO_PA->PXD;
	uint32_t pinBVal = LX_GPIO_PB->PXD;
	uint32_t pinCVal = LX_GPIO_PC->PXD;

	if (!(pinAVal & (1UL << 31)))		btn |= BTN0;	// SA9, btn7
	if (!(pinAVal & (1UL << 30)))		btn |= BTN1;	// SA8, btn6
	if (!(pinBVal & (1UL << 2)))		btn |= BTN2;	// SA4, btn2
	if (!(pinAVal & (1UL << 28)))		btn |= BTN3;	// SA6, btn4
	if (!(pinBVal & (1UL << 1)))		btn |= BTN4;	// SA3, btn1
	if (!(pinBVal & (1UL << 0)))		btn |= BTN5;	// SA2, btn0
	if (!(pinBVal & (1UL << 3)))		btn |= BTN6;	// SA5, btn3
	if (!(pinAVal & (1UL << 29)))		btn |= BTN7;	// SA7, btn5

	return btn;
}

