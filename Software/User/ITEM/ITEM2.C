#include "ITEM2.H"


static uint8_t optionCount = 9;

static void menus_select(void);		//菜单确定选择函数
static void loader(uint8_t num);	//菜单查找执行函数

static prev_menu_s prev_menu = {NULL,0};

void ITEM2_menu(void)
{
	Set_Draw_Func(DRAW_MENU);
	Set_Button_Func(menu_up,menu_select,menu_down);
	
	Set_Menu_Info(optionCount,MENU_TYPE_STR,PSTR(STR_SETTINGMENU));
	Set_Menu_Func(prevOption,menus_select,nextOption,loader);
	Set_Menu_Pre(&prev_menu,ITEM2_menu);
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
	
	Set_Menu_Option(i++, PSTR(GAME1), &SETTING_MUNE[0][0], GAME_LIFE);
	Set_Menu_Option(i++, PSTR(GAME2), &SETTING_MUNE[1][0], GAME_Flybrid);
	Set_Menu_Option(i++, PSTR(GAME3), &SETTING_MUNE[0][0], NULL);
	Set_Menu_Option(i++, PSTR(GAME4), &SETTING_MUNE[1][0], NULL);
	Set_Menu_Option(i++, PSTR(GAME5), &SETTING_MUNE[0][0], NULL);
	Set_Menu_Option(i++, PSTR(GAME6), &SETTING_MUNE[1][0], NULL);
	Set_Menu_Option(i++, PSTR(GAME7), &SETTING_MUNE[0][0], NULL);
	Set_Menu_Option(i++, PSTR(GAME8), &SETTING_MUNE[1][0], NULL);
	AddExitOption();
}

