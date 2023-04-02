#include "ITEM_SOUND.H"


static uint8_t optionCount = 6;

static void menus_select(void);		//菜单确定选择函数
static void loader(uint8_t num);	//菜单查找执行函数

static prev_menu_s prev_menu = {NULL,0};

static void SOUND_ADD(void);
static void SOUND_0(void);
static void SOUND_1(void);
static void SOUND_2(void);
static void SOUND_3(void);


void SOUND(void)
{
	Set_Draw_Func(DRAW_MENU);
	Set_Button_Func(menu_up,menu_select,menu_down);
	
	Set_Menu_Info(optionCount,MENU_TYPE_ICON,PSTR(STR_SETTINGMENU));
	Set_Menu_Func(prevOption,menus_select,nextOption,loader);
	Set_Menu_Pre(&prev_menu,SOUND);
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
	
	Set_Menu_Option(i++, PSTR(MENU1), &BEEPER_BMP[vol][0], SOUND_ADD);
	Set_Menu_Option(i++, PSTR(MENU1), &BEEPER_BMP[0][0], SOUND_0);
	Set_Menu_Option(i++, PSTR(MENU2), &BEEPER_BMP[1][0], SOUND_1);
	Set_Menu_Option(i++, PSTR(MENU3), &BEEPER_BMP[2][0], SOUND_2);
	Set_Menu_Option(i++, PSTR(MENU3), &BEEPER_BMP[3][0], SOUND_3);
	AddExitOption();
}

static void SOUND_ADD(void)
{
	vol = (++vol > 3)?0:vol;
}

static void SOUND_0(void)
{
	vol = 0;
}
static void SOUND_1(void)
{
	vol = 1;
}
static void SOUND_2(void)
{
	vol = 2;
}
static void SOUND_3(void)
{
	vol = 3;
}
