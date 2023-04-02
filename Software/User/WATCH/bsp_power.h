#ifndef __BSP_POWER_H
#define __BSP_POWER_H

#include "stm32f10x.h"



#define ADCx			ADC1
#define ADC_CLK			RCC_APB2Periph_ADC1

#define POWER_GPIO_PORT	GPIOA
#define POWER_GPIO_CLK	RCC_APB2Periph_GPIOA
#define POWER_GPIO_PIN	GPIO_Pin_2

#define ADC_CHANNEL		ADC_Channel_2


#define ADC_DMA_CHANNEL DMA1_Channel1


extern uint16_t ADC_ValueConvertion;


void POWER_Init(void);

#endif /* __BSP_POWER_H */
