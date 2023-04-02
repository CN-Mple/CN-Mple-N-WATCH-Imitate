#include "BUZZER.H"

static uint32_t BuzzLen;		//奏响时长
static uint32_t StartTime;		//开始时间
static void(*OnFinish)(void);	//虚接函数

static uint8_t idx = 0;			// 音节编号
static const uint32_t* tune;	// 曲谱
uint32_t vol = 3;					// 音量
static uint32_t prio;			// 优先级

//Buzzer初始化
void Buzzer_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
//	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1439;					//20us * 50000 = 1000,000us
	TIM_TimeBaseInitStructure.TIM_Period = 999;						//ARR
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;				//选择定时器模式:PWM2 __--
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 
	
	TIM_OCInitStructure.TIM_Pulse = 500; 							//CCR
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  						//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  				//CCR预装载使能
	TIM_ARRPreloadConfig(TIM1, ENABLE); 							//ARR预装载使能
	
	TIM_Cmd(TIM1, DISABLE);
}

//播放曲谱
void tune_play(const uint32_t* _tune,uint32_t _prio)
{
	// Check priority, if lower than currently playing tune priority then ignore it
	if(_prio < prio)
		return;

	prio	= _prio;
	tune	= _tune;

	idx		= 0;
	// Begin playing
	Buzzer_Next();
}

//下一个音节
void Buzzer_Next()
{
	uint16_t len;
	uint16_t tone;
	
	uint32_t data = tune[idx++];
	
	len = data;
	tone = (data >> 16);
	
	Buzzer_Buzz( len,tone,vol,Buzzer_Next);
	if(len == 0)
	{
		prio = 0;
		
	}
}
//奏响音节
void Buzzer_Buzz( uint16_t len,uint16_t tone ,uint16_t vol,void(*_OnFinish)(void))
{
	if(tone == 0)
	{
		Buzzer_BuzzStop();
		return;
	}
	uint16_t ocr;
	
	BuzzLen = len;
	StartTime = millis();
	OnFinish = _OnFinish;
	
	if(tone == TONE_PAUSE)
	{
		TIM_SetCompare1(TIM1,5001);			//确保停止
		TIM_Cmd(TIM1, DISABLE);  			//停止TIM1
		return;
	}
	
	ocr = 50000/tone;
	
	switch(vol)
	{
		case 0:
			TIM_SetCompare1(TIM1,5000);	//CCR
			break;
		case 1:
			TIM_SetCompare1(TIM1,ocr/8);//1/4音量
			break;
		case 2:
			TIM_SetCompare1(TIM1,ocr/4);//1/2音量
			break;
		case 3:
			TIM_SetCompare1(TIM1,ocr/2);//1/1音量
			break;
	}
	TIM_SetAutoreload(TIM1,ocr);		//ARR
	TIM_SetCounter(TIM1,0);				//CNT

	TIM_Cmd(TIM1, ENABLE);	
}

//Buzzer上传状态
void Buzzer_updata(void)
{
	static uint32_t now = 0;
	
	now = millis();
	if(BuzzLen && ( ( BuzzLen * 2 + StartTime ) < now ))
	{
		Buzzer_BuzzStop();
		//结束后播放下一音节
		if(OnFinish != NULL)
		{
			OnFinish();
		}
	}
}
//Buzzer停止
void Buzzer_BuzzStop(void)
{
	BuzzLen = 0;
	TIM_SetCompare1(TIM1,5001);			//CCR
	TIM_SetAutoreload(TIM1,5000);		//ARR
	TIM_SetCounter(TIM1,0);				//CNT
	TIM_Cmd(TIM1, DISABLE);				//STOP

	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}




