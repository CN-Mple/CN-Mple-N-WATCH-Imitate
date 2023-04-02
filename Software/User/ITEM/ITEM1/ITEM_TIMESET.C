#include "ITEM_TIMESET.H"

static void Exit_Menu(void);//返回菜单
static prev_menu_s prev_menu;

static void TIME_SET(void);

static void Time_Set_Screen();
static void UI_Draw_square(uint8_t length,uint8_t weighth);

static void Trun_True(void);
static void Trun_Up(void);

static timeDate_s timeDate_Set = {{30,20,15},{DAY_TUE,14,MONTH_FEB,23}};

void Sys_Time_Set(void)
{
	Set_Draw_Func((draw_f)Time_Set_Screen);
	Set_Button_Func((button_f)TIME_SET,(button_f) Exit_Menu,(button_f)Trun_True);
	
	Set_Menu_Pre(&prev_menu,Sys_Time_Set);
	
	Animation_InTo(NULL);
}

static void Exit_Menu(void)
{
	displayauto = 0;

	Main_Menu.prevMenu = (&prev_menu)->last;
	(&prev_menu)->lastSelected = 0;
	Exit();
}


static void TIME_SET(void)
{
	uint8_t temp = 0;

    temp = HEXtoBCD(timeDate_Set.date.year);
	DS3231_WR_Byte(0x06,temp);
	
	temp = HEXtoBCD(timeDate_Set.date.month);
	DS3231_WR_Byte(0x05,temp);
	
	temp = HEXtoBCD(timeDate_Set.date.date);
	DS3231_WR_Byte(0x04,temp);
	
	temp = HEXtoBCD(timeDate_Set.time.hour);
	DS3231_WR_Byte(0x02,temp);
	
	temp = HEXtoBCD(timeDate_Set.time.mins);
	DS3231_WR_Byte(0x01,temp);
	
	temp = HEXtoBCD(timeDate_Set.time.secs);
	DS3231_WR_Byte(0x00,temp);
	
	temp = HEXtoBCD(timeDate_Set.date.day);
	DS3231_WR_Byte(0x03,temp);
}

static void Time_Set_Screen()
{
	char buf[10];
	
	sprintf(buf,"<TIME__SET>");
	Draw_Str(0,20,buf);
	
	sprintf(buf,"year:20%2d",timeDate_Set.date.year);
	Draw_Str(8, 0,buf);
	sprintf(buf,"mon :  %2d",timeDate_Set.date.month);
	Draw_Str(16, 0,buf);
	sprintf(buf,"mday:  %2d",timeDate_Set.date.date);
	Draw_Str(24, 0,buf);
	sprintf(buf,"wday:  %2d",timeDate_Set.date.day);
	Draw_Str(32, 0,buf);
	sprintf(buf,"hour:  %2d",timeDate_Set.time.hour);
	Draw_Str(40, 0,buf);
	sprintf(buf,"min :  %2d",timeDate_Set.time.mins);
	Draw_Str(48, 0,buf);
	sprintf(buf,"sec :  %2d",timeDate_Set.time.secs);
	Draw_Str(56, 0,buf);
	
	UI_Draw_square(72,8);
	Trun_Up();
}

static uint8_t x_dex = 8;
static uint8_t x_target = 8;
static uint8_t y_dex = 0;
static uint8_t y_target = 0;

static void Trun_True(void)
{
	x_target = (x_target + 8) % 64;
	if(x_target < 8)
		x_target = 8;
}

extern float Pitch;extern float roll;extern float yaw;

static void Trun_Up(void)
{
	static uint8_t i = 0,count = 0;
	if(roll  < -50 || roll > 50)
	{
		i = 0;
		return;
	}
		
	switch(i)
	{
		case 0:if(Pitch < -15)i = 1;break;
		case 1:if(Pitch > 28){if(count++ == 3){count = 0;i = 2;}else{i = 1;}};break;
		default:break;
	}
	if(i == 2)
	{
		switch(x_target / 8)
		{
			case 1:timeDate_Set.date.year = ( timeDate_Set.date.year + 1 ) % 100;			  break;
			case 2:timeDate_Set.date.month= (month_t)( ( timeDate_Set.date.month + 1 ) % 12 );break;
			case 3:timeDate_Set.date.date = ( timeDate_Set.date.date + 1 ) % 31;			  break;
			case 4:timeDate_Set.date.day  = (day_t)( ( timeDate_Set.date.day + 1 ) % 7 );	  break;
			case 5:timeDate_Set.time.hour = ( timeDate_Set.time.hour + 1 ) % 24;break;
			case 6:timeDate_Set.time.mins = ( timeDate_Set.time.mins + 1 ) % 60;break;
			case 7:timeDate_Set.time.secs = ( timeDate_Set.time.secs + 1 ) % 60;break;
			default:break;
		}
		i = 0;
	}
}


static uint8_t  W[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static uint8_t  H[] = {0xFF};
static uint8_t _W[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static uint8_t _H[] = {0x00};
	
static void Draw_W_Square(uint8_t x,uint8_t y)
{
	Draw_bmp(x,y,W,72,1,0);
}
static void Draw_H_Square(uint8_t x,uint8_t y)
{
	Draw_bmp(x,y,H,1,8,0);
}
static void Clr_W_Square(uint8_t x,uint8_t y)
{
	Draw_bmp(x,y,_W,72,1,0);
}
static void Clr_H_Square(uint8_t x,uint8_t y)
{
	Draw_bmp(x,y,_H,1,8,0);
}

static void Draw_Square(uint8_t x,uint8_t y)
{
	Draw_W_Square(x,y);
	Draw_W_Square(x + 8,y);
	Draw_H_Square(x,y);
	Draw_H_Square(x,y + 72);
}
static void Clr_Square(uint8_t x,uint8_t y)
{
	Clr_W_Square(x,y);
	Clr_W_Square(x + 8,y);
	Clr_H_Square(x,y);
	Clr_H_Square(x,y + 72);
}

static void UI_Draw_square(uint8_t length,uint8_t weighth)
{
	if(x_dex != x_target || y_dex != y_target)		//动态
	{
		Clr_Square(x_dex,y_dex);	//消除
		if(x_dex < x_target)
		{
			x_dex++;
		}
		else if(x_dex > x_target)
		{
			x_dex--;
		}
		if(y_dex < y_target)
		{
			y_dex++;
		}
		else if(y_dex > y_target)
		{
			y_dex--;
		}
		Draw_Square(x_dex,y_dex);	//重构
	}
	else											//静态
	{
		Draw_Square(x_dex,y_dex);
	}
}
