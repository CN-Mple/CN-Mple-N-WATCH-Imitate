#ifndef __BUTTON_H
#define __BUTTON_H

#include "MENU.H"
#include "BUZZER.H"
#include <stdbool.h>
#include "bsp_time.h"

typedef bool 		(*button_f)(void);

typedef enum
{
	BTN_1 = 0,
	BTN_2 = 1,
	BTN_3 = 2,
	BTN_COUNT = 3 			// 按键数量
} btn_t;

typedef struct {
	uint8_t id;
	uint32_t pressedTime;	// 时间戳
	bool processed;			// 按键状态
	uint8_t counter;		// 按键计时
	bool funcDone;			// 功能执行状态
	button_f onPress;		// 功能函数
	const uint32_t* tune;	// 曲调
} s_button;


void GPIOInit(void);


#define Button_0	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)//sw2
#define Button_1	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)//sw1
#define Button_2	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)//sw3

void Button_Init(void);

void Set_Button_Func(button_f Button0,button_f Button1,button_f Button2);

void Buttons_updata(void);


#endif /* __BUTTON_H */

