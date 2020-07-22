/*
 *
 *	‘айл: 		hal_interrupt.h
 *	ќписание: 	HAL дл¤ контроллера прерываний
 *
 *	»стори¤:
 *	 			Zatonsky Pavel 		- 	создан
 */
#ifndef __HAL_INTERRUPT_H__
#define __HAL_INTERRUPT_H__

#include <stdint.h>

typedef enum
{
	intKERNEL = 0,
	intGPIO,
	intTMR0LP,
	intTMR1LP,
	intUART0,
	intUART1,
	intLINK0,
	intLINK1,
	intNAND = 10,
	intMIL0,
	intMIL1,
	intDIGC,
	intDMA0,
	intDMA1,
	intDMA2,
	intDMA3,
	intARINC_RX,
	intARINC_TX,
	intSPI1,
	intSPI2,
	intDMA4,
	intDMA5,
	intDMA6,
	intDMA7,
	intI2C,
	intTIM0,
	intTIM1,
	intDMA8,
	intDMA9,
	intDMA10,
	intDMA11,
	intUPDOWN0,
	intUPDOWN1,
	intUPDOWN2,
	intUPDOWN3,
	intDMA12,
	intDMA13,
	intLCD,
	intIRQ0 = 41,
	intIRQ1,
	intIRQ2,
	intIRQ3,
	intSPI0,
	intSSI0,
	intSSI1,
	intVIRPT,
	intVCAM,
	intBUSLK,
	intH264,
	intTMR0HP,
	intTMR1HP,
	intALARM,
	intTIC,
	intWDOG,
	intHW
} InterruptRequest_type;


#ifdef __cplusplus
extern "C"
{
#endif
	void HAL_Interrupt_Enable			( InterruptRequest_type intRQST, void *ptrHndlr );
	void HAL_Interrupt_Disable			( InterruptRequest_type intRQST );
	void HAL_Interrupt_GlobalEnable		( void );
	void HAL_Interrupt_GlobalDisable	( void );
	void HAL_Interrupt_IMASKClear		( void );
	void HAL_Interrupt_ILATClear		( void );
	void HAL_Interrupt_RqstReset		( InterruptRequest_type intRQST );
	void HAL_Interrupt_RqstSet			( InterruptRequest_type intRQST );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_INTERRUPT_H__ */
