#ifndef __BREATHSCREEN_H
#define __BREATHSCREEN_H

#include "stm32f10x.h"

#include "MENU.H"
#include <stdbool.h>
#include "bsp_oled.h"

typedef struct User_Screens
{
	bool Screen_stat;
	bool active;
}User_Screen_t;


typedef struct User
{
	User_Screen_t UserScreen;
}User_t;

void User_screen(void);

void Screen_Open(void);

void Screen_Close(void);

void Screen_Change(void);


extern User_t User;

#endif /* __BREATHSCREEN_H */
