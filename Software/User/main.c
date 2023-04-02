#include "stm32f10x.h"

#include "string.h"
#include "bsp_usart.h"
#include "bsp_oled.h"
#include "bsp_time.h"

#include "bsp_power.h"
#include "bsp_mpu6050.h"

#include "bsp_SysTick.h"

#include "WATCH.H"
#include "DISPLAY.H"
#include "MY_MENU.H"
#include "BUTTON.H"
#include "BUZZER.H"
#include "DS3231.H"

static void Delay(__IO uint32_t nCount) //简单的延时函数
{
	for (; nCount != 0; nCount--);
}

int main()
{
	uint8_t i;
	
	POWER_Init();					//供电IO初始化
//	USART_Config();
	Button_Init();					//按键初始化
	OLED_Init();					//OLED初始化
	delay_init();					//微妙 毫秒级延时初始化
//	printf("holle");
//	SysTick_Init();
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	
	MPU6050_Init();					//Mpu初始化
	i = mpu_dmp_init();
	while(i)
	{
		Delay(50000);
		i = mpu_dmp_init();
	}
	Time3_Init();					//时间设置
	
	Time4_Init();
	
	Buzzer_Init();					//Buzz初始化
	DS3231_Init();					//时间初始化
	
	Watch_load();					//表盘载入
	while(1)
	{
		
//		MPU6050_updata();			//MPU原始数据上传
//		time_updata();				//时间上传
		
		
		Buzzer_updata();			//蜂鸣器曲调播放
		
		display_updata();			//用户操作
	}
}
