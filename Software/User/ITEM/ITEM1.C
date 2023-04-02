#include "ITEM1.H"


static uint8_t optionCount = 5;

static void menus_select(void);		//菜单确定选择函数
static void loader(uint8_t num);	//菜单查找执行函数

static prev_menu_s prev_menu = {NULL,0};


void ITEM1_menu(void)
{
	Set_Draw_Func(DRAW_MENU);
	Set_Button_Func(menu_up,menu_select,menu_down);
	
	Set_Menu_Info(optionCount,MENU_TYPE_ICON,PSTR(STR_SETTINGMENU));
	Set_Menu_Func(prevOption,menus_select,nextOption,loader);
	Set_Menu_Pre(&prev_menu,ITEM1_menu);
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
	
	Set_Menu_Option(i++, PSTR(TXTS), &SETTING_MUNE[0][0], NULL);
	Set_Menu_Option(i++, PSTR(TIMES), &SETTING_MUNE[1][0], Sys_Time_Set);
	Set_Menu_Option(i++, PSTR(SOUNDS), &SETTING_MUNE[2][0], SOUND);
	Set_Menu_Option(i++, PSTR(LIGHTS), &SETTING_MUNE[3][0], LIGHT);
	AddExitOption();
}
