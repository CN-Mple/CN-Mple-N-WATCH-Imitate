#include "bsp_time.h"

uint32_t milliseconds = 0;


uint32_t millis(void)
{
	uint32_t now;
	now = milliseconds;
	return now;
}

static void NVIC_TIME3_Config(void)
{
	NVIC_InitTypeDef NVIC_structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_structure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_structure.NVIC_IRQChannelSubPriority = 3;
	NVIC_structure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_structure);
}

void Time3_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBasestructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBasestructure.TIM_Period = 999;//1ms
	TIM_TimeBasestructure.TIM_Prescaler = 71;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBasestructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
}



void Time3_Init(void)
{
	NVIC_TIME3_Config();
	Time3_Config();
}


static void NVIC_TIME4_Config(void)
{
	NVIC_InitTypeDef NVIC_structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_structure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_structure.NVIC_IRQChannelSubPriority = 4;
	NVIC_structure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_structure);
}

void Time4_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBasestructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBasestructure.TIM_Period = ( 20000 - 1 );//1ms
	TIM_TimeBasestructure.TIM_Prescaler = 71;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBasestructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
}



void Time4_Init(void)
{
	NVIC_TIME4_Config();
	Time4_Config();
}


