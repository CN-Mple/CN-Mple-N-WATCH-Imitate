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
//	DISPLAY_TOOFAST		//ȱ
} display_t;

typedef enum			//�˵�����
{
	MENU_TYPE_STR,
	MENU_TYPE_ICON
} menu_type_t;


//������ע����

typedef display_t	(*draw_f)(void);
typedef void 		(*display_f)(void);
typedef bool 		(*button_f)(void);
typedef void 		(*menu_f)(void);
typedef void 		(*itemLoader_f)(uint8_t);


typedef struct{				//�˵����庯��
	menu_f btn1;			//������Ӧ����1
	menu_f btn2;			//������Ӧ����2
	menu_f btn3;			//������Ӧ����3
	draw_f draw;			//��ǰ���ƺ���
	itemLoader_f loader;	//��ǰװ��Ӧ��
}menuFuncs_t;

typedef struct{				//�˵�ʵ��
	uint8_t selected;		//ѡ��
	uint8_t scroll;			//����ƫ��
	uint8_t optionCount;	//��Ŀ��
	bool isOpen;			//�˵�״̬ Open or Close
	const char* title;		//����
	menu_type_t menuType;	//�˵�����
	menuFuncs_t func;		//ʵ�庯��
	menu_f prevMenu;		//����·��
}menu_s;

typedef struct{
	uint8_t lastSelected;	//����ѡ���
	menu_f last;			//�ϼ�·��
}prev_menu_s;


typedef enum
{
	OPERATION_DRAWICON,       //��ͼ�����
	OPERATION_DRAWNAME_ICON,  //��ͼ�����ֲ���
	OPERATION_DRAWNAME_STR,   //���ַ�����
	OPERATION_ACTION    	  //ִ�в���
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

void Set_Menu_Pre(prev_menu_s *prev_menu/* ���ؾ�̬�洢����·�� */,menu_f current_prevMenu);
void Get_Menu_Pre(prev_menu_s *prev_menu/* ���ؾ�̬�洢����·�� */);


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