#include "bsp_rtc.h"

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  一天有多少s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

static int month_days[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
char const *en_WEEK_STR[] = { "Sun","Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
__IO uint32_t TimeDisplay = 0;

struct rtc_time systmtime=
{
0,0,0,18,1,2023,0
};

static void My_RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVIC_InitStruct);
}

static void RTC_Configuration(void)
{
	/* 使能 PWR 和 Backup 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* 允许访问 Backup 区域 */
	PWR_BackupAccessCmd(ENABLE);
	
	/* 复位 Backup 区域 */
//	BKP_DeInit();/*****/
	
//使用外部时钟还是内部时钟（在bsp_rtc.h文件定义）	
//使用外部时钟时，在有些情况下晶振不起振
//批量产品的时候，很容易出现外部晶振不起振的情况，不太可靠	
#ifdef 	RTC_CLOCK_SOURCE_LSE
	/* 使能 LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* 等待 LSE 准备好 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* 选择 LSE 作为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待 RTC 寄存器 同步
	 * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
	 */
	RTC_WaitForSynchro();
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 使能 RTC 秒中断 */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 设置 RTC 分频: 使 RTC 周期为1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
#else

	/* 使能 LSI */
	RCC_LSICmd(ENABLE);

	/* 等待 LSI 准备好 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* 选择 LSI 作为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待 RTC 寄存器 同步
	 * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
	 */
	RTC_WaitForSynchro();
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 使能 RTC 秒中断 */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 设置 RTC 分频: 使 RTC 周期为1s ,LSI约为40KHz */
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */	
	RTC_SetPrescaler(40000-1); 
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
#endif
}

 /*计算公历*/
void GregorianDay(struct rtc_time * tm)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear=tm->tm_year-1;

	/*计算从公元元年到计数的前一年之中一共经历了多少个闰年*/
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;      

     /*如若计数的这一年为闰年，且计数的月份在2月之后，则日数加1，否则不加1*/
	if((tm->tm_year%4==0) &&
	   ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
	   (tm->tm_mon>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->tm_mon-1] + tm->tm_mday; /*计算从公元元年元旦到计数日期一共有多少天*/

	tm->tm_wday=day%7;
}

void to_tm(u32 tim, struct rtc_time * tm)
{
	register u32    i;
	register long   hms, day;

	day = tim / SECDAY;			/* 有多少天 */
	hms = tim % SECDAY;			/* 今天的时间，单位s */

	/* Hours, minutes, seconds are easy */
	tm->tm_hour = hms / 3600;
	tm->tm_min = (hms % 3600) / 60;
	tm->tm_sec = (hms % 3600) % 60;

	/* Number of years in days */ /*算出当前年份，起始的计数年份为1970年*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->tm_year = i;

	/* Number of months in days left */ /*计算当前的月份*/
	if (leapyear(tm->tm_year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->tm_mon = i;

	/* Days are what is left over (+1) from all that. *//*计算当前日期*/
	tm->tm_mday = day + 1;

	/*
	 * Determine the day of week
	 */
	GregorianDay(tm);
	
}

u32 mktimev(struct rtc_time *tm)
{
	if (0 >= (int) (tm->tm_mon -= 2)) {	/* 1..12 -> 11,12,1..10 */
		tm->tm_mon += 12;		/* Puts Feb last since it has leap day */
		tm->tm_year -= 1;
	}

	return (((
		(u32) (tm->tm_year/4 - tm->tm_year/100 + tm->tm_year/400 + 367*tm->tm_mon/12 + tm->tm_mday) +
			tm->tm_year*365 - 719499
	    )*24 + tm->tm_hour /* now have hours */
	  )*60 + tm->tm_min /* now have minutes */
	)*60 + tm->tm_sec; /* finally seconds */	 
}

void Time_Adjust(struct rtc_time *tm)
{
	
			/* RTC 配置 */
		RTC_Configuration();

	  /* 等待确保上一次操作完成 */
	  RTC_WaitForLastTask();
		  
	  /* 计算星期 */
	  GregorianDay(tm);

	  /* 由日期计算时间戳并写入到RTC计数寄存器 */
	  RTC_SetCounter(mktimev(tm)-TIME_ZOOM);

	  /* 等待确保上一次操作完成 */
	  RTC_WaitForLastTask();
}

void My_RTC_Config(void)
{
	My_RTC_NVIC_Config();
	
	/* 使能 PWR 和 Backup 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // 设置PWR和BKP的时钟

	PWR_BackupAccessCmd(ENABLE); // 使能BKP寄存器的设置
	
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		printf("\r\nBKP = %d",BKP_ReadBackupRegister(RTC_BKP_DRX));
		Time_Adjust(&systmtime);
		/* 使能 PWR 和 Backup 时钟 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // 设置PWR和BKP的时钟

		PWR_BackupAccessCmd(ENABLE); // 使能BKP寄存器的设置
		/*向BKP_DR1寄存器写入标志，说明RTC已在运行*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		/* 使能 PWR 和 Backup 时钟 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

		/* 允许访问 Backup 区域 */
		PWR_BackupAccessCmd(ENABLE);

		/*LSE启动无需设置新时钟*/

		#ifdef RTC_CLOCK_SOURCE_LSI		
		/* 使能 LSI */
		RCC_LSICmd(ENABLE);

		/* 等待 LSI 准备好 */
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{}
		#endif

		/*检查是否掉电重启*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		printf("\r\n\r\n Power On Reset occurred....");
		}
		/*检查是否Reset复位*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		printf("\r\n\r\n External Reset occurred....");
		}

		printf("\r\n No need to configure RTC....");

		/*等待寄存器同步*/
		RTC_WaitForSynchro();

		/*允许RTC秒中断*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/*等待上次RTC寄存器写操作完成*/
		RTC_WaitForLastTask();
	}
	/*定义了时钟输出宏，则配置校正时钟输出到PC13*/
	#ifdef RTCClockOutput_Enable
	/* 使能 PWR 和 Backup 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* 允许访问 Backup 区域 */
	PWR_BackupAccessCmd(ENABLE);

	/* 禁止 Tamper 引脚 */
	/* 要输出 RTCCLK/64 到 Tamper 引脚,  tamper 功能必须禁止 */	
	BKP_TamperPinCmd(DISABLE); 

	/* 使能 RTC 时钟输出到 Tamper 引脚 */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif

	/* 清除复位标志 flags */
	RCC_ClearFlag();
	while(TimeDisplay == 0);
}

void Time_Display(uint32_t TimeVar,struct rtc_time *tm)
{
	   uint32_t BJ_TimeVar;	

	   /*  把标准时间转换为北京时间*/
	   BJ_TimeVar =TimeVar + TIME_ZOOM;

	   to_tm(BJ_TimeVar, tm);/*把定时器的值转换为北京时间*/	
}
