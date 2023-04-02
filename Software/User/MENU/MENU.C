#include "MENU.H"
#include "MY_MENU.H"

menu_s Main_Menu = {0};

operation_s Menu_operation = {0};

static bool exitSelected(void);

//设置菜单主体函数
void Set_Menu_Func(menu_f btn1,menu_f btn2,menu_f btn3,itemLoader_f loader)
{
	Main_Menu.func.btn1 = btn1;
	Main_Menu.func.btn2 = btn2;
	Main_Menu.func.btn3 = btn3;
	Main_Menu.func.loader = loader;
}

//设置菜单主体信息
void Set_Menu_Info(uint8_t optionCount,menu_type_t menuType,const char* title)
{
	Main_Menu.optionCount = optionCount;
	Main_Menu.menuType = menuType;
	Main_Menu.title = title;
}

//退出检测
static bool exitSelected(void)
{
	return Main_Menu.selected == Main_Menu.optionCount - 1;
}

//保存当前菜单信息
void Set_Menu_Pre(prev_menu_s *prev_menu/* 本地静态存储返回路径 */,menu_f current_prevMenu)
{
	if(prev_menu->last == NULL)
	{
		prev_menu->last = Main_Menu.prevMenu;				//存储上级菜单
	}
		Main_Menu.selected = prev_menu->lastSelected;		//上级进入为0 下级进入恢复选项号
		Main_Menu.prevMenu = current_prevMenu;				//记录当前菜单
}
//返回上级菜单信息
void Get_Menu_Pre(prev_menu_s *prev_menu/* 本地静态存储返回路径 */)
{
	if(!exitSelected())										//进入下级菜单
	{
		prev_menu->lastSelected = Main_Menu.selected;		//存储当前选项
	}
	else													//返回上级菜单
	{
		Main_Menu.prevMenu = prev_menu->last;				//恢复返回路径
		prev_menu->lastSelected = 0;						//清除当前选项号
	}
}

//下一项
void nextOption(void)
{
	Main_Menu.selected++;
	if(Main_Menu.selected >= Main_Menu.optionCount)
		Main_Menu.selected = 0;
	checkScroll();
}
//上一项
void prevOption(void)
{
	Main_Menu.selected--;
	if(Main_Menu.selected >= Main_Menu.optionCount)
		Main_Menu.selected = Main_Menu.optionCount - 1;
	checkScroll();
}

//检查设置滚动的行数scroll
void checkScroll(void)
{
	uint8_t scroll = Main_Menu.scroll;
	if(Main_Menu.selected >= scroll + MAX_MENU_ITEMS)
		scroll = (Main_Menu.selected + 1) - MAX_MENU_ITEMS;
	else if(Main_Menu.selected < scroll)
		scroll = Main_Menu.selected;
	Main_Menu.scroll = scroll;
}


//执行按键
static void doBtn(menu_f btn)
{
	if( (!animation_active() || !animation_movingOn()) && btn != NULL)
		btn();
}

//主菜单选择界面
bool menu_select(void)
{
	if(!animation_active() || animation_movingOn())
	{
		//第一次进入主菜单函数，执行mMainOpen();打开主菜单
		if(!Main_Menu.isOpen)
		{
			Main_Menu.isOpen = true;
			
			Animation_Leave(main_menu);
		}
		else if(Main_Menu.func.btn2 != NULL)  //打开后再次按下确认功能
		if(Main_Menu.func.btn2 != NULL)  //打开后再次按下确认功能
		{
			Main_Menu.func.btn2();

		}
	}
	return true;
}
//执行菜单按键
bool menu_down(void)
{
	doBtn(Main_Menu.func.btn3);
	return true;
}
//执行菜单按键
bool menu_up(void)
{
	doBtn(Main_Menu.func.btn1);
	return true;
}




//单步选项执行函数
bool Menu_Option(uint8_t num, const char* name, const uint8_t* icon, menu_f actionFunc)
{
	if( num != Menu_operation.id )													//非目标选项退出
	{
		return 0;
	}
	switch(Menu_operation.op)
	{
		case OPERATION_DRAWICON:Draw_icon(16,Menu_operation.data - 32,icon);break;	//绘制项目图标
		
		case OPERATION_DRAWNAME_ICON:Draw_Str(56,0,name);break;						//绘制项目名称
		
		case OPERATION_DRAWNAME_STR:Draw_Str(Menu_operation.data,8,name);break;		//绘制项目字符
		
		case OPERATION_ACTION:														//绘制动画执行
			if(actionFunc != NULL)
				Menu_operation.data ? Animation_Leave(actionFunc): actionFunc();
			break;
		default:
			break;
	}
	return 1;
}

//设置选项执行函数
void Set_Menu_Option(uint8_t num, const char* name, const uint8_t* icon, menu_f actionFunc)
{
	if(num != Menu_operation.id)
		return;

	char buff[BUFFSIZE_STR_MENU];
	strcpy(buff, name);
	Menu_Option(num, buff, icon, actionFunc);
}
//添加退出选项
void AddExitOption(void)
{
	Set_Menu_Option(Main_Menu.optionCount - 1, "Exit", &MAIN_MUNE[5][0], Exit);
}


//返回上级菜单，上级只有打开动画过度
void Exit(void)
{
	Main_Menu.prevMenu != NULL ? Animation_Leave( Main_Menu.prevMenu): Main_Menu.prevMenu()/*mMainOpen()*/;
}


static void loader(operation_t op, uint8_t num, uint8_t data)
{
	Menu_operation.op = op;  				//选项类型
	Menu_operation.id = num;  				//选项号
	Menu_operation.data = data;  			//anim，是否动画

	if(Main_Menu.func.loader != NULL)
		Main_Menu.func.loader(num);  		//指向itemLoader函数
}
//执行函数后切换菜单
void DoAction(bool anim)
{
	loader(OPERATION_ACTION, Main_Menu.selected, anim);
}

//绘制菜单标题
static void MenuDraw_Title(void)
{
	char buff[BUFFSIZE_STR_MENU];
	memset(buff,' ', sizeof(buff));
	strcpy((buff + (8 - (strlen(Main_Menu.title) / 2))),Main_Menu.title);
	Draw_Str(0,0,buff);
}
//绘制菜单框选
static void MenuDraw_Button(void)
{
	Draw_bmp(15,44,UP,40,5,0);
	Draw_bmp(44,44,DOWN,40,5,0);
}

//绘制菜单图标
void MenuDraw_Icon(void)
{
	uint8_t i,speed;
	static int Anim_y = 48;
	
	int y = 48 - Main_Menu.selected * 48;		
	
	if(y > Anim_y)
	{
		speed = ((y - Anim_y) / 4) + 1;
		if(speed > 16)
			speed = 16;
		Anim_y += speed;
		if(y <= Anim_y)
			Anim_y = y;
	}
	else if(y < Anim_y)
	{
		speed = ((Anim_y - y) / 4) + 1;
		if(speed > 16)
			speed = 16;
		Anim_y -= speed;
		if(Anim_y <= y)
			Anim_y = y;
	}
	
	y = Anim_y;
	for(i = 0;i < Main_Menu.optionCount;i++)
	{
		if( ( y > -32 ) && ( y < 128 ) )
		loader(OPERATION_DRAWICON, i, y + 32);
		y += 48;
	}
	
	MenuDraw_Button();
	MenuDraw_Title();
	loader(OPERATION_DRAWNAME_ICON, Main_Menu.selected, 0);
}

//绘制菜单字符
void MenuDraw_Str(void)
{
	MenuDraw_Title();

	uint8_t scroll = Main_Menu.scroll;
	uint8_t count = ((Main_Menu.optionCount > MAX_MENU_ITEMS ) ? MAX_MENU_ITEMS : Main_Menu.optionCount) + scroll;
	for(uint8_t i=scroll;i<count;i++)  //从滚动的行数scroll到count遍历7行要显示的字符串
	{
		uint8_t y = 8 + (8 * (i - scroll));
		if(i == Main_Menu.selected)
			Draw_Str(y,0,">");
		loader(OPERATION_DRAWNAME_STR, i, y);
	}
}

display_t DRAW_MENU(void)
{
	if(Main_Menu.menuType == MENU_TYPE_ICON)
	{
		MenuDraw_Icon();
	}
	else if(Main_Menu.menuType == MENU_TYPE_STR)
	{
		MenuDraw_Str();
	}
	return DISPLAY_BUSY;
}
