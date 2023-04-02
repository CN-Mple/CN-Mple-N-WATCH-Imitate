#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "stm32f10x.h"
#include "main.h"

//ʹ��LSE�ⲿʱ�� �� LSI�ڲ�ʱ��
#define RTC_CLOCK_SOURCE_LSE      
//#define RTC_CLOCK_SOURCE_LSI

#define RTC_BKP_DRX          BKP_DR1
// д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA         0xA5A5
//����ʱ���ʱ��������
#define TIME_ZOOM						(8*60*60)

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
};

extern struct rtc_time systmtime;
extern __IO uint32_t TimeDisplay;
extern char const *en_WEEK_STR[];

void My_RTC_Config(void);

void Time_Set_Funcation(void);
void Time_Adjust(struct rtc_time *tm);
void Time_Display(uint32_t TimeVar,struct rtc_time *tm);


#endif /* __BSP_RTC_H */
