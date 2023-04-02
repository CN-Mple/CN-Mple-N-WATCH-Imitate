#include "ITEM_LIGHT.H"

static void Exit_Menu(void);//·µ»Ø²Ëµ¥

static void light(void);

static prev_menu_s prev_menu;


void LIGHT(void)
{
	Set_Draw_Func((draw_f)light);
	Set_Button_Func((button_f)NULL,(button_f) Exit_Menu,(button_f)NULL);
	
	Set_Menu_Pre(&prev_menu,LIGHT);
	
	Animation_InTo(NULL);
}

static void light(void)
{
	memset(Grame, 0xFF, HEIGHTH_MAX * WHITH_MAX / 8);
}

static void Exit_Menu(void)
{
	displayauto = 0;

	Main_Menu.prevMenu = (&prev_menu)->last;
	(&prev_menu)->lastSelected = 0;
	Exit();
}
