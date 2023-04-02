#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"
#include "bsp_time.h"
#include "string.h"

extern uint32_t vol;

typedef enum
{
	TONE_STOP = 0,
	TONE_PAUSE = 1,
	TONE_REPEAT = 2,
	TONE_2KHZ = 250,
	TONE_2_5KHZ = 200,
	TONE_3KHZ = 166,
	TONE_3_5KHZ = 143,
	TONE_4KHZ = 125,
	TONE_4_5KHZ = 111,
	TONE_5KHZ = 100,
} tone_t;


void Buzzer_Init(void);

void Buzzer_Buzz( uint16_t len,uint16_t tone ,uint16_t vol,void(*_OnFinish)(void));

void Buzzer_Next(void);

void Buzzer_updata(void);

void Buzzer_BuzzStop(void);


void tune_play(const uint32_t* _tune,uint32_t _prio);

#endif /* __BUZZER_H */
