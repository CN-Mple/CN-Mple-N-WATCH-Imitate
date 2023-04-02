#ifndef __BSP_TIME_H
#define __BSP_TIME_H

#include "stm32f10x.h"


void NVIC_TIME_Config(void);
void Timex_Config(void);

void Time3_Init(void);
void Time4_Init(void);
	

uint32_t millis(void);

#endif /* __BSP_TIME_H */
