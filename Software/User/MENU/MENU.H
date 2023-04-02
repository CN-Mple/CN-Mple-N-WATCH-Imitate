#ifndef __MUENU_H
#define __MUENU_H

#include "stm32f10x.h"

#include "MENU_TITLE.H"

#include "bsp_oled.h"


#include "string.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>



#define MAX_MENU_ITEMS		7
#define BUFFSIZE_STR_MENU	16

typedef enum
{
	DISPLAY_DONE,
	DISPLAY_BUSY,
//	DISPLAY_TOOFAST		//缺
} display_t;

typedef enum			//菜单类型
{
	MENU_TYPE_STR,
	MENU_TYPE_ICON
} menu_type_t;


//函数标注声明

typedef display_t	(*draw_f)(void);
typedef void 		(*display_f)(void);
typedef bool 		(*button_f)(void);
typedef void 		(*menu_f)(void);
typedef void 		(*itemLoader_f)(uint8_t);


typedef struct{				//菜单主体函数
	menu_f btn1;			//按键对应函数1
	menu_f btn2;			//按键对应函数2
	menu_f btn3;			//按键对应函数3
	draw_f draw;			//当前绘制函数
	itemLoader_f loader;	//当前装载应用
}menuFuncs_t;

typedef struct{				//菜单实体
	uint8_t selected;		//选项
	uint8_t scroll;			//滚动偏移
	uint8_t optionCount;	//项目数
	bool isOpen;			//菜单状态 Open or Close
	const char* title;		//标题
	menu_type_t menuType;	//菜单类型
	menuFuncs_t func;		//实体函数
	menu_f prevMenu;		//返回路径
}menu_s;

typedef struct{
	uint8_t lastSelected;	//本级选项号
	menu_f last;			//上级路径
}prev_menu_s;


typedef enum
{
	OPERATION_DRAWICON,       //画图标操作
	OPERATION_DRAWNAME_ICON,  //画图标名字操作
	OPERATION_DRAWNAME_STR,   //画字符操作
	OPERATION_ACTION    	  //执行操作
} operation_t;

typedef struct
{
	uint8_t data;
	operation_t op;
	uint8_t id;
}operation_s;


extern menu_s Main_Menu;



void Set_Menu_Func(menu_f btn1,menu_f btn2,menu_f btn3,itemLoader_f loader);
void Set_Menu_Info(uint8_t optionCount,menu_type_t menuType,const char* title);

bool exitSelected(void);

void Set_Menu_Pre(prev_menu_s *prev_menu/* 本地静态存储返回路径 */,menu_f current_prevMenu);
void Get_Menu_Pre(prev_menu_s *prev_menu/* 本地静态存储返回路径 */);


void checkScroll(void);
void nextOption(void);
void prevOption(void);

bool menu_select(void);
bool menu_down(void);
bool menu_up(void);
	
bool Menu_Option(uint8_t num, const char* name, const uint8_t* icon, menu_f actionFunc);
void Set_Menu_Option(uint8_t num, const char* name, const uint8_t* icon, menu_f actionFunc);


void AddExitOption(void);
void Exit(void);

void DoAction(bool anim);

void MenuDraw_Icon(void);
void MenuDraw_Str(void);
display_t DRAW_MENU(void);


#endif /* __MUENU_H */
