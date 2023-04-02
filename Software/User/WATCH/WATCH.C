#include "WATCH.H"


const char days[7][BUFFSIZE_STR_DAYS] = {
  STR_MON,
  STR_TUE,
  STR_WED,
  STR_THU,
  STR_FRI,
  STR_SAT,
  STR_SUN
};

const char months[12][BUFFSIZE_STR_MONTHS] = {
  STR_JAN,
  STR_FEB,
  STR_MAR,
  STR_APR,
  STR_MAY,
  STR_JUN,
  STR_JUL,
  STR_AUG,
  STR_SEP,
  STR_OCT,
  STR_NOV,
  STR_DEC
};



timeDate_s timeDate = {{30,20,15},{DAY_TUE,14,MONTH_FEB,23}};

static display_t Watch_Draw(void);

static void drawTickerNum(tickerData_t* data);

static void draw_Date(void);
static display_t ticker(void);
static void drawBattery(void);

bool Close(void);




void Watch_load(void)
{
	Set_Draw_Func(Watch_Draw);
	Set_Button_Func(Close,(button_f)Screen_Close,menu_select);
	Main_Menu.isOpen = false;
	Animation_InTo(Screen_Change);
}

#define MIDFONT_WIDTH 16
#define MIDFONT_HEIGHT 32

bool Close(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	return false;
}


static display_t Watch_Draw(void)
{
	draw_Date();
	drawBattery();
	ticker();
	return DISPLAY_DONE;
}


static void drawBattery(void)//绘制电池
{   
	uint16_t Power_num = 0;
	char buff[16];
	Power_num = ( ADC_ValueConvertion * 0.0008056640625 - 2.7f) * 66.66666666666667;
	const uint8_t* battIcon;
	if(Power_num < 33)
		battIcon = EMP;
	else if(Power_num < 66)
		battIcon = LOW;
	else if(Power_num < 99)
		battIcon = MID;
	else
		battIcon = FULL;
	
	Draw_bmp(56,0,battIcon,16,8,0);
	sprintf(buff,"%3d",Power_num);
	Draw_Str(56,16,buff);
}



static void draw_Date(void)
{
	// Get day string
	char day[4];
	strcpy(day, days[timeDate.date.day]);

	// Get month string
	char month[BUFFSIZE_STR_MONTHS];
	strcpy(month, months[timeDate.date.month]);

	// Draw date
	char buff[BUFFSIZE_DATE_FORMAT];//星期 日 月 年
	sprintf(buff, PSTR(DATE_FORMAT), day, timeDate.date.date, month, timeDate.date.year);
	buff[BUFFSIZE_DATE_FORMAT - 1] = '\0';
	Draw_Str(0,0,buff);
}

uint8_t div10(uint8_t val)
{
	return val / 10;
}

uint8_t mod10(uint8_t val)
{
	return val % 10;
}

static display_t ticker(void)
{
	static uint8_t yPos;
	static uint8_t yPos_secs;
	static bool moving = false;
	static bool moving2[5] = {0};


	static uint8_t hour;
	static uint8_t mins;
	static uint8_t secs;

	if(timeDate.time.secs != secs)
	{
		yPos = 0;
		yPos_secs = 0;
		moving = true;

		moving2[0] = div10(timeDate.time.hour) != div10(hour);
		moving2[1] = mod10(timeDate.time.hour) != mod10(hour);
		moving2[2] = div10(timeDate.time.mins) != div10(mins);
		moving2[3] = mod10(timeDate.time.mins) != mod10(mins);
		moving2[4] = div10(timeDate.time.secs) != div10(secs);

		hour = timeDate.time.hour;
		mins = timeDate.time.mins;
		secs = timeDate.time.secs;
	}

	if(moving)
	{
		if(yPos <= 3)
			yPos++;
		else if(yPos <= 6)
			yPos += 3;
		else if(yPos <= 16)
			yPos += 5;
		else if(yPos <= 22)
			yPos += 3;
		else if(yPos <= MIDFONT_HEIGHT)
			yPos++;

		if(yPos > MIDFONT_HEIGHT)
			yPos = 255;

		if(yPos_secs <= 1)
			yPos_secs++;
		else if(yPos_secs <= 13)
			yPos_secs += 3;
		else if(yPos_secs <= 24)
			yPos_secs++;

		if(yPos_secs >= 24)
			yPos_secs = 255;

		if(yPos > MIDFONT_HEIGHT && yPos_secs > 24)
		{
			yPos = 0;
			yPos_secs = 0;
			moving = false;
			memset(moving2, false, sizeof(moving2));
		}
	}

	tickerData_t data;

	// 秒设置
	data.x = 24;
	data.y = 80;
	data.bitmap = (const uint8_t*)&Char_16X24;
	data.w = 16;
	data.h = 24;
	data.offsetY = yPos_secs;
	data.val = div10(timeDate.time.secs);
	data.maxVal = 5;
	data.moving = moving2[4];
	drawTickerNum(&data);

	data.y = 96;
	data.val = mod10(timeDate.time.secs);
	data.maxVal = 9;
	data.moving = moving;
	drawTickerNum(&data);
	
	// 设置新字体
	data.x = 16;
	data.w = MIDFONT_WIDTH;
	data.h = MIDFONT_HEIGHT;
	data.bitmap = (const uint8_t*)&Char_16X32;
	data.offsetY = yPos;

	// 分设置
	data.y = 48;
	data.val = div10(timeDate.time.mins);
	data.maxVal = 5;
	data.moving = moving2[2];
	drawTickerNum(&data);

	data.y = 64;
	data.val = mod10(timeDate.time.mins);
	data.maxVal = 9;
	data.moving = moving2[3];
	drawTickerNum(&data);

	// 时设置
	data.y = 0;
	data.val = div10(timeDate.time.hour);
	data.maxVal = 5;
	data.moving = moving2[0];
	drawTickerNum(&data);

	data.y = 16;
	data.val = mod10(timeDate.time.hour);
	data.maxVal = 9;
	data.moving = moving2[1];
	drawTickerNum(&data);
	// 冒号 1s
	if(timeDate.time.secs % 2)
	Draw_bmp(16,32,MAOHAO,16,32,0);
	else
	Draw_bmp(16,32,KONG,16,32,0);

	return (moving ? DISPLAY_BUSY : DISPLAY_DONE);
}
static void drawTickerNum(tickerData_t* data)
{
	uint8_t arraySize = (data->w * data->h) / 8;
	uint8_t yPos = data->offsetY;
	
	const uint8_t* bitmap = &data->bitmap[data->val * arraySize];
	uint8_t x = data->x;
	uint8_t y = data->y;
	
	if(!data->moving || yPos == 0 || yPos == 255)	//静态显示
		Draw_bmp(x,y,bitmap,data->w,data->h,0);
	else											//执行动画
	{
		uint8_t prev = data->val - 1;
		if(prev == 255)
			prev = data->maxVal;
		
		Draw_bmp(x,y,bitmap,data->w,data->h,yPos - data->h);
		Draw_bmp(x,y,&data->bitmap[prev * arraySize],data->w,data->h,yPos);
	}	
}

bool update;

void time_updata(void)//时间上传接口
{
	if(!update)
		return;
	update = false;
	Get_DS3231_Time();
}
