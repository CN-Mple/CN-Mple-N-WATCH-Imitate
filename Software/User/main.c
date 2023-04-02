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

static void Delay(__IO uint32_t nCount) //�򵥵���ʱ����
{
	for (; nCount != 0; nCount--);
}

int main()
{
	uint8_t i;
	
	POWER_Init();					//����IO��ʼ��
//	USART_Config();
	Button_Init();					//������ʼ��
	OLED_Init();					//OLED��ʼ��
	delay_init();					//΢�� ���뼶��ʱ��ʼ��
//	printf("holle");
//	SysTick_Init();
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	
	MPU6050_Init();					//Mpu��ʼ��
	i = mpu_dmp_init();
	while(i)
	{
		Delay(50000);
		i = mpu_dmp_init();
	}
	Time3_Init();					//ʱ������
	
	Time4_Init();
	
	Buzzer_Init();					//Buzz��ʼ��
	DS3231_Init();					//ʱ���ʼ��
	
	Watch_load();					//��������
	while(1)
	{
		
//		MPU6050_updata();			//MPUԭʼ�����ϴ�
//		time_updata();				//ʱ���ϴ�
		
		
		Buzzer_updata();			//��������������
		
		display_updata();			//�û�����
	}
}
