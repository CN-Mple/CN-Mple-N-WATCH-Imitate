#include "ITEM3.H"


static uint8_t optionCount = 5;

static void menus_select(void);		//菜单确定选择函数
static void loader(uint8_t num);	//菜单查找执行函数

static prev_menu_s prev_menu = {NULL,0};

static void SUN_SET(void);
static void INVERT_SET(void);
static void ROTATE_SET(void);
static void ANIMATIONS_SET(void);

void ITEM3_menu(void)
{
	Set_Draw_Func(DRAW_MENU);
	Set_Button_Func(menu_up,menu_select,menu_down);
	
	Set_Menu_Info(optionCount,MENU_TYPE_ICON,PSTR(STR_SETTINGMENU));
	Set_Menu_Func(prevOption,menus_select,nextOption,loader);
	Set_Menu_Pre(&prev_menu,ITEM3_menu);
	checkScroll();
	Animation_InTo(NULL);
}

static void menus_select(void)
{
	Get_Menu_Pre(&prev_menu);
	DoAction(false);
}

static void loader(uint8_t num)
{
	uint8_t i = 0;
	
	Set_Menu_Option(i++, PSTR(MENU1), &DISPLAY_MUNE[0][0], SUN_SET);
	Set_Menu_Option(i++, PSTR(MENU2), &DISPLAY_MUNE[1][0], INVERT_SET);
	Set_Menu_Option(i++, PSTR(MENU3), &DISPLAY_MUNE[2][0], ROTATE_SET);
	Set_Menu_Option(i++, PSTR(MENU4), &DISPLAY_MUNE[3][0], ANIMATIONS_SET);
	AddExitOption();
}


/*应用函数*/
static void SUN_SET(void)
{
	static uint8_t i;

	OLED_CMD(0x81);
	switch(i++%3)
	{
		case 0:OLED_CMD(0xA0);break;
		case 1:OLED_CMD(0x0F);break;
		case 2:OLED_CMD(0xDF);break;
		
	}
}

static void INVERT_SET(void)
{
	static uint8_t i;

	if(++i%2)
	{
		OLED_CMD(0xa1);
	}
	else
	{
		OLED_CMD(0xa0);
	}
}

static void ROTATE_SET(void)
{
	static uint8_t i;

	if(++i%2)
	{
		OLED_CMD(0xc8);
		OLED_CMD(0xa1);
	}
	else
	{
		OLED_CMD(0xc0);
		OLED_CMD(0xa0);
	}
}

static void ANIMATIONS_SET(void)
{
	static uint8_t i;

	if(++i%2)
	{
		MOD = 1;
	}
	else
	{
		MOD = 0;
	} 
}
