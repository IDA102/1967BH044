#ifndef __CONFIG_H_
#define __CONFIG_H_

//=====================================================================================
//===== OPTIONAL ======================================================================
//=====================================================================================

#define DMA_CH_AC_TX		6
#define DMA_CH_AC_RX		10
#define LCD_DMA_CHANNEL		7

//=====================================================================================
//===== MANDATORY =====================================================================
//=====================================================================================
// Clock section
#define XTI_FREQ_KHZ		48000	//48000// The XTI clock frequency is determined by external quarz or generator
#define CORE_FREQ_KHZ		200000  //200000	// Desired core clock frequency (for PLL setup)
#define SOC_CLK_KHZ			CORE_FREQ_KHZ/2
#define	BUS_FREQ_KHZ		80000	// up to 140000
#define CLK_PER_1MS 		BUS_FREQ_KHZ

#define DAC_CLK				25.77e6 //частота приема одного 16ти битного слова ЦАПом


#endif	//__CONFIG_H_
