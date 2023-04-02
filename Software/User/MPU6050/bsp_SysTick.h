#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

int get_tick_count(unsigned long *count);
void mdelay(unsigned long nTime);
void SysTick_Init(void);

void TimingDelay_Decrement(void);
void TimeStamp_Increment(void);


void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);
#endif /* __SYSTICK_H */

