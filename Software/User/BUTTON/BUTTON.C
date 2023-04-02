#include "BUTTON.H"


static s_button buttons[BTN_COUNT];								//定义按键

const uint32_t tuneBtn1[] = {
	TONE_2KHZ<<16 | 100,
	TONE_PAUSE<<16 | 50,
	TONE_3KHZ<<16 | 100,
	TONE_STOP
};

const uint32_t tuneBtn2[] = {
	TONE_3KHZ<<16 | 100,
	TONE_PAUSE<<16 | 50,
	TONE_3KHZ<<16 | 100,
	TONE_STOP
};

const uint32_t tuneBtn3[] = {
	TONE_3KHZ<<16 | 100,
	TONE_PAUSE<<16 | 50,
	TONE_2KHZ<<16 | 100,
	TONE_STOP
};

static void processButton(s_button* buttons,bool isPressed);
static void processButtons(void);


void GPIOInit(void)
{

}
//按键IO初始化 曲调配置
void Button_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	
	
	buttons[BTN_1].tune = tuneBtn1;
	buttons[BTN_2].tune = tuneBtn2;
	buttons[BTN_3].tune = tuneBtn3;
	
	buttons[BTN_1].id = 0;
	buttons[BTN_2].id = 1;
	buttons[BTN_3].id = 2;
}
//按键函数装载
void Set_Button_Func(button_f Button0,button_f Button1,button_f Button2)
{
	buttons[BTN_1].onPress = Button0;
	buttons[BTN_2].onPress = Button1;
	buttons[BTN_3].onPress = Button2;
}


//按键定时上传  10ms
void Buttons_updata(void)
{
	static uint32_t lastUpdate;

	uint32_t now = millis();
	if ((uint32_t)(now - lastUpdate) >= 10)
	{
		lastUpdate = now;

		processButtons();
	}
}
#include "bsp_oled.h"
//按键检测
static void processButtons(void)
{
	uint8_t i;

	bool isPressed[BTN_COUNT];
	isPressed[BTN_1] = Button_0;
	isPressed[BTN_2] = Button_1;
	isPressed[BTN_3] = Button_2;

	
	for(i = 0; i < BTN_COUNT; i++)
	{
		processButton(&buttons[i], !isPressed[i]);
	}
}

static void processButton(s_button* buttons,bool isPressed)
{
	buttons->counter <<= 1;
	if( isPressed )
	{
		buttons->counter++;
		if( buttons->counter > 16 )
		{
			buttons->pressedTime = 1;
			buttons->processed = true;
			if( !buttons->funcDone && buttons->onPress != NULL)
			{
				buttons->onPress();
				buttons->funcDone = true;
				tune_play(buttons->tune,5);
			}
		}
		
	}
	else if(buttons->counter)
	{
		buttons->counter = 0;
		buttons->processed = false;
		buttons->funcDone = false;
	}
}
