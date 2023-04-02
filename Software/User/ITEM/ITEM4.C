#include "ITEM4.H"

static void Exit_Menu(void);//·µ»Ø²Ëµ¥

static void Draw_Tick(void);
static void Rerun(void);
static void Stop(void);

static prev_menu_s prev_menu;

uint8_t Time_tick_flag = 0;
uint32_t Time_tick = 0;

void ITEM4_menu(void)
{
	Set_Draw_Func((draw_f)Draw_Tick);
	Set_Button_Func((button_f)Rerun,(button_f) Exit_Menu,(button_f)Stop);
	
	Set_Menu_Pre(&prev_menu,ITEM4_menu);
	
	Animation_InTo(NULL);
}

static void Rerun(void)
{
	Time_tick_flag = 0;
	Time_tick = 0;
}

static void Stop(void)
{
	Time_tick_flag ^= 1;
}

static uint8_t  W[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static uint8_t  H[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	
static void Draw_W_Square(uint8_t x,uint8_t y)
{
	Draw_bmp(x,y,W,128,1,0);
}
static void Draw_H_Square(uint8_t x,uint8_t y)
{
	Draw_bmp(x,y,H,1,32,0);
}

static void Draw_Square(uint8_t x,uint8_t y)
{
	Draw_W_Square(x,y);
	Draw_W_Square(x + 32,y);
	Draw_H_Square(x,y);
	Draw_H_Square(x,y + 127);
}

extern timeDate_s timeDate;

static void Draw_Tick(void)
{
	char buff[16];
	sprintf(buff,"%2d:%2d:%2d",timeDate.time.hour,timeDate.time.mins,timeDate.time.secs);
	Draw_Str(0,32,buff);
	
	Draw_bmp(16, 0,(const uint8_t*)&Char_16X32[Time_tick/6000%60/10][0],16,32,0);
	Draw_bmp(16,16,(const uint8_t*)&Char_16X32[Time_tick/6000%60%10][0],16,32,0);
	
	Draw_bmp(16,32,MAOHAO,16,32,0);
	
	Draw_bmp(16,48,(const uint8_t*)&Char_16X32[Time_tick/100%60/10][0],16,32,0);
	Draw_bmp(16,64,(const uint8_t*)&Char_16X32[Time_tick/100%60%10][0],16,32,0);
	
	Draw_bmp(24,80,(const uint8_t*)&Char_16X24[Time_tick%100/10][0],16,24,0);
	Draw_bmp(24,96,(const uint8_t*)&Char_16X24[Time_tick%100%10][0],16,24,0);
	
	Draw_Square(16,0);
}
static void Exit_Menu(void)
{
	displayauto = 0;

	Main_Menu.prevMenu = (&prev_menu)->last;
	(&prev_menu)->lastSelected = 0;
	Exit();
}
